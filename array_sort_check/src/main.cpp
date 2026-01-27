/******************************************************************************
# Author:            Stevie Ranslam & Liv Callister
# Assignment:        Discussion 3: Is Array Sorted? (CS161B)
# Date:              (Jan 11th, 2026) to (Jan 12th, 2026)
# Description:       Write a program that reads a list of integers from input
#                    into an array and outputs "yes" if the list is sorted in
#                    ascending order between two provided positions. Otherwise,
#                    output "no". The first input specifies the number of items
#                    in the list. The next set of inputs is the list. The last
#                    two inputs are the start and end positions (inclusive,
#                    between 0 and "count", where "count" is the number of
#                    values in the array). Assume the list contains no more
#                    than 20 integers and position 0 is the first element.
#
# Input:             int array[], int (max index), int range[2], char (y/n)
# Output:            Prompts for the above inputs, error messages, message
#                    detailing whether the indexed section of the array is
#                    sorted
# Sources:           Assignment 1 specifications, en.cppreference.com
#*****************************************************************************/

// Program does these steps:
//    1. Ask the user how many variables (up to 20) to input
//    2. Ask the user to input variables into array
//    3. Ask the user to input two provided positions
//    4. Check if they're sorted in "ascending" order

#include <iostream>
#include <string>
#include <limits>

using namespace std;

const int INDEX_SIZE = 20; // Will be used later in readInput() function
int readInt(string prompt); // Reads input from a prompt
int readInput(int range); 
void inputArray(int arr[], int num);
void inputIndex(int arr[], int maxIndex);
bool isAscending(int start, int end, int arr[]);
void boolLogic(bool checkThis); // Checks the result from the above function

int main() {
   bool numAscending = false;
   int integers[INDEX_SIZE];
   int indexes[2];
   int maxIndex = 0;

   cout << "Welcome to our integer array program!" << endl;
   maxIndex = readInput(INDEX_SIZE);
   inputArray(integers, maxIndex);
   inputIndex(indexes, maxIndex);
   numAscending = isAscending(indexes[0], indexes[1], integers);
   boolLogic(numAscending);

   return 0;
}

//   Name: readInt()
//   Desc: This function reads an int and returns it.
//  Input: string prompt
// Output: Possible Error Message
// Return: int (user int)
int readInt(string prompt) {
   int tempInt = 0;
   cout << prompt;
   while (!(cin >> tempInt)) {
      cout << "Invalid input! Retry entry:\n>> ";
      cin.clear();
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
   }
   cin.ignore(numeric_limits<streamsize>::max(), '\n');
   return tempInt;
}

//   Name: doubleCheck()
//   Desc: This function verifies with the user that they inputted what they
//         wanted to input.
//  Input: string prompt, 
// Output: Asking if the entered info is correct
// Return: bool (user says yes)
bool doubleCheck(string prompt) {
   char userChar = '\0';
   bool userSaysYes = false;
   cout << prompt;
   while (!(cin >> userChar) || !(userChar == 'y' || userChar == 'n')) {
      cout << "Invalid input! Retry entry:\n>> ";
      cin.clear();
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
   }
   cin.ignore(numeric_limits<streamsize>::max(), '\n');

   if (userChar == 'y') {
      userSaysYes = true;
   }
   else if (userChar == 'n') {
      userSaysYes = false;
   }
   else {
      // If we arrived here, something went very wrong. Let's abort.
      abort();
   }

   return userSaysYes;
}

//   Name: readInput()
//   Desc: This function reads the input and returns it
//  Input: int range
// Output: Possible Error Message
// Return: int userOption
int readInput(int range) {
   int userOption = 0;
   bool verify = false;
   while (!verify) {
      cout << "First, enter the amount of integers you want (up to " <<
         range << "):";
      userOption = readInt("\n>> ");
      while (userOption < 1 || userOption > range) {
         cout << "Invalid! Enter a number between 1 and " << range << "!";
         userOption = readInt("\n>> ");
      }
      cout << "You want to input " << userOption << " numbers. ";
      verify = doubleCheck("Is this correct? (y/n)\n>> ");
      
      if (verify) {
         cout << "Good, let's go to the next step!" << endl;
      }
      else {
         cout << "That's okay! Try again!" << endl;
      }
   }

   return (userOption - 1);
}

//   Name: inputArray()
//   Desc: This function prompts the user to input scores and 
//         put them into an array
//  Input: An int array[] and an int number of integers
// Output: Possible Error Message
// Return: None
void inputArray(int arr[], int maxIndex) {
   bool verify = false;
   while (!verify) {
      cout << "Enter the integers you want to input!" << endl << ">> ";
      for (int i = 0; i <= maxIndex; i++) {
         cin >> arr[i];
      }
      cin.ignore(numeric_limits<streamsize>::max(), '\n');

      cout << "Your integers are ";
      for (int i = 0; i <= maxIndex; i++) {
         if (i == maxIndex) {
            cout << "& " << arr[i];
         }
         else {
            cout << arr[i] << ", ";
         }
      }
      cout << ". ";

      verify = doubleCheck("Is this correct? (y/n)\n>> ");

      if (verify == true) {
         cout << "Good, let's go to the next step!" << endl;
      }
      else {
         cout << "That's okay! Try again!" << endl;
      }
   }
}

//   Name: inputIndex()
//   Desc: This function prompts the user to input two indexes
//  Input: An int array[2] and int maximum value
// Output: Possible Error Message
// Return: None
void inputIndex(int arr[], int maxIndex) {
   bool rangeIsValid = false;
   bool verify = false;

   while (!rangeIsValid || !verify) {
      rangeIsValid = false;
      verify = false;
      cout << "Enter start and end index:\n" << ">> ";
      for (int i = 0; i < 2; i++) {
         cin >> arr[i];
      }
      cin.ignore(numeric_limits<streamsize>::max(), '\n');

      if (arr[0] < 0 || arr[0] >= arr[1] || arr[1] > maxIndex) {
         cout << "Indexes won't work! Redo it!" << endl;
      } else {
         rangeIsValid = true;
      }
      
      if (rangeIsValid) {
         verify = doubleCheck("You want to compare every number between "
            + to_string(arr[0]) + " and " + to_string(arr[1])
            + ". Is that correct? (y/n)\n>> ");
      }

      if (rangeIsValid && verify) {
         cout << "Good, let's go to the next step!" << endl;
      }
      else {
         cout << "That's okay, try again!" << endl;
      }
   }
}

//   Name: isAscending()
//   Desc: This function prompts the user to input two indexes
//  Input: indexes int start and int end, and int arr[]
// Output: Possible Error Message
// Return: bool tempCheck
bool isAscending(int start, int end, int arr[]) {
   bool tempCheck = true; // True until proven otherwise!
   
   for (int i = start; i < end; i++) {
      if (arr[i] > arr[i + 1]) {
         tempCheck = false;
      }
   }

   return tempCheck;
}

//   Name: boolLogic()
//   Desc: This function gets the return value from isAscending and acts
//         accordingly
//  Input: bool checkThis
// Output: Whether if the index within the array is sorted or not
// Return: bool tempCheck
void boolLogic(bool checkThis) {
   if (checkThis) {
      cout << "Yes. That's sorted in ascending order!" << endl;
   }
   else {
      cout << "No. That's NOT sorted in ascending order!" << endl;
   }
}

