#ifndef __NMEA_HPP
#define __NMEA_HPP

#include <string>
#include <gps_data.hpp>

#define NMEA_GPGGA      "$GPGGA"
#define NMEA_GPRMC      "$GPRMC"
#define NMEA_GPZDA      "$GPZDA"
#define NMEA_GPGSV      "$GPGSV"
#define NMEA_GPGLL      "$GPGLL"

namespace nmea {

    enum {
        UNDEFINED = 0,
        GPGGA,
        GPRMC,
        GPZDA,
        GPGLL,
        GPGSV
    };

    bool valid_checksum(const std::string &message);

    int message_type(const std::string &message);

    void parse_gpgga(const std::string &message, gps_data_t &info);

    void parse_gprmc(const std::string &message, gps_data_t &info);

    void parse_gpzda(const std::string &message, gps_data_t &info);

    void parse_gpgll(const std::string &message, gps_data_t &info);

    void parse_gpgsv(const std::string &message, gps_data_t &info);

}

#endif // __NMEA_HPP

