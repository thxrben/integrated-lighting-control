#include "config/Config.hpp"

boost::property_tree::ptree defaultConfig;
boost::property_tree::ptree currentConfig;

void readDefaultConfig() {
    boost::property_tree::ini_parser::read_ini("/current_config/default.ini", defaultConfig);
    printf("[CONFIG] Config on /current_config/default.ini has been read!\n");
}

void readConfig() {
    boost::property_tree::ini_parser::read_ini("/current_config/config.ini", currentConfig);
    printf("[CONFIG] Config on /current_config/config.ini has been read!\n");
}
