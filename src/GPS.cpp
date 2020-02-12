#include <GPS.hpp>
#include <nmea.hpp>

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

void GPS::process_messages() {
    std::string message;
    message.resize(256);

    while (_running) {
        // read message from UART and check the checksum
        serial::readln(_serial, message);
        if (!nmea::valid_checksum(message)) {
            throw std::runtime_error("invalid checksum encountered");
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