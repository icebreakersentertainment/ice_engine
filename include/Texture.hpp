#ifndef TEXTURE_H_
#define TEXTURE_H_

#include <vector>
#include <string>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include <assimp/material.h>

#include "graphics/ITexture.hpp"

#include "logger/ILogger.hpp"
#include "fs/IFileSystem.hpp"

#include "Image.hpp"

#include "Types.hpp"

namespace ice_engine
{

class IResourceCache;

class Texture : public graphics::ITexture
{
public:

	Texture() = default;
	
	Texture(
		std::string name,
		IImage* image
	)
	:
		name_(std::move(name)),
		image_(image)
	{
	}

	Texture(const std::string& name, const std::string& filename, const uint32 index, const aiMaterial* material, IResourceCache* resourceCache, logger::ILogger* logger, fs::IFileSystem* fileSystem)
	{
		import(name, filename, index, material, resourceCache, logger, fileSystem);
	}

	~Texture() override = default;

	const std::string& name() const override
	{
		return name_;
	}

	IImage* image()
	{
		return image_;
	}

	const graphics::IImage* image() const override
	{
		return image_;
	}

private:
	std::string name_;
	IImage* image_ = nullptr;

	void import(const std::string& name, const std::string& filename, const uint32 index, const aiMaterial* material, IResourceCache* resourceCache, logger::ILogger* logger, fs::IFileSystem* fileSystem);
};

}

#endif /* TEXTURE_H_ */
