#ifndef MESH_H_
#define MESH_H_

#include <vector>
#include <unordered_map>
#include <string>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include "graphics/IMesh.hpp"
#include "graphics/ISkeleton.hpp"

#include "logger/ILogger.hpp"
#include "fs/IFileSystem.hpp"

#include "exceptions/RuntimeException.hpp"

#include "Types.hpp"

struct aiMesh;

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
	std::unordered_map< std::string, uint32 > boneIndexMap;
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

	void import(const std::string& name, const std::string& filename, const uint32 index, const aiMesh* mesh, const BoneData& boneData, const uint32 numberOfVertices, logger::ILogger* logger, fs::IFileSystem* fileSystem);
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

	void importBones(const std::string& name, const std::string& filename, uint32 index, const aiMesh* mesh, logger::ILogger* logger, fs::IFileSystem* fileSystem);

	void import(const std::string& name, const std::string& filename, const uint32 index, const aiMesh* mesh, logger::ILogger* logger, fs::IFileSystem* fileSystem);
};

}

#endif /* MESH_H_ */
