#ifndef SKELETON_ID_H_
#define SKELETON_ID_H_

#include <iostream>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

namespace graphics
{

class SkeletonId
{
public:
	SkeletonId();
	explicit SkeletonId(glm::detail::int32 id);
	// We make the destructor non-virtual so that we can't extend this class easily
	~SkeletonId();
	
	glm::detail::int32 getId() const;
	
	bool operator == (const SkeletonId &other) const;
	bool operator != (const SkeletonId &other) const;
	bool operator < (const SkeletonId &other) const;
	bool operator > (const SkeletonId &other) const;
	bool operator <= (const SkeletonId &other) const;
	bool operator >= (const SkeletonId &other) const;
	bool operator << (const SkeletonId &other) const;
	
	friend std::ostream& operator << (std::ostream& os, const SkeletonId& id);
	
	static const SkeletonId INVALID;

private:
	glm::detail::int32 id_;
};

}

#endif /* SKELETON_ID_H_ */
