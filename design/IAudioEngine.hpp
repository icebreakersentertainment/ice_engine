class AudioSampleHandle
{
};
	
virtual void play(const std::string& filename) const = 0;
virtual AudioSampleHandle load(const std::string& filename, const std::string& name = std::string()) = 0;
virtual void unload(const std::string& name) = 0;
virtual void unload(const AudioSampleHandle& audioSampleHandle) = 0;
virtual void unloadAll() = 0;
virtual void reload(const std::string& name) = 0;
virtual void reload(const AudioSampleHandle& audioSampleHandle) = 0;
virtual void reloadAll() = 0;
virtual AudioSampleHandle getAudioSample(const std::string& name) const = 0;
