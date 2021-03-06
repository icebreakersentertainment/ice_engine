#include "ResourceCache.hpp"

namespace ice_engine
{

void ResourceCache::addAudio(const std::string& name, std::unique_ptr<Audio> audio)
{
	std::lock_guard<std::recursive_mutex> lock(audioMutex_);
	
	if (getAudio(name) != nullptr)
	{
		throw RuntimeException(detail::format("Audio with name '%s' already exists.", name));
	}
	
	audios_[name] = std::move(audio);
}

void ResourceCache::addImage(const std::string& name, std::unique_ptr<Image> image)
{
	std::lock_guard<std::recursive_mutex> lock(imageMutex_);
	
	if (getImage(name) != nullptr)
	{
		throw RuntimeException(detail::format("Image with name '%s' already exists.", name));
	}
	
	images_[name] = std::move(image);
}

void ResourceCache::addModel(const std::string& name, std::unique_ptr<Model> model)
{
	std::lock_guard<std::recursive_mutex> lock(modelMutex_);
	
	if (getModel(name) != nullptr)
	{
		throw RuntimeException(detail::format("Model with name '%s' already exists.", name));
	}
	
	models_[name] = std::move(model);
}

void ResourceCache::removeAudio(const std::string& name)
{
	std::lock_guard<std::recursive_mutex> lock(audioMutex_);
	
	auto it = audios_.find(name);
	if (it != audios_.end())
	{
		audios_.erase(it);
	}
}

void ResourceCache::removeImage(const std::string& name)
{
	std::lock_guard<std::recursive_mutex> lock(imageMutex_);
	
	auto it = images_.find(name);
	if (it != images_.end())
	{
		images_.erase(it);
	}
}

void ResourceCache::removeModel(const std::string& name)
{
	std::lock_guard<std::recursive_mutex> lock(modelMutex_);
	
	auto it = models_.find(name);
	if (it != models_.end())
	{
		models_.erase(it);
	}
}

Audio* ResourceCache::getAudio(const std::string& name) const
{
	std::lock_guard<std::recursive_mutex> lock(audioMutex_);
	
	auto it = audios_.find(name);
	if (it != audios_.end())
	{
		return it->second.get();
	}
	
	return nullptr;
}

Image* ResourceCache::getImage(const std::string& name) const
{
	std::lock_guard<std::recursive_mutex> lock(imageMutex_);
	
	auto it = images_.find(name);
	if (it != images_.end())
	{
		return it->second.get();
	}
	
	return nullptr;
}

Model* ResourceCache::getModel(const std::string& name) const
{
	std::lock_guard<std::recursive_mutex> lock(modelMutex_);
	
	auto it = models_.find(name);
	if (it != models_.end())
	{
		return it->second.get();
	}
	
	return nullptr;
}


}
