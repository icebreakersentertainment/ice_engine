#ifndef CAMERA_ID_H_
#define CAMERA_ID_H_

#include <iostream>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

namespace graphics
{

class CameraId
{
public:
	CameraId();
	explicit CameraId(glm::detail::int32 id);
	// We make the destructor non-virtual so that we can't extend this class easily
	~CameraId();
	
	glm::detail::int32 getId() const;
	
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
	glm::detail::int32 id_;
};

}

#endif /* CAMERA_ID_H_ */
