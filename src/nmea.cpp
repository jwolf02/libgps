#include <cstdlib>
#include <cinttypes>
#include <cstring>
#include <nmea.hpp>
#include <cmath>

static inline  bool starts_with(const std::string &str1, const std::string &str2) {
    return str1.size() >= str2.size() ? std::equal(str1.begin(), str1.begin() + str2.size(), str2.begin()) : false;
}

static double gps_deg_dec(double deg_point) {
    // nmea: ddmm.mmmm
    const double min = std::fmod(deg_point, 100.0);
    return ((deg_point - min) / 100.0) + (min / 60.0);
    /*
    double ddeg;
    double sec = modf(deg_point, &ddeg);
    double deg = (double) (int(ddeg) / 100);
    double min = (double) int(deg_point-(deg*100));

    double absdlat = round(deg * 1000000.);
    double absmlat = round(min * 1000000.);
    double absslat = round(sec * 1000000.);

    return deg + (min + sec) / 60.0;*/
}

bool nmea::valid_checksum(const std::string &message) {
    auto checksum = (uint8_t) strtol(strchr(message.c_str(), '*') + 1, nullptr, 16);

    uint8_t sum = 0;
    const char *end = strchr(message.c_str(), '*');
    if (end == nullptr)
        return false;

    for (auto ptr = message.c_str() + 1; ptr != end; ++ptr) {
        sum = sum xor *ptr;
    }

    return sum == checksum;
}

int nmea::message_type(const std::string & message) {
    if (starts_with(message, NMEA_GPGGA)) {
        return nmea::GPGGA;
    } else if (starts_with(message, NMEA_GPRMC)) {
        return nmea::GPRMC;
    } else if (starts_with(message, NMEA_GPZDA)) {
        return nmea::GPZDA;
    } else if (starts_with(message, NMEA_GPGSV)) {
        return nmea::GPGSV;
    } else if (starts_with(message, NMEA_GPGLL)) {
        return nmea::GPGLL;
    } else {
        return nmea::UNDEFINED;
    }

}

void nmea::parse_gpgga(const std::string &message, gps_data_t &info) {
    const char *p = message.c_str();

    p = strchr(p, ',') + 1;
    info.hours = std::strtoul(std::string({ p[0], p[1], '\0' }).c_str(), nullptr, 10);
    info.minutes = std::strtoul(std::string({ p[2], p[3], '\0' }).c_str(), nullptr, 10);
    info.seconds = std::strtoul(std::string({ p[4], p[5], '\0' }).c_str(), nullptr, 10);

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

void nmea::parse_gprmc(const std::string &message, gps_data_t &info) {
    const char *p = message.c_str();

    p = strchr(p, ',') + 1;
    info.hours = std::strtoul(std::string({ p[0], p[1], '\0' }).c_str(), nullptr, 10);
    info.minutes = std::strtoul(std::string({ p[2], p[3], '\0' }).c_str(), nullptr, 10);
    info.seconds = std::strtoul(std::string({ p[4], p[5], '\0' }).c_str(), nullptr, 10);

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

void nmea::parse_gpzda(const std::string &message, gps_data_t &info) {
    const char *p = message.c_str();

    p = strchr(p, ',') + 1;
    info.hours = std::strtoul(std::string({ p[0], p[1], '\0' }).c_str(), nullptr, 10);
    info.minutes = std::strtoul(std::string({ p[2], p[3], '\0' }).c_str(), nullptr, 10);
    info.seconds = std::strtoul(std::string({ p[4], p[5], '\0' }).c_str(), nullptr, 10);

    p = strchr(p, ',') + 1;
    info.day = strtoul(p, nullptr, 10);

    p = strchr(p, ',') + 1;
    info.month = strtoul(p, nullptr, 10);

    p = strchr(p, ',') + 1;
    info.year = strtoul(p, nullptr, 10);

    p = strchr(p, ',') + 1;
    info.timezone = strtoul(p, nullptr, 10);
}

void nmea::parse_gpgll(const std::string & message, struct gps_data_t & info) {
    const char *p = message.c_str();

    p = strchr(p, ',') + 1;
    const double lat = std::strtod(p, nullptr);

    p = strchr(p, ',') + 1;
    info.latitude = gps_deg_dec(p[0] == 'N' ? lat : -1.0 * lat);

    p = strchr(p, ',') + 1;
    const double lon = std::strtod(p, nullptr);

    p = strchr(p, ',') + 1;
    info.longitude = gps_deg_dec(p[0] == 'E' ? lon : -1.0 * lon);

    p = strchr(p, ',') + 1;
    info.hours = std::strtoul(std::string({ p[0], p[1], '\0' }).c_str(), nullptr, 10);
    info.minutes = std::strtoul(std::string({ p[2], p[3], '\0' }).c_str(), nullptr, 10);
    info.seconds = std::strtoul(std::string({ p[4], p[5], '\0' }).c_str(), nullptr, 10);
}

void nmea::parse_gpgsv(const std::string & message, struct gps_data_t & info) {
    const char *p = message.c_str();

    p = strchr(p, ',') + 1; // skip number of messages
    p = strchr(p, ',') + 1; // skip message number
    p = strchr(p, ',') + 1;
    info.satellites_in_view = strtoul(p, nullptr, 10);

    for (unsigned i = 0; i < 4; ++i) {
        auto &sat = info.satellites[i];

        p = strchr(p, ',') + 1;
        sat.sv_prn = strtoul(p, nullptr, 10);

        p = strchr(p, ',') + 1;
        sat.elevation = strtoul(p, nullptr, 10);

        p = strchr(p, ',') + 1;
        sat.azimuth = strtoul(p, nullptr, 10);

        p = strchr(p, ',') + 1;
        sat.snr = strtoul(p, nullptr, 10);;
    }
}
