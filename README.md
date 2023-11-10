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
Currently the upper freq. limit is about 9.8 MHz and it is achieved only using 
pi pico overclocking to 270MHz.

# Phased locked loop in C
The DCO uses phase locked loop principle programmed in C.

# Integer arithmetic
The DCO does *NOT* use any floating point operations - all time-critical 
instructions run in 1 CPU cycle.

# Radio transmitters
Owing to the meager frequency step, it is possible to use 3, 5, or 7th harmonics 
of generated frequency. Such solution completely cover all HF and low band up to 
65.8 MHz.

# For what?
This is an experimental project of amateur radio class and it is devised by me on 
the free will base in order to experiment with QRP narrowband digital modes.

# Feedback
I gracefully appreciate any thoughts or comments on that matter.

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
