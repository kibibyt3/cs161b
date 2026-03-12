/******************************************************************************
# Author:
# Assignment:
# Date:
# Description:
# Input:
# Output:
# Sources:
#*****************************************************************************/
#include <iostream>
#include <cstdlib>
#include <cassert>

using namespace std;

struct Pixel {
   int r;
   int g;
   int b;
};

struct Image {
   Pixel **pixels;
   int width;
   int height;
};

Image *image_new(int width, int height);
void image_delete(Image *image);
Image *p3_to_image(ifstream &inFile);
void image_to_p3(Image *image, ofstream &outFile);
void horizontal_flip_image(Image *image);
void grayscale_image(Image *image);

const int DIMENSION_MAX = 10000;

// Name:
// Desc:
// Input:
// Output:
// Return:
int main() {
   Image *image;

   return 0;
}

Image *image_new(int width, int height) {
   size_t row = 0;
   Image *image;

   if (
         width < 1 || height < 1 ||
         width > DIMENSION_MAX || height > DIMENSION_MAX) {
      image = NULL;
   } else {
      Pixel **pixels = new Pixel*[height];
      for (row = 0; row < static_cast<size_t>(height); row++) {
         pixels[row] = new Pixel[width];
      }

      image = new Image;
      image->pixels = pixels;
      image->width = width;
      image->height = height;
   }

   return image;
}

void image_delete(Image *image) {
   size_t row = 0;

   for (row = 0; row < static_cast<size_t>(image->height); row++) {
      delete[] image->pixels[row];
   }
   delete[] image->pixels;
}

void horizontal_flip_image(Image *image) {
   size_t row = 0;
   size_t col = 0;
   Pixel temp_pixel;

   for (row = 0; row < static_cast<size_t>(image->height); row++) {
      for (col = 0; col < static_cast<size_t>(image->width / 2); col++) {
         temp_pixel = image->pixels[row][col];
         image->pixels[row][col] = image->pixels[row][image->width - 1 - col];
         image->pixels[row][image->width - 1 - col] = temp_pixel;
      }
   }
}

void grayscale_image(Image *image) {
   size_t row = 0;
   size_t col = 0;
   int sum = 0;

   for (row = 0; row < static_cast<size_t>(image->height); row++) {
      for (col = 0; col < static_cast<size_t>(image->width); col++) {
         sum = 0;
         sum += image->pixels[row][col].r;
         sum += image->pixels[row][col].g;
         sum += image->pixels[row][col].b;
         
         sum /= 3;
         image->pixels[row][col].r = sum;
         image->pixels[row][col].g = sum;
         image->pixels[row][col].b = sum;
      }
   }
}
