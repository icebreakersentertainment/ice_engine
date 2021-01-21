#ifndef ENGINERESOURCEMANAGER_MESHHANDLE_HPP_
#define ENGINERESOURCEMANAGER_MESHHANDLE_HPP_

#include "resources/EngineResourceManager.hpp"
#include "resources/GraphicsEngineResourceManager.hpp"

#include "graphics/MeshHandle.hpp"

#include "detail/Validation.hpp"

namespace ice_engine
{

template<>
class EngineResourceManager<graphics::MeshHandle>
:
    public AbstractEngineResourceManager<graphics::MeshHandle, EngineResourceManager<graphics::MeshHandle>>,
    GraphicsEngineResourceManager<graphics::MeshHandle>
{
public:
    using GraphicsEngineResourceManager<graphics::MeshHandle>::GraphicsEngineResourceManager;

    ~EngineResourceManager() override = default;

    friend class AbstractEngineResourceManager<graphics::MeshHandle, EngineResourceManager<graphics::MeshHandle>>;

protected:
    template<typename ... Args>
    graphics::MeshHandle _create(const Args& ... args)
    {
        return graphicsEngine_->createStaticMesh(args ...);
    }
};

}

#endif //ENGINERESOURCEMANAGER_MESHHANDLE_HPP_
