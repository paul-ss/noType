#pragma once

#include <string>
#include <unordered_map>
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>

#include "logger.hpp"

template <typename Derived, typename T>
class ResourceManager {
    public:
        explicit ResourceManager(const std::string& l_pathsFile) {
            loadPaths(l_pathsFile);
        }

        virtual ~ResourceManager() {
            PurgeResources();
        }

        T* GetResource(const std::string& l_id);
        std::string GetPath(const std::string& l_id);
        bool RequireResource(const std::string& l_id);
        bool ReleaseResource(const std::string& l_id);
        void PurgeResources();

    protected:
        T* load(const std::string& l_path);

    private:
        std::pair<T*, unsigned int>* find(const std::string& l_id);
        bool unload(const std::string& l_id);
        void loadPaths(const std::string& l_pathFile);

    private:
        std::unordered_map<std::string,
                std::pair<T*, unsigned int>> _resources;
        std::unordered_map<std::string, std::string> _paths;
};

template <typename Derived, typename T>
T* ResourceManager<Derived, T>::GetResource(const std::string& l_id) {
    auto res = find(l_id);
    return(res ? res->first : nullptr);
}

template <typename Derived, typename T>
std::string ResourceManager<Derived, T>::GetPath(const std::string& l_id) {
    auto path = _paths.find(l_id);
    return(path != _paths.end() ? path->second : "");
}

template <typename Derived, typename T>
bool ResourceManager<Derived, T>::RequireResource(const std::string& l_id) {
    auto res = find(l_id);
    if(res) {
        ++res->second;
        return true;
    }
    auto path = _paths.find(l_id);
    if (path == _paths.end()) {
        return false;
    }
    T* resource = load(path->second);
    if (!resource) {
        return false;
    }
    _resources.emplace(l_id, std::make_pair(resource, 1));
    return true;
}

template <typename Derived, typename T>
bool ResourceManager<Derived, T>::ReleaseResource(const std::string& l_id) {
    auto res = find(l_id);
    if (!res) {
        return false;
    }
    --res->second;
    if (!res->second) {
        unload(l_id);
    }
    return true;
}

template <typename Derived, typename T>
void ResourceManager<Derived, T>::PurgeResources() {
    BOOST_LOG_TRIVIAL(info) << "Purging all resources";
    while(_resources.begin() != _resources.end()) {
    BOOST_LOG_TRIVIAL(info) << "Removing: " << _resources.begin()->first;
        delete _resources.begin()->second.first;
        _resources.erase(_resources.begin());
    }
    BOOST_LOG_TRIVIAL(info) << "Purging finished";
}

template <typename Derived, class T>
T* ResourceManager<Derived, T>::load(const std::string& l_path) {
    return static_cast<Derived*>(this)->load(l_path);
}

template <typename Derived, typename T>
std::pair<T*, unsigned int>* ResourceManager<Derived, T>::find(const std::string& l_id) {
    auto itr = _resources.find(l_id);
    return (itr != _resources.end() ? &itr->second : nullptr);
}

template <typename Derived, typename T>
bool ResourceManager<Derived, T>::unload(const std::string& l_id) {
    auto itr = _resources.find(l_id);
    if (itr == _resources.end()) {
        return false;
    }
    delete itr->second.first;
    _resources.erase(itr);
    return true;
}

template <typename Derived, typename T>
void ResourceManager<Derived, T>::loadPaths(const std::string& l_pathFile) {
    std::ifstream paths;
    paths.open(std::filesystem::absolute(l_pathFile));
    if(paths.is_open()) {
        std::string line;
        while(std::getline(paths, line)) {
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
    BOOST_LOG_TRIVIAL(error) << "Failed loading the path file: " << l_pathFile;
}
