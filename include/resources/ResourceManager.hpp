#ifndef RESOURCEMANAGER_HPP_
#define RESOURCEMANAGER_HPP_

#include <string>
#include <memory>
#include <unordered_map>
#include <mutex>

#include <boost/type_index.hpp>

#include "resources/BaseResourceManager.hpp"

#include "IResourceImporter.hpp"

#include "exceptions/InvalidArgumentException.hpp"

#include "detail/Format.hpp"

namespace ice_engine
{

template<typename T>
class ResourceManager : public BaseResourceManager
{
public:
    ResourceManager(std::unordered_map<std::string, std::unique_ptr<IResourceImporter<T>>> importers)
            :
            importers_(std::move(importers))
    {

    }

    ~ResourceManager() override = default;

    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;

    ResourceManager(ResourceManager&&) = default;
    ResourceManager& operator=(ResourceManager&&) noexcept = default;

    T* import(const std::string& name, const std::string& filename, const std::string& importer = "")
    {
        std::lock_guard<std::recursive_mutex> lock(mutex_);

        if (exists(name))
        {
            throw RuntimeException(detail::format("%s with name '%s' already exists.", boost::typeindex::type_id<T>().pretty_name(), name));
        }

        const auto _importer = findImporter(filename, importer);

        if (!_importer)
        {
            if (importer.empty())
            {
                throw RuntimeException(detail::format("Unable to find a resource importer for %s that supports '%s'.", boost::typeindex::type_id<T>().pretty_name(), filename));
            }
            else
            {
                throw RuntimeException(detail::format("Unable to find a resource importer for %s with name '%s'.", boost::typeindex::type_id<T>().pretty_name(), importer));
            }
        }

        auto resource = _importer->import(name, filename);

        map_[name] = std::move(resource);

        return map_[name].get();
    }

//    T* load(const std::string& name, const std::string& filename)

    void destroy(const std::string& name)
    {
        std::lock_guard<std::recursive_mutex> lock(mutex_);

        if (!exists(name)) return;

        map_.erase(name);
    }

    void destroy(const T* resource)
    {
        if (resource == nullptr) throw InvalidArgumentException("Resource must not be null.");

        std::lock_guard<std::recursive_mutex> lock(mutex_);

        for (const auto& kv : map_)
        {
            if (kv.second.get() == resource)
            {
                destroy(kv.first);
                break;
            }
        }
    }

    void destroyAll()
    {
        std::lock_guard <std::recursive_mutex> lock(mutex_);

        for (auto it = map_.begin(); it != map_.end();)
        {
            const auto eraseIt = it;
            ++it;
            destroy(eraseIt->first);
        }
    }

    bool exists(const std::string& name) const
    {
        std::lock_guard <std::recursive_mutex> lock(mutex_);

        return map_.find(name) != map_.end();
    }

    T* get(const std::string& name) const
    {
        std::lock_guard <std::recursive_mutex> lock(mutex_);

        if (!exists(name))
        {
            throw RuntimeException(detail::format("%s with name '%s' does not exist.", boost::typeindex::type_id<T>().pretty_name(), name));
        }

        const auto it = map_.find(name);
        return it->second.get();
    }

protected:
    //    std::unique_ptr<IResourceImporter<T>> loader_;
    std::unordered_map<std::string, std::unique_ptr<IResourceImporter<T>>> importers_;
    std::unordered_map<std::string, std::unique_ptr<T>> map_;
    mutable std::recursive_mutex mutex_;

    IResourceImporter<T>* findImporter(const std::string& filename, const std::string& importer = "")
    {
        if (!importer.empty())
        {
            const auto it = importers_.find(importer);

            if (it != importers_.end()) return it->second.get();

            return nullptr;
        }

        for (const auto& kv : importers_)
        {
            if (kv.second->supports(filename))
            {
                return kv.second.get();
            }
        }

        return nullptr;
    }
};

}

#endif //RESOURCEMANAGER_HPP_
