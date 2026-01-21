/************************************************************************
 # Author:        Liv Callister and Milo Lubin
 # Lab:           Discussion #2
 # Date:          18 January 2026
 # Description:   This program generates a strong, personalised password.
 # Input:         char word[], char number[], char symbol[]
 # Output:        char password[]
 # Sources:       en.cppreference.com
 #**********************************************************************/

#include <iostream>
#include <cstring>
#include <iomanip>
#include <limits>

bool number_is_valid(char number[]);
bool symbol_is_valid(char symbol[]);

using namespace std;

// Use size_t instead of int; int isn't guaranteed to be able to store the
// maximum possible array index.
const size_t MAX_WORD_LENGTH = 20;
const size_t MAX_NUMBER_LENGTH = 4;
const size_t MAX_SYMBOL_LENGTH = 1;

int main() {
   // Leave space for the null-terminating character on all of these.
   char word[MAX_WORD_LENGTH + 1];
   char number[MAX_NUMBER_LENGTH + 1];
   char symbol[MAX_SYMBOL_LENGTH + 1];

   char password[MAX_WORD_LENGTH + MAX_NUMBER_LENGTH + MAX_SYMBOL_LENGTH + 1];

   cout << "Password Generator" << endl;
   cout << "------------------" << endl;
   cout << "Please input a word (max " << MAX_WORD_LENGTH << " letters): ";

   // Use setw() to avoid any buffer overflows.
   cin >> setw(MAX_WORD_LENGTH + 1) >> word;

   // This clears the cin buffer so that an overflow doesn't cause issues the
   // next time cin is used (a downside of using setw()).
   cin.ignore(numeric_limits<streamsize>::max(), '\n');

   // Use do-while since we don't know the contents of an uninitialized array.
   do {
      cout << "Please input a number (max " << MAX_NUMBER_LENGTH << " digits): ";
      cin >> setw(MAX_NUMBER_LENGTH + 1) >> number;
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
   }
   while (!number_is_valid(number));

   do {
      cout << "Please input a non-alphanumeric symbol: ";
      cin >> setw(MAX_SYMBOL_LENGTH + 1) >> symbol;
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
   }
   while (!symbol_is_valid(symbol));

   strcpy(password, word);
   strcat(password, number);
   strcat(password, symbol);
   cout << "Your strong password is: " << password << endl;

   return 0;
}

// Name: number_is_valid(char number[])
// Desc: This function ensures that a string is only composed of digits.
// Input: char number[]
// Output: None
// Return: bool is_valid
bool number_is_valid(char number[]) {
   size_t length = strlen(number);
   size_t i = 0;
   bool is_valid = true;

   // Check digits
   for (i = 0; i < length; i++) {
      if (!isdigit(number[i])) {
         is_valid = false;
      }
   }

   return is_valid;
   
   // The below approach is really elegant but perhaps less readable.
   // for (i = 0; i < length && isdigit(number[i]); i++);
   // return i == length;
}

// Name: symbol_is_valid(char symbol[])
// Desc: This function ensures that a string of length 1 has a non-alphanumeric
// character.
// Input: char symbol[]
// Output: None
// Return: bool is_valid
bool symbol_is_valid(char symbol[]) {
   return !isalnum(symbol[0]);
}
