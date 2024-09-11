PCA9685 Utility
---------------
PCA9685 servo driver utility for the Raspberry Pi, using wiringPi

## Prerequisite
Please install the pca9685 driver from @Reinbert.  Also, this utility is based on [WiringPi](https://github.com/WiringPi/WiringPi), so, you'll need make sure you have WiringPi installed before you can succesfully compile this library.  

> [https://github.com/Reinbert/pca9685.git](https://github.com/Reinbert/pca9685.git)


## Install
To compile this library, navigate into the src folder and use the make utility to compile 
and install the library.

    $ make && make install

## Usage

    $ pca9685
    usage: pwm [-a 00-FF] [-c cap] [-p 0-15] [-f freq] [-m freq] [-x freq] [-e]
    a = hexadecimal i2c address of the PCA9685 ($ gpio i2cd); default 0x40
    p = PCA9685 pin number (0-15)
    e = use even split pulse, default is trigger pulse which is for servos
    c = frequency cap (40-1526).  The default is 50, which is for servos
    m = min freq
    x = max freq
    f = frequency 0-100
    Mode:   f     - set frequency and exit. (override m & x)        m & x - run test over range.


## Example
Examples using the utility:

    $ pca9685 -p 0 -f 230
    $ pca9685 -p 1 -f 325

    $ pca9685 -p 0 -f 163
    $ pca9685 -p 1 -f 397
