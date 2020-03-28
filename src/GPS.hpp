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

/***
 * Wrapper class for a GPS device connected over serial UART
 * tested on a Raspberry Pi (Neo M6 GPS Module)
 * Once started this class spawns a new worker thread in the
 * background that constantly parses the messages that come
 * from the device.
 *
 * In order to keep the user data consistent the worker uses
 * his own struct that he updates and notifies the user
 * over the available() function that the user can update
 * his struct by calling update().
 */
class GPS {
public:

    // gps modi
    enum {
        NO_FIX = 0,
        GPS_FIX = 1,
        DIFF_GPS_FIX = 2
    };

    /*
     * compute the distance between two gps coordinates using the haversine distance
     */
    static double distance(double lat0, double lon0, double lat1, double lon1);

    GPS() = default;

    /***
     * open a new gps device
     */
    explicit GPS(const std::string &devname);

    GPS(const GPS &gps) = delete;

    ~GPS();

    GPS& operator=(const GPS &gps) = delete;

    /***
     * Open a new gps device, uses a serial terminal internally
     * communicating a 9600 baud
     * @param devname
     */
    void open(const std::string &devname);

    /***
     * close the gps device by terminating the worker thread
     * and closing the serial terminal
     */
    void close();

    /***
     * start the worker thread
     */
    void start();

    /***
     * stop the worker thread
     */
    void stop();

    /***
     * check if the gps device is opened
     * @return
     */
    bool isOpen() const;

    /***
     * check if the worker thread is running
     * @return
     */
    bool isStarted() const;

    /***
     * check if after parsing a message new data became availabe
     * @return
     */
    bool available() const;

    /***
     * update the user accessible data
     */
    void update();

    double latitude() const;

    double longitude() const;

    double altitude() const;

    double speed() const;

    double course() const;

    bool online() const;

    unsigned quality() const;

    unsigned satellitesInView() const;

    /***
     * get frequency of location update in Hz
     * @return
     */
    double locationUpdateFrequency() const;

    /***
     * send a command to the gps device
     * @param command
     */
    void write(const std::string &command);

private:

    void process_messages();

    gps_data_t _userdata; // user accesses this

    gps_data_t _data; // internal updates go here

    serial_t _serial = 0; // serial terminal (UART)

    std::thread _thread; // worker thread

    std::mutex _mtx; // protect data

    std::atomic_bool _running = { false }; // worker thread running flag

    std::atomic_bool _available = { false }; // update available

    std::chrono::system_clock::time_point _last_location_update;

    double _location_update_freq = 0.0;

};

#endif // __GPS_HPP

