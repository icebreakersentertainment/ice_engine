#ifndef MESH_ID_H_
#define MESH_ID_H_

#include <iostream>

#include "Types.hpp"

namespace hercules
{
namespace graphics
{

class MeshId
{
public:
	MeshId();
	explicit MeshId(int32 id);
	// We make the destructor non-virtual so that we can't extend this class easily
	~MeshId();
	
	int32 getId() const;
	
	bool operator == (const MeshId &other) const;
	bool operator != (const MeshId &other) const;
	bool operator < (const MeshId &other) const;
	bool operator > (const MeshId &other) const;
	bool operator <= (const MeshId &other) const;
	bool operator >= (const MeshId &other) const;
	bool operator << (const MeshId &other) const;
	
	friend std::ostream& operator << (std::ostream& os, const MeshId& id);
	
	static const MeshId INVALID;

private:
	int32 id_;
};

}

}

#endif /* MESH_ID_H_ */
