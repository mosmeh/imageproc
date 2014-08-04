#include <iostream>
#include <cmath>
#include <png++/png.hpp>

int main(int argc, char** argv) {
  png::image<png::gray_pixel> img(argv[1]);

  size_t width = img.get_width();
  size_t height = img.get_height();

  png::image<png::gray_pixel> tempimg(width, height);

  for (size_t y = 0; y < height; ++y) {
    for (size_t x = 0; x < width; ++x) {
      int sumw = 0, sumh = 0;

      for (int k = -1; k <= 1; ++k) {
        for (int l = -1; l <= 1; ++l) {
          png::gray_pixel pixel;
          if (x + l < 0 || x + l >= width || y + k < 0 || y + k >= height)
            pixel = img[y][x];
          else
            pixel = img[y + k][x + l];

          sumw += (l == 0 ? k * 2 : k) * pixel;
          sumh += (k == 0 ? l * 2 : l) * pixel;
        }
      }

      tempimg[y][x] = sqrt(sumw * sumw + sumh * sumh);
    }
  }
  tempimg.write("sobel.png");
}
