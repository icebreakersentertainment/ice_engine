#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

// C++ importer interface
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

#include "graphics/model/BoneNode.hpp"
#include "graphics/model/BoneData.hpp"
#include "graphics/model/AnimatedBoneNode.hpp"
#include "graphics/model/ModelLoader.hpp"
#include "graphics/model/Model.hpp"
#include "graphics/model/AnimationSet.hpp"
#include "graphics/model/Animation.hpp"
#include "graphics/model/Mesh.hpp"
#include "graphics/model/Texture.hpp"
#include "graphics/model/Material.hpp"

#include "utilities/AssImpUtilities.hpp"

namespace hercules
{
namespace graphics
{
namespace model
{

namespace
{

Mesh importMesh(const std::string& name, const std::string& filename, uint32 index, const aiMesh* mesh, std::map< std::string, uint32 >& boneIndexMap, logger::ILogger* logger)
{
	Mesh data = Mesh();
	
	logger->debug( "importing mesh '" + filename + "'." );
	
	// Set the mesh name
	if (mesh->mName.length > 0)
	{
		data.name = std::string( mesh->mName.C_Str() );
	}
	else
	{
		data.name = name + "_mesh_" + std::to_string(index);
	}
	
	logger->debug( "mesh name: " + data.name );

	if (mesh->mNormals == 0)
	{
		logger->warn( "Unable to import mesh - it does not have any normals." );
		return data;
	}

	// Load vertices, indices, normals, texture coordinates, and colors
	data.vertices.resize(mesh->mNumVertices);
	data.normals.resize(mesh->mNumVertices);
	data.textureCoordinates.resize(mesh->mNumVertices);
	data.colors.resize(mesh->mNumVertices);

	for ( uint32 i = 0; i < mesh->mNumVertices; i++ )
	{
		data.vertices[i] = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
		data.normals[i] = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);

		if ( mesh->HasTextureCoords(0))
		{
			data.textureCoordinates[i] = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
		}

		if ( mesh->mColors[0] != 0 )
		{
			data.colors[i] = glm::vec4(
				(float)mesh->mColors[0][i].a,
				(float)mesh->mColors[0][i].b,
				(float)mesh->mColors[0][i].g,
				(float)mesh->mColors[0][i].r
				);
		}
	}
	
	data.indices.reserve(mesh->mNumVertices);
	
	for ( uint32 i = 0; i < mesh->mNumFaces; i++ )
	{
		const aiFace& face = mesh->mFaces[i];
		
		if ( face.mNumIndices != 3 )
		{
			const auto msg = std::string("Unable to import model...Unsupported number of indices per face (") + std::to_string(face.mNumIndices) + std::string(").");
			throw std::runtime_error(msg);
		}
		
		data.indices.push_back(face.mIndices[0]);
		data.indices.push_back(face.mIndices[1]);
		data.indices.push_back(face.mIndices[2]);
	}
	
	int temp = 0;
	
	// If we have any bones, load them
	if (mesh->mNumBones > 0)
	{
		data.bones.resize( data.vertices.size() );
		
		// Load bone data
		for (uint32 i = 0; i < mesh->mNumBones; i++)
		{
			uint32 boneIndex = boneIndexMap[ mesh->mBones[i]->mName.C_Str() ];
			
			for (uint32 j = 0; j < mesh->mBones[i]->mNumWeights; j++)
			{
				uint32 vertexID = mesh->mBones[i]->mWeights[j].mVertexId;
				float32 weight = mesh->mBones[i]->mWeights[j].mWeight;

				temp++;
				data.bones[ vertexID ].addBoneWeight( boneIndex, weight );
			}
		}
	}
	
	//std::cout << "Load results: " << mesh->mNumVertices << " " << mesh->mNumBones << " " << temp << " " << data.bones.size() << std::endl;

	logger->debug( "done importing mesh '" + filename + "'." );
	
	return data;
}

Texture importTexture(const std::string& name, const std::string& filename, uint32 index, const aiMaterial* material, logger::ILogger* logger)
{
	assert( material != nullptr );
	
	Texture data = Texture();

	logger->debug( "Loading texture." );

	aiReturn texFound = AI_SUCCESS;
	aiString texPath;
	
	if ( material->GetTextureCount(aiTextureType_DIFFUSE) > 0 )
	{
		texFound = material->GetTexture(aiTextureType_DIFFUSE, 0, &texPath);
		
		// Error check
		if (texFound != AI_SUCCESS)
		{
			logger->warn( "Texture not found for model filename: " + filename );
			return data;
		}

		logger->debug( std::string("Texture has filename: ") + texPath.data );
		
		data.filename = texPath.data;
		
	}
	else
	{
		logger->debug( "No texture specified." );
	}
	
	return data;
}

glm::mat4 convertAssImpMatrix(const aiMatrix4x4* m)
{
	return glm::mat4(
		m->a1, m->b1, m->c1, m->d1,
		m->a2, m->b2, m->c2, m->d2,
		m->a3, m->b3, m->c3, m->d3,
		m->a4, m->b4, m->c4, m->d4
	);
}

Material importMaterial(const std::string& name, const std::string& filename, uint32 index, const aiMaterial* material, logger::ILogger* logger)
{	
	assert( material != nullptr );
		
	Material data = Material();

	logger->debug( "importing material." );
	aiColor4D c;
	
	// Set the material name
	data.name = name + "_material_" + std::to_string(index);

	logger->debug( "material name: " + data.name );

	data.diffuse[0] = 0.8f;
	data.diffuse[1] = 0.8f;
	data.diffuse[2] = 0.8f;
	data.diffuse[3] = 1.0f;
	if ( AI_SUCCESS == aiGetMaterialColor(material, AI_MATKEY_COLOR_DIFFUSE, &c) )
	{
		utilities::color4ToVec4(&c, data.diffuse);
	}

	//utilities::setFloat4(c, 0.0f, 0.0f, 0.0f, 1.0f);
	data.specular[0] = 0.0f;
	data.specular[1] = 0.0f;
	data.specular[2] = 0.0f;
	data.specular[3] = 1.0f;
	if ( AI_SUCCESS == aiGetMaterialColor(material, AI_MATKEY_COLOR_SPECULAR, &c) )
	{
		utilities::color4ToVec4(&c, data.specular);
	}

	//utilities::setFloat4(c, 0.2f, 0.2f, 0.2f, 1.0f);
	data.ambient[0] = 0.2f;
	data.ambient[1] = 0.2f;
	data.ambient[2] = 0.2f;
	data.ambient[3] = 1.0f;
	if ( AI_SUCCESS == aiGetMaterialColor(material, AI_MATKEY_COLOR_AMBIENT, &c) )
	{
		utilities::color4ToVec4(&c, data.ambient);
	}

	//utilities::setFloat4(c, 0.0f, 0.0f, 0.0f, 1.0f);
	data.emission[0] = 0.0f;
	data.emission[1] = 0.0f;
	data.emission[2] = 0.0f;
	data.emission[3] = 1.0f;
	if ( AI_SUCCESS == aiGetMaterialColor(material, AI_MATKEY_COLOR_EMISSIVE, &c) )
	{
		utilities::color4ToVec4(&c, data.emission);
	}
	
	logger->debug( "done importing material." );
	
	return data;
}

BoneData importBones(const std::string& name, const std::string& filename, uint32 index, const aiMesh* mesh, logger::ILogger* logger)
{
	assert( mesh != nullptr );
	
	BoneData boneData = BoneData();
	
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
		
		if (boneData.boneIndexMap.find(data.name) == boneData.boneIndexMap.end())
		{
			boneIndex = boneData.boneIndexMap.size();
			boneData.boneTransform.push_back(data);
		}
		else
		{
			boneIndex = boneData.boneIndexMap[data.name];
		}

		boneData.boneIndexMap[data.name] = boneIndex;
		boneData.boneTransform[boneIndex].boneOffset = convertAssImpMatrix( &(mesh->mBones[i]->mOffsetMatrix) );
	} 

	logger->debug( "done importing boneData." );
	
	return boneData;
}

BoneNode importBoneNode( const aiNode* node )
{
	BoneNode boneNode = BoneNode();
	boneNode.name = std::string( node->mName.C_Str() );
	boneNode.transformation = convertAssImpMatrix( &(node->mTransformation) );
	
	for (uint32 i = 0; i < node->mNumChildren; i++)
	{
		boneNode.children.push_back( importBoneNode( node->mChildren[i] ) );
	}
	
	return boneNode;
}

AnimationSet importAnimations(const std::string& name, const std::string& filename, const aiScene* scene, logger::ILogger* logger)
{
	assert( scene != nullptr );
	
	AnimationSet animationSet = AnimationSet();
	
	std::stringstream msg;
	msg << "importing " << scene->mNumAnimations << " animation(s).";
	logger->debug( msg.str() );
	
	glm::mat4 globalInverseTransformation = convertAssImpMatrix( &(scene->mRootNode->mTransformation) );
	globalInverseTransformation = glm::inverse( globalInverseTransformation );
	
	animationSet.globalInverseTransformation = globalInverseTransformation;
	
	// Load BoneNodes
	const aiNode* assImpRootNode = scene->mRootNode;
	animationSet.rootBoneNode = importBoneNode( assImpRootNode );
	
	for (uint32 i = 0; i < scene->mNumAnimations; i++)
	{
		// Load some basic animation data
		Animation animation = Animation();
		animation.name = std::string( scene->mAnimations[i]->mName.C_Str() );
		
		// Error check - animations with no name are not allowed
		if (animation.name.compare( std::string("") ) == 0)
		{
			logger->warn( "Animations with no name are not allowed." );
			
			animation.name = name + "_animation_" + std::to_string(i);
			logger->warn( "Setting animation name to: " + animation.name );
			// TODO: should we throw an exception?
			//throw std::runtime_error(msg);
			
			// TODO: Should we skip to the next animation?
			//continue;
		}
		std::string msg = std::string("Loading animation...");
		
		animation.duration = scene->mAnimations[i]->mDuration;
		animation.ticksPerSecond = scene->mAnimations[i]->mTicksPerSecond;
		
		// Load AnimatedBoneNodes for this animation
		for (uint32 j = 0; j < scene->mAnimations[i]->mNumChannels; j++)
		{
			const aiNodeAnim* pNodeAnim = scene->mAnimations[i]->mChannels[j];
			
			AnimatedBoneNode abn = AnimatedBoneNode();
			abn.name = std::string( pNodeAnim->mNodeName.C_Str() );

			for (uint32 k = 0; k < pNodeAnim->mNumPositionKeys; k++)
			{
				abn.positionTimes.push_back( pNodeAnim->mPositionKeys[k].mTime );
				abn.positions.push_back( glm::vec3( pNodeAnim->mPositionKeys[k].mValue.x, pNodeAnim->mPositionKeys[k].mValue.y, pNodeAnim->mPositionKeys[k].mValue.z ) );
			}
			
			for (uint32 k = 0; k < pNodeAnim->mNumRotationKeys; k++)
			{
				const auto& rk = pNodeAnim->mRotationKeys[k];

				abn.rotationTimes.push_back( rk.mTime );

				glm::quat rotation = glm::quat( rk.mValue.w, rk.mValue.x, rk.mValue.y, rk.mValue.z );
				rotation = glm::normalize( rotation );
				abn.rotations.push_back( rotation );
			}
			
			for (uint32 k = 0; k < pNodeAnim->mNumScalingKeys; k++)
			{
				abn.scalingTimes.push_back( pNodeAnim->mScalingKeys[k].mTime );
				abn.scalings.push_back( glm::vec3( pNodeAnim->mScalingKeys[k].mValue.x, pNodeAnim->mScalingKeys[k].mValue.y, pNodeAnim->mScalingKeys[k].mValue.z ) );
			}

			// Add AnimatedBoneNode to the animation
			if( animation.animatedBoneNodes.find( abn.name ) == animation.animatedBoneNodes.end() )
			{
				animation.animatedBoneNodes[ abn.name ] = abn;
			}
			else 
			{
				// Warning - animated bone node already exists!
				logger->warn( "Animated bone node with name '" + abn.name + "' already exists!" );
			}
		}
		
		// Add animation to animation data
		if( animationSet.animations.find( animation.name ) == animationSet.animations.end() )
		{
			animationSet.animations[ animation.name ] = animation;
		}
		else 
		{
			// Warning - animation already exists!
			logger->warn( "Animation with name '" + animation.name + "' already exists!" );
		}
	}

	logger->debug( "done importing animation." );
	
	return animationSet;
}

Model importModelData(const std::string& name, const std::string& filename, logger::ILogger* logger)
{
	logger->debug( "Importing model data from file '" + filename + "'." );

	auto model = Model();

	// We don't currently support aiProcess_JoinIdenticalVertices or aiProcess_FindInvalidData
	// aiProcess_FindInvalidData - I think it's due to the reduction of animation tracks containing redundant keys..
	const aiScene* scene = aiImportFile(filename.c_str(), aiProcessPreset_TargetRealtime_MaxQuality ^ aiProcess_FindInvalidData);

	// Error checking
	if ( scene == nullptr )
	{		
		std::stringstream ss;
		ss << "Unable to import model data from file '" << filename << "'.";
		throw std::runtime_error(ss.str());
	}
	else if ( scene->HasTextures() )
	{
		// Cleanup
		aiReleaseImport(scene);
		
		std::string msg = std::string("Support for meshes with embedded textures is not implemented.");
		throw std::runtime_error(msg);
	}
	
	try
	{
		model.meshes.resize( scene->mNumMeshes );
		model.materials.resize( scene->mNumMeshes );
		model.textures.resize( scene->mNumMeshes );
		model.boneData.resize( scene->mNumMeshes );
		
		auto animationSet = importAnimations(name, filename, scene, logger);
		
		// Create bone structure (tree structure)
		model.rootBoneNode = animationSet.rootBoneNode;
		
		// Set the global inverse transformation
		model.globalInverseTransformation = animationSet.globalInverseTransformation;
		
		// Load the animation information
		for ( auto& kv : animationSet.animations)
		{
			model.animations.push_back(kv.second);
			/*
			// Create animated bone node information
			auto animatedBoneNodes = std::map< std::string, AnimatedBoneNode >();
			for ( auto& kvAnimatedBoneNode : kv.second.animatedBoneNodes )
			{
				animatedBoneNodes[ kvAnimatedBoneNode.first ] = AnimatedBoneNode( 
					kvAnimatedBoneNode.second.name, 
					kvAnimatedBoneNode.second.positionTimes, 
					kvAnimatedBoneNode.second.rotationTimes, 
					kvAnimatedBoneNode.second.scalingTimes, 
					kvAnimatedBoneNode.second.positions, 
					kvAnimatedBoneNode.second.rotations, 
					kvAnimatedBoneNode.second.scalings
				);
			}
			
			// Actually create the animation
			auto animation = Animation( kv.second.name, kv.second.duration, kv.second.ticksPerSecond, animatedBoneNodes );
			
			assert(animation != nullptr);
			
			model.animations.push_back(animation);
			
			// TODO: add animations properly (i.e. with names specifying the animation i guess?)
			//std::cout << "anim: " << animation->getName() << std::endl;
			*/
		}
		
		std::stringstream msg;
		msg << "Model has " << model.meshes.size() << " meshes.";
		logger->debug( msg.str() );
		
		for ( uint32 i=0; i < model.meshes.size(); i++ )
		{
			model.meshes[i] = Mesh();
			model.materials[i] = Material();
			model.textures[i] = Texture();
			model.boneData[i] = BoneData();
			
			model.boneData[i] = importBones( name, filename, i, scene->mMeshes[i], logger );
			model.meshes[i] = importMesh( name, filename, i, scene->mMeshes[i], model.boneData[i].boneIndexMap, logger );
			model.materials[i] = importMaterial( name, filename, i, scene->mMaterials[ scene->mMeshes[i]->mMaterialIndex ], logger );
			model.textures[i] = importTexture( name, filename, i, scene->mMaterials[ scene->mMeshes[i]->mMaterialIndex ], logger );
		}
		
		bool hasTextures = false;
		for ( uint32 i=0; i < model.meshes.size(); i++ )
		{
			if (model.textures[i].filename != std::string(""))
			{
				hasTextures = true;
				break;
			}
		}
		
		if (!hasTextures)
		{
			logger->warn( "Model does not have any texture - either assign it a texture, or use a shader that doesn't need textures." );
		}
	}
	catch (const std::exception& e)
	{
		// cleanup - calling 'aiReleaseImport' is important, as the library
		// keeps internal resources until the scene is freed again. Not
		// doing so can cause severe resource leaking.
		aiReleaseImport(scene);
		
		throw e;
	}

	logger->debug( "Done importing model data for file '" + filename + "'." );
	
	return std::move(model);
}

}

Model load(const std::string& name, const std::string& filename, logger::ILogger* logger)
{
	logger->debug( "Loading model '" + name + "' - " + filename + "." );
	
	auto model = Model();
	
	//model = loadModelData(name, filename);

	logger->debug( "Done loading model '" + name + "'." );
	
	return std::move(model);
}

Model import(const std::string& name, const std::string& filename, logger::ILogger* logger)
{
	logger->debug( "Importing model '" + name + "' - " + filename + "." );
	
	// get a handle to the predefined STDOUT log stream and attach
	// it to the logging system. It remains active for all further
	// calls to aiImportFile(Ex) and aiApplyPostProcessing.
	//stream = aiGetPredefinedLogStream(aiDefaultLogStream_FILE,"assimp_log.txt");
#ifdef DEBUG
	aiLogStream stream = aiGetPredefinedLogStream(aiDefaultLogStream_STDOUT, nullptr);
	aiAttachLogStream(&stream);
#endif

	auto model = Model();
	try
	{
		model = importModelData(name, filename, logger);

		logger->debug( "Done importing model '" + name + "'." );
	}
	catch (const std::exception& e)
	{
		// We added a log stream to the library, it's our job to disable it
		// again. This will definitely release the last resources allocated
		// by Assimp.
#ifdef DEBUG
		aiDetachAllLogStreams();
#endif
		
		throw e;
	}

	// We added a log stream to the library, it's our job to disable it
	// again. This will definitely release the last resources allocated
	// by Assimp.
#ifdef DEBUG
	aiDetachAllLogStreams();
#endif

	return std::move(model);
}

void save(const std::string& name, const std::string& filename, const Model& model, logger::ILogger* logger)
{
	/*
	logger->debug( "Saving model '" + name + "' - " + filename + "." );
	
	// get a handle to the predefined STDOUT log stream and attach
	// it to the logging system. It remains active for all further
	// calls to aiImportFile(Ex) and aiApplyPostProcessing.
	//stream = aiGetPredefinedLogStream(aiDefaultLogStream_FILE,"assimp_log.txt");
#ifdef DEBUG
	aiLogStream stream = aiGetPredefinedLogStream(aiDefaultLogStream_STDOUT, nullptr);
	aiAttachLogStream(&stream);
#endif

	try
	{
		saveModelData(name, filename, model);

		logger->debug( "Done saving model '" + name + "'." );
	}
	catch (const std::exception& e)
	{
		// We added a log stream to the library, it's our job to disable it
		// again. This will definitely release the last resources allocated
		// by Assimp.
#ifdef DEBUG
		aiDetachAllLogStreams();
#endif
		
		throw e;
	}

	// We added a log stream to the library, it's our job to disable it
	// again. This will definitely release the last resources allocated
	// by Assimp.
#ifdef DEBUG
	aiDetachAllLogStreams();
#endif
	*/
}

}
}
}
