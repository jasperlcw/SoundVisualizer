# CMPT 433 Final Project - LED Visualizer
Team members: Jasper Wong, Jaspal Raman, Andrew Liang, Erin Desaulniers
## Project Idea (note that the zen cape might be different between members):
* Baseline idea is to work with LEDs and to control it with hardware buttons and through the local network through UDP(embedded with react front end)
* Buttons will be the soldered joystick on the BeagleBone
* Current light mode will be stored on the EEPROM
* Change the IP config to have a static IP instead of DHCP on the WiFi dongle
* 16x32 RGB LED panel -> could do an audio visualizer
RGB rainbow
* Bar audio visualizer (single color -> then progress to multi-color)
* Current time on the RGB panel
* AM/PM and 24 hr clock
* 2 claps to toggle lights on/off
* Will need to write our own audio drivers

## List of parts to borrow:
* 16x32 RGB LED panel (http://www.adafruit.com/products/420) 
* Microphone (https://ca.robotshop.com/products/breakout-board-electret-microphone)
    * Do we need an amp for this?
* WiFi dongle (https://www.amazon.ca/Edimax-EW-7811Un-150Mbps-Raspberry-Supports/dp/B003MTTJOY) 

Milestones subject to change
## February 16th Milestone 1
* Plan out modules
* Display current time on the LED board
* Use joystick and UDP to change between AM/PM and 24h clock
* Connection to local network through WiFi dongle and assign a static IP address (system configuration)

## March 16th Milestone 2
* Audio drivers
* Double clap toggle on/off
* If sound is playing the lights turn on

## April 5th Demo Day

