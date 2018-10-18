#ifndef IAUDIOPLUGIN_H_
#define IAUDIOPLUGIN_H_

#include <memory>

#include "audio/IAudioEngineFactory.hpp"

#include "IPlugin.hpp"

namespace ice_engine
{

class IAudioPlugin : public virtual IPlugin
{
public:
	virtual ~IAudioPlugin()
	{
	}
	;

	virtual std::unique_ptr<audio::IAudioEngineFactory> createFactory() const = 0;

};

}

#endif /* IAUDIOPLUGIN_H_ */
