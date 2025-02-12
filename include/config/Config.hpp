#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <boost/property_tree/exceptions.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

#include <print>
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
        template<typename T> T configPropertyGetDefault(ConfigProperty property);

    public:
        Config();

        template<typename T> T getProperty(ConfigProperty property);
        std::string getPropertyDefault(ConfigProperty property);
};

template <typename T> 
T Config::configPropertyGetDefault(ConfigProperty property) {
    T result;

    if(property == IPC_PORT) {
        if(!std::is_same<T, int>::value)
            return "";
        result = 5555;
    } else if(property == SESSION_ID) {
        if(std::is_same<T, int>::value)
            return "";
        result = 1;
    } else if(property == SESSION_NAME) {
        if(!std::is_same<T, std::string>::value)
            return "";
        result = "Default Session";
    } else if(property == NETWORK_IFACE_NAME) {
        if(!std::is_same<T, std::string>::value)
            return "";
        result = "eth0";
    } else if(property == NETWORK_ETH_ADDR) {
        if(!std::is_same<T, std::string>::value)
            return "";
        result = "10.0.0.5";
    } else if(property == NETWORK_ETH_MASK) {
        if(!std::is_same<T, std::string>::value)
            return "";
        result = "255.0.0.0";
    }
};


template <typename T> T Config::getProperty(ConfigProperty property) {
    T result;
    bool resultWasAssigned = false;
    std::string stringPath;
    
    stringPath = configPropertyToStringPath(property);

    try {
        result = currentConfig.get<T>(stringPath);
        resultWasAssigned = true;
    } catch(boost::property_tree::ptree_bad_data &ex) {
        std::cerr << "The config was not formatted correctly while trying to read property \"" << property << "\"" << std::endl;
        resultWasAssigned = false;
    } catch(boost::property_tree::ptree_bad_path &ex) {
        std::cerr << "Config read error: Property \"" << property << "\" / \"" << stringPath <<  "\" not found!" << std::endl;
        resultWasAssigned = false;
    } catch (boost::property_tree::ptree_error &ex) {
        std::cerr << "The config could not be read while trying to read property \"" << property << "\"" << std::endl;
        resultWasAssigned = false;
    } catch(std::exception &ex) {
        std::cerr << "WHAT THE FUCK?" << std::endl;
        resultWasAssigned = false;
    }

    if(resultWasAssigned)
        return result;

    result = Config::configPropertyGetDefault<T>(property);

    return result;
};


#endif