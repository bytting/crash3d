#ifndef SESSION_H
#define SESSION_H

#include <vector>
#include <string>

class Spectrum;

class Session
{
public:
    Session();
    ~Session();

    void load(const std::string &path);

    std::string name;

private:

    std::vector<Spectrum*> mSpecList;
};

#endif // SESSION_H
