#include <string>

#include "Platform.hpp"
#include "Types.hpp"

#include "audio/IAudioEngine.hpp"

#include "AudioEngineBindingDelegate.hpp"
#include "BindingDelegateUtilities.hpp"

#include "scripting/IScriptingEngine.hpp"

#include "GameEngine.hpp"

namespace hercules
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
}
	
};
