#include <iostream>
#include <nmea.hpp>
#include <cstdlib>
#include <gps_data.hpp>
#include "test.hpp"

static void test_gpgga() {
    gps_data_t data;
    const std::string gpgga_message("$GPGGA,001038.00,4124.8963,N,08151.6838,W,2,04,5.4,354.682,M,-26.574,M,7.0,0138*7F");

    ASSERT(nmea::valid_checksum(gpgga_message));
    ASSERT_EQUAL(nmea::message_type(gpgga_message), nmea::GPGGA);
    nmea::parse_gpgga(gpgga_message, data);

    ASSERT_EQUAL(0, data.hours);
    ASSERT_EQUAL(10, data.minutes);
    ASSERT_EQUAL(38, data.seconds);
    ASSERT_EQUAL_FLOAT(41.414938, data.latitude);
    ASSERT_EQUAL_FLOAT(-81.861396, data.longitude);
    ASSERT_EQUAL(2, data.quality);
    ASSERT_EQUAL(4, data.satellites_connected);
    ASSERT_EQUAL_FLOAT(354.682, data.altitude);
}

static void test_gprmc() {
    gps_data_t data;
    const std::string gprmc_message("$GPRMC,081836,A,9000.0100,S,01234.5678,E,000.05,360.0,130998,011.3,E*56");

    ASSERT(nmea::valid_checksum(gprmc_message));
    ASSERT_EQUAL(nmea::message_type(gprmc_message), nmea::GPRMC);
    nmea::parse_gprmc(gprmc_message, data);

    ASSERT_EQUAL(8, data.hours);
    ASSERT_EQUAL(18, data.minutes);
    ASSERT_EQUAL(36, data.seconds);
    ASSERT_EQUAL_FLOAT(-90.000166, data.latitude);
    ASSERT_EQUAL_FLOAT(12.576130, data.longitude);
    ASSERT_EQUAL_FLOAT(0.05 * 1.852, data.speed)
    ASSERT_EQUAL_FLOAT(360.0, data.course);
}

static void test_gpzda() {
    gps_data_t data;
    const std::string gpzda_message("$GPZDA,235959,31,12,1999,01,00*41");

    ASSERT(nmea::valid_checksum(gpzda_message));
    ASSERT_EQUAL(nmea::message_type(gpzda_message), nmea::GPZDA);
    nmea::parse_gpzda(gpzda_message, data);

    ASSERT_EQUAL(23, data.hours);
    ASSERT_EQUAL(59, data.minutes);
    ASSERT_EQUAL(59, data.seconds);
    ASSERT_EQUAL(31, data.day);
    ASSERT_EQUAL(12, data.month);
    ASSERT_EQUAL(1999, data.year);
    ASSERT_EQUAL(1, data.timezone);
}

static void test_gpgsv() {
    gps_data_t data;
    const std::string gpgsv_message("$GPGSV,3,1,11,03,03,111,00,04,15,270,00,06,01,010,00,13,06,292,00*74");

    ASSERT(nmea::valid_checksum(gpgsv_message));
    ASSERT_EQUAL(nmea::message_type(gpgsv_message), nmea::GPGSV);
    nmea::parse_gpgsv(gpgsv_message, data);

    ASSERT_EQUAL(11, data.satellites_in_view);

    ASSERT_EQUAL(3, data.satellites[0].sv_prn);
    ASSERT_EQUAL(3, data.satellites[0].elevation);
    ASSERT_EQUAL(111, data.satellites[0].azimuth);
    ASSERT_EQUAL(0, data.satellites[0].snr);

    ASSERT_EQUAL(4, data.satellites[1].sv_prn);
    ASSERT_EQUAL(15, data.satellites[1].elevation);
    ASSERT_EQUAL(270, data.satellites[1].azimuth);
    ASSERT_EQUAL(0, data.satellites[1].snr);

    ASSERT_EQUAL(6, data.satellites[2].sv_prn);
    ASSERT_EQUAL(1, data.satellites[2].elevation);
    ASSERT_EQUAL(10, data.satellites[2].azimuth);
    ASSERT_EQUAL(0, data.satellites[2].snr);

    ASSERT_EQUAL(13, data.satellites[3].sv_prn);
    ASSERT_EQUAL(6, data.satellites[3].elevation);
    ASSERT_EQUAL(292, data.satellites[3].azimuth);
    ASSERT_EQUAL(0, data.satellites[3].snr);
}

void test_gpgll() {
    gps_data_t data;
    const std::string gpgsv_message("$GPGSV,3,1,11,03,03,111,00,04,15,270,00,06,01,010,00,13,06,292,00*74");

    // TODO: implement
}

int main(int argc, const char *argv[]) {

    test_gpgga();
    test_gprmc();
    test_gpzda();
    test_gpgsv();
    test_gpgll();

    std::cout << "test finished successfully" << std::endl;

    return EXIT_SUCCESS;
}