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
	virtual ~IAudio()
	{
	}
	;
	
	virtual const std::vector<uint8>& data() const = 0;
	virtual uint32 length() const = 0;
	virtual uint32 frequency() const = 0;
};

}
}

#endif /* IAUDIO_H_ */
