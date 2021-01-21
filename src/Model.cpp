// C++ importer interface
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/DefaultLogger.hpp>
#include <assimp/IOStream.hpp>
#include <assimp/IOSystem.hpp>
#include <assimp/version.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

#include "Model.hpp"

#include "detail/Format.hpp"
#include "detail/Assert.hpp"

#include "exceptions/RuntimeException.hpp"

namespace ice_engine
{

class IResourceCache;

namespace
{

std::ios_base::seekdir toStdSeekdir(aiOrigin pOrigin)
{
	switch (pOrigin)
	{
		case aiOrigin::aiOrigin_SET:
			return std::ios_base::beg;

		case aiOrigin::aiOrigin_CUR:
			return std::ios_base::cur;

		case aiOrigin::aiOrigin_END:
			return std::ios_base::end;

		default:
			throw RuntimeException("invalid seek direction");
	}
}

class IoStream : public Assimp::IOStream
{
public:
	~IoStream() = default;

	size_t Read( void* pvBuffer, size_t pSize, size_t pCount) override
	{
		file_->getInputStream().read(static_cast<char*>(pvBuffer), pSize*pCount);

		return file_->getInputStream().gcount();
	}

	size_t Write( const void* pvBuffer, size_t pSize, size_t pCount) override
	{
		return 0;
	}

	aiReturn Seek( size_t pOffset, aiOrigin pOrigin) override
	{
		file_->getInputStream().seekg(pOffset, toStdSeekdir(pOrigin));

		if ( (file_->getInputStream().rdstate() & std::ifstream::failbit ) != 0 )
		{
			return aiReturn_FAILURE;
		}

		return aiReturn_SUCCESS;
	}

	size_t Tell() const override
	{
		return file_->getInputStream().tellg();
	}

	size_t FileSize() const override
	{
		return file_->size();
	}

	void Flush () override
	{}

protected:
	friend class IoSystem;

	IoStream(std::unique_ptr<fs::IFile> file) : file_(std::move(file))
	{
	}

private:
	std::unique_ptr<fs::IFile> file_;
};

class IoSystem : public Assimp::IOSystem
{
public:
	IoSystem(fs::IFileSystem* fileSystem) : fileSystem_(fileSystem)
	{
	}

	~IoSystem() override = default;

	bool ComparePaths(const char *one, const char *second) const override
	{
		return Assimp::IOSystem::ComparePaths(one, second);
	}

	bool Exists(const char *pFile) const override
	{
		return fileSystem_->exists(pFile);
	}

	char getOsSeparator() const override
	{
		return fileSystem_->getDirectorySeperator()[0];
	}

	IoStream* Open(const char *pFile, const char *pMode="rb") override
	{
		uint32 flags;

		if (pMode[0] == 'r')
		{
			flags |= fs::FileFlags::READ;
		}
		else if (pMode[0] == 'w')
		{
			flags |= fs::FileFlags::WRITE;
		}

		if (strlen(pMode) > 1 && pMode[1] == 'b') flags |= fs::FileFlags::BINARY;

		return new IoStream(fileSystem_->open(pFile, flags));
	}

	void Close(Assimp::IOStream* pFile) override
	{
		delete pFile;
	}

private:
	fs::IFileSystem* fileSystem_ = nullptr;
};

class Logger: public Assimp::Logger
{
public:
	Logger(logger::ILogger* logger) : logger_(logger)
	{
	}

	~Logger() override = default;

	bool attachStream(Assimp::LogStream *pStream, unsigned int severity=Debugging|Err|Warn|Info) override
	{
		return false;
	}

	bool detatchStream(Assimp::LogStream *pStream, unsigned int severity=Debugging|Err|Warn|Info) override
	{
		return false;
	}

	void OnInfo (const char *message) override
	{
//		LOG_INFO(logger_, message);
		logger_->info(message);
	}

	void OnDebug (const char *message) override
	{
//		LOG_DEBUG(logger_, message);
        logger_->debug(message);
	}

	void OnWarn (const char *message) override
	{
//		LOG_WARN(logger_, message);
        logger_->warn(message);
	}

	void OnError (const char *message) override
	{
//		LOG_ERROR(logger_, message);
        logger_->error(message);
	}

private:
	logger::ILogger* logger_ = nullptr;
};

}

void Model::import(const std::string& filename, IResourceCache* resourceCache,
		logger::ILogger* logger, fs::IFileSystem* fileSystem)
{
	const auto name = fileSystem->getFilenameWithoutExtension(filename);

    LOG_DEBUG(logger, "Importing model '%s' - %s.", name, filename);

	// get a handle to the predefined STDOUT log stream and attach
	// it to the logging system. It remains active for all further
	// calls to aiImportFile(Ex) and aiApplyPostProcessing.
	//stream = aiGetPredefinedLogStream(aiDefaultLogStream_FILE,"assimp_log.txt");
//#ifdef DEBUG
//	aiLogStream stream = aiGetPredefinedLogStream(aiDefaultLogStream_STDOUT, nullptr);
//	aiAttachLogStream(&stream);
//#endif

	try
	{
        Assimp::Importer importer;

        std::string extensionsSupported;
        importer.GetExtensionList(extensionsSupported);

        LOG_DEBUG(logger, "Importing using AssImp version %s.%s.%s", aiGetVersionMajor(), aiGetVersionMinor(), aiGetVersionRevision());
        LOG_DEBUG(logger, "Supported extensions are %s", extensionsSupported);

#if defined(DEBUG) || defined(ICEENGINE_ENABLE_DEBUG_LOGGING)
        Assimp::Logger::LogSeverity logSeverity = Assimp::Logger::LogSeverity::VERBOSE;
#else
        Assimp::Logger::LogSeverity logSeverity = Assimp::Logger::LogSeverity::NORMAL;
#endif

        Assimp::DefaultLogger::create("", logSeverity);
        Assimp::DefaultLogger::set(new Logger(logger));

		importer.SetIOHandler(new IoSystem(fileSystem));

		// We don't currently support aiProcess_JoinIdenticalVertices or aiProcess_FindInvalidData
		// aiProcess_FindInvalidData - I think it's due to the reduction of animation tracks containing redundant keys..
		const aiScene* scene = importer.ReadFile(filename, aiProcessPreset_TargetRealtime_MaxQuality ^ aiProcess_FindInvalidData);

		// Error checking
		if (scene == nullptr)
		{
			throw RuntimeException(detail::format("Unable to import model data from file '%s': %s", filename, importer.GetErrorString()));
		}
		else if (scene->HasTextures())
		{
			throw RuntimeException("Support for meshes with embedded textures is not implemented.");
		}

		meshes_.resize( scene->mNumMeshes );
//			materials.resize( scene->mNumMeshes );
		textures_.resize( scene->mNumMeshes );
//			boneData.resize( scene->mNumMeshes );

		importAnimations(name, filename, scene, logger, fileSystem);
		skeleton_ = Skeleton(name, filename, scene, logger, fileSystem);

		/*
		// Load the animation information
		for ( auto& kv : animationSet.animations)
		{
			LOG_DEBUG(logger, "loading animation information for animation " + kv.second.name);

			animations.push_back(kv.second);

			// Create animated bone node information
			auto animatedBoneNodes = std::map< std::string, AnimatedBoneNode >();
			for ( auto& kvAnimatedBoneNode : kv.second.animatedBoneNodes )
			{
				animatedBoneNodes[ kvAnimatedBoneNode.first ] = AnimatedBoneNode(
					kvAnimatedBoneNode.second.name,
					kvAnimatedBoneNode.second.positionTimes,
					kvAnimatedBoneNode.second.rotationTimes,
					kvAnimatedBoneNode.second.scalingTimes,
					kvAnimatedBoneNode.second.positions,
					kvAnimatedBoneNode.second.rotations,
					kvAnimatedBoneNode.second.scalings
				);
			}

			// Actually create the animation
			auto animation = Animation( kv.second.name, kv.second.duration, kv.second.ticksPerSecond, animatedBoneNodes );

			assert(animation != nullptr);

			animations.push_back(animation);

			// TODO: add animations properly (i.e. with names specifying the animation i guess?)
			//std::cout << "anim: " << animation->getName() << std::endl;
		}
		*/

        LOG_DEBUG(logger, "Model '%s' has %s meshes.", name, meshes_.size());

		for (size_t i=0; i < meshes_.size(); ++i)
		{
//				meshes_[i] = Mesh();
//				materials[i] = Material();
//				textures_[i] = Texture();
//				boneData[i] = BoneData();

			meshes_[i] = Mesh(name, filename, i, scene->mMeshes[i], logger, fileSystem);
//				materials[i] = importMaterial(name, filename, i, scene->mMaterials[ scene->mMeshes[i]->mMaterialIndex ], logger, fileSystem);
			textures_[i] = Texture(name, filename, i, scene->mMaterials[ scene->mMeshes[i]->mMaterialIndex ], resourceCache, logger, fileSystem);
		}

		bool hasTextures = false;
		for (const auto& texture : textures_)
		{
			if (texture.name() != std::string(""))
			{
				hasTextures = true;
				break;
			}
		}

		if (!hasTextures)
		{
            LOG_WARN(logger, "Model '%s' does not have any textures - either assign it a texture, or use a shader that doesn't need textures.", name);
		}

        LOG_DEBUG(logger, "Done importing model '%s'.", name);
	}
	catch (const Exception& e)
	{
		Assimp::DefaultLogger::kill();

		// We added a log stream to the library, it's our job to disable it
		// again. This will definitely release the last resources allocated
		// by Assimp.
//#ifdef DEBUG
//		aiDetachAllLogStreams();
//#endif

		throw e;
	}
	catch (const std::exception& e)
	{
		Assimp::DefaultLogger::kill();

		// We added a log stream to the library, it's our job to disable it
		// again. This will definitely release the last resources allocated
		// by Assimp.
//#ifdef DEBUG
//		aiDetachAllLogStreams();
//#endif

		throw e;
	}

	// We added a log stream to the library, it's our job to disable it
	// again. This will definitely release the last resources allocated
	// by Assimp.
//#ifdef DEBUG
//	aiDetachAllLogStreams();
//#endif
}

void Model::importAnimations(const std::string& name,
		const std::string& filename, const aiScene* scene,
		logger::ILogger* logger, fs::IFileSystem* fileSystem)
{
    ICE_ENGINE_ASSERT(scene != nullptr);

    LOG_DEBUG(logger, "Importing %s animation(s) for model '%s'.", scene->mNumAnimations, name);

	for (uint32 i = 0; i < scene->mNumAnimations; ++i)
	{
		auto animation = Animation(name, filename, scene->mAnimations[i], logger, fileSystem);

		// Add animation to animation data
		if (animations_.find(animation.name()) == animations_.end())
		{
			animations_[animation.name()] = std::move(animation);
		}
		else
		{
			// Warning - animation already exists!
            LOG_WARN(logger, "Animation with name '%s' already exists for model '%s'!", animation.name(), name);
		}
	}

    LOG_DEBUG(logger, "Done importing animation(s) for model '%s'.", name);
}

}
