#ifndef IRESOURCECACHE_H_
#define IRESOURCECACHE_H_

#include <string>
#include <memory>

#include "graphics/model/Model.hpp"
#include "Image.hpp"
#include "Audio.hpp"

namespace ice_engine
{

class IResourceCache
{
public:
	virtual ~IResourceCache()
	{
	}
	;

	virtual void addAudio(const std::string& name, std::unique_ptr<Audio> audio) = 0;
	virtual void addImage(const std::string& name, std::unique_ptr<Image> image) = 0;
	virtual void addModel(const std::string& name, std::unique_ptr<graphics::model::Model> model) = 0;
	
	virtual void removeAudio(const std::string& name) = 0;
	virtual void removeImage(const std::string& name) = 0;
	virtual void removeModel(const std::string& name) = 0;
	
	virtual Audio* getAudio(const std::string& name) const = 0;
	virtual Image* getImage(const std::string& name) const = 0;
	virtual graphics::model::Model* getModel(const std::string& name) const = 0;

};

}

#endif /* IRESOURCECACHE_H_ */
