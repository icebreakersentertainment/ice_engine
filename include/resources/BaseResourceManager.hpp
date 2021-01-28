#ifndef BASERESOURCEMANAGER_HPP_
#define BASERESOURCEMANAGER_HPP_

namespace ice_engine
{

class BaseResourceManager
{
public:
    virtual ~BaseResourceManager() = default;

    BaseResourceManager(const BaseResourceManager&) = delete;
    BaseResourceManager& operator=(const BaseResourceManager&) = delete;

    BaseResourceManager(BaseResourceManager&&) = default;
    BaseResourceManager& operator=(BaseResourceManager&&) noexcept = default;

protected:
    BaseResourceManager() = default;
};

}

#endif //BASERESOURCEMANAGER_HPP_
