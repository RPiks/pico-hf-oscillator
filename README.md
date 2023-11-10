# pico-DCO
# Digital controlled radio frequency oscillator for Raspberry Pi Pico 

The library for Raspberry Pi Pico includes the headers and source code and all 
necessary build files to build a custom application which turns pico into
precise PLL digital frequency oscillator of the portion of HF radio spectrum
1.1 to 9.4MHz with high resolution.

# Precise frequency resolution
The library provides 1 Hz frequency resolution in initialize function. This
resolution can be easily increased to 23 millihertz, which is limited by
24-bit register which is used in algorithm.
Currently the upper freq. limit is about 9.4 MHz and it is achieved only using 
pi pico overclocking to 270MHz.

# Phased locked loop in C
The DCO uses phase locked loop principle programmed in C.

# *NO* additional hardware
The DCO provides the output signal on the GPIO pin. However if you want to
transmit the signal, you should calculate and provide a lowpass filter of
appropriate frequency. Please also figure out whether you possess rights
to emit radio frequency energy on desired frequency.

# Dual-core
The DCO uses extensively the secodary core of the pico. The first one is for
your ideas how to modulate the DCO to obtain a desired signal.
The DCO does *NOT* use any floating point operations - all time-critical 
instructions run in 1 CPU cycle.

# Radio transmitters
Owing to the meager frequency step, it is possible to use 3, 5, or 7th harmonics 
of generated frequency. The practical resolution will be quite the same - far
below 1 Hz. Such solution completely cover all HF and low band up to 65.8 MHz.

# For what?
This is an experimental project of amateur radio hobby and it is devised by me 
in order to experiment with QRP narrowband digital modes.
I am licensed radio amateur who is keen on experiments in the area of the 
digital modes on HF. 
My QRZ page is https://www.qrz.com/db/R2BDY

![DSC_0269](https://github.com/RPiks/pico-DCO/assets/47501785/dfa3ae65-5ceb-46cb-a4ec-bf7b5defc5ec)

# Feedback
I gracefully appreciate any thoughts or comments on that matter.
I strongly appreciate if you use this project as a part of your one.
I have plans of building a transceiver on the base of this library.

# Quick-start
1. Install Raspberry Pi Pico SDK. Configure environment variables. Test whether 
it is built successfully.

2. git clone this repository. cd pico-DCO ; ./build.sh
Check whether output file ./build/pico-DCO.uf2 appears.

3. Prepare the surrogate antenna (if you possess an SSB receiver) or pin-out
for an oscilloscope or a spectrum analyser. The default output pin is GPIO6.

4. Load the .uf2 file (2) into the Pico.

5. Initialy the running frequency is 9.4 MHz.

6. Set any other frequency ranging from 1.1 to 9.4 MHz by #define GEN_FRQ_HZ and build the project. 

7. Provide the feedback by clicking like on the github page of the project.


Cheers,
Roman Piksaykin, amateur callsign R2BDY
https://www.qrz.com/db/R2BDY
