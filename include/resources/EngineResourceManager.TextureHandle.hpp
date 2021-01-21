#ifndef ENGINERESOURCEMANAGER_TEXTUREHANDLE_HPP_
#define ENGINERESOURCEMANAGER_TEXTUREHANDLE_HPP_

#include "resources/EngineResourceManager.hpp"
#include "resources/GraphicsEngineResourceManager.hpp"

#include "graphics/TextureHandle.hpp"

#include "detail/Validation.hpp"

namespace ice_engine
{

template<>
class EngineResourceManager<graphics::TextureHandle>
        :
                public AbstractEngineResourceManager<graphics::TextureHandle, EngineResourceManager<graphics::TextureHandle>>,
                GraphicsEngineResourceManager<graphics::TextureHandle>
{
public:
    using GraphicsEngineResourceManager<graphics::TextureHandle>::GraphicsEngineResourceManager;

    ~EngineResourceManager() override = default;

    friend class AbstractEngineResourceManager<graphics::TextureHandle, EngineResourceManager<graphics::TextureHandle>>;

protected:
    template<typename ... Args>
    graphics::TextureHandle _create(const Args& ... args)
    {
        return graphicsEngine_->createTexture2d(args ...);
    }
};

}

#endif //ENGINERESOURCEMANAGER_TEXTUREHANDLE_HPP_
