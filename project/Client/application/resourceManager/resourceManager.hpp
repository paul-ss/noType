#pragma once

template<typename Derived, typename T>
class ResourceManager {
    public:
        expclicit ResourceManager(const std::string& pathsFile) {
            LoadPaths(pathsFile);
        }
        virtual ~ResourceManager() {
            PurgeResources();
        }

    private:
        std::unordered_map<std::string, std::pair<T*, size_t> > _resources;
        std::unordered_map<std::string, std::string> _paths;
};
