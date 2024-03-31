#include "config/Config.hpp"

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)
#define AT __FILE__ ":" TOSTRING(__LINE__)

#ifdef DEBUG_BUILD
    #define CONFIG_PATH_PREFIX "./current_config"
#elif
    #define CONFIG_PATH_PREFIX "/current_config"
#endif

boost::property_tree::ptree defaultConfig;
boost::property_tree::ptree currentConfig;

void readDefaultConfig() {
    boost::property_tree::ini_parser::read_ini( TOSTRING(CONFIG_PATH_PREFIX) "/default.ini", defaultConfig);
    printf("[CONFIG] Config on /current_config/default.ini has been read!\n");
}

void readConfig() {
    boost::property_tree::ini_parser::read_ini( TOSTRING(CONFIG_PATH_PREFIX) "/config.ini", currentConfig);
    printf("[CONFIG] Config on /current_config/config.ini has been read!\n");
}
