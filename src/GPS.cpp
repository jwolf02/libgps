#include <GPS.hpp>
#include <nmea.hpp>
#include <serial.hpp>
#include <gps_data.hpp>

void GPS::process_nmea_messages() {
    while (_running) {
        std::string message;
        message.resize(256);

        // read message from UART and check the checksum
        serial::readln(_serial, message);
        if (!nmea::valid_checksum(message)) {
            throw std::runtime_error("invalid checksum encountered");
        }

        // determine message type and parse accordingly
        {
            std::lock_guard<std::mutex> lock(_mtx);
            nmea::parse_message(message, _data);
        }
        _updated = true;
    }
}

GPS::GPS(const std::string &devname) {
    open(devname);
}

GPS::~GPS() {
    close();
}

void GPS::open(const std::string & devname) {
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
    _running = true;
    _t = std::thread(&GPS::process_nmea_messages, this);
}

void GPS::stop() {
    if (isStarted()) {
        _running = false;
        _t.join();
    }
}

bool GPS::isOpen() const {
    return _serial > 0;
}

bool GPS::isStarted() const {
    return _running;
}

bool GPS::available() const {
    return _updated;
}

void GPS::update(gps_data &data) {
    std::lock_guard<std::mutex> lock(_mtx);
    data = _data;
    _updated = false;
}