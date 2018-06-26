#ifndef PBRMATERIAL_H_
#define PBRMATERIAL_H_

#include "Image.hpp"

#include "graphics/IPbrMaterial.hpp"

namespace ice_engine
{

class PbrMaterial : public graphics::IPbrMaterial
{
public:

	PbrMaterial(Image* albedo = nullptr, Image* normal = nullptr, Image* metalness = nullptr, Image* roughness = nullptr, Image* ambientOcclusion = nullptr)
	:
		albedo_(albedo),
		normal_(normal),
		metalness_(metalness),
		roughness_(roughness),
		ambientOcclusion_(ambientOcclusion)
	{

	}

	virtual ~PbrMaterial() = default;

	const graphics::IImage* albedo() const override
	{
		return albedo_;
	}

	const graphics::IImage* normal() const override
	{
		return normal_;
	}

	const graphics::IImage* metalness() const override
	{
		return metalness_;
	}

	const graphics::IImage* roughness() const override
	{
		return roughness_;
	}

	const graphics::IImage* ambientOcclusion() const override
	{
		return ambientOcclusion_;
	}

private:
	Image* albedo_ = nullptr;
	Image* normal_ = nullptr;
	Image* metalness_ = nullptr;
	Image* roughness_ = nullptr;
	Image* ambientOcclusion_ = nullptr;
};

}

#endif /* PBRMATERIAL_H_ */
