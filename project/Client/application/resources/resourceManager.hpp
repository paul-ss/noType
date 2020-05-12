#pragma once

#include <string>
#include <unordered_map>
#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>
#include <filesystem>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include "logger.hpp"

template <typename Derived, typename T>
class ResourceManager {
public:
    explicit ResourceManager(const std::string& l_pathsFile, const std::string& l_key) {
        loadPaths(l_pathsFile, l_key);
    }

    virtual ~ResourceManager() = default;

    std::weak_ptr<T> GetResource(const std::string& l_id);
    std::string GetPath(const std::string& l_id);
    bool RequireResource(const std::string& l_id);

protected:
    std::shared_ptr<T> load(const std::string& l_path);

private:
    void loadPaths(const std::string& l_pathFile, const std::string& l_key);

private:
    std::unordered_map<std::string, std::shared_ptr<T>> _resources;
    std::unordered_map<std::string, std::string> _paths;
};

template <typename Derived, typename T>
std::weak_ptr<T> ResourceManager<Derived, T>::GetResource(const std::string& l_id) {
    auto res = _resources.find(l_id);
    if (res == _resources.end()) {
        return {};
    }
    return res->second;
}

template <typename Derived, typename T>
std::string ResourceManager<Derived, T>::GetPath(const std::string& l_id) {
    auto path = _paths.find(l_id);
    return(path != _paths.end() ? path->second : std::string());
}

template <typename Derived, typename T>
bool ResourceManager<Derived, T>::RequireResource(const std::string& l_id) {
    auto res = _resources.find(l_id);
    if(res != _resources.end()) {
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
void ResourceManager<Derived, T>::loadPaths(const std::string& l_pathFile,
        const std::string& l_key) {
    try {
        boost::property_tree::ptree root;
        boost::property_tree::read_json(l_pathFile, root);

        for (boost::property_tree::ptree::value_type& currPath : root.get_child(l_key)) {
            _paths.emplace(currPath.first.data(), currPath.second.data());
        }
    } catch (const boost::property_tree::ptree_error& e) {
    BOOST_LOG_TRIVIAL(error) << e.what() << " " << l_pathFile;
    }
}
