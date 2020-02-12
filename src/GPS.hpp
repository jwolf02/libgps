#ifndef __GPS_HPP
#define __GPS_HPP

#include <thread>
#include <string>
#include <atomic>
#include <serial.hpp>
#include <gps_data.hpp>
#include <thread>
#include <mutex>
#include <cmath>
#include <chrono>

class GPS {
public:

    GPS() = default;

    explicit GPS(const std::string &devname);

    GPS(const GPS &gps) = delete;

    ~GPS();

    GPS& operator=(const GPS &gps) = delete;

    void open(const std::string &devname);

    void close();

    void start();

    void stop();

    bool isOpen() const;

    bool isStarted() const;

    bool available() const;

    void update();

    double latitude() const;

    double longitude() const;

    double altitude() const;

    double speed() const;

    double course() const;

private:

    void process_messages();

    gps_data _userdata; // user accesses this

    gps_data _data; // internal updates get here

    serial_t _serial = 0;

    std::thread _thread;

    std::mutex _mtx;

    std::atomic_bool _running = { false };

    std::atomic_bool _available = { false };

};

#endif // __GPS_HPP

