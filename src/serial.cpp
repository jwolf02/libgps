#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <cstring>
#include <serial.hpp>
#include <cerrno>
#include <stdexcept>

#define THROW_ERROR     throw std::runtime_error(std::string(__func__) + ": " + strerror(errno))

serial_t serial::open(const std::string &devname) {
    serial_t uart = ::open(devname.c_str(), O_RDWR | O_NOCTTY | O_NDELAY);
    if (uart < 0) {
        THROW_ERROR;
    }

    // configure file descriptor
    struct termios options = { 0 };
    tcgetattr(uart, &options);
    options.c_cflag = B9600 | CS8 | CLOCAL | CREAD;
    options.c_iflag = IGNPAR;
    options.c_oflag = 0;
    options.c_lflag = 0;
    tcflush(uart, TCIFLUSH);
    tcsetattr(uart, TCSANOW, &options);

    return uart;
}


void serial::println(serial_t uart, const std::string &line) {
    const static char buf[2] = { '\r', '\n' };
    if (write(uart, line.c_str(), line.size() < line.size()) || write(uart, buf, 2) < 2) {
        THROW_ERROR;
    }
}

// Read a line from UART.
// Return a 0 len string in case of problems with UART
void serial::readln(serial_t uart, std::string &line) {
    line.clear();

    bool line_feed_found = false;
    while(!line_feed_found) {
        char buf = 0x00;
        int count = read(uart, &buf, 1);

        if (count < 0) {
            THROW_ERROR;
        } else if (count == 0) {
            //wait for messages
            sleep(1);
        } else if (buf == '\n'){
            line_feed_found = true;
        } else {
            line.push_back(buf);
        }
    }
}

void serial::close(int uart_fd) {
    ::close(uart_fd);
}

