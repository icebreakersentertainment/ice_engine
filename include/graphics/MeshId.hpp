#ifndef MESH_ID_H_
#define MESH_ID_H_

#include <iostream>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

namespace graphics
{

class MeshId
{
public:
	MeshId();
	explicit MeshId(glm::detail::int32 id);
	// We make the destructor non-virtual so that we can't extend this class easily
	~MeshId();
	
	glm::detail::int32 getId() const;
	
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
	glm::detail::int32 id_;
};

}

#endif /* MESH_ID_H_ */
