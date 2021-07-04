This Repository contains a collection of applications to facilitate the integration XLC series thermocouple dataloggers into user applications.

This repository, has so far been tested in Ubuntu 18.04.

Usage: 
1. Clone the repository
2. Navigate to an applciation directory (example: simple_superloop) and run make in the command line 
3. Configure the serial port with stty. (see below for example usage) 
4. launch the application binary located in the build directory inside the application directory.

Configuring the serial port. 
The following command configures the serial port /dev/ttyS0 with 9600 baud, 8 data bits, 1 stop bit, no parity, no hardware flow control, local echo disabled.
"stty -F /dev/ttyS0 9600 cs8 -cstopb -parenb -ixon -echo" 
