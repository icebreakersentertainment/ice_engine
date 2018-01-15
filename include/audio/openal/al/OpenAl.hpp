#ifndef OPENAL_H_
#define OPENAL_H_

#include <string>
#include <stdexcept>

#include <AL/al.h>

namespace ice_engine
{
namespace audio
{
namespace openal
{
namespace al
{

#define ASSERT_AL_ERROR(filename, line) checkAlError(filename, line);

inline void checkAlError(const std::string& filename, const int line)
{
	ALenum err(alGetError());

	if (err != AL_NO_ERROR)
	{
		std::string error;

		switch (err)
		{
			case AL_INVALID_NAME:	  				error = "INVALID_NAME";						break;
			case AL_INVALID_OPERATION:	  			error = "INVALID_OPERATION";				break;
			case AL_INVALID_ENUM:		  			error = "INVALID_ENUM";		  				break;
			case AL_INVALID_VALUE:		  			error = "INVALID_VALUE";		  			break;
			case AL_OUT_OF_MEMORY:		 			error = "OUT_OF_MEMORY";		  			break;
		}

		throw std::runtime_error(filename + " (" + std::to_string(line) + "): " + std::string("AL_") + error);
	}
}

inline void checkAlError()
{
	ALenum err(alGetError());

	if (err != AL_NO_ERROR)
	{
		std::string error;

		switch (err)
		{
			case AL_INVALID_NAME:	  				error = "INVALID_NAME";						break;
			case AL_INVALID_OPERATION:	  			error = "INVALID_OPERATION";				break;
			case AL_INVALID_ENUM:		  			error = "INVALID_ENUM";		  				break;
			case AL_INVALID_VALUE:		  			error = "INVALID_VALUE";		  			break;
			case AL_OUT_OF_MEMORY:		 			error = "OUT_OF_MEMORY";		  			break;
		}

		throw std::runtime_error(std::string("AL_") + error);
	}
}

}
}
}
}

#endif /* OPENAL_H_ */
