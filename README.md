# WaveShare-2Ch-CAN-FD-Hat
Instructions For WaveShare 2ch CAN-FD Hat Installation

## Table of Contents
Work In Progress, check back soon for updates...


## Cloning Repository and Dependency Installation
Clone Repository :
`git clone https://github.com/mitchdetailed/WaveShare-2Ch-CAN-FD-Hat.git`

Go into the directory and install the BCM2835 Library : 
```cd ./WaveShare-2Ch-CAN-FD-Hat/bcm2835-1.60/
sudo ./configure
sudo make
sudo make check
sudo make install```

Install WiringPi Library : 
`sudo apt-get install wiringpi`
`cd /tmp`
`wget https://project-downloads.drogon.net/wiringpi-latest.deb`
`sudo dpkg -i wiringpi-latest.deb`
`gpio -v`

Install Python3 Libraries :
`sudo apt-get update`
`sudo apt-get install python3-pip python3-pil python3-numpy`
`sudo pip3 install RPi.GPIO spidev python-can`

## Driver Installation
Mode A Driver Installation : 

Navigate to the Mode A Directory, If Repository was cloned into the home directory : 


