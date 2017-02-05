#ifndef SKELETON_ID_H_
#define SKELETON_ID_H_

#include <iostream>

#include "Types.hpp"

namespace hercules
{
namespace graphics
{

class SkeletonId
{
public:
	SkeletonId();
	explicit SkeletonId(int32 id);
	// We make the destructor non-virtual so that we can't extend this class easily
	~SkeletonId();
	
	int32 getId() const;
	
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
	int32 id_;
};

}

}

#endif /* SKELETON_ID_H_ */
