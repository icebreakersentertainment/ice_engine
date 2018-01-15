#ifndef IAUDIOENGINEFACTORY_H_
#define IAUDIOENGINEFACTORY_H_

#include <memory>

#include "IAudioEngine.hpp"

#include "utilities/Properties.hpp"
#include "fs/IFileSystem.hpp"
#include "logger/ILogger.hpp"

namespace ice_engine
{
namespace audio
{

class IAudioEngineFactory
{
public:
	virtual ~IAudioEngineFactory()
	{
	}
	;
	
	virtual std::unique_ptr<IAudioEngine> create(
		utilities::Properties* properties,
		fs::IFileSystem* fileSystem,
		logger::ILogger* logger
	) = 0;
};

}
}

#endif /* IAUDIOENGINEFACTORY_H_ */
