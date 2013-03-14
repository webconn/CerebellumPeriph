CerebellumPeriph
================

Peripheral board - a part of Cerebellum project (source code)

Cerebellum PeriphBoard is a board by DIMRobotics used for peripheral devices such a servos, DC motors, relays etc. The board contains ATmega128 controller, 5V TRACO POWER 4A regulator, 4 relays (max current 2A per relay), ULN2803 (8 hi-current channels), 8 10-bit ADC channels, 2 UART, 1 I2C. Generally, it has 25 GPIO (excluding ULN, communication interfaces and relays), each planned as 3-pin connector (signal, 5V power and ground), according to most devices' pinout.

This repository is created for library sources (for ADC, servos, relays etc). Configuring and image building made by Antares uC buildsystem (http://github.com/nekromant/antares).

First use
=========

Clone this repository, in the directory run:

make antares

After this action (in case if you have all toolchains and packages installed and configured), you can run:

make deploy 

to build sources and upload image to on-board uC.
