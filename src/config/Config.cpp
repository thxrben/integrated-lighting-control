#include "config/Config.hpp"

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)
#define AT __FILE__ ":" TOSTRING(__LINE__)

#ifdef DEBUG_BUILD
    #define CONFIG_PATH_PREFIX "./current_config"
#else
    #define CONFIG_PATH_PREFIX "/current_config"
#endif

boost::property_tree::ptree defaultConfig;
boost::property_tree::ptree currentConfig;

void readDefaultConfig() {
    try {
        boost::property_tree::ini_parser::read_ini( TOSTRING(CONFIG_PATH_PREFIX) "/default.ini", defaultConfig);
    } catch(boost::property_tree::ptree_bad_data &ex) {
        std::cerr << "The config was not formatted correctly." << std::endl;
    } catch(boost::property_tree::ptree_bad_path &ex) {
        std::cerr << "The config could not be read due to a path error!" << std::endl;
    } catch (boost::property_tree::ptree_error &ex) {
        std::cerr << "The config could not be read!" << std::endl;
    } catch(std::exception &ex) {
        std::cerr << "WHAT THE FUCK?" << std::endl;
    }
    printf("[CONFIG] Config on /current_config/default.ini has been read!\n");
}

void readConfig() {
    try {
        boost::property_tree::ini_parser::read_ini( TOSTRING(CONFIG_PATH_PREFIX) "/config.ini", currentConfig);
    } catch(boost::property_tree::ptree_bad_data &ex) {
        std::cerr << "The config was not formatted correctly." << std::endl;
    } catch(boost::property_tree::ptree_bad_path &ex) {
        std::cerr << "The config could not be read due to a path error!" << std::endl;
    } catch (boost::property_tree::ptree_error &ex) {
        std::cerr << "The config could not be read!" << std::endl;
    } catch(std::exception &ex) {
        std::cerr << "WHAT THE FUCK?" << std::endl;
    }
    
    printf("[CONFIG] Config on /current_config/config.ini has been read!\n");
}
