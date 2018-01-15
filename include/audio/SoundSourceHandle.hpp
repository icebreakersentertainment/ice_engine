#ifndef SOUND_SOURCE_HANDLE_H_
#define SOUND_SOURCE_HANDLE_H_

#include "handles/Handle.hpp"

namespace ice_engine
{
namespace audio
{

class SoundSourceHandle : public handles::Handle<SoundSourceHandle>
{
public:
	using handles::Handle<SoundSourceHandle>::Handle;
};

}

}

#endif /* SOUND_SOURCE_HANDLE_H_ */
