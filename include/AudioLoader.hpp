#ifndef AUDIOLOADER_H_
#define AUDIOLOADER_H_

#include <SDL2/SDL.h>

#include "Audio.hpp"

namespace ice_engine
{

/**
 * Will load the provided audio data into a proper audio.
 * 
 * @param data The audio data to load.
 */
inline std::unique_ptr<Audio> importAudio(const std::vector<uint8>& data)
{
	uint8* buffer = nullptr;
	
	auto audio = std::make_unique<Audio>();
	
	try
	{
		SDL_AudioSpec spec;
		
		SDL_RWops* rwOps = SDL_RWFromConstMem(&data[0], data.size());

		auto result = SDL_LoadWAV_RW(rwOps, 1, &spec, &buffer, &audio->length_);
		if (!result)
		{
			auto message = std::string("Unable to load audio file: ") + SDL_GetError();
			throw std::runtime_error(message);
		}
		
		audio->buffer_ = std::vector<uint8>(buffer, buffer + audio->length_);
		audio->frequency_ = spec.freq;
		audio->channels_ = spec.channels;//getNumChannels(spec.format);
		audio->bitsPerSample_ = SDL_AUDIO_BITSIZE(spec.format);
		
		SDL_FreeWAV(buffer);
	}
	catch (const std::exception& e)
	{
		if (buffer != nullptr)
		{
			SDL_FreeWAV(buffer);
		}
		
		throw e;
	}

	return std::move(audio);
}

/**
 * Will load the provided audio data into a proper audio.
 * 
 * @param inputStream Stream from which we will load our audio data.
 */
inline std::unique_ptr<Audio> importAudio(std::istream& inputStream)
{
	std::vector<uint8> data((std::istreambuf_iterator<char>(inputStream)), std::istreambuf_iterator<char>());
	
	return importAudio(data);
}

}

#endif /* AUDIOLOADER_H_ */
