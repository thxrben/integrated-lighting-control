

#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <boost/property_tree/exceptions.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <iostream>
#include <string>

extern boost::property_tree::ptree defaultConfig;
extern boost::property_tree::ptree currentConfig;

extern void readDefaultConfig();
extern void readConfig();


#endif