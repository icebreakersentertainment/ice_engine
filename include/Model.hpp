#ifndef MODEL_H_
#define MODEL_H_

#include <vector>

#include "Mesh.hpp"
#include "Texture.hpp"
#include "Animation.hpp"
#include "Skeleton.hpp"

#include "logger/ILogger.hpp"
#include "fs/IFileSystem.hpp"

#include "Types.hpp"

namespace ice_engine
{

class IResourceCache;

class Model
{
public:

	Model() = default;
	
	Model(
		std::string name,
		std::vector<Mesh> meshes,
	//	std::vector<Material> materials,
		std::vector<Texture> textures,
		Skeleton skeleton,
		std::unordered_map<std::string, Animation> animations
	)
	:
		name_(std::move(name)),
		meshes_(std::move(meshes)),
		textures_(std::move(textures)),
		skeleton_(std::move(skeleton)),
		animations_(std::move(animations))
	{
	}

	Model(const std::string& filename, IResourceCache* resourceCache, logger::ILogger* logger, fs::IFileSystem* fileSystem)
	{
		import(filename, resourceCache, logger, fileSystem);
	}

	const std::string& name() const
	{
		return name_;
	}

	const std::vector<Mesh>& meshes() const
	{
		return meshes_;
	}

	const std::vector<Texture>& textures() const
	{
		return textures_;
	}

	const Skeleton& skeleton() const
	{
		return skeleton_;
	}

	const std::unordered_map<std::string, Animation>& animations() const
	{
		return animations_;
	}

private:
	std::string name_;
	std::vector<Mesh> meshes_;
//	std::vector<Material> materials_;
	std::vector<Texture> textures_;
	Skeleton skeleton_;
	std::unordered_map<std::string, Animation> animations_;

/*
	Material importMaterial(const std::string& name, const std::string& filename, uint32 index, const aiMaterial* material, logger::ILogger* logger, fs::IFileSystem* fileSystem)
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
			utilities::toGlm(&c, data.emission);
		}

		logger->debug( "done importing material." );

		return std::move(data);
	}
*/

	void importAnimations(const std::string& name, const std::string& filename, const aiScene* scene, logger::ILogger* logger, fs::IFileSystem* fileSystem);

	void import(const std::string& filename, IResourceCache* resourceCache, logger::ILogger* logger, fs::IFileSystem* fileSystem);
};

}

#endif /* MODEL_H_ */
