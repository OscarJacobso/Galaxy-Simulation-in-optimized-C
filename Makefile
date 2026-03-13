CC ?= gcc
CFLAGS ?= -O3 -std=c11 -Wall -Wextra
CPPFLAGS ?=
LDFLAGS ?=
LDLIBS ?= -lm

# Optional extra graphics linker flags (example: -lX11 on Linux)
GRAPHICS_LIBS ?=

# URLs for downloading graphics helper files
GRAPHICS_HEADER_URL ?=
GRAPHICS_SOURCE_URL ?=

TARGETS := galsim galsimCallOpt galsimopt

.PHONY: all clean download-graphics

all: $(TARGETS)

galsim: galsim.c
	$(CC) $(CPPFLAGS) $(CFLAGS) $< -o $@ $(LDFLAGS) $(LDLIBS) $(GRAPHICS_LIBS)

galsimCallOpt: galsimCallOpt.c
	$(CC) $(CPPFLAGS) $(CFLAGS) $< -o $@ $(LDFLAGS) $(LDLIBS) $(GRAPHICS_LIBS)

galsimopt: galsimopt.c
	$(CC) $(CPPFLAGS) $(CFLAGS) $< -o $@ $(LDFLAGS) $(LDLIBS) $(GRAPHICS_LIBS)

download-graphics:
	@if [ -z "$(GRAPHICS_HEADER_URL)" ]; then \
		echo "Set GRAPHICS_HEADER_URL to a direct graphics.h URL."; \
		exit 1; \
	fi
	@command -v curl >/dev/null 2>&1 || { echo "curl is required for download-graphics"; exit 1; }
	@curl -L "$(GRAPHICS_HEADER_URL)" -o graphics.h
	@if [ -n "$(GRAPHICS_SOURCE_URL)" ]; then \
		curl -L "$(GRAPHICS_SOURCE_URL)" -o graphics.c; \
	fi
	@echo "Downloaded graphics helper files."

clean:
	rm -f $(TARGETS) *.o result.gal