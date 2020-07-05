#ifndef PBRMATERIAL_H_
#define PBRMATERIAL_H_

#include "IImage.hpp"

#include "graphics/IPbrMaterial.hpp"

namespace ice_engine
{

class PbrMaterial : public graphics::IPbrMaterial
{
public:

	PbrMaterial(IImage* albedo = nullptr, IImage* normal = nullptr, IImage* metalness = nullptr, IImage* roughness = nullptr, IImage* ambientOcclusion = nullptr)
	:
		albedo_(albedo),
		normal_(normal),
		metalness_(metalness),
		roughness_(roughness),
		ambientOcclusion_(ambientOcclusion)
	{

	}

	~PbrMaterial() override = default;

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
	IImage* albedo_ = nullptr;
	IImage* normal_ = nullptr;
	IImage* metalness_ = nullptr;
	IImage* roughness_ = nullptr;
	IImage* ambientOcclusion_ = nullptr;
};

}

#endif /* PBRMATERIAL_H_ */
