#ifndef AUDIO_H_
#define AUDIO_H_

#include "audio/IAudio.hpp"

namespace ice_engine
{

class Audio : public audio::IAudio
{
public:
	Audio() {};
	virtual ~Audio() {};
	
	uint32 length_ = 0;
	std::vector<uint8> buffer_;
	int frequency_ = 0;
	uint8 channels_ = 0;
	uint16 bitsPerSample_ = 0;
	
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
	
};

}

#endif /* AUDIO_H_ */
