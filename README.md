DigitalColourOrgan
==================

This project is done in guidance of Prof. Dhananjay V. Gadre. and with my collegue Rohan Seth at 
Centre for Electronic Design and Technology(CEDT), Netaji Subhas Institute of Technology, New Delhi.

Implemented using Digital Filters on ARM Cortex-M3 microcontroller. Three filters of low, mid and high
frequency range are used. Audio input is provided through a 3.5mm Audio Jack with a clamping circuit 
as output through audio jack can be negative which can not sampled by ADC(Analog to Digital Convertor).
IQmath Library is used to perform floating point calculations. And as we generally do have more flash 
memory than SRAM so filter coefficients are stored in Flash to achieve high order filters.
