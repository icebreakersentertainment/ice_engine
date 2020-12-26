#include <assimp/anim.h>

#include "Animation.hpp"

namespace ice_engine
{

void Animation::import(const std::string& name, const std::string& filename, const aiAnimation* animation, logger::ILogger* logger, fs::IFileSystem* fileSystem)
{
    assert(scene != nullptr);

    LOG_DEBUG(logger, "Importing animation for mesh '%s' for model '%s'." , filename, name);

    name_ = std::string( animation->mName.C_Str() );

    LOG_DEBUG(logger, "Animation has name '%s' for mesh '%s' for model '%s'." , name_, filename, name);

    // Error check - animations with no name are not allowed
    if (name_.compare( std::string("") ) == 0)
    {
        LOG_WARN(logger, "Animations with no name are not allowed for mesh '%s' for model '%s'." , filename, name);

        name_ = name + "_animation";
        LOG_WARN(logger, "Setting animation name to '%s' for mesh '%s' for model '%s'." , name_, filename, name);
        // TODO: should we throw an exception?
        //throw std::runtime_error(msg);

        // TODO: Should we skip to the next animation?
        //continue;
    }

    duration_ = animation->mDuration;
    ticksPerSecond_ = animation->mTicksPerSecond;

    // Load AnimatedBoneNodes for this animation
    for (uint32 j = 0; j < animation->mNumChannels; ++j)
    {
        const aiNodeAnim* pNodeAnim = animation->mChannels[j];

        AnimatedBoneNode abn;
        abn.name = std::string( pNodeAnim->mNodeName.C_Str() );

        for (uint32 k = 0; k < pNodeAnim->mNumPositionKeys; ++k)
        {
            abn.positionTimes.push_back( pNodeAnim->mPositionKeys[k].mTime );
            abn.positions.push_back( glm::vec3( pNodeAnim->mPositionKeys[k].mValue.x, pNodeAnim->mPositionKeys[k].mValue.y, pNodeAnim->mPositionKeys[k].mValue.z ) );
        }

        for (uint32 k = 0; k < pNodeAnim->mNumRotationKeys; ++k)
        {
            const auto& rk = pNodeAnim->mRotationKeys[k];

            abn.rotationTimes.push_back( rk.mTime );

            glm::quat rotation = glm::quat( rk.mValue.w, rk.mValue.x, rk.mValue.y, rk.mValue.z );
            rotation = glm::normalize( rotation );
            abn.rotations.push_back( rotation );
        }

        for (uint32 k = 0; k < pNodeAnim->mNumScalingKeys; ++k)
        {
            abn.scalingTimes.push_back( pNodeAnim->mScalingKeys[k].mTime );
            abn.scalings.push_back( glm::vec3( pNodeAnim->mScalingKeys[k].mValue.x, pNodeAnim->mScalingKeys[k].mValue.y, pNodeAnim->mScalingKeys[k].mValue.z ) );
        }

        // Add AnimatedBoneNode to the animation
        if( animatedBoneNodes_.find( abn.name ) == animatedBoneNodes_.end() )
        {
            animatedBoneNodes_[ abn.name ] = std::move(abn);
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
