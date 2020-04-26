#pragma once

#include "utils.hpp"

#include <string>
#include <unordered_map>
#include <iostream>
#include <fstream>
#include <sstream>

template <typename Derived, typename T>
class ResourceManager {
    public:
        ResourceManager(const std::string& pathsFile) {
            loadPaths(pathsFile);
        }

        virtual ~ResourceManager() {
            purgeResources();
        }

        T* getResource(const std::string& id);
        std::string getPath(const std::string& id);
        bool requireResource(const std::string& id);
        bool releaseResource(const std::string& id);
        void purgeResources();

    protected:
        T* load(const std::string& path);

    private:
        std::pair<T*, unsigned int>* find(const std::string& id);
        bool unload(const std::string& id);
        void loadPaths(const std::string& pathFile);

    private:
        std::unordered_map<std::string,
                std::pair<T*, unsigned int>> _resources;
        std::unordered_map<std::string, std::string> _paths;
};
