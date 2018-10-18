#ifndef MESH_H_
#define MESH_H_

#include <vector>
#include <map>
#include <string>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include <assimp/mesh.h>

#include "graphics/IMesh.hpp"
#include "graphics/ISkeleton.hpp"

#include "logger/ILogger.hpp"
#include "fs/IFileSystem.hpp"

#include "exceptions/RuntimeException.hpp"

#include "detail/AssImpUtilities.hpp"

#include "Types.hpp"

namespace ice_engine
{

struct Bone
{
	std::string name;
	glm::mat4 boneOffset;
};

struct BoneData
{
	std::string name;
	std::map< std::string, uint32 > boneIndexMap;
	std::vector< Bone > boneTransform;
};

class VertexBoneData : public graphics::ISkeleton
{
public:

	VertexBoneData() = default;

	VertexBoneData(
		std::vector<glm::ivec4> boneIds,
		std::vector<glm::vec4> boneWeights
	)
	:
		boneIds_(std::move(boneIds)),
		boneWeights_(std::move(boneWeights))
	{
	}

	VertexBoneData(const std::string& name, const std::string& filename, const uint32 index, const aiMesh* mesh, const BoneData& boneData, const uint32 numberOfVertices, logger::ILogger* logger, fs::IFileSystem* fileSystem)
	{
		import(name, filename, index, mesh, boneData, numberOfVertices, logger, fileSystem);
	}

	VertexBoneData(const VertexBoneData&) = default;
	VertexBoneData(VertexBoneData&&) = default;

	~VertexBoneData() override = default;

	VertexBoneData& operator=(const VertexBoneData&) = default;
	VertexBoneData& operator=(VertexBoneData&&) = default;

	const std::string& name() const override
	{
		return name_;
	}

	const std::vector<glm::ivec4>& boneIds() const override
	{
		return boneIds_;
	}

	const std::vector<glm::vec4>& boneWeights() const override
	{
		return boneWeights_;
	}

private:
	std::string name_;
	std::vector<glm::ivec4> boneIds_;
	std::vector<glm::vec4> boneWeights_;

	void import(const std::string& name, const std::string& filename, const uint32 index, const aiMesh* mesh, const BoneData& boneData, const uint32 numberOfVertices, logger::ILogger* logger, fs::IFileSystem* fileSystem)
	{
		boneIds_.resize(numberOfVertices);
		boneWeights_.resize(numberOfVertices);

		// Load bone data
		for (uint32 i = 0; i < mesh->mNumBones; i++)
		{
			auto it = boneData.boneIndexMap.find(mesh->mBones[i]->mName.C_Str());

			if (it == boneData.boneIndexMap.end())
			{
				continue;
			}

			uint32 boneIndex = it->second;

			for (uint32 j = 0; j < mesh->mBones[i]->mNumWeights; j++)
			{
				const uint32 vertexID = mesh->mBones[i]->mWeights[j].mVertexId;
				const float32 weight = mesh->mBones[i]->mWeights[j].mWeight;

				for (uint32 i = 0; i < 4; i++)
				{
					if (boneWeights_[vertexID][i] == 0.0f)
					{
						boneIds_[vertexID][i] = boneIndex;
						boneWeights_[vertexID][i] = weight;

						assert( boneWeights_[vertexID][0] + boneWeights_[vertexID][1] + boneWeights_[vertexID][2] + boneWeights_[vertexID][3] >= 1.01f );

						break;
					}
				}
			}
		}
	}
};

class Mesh : public graphics::IMesh
{
public:

	Mesh() = default;
	
	Mesh(
		std::string name,
		std::vector< glm::vec3 > vertices,
		std::vector< uint32 > indices,
		std::vector< glm::vec4 > colors,
		std::vector< glm::vec3 > normals,
		std::vector< glm::vec2 > textureCoordinates,
		VertexBoneData vertexBoneData = VertexBoneData(),
		BoneData boneData = BoneData()
	)
	:
		name_(std::move(name)),
		vertices_(std::move(vertices)),
		indices_(std::move(indices)),
		colors_(std::move(colors)),
		normals_(std::move(normals)),
		textureCoordinates_(std::move(textureCoordinates)),
		vertexBoneData_(std::move(vertexBoneData)),
		boneData_(std::move(boneData))
	{
	}

	Mesh(const std::string& name, const std::string& filename, const uint32 index, const aiMesh* mesh, logger::ILogger* logger, fs::IFileSystem* fileSystem)
	{
		import(name, filename, index, mesh, logger, fileSystem);
	}

	~Mesh() override = default;

	const std::string& name() const override
	{
		return name_;
	}

	const VertexBoneData& vertexBoneData() const
	{
		return vertexBoneData_;
	}

	const BoneData& boneData() const
	{
		return boneData_;
	}

	const std::vector<glm::vec3>& vertices() const override
	{
		return vertices_;
	}

	const std::vector<uint32>& indices() const override
	{
		return indices_;
	}

	const std::vector<glm::vec4>& colors() const override
	{
		return colors_;
	}

	const std::vector<glm::vec3>& normals() const override
	{
		return normals_;
	}

	const std::vector< glm::vec2 >& textureCoordinates() const override
	{
		return textureCoordinates_;
	}

private:
	std::string name_;
	std::vector< glm::vec3 > vertices_;
	std::vector< uint32 > indices_;
	std::vector< glm::vec4 > colors_;
	std::vector< glm::vec3 > normals_;
	std::vector< glm::vec2 > textureCoordinates_;
	VertexBoneData vertexBoneData_;
	BoneData boneData_;

	void importBones(const std::string& name, const std::string& filename, uint32 index, const aiMesh* mesh, logger::ILogger* logger, fs::IFileSystem* fileSystem)
	{
		assert( mesh != nullptr );

		boneData_ = BoneData();

		logger->debug( "importing boneData." );

		for (uint32 i = 0; i < mesh->mNumBones; i++) {
			Bone data = Bone();
			uint32 boneIndex = 0;

			// Set the bone name
			if (mesh->mBones[i]->mName.length > 0)
			{
				data.name = std::string( mesh->mBones[i]->mName.C_Str() );
			}
			else
			{
				data.name = std::string( name ) + "_bone_" + std::to_string(index);
			}

			logger->debug( "bone name: " + data.name );

			if (boneData_.boneIndexMap.find(data.name) == boneData_.boneIndexMap.end())
			{
				boneIndex = boneData_.boneIndexMap.size();
				boneData_.boneTransform.push_back(data);
			}
			else
			{
				boneIndex = boneData_.boneIndexMap[data.name];
			}

			boneData_.boneIndexMap[data.name] = boneIndex;
			boneData_.boneTransform[boneIndex].boneOffset = detail::convertAssImpMatrix( &(mesh->mBones[i]->mOffsetMatrix) );
		}

		logger->debug( "done importing boneData." );
	}

	void import(const std::string& name, const std::string& filename, const uint32 index, const aiMesh* mesh, logger::ILogger* logger, fs::IFileSystem* fileSystem)
	{
		logger->debug( "importing mesh '" + filename + "'." );

		importBones( name, filename, index, mesh, logger, fileSystem );

		// Set the mesh name
		if (mesh->mName.length > 0)
		{
			name_ = std::string( mesh->mName.C_Str() );
		}
		else
		{
			name_ = name + "_mesh_" + std::to_string(index);
		}

		logger->debug( "mesh name: " + name_ );

		if (mesh->mNormals == 0)
		{
			logger->warn( "Unable to import mesh - it does not have any normals." );
			return;
		}

		// Load vertices, indices, normals, texture coordinates, and colors
		vertices_.resize(mesh->mNumVertices);
		normals_.resize(mesh->mNumVertices);
		textureCoordinates_.resize(mesh->mNumVertices);
		colors_.resize(mesh->mNumVertices);

		for ( uint32 i = 0; i < mesh->mNumVertices; i++ )
		{
			vertices_[i] = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
			normals_[i] = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);

			if ( mesh->HasTextureCoords(0))
			{
				textureCoordinates_[i] = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
			}

			if ( mesh->mColors[0] != 0 )
			{
				colors_[i] = glm::vec4(
					(float)mesh->mColors[0][i].a,
					(float)mesh->mColors[0][i].b,
					(float)mesh->mColors[0][i].g,
					(float)mesh->mColors[0][i].r
					);
			}
		}

		indices_.reserve(mesh->mNumVertices);

		for ( uint32 i = 0; i < mesh->mNumFaces; i++ )
		{
			const aiFace& face = mesh->mFaces[i];

			if ( face.mNumIndices != 3 )
			{
				const auto msg = std::string("Unable to import model...Unsupported number of indices per face (") + std::to_string(face.mNumIndices) + std::string(").");
				throw RuntimeException(msg);
			}

			indices_.push_back(face.mIndices[0]);
			indices_.push_back(face.mIndices[1]);
			indices_.push_back(face.mIndices[2]);
		}

		vertexBoneData_ = VertexBoneData();

		// If we have any bones, load them
		if (mesh->mNumBones > 0)
		{
			vertexBoneData_ = VertexBoneData( name, filename, index, mesh, boneData_, vertices_.size(), logger, fileSystem );
		}

		//std::cout << "Load results: " << mesh->mNumVertices << " " << mesh->mNumBones << " " << temp << " " << data.bones.size() << std::endl;

		logger->debug( "done importing mesh '" + filename + "'." );
	}
};

}

#endif /* MESH_H_ */
