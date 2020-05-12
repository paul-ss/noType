#include "baseElement.hpp"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

void BaseElement::loadStyle(const std::string& l_path) {
    try {
        boost::property_tree::ptree root;
        boost::property_tree::read_json(l_path, root);
    } catch (const boost::property_tree::ptree_error& e) {
    BOOST_LOG_TRIVIAL(error) << e.what() << " " << l_path;
    }
}