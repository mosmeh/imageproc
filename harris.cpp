#include <cmath>
#include <iostream>
#include <png++/png.hpp>

const double K = 0.06;
const int THRESHOLD = 1000000;

int main(int argc, char** argv) {
  png::image<png::gray_pixel> img(argv[1]);

  size_t width = img.get_width();
  size_t height = img.get_height();

  double *dx = new double[width * height],
         *dy = new double[width * height],
         *dxy = new double[width * height];

  // Preswitt filter
  for (size_t y = 0; y < height; ++y) {
    for (size_t x = 0; x < width; ++x) {
      int sumw = 0, sumh = 0;

      for (int l = -1; l <= 1; ++l) {
        for (int k = -1; k <= 1; ++k) {
          png::gray_pixel pixel;
          if (x + k < 0 || x + k >= width || y + l < 0 || y + l >= height)
            pixel = img[y][x];
          else
            pixel = img[y + l][x + k];

          sumw += k * pixel;
          sumh += l * pixel;
        }
      }

      dx[x + y * width] = sumw * sumw;
      dy[x + y * width] = sumh * sumh;
      dxy[x + y * width] = sqrt(static_cast<double>(sumw * sumw + sumh * sumh));
    }
  }

  // Gaussian filter
  for (size_t y = 0; y < height; ++y) {
    for (size_t x = 0; x < width; ++x) {
      double sumdx = 0, sumdy = 0, sumdxy = 0;

      for (int l = -1; l <= 1; ++l) {
        for (int k = -1; k <= 1; ++k) {
          double pdx, pdy, pdxy;
          if (x + k < 0 || x + k >= width || y + l < 0 || y + l >= height) {
            pdx = dx[x + y * width];
            pdy = dy[x + y * width];
            pdxy = dxy[x + y * width];
          } else {
            pdx = dx[(x + k) + (y + l) * width];
            pdy = dy[(x + k) + (y + l) * width];
            pdxy = dxy[(x + k) + (y + l) * width];
          }

          double coefficient = l == 0 || k == 0 ? 0.125 : 0.0625;
          sumdx += coefficient * pdx;
          sumdy += coefficient * pdy;
          sumdxy += coefficient * pdxy;
        }
      }

      dx[x + y * width] = sumdx;
      dy[x + y * width] = sumdy;
      dxy[x + y * width] = sumdxy;
    }
  }

  double* harris = new double[width * height];

  // Harris operator
  for (size_t y = 0; y < height; ++y) {
    for (size_t x = 0; x < width; ++x) {
      double pdx = dx[x + y * width];
      double pdy = dy[x + y * width];
      double pdxy = dxy[x + y * width];
      harris[x + y * width] = pdx * pdy - pdxy * pdxy - K * (pdx + pdy) * (pdx + pdy);
    }
  }

  delete[] dx, dy, dxy;

  png::image<png::rgb_pixel> colorimg(argv[1]);

  // select maxima
  for (size_t y = 0; y < height; ++y) {
    for (size_t x = 0; x < width; ++x) {

      if (harris[x + y * width] > THRESHOLD) {
        bool ismaximum = true;

        for (int l = -1; l <= 1; ++l) {
          for (int k = -1; k <= 1; ++k) {
            if (x + k >= 0 && x + k < width && y + l >= 0 && y + l < height) {
              if (harris[x + y * width] < harris[(x + k) + (y + l) * width]) {
                ismaximum = false;
                break;
              }
            }
          }
        }

        if (ismaximum) {
          std::cout << x << ", " << y << std::endl;

          for (int l = -1; l <= 1; ++l) {
            for (int k = -1; k <= 1; ++k) {
              if (x + k >= 0 && x + k < width && y + l >= 0 && y + l < height) {
                colorimg[y + l][x + k] = png::rgb_pixel(255, 0, 0);
              }
            }
          }
        }
      }
    }
  }

  delete[] harris;

  colorimg.write("harris.png");
}
