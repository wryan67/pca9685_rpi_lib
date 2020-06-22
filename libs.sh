#!/bin/sh

if [ ! -f /usr/lib/libwiringPiPca9685.so ];then
  echo Please install https://github.com/Reinbert/pca9685.git
  exit 2
fi

if [ ! -f /usr/lib/libwiringPi.so ];then
  echo Please install https://github.com/Reinbert/pca9685.git
  exit 2
fi

exit 0
