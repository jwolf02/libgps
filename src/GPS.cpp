#include <GPS.hpp>
#include <nmea.hpp>
#include <unistd.h>

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
        throw std::runtime_error("gps device already started");
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
                } case nmea::GPGSA: {
                    nmea::parse_gpgsa(message, _data);
                    break;
                }
            }
        }
        _available = true;
    }
}