#ifndef __GPS_HPP
#define __GPS_HPP

#include <thread>
#include <string>
#include <atomic>
#include <serial.hpp>
#include <gps_data.hpp>
#include <mutex>

class GPS {
public:

	// default contructor
	GPS() = default;

	explicit GPS(const std::string &devname);

	~GPS();

	void open(const std::string &devname);

	void close();

	bool isOpen() const;

    void start();

    void stop();

    bool isStarted() const;

    bool available() const;

    void update(gps_data &data);

private:

    void process_nmea_messages();

	std::thread _t;

    std::atomic_bool _running = { false };

	gps_data _data;

	std::atomic_bool _updated = { false };

	std::mutex _mtx;

    serial_t _serial = 0;

};

#endif // __GPS_HPP

