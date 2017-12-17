#ifndef IRESOURCECACHE_H_
#define IRESOURCECACHE_H_

#include <string>
#include <memory>

#include "graphics/model/Model.hpp"
#include "image/Image.hpp"

namespace ice_engine
{

class IResourceCache
{
public:
	virtual ~IResourceCache()
	{
	}
	;

	//virtual void addAudioSample(std::unique_ptr<AudioSample> audioSample, const std::string& name) = 0;
	virtual void addImage(const std::string& name, std::unique_ptr<image::Image> image) = 0;
	virtual void addModel(const std::string& name, std::unique_ptr<graphics::model::Model> model) = 0;
	
	//virtual void removeAudioSample(const std::string& name) = 0;
	virtual void removeImage(const std::string& name) = 0;
	virtual void removeModel(const std::string& name) = 0;
	
	//virtual AudioSample* getAudioSample(const std::string& name) const = 0;
	virtual image::Image* getImage(const std::string& name) const = 0;
	virtual graphics::model::Model* getModel(const std::string& name) const = 0;

};

}

#endif /* IRESOURCECACHE_H_ */
