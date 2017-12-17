#ifndef AUDIOFACTORY_H_
#define AUDIOFACTORY_H_

#include <memory>

#include "IAudioEngine.hpp"

#include "utilities/Properties.hpp"
#include "fs/IFileSystem.hpp"
#include "logger/ILogger.hpp"

namespace ice_engine
{
namespace audio
{

class AudioFactory
{
public:

	static std::unique_ptr<IAudioEngine> createAudioEngine(
		utilities::Properties* properties,
		fs::IFileSystem* fileSystem,
		logger::ILogger* logger
	);

private:
	AudioFactory();
	AudioFactory(const AudioFactory& other);
	virtual ~AudioFactory();
};

}

}

#endif /* AUDIOFACTORY_H_ */
