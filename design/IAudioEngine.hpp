class AudioSampleHandle
{
};
class AudioSampleInstanceHandle
{
};
class AudioSceneHandle
{
};
class AudioSample
{
};

virtual void tick(const AudioSceneHandle& audioSceneHandle, const float32 delta) = 0;

virtual AudioSampleInstanceHandle createAudioSampleInstance(
	const AudioSceneHandle& audioSceneHandle,
	const AudioSampleHandle& audioSampleHandle,
	const bool loop = false,
	const float32 near = 0.0f,
	const float32 far = 10.0f,
	const float32 frequency = 1.0f
) = 0;
virtual AudioSampleInstanceHandle createAudioSample3dInstance(
	const AudioSceneHandle& audioSceneHandle,
	const AudioSampleHandle& audioSampleHandle,
	const glm::vec3& position,
	const bool loop = false,
	const float32 near = 0.0f,
	const float32 far = 10.0f,
	const float32 frequency = 1.0f
) = 0;
virtual void destroyAudioSampleInstance(const AudioSceneHandle& audioSceneHandle, const AudioSampleInstanceHandle& audioSampleInstanceHandle) = 0;
virtual void destroyAudioSample3dInstance(const AudioSceneHandle& audioSceneHandle, const AudioSample3dInstanceHandle& audioSample3dInstanceHandle) = 0;

virtual void play(const AudioSceneHandle& audioSceneHandle, const AudioSampleInstanceHandle& audioSampleInstanceHandle) = 0;
virtual void play(const AudioSceneHandle& audioSceneHandle, const AudioSample3dInstanceHandle& audioSample3dInstanceHandle) = 0;
virtual void stop(const AudioSceneHandle& audioSceneHandle, const AudioSampleInstanceHandle& audioSampleInstanceHandle) = 0;
virtual void stop(const AudioSceneHandle& audioSceneHandle, const AudioSample3dInstanceHandle& audioSample3dInstanceHandle) = 0;

virtual void loop(const AudioSceneHandle& audioSceneHandle, const AudioSampleInstanceHandle& audioSampleInstanceHandle, const bool loop) = 0;
virtual bool loop(const AudioSceneHandle& audioSceneHandle, const AudioSampleInstanceHandle& audioSampleInstanceHandle) = 0;
virtual void near(const AudioSceneHandle& audioSceneHandle, const AudioSampleInstanceHandle& audioSampleInstanceHandle, const float32 near) = 0;
virtual float32 near(const AudioSceneHandle& audioSceneHandle, const AudioSampleInstanceHandle& audioSampleInstanceHandle) = 0;
virtual void far(const AudioSceneHandle& audioSceneHandle, const AudioSampleInstanceHandle& audioSampleInstanceHandle, const float32 far) = 0;
virtual float32 far(const AudioSceneHandle& audioSceneHandle, const AudioSampleInstanceHandle& audioSampleInstanceHandle) = 0;
virtual void frequency(const AudioSceneHandle& audioSceneHandle, const AudioSampleInstanceHandle& audioSampleInstanceHandle, const float32 frequency) = 0;
virtual float32 frequency(const AudioSceneHandle& audioSceneHandle, const AudioSampleInstanceHandle& audioSampleInstanceHandle) = 0;
virtual void position(const AudioSceneHandle& audioSceneHandle, const AudioSampleInstanceHandle& audioSampleInstanceHandle, const glm::vec3& position) = 0;
virtual glm::vec3 position(const AudioSceneHandle& audioSceneHandle, const AudioSampleInstanceHandle& audioSampleInstanceHandle) = 0;



/* AUDIO SCENE STUFF */
virtual AudioSceneHandle createAudioScene() = 0;
virtual void destroyAudioScene(const AudioSceneHandle& audioSceneHandle) = 0;

virtual void setListenerPositionAndOrientation(const AudioSceneHandle& audioSceneHandle, const glm::vec3& position, const glm::quat& orientation) = 0;
virtual glm::vec3 getListenerPosition(const AudioSceneHandle& audioSceneHandle) const = 0;
virtual glm::quat getListenerOrientation(const AudioSceneHandle& audioSceneHandle) const = 0;



/* AUDIO SAMPLE STUFF */
virtual AudioSampleHandle load(const AudioSample& audioSample, const std::string& name = std::string()) = 0;
virtual void unload(const std::string& name) = 0;
virtual void unload(const AudioSampleHandle& audioSampleHandle) = 0;
virtual void unloadAll() = 0;
virtual AudioSampleHandle getAudioSample(const std::string& name) const = 0;
