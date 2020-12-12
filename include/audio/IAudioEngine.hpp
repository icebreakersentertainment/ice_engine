#ifndef IAUDIOENGINE_H_
#define IAUDIOENGINE_H_

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include "Types.hpp"

#include "AudioSceneHandle.hpp"
#include "SoundHandle.hpp"
#include "ListenerHandle.hpp"
#include "SoundSourceHandle.hpp"

#include "IAudio.hpp"

namespace ice_engine
{
namespace audio
{

class IAudioEngine
{
public:
	virtual ~IAudioEngine() = default;
	
	virtual AudioSceneHandle createAudioScene() = 0;
	virtual void destroyAudioScene(const AudioSceneHandle& audioSceneHandle) = 0;
	
	virtual void tick(const AudioSceneHandle audioSceneHandle, const float32 delta) = 0;

	virtual void beginRender() = 0;
	virtual void render(const AudioSceneHandle& audioSceneHandle) = 0;
	virtual void endRender() = 0;
	
	virtual SoundSourceHandle play(const AudioSceneHandle& audioSceneHandle, const SoundHandle& soundHandle, const glm::vec3& position) = 0;
	
	virtual void stop(const AudioSceneHandle& audioSceneHandle, const SoundSourceHandle& soundSourceHandle) = 0;
	virtual void stopAll(const AudioSceneHandle& audioSceneHandle) = 0;
	
	virtual SoundHandle createSound(const IAudio& audio) = 0;
	virtual void destroy(const SoundHandle soundHandle) = 0;

	virtual ListenerHandle createListener(const AudioSceneHandle& audioSceneHandle, const glm::vec3& position) = 0;
	
	virtual void setPosition(const AudioSceneHandle& audioSceneHandle, const SoundSourceHandle& soundSourceHandle, const float32 x, const float32 y, const float32 z) = 0;
	virtual void setPosition(const AudioSceneHandle& audioSceneHandle, const SoundSourceHandle& soundSourceHandle, const glm::vec3& position) = 0;
	virtual glm::vec3 position(const AudioSceneHandle& audioSceneHandle, const SoundSourceHandle& soundSourceHandle) const = 0;
	
	virtual void setPosition(const AudioSceneHandle& audioSceneHandle, const ListenerHandle& listenerHandle, const float32 x, const float32 y, const float32 z) = 0;
	virtual void setPosition(const AudioSceneHandle& audioSceneHandle, const ListenerHandle& listenerHandle, const glm::vec3& position) = 0;
	virtual glm::vec3 position(const AudioSceneHandle& audioSceneHandle, const ListenerHandle& listenerHandle) const = 0;
};

}

}

#endif /* IAUDIOENGINE_H_ */
