#include <string>
#include <iostream>

#include <SDL2/SDL_audio.h>

#include "Audio.hpp"

namespace ice_engine
{
namespace
{
int32 convertSdlFormat(const SDL_AudioFormat format)
{
    switch (format)
    {
        case AUDIO_S8:
            return audio::IAudio::FORMAT_AUDIO_S8;

        case AUDIO_U8:
            return audio::IAudio::FORMAT_AUDIO_U8;


        case AUDIO_S16LSB:
            return audio::IAudio::FORMAT_AUDIO_S16LSB;

        case AUDIO_S16MSB:
            return audio::IAudio::FORMAT_AUDIO_S16MSB;

//        case AUDIO_S16SYS:
//			return audio::IAudio::FORMAT_AUDIO_S16SYS;

//        case AUDIO_S16:
//			return audio::IAudio::FORMAT_AUDIO_S16;


        case AUDIO_U16LSB:
            return audio::IAudio::FORMAT_AUDIO_U16LSB;

        case AUDIO_U16MSB:
            return audio::IAudio::FORMAT_AUDIO_U16MSB;

//        case AUDIO_U16SYS:
//			return audio::IAudio::FORMAT_AUDIO_U16SYS;
//
//        case AUDIO_U16:
//			return audio::IAudio::FORMAT_AUDIO_U16;


        case AUDIO_S32LSB:
            return audio::IAudio::FORMAT_AUDIO_S32LSB;

        case AUDIO_S32MSB:
            return audio::IAudio::FORMAT_AUDIO_S32MSB;

//        case AUDIO_S32SYS:
//			return audio::IAudio::FORMAT_AUDIO_S32SYS;
//
//        case AUDIO_S32:
//			return audio::IAudio::FORMAT_AUDIO_S32;


        case AUDIO_F32LSB:
            return audio::IAudio::FORMAT_AUDIO_F32LSB;

        case AUDIO_F32MSB:
            return audio::IAudio::FORMAT_AUDIO_F32MSB;

//        case AUDIO_F32SYS:
//			return audio::IAudio::FORMAT_AUDIO_F32SYS;
//
//        case AUDIO_F32:
//            return audio::IAudio::FORMAT_AUDIO_F32;

        default:
            return audio::IAudio::FORMAT_UNKNOWN;
    }
}
}

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
		this->format_ = convertSdlFormat(spec.format);

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
