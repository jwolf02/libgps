#ifndef __GPS_DATA_HPP
#define __GPS_DATA_HPP

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

    // up to 4 connected satellites
    satellite satellites[4];

};

#endif // __GPS_DATA_HPP
