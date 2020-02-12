#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <cstring>
#include <serial.hpp>
#include <cerrno>
#include <stdexcept>

#define ERROR     throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + ": " + strerror(errno))

serial_t serial::open(const std::string &devname) {
    serial_t uart = ::open(devname.c_str(), O_RDWR | O_NOCTTY | O_NDELAY);
    if (uart < 0) {
        ERROR;
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
        ERROR;
    }
}

// Read a line from UART
void serial::readln(serial_t uart, std::string &line, uint32_t timeout_in_ms) {
    line.clear();

    fd_set set;
    FD_ZERO(&set);
    FD_SET(uart, &set);

    struct timeval tv = { (long int) timeout_in_ms / 1000, (long int) (timeout_in_ms % 1000) * 1000 };

    bool line_feed_found = false;
    do {
	    // wait for the file descriptor to
	    // become readable
        auto ret = select(FD_SETSIZE, &set, nullptr, nullptr, &tv);
        switch (ret) {
            case -1: { // an error occurred
                ERROR;
            } case 0: { // timeout
                continue;
            } case 1: { // fd ready
                break;
            }
        }

	// read up to 255 bytes from UART
	char buf[256] = { 0 };
	auto count = read(uart, buf, 256);
	if (count < 0) {
		ERROR;
	} else if (count != 0) {
		// check if there has been read an end of line
		auto p = strchr(buf, '\n');
		if (p != nullptr) {
			line_feed_found = true;
		}
		line.append(buf, count);
	}
    } while (!line_feed_found);

    line.pop_back(); // remove '\n'
    line.pop_back(); // remove '\r'
}

void serial::close(int uart_fd) {
    ::close(uart_fd);
}