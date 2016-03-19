
#include "session.h"
#include "spectrum.h"

Session::Session()
{

}

Session::~Session()
{
    for(std::vector<Spectrum*>::iterator it = mSpecList.begin(); it != mSpecList.end(); ++it)
    {
        delete *it;
    }
}

void Session::load(const std::string &path)
{

}
