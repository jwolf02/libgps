#include <cstdio>
#include <cstdlib>
#include <cinttypes>
#include <cstring>
#include <nmea.hpp>
#include <cmath>

static inline  bool starts_with(const std::string &str1, const std::string &str2) {
    return str1.size() >= str2.size() ? std::equal(str1.begin(), str1.begin() + str2.size(), str2.begin()) : false;
}

static double gps_deg_dec(double deg_point) {
    double ddeg;
    double sec = modf(deg_point, &ddeg)*60;
    int deg = (int)(ddeg/100);
    int min = (int)(deg_point-(deg*100));

    double absdlat = round(deg * 1000000.);
    double absmlat = round(min * 1000000.);
    double absslat = round(sec * 1000000.);

    return round(absdlat + (absmlat/60) + (absslat/3600)) /1000000;
}

void nmea::parse_message(const std::string &message, gps_data &info) {
    if (starts_with(message, NMEA_GPGGA)) {
        nmea::parse_gpgga(message, info);
    } else if (starts_with(message, NMEA_GPRMC)) {
        nmea::parse_gprmc(message, info);
    } else if (starts_with(message, NMEA_GPZDA)) {
        nmea::parse_gpzda(message, info);
    } else {
        // throw error or something
    }
}

bool nmea::valid_checksum(const std::string &message) {
    auto checksum = (uint8_t) strtol(strchr(message.c_str(), '*') + 1, nullptr, 16);

    uint8_t sum = 0;
    for (auto ptr = message.c_str() + 1; *ptr != '*'; ++ptr) {
        sum = sum xor *ptr;
    }

    return sum == checksum;
}

void nmea::parse_gpgga(const std::string &message, gps_data &info) {
    const char *p = message.c_str();

    p = strchr(p, ',') + 1; // skip time

    p = strchr(p, ',') + 1;
    const double lat = std::strtod(p, nullptr);

    p = strchr(p, ',') + 1;
    info.latitude = gps_deg_dec(p[0] == 'N' ? lat : -1.0 * lat);

    p = strchr(p, ',') + 1;
    const double lon = std::strtod(p, nullptr);

    p = strchr(p, ',') + 1;
    info.longitude = gps_deg_dec(p[0] == 'E' ? lon : -1.0 * lon);

    p = strchr(p, ',') + 1;
    info.quality = strtoul(p, nullptr, 10);

    p = strchr(p, ',') + 1;
    info.satellites_connected = strtoul(p, nullptr, 10);

    p = strchr(p, ',') + 1;

    p = strchr(p, ',') + 1;
    info.altitude = std::strtod(p, nullptr);
}

void nmea::parse_gprmc(const std::string &message, gps_data &info) {
    const char *p = message.c_str();

    p = strchr(p, ',') + 1; // skip time
    p = strchr(p, ',') + 1; // skip status

    p = strchr(p, ',') + 1;
    const double lat = std::strtod(p, nullptr);

    p = strchr(p, ',') + 1;
    info.latitude = gps_deg_dec(p[0] == 'N' ? lat : -1.0 * lat);

    p = strchr(p, ',') + 1;
    const double lon = std::strtod(p, nullptr);

    p = strchr(p, ',') + 1;
    info.longitude = gps_deg_dec(p[0] == 'E' ? lon : -1.0 * lon);

    p = strchr(p, ',') + 1;
    info.speed = std::strtod(p, nullptr) * 1.852;

    p = strchr(p, ',') + 1;
    info.course = std::strtod(p, nullptr);
}

void nmea::parse_gpzda(const std::string &message, gps_data &info) {
    // not implemented yet
    return;
}
