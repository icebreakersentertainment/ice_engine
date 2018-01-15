#ifndef RESOURCECACHE_H_
#define RESOURCECACHE_H_

#include <map>
#include <mutex>

#include "IResourceCache.hpp"

namespace ice_engine
{

class ResourceCache : public IResourceCache
{
public:
	ResourceCache();
	virtual ~ResourceCache();

	virtual void addAudio(const std::string& name, std::unique_ptr<Audio> audio) override;
	virtual void addImage(const std::string& name, std::unique_ptr<image::Image> image) override;
	virtual void addModel(const std::string& name, std::unique_ptr<graphics::model::Model> model) override;
	
	virtual void removeAudio(const std::string& name) override;
	virtual void removeImage(const std::string& name) override;
	virtual void removeModel(const std::string& name) override;
	
	virtual Audio* getAudio(const std::string& name) const override;
	virtual image::Image* getImage(const std::string& name) const override;
	virtual graphics::model::Model* getModel(const std::string& name) const override;

private:
	std::map<std::string, std::unique_ptr<graphics::model::Model>> models_;
	std::map<std::string, std::unique_ptr<Audio>> audios_;
	std::map<std::string, std::unique_ptr<image::Image>> images_;
	std::unique_ptr<image::Image> imagesTest_;
	
	mutable std::recursive_mutex audioMutex_;
	mutable std::recursive_mutex imageMutex_;
	mutable std::recursive_mutex modelMutex_;

};

}

#endif /* RESOURCECACHE_H_ */
