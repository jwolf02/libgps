#include <iostream>
#include <GPS.hpp>
#include <test.hpp>

void test_distance() {
    ASSERT_EQUAL_FLOAT(GPS::distance(49.123, 8.123, 79.321, -12.345), 3451512.656966);
    ASSERT_EQUAL_FLOAT(GPS::distance(123, 456, 789, 10), 4412243.147101);
    ASSERT_EQUAL_FLOAT(GPS::distance(0, 0, 0, 0), 0);
}

int main() {

    test_distance();

    std::cout << "test finished successfully" << std::endl;

    return EXIT_SUCCESS;

}