#ifndef SPLATMAP_H_
#define SPLATMAP_H_

#include <vector>
#include <memory>

#include "PbrMaterial.hpp"
#include "graphics/ISplatMap.hpp"

namespace ice_engine
{

class SplatMap : public graphics::ISplatMap
{
public:

	SplatMap() = default;
	
	SplatMap(std::vector<PbrMaterial> materialMap, Image* terrainMap)
	:
		materialMap_(std::move(materialMap)),
		terrainMap_(terrainMap)
	{
		materialMapPointers_.clear();

		for (auto& material : materialMap_) materialMapPointers_.push_back(&material);
		/*
		for (auto material : materialMap_)
		{
			if (material.albedo) flipVertical(*static_cast<Image*>(material.albedo));
			if (material.normal) flipVertical(*static_cast<Image*>(material.normal));
			if (material.metalness) flipVertical(*static_cast<Image*>(material.metalness));
			if (material.roughness) flipVertical(*static_cast<Image*>(material.roughness));
			if (material.ambientOcclusion) flipVertical(*static_cast<Image*>(material.ambientOcclusion));
		}
		flipVertical(*static_cast<Image*>(terrainMap_.get()));
		*/
	}
/*
	SplatMap(const Image& heightMap, fs::IFileSystem* fileSystem)
	{
		//terrainMap_ = generateTerrainMap(heightMap, fileSystem);
		materialMap_ = generateMaterialMap(heightMap, fileSystem);
		terrainMap_ = generateTerrainMap(heightMap);

		materialMapPointers_.clear();

		for (auto& material : materialMap_) materialMapPointers_.push_back(&material);
*/
		/*
		for (auto material : materialMap_)
		{
			if (material.albedo) flipVertical(*static_cast<Image*>(material.albedo));
			if (material.normal) flipVertical(*static_cast<Image*>(material.normal));
			if (material.metalness) flipVertical(*static_cast<Image*>(material.metalness));
			if (material.roughness) flipVertical(*static_cast<Image*>(material.roughness));
			if (material.ambientOcclusion) flipVertical(*static_cast<Image*>(material.ambientOcclusion));
		}
		flipVertical(*static_cast<Image*>(terrainMap_.get()));
		*/
	/*}*/

	SplatMap(const SplatMap& other)
	{
		terrainMap_ = other.terrainMap_;
		materialMap_ = other.materialMap_;

		for (auto& material : materialMap_) materialMapPointers_.push_back(&material);
	}

	virtual ~SplatMap() = default;

	const std::vector<graphics::IPbrMaterial*>& materialMap() const override
	{
		return materialMapPointers_;
	}

	const graphics::IImage* terrainMap() const override
	{
		return terrainMap_;
	}


private:
	Image* terrainMap_;
	std::vector<PbrMaterial> materialMap_;
	std::vector<graphics::IPbrMaterial*> materialMapPointers_;

	void flipVertical(Image& image)
	{
		std::vector<char> data;
		data.resize(image.width()*image.height()*4);

		int j=data.size()-image.width()*4;
		for (int i=0; i < data.size(); i+=image.width()*4)
		{
			for (int k=0; k < image.width()*4; ++k)
			{
				data[i+k] = image.data()[j+k];
			}

			j-=image.width()*4;
		}

		image.data_ = std::move(data);
	}
//
//	std::unique_ptr<Image> generateTerrainMap(const Image& heightMap)
//	{
//		std::vector<char> data = std::vector<char>();
//		data.resize(heightMap.width()*heightMap.height()*4);
//		int width = heightMap.width();
//		int height = heightMap.height();
//		Image::Format format = Image::Format::FORMAT_RGBA;
//
//		for (int i=0; i < heightMap.data().size(); i+=4)
//		{
//			const unsigned char height = static_cast<unsigned char>(heightMap.data()[i+3]);
//
//			data[i] = 0;
//			data[i+1] = 1;
//			data[i+2] = 2;
//
//			if (height <= 30)
//			{
//				// 1111 0000
//				data[i+3] = 208 + 0;
//			}
//			else if (height <= 50)
//			{
//				// 1000 1000
//				data[i+3] = 128 + 8;
//			}
//			else if (height > 50 && height <= 80)
//			{
//				// 0000 1111
//				data[i+3] = 0 + 15;
//			}
//			else if (height > 80 && height < 100)
//			{
//				// 0000 1000
//				data[i+3] = 0 + 8;
//			}
//			else
//			{
//				// 0000 0000
//				data[i+3] = 0;
//			}
//		}
//
//		return std::make_unique<Image>(data, width, height, format);
//	}
/*
	std::vector<PbrMaterial> generateMaterialMap(const Image& heightMap, fs::IFileSystem* fileSystem)
	{
		std::vector<PbrMaterial> materialMap;

		{
			auto file1 = fileSystem->open("../assets/placeholders/textures/grass1-Unreal-Engine2/grass1-albedo3-small.png", fs::FileFlags::READ | fs::FileFlags::BINARY);
			auto file2 = fileSystem->open("../assets/placeholders/textures/grass1-Unreal-Engine2/grass1-normal2-small.png", fs::FileFlags::READ | fs::FileFlags::BINARY);
			auto file3 = fileSystem->open("../assets/placeholders/textures/grass1-Unreal-Engine2/grass1-rough-small.png", fs::FileFlags::READ | fs::FileFlags::BINARY);
			auto file4 = fileSystem->open("../assets/placeholders/textures/grass1-Unreal-Engine2/grass1-ao-small.png", fs::FileFlags::READ | fs::FileFlags::BINARY);

			PbrMaterial material(
				new Image(file1->getInputStream()),
				new Image(file2->getInputStream()),
				new Image(file3->getInputStream()),
				new Image(file4->getInputStream())
			);

			materialMap.push_back(material);
		}

		{
			auto file1 = fileSystem->open("../assets/placeholders/textures/grass1-Unreal-Engine2/grass1-albedo3-small.png", fs::FileFlags::READ | fs::FileFlags::BINARY);
			auto file2 = fileSystem->open("../assets/placeholders/textures/grass1-Unreal-Engine2/grass1-normal2-small.png", fs::FileFlags::READ | fs::FileFlags::BINARY);
			auto file3 = fileSystem->open("../assets/placeholders/textures/grass1-Unreal-Engine2/grass1-rough-small.png", fs::FileFlags::READ | fs::FileFlags::BINARY);
			auto file4 = fileSystem->open("../assets/placeholders/textures/grass1-Unreal-Engine2/grass1-ao-small.png", fs::FileFlags::READ | fs::FileFlags::BINARY);

			PbrMaterial material(
				new Image(file1->getInputStream()),
				new Image(file2->getInputStream()),
				new Image(file3->getInputStream()),
				new Image(file4->getInputStream())
			);

			materialMap.push_back(material);
		}

		{
			auto file1 = fileSystem->open("../assets/placeholders/textures/sandydrysoil-ue4/sandydrysoil-albedo-small.png", fs::FileFlags::READ | fs::FileFlags::BINARY);
			auto file2 = fileSystem->open("../assets/placeholders/textures/sandydrysoil-ue4/sandydrysoil-normal-ue-small.png", fs::FileFlags::READ | fs::FileFlags::BINARY);
			auto file3 = fileSystem->open("../assets/placeholders/textures/sandydrysoil-ue4/sandydrysoil-metalness-small.png", fs::FileFlags::READ | fs::FileFlags::BINARY);
			auto file4 = fileSystem->open("../assets/placeholders/textures/sandydrysoil-ue4/sandydrysoil-roughness-small.png", fs::FileFlags::READ | fs::FileFlags::BINARY);

			PbrMaterial material(
				new Image(file1->getInputStream()),
				new Image(file2->getInputStream()),
				new Image(file3->getInputStream()),
				new Image(file4->getInputStream())
			);

			materialMap.push_back(material);
		}

		{
			auto file1 = fileSystem->open("../assets/placeholders/textures/graniterockface1-Unreal-Engine/graniterockface1_Base_Color-small.png", fs::FileFlags::READ | fs::FileFlags::BINARY);
			auto file2 = fileSystem->open("../assets/placeholders/textures/graniterockface1-Unreal-Engine/graniterockface1_Normal-small.png", fs::FileFlags::READ | fs::FileFlags::BINARY);
			auto file3 = fileSystem->open("../assets/placeholders/textures/graniterockface1-Unreal-Engine/graniterockface1_Metallic-small.png", fs::FileFlags::READ | fs::FileFlags::BINARY);
			auto file4 = fileSystem->open("../assets/placeholders/textures/graniterockface1-Unreal-Engine/graniterockface1_Roughness-small.png", fs::FileFlags::READ | fs::FileFlags::BINARY);
			auto file5 = fileSystem->open("../assets/placeholders/textures/graniterockface1-Unreal-Engine/graniterockface1_Ambient_Occlusion-small.png", fs::FileFlags::READ | fs::FileFlags::BINARY);

			PbrMaterial material(
				new Image(file1->getInputStream()),
				new Image(file2->getInputStream()),
				new Image(file3->getInputStream()),
				new Image(file4->getInputStream()),
				new Image(file5->getInputStream())
			);

			materialMap.push_back(material);
		}

		return std::move(materialMap);
	}
	*/
};

}

#endif /* SPLATMAP_H_ */
