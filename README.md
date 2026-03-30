# Galaxy Simulation in C

This project contains three C implementations of an N-body galaxy simulation :

- `galsim.c`
- `galsimCallOpt.c`
- `galsimopt.c`

## Requirements

You need the following tools installed:

- A C compiler (`gcc` or `clang`)
- `make`
- `curl` (only needed if you use `make download-graphics`)

The simulation source files include `graphics.h`, so you also need the graphics helper package (`graphics.h` and, if required by your source, `graphics.c`) available in this project directory.

## Build

Build all binaries:

```bash
make
```

Build one binary:

```bash
make galsim
make galsimCallOpt
make galsimopt
```

Clean build outputs:

```bash
make clean
```

## Download graphics helper files

If you found `graphics.h` online, you can download it directly with Make:

```bash
make download-graphics GRAPHICS_HEADER_URL="https://example.com/graphics.h"
```

If the package also includes `graphics.c`, download both files:

```bash
make download-graphics \
	GRAPHICS_HEADER_URL="https://example.com/graphics.h" \
	GRAPHICS_SOURCE_URL="https://example.com/graphics.c"
```

## Linker flags for graphics libraries

Depending on your graphics package and OS, you may need extra linker flags.
Example on Linux/X11:

```bash
make GRAPHICS_LIBS="-lX11"
```

## Run

Each binary expects five arguments:

1. Number of particles (`N`)
2. Input `.gal` file path
3. Number of timesteps
4. Delta timestep
5. Graphics on/off (`1` or `0`)

Example:

```bash
./galsim 1000 input.gal 100 0.001 1
```