#ifndef GRAPHICSENGINERESOURCEMANAGER_HPP_
#define GRAPHICSENGINERESOURCEMANAGER_HPP_

#include "graphics/IGraphicsEngine.hpp"

#include "detail/Validation.hpp"

namespace ice_engine
{

template<typename T>
class GraphicsEngineResourceManager
{
public:
    explicit GraphicsEngineResourceManager(graphics::IGraphicsEngine* graphicsEngine) : graphicsEngine_(graphicsEngine) {}

    GraphicsEngineResourceManager(const GraphicsEngineResourceManager&) = delete;
    GraphicsEngineResourceManager& operator=(const GraphicsEngineResourceManager&) = delete;

    GraphicsEngineResourceManager(GraphicsEngineResourceManager&&) = default;
    GraphicsEngineResourceManager& operator=(GraphicsEngineResourceManager&&) noexcept = default;

protected:
    virtual ~GraphicsEngineResourceManager() = default;

    graphics::IGraphicsEngine* graphicsEngine_;

    void _checkHandleValidity(const T& resource)
    {
        ice_engine::detail::checkHandleValidity(graphicsEngine_, resource);
    }

    void _destroy(const T& resource)
    {
        graphicsEngine_->destroy(resource);
    }
};

}

#endif //GRAPHICSENGINERESOURCEMANAGER_HPP_
