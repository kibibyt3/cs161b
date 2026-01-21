/**********************************************
 # Author:        Liv Callister and
 # Lab:           Discussion #2
 # Date:          18 January 2026
 # Description:   This program generates a strong, personalised password.
 # Input:         char word[], char number[], char symbol[]
 # Output:        char password[]
 # Sources:       en.cppreference.com
 #********************************************/

#include <iostream>
#include <cstring>

using namespace std;

int main() {
   char word[20];
   char number[5];
   char symbol[2];

   char password[20];

   bool number_is_valid = false;
   bool symbol_is_valid = false;

   cout << "Password Generator" << endl;
   cout << "------------------" << endl;
   cout << "Please input a word: ";

   cin >> word;
   sanitize_word(word);

   while (!number_is_valid) {
      cout << "Please input a number (max 4 digits): ";
      cin >> number;
      number_is_valid = validate_number(number);
   }

   while (!symbol_is_valid) {
      cout << "Please input a non-alphanumeric symbol: ";
      cin >> symbol;
      symbol_is_valid = validate_symbol(symbol);
   }
   symbol[1] = '\0';
   
   strcpy(password, word);
   strcat(password, number);
   strcat(password, symbol);
   cout << "Your strong password is: " << password;

   return 0;
}
