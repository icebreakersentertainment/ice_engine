#include <SDL2/SDL.h>

#include "audio/openal/AudioEngine.hpp"

namespace ice_engine
{
namespace audio
{
namespace openal
{

ALuint source = 0;
ALuint buffer = 0;

AudioEngine::AudioEngine(utilities::Properties* properties, fs::IFileSystem* fileSystem, logger::ILogger* logger)
	:
	properties_(properties),
	fileSystem_(fileSystem),
	logger_(logger)
{
	device_ = alcOpenDevice(nullptr);
	
	if (!device_)
	{
		auto message = std::string("Unable to initialize OpenAL");
		throw std::runtime_error(message);
	}
	
	ALboolean enumeration;
	
	enumeration = alcIsExtensionPresent(nullptr, "ALC_ENUMERATION_EXT");
	if (enumeration == AL_FALSE)
	{
        logger_->warn("OpenAL enumeration not supported");
	}
	else
	{
        logger_->info("OpenAL enumeration supported");
	}
	
	context_ = alcCreateContext(device_, nullptr);
	
	if (!context_)
	{
		auto message = std::string("Unable to create OpenAL context");
		throw std::runtime_error(message);
	}
	
	{
		auto result = alcMakeContextCurrent(context_);
		
		if (!result)
		{
			auto message = std::string("Unable to make OpenAL context current");
			throw std::runtime_error(message);
		}
	}
	
	ASSERT_AL_ERROR(__FILE__, __LINE__);
	/*
	alListener3f(AL_POSITION, 0, 0, 1.0f);
	alListener3f(AL_VELOCITY, 0, 0, 0);
	ALfloat listenerOri[] = { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f };
	alListenerfv(AL_ORIENTATION, listenerOri);
	
	ASSERT_AL_ERROR(__FILE__, __LINE__);
	
	// Test source
	alGenSources((ALuint)1, &source);
	// check for errors
	
	alSourcef(source, AL_PITCH, 1);
	// check for errors
	alSourcef(source, AL_GAIN, 1);
	// check for errors
	alSource3f(source, AL_POSITION, 0, 0, 0);
	// check for errors
	alSource3f(source, AL_VELOCITY, 0, 0, 0);
	// check for errors
	alSourcei(source, AL_LOOPING, AL_FALSE);
	// check for errros
	
	ASSERT_AL_ERROR(__FILE__, __LINE__);
	
	alGenBuffers((ALuint)1, &buffer);
	
	ASSERT_AL_ERROR(__FILE__, __LINE__);
	
	auto wav = loadWav("C:\\Users\\jarrett\\projects\\icebreakersentertainment\\dark_horizon\\assets\\sounds\\0614.wav");
	
	std::cout << "Device: " << alcGetString(device_, ALC_DEVICE_SPECIFIER) << std::endl;
	std::cout << wav->length << " " << wav->frequency << " ch:" << std::to_string(wav->channels) << " sa:" << wav->bitsPerSample << std::endl;
	alBufferData(buffer, AL_FORMAT_MONO16, wav->buffer, wav->length, wav->frequency);
	
	ASSERT_AL_ERROR(__FILE__, __LINE__);
	
	alSourcei(source, AL_BUFFER, buffer);
	ASSERT_AL_ERROR(__FILE__, __LINE__);
	
	alSourcePlay(source);
	
	ASSERT_AL_ERROR(__FILE__, __LINE__);
	*/
}

AudioEngine::AudioEngine(const AudioEngine& other)
{
}

AudioEngine::~AudioEngine()
{
	alcMakeContextCurrent(nullptr);
	alcDestroyContext(context_);
	alcCloseDevice(device_);
}

AudioSceneHandle AudioEngine::createAudioScene()
{
	return audioSceneHandles_.create();
}

void AudioEngine::destroyAudioScene(const AudioSceneHandle& audioSceneHandle)
{
	
}

void AudioEngine::beginRender()
{
	
}

void AudioEngine::render(const AudioSceneHandle& audioSceneHandle)
{
	auto& audioScene = audioSceneHandles_[audioSceneHandle];
	
	alListener3f(AL_POSITION, 0, 0, 1.0f);
	alListener3f(AL_VELOCITY, 0, 0, 0);
	ALfloat listenerOri[] = { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f };
	alListenerfv(AL_ORIENTATION, listenerOri);
	
	ASSERT_AL_ERROR(__FILE__, __LINE__);
	
	std::vector<SoundSourceHandle> stopped;
	
	for (auto it = audioScene.soundSources.begin(); it != audioScene.soundSources.end(); it++)
	{
		ALint value = 0;
		alGetSourcei(it->source, AL_SOURCE_STATE, &value);
		
		if (value == AL_STOPPED)
		{
			alDeleteSources(1, &it->source);
			ASSERT_AL_ERROR(__FILE__, __LINE__);
			
			stopped.push_back(it.handle());
			
		}
	}
	
	for (auto& h : stopped)
	{
		audioScene.soundSources.destroy(h);
	}
}

void AudioEngine::endRender()
{
	
}

SoundSourceHandle AudioEngine::play(const AudioSceneHandle& audioSceneHandle, const SoundHandle& soundHandle, const glm::vec3& position)
{
	auto buffer = soundHandles_[soundHandle];
	
	auto handle = audioSceneHandles_[audioSceneHandle].soundSources.create();
	auto& soundSource = audioSceneHandles_[audioSceneHandle].soundSources[handle];
	
	alGenSources(1, &soundSource.source);
	
	ASSERT_AL_ERROR(__FILE__, __LINE__);
	
	alSourcef(soundSource.source, AL_PITCH, 1);
	alSourcef(soundSource.source, AL_GAIN, 1);
	alSource3f(soundSource.source, AL_POSITION, position.x, position.y, position.z);
	alSource3f(soundSource.source, AL_VELOCITY, 0, 0, 0);
	alSourcei(soundSource.source, AL_LOOPING, AL_FALSE);
	
	ASSERT_AL_ERROR(__FILE__, __LINE__);
	
	soundSource.buffer = buffer;
	
	alSourcei(soundSource.source, AL_BUFFER, soundSource.buffer);
	ASSERT_AL_ERROR(__FILE__, __LINE__);
	
	alSourcePlay(soundSource.source);
	
	return handle;
}

void AudioEngine::stop(const AudioSceneHandle& audioSceneHandle, const SoundSourceHandle& soundSourceHandle)
{
	
}

void AudioEngine::stopAll(const AudioSceneHandle& audioSceneHandle)
{
	
}

SoundHandle AudioEngine::createSound(const IAudio* audio)
{
	auto handle = soundHandles_.create();
	auto& buffer = soundHandles_[handle];
	
	alGenBuffers(1, &buffer);
	
	ASSERT_AL_ERROR(__FILE__, __LINE__);
	
	//std::cout << "Device: " << alcGetString(device_, ALC_DEVICE_SPECIFIER) << std::endl;
	
	alBufferData(buffer, AL_FORMAT_MONO16, audio->data().data(), audio->length(), audio->frequency());
	
	ASSERT_AL_ERROR(__FILE__, __LINE__);
	
	return handle;
}

ListenerHandle AudioEngine::createListener(const AudioSceneHandle& audioSceneHandle, const glm::vec3& position)
{
	return audioSceneHandles_[audioSceneHandle].listenerHandle;
}

void AudioEngine::setPosition(const AudioSceneHandle& audioSceneHandle, const SoundSourceHandle& soundSourceHandle, const float32 x, const float32 y, const float32 z)
{
	
}

void AudioEngine::setPosition(const AudioSceneHandle& audioSceneHandle, const SoundSourceHandle& soundSourceHandle, const glm::vec3& position)
{
	
}

glm::vec3 AudioEngine::position(const AudioSceneHandle& audioSceneHandle, const SoundSourceHandle& soundSourceHandle) const
{
	return glm::vec3();
}

void AudioEngine::setPosition(const AudioSceneHandle& audioSceneHandle, const ListenerHandle& listenerHandle, const float32 x, const float32 y, const float32 z)
{
	
}

void AudioEngine::setPosition(const AudioSceneHandle& audioSceneHandle, const ListenerHandle& listenerHandle, const glm::vec3& position)
{
	
}

glm::vec3 AudioEngine::position(const AudioSceneHandle& audioSceneHandle, const ListenerHandle& listenerHandle) const
{
	return glm::vec3();
}

}
}
}
