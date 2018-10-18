#include <string>

#include <SDL2/SDL_audio.h>

#include "Audio.hpp"

namespace ice_engine
{

void Audio::importAudio(const std::vector<uint8>& data)
{
	uint8* buffer = nullptr;

	try
	{
		SDL_AudioSpec spec;

		SDL_RWops* rwOps = SDL_RWFromConstMem(&data[0], data.size());

		auto result = SDL_LoadWAV_RW(rwOps, 1, &spec, &buffer, &this->length_);
		if (!result) throw std::runtime_error(std::string("Unable to load audio file: ") + SDL_GetError());

		this->buffer_ = std::vector<uint8>(buffer, buffer + this->length_);
		this->frequency_ = spec.freq;
		this->channels_ = spec.channels;//getNumChannels(spec.format);
		this->bitsPerSample_ = SDL_AUDIO_BITSIZE(spec.format);

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
}

}
