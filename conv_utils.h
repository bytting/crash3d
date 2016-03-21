#ifndef CONV_UTILS_H
#define CONV_UTILS_H

#include <string>
#include <sstream>
#include <iomanip>

template<class T>
std::string to_string(T val, int precision = 12)
{
    std::stringstream ss;
    ss << std::setprecision(precision) << val;
    return ss.str();
}

template<class T>
T from_string(const std::string &str, int precision = 12)
{
    T val;
    std::stringstream ss;
    ss << std::setprecision(precision) << str;
    ss >> val;
    return val;
}

int extract_id_from_name(const std::string &name);

#endif // CONV_UTILS_H
