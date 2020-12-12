#ifndef RESOURCECACHE_H_
#define RESOURCECACHE_H_

#include <unordered_map>
#include <mutex>

#include "IResourceCache.hpp"

namespace ice_engine
{

class ResourceCache : public IResourceCache
{
public:
	~ResourceCache() override = default;

	void addAudio(const std::string& name, std::unique_ptr<Audio> audio) override;
	void addImage(const std::string& name, std::unique_ptr<Image> image) override;
	void addModel(const std::string& name, std::unique_ptr<Model> model) override;
	
	void removeAudio(const std::string& name) override;
	void removeImage(const std::string& name) override;
	void removeModel(const std::string& name) override;
	
	Audio* getAudio(const std::string& name) const override;
	Image* getImage(const std::string& name) const override;
	Model* getModel(const std::string& name) const override;

private:
	std::unordered_map<std::string, std::unique_ptr<Model>> models_;
	std::unordered_map<std::string, std::unique_ptr<Audio>> audios_;
	std::unordered_map<std::string, std::unique_ptr<Image>> images_;
	std::unique_ptr<Image> imagesTest_;
	
	mutable std::recursive_mutex audioMutex_;
	mutable std::recursive_mutex imageMutex_;
	mutable std::recursive_mutex modelMutex_;

};

}

#endif /* RESOURCECACHE_H_ */
