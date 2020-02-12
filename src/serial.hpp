#ifndef __SERIAL_HPP
#define __SERIAL_HPP

#include <cinttypes>
#include <string>

// alias file descriptor as serial type
typedef int serial_t;

namespace serial {

    serial_t open(const std::string &devname);

    void println(serial_t s, const std::string &line);

    void readln(serial_t s, std::string &line, uint32_t timeout_in_ms=1000);

    void close(serial_t s);

}

#endif // __SERIAL_HPP
