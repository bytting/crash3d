#ifndef SPECTRUM_H
#define SPECTRUM_H

#include <vector>
#include <string>

class Spectrum
{
public:
    Spectrum();
    ~Spectrum();

    void load(const std::string &filename);

    int sessionIndex;
    double latitudeStart, latitudeStartErr;
    double latitudeEnd, latitudeEndErr;
    double longitudeStart, longitudeStartErr;
    double longitudeEnd, longitudeEndErr;
    double altitudeStart, altitudeStartErr;
    double altitudeEnd, altitudeEndErr;
    double gpsSpeedStart, gpsSpeedStartErr;
    double gpsSpeedEnd, gpsSpeedEndErr;
    std::string gpsTimeStart, gpsTimeEnd;
    int realtime, livetime;
    int totalCount;
    int numChannels;
    std::vector<int> channels;

private:
};

#endif // SPECTRUM_H
