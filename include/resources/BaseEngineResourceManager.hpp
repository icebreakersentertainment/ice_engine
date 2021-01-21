#ifndef BASEENGINERESOURCEMANAGER_HPP_
#define BASEENGINERESOURCEMANAGER_HPP_

namespace ice_engine
{

class BaseEngineResourceManager
{
public:
    virtual ~BaseEngineResourceManager() = default;

    BaseEngineResourceManager(const BaseEngineResourceManager&) = delete;
    BaseEngineResourceManager& operator=(const BaseEngineResourceManager&) = delete;

    BaseEngineResourceManager(BaseEngineResourceManager&&) = default;
    BaseEngineResourceManager& operator=(BaseEngineResourceManager&&) noexcept = default;

protected:
    BaseEngineResourceManager() = default;
};

}

#endif //BASEENGINERESOURCEMANAGER_HPP_
