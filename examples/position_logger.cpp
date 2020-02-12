#include <iostream>
#include <cstdlib>
#include <GPS.hpp>
#include <csignal>
#include <unistd.h>

volatile bool flag = true;

GPS gps;

void sig_handler(int signum) {
    if (signum == SIGINT) {
        gps.close();
        exit(EXIT_SUCCESS);
    }
}

int main(int argc, const char *argv[]) {

    signal(SIGINT, sig_handler);

    std::string devname = argc > 1 ? argv[1] : "/dev/ttyAMA0";

    gps.open(devname);
    gps.start();

    if (!gps.isOpen()) {
        std::cout << "cannot connect to gpsd" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::cout << "Logging GPS location" << std::endl;
    std::cout << "Press Ctrl+C to stop" << std::endl;
    while (flag) {
        while (!gps.available())
            usleep(250);
        gps.update();
        std::cout << "latitude=" << gps.latitude() << ", longitude=" << gps.longitude() << std::endl;
    }

    return EXIT_SUCCESS;
}