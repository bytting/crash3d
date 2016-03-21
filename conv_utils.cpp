#include "conv_utils.h"

int extract_id_from_name(const std::string &name)
{
    int id;
    std::string typ;
    std::stringstream ss;
    ss << name;
    ss >> typ;
    ss >> id;
    return id;
}
