#include <GPS.hpp>
#include <nmea.hpp>
#include <unistd.h>
#include <cstring>
#include <cmath>

#define EARTH_RADIUS    (63671370.)
#define RADIANS(x)      ((x) * M_PI / 180.0)
#define DEGREES(x)      ((x) * 180.0 / M_PI)

template <typename T>
static constexpr T sqr(T x) {
    return x * x;
}

double GPS::distance(double lat0, double lon0, double lat1, double lon1) {
    lat0 = RADIANS(lat0);
    lon0 = RADIANS(lon0);
    lat1 = RADIANS(lat1);
    lon1 = RADIANS(lon1);

    double d_lat = (lat1 - lat0);
    double d_lon = (lon1 - lon0);
    double a = sqr(std::sin(d_lat / 2.0)) + std::cos(lat1) * sqr(std::sin(d_lon / 2.0));
    double c = 2.0 * std::atan2(std::sqrt(a), std::sqrt(1.0 - a));
    return EARTH_RADIUS * c;
}

GPS::GPS(const std::string &devname) {
    open(devname);
}

GPS::~GPS() {
    close();
}

void GPS::open(const std::string & devname) {
    if (isOpen())
        close();
    _serial = serial::open(devname);
}

void GPS::close() {
    if (isOpen()) {
        stop();
        serial::close(_serial);
    }
    _serial = 0;
}

void GPS::start() {
    if (_running)
        throw std::runtime_error("gps device already running");
    _running = true;
    _thread = std::thread(&GPS::process_messages, this);
}

void GPS::stop() {
    if (isStarted()) {
        _running = false;
        _thread.join();
    }
}

bool GPS::isOpen() const {
    return _serial > 0;
}

bool GPS::isStarted() const {
    return _running;
}

bool GPS::available() const {
    return _available;
}

void GPS::update() {
    std::lock_guard<std::mutex> lock(_mtx);
     _userdata = _data;
    //memcpy(&_userdata, &_data, sizeof(gps_data_t));
    _available = false;
}

bool GPS::waitUntilAvailable(uint32_t timeout_in_ms, uint32_t sleeptime_in_ms) {
    uint32_t counter = 0;
    while (!available() && (timeout_in_ms == 0 || counter < timeout_in_ms)) {
        usleep(sleeptime_in_ms * 1000);
        counter += sleeptime_in_ms;
    }
    return available();
}

bool GPS::waitUntilOnline(uint32_t timeout_in_ms, uint32_t sleeptime_in_ms) {
    uint32_t counter = 0;
    while (!online() && (timeout_in_ms == 0 || counter < timeout_in_ms)) {
        usleep(sleeptime_in_ms * 1000);
        counter += sleeptime_in_ms;
    }
    return online();
}

double GPS::latitude() const {
    return _userdata.latitude;
}

double GPS::longitude() const {
    return _userdata.longitude;
}

double GPS::altitude() const {
    return _userdata.altitude;
}

double GPS::speed() const {
    return _userdata.speed;
}

double GPS::course() const {
    return _userdata.course;
}

bool GPS::online() const {
    return quality() != NO_FIX;
}

unsigned GPS::quality() const {
    return _userdata.quality;
}

void GPS::write(const std::string &command) {
    serial::println(_serial, command);
};

#include <iostream>

void GPS::process_messages() {
    std::string message;

    while (_running) {
        // read message from UART and check the checksum
        serial::readln(_serial, message);
        if (!nmea::valid_checksum(message)) {
            // most often the first message is read incompletely so we do not want
            // to throw an error for that
            continue;
        }

        // determine message type and parse accordingly
        {
            std::lock_guard<std::mutex> lock(_mtx);
            auto t = nmea::message_type(message);
            std::cout << t << std::endl;
            switch (nmea::message_type(message)) {
                case nmea::GPGGA: {
                    nmea::parse_gpgga(message, _data);
                    break;
                } case nmea::GPRMC: {
                    nmea::parse_gprmc(message, _data);
                    break;
                } case nmea::GPZDA: {
                    nmea::parse_gpzda(message, _data);
                    break;
                } case nmea::GPGSV: {
                    nmea::parse_gpgsv(message, _data);
                    break;
                }
            }
        }
        _available = true;
    }
}