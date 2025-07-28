#pragma once

#include "PCH.hpp"

#include "Core/Logger.hpp"

namespace AE
{
    template <typename ResourceType>
    class ResourceManager
    {
    public:

        ResourceManager(const std::string& resourceName = "Resource")
            : _resourceName(resourceName) {}

        bool Add(const std::string& name, std::shared_ptr<ResourceType> resource)
        {
            LoggerContext ctx(_resourceName + "Manager", "Add");

            if (name.empty())
            {
                Logger::Error("Invalid params! Name cannot be empty!");
                return false;
            }
            
            if (Has(name))
            {
                Logger::Error("{} with name '{}' already exists!", _resourceName, name);
                return false;    
            }
            
            _resources[name] = std::move(resource);

            return true;
        }

        bool Remove(const std::string& name)
        {
            LoggerContext ctx(_resourceName + "Manager", "Remove");

            if (name.empty())
            {
                Logger::Error("Invalid params! Name cannot be empty!");
                return false;
            }
            
            auto it = _resources.find(name);
            if (it != _resources.end())
            {
                _resources.erase(it);
                return true;
            }

            Logger::Error("{} with name '{}' not found!", _resourceName, name);

            return false;
        }

        void Clear()
        {
            _resources.clear();
        }

        std::shared_ptr<ResourceType> Get(const std::string& name) const
        {
            auto it = _resources.find(name);
            if (it != _resources.end())
                return it->second.lock();
            
            return nullptr;
        }
        
        bool Has(const std::string& name) const
        {
            auto it = _resources.find(name);
            return it != _resources.end() && !it->second.expired();
        }

        std::size_t Count() const
        {
            return _resources.size();
        }

    private:

        std::string _resourceName;
        std::unordered_map<std::string, std::weak_ptr<ResourceType>> _resources;

    };
}
