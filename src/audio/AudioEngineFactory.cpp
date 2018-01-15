#include "audio/AudioEngineFactory.hpp"
#include "audio/openal/AudioEngine.hpp"

namespace ice_engine
{
namespace audio
{

AudioEngineFactory::AudioEngineFactory()
{
}

AudioEngineFactory::AudioEngineFactory(const AudioEngineFactory& other)
{
}

AudioEngineFactory::~AudioEngineFactory()
{
}

std::unique_ptr<IAudioEngine> AudioEngineFactory::create(
	utilities::Properties* properties,
	fs::IFileSystem* fileSystem,
	logger::ILogger* logger
)
{
	std::unique_ptr<IAudioEngine> ptr = std::make_unique< openal::AudioEngine >( properties, fileSystem, logger );
	
	return std::move( ptr );
}

}
}
