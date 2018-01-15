#ifndef AUDIOENGINE_H_
#define AUDIOENGINE_H_

#include <string>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include <AL/al.h>
#include <AL/alc.h>

#include "audio/IAudioEngine.hpp"

#include "al/OpenAl.hpp"

#include "handles/HandleVector.hpp"
#include "utilities/Properties.hpp"
#include "fs/IFileSystem.hpp"
#include "logger/ILogger.hpp"

using namespace ice_engine::audio::openal::al;

namespace ice_engine
{
namespace audio
{
namespace openal
{
/*
struct AudioData
{
	float32 pitch;
	float32 gain;
	glm::vec3 position;
	glm::vec3 velocity;
	glm::vec3 direction;
};
*/
struct SoundSource
{
	ALuint source;
	ALuint buffer;
};

struct AudioScene
{
	handles::HandleVector<SoundSource, SoundSourceHandle> soundSources;
	ListenerHandle listenerHandle = ListenerHandle(1);
};

class AudioEngine : public IAudioEngine
{
public:
	AudioEngine(utilities::Properties* properties, fs::IFileSystem* fileSystem, logger::ILogger* logger);
	virtual ~AudioEngine();
	
	virtual AudioSceneHandle createAudioScene() override;
	virtual void destroyAudioScene(const AudioSceneHandle& audioSceneHandle) override;
	
	virtual void beginRender() override;
	virtual void render(const AudioSceneHandle& audioSceneHandle) override;
	virtual void endRender() override;
	
	virtual SoundSourceHandle play(const AudioSceneHandle& audioSceneHandle, const SoundHandle& soundHandle, const glm::vec3& position) override;
	
	virtual void stop(const AudioSceneHandle& audioSceneHandle, const SoundSourceHandle& soundSourceHandle) override;
	virtual void stopAll(const AudioSceneHandle& audioSceneHandle) override;
	
	virtual SoundHandle createSound(const IAudio* audio) override;
	
	virtual ListenerHandle createListener(const AudioSceneHandle& audioSceneHandle, const glm::vec3& position) override;
	
	virtual void setPosition(const AudioSceneHandle& audioSceneHandle, const SoundSourceHandle& soundSourceHandle, const float32 x, const float32 y, const float32 z) override;
	virtual void setPosition(const AudioSceneHandle& audioSceneHandle, const SoundSourceHandle& soundSourceHandle, const glm::vec3& position) override;
	virtual glm::vec3 position(const AudioSceneHandle& audioSceneHandle, const SoundSourceHandle& soundSourceHandle) const override;
	
	virtual void setPosition(const AudioSceneHandle& audioSceneHandle, const ListenerHandle& listenerHandle, const float32 x, const float32 y, const float32 z) override;
	virtual void setPosition(const AudioSceneHandle& audioSceneHandle, const ListenerHandle& listenerHandle, const glm::vec3& position) override;
	virtual glm::vec3 position(const AudioSceneHandle& audioSceneHandle, const ListenerHandle& listenerHandle) const override;

private:
	AudioEngine(const AudioEngine& other);
	
	ALCdevice* device_ = nullptr;
	ALCcontext* context_ = nullptr;

	utilities::Properties* properties_;
	fs::IFileSystem* fileSystem_;
	logger::ILogger* logger_;
	
	handles::HandleVector<ALuint, SoundHandle> soundHandles_;
	handles::HandleVector<AudioScene, AudioSceneHandle> audioSceneHandles_;
};

}
}
}

#endif /* AUDIOENGINE_H_ */
