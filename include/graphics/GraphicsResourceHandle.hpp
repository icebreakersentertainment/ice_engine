#ifndef GRAPHICS_RESOURCE_HANDLE_H_
#define GRAPHICS_RESOURCE_HANDLE_H_

#include <iostream>

#include "Types.hpp"

namespace hercules
{
namespace graphics
{

class GraphicsResourceHandle
{
public:
	GraphicsResourceHandle() : id_(-1)
	{
	}

	explicit GraphicsResourceHandle(int32 id) : id_(id)
	{
	}

	virtual ~GraphicsResourceHandle()
	{
	}
	
	int32 getId() const
	{
		return id_;
	} 
	
	bool operator == (const GraphicsResourceHandle &other) const
	{
		return id_ == other.id_;
	}

	bool operator != (const GraphicsResourceHandle &other) const
	{
		return id_ != other.id_;
	}

	bool operator < (const GraphicsResourceHandle &other) const
	{
		return id_ < other.id_;
	}

	bool operator > (const GraphicsResourceHandle &other) const
	{
		return other.id_ < id_;
	}

	bool operator <= (const GraphicsResourceHandle &other) const
	{
		return !(id_ > other.id_);
	}

	bool operator >= (const GraphicsResourceHandle &other) const
	{
		return !(id_ < other.id_);
	}
/*
	bool operator << (const GraphicsResourceHandle &other) const;
	
	friend std::ostream& operator << (std::ostream& os, const GraphicsResourceHandle& id);
*/
private:
	int32 id_;
};

/*
std::ostream& operator << (std::ostream& os, const GraphicsResourceHandle& id)
{
	os << id.id_;
	return os;
}
*/

}

}

#endif /* GRAPHICS_RESOURCE_HANDLE_H_ */
