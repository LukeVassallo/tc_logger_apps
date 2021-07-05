# Thermocouple Datalogger Integration Software
This Repository contains a collection of applications to facilitate the integration XLC series thermocouple dataloggers into user applications.

<figure align="center">
<img src="https://wwww.lukevassallo.com/thermocouple_datalogger/images/xlc4_top_view.jpg" width="65%" height="65%" alt="4 Channel thermocouple datalogger top view">
<figcaption>Fig.1 - Top view Quad Channel Thermocouple Datalogger.</figcaption>
</figure>

This repository, has so far been tested in Ubuntu 18.04.

### Basic usage: 
1. Clone the repository and change into the directory.
    ```sh
    git clone https:\\www.github.com\lukevassallo\tc_logger_apps.git
    cd tc_logger_apps
    ```
2. Run make at the command line to build everything.
    ```sh
    make all
    ```
3. Configure the serial port with stty. **Replace /dev/ttyS0 and 9600 with the appropriate port and baudrate.** (see below for explanation) 
    ```sh
    stty -F /dev/ttyS0 9600 cs8 -cstopb -parenb -ixon -echo"
    ```
4. Start with application simple_parser. The binary will be located in the build directory. **Replace /dev/ttyS0 the appropriate port.**
    ```sh
    ./parser/build/parser /dev/ttyS0
    ```
5. Optionally download the Thermocouple Configuration and Logging Software to effortlessly get acquainted with the device. 
    ```sh
    source ./scripts/get_lnx_sw.sh
    ```

### Configuring the serial port: 
The following command configures the serial port /dev/ttyS0 with 9600 baud, 8 data bits, 1 stop bit, no parity, no hardware flow control, no local echo.
stty -F /dev/ttyS0 9600 cs8 -cstopb -parenb -ixon -echo
