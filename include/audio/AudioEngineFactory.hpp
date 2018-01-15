#ifndef AUDIOENGINEFACTORY_H_
#define AUDIOENGINEFACTORY_H_

#include "IAudioEngineFactory.hpp"

namespace ice_engine
{
namespace audio
{

class AudioEngineFactory : public IAudioEngineFactory
{
public:
	AudioEngineFactory();
	virtual ~AudioEngineFactory();
	
	virtual std::unique_ptr<IAudioEngine> create(
		utilities::Properties* properties,
		fs::IFileSystem* fileSystem,
		logger::ILogger* logger
	) override;

private:
	AudioEngineFactory(const AudioEngineFactory& other);
};

}

}

#endif /* AUDIOENGINEFACTORY_H_ */
