#include "ResourceCache.hpp"

namespace ice_engine
{

ResourceCache::ResourceCache()
{
}

ResourceCache::~ResourceCache()
{
}

void ResourceCache::addAudio(const std::string& name, std::unique_ptr<Audio> audio)
{
	std::lock_guard<std::recursive_mutex> lock(audioMutex_);
	
	if (getAudio(name) != nullptr)
	{
		throw std::runtime_error("Audio with name '" + name + "' already exists.");
	}
	
	audios_[name] = std::move(audio);
}

void ResourceCache::addImage(const std::string& name, std::unique_ptr<image::Image> image)
{
	std::lock_guard<std::recursive_mutex> lock(imageMutex_);
	
	if (getImage(name) != nullptr)
	{
		throw std::runtime_error("Image with name '" + name + "' already exists.");
	}
	
	images_[name] = std::move(image);
}

void ResourceCache::addModel(const std::string& name, std::unique_ptr<graphics::model::Model> model)
{
	std::lock_guard<std::recursive_mutex> lock(modelMutex_);
	
	if (getModel(name) != nullptr)
	{
		throw std::runtime_error("Model with name '" + name + "' already exists.");
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

image::Image* ResourceCache::getImage(const std::string& name) const
{
	std::lock_guard<std::recursive_mutex> lock(imageMutex_);
	
	auto it = images_.find(name);
	if (it != images_.end())
	{
		return it->second.get();
	}
	
	return nullptr;
}

graphics::model::Model* ResourceCache::getModel(const std::string& name) const
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
