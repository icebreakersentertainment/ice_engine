#ifndef ENGINERESOURCEMANAGER_VERTEXSHADERHANDLE_HPP_
#define ENGINERESOURCEMANAGER_VERTEXSHADERHANDLE_HPP_

#include "resources/EngineResourceManager.hpp"
#include "resources/GraphicsEngineResourceManager.hpp"

#include "graphics/VertexShaderHandle.hpp"

#include "detail/Validation.hpp"

namespace ice_engine
{

template<>
class EngineResourceManager<graphics::VertexShaderHandle>
:
    public AbstractEngineResourceManager<graphics::VertexShaderHandle, EngineResourceManager<graphics::VertexShaderHandle>>,
    GraphicsEngineResourceManager<graphics::VertexShaderHandle>
{
public:
    using GraphicsEngineResourceManager<graphics::VertexShaderHandle>::GraphicsEngineResourceManager;

    ~EngineResourceManager() override = default;

    friend class AbstractEngineResourceManager<graphics::VertexShaderHandle, EngineResourceManager<graphics::VertexShaderHandle>>;

protected:
    template<typename ... Args>
    graphics::VertexShaderHandle _create(const Args& ... args)
    {
        return graphicsEngine_->createVertexShader(args ...);
    }
};

}

#endif //ENGINERESOURCEMANAGER_VERTEXSHADERHANDLE_HPP_
