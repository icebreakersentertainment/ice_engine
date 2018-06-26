#ifndef IPBRMATERIAL_H_
#define IPBRMATERIAL_H_

#include "graphics/IImage.hpp"

#include "Types.hpp"

namespace ice_engine
{
namespace graphics
{

class IPbrMaterial
{
public:
	virtual ~IPbrMaterial()
	{
	}
	;
	
	virtual const IImage* albedo() const = 0;
	virtual const IImage* normal() const = 0;
	virtual const IImage* metalness() const = 0;
	virtual const IImage* roughness() const = 0;
	virtual const IImage* ambientOcclusion() const = 0;
};

}
}

#endif /* IPBRMATERIAL_H_ */
