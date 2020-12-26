#include <assimp/mesh.h>

#include "Mesh.hpp"

#include "detail/AssImpUtilities.hpp"

namespace ice_engine
{

void VertexBoneData::import(const std::string& name, const std::string& filename, const uint32 index, const aiMesh* mesh, const BoneData& boneData, const uint32 numberOfVertices, logger::ILogger* logger, fs::IFileSystem* fileSystem)
{
    boneIds_.resize(numberOfVertices);
    boneWeights_.resize(numberOfVertices);

    // Load bone data
    for (uint32 i = 0; i < mesh->mNumBones; i++)
    {
        auto it = boneData.boneIndexMap.find(mesh->mBones[i]->mName.C_Str());

        if (it == boneData.boneIndexMap.end()) continue;

        uint32 boneIndex = it->second;

        for (uint32 j = 0; j < mesh->mBones[i]->mNumWeights; ++j)
        {
            const uint32 vertexID = mesh->mBones[i]->mWeights[j].mVertexId;
            const float32 weight = mesh->mBones[i]->mWeights[j].mWeight;

            for (uint32 i = 0; i < 4; ++i)
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

void Mesh::importBones(const std::string& name, const std::string& filename, uint32 index, const aiMesh* mesh, logger::ILogger* logger, fs::IFileSystem* fileSystem)
{
    assert(mesh != nullptr);

    boneData_ = BoneData();

    LOG_DEBUG(logger, "Importing boneData for mesh '%s' for model '%s'." , filename, name);

    for (uint32 i = 0; i < mesh->mNumBones; ++i) {
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

        LOG_DEBUG(logger, "Found bone with name '%s' for model '%s'." , data.name, name);

        if (boneData_.boneIndexMap.find(data.name) == boneData_.boneIndexMap.end())
        {
            boneIndex = static_cast<uint32>(boneData_.boneIndexMap.size());
            boneData_.boneTransform.push_back(data);
        }
        else
        {
            boneIndex = boneData_.boneIndexMap[data.name];
        }

        boneData_.boneIndexMap[data.name] = boneIndex;
        boneData_.boneTransform[boneIndex].boneOffset = detail::convertAssImpMatrix( &(mesh->mBones[i]->mOffsetMatrix) );
    }

    LOG_DEBUG(logger, "Done importing boneData for mesh '%s' for model '%s'." , filename, name);
}

void Mesh::import(const std::string& name, const std::string& filename, const uint32 index, const aiMesh* mesh, logger::ILogger* logger, fs::IFileSystem* fileSystem)
{
    LOG_DEBUG(logger, "Importing mesh '%s' for model '%s'.", filename, name);

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

    LOG_DEBUG(logger, "Mesh name is '%s' for model '%s'." , name_, name);

    if (mesh->mNormals == 0)
    {
        LOG_WARN(logger, "Unable to import mesh '%s' for model '%s' - it does not have any normals." , filename, name);
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
        vertexBoneData_ = VertexBoneData( name, filename, index, mesh, boneData_, static_cast<uint32>(vertices_.size()), logger, fileSystem );
    }

    //std::cout << "Load results: " << mesh->mNumVertices << " " << mesh->mNumBones << " " << temp << " " << data.bones.size() << std::endl;

    LOG_DEBUG(logger, "Done importing mesh '%s' for model '%s'." , filename, name);
}
}
