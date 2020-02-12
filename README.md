# libgps
C++ library to interface with GPS receiver over UART with efficient NMEA parser

## Usage
mkdir build  
cd build  
cmake ..  
make -j2 && make test  
make install (optional)  

## Examples
This repository includes two useful examples for how to use this software.

### 1. position_logger
./position_logger \[UART device\]  
Very simple program that prints the GPS location on the console.

### 2. compute_uncertainity
./compute_uncertainity \[UART device\] \[num_samples\]  
This is a little more sophisticated. It takes a number of samples from the  
GPS device and computes the mean, the standard deviation and the maximum outlier  
from the samples and prints it on the console. One can use this to determine how  
a given position might deviate from the real location.