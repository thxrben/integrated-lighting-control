#include "config/Config.hpp"

std::string Config::configPropertyToStringPath(ConfigProperty property) {
    if(property == IPC_PORT) {
        return "ipc.Port";
    } else if(property == SESSION_ID) {
        return "session.ID";
    } else if(property == SESSION_NAME) {
        return "session.Name";
    } else if(property == NETWORK_IFACE_NAME) {
        return "network.IfaceName";
    } else if(property == NETWORK_ETH_ADDR) {
        return "network.ethAddr";
    } else if(property == NETWORK_ETH_MASK) {
        return "network.ethMask";
    } else {
        return "";
    }
};

/**
 std::string Config::configPropertyGetDefault(ConfigProperty property) {
    if(property == IPC_PORT) {
        return "5555";
    } else if(property == SESSION_ID) {
        return "1";
    } else if(property == SESSION_NAME) {
        return "Default Session";
    } else if(property == NETWORK_IFACE_NAME) {
        return "eth0";
    } else if(property == NETWORK_ETH_ADDR) {
        return "10.0.0.5";
    } else if(property == NETWORK_ETH_MASK) {
        return "255.0.0.0";
    } else {
        return nullptr;
    }
};

*/


bool Config::readConfig() {
    try {
        std::string configPath = "/current_config/config.ini";
        std::print("[CONFIG] Reading from {}\n", configPath);
        boost::property_tree::ini_parser::read_ini( configPath, currentConfig);
    } catch(boost::property_tree::ptree_bad_data &ex) {
        std::cerr << "The config was not formatted correctly." << std::endl;
        return false;
    } catch (boost::property_tree::ptree_error &ex) {
        std::cerr << "The config could not be read!" << std::endl;
        return false;
    } catch(std::exception &ex) {
        std::cerr << "WHAT THE FUCK?" << std::endl;
        return false;
    }

    printf("[CONFIG] Config on /current_config/config.ini has been read!\n");
    return true;
}

Config::Config() {
    printf("[CONFIG] Trying to read config... \n");

    this->configReadSuccess = readConfig();

    printf("[CONFIG] Success: %d\n", this->configReadSuccess);
};

/**
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
        std::cerr << "Config read error: Property \"" << property << "\" not found!" << std::endl;
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

    result = configPropertyGetDefault(property);

    return result;
};
*/