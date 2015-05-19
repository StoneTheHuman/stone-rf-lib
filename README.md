# StoneRFLib

This is a library that enables you to let AVR-devices communicate with each other through RF receiver and transmitter modules. The modules I personally use are the XD-FST (sender) and the XD-RF-5V (receiver). It should also work with other cheap RF modules.

Currently the supported AVR-devices are:
- atmega328p
- attiny85

And the supported clock rates are:
- 1 MHz
- 16 MHz

Keep in mind that if you are using multiple transmitter devices, you will have to make sure they're not transmitting at the same time because this will not work. I am currently building a master/slave system which I might upload later.

RFReceiveLib is for the device with the RF receiver and RFSendLib is for the device with the RF transmitter.

Refer to the README's in the lib-folders for more specific information.
