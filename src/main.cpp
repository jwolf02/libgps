#include <iostream>
#include <cstdlib>
#include <GPS.hpp>
#include <csignal>

bool flag = true;

void sig_handler(int signum) {
    if (signum == SIGINT) {
        flag = false;
    }
}

int main(int argc, const char *argv[]) {
    std::string dev("/dev/ttyAMAO");
    if (argc > 1) {
        std::string arg(argv[1]);
        if (arg == "--help") {
            std::cout << "Usage: " << argv[0] << " [UART device (default=/dev/ttyAMAO)]" << std::endl;
            return EXIT_SUCCESS;
        } else {
            dev = arg;
        }
    }

    signal(SIGINT, sig_handler);

    GPS gps(dev);
    gps.start();

    gps_data data;

    std::cout << "Logging GPS location" << std::endl;
    std::cout << "Press Ctrl+C to stop" << std::endl;
    while (flag) {
        if (gps.available()) {
            gps.update(data);
        }
        std::cout << '\r' << data.latitude << ' ' << data.longitude << std::flush;
    }

    return EXIT_SUCCESS;
}