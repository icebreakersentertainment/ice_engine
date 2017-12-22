#ifndef AUDIOENGINEBINDINGDELEGATE_H_
#define AUDIOENGINEBINDINGDELEGATE_H_

namespace ice_engine
{

namespace scripting
{
class IScriptingEngine;
}
namespace audio
{
class IAudioEngine;
}
namespace logger
{
class ILogger;
}

class GameEngine;

class AudioEngineBindingDelegate
{
public:
	AudioEngineBindingDelegate(logger::ILogger* logger, scripting::IScriptingEngine* scriptingEngine, GameEngine* gameEngine, audio::IAudioEngine* audioEngine);
	~AudioEngineBindingDelegate() = default;

	void bind();
	
private:
	logger::ILogger* logger_;
	GameEngine* gameEngine_;
	scripting::IScriptingEngine* scriptingEngine_;
	audio::IAudioEngine* audioEngine_;
};

}

#endif /* AUDIOENGINEBINDINGDELEGATE_H_ */

