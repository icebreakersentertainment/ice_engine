#ifndef IAUDIO_H_
#define IAUDIO_H_

#include <vector>

#include "Types.hpp"

namespace ice_engine
{
namespace audio
{

class IAudio
{
public:
    enum Format
    {
        FORMAT_UNKNOWN = -1,

        FORMAT_AUDIO_S8,
        FORMAT_AUDIO_U8,

        FORMAT_AUDIO_S16LSB,
        FORMAT_AUDIO_S16MSB,
        FORMAT_AUDIO_S16SYS,
        FORMAT_AUDIO_S16,

        FORMAT_AUDIO_U16LSB,
        FORMAT_AUDIO_U16MSB,
        FORMAT_AUDIO_U16SYS,
        FORMAT_AUDIO_U16,

        FORMAT_AUDIO_S32LSB,
        FORMAT_AUDIO_S32MSB,
        FORMAT_AUDIO_S32SYS,
        FORMAT_AUDIO_S32,

        FORMAT_AUDIO_F32LSB,
        FORMAT_AUDIO_F32MSB,
        FORMAT_AUDIO_F32SYS,
        FORMAT_AUDIO_F32
    };

	virtual ~IAudio() = default;
	
	virtual const std::vector<uint8>& data() const = 0;
	virtual uint32 length() const = 0;
	virtual uint32 frequency() const = 0;
	virtual uint8 channels() const = 0;
	virtual uint16 bitsPerSample() const = 0;
    virtual int32 format() const = 0;
};

}
}

#endif /* IAUDIO_H_ */
