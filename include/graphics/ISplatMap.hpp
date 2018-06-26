#ifndef ISPLATMAP_H_
#define ISPLATMAP_H_

#include <vector>

#include "graphics/IPbrMaterial.hpp"
#include "graphics/IImage.hpp"

#include "Types.hpp"

namespace ice_engine
{
namespace graphics
{

class ISplatMap
{
public:
	virtual ~ISplatMap()
	{
	}
	;
	
	virtual const std::vector<IPbrMaterial*>& materialMap() const = 0;
	virtual const IImage* terrainMap() const = 0;
};

}
}

#endif /* ISPLATMAP_H_ */
