#ifndef ENGINERESOURCEMANAGER_HPP_
#define ENGINERESOURCEMANAGER_HPP_

#include "resources/AbstractEngineResourceManager.hpp"

namespace ice_engine
{

template<typename T>
class EngineResourceManager : public AbstractEngineResourceManager<T, EngineResourceManager<T>>
{
public:
    EngineResourceManager(const EngineResourceManager&) = delete;
    EngineResourceManager& operator=(const EngineResourceManager&) = delete;

    EngineResourceManager(EngineResourceManager&&) = default;
    EngineResourceManager& operator=(EngineResourceManager&&) noexcept = default;

protected:
    EngineResourceManager() = default;

    virtual ~EngineResourceManager() = default;
};

}

#endif //ENGINERESOURCEMANAGER_HPP_
