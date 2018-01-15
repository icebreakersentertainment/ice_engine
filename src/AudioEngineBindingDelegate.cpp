#include "Platform.hpp"
#include "Types.hpp"

#include "audio/IAudioEngine.hpp"

#include "AudioEngineBindingDelegate.hpp"
#include "BindingDelegateUtilities.hpp"

#include "scripting/IScriptingEngine.hpp"

#include "GameEngine.hpp"

namespace ice_engine
{

AudioEngineBindingDelegate::AudioEngineBindingDelegate(logger::ILogger* logger, scripting::IScriptingEngine* scriptingEngine, GameEngine* gameEngine, audio::IAudioEngine* audioEngine)
	:
	logger_(logger),
	scriptingEngine_(scriptingEngine),
	gameEngine_(gameEngine),
	audioEngine_(audioEngine)
{
}

void AudioEngineBindingDelegate::bind()
{
	registerHandleBindings<audio::AudioSceneHandle>(scriptingEngine_, "AudioSceneHandle");
	registerHandleBindings<audio::SoundHandle>(scriptingEngine_, "SoundHandle");
	registerHandleBindings<audio::SoundSourceHandle>(scriptingEngine_, "SoundSourceHandle");
	registerHandleBindings<audio::ListenerHandle>(scriptingEngine_, "ListenerHandle");

	scriptingEngine_->registerObjectType("IAudio", 0, asOBJ_REF | asOBJ_NOCOUNT);
	
	//registerSharedFutureBindings<audio::IAudio*>(scriptingEngine_, "shared_futureIAudio", "IAudio@");
	
	// IAudioEngine
	scriptingEngine_->registerObjectType("IAudioEngine", 0, asOBJ_REF | asOBJ_NOCOUNT);
	scriptingEngine_->registerGlobalProperty("IAudioEngine audio", audioEngine_);
	scriptingEngine_->registerClassMethod(
		"IAudioEngine",
		"SoundHandle createSound(const IAudio@)",
		asMETHODPR(audio::IAudioEngine, createSound, (const audio::IAudio*), audio::SoundHandle)
	);
}
	
};
