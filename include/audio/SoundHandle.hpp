#ifndef SOUND_HANDLE_H_
#define SOUND_HANDLE_H_

#include "handles/Handle.hpp"

namespace ice_engine
{
namespace audio
{

class SoundHandle : public handles::Handle<SoundHandle>
{
public:
	using handles::Handle<SoundHandle>::Handle;
};

}

}

#endif /* SOUND_HANDLE_H_ */
