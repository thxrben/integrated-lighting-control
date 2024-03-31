#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <boost/property_tree/exceptions.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

#include <iostream>
#include <string>
#include <map>

enum ConfigProperty {
    IPC_PORT,
    SESSION_ID,
    SESSION_NAME,
    NETWORK_IFACE_NAME,
    NETWORK_ETH_ADDR,
    NETWORK_ETH_MASK
};


class Config {
    private:
        bool readConfig();

        boost::property_tree::ptree currentConfig;
        std::map<ConfigProperty, std::string> configMap;

        bool configReadSuccess = false;
        
        std::string configPropertyToStringPath(ConfigProperty property);
        std::string configPropertyGetDefault(ConfigProperty property);

    public:
        Config();

        template<typename T> T getProperty(ConfigProperty property);
        std::string getPropertyDefault(ConfigProperty property);
};

#endif