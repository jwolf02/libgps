#include <GPS.hpp>
#include <iostream>
#include <vector>
#include <csignal>
#include <unistd.h>
#include <algorithm>

volatile bool flag = true;

GPS gps;

void sig_handler(int signum) {
    if (signum == SIGINT) {
        gps.close();
        exit(EXIT_SUCCESS);
    }
}

template <typename It>
double sum(It begin, It end, double init=0) {
    double accum = init;
    for (auto it = begin; it != end; ++it) {
        accum = accum + *it;
    }
    return accum;
}

int main(int argc, const char *argv[]) {

    std::string devname = argc > 1 ? argv[1] : "/dev/ttyAMA0";
    int num_samples = argc > 2 ? strtol(argv[2], nullptr, 10) : 100;
    std::cout << "taking " << num_samples << " samples" << std::endl;

    std::vector<double> lats(num_samples);
    std::vector<double> lons(num_samples);

    gps.open(devname);
    gps.start();

    if (!gps.isOpen() || !gps.isStarted()) {
        std::cout << "failed to connect to gps device" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::cout << "samples: " << 0 << std::endl;
    for (int i = 0; i < num_samples; ++i) {
        while (!gps.available()) {
            usleep(250000);
        }
        gps.update();
        lats.push_back(gps.latitude());
        lons.push_back(gps.longitude());
        std::cout << '\r' << "samples: " << (i + 1)<< std::endl;
    }

    double lat_mean = sum(lats.begin(), lats.end()) / num_samples;
    double lon_mean = sum(lons.begin(), lons.end()) / num_samples;

    double s = 0;
    for (int i = 0; i < num_samples; ++i) {
        s += std::abs(GPS::distance(lats[i], lons[i], lat_mean, lon_mean));
    }

    printf("latitude (mean)=%.8f longitude (mean)=%.8f distance (avg)=%0.6f\n", lat_mean, lon_mean, s / num_samples);

    return EXIT_SUCCESS;
}