#include <assimp/anim.h>

#include "Animation.hpp"

#include "detail/DebugSerializer.hpp"
#include "detail/AssImpUtilities.hpp"
#include "detail/Assert.hpp"

namespace ice_engine
{

void Animation::import(const std::string& name, const std::string& filename, const aiAnimation* animation, logger::ILogger* logger, fs::IFileSystem* fileSystem)
{
    ICE_ENGINE_ASSERT(animation != nullptr);

    LOG_DEBUG(logger, "Importing animation for mesh '%s' for model '%s'." , filename, name);

    name_ = std::string(animation->mName.C_Str());

    LOG_DEBUG(logger, "Animation has name '%s' for mesh '%s' for model '%s'." , name_, filename, name);

    // Error check - animations with no name are not allowed
    if (name_.compare("") == 0)
    {
        LOG_WARN(logger, "Animations with no name are not allowed for mesh '%s' for model '%s'." , filename, name);

        name_ = name + "_animation";
        LOG_WARN(logger, "Setting animation name to '%s' for mesh '%s' for model '%s'." , name_, filename, name);
        // TODO: should we throw an exception?
        //throw std::runtime_error(msg);

        // TODO: Should we skip to the next animation?
        //continue;
    }

    duration_ = std::chrono::duration<float32>(static_cast<float32>(animation->mDuration));
    ticksPerSecond_ = static_cast<float32>(animation->mTicksPerSecond);

    LOG_DEBUG(logger, "Animation has duration = %s, ticksPerSecond = %s, numChannels = %s for mesh '%s' for model '%s'.", duration_, ticksPerSecond_, animation->mNumChannels, filename, name);

    // Load AnimatedBoneNodes for this animation
    for (uint32 j = 0; j < animation->mNumChannels; ++j)
    {
        const aiNodeAnim* pNodeAnim = animation->mChannels[j];

        AnimatedBoneNode abn;
        abn.name = std::string(pNodeAnim->mNodeName.C_Str());

        LOG_TRACE(logger, "Animation has animated bone node with name = %s for mesh '%s' for model '%s'.", abn.name, filename, name);

        for (uint32 k = 0; k < pNodeAnim->mNumPositionKeys; ++k)
        {
            const std::chrono::duration<float32> time{static_cast<float32>(pNodeAnim->mPositionKeys[k].mTime)};
            const glm::vec3 position = detail::toGlm(pNodeAnim->mPositionKeys[k].mValue);

            abn.positionKeyFrames.push_back({time, position});
        }

        for (uint32 k = 0; k < pNodeAnim->mNumRotationKeys; ++k)
        {
            const auto& rk = pNodeAnim->mRotationKeys[k];

            const std::chrono::duration<float32> time{static_cast<float32>(rk.mTime)};
            const glm::quat rotation = glm::normalize(detail::toGlm(rk.mValue));

            abn.rotationKeyFrames.push_back({time, rotation});
        }

        for (uint32 k = 0; k < pNodeAnim->mNumScalingKeys; ++k)
        {
            const std::chrono::duration<float32> time{static_cast<float32>(pNodeAnim->mScalingKeys[k].mTime)};
            const glm::vec3 scaling = detail::toGlm(pNodeAnim->mScalingKeys[k].mValue);

            abn.scalingKeyFrames.push_back({time, scaling});
        }

        // Add AnimatedBoneNode to the animation
        if (animatedBoneNodes_.find(abn.name) == animatedBoneNodes_.end())
        {
            animatedBoneNodes_[abn.name] = std::move(abn);
        }
        else
        {
            // Warning - animated bone node already exists!
            LOG_WARN(logger, "Animated bone node with name '%s' already exists for mesh '%s' for model '%s'!" , abn.name, filename, name);
        }
    }

    LOG_DEBUG(logger, "Done importing animation with name '%s' for mesh '%s' for model '%s'." , name_, filename, name);
}

}
