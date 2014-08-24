fdim
====

FDim is a GUI to compute fractal dimension of a grayscale image. The fractal dimension can be seen as a measure of roughness of the image. Therefore the program can be used to, e.g., compare different textures.

Fractal algorithms supported:
- Capacity Dimension
- Information Dimension
- Correlation Dimension
- Probability Dimension


## Compile

To compile on linux, MAC or Windows (mingw):
- Install and compile wxWidgets
- Edit the appropriate makefile to point to WXCONFIG
- You may also want to point the makefile to UPX (for packing the binary)
- Run the appropriate makefile, e.g. make -f Makefile.linux

The binary 'fdim' will be in the build subdirectory. It may be that
you have to create the build subdirectory before compiling (mkdir build).

Homepage: http://reuter.mit.edu/software/fdim

Author: Martin Reuter

