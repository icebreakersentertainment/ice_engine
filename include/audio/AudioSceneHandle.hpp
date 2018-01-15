#ifndef AUDIO_SCENE_HANDLE_H_
#define AUDIO_SCENE_HANDLE_H_

#include "handles/Handle.hpp"

namespace ice_engine
{
namespace audio
{

class AudioSceneHandle : public handles::Handle<AudioSceneHandle>
{
public:
	using handles::Handle<AudioSceneHandle>::Handle;
};

}

}

#endif /* AUDIO_SCENE_HANDLE_H_ */
