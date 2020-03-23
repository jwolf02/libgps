#include <GPS.hpp>
#include <nmea.hpp>
#include <cmath>

#define EARTH_RADIUS    (6367137.)
#define RADIANS(x)      ((x) * M_PI / 180.0)
#define DEGREES(x)      ((x) * 180.0 / M_PI)

double GPS::distance(double lat0, double lon0, double lat1, double lon1) {
    lat0 = RADIANS(lat0);
    lon0 = RADIANS(lon0);
    lat1 = RADIANS(lat1);
    lon1 = RADIANS(lon1);


    double d_lat = (lat1 - lat0);
    double d_lon = (lon1 - lon0);
    /*double u = std::sin(d_lat / 2.0);
    double v = std::sin(d_lon / 2.0);
    double a = u * u + std::cos(lat0) * std::cos(lat1) * v * v;
    return 2 * EARTH_RADIUS * 2.0 * std::atan2(std::sqrt(a), std::sqrt(1 - a)); */
    return 2 * EARTH_RADIUS * asin(sqrt(sin(d_lat / 2) * sin(d_lat / 2) + cos(lat0) * cos(lat1) * sin(d_lon / 2) * sin(d_lon / 2)));
}

GPS::GPS(const std::string &devname) {
    open(devname);
}

GPS::~GPS() {
    close();
}

void GPS::open(const std::string & devname) {
    if (isOpen()) {
        close();
    }
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
    while (_flag.test_and_set())
        ;
     _userdata = _data;
    //memcpy(&_userdata, &_data, sizeof(gps_data_t));
    _available = false;
    _flag.clear();
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
    return quality() != NO_FIX && latitude() != 0.0 && longitude() != 0.0;
}

unsigned GPS::quality() const {
    return _userdata.quality;
}

unsigned GPS::satellitesInView() const {
    return _userdata.satellites_in_view;
}

double GPS::locationUpdateFrequency() const {
    return _location_update_freq;
}

void GPS::write(const std::string &command) {
    serial::println(_serial, command);
};

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

        while (_flag.test_and_set())
            ;
        auto mtype = nmea::message_type(message);
        switch (mtype) {
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
                //nmea::parse_gpgsv(message, _data);
                break;
            } case nmea::GPGLL: {
                nmea::parse_gpgll(message, _data);
                break;
            }
            default: break;
        }

        _flag.clear();

        if (mtype == nmea::GPGGA || mtype == nmea::GPRMC || mtype == nmea::GPGLL) {
            const auto tp = std::chrono::system_clock::now();
            _location_update_freq = 1.0 / std::chrono::duration_cast<std::chrono::milliseconds>(tp - _last_location_update).count();
            _last_location_update = tp;
        }
        _available = true;
    }
}