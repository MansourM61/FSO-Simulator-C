# FSO Simulator in C
A free-space optical (FSO) commuication link simulator written in C language

Written by Mojtaba Mansour Abadi

The code is written in C language using Code::Blocks. The code performs a real time simulation by generating psedurandom bit sequence on the fly. The bit sequence is changed into electrical signal which modulates a laser later on. The output of the laser is launched into a free space optical channel where laser output is exposed to propagation, fog/smoke, turbulence, pointing error, and miscellaneous effects. The optical beam then is received by an aperture and then converted to electrical signal using a photo-receiver. Then received bits are extracted and compared to transmit bits in order to measure bit-error-rate (BER) of the system. The turbulence effect is based on Log-Normal model and for fog/smoke, Kim model is utilised.

Code::Blocks ide can be downloaded from [here](http://www.codeblocks.org/).
For GUIs and plots DISLIN a free library is used. DISLIN can be downloaded for both windows and linux from [here](https://www.mps.mpg.de/dislin).
To perform FFT operation, GSL an open source library is used. GSL is included in linux and for windows it can be downloaded from [here](https://www.gnu.org/software/gsl/).

## Windows
- Add the following libraries to the build linker setting:
DISLIN Library: "dismg.a"
OS Library: "user32", "gdi32", and "opengl32"
GSL Library: "gsl folder\lib\libgsl.a", and "gsl folder\lib\libgslcblas.a"
- Add DISLIN header file folder to search directories compiler

## Linux
- Add the following libraries to the build linker setting:
DISLIN Library: "libdislin.so"
OS Library: "pthread"
GSL Library: "libgsl", and "libgslcblas"
- Add DISLIN header file folder to search directories compiler

![Screenshot](Screenshot.jpg)
