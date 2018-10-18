#include "Texture.hpp"

#include "IResourceCache.hpp"

#include "exceptions/FileNotFoundException.hpp"

namespace ice_engine
{

void Texture::import(const std::string& name,
		const std::string& filename, const uint32 index,
		const aiMaterial* material, IResourceCache* resourceCache,
		logger::ILogger* logger, fs::IFileSystem* fileSystem)
{
	assert( material != nullptr );

	LOG_DEBUG(logger, "Loading texture %s with filename %s.", name, filename);

	aiReturn texFound = AI_SUCCESS;
	aiString texPath;

	if ( material->GetTextureCount(aiTextureType_DIFFUSE) > 0 )
	{
		texFound = material->GetTexture(aiTextureType_DIFFUSE, 0, &texPath);

		// Error check
		if (texFound != AI_SUCCESS)
		{
			logger->warn( "Texture not found for model filename: " + filename );
			return;
		}

		auto basePath = fileSystem->getBasePath(filename);
		auto fullPath = basePath + fileSystem->getDirectorySeperator() + texPath.data;

		LOG_DEBUG(logger, "Texture has filename: %s", fullPath);

		auto image = resourceCache->getImage(fullPath);
		if (image != nullptr)
		{
			LOG_DEBUG(logger, "Image found in cache");
			image_ = image;
		}
		else
		{
			if (!fileSystem->exists(fullPath))
			{
				throw FileNotFoundException("Texture with filename '" + fullPath + "' does not exist.");
			}

//			data.filename = fullPath;

			auto file = fileSystem->open(fullPath, fs::FileFlags::READ | fs::FileFlags::BINARY);
			auto image = std::make_unique<Image>(file->getInputStream());

			resourceCache->addImage(fullPath, std::move(image));

			image_ = resourceCache->getImage(fullPath);
		}
	}
	else
	{
		LOG_DEBUG(logger, "No texture specified.");
	}
}


}
