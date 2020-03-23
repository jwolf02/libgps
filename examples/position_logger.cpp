#include <iostream>
#include <cstdlib>
#include <GPS.hpp>
#include <csignal>
#include <unistd.h>
#include <cstdio>

volatile bool flag = true;

GPS gps;

void sig_handler(int signum) {
    if (signum == SIGINT) {
        std::cout << "terminated" << std::endl;
        gps.close();
        exit(EXIT_SUCCESS);
    }
}

int main(int argc, const char *argv[]) {

    signal(SIGINT, sig_handler);

    std::string devname = argc > 1 ? argv[1] : "/dev/ttyAMA0";

    gps.open(devname);
    gps.start();

    if (!gps.isOpen() || !gps.isStarted()) {
        std::cout << "failed to connect to gps device" << std::endl;
        exit(EXIT_FAILURE);
    }

    while (!gps.online()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    std::cout << "Logging GPS location" << std::endl;
    std::cout << "Press Ctrl+C to stop" << std::endl;
    while (flag) {
        while (!gps.available()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        gps.update();
        printf("latitude=%.8f longitude=%.8f\n", gps.latitude(), gps.longitude());
    }

    return EXIT_SUCCESS;
}