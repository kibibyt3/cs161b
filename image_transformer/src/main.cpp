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
#include <fstream>
#include <cstring>

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
   int max_val;
};

Image *image_new(int width, int height, int max_val);
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
   Image *image = NULL;

   return 0;
}

Image *image_new(int width, int height, int max_val) {
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
      image->max_val = max_val;
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

Image *p3_to_image(ifstream &inFile) {
   Image *image = NULL;
   bool is_valid = true;
   char header[3];
   int width = 0;
   int height = 0;
   int max_val = 0;
   size_t row = 0;
   size_t col = 0;
   int r = 0;
   int g = 0;
   int b = 0;

   inFile >> header;
   if (strncmp(header, "P3", 3) != 0) {
      is_valid = false;
   }

   inFile >> width;
   inFile >> height;
   inFile >> max_val;
   
   // After this, let's check validity just by seeing if image == NULL;
   if (is_valid) {
      image = image_new(width, height, max_val);
   }

   for (row = 0; row < static_cast<size_t>(height) && image != NULL; row++) {
      for (col = 0; col < static_cast<size_t>(width) && image != NULL; col++) {
         inFile >> r;
         inFile >> g;
         inFile >> b;

         if (r > max_val || g > max_val || b > max_val || inFile.fail()) {
            image_delete(image);
            image = NULL;
         } else {
            image->pixels[row][col] = Pixel { r, g, b };
         }
      }
   }

   return image;
}


void image_to_p3(Image *image, ofstream &outFile) {
   size_t row = 0;
   size_t col = 0;

   outFile << "P3" << endl;
   outFile << image->width << ' ' << image->height << endl;
   outFile << image->max_val << endl;

   for (row = 0; row < static_cast<size_t>(image->height); row++) {
      for (col = 0; col < static_cast<size_t>(image->width); col++) {
         cout << image->pixels[row][col].r << '\t'
            << image->pixels[row][col].g << '\t'
            << image->pixels[row][col].b << "\t\t";
      }
      cout << endl;
   }
}
