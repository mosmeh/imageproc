CXX = g++
CXXFLAGS = -O3 --inline

LIBPNG_CXXFLAGS = $(shell libpng-config --cppflags)
LIBPNG_LDFLAGS  = $(shell libpng-config --ldflags)

.PHONY: all

all: prewitt sobel gaussian harris

prewitt: prewitt.cpp
	$(CXX) $(CXXFLAGS) $< -o $@ $(LIBPNG_CXXFLAGS) $(LIBPNG_LDFLAGS)

sobel: sobel.cpp
	$(CXX) $(CXXFLAGS) $< -o $@ $(LIBPNG_CXXFLAGS) $(LIBPNG_LDFLAGS)

gaussian: gaussian.cpp
	$(CXX) $(CXXFLAGS) $< -o $@ $(LIBPNG_CXXFLAGS) $(LIBPNG_LDFLAGS)

harris: harris.cpp
	$(CXX) $(CXXFLAGS) $< -o $@ $(LIBPNG_CXXFLAGS) $(LIBPNG_LDFLAGS)
