#ifndef AUDIO_H_
#define AUDIO_H_

#include <iterator>
#include <istream>

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
	}

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

	~Audio() override = default;
	
	const std::vector<uint8>& data() const override
	{
		return buffer_;
	}

	uint32 length() const override
	{
		return length_;
	}

	uint32 frequency() const override
	{
		return frequency_;
	}

	uint8 channels() const override
	{
		return channels_;
	}

	uint16 bitsPerSample() const override
	{
		return bitsPerSample_;
	}

    int32 format() const override
    {
        return format_;
    }

private:
	uint32 length_ = 0;
	std::vector<uint8> buffer_;
	int frequency_ = 0;
	uint8 channels_ = 0;
	uint16 bitsPerSample_ = 0;
    int32 format_ = Format::FORMAT_UNKNOWN;

	void importAudio(const std::vector<uint8>& data);
};

}

#endif /* AUDIO_H_ */
