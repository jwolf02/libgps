#ifndef __NMEA_HPP
#define __NMEA_HPP

#include <string>

#define NMEA_GPGGA      "$GPGGA"
#define NMEA_GPRMC      "$GPRMC"
#define NMEA_GPZDA      "$GPZDA"
#define NMEA_GPGSV      "$GPGSV"
#define NMEA_GPGLL      "$GPGLL"

struct satellite {

    satellite() = default;

    // SV PRN
    unsigned sv_prn = 0;

    // in degrees [0..90]
    unsigned elevation = 0;

    // in degrees [0..359]
    unsigned azimuth = 0;

    // in dB [0..99]
    unsigned snr = 0;

};

struct gps_data_t {

    gps_data_t() = default;

    // [0..23]
    unsigned hours = 0;

    // [0..59]
    unsigned minutes = 0;

    // [0..59]
    unsigned seconds = 0;

    // [0..999] not used
    unsigned milliseconds = 0;

    // [1..31]
    unsigned day = 0;

    // [1..12]
    unsigned month = 0;

    // e.g. 2020
    unsigned year = 0;

    // timezone hour difference [-13..13]
    unsigned timezone = 0;

    // 0 = invalid
    // 1 = GPS fix
    // 2 = Diff GPS fix
    unsigned quality = 0;

    // self explanatory
    unsigned satellites_in_view = 0;

    // [0..4]
    unsigned satellites_connected = 0;

    // decimal latitude [-180..180]
    double latitude = 0.0;

    // decimal longitude [-180..180]
    double longitude = 0.0;

    // in meters above sea level
    double altitude = 0.0;

    // speed in kilometers per hour
    double speed = 0.0;

    // in degrees
    double course = 0.0;

    // rate of position update in Hz
    double position_update_frequency;

    // up to 4 connected satellites
    satellite satellites[4];

};

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

    bool parse_message(const std::string &message, gps_data_t &info);

}

#endif // __NMEA_HPP

