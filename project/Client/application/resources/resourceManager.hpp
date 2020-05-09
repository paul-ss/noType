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

    virtual ~ResourceManager() = default;

    std::weak_ptr<T> GetResource(const std::string& l_id);
    std::string GetPath(const std::string& l_id);
    bool RequireResource(const std::string& l_id);

protected:
    std::shared_ptr<T> load(const std::string& l_path);

private:
    void loadPaths(const std::string& l_pathFile);

private:
    std::unordered_map<std::string, std::shared_ptr<T>> _resources;
    std::unordered_map<std::string, std::string> _paths;
};

template <typename Derived, typename T>
std::weak_ptr<T> ResourceManager<Derived, T>::GetResource(const std::string& l_id) {
    auto res = _resources.find(l_id);
    return(res ? res->second : std::weak_ptr<T>());
}

template <typename Derived, typename T>
std::string ResourceManager<Derived, T>::GetPath(const std::string& l_id) {
    auto path = _paths.find(l_id);
    return(path != _paths.end() ? path->second : std::string());
}

template <typename Derived, typename T>
bool ResourceManager<Derived, T>::RequireResource(const std::string& l_id) {
    auto res = _resources.find(l_id).lock();
    if(res) {
        return true;
    }
    auto path = _paths.find(l_id);
    if (path == _paths.end()) {
        return false;
    }
    auto resource = load(path->second);
    if (!resource) {
        return false;
    }
    _resources.emplace(l_id, resource);
    return true;
}

template <typename Derived, class T>
std::shared_ptr<T> ResourceManager<Derived, T>::load(const std::string& l_path) {
    return static_cast<Derived*>(this)->load(l_path);
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
