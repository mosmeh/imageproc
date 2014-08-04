CXX := g++
CXXFLAGS := -O3 --inline

LIBPNG_CXXFLAGS := $(shell libpng-config --cppflags)
LIBPNG_LDFLAGS  := $(shell libpng-config --ldflags)

PREFIX := bin
$(shell mkdir -p $(PREFIX))

TARGETS = prewitt sobel gaussian harris

.PHONY: all

all: $(addprefix $(PREFIX)/, $(TARGETS))

$(PREFIX)/prewitt: prewitt.cpp
	$(CXX) $(CXXFLAGS) $< $(OUTPUT_OPTION) $(LIBPNG_CXXFLAGS) $(LIBPNG_LDFLAGS)

$(PREFIX)/sobel: sobel.cpp
	$(CXX) $(CXXFLAGS) $< $(OUTPUT_OPTION) $(LIBPNG_CXXFLAGS) $(LIBPNG_LDFLAGS)

$(PREFIX)/gaussian: gaussian.cpp
	$(CXX) $(CXXFLAGS) $< $(OUTPUT_OPTION) $(LIBPNG_CXXFLAGS) $(LIBPNG_LDFLAGS)

$(PREFIX)/harris: harris.cpp
	$(CXX) $(CXXFLAGS) $< $(OUTPUT_OPTION) $(LIBPNG_CXXFLAGS) $(LIBPNG_LDFLAGS)
