#ifndef CAMERA_ID_H_
#define CAMERA_ID_H_

#include <iostream>

#include "Types.hpp"

namespace hercules
{
namespace graphics
{

class CameraId
{
public:
	CameraId();
	explicit CameraId(int32 id);
	// We make the destructor non-virtual so that we can't extend this class easily
	~CameraId();
	
	int32 getId() const;
	
	bool operator == (const CameraId &other) const;
	bool operator != (const CameraId &other) const;
	bool operator < (const CameraId &other) const;
	bool operator > (const CameraId &other) const;
	bool operator <= (const CameraId &other) const;
	bool operator >= (const CameraId &other) const;
	bool operator << (const CameraId &other) const;
	
	friend std::ostream& operator << (std::ostream& os, const CameraId& id);
	
	static const CameraId INVALID;

private:
	int32 id_;
};

}
}

#endif /* CAMERA_ID_H_ */
