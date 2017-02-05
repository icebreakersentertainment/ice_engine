#ifndef RENDERABLE_ID_H_
#define RENDERABLE_ID_H_

#include <iostream>

#include "Types.hpp"

namespace hercules
{
namespace graphics
{

class RenderableId
{
public:
	RenderableId();
	explicit RenderableId(int32 id);
	// We make the destructor non-virtual so that we can't extend this class easily
	~RenderableId();
	
	int32 getId() const;
	
	bool operator == (const RenderableId &other) const;
	bool operator != (const RenderableId &other) const;
	bool operator < (const RenderableId &other) const;
	bool operator > (const RenderableId &other) const;
	bool operator <= (const RenderableId &other) const;
	bool operator >= (const RenderableId &other) const;
	bool operator << (const RenderableId &other) const;
	
	friend std::ostream& operator << (std::ostream& os, const RenderableId& id);
	
	static const RenderableId INVALID;

private:
	int32 id_;
};

}

}

#endif /* RENDERABLE_ID_H_ */
