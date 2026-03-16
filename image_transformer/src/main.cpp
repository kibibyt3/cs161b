/******************************************************************************
# Author:      Liv Callister
# Assignment:  Assignment 3
# Date:        16 March 2026
# Description: This program performs transformations on a .ppm image.
# Input:       char *(out filename), char *(in filename), bool (make flipped),
#              bool (make greyscale)
# Output:      output file
# Sources:     en.cppreference.com, Assignment 3 Specifications
#*****************************************************************************/
#include <iostream>
#include <cstdlib>
#include <cassert>
#include <fstream>
#include <cstring>
#include <limits>

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
Image *p3_to_image(ifstream &in_file);
void image_to_p3(Image *image, ofstream &out_file);
void horizontal_flip_image(Image *image);
void grayscale_image(Image *image);
bool yes_no_prompt();

const int DIMENSION_MAX = 10000;

// Name:    int main()
// Desc:    This function controls the high-level control flow of the program.
// Input:   char *in_filename, char *out_filename, bool make_flipped,
//          bool make_greyscale
// Output:  out_file, prompts
// Return:  0 on success, 1 on error opening in_file, 2 on error opening
//          out_file, 3 on error parsing in_file
int main() {
   Image *image = NULL;
   ifstream in_file;
   ofstream out_file;
   int exit_code = 0;
   char in_filename[50];
   char out_filename[50];
   bool make_flipped = false;
   bool make_greyscale = false;
   bool in_file_opened = false;

   cout << "Welcome to liv's image transformer!" << endl;
   cout << "Please enter the name of the .ppm image you'd like to transform.";
   cout << endl;
   cout << "Note: Image must have a magic number of P3." << endl;
   cout << ">> ";
   cin >> in_filename;
   cout << "Please enter the name of the output file." << endl;
   cout << ">> ";
   cin >> out_filename;
   cin.ignore(numeric_limits<streamsize>::max(), '\n');

   in_file.open(in_filename);
   if (in_file) {
      image = p3_to_image(in_file);
      in_file.close();
      in_file_opened = true;
   } else {
      cout << "ERROR: Could not open input file " << in_filename
         << '.' << endl;
      exit_code = 1;
   }

   if (in_file_opened) {
      cout << "Would you like to convert " << in_filename
         << " to greyscale? (y/N)" << endl;
      make_greyscale = yes_no_prompt();

      cout << "Would you like to flip " << in_filename
         << " horizontally? (y/N)" << endl;
      make_flipped = yes_no_prompt();
   }

   if (image != NULL) {
      out_file.open(out_filename);
      if (out_file) {
         if (make_flipped) {
            horizontal_flip_image(image);
         }
         
         if (make_greyscale) {
            grayscale_image(image);
         }

         image_to_p3(image, out_file);
         out_file.close();
      } else {
         cout << "ERROR: Could not open output file " << out_filename
            << '.' << endl;
         exit_code = 2;
      }
   } else if (in_file_opened) {
      cout << "ERROR: Could not parse input file " << in_filename
         << '.' << endl;
      exit_code = 3;
   }

   if (image != NULL) {
      image_delete(image);
   }

   if (exit_code == 0) {
      cout << "Transformation(s) executed successfully; output available "
         << "at " << out_filename << '.' << endl;
   }

   return exit_code;
}

// Name:    Image *image_new(int width, int height, int max_val)
// Desc:    This function creates a new Image.
// Input:   int (width of Image), int (height of Image), int (max value of each
//          component of each Pixel)
// Output:  None
// Return:  Pointer to Image on success, NULL on error
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

// Name:    image_delete(Image *image)
// Desc:    This function deletes an Image.
// Input:   Image *image
// Output:  None
// Return:  None
void image_delete(Image *image) {
   size_t row = 0;

   for (row = 0; row < static_cast<size_t>(image->height); row++) {
      delete[] image->pixels[row];
   }
   delete[] image->pixels;
}

// Name:    horizontal_flip_image(Image *image)
// Desc:    This function flips an Image horizontally.
// Input:   Image *image
// Output:  None
// Return:  None
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

// Name:    greyscale_image(Image *image)
// Desc:    This function converts an Image to greyscale.
// Input:   Image *image
// Output:  None
// Return:  None
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

// Name:    Image *p3_to_image(ifstream &in_file)
// Desc:    This image converts a P3 .ppm filestream to an Image.
// Input:   ifstream &(.ppm filestream)
// Output:  None
// Return:  Image *image
Image *p3_to_image(ifstream &in_file) {
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

   in_file >> header;
   if (strncmp(header, "P3", 3) != 0) {
      is_valid = false;
   }

   in_file >> width;
   in_file >> height;
   in_file >> max_val;
   
   // After this, let's check validity just by seeing if image == NULL;
   if (is_valid) {
      image = image_new(width, height, max_val);
   }

   for (row = 0; row < static_cast<size_t>(height) && image != NULL; row++) {
      for (col = 0; col < static_cast<size_t>(width) && image != NULL; col++) {
         in_file >> r;
         in_file >> g;
         in_file >> b;

         if (r > max_val || g > max_val || b > max_val || in_file.fail()) {
            image_delete(image);
            image = NULL;
         } else {
            image->pixels[row][col] = Pixel { r, g, b };
         }
      }
   }

   return image;
}

// Name:    image_to_p3(Image *image, ofstream &outFile)
// Desc:    This function converts an Image to a P3 .ppm file.
// Input:   Image *image, ofstream &(.ppm output filestream)
// Output:  None
// Return:  None
void image_to_p3(Image *image, ofstream &out_file) {
   size_t row = 0;
   size_t col = 0;

   out_file << "P3" << endl;
   out_file << image->width << ' ' << image->height << endl;
   out_file << image->max_val << endl;

   for (row = 0; row < static_cast<size_t>(image->height); row++) {
      for (col = 0; col < static_cast<size_t>(image->width); col++) {
         out_file << image->pixels[row][col].r << '\t'
            << image->pixels[row][col].g << '\t'
            << image->pixels[row][col].b << "\t\t";
      }
      out_file << endl;
   }
}

// Name:    bool yes_no_prompt()
// Desc:    This function prints a (y/N) prompt.
// Input:   char (input character)
// Output:  Prompt
// Return:  bool (true if yes, false if no)
bool yes_no_prompt() {
   char ch = '\0';
   cout << ">> ";
   cin.get(ch);
   if (ch != '\n') {
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
   }
   ch = static_cast<char>(toupper(ch));
   while (ch != '\n' && ch != 'Y' && ch != 'N') {
      cout << "Please type 'y' or 'n'." << endl;
      cout << ">> ";
      cin.get(ch);
      if (ch != '\n') {
         cin.ignore(numeric_limits<streamsize>::max(), '\n');
      }
      ch = static_cast<char>(toupper(ch));
   }
   return ch == 'Y';
}
