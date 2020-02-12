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

int main(int argc, const char *argv[]) {
    if (argc > 1 && std::string(argv[1]) == "--help") {
        std::cout << "Usage: " << argv[0] << " [UART device] [num_samples]" << std::endl;
        exit(EXIT_SUCCESS);
    }
    std::string devname = argc > 1 ? argv[1] : "/dev/ttyAMA0";
    uint32_t num_samples = argc > 2 ? strtol(argv[2], nullptr, 10) : 100;
    std::cout << "taking " << num_samples << " samples" << std::endl;

    std::vector<double> lats(num_samples);
    std::vector<double> lons(num_samples);

    gps.open(devname);
    gps.start();

    if (!gps.isOpen() || !gps.isStarted()) {
        std::cout << "failed to connect to gps device" << std::endl;
        exit(EXIT_FAILURE);
    }

    // wait for useful data forever
    gps.waitUntilOnline(GPS::NO_TIMEOUT);

    std::cout << "samples: " << 0 << std::flush;
    for (int i = 0; i < num_samples; ++i) {
        // wait until new data becomes available
        gps.waitUntilAvailable(GPS::NO_TIMEOUT);
        gps.update();
        lats[i] = gps.latitude();
        lons[i] = gps.longitude();
        std::cout << '\r' << "samples: " << (i + 1)<< std::flush;
    }
    std::cout << std::endl;

    double lat_mean = 0;
    double lon_mean = 0;
    for (int i = 0; i < num_samples; ++i) {
        lat_mean += lats[i];
        lon_mean += lons[i];
    }

    lat_mean /= num_samples;
    lon_mean /= num_samples;

    double mean_dist = 0.0;
    double max_dist = -INFINITY;

    for (int i = 0; i < num_samples; ++i) {
        double dist = std::abs(GPS::distance(lats[i], lons[i], lat_mean, lon_mean));
        mean_dist += dist;
        max_dist = std::max(max_dist, dist);
    }

    printf("latitude (mean)=%.8f, longitude (mean)=%.8f\n", lat_mean, lon_mean);
    printf("distance (mean)=%.3f meters, distance (max)=%.3f meters\n", mean_dist / num_samples, max_dist);

    return EXIT_SUCCESS;
}