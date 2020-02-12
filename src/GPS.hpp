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

    enum {
        NO_FIX = 0,
        GPS_FIX = 1,
        DIFF_GPS_FIX = 2
    };

    static constexpr uint32_t NO_TIMEOUT = 0;

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

    bool waitUntilAvailable(uint32_t timeout_in_ms, uint32_t sleeptime_in_ms=250);

    bool waitUntilOnline(uint32_t timeout_in_ms, uint32_t sleeptime_in_ms=250);

    double latitude() const;

    double longitude() const;

    double altitude() const;

    double speed() const;

    double course() const;

    bool online() const;

    unsigned quality() const;

private:

    void process_messages();

    gps_data_t _userdata; // user accesses this

    gps_data_t _data; // internal updates get here

    serial_t _serial = 0;

    std::thread _thread;

    std::mutex _mtx;

    std::atomic_bool _running = { false };

    std::atomic_bool _available = { false };

};

#endif // __GPS_HPP

