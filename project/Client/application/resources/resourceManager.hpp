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
            PurgeResources();
        }

        T* GetResource(const std::string& id);
        std::string GetPath(const std::string& id);
        bool RequireResource(const std::string& id);
        bool ReleaseResource(const std::string& id);
        void PurgeResources();

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

template <typename Derived, typename T>
T* ResourceManager<Derived, T>::GetResource(const std::string& id) {
    auto res = find(id);
    return(res ? res->first : nullptr);
}

template <typename Derived, typename T>
std::string ResourceManager<Derived, T>::GetPath(const std::string& id) {
    auto path = _paths.find(id);
    return(path != _paths.end() ? path->second : "");
}

template <typename Derived, typename T>
bool ResourceManager<Derived, T>::RequireResource(const std::string& id) {
    auto res = find(id);
    if(res) {
        ++res->second;
        return true;
    }
    auto path = _paths.find(id);
    if (path == _paths.end()){ return false; }
    T* resource = load(path->second);
    if (!resource) {
        return false;
    }
    _resources.emplace(id, std::make_pair(resource, 1));
    return true;
}

template <typename Derived, typename T>
bool ResourceManager<Derived, T>::ReleaseResource(const std::string& id) {
    auto res = find(id);
    if (!res) {
        return false;
    }
    --res->second;
    if (!res->second) {
        unload(id);
    }
    return true;
}

template <typename Derived, typename T>
void ResourceManager<Derived, T>::PurgeResources() {
    std::cout << "Purging all resources:" << std::endl;
    while(_resources.begin() != _resources.end()){
        std::cout << "Removing: " 
            << _resources.begin()->first << std::endl;
        delete _resources.begin()->second.first;
        _resources.erase(_resources.begin());
    }
    std::cout << "Purging finished." << std::endl;
}

template <typename Derived, class T>
T* ResourceManager<Derived, T>::load(const std::string& path) {
    return static_cast<Derived*>(this)->Load(path);
}

template <typename Derived, typename T>
std::pair<T*, unsigned int>* ResourceManager<Derived, T>::find(const std::string& id) {
    auto itr = _resources.find(id);
    return (itr != _resources.end() ? &itr->second : nullptr);
}

template <typename Derived, typename T>
bool ResourceManager<Derived, T>::unload(const std::string& id) {
    auto itr = _resources.find(id);
    if (itr == _resources.end()) {
        return false;
    }
    delete itr->second.first;
    _resources.erase(itr);
    return true;
}

template <typename Derived, typename T>
void ResourceManager<Derived, T>::loadPaths(const std::string& pathFile) {
    std::ifstream paths;
    paths.open(utils::GetWorkingDirectory() + pathFile);
    if(paths.is_open()){
        std::string line;
        while(std::getline(paths, line)){
            std::stringstream keystream(line);
            std::string pathName;
            std::string path;
            keystream >> pathName;
            keystream >> path;
            _paths.emplace(pathName, path);
        }
        paths.close();
        return;
    }
    std::cerr <<
        "! Failed loading the path file: "
        << pathFile << std::endl;
}
