#ifndef AUDIO_H_
#define AUDIO_H_

#include <SDL2/SDL_Audio.h>

#include "audio/IAudio.hpp"

namespace ice_engine
{

class Audio : public audio::IAudio
{
public:
	/**
	 * Will load the provided audio data into a proper audio.
	 *
	 * @param data The audio data to load.
	 */
	Audio(const std::vector<uint8>& data)
	{
		importAudio(data);
	};

	/**
	 * Will load the provided audio data into a proper audio.
	 *
	 * @param inputStream Stream from which we will load our audio data.
	 */
	Audio(std::istream& inputStream)
	{
		std::vector<uint8> data((std::istreambuf_iterator<char>(inputStream)), std::istreambuf_iterator<char>());

		importAudio(data);
	}

	virtual ~Audio() {};
	
	virtual const std::vector<uint8>& data() const override
	{
		return buffer_;
	}

	virtual uint32 length() const override
	{
		return length_;
	}

	virtual uint32 frequency() const override
	{
		return frequency_;
	}

	virtual uint8 channels() const override
	{
		return channels_;
	}

	virtual uint16 bitsPerSample() const override
	{
		return bitsPerSample_;
	}

private:
	uint32 length_ = 0;
	std::vector<uint8> buffer_;
	int frequency_ = 0;
	uint8 channels_ = 0;
	uint16 bitsPerSample_ = 0;

	void importAudio(const std::vector<uint8>& data)
	{
		uint8* buffer = nullptr;

		try
		{
			SDL_AudioSpec spec;

			SDL_RWops* rwOps = SDL_RWFromConstMem(&data[0], data.size());

			auto result = SDL_LoadWAV_RW(rwOps, 1, &spec, &buffer, &this->length_);
			if (!result)
			{
				auto message = std::string("Unable to load audio file: ") + SDL_GetError();
				throw std::runtime_error(message);
			}

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
};

}

#endif /* AUDIO_H_ */
