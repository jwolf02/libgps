#ifndef __GPS_DATA_HPP
#define __GPS_DATA_HPP

struct gps_data {

    gps_data() = default;

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

};

#endif // __GPS_DATA_HPP
