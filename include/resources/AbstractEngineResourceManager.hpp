#ifndef ABSTRACTENGINERESOURCEMANAGER_HPP_
#define ABSTRACTENGINERESOURCEMANAGER_HPP_

#include <boost/type_index.hpp>

#include "resources/BaseEngineResourceManager.hpp"

#include "detail/Format.hpp"

namespace ice_engine
{

template<typename T, typename Crtp>
class AbstractEngineResourceManager : public BaseEngineResourceManager
{
public:
    AbstractEngineResourceManager(const AbstractEngineResourceManager&) = delete;
    AbstractEngineResourceManager& operator=(const AbstractEngineResourceManager&) = delete;

    AbstractEngineResourceManager(AbstractEngineResourceManager&&) = default;
    AbstractEngineResourceManager& operator=(AbstractEngineResourceManager&&) noexcept = default;

    template<typename ... Args>
    const T& create(const std::string& name, const Args& ... args)
    {
        std::lock_guard<std::recursive_mutex> lock(mutex_);

        if (exists(name))
        {
            throw RuntimeException(detail::format("%s with name '%s' already exists.", boost::typeindex::type_id<T>().pretty_name(), name));
        }

        const auto handle = static_cast<Crtp*>(this)->_create(args ...);

        map_[name] = handle;

        return map_[name];
    }

    void destroy(const std::string& name)
    {
        std::lock_guard<std::recursive_mutex> lock(mutex_);

        if (!exists(name)) return;

        const auto resource = get(name);

        static_cast<Crtp*>(this)->_destroy(resource);
    }

    void destroy(const T& resource)
    {
        static_cast<Crtp*>(this)->_checkHandleValidity(resource);

        std::lock_guard<std::recursive_mutex> lock(mutex_);

        for (const auto& kv : map_)
        {
            if (kv.second == resource)
            {
                destroy(kv.first);
                break;
            }
        }
    }

    void destroyAll()
    {
        std::lock_guard <std::recursive_mutex> lock(mutex_);

        for (const auto& kv  : map_) destroy(kv.first);
    }

    bool exists(const std::string& name) const
    {
        std::lock_guard <std::recursive_mutex> lock(mutex_);

        return map_.find(name) != map_.end();
    }

    const T& get(const std::string& name) const
    {
        std::lock_guard <std::recursive_mutex> lock(mutex_);

        if (!exists(name))
        {
            throw RuntimeException(detail::format("%s with name '%s' does not exist.", boost::typeindex::type_id<T>().pretty_name(), name));
        }

        const auto it = map_.find(name);
        return it->second;
    }

protected:
    AbstractEngineResourceManager() = default;
    virtual ~AbstractEngineResourceManager() = default;

    std::unordered_map <std::string, T> map_;
    mutable std::recursive_mutex mutex_;

//    void _checkHandleValidity(const T& resource)
//    {
//        static_cast<Crtp*>(this)->_checkHandleValidity(resource);
//    }
//
//    template<typename ... Args>
//    const T& _create(const Args& ... args)
//    {
//        return static_cast<Crtp*>(this)->_create(args ...);
//    }
//
//    void _destroy(const T& resource)
//    {
//        static_cast<Crtp*>(this)->_destroy(resource);
//    }
};

}

#endif //ABSTRACTENGINERESOURCEMANAGER_HPP_
