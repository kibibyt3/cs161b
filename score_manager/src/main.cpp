/******************************************************************************
 # Author:        Liv Callister
 # Assignment:    Assignment 1
 # Date:          21 January 2026
 # Description:   This program operates on a user-input list of scores.
 # Input:         int scores[]
 # Output:        int scores[], int maximum, int minimum, double average,
 #                int (count above threshold)
 # Sources:       Assignment 1 s`pecifications, en.cppreference.com
 #****************************************************************************/
#include <iostream>
#include <cmath>
#include <cassert>

using namespace std;

void PrintScores(int arr[], size_t size);
int FindMax(int arr[], size_t size);
int FindMin(int arr[], size_t size);
double CalculateAverage(int arr[], size_t size);
int CountAboveThreshold(int arr[], size_t size, int threshold);
void tests();

// Name:    main()
// Desc:    This function handles high-level control flow.
// Input:   None
// Output:  int maximum, int minimum, double average,
//          int (count above threshold)
// Return:  None
int main() {
   int scores[100];
   int score = 0;
   size_t score_count = 0;
   int max = 0;
   int min = 0;
   double avg = 0.0;
   int threshold = 0;
   
   int menu_option = 0;
 
   // tests(); // Uncomment this line to run unit tests.
   cout << "Enter a list of scores." << endl;
   cout << "All scores must be positive numbers." << endl;

   do {
      cout << "Score #" << score_count + 1 << " (type -1 to finish entries): ";
      cin >> score;
      if (score != -1) {
         scores[score_count++] = score;
      }
   } while (score != -1);
   cout << endl;

   do {
      cout << "M A I N   M E N U" << endl;
      cout << "-----------------" << endl;

      cout << "1. Print all scores\n\
2. Find maximum score\n\
3. Find minimum score\n\
4. Calculate average\n\
5. Count scores above a certain threshold\n\
0. Quit" << endl << endl;
   
      cout << "Select option: ";
      cin >> menu_option;
      switch (menu_option) {
         case 1:
            PrintScores(scores, score_count);
            break;
         case 2:
            max = FindMax(scores, score_count);
            if (max == -1) {
               cout << "Cannot compute maximum of empty score list!" << endl;
            }
            else {
               cout << "The maximum score is: " << max << endl;
            }
            break;
         case 3:
            min = FindMin(scores, score_count);
            if (min == -1) {
               cout << "Cannot compute minimum of empty score list!" << endl;
            }
            else {
               cout << "The minimum score is: " << min << endl;
            }
            break;
         case 4:
            avg = CalculateAverage(scores, score_count);
            if (isnan(avg)) {
               cout << "Cannot compute average of empty score list!" << endl;
            }
            else {
               cout << "The average score is: " << avg << endl;
            }
            break;
         case 5:
            cout << "Enter lower threshold: ";
            cin >> threshold;
            cout << "There are ";
            cout << CountAboveThreshold(scores, score_count, threshold);
            cout << " scores greater than " << threshold << '.' << endl;
            break;
         case 0:
            cout << "Exiting score manager." << endl;
            break;
         default:
            cout << menu_option << " is not a valid menu option." << endl;
      }
      cout << "Press [ENTER] to continue..." << endl;

      // Clear cin so cin.get() doesn't get confused.
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
      cin.get();
   } while (menu_option != 0);
   
   
   return 0;
}

// Name:    PrintScores(int arr[], size_t size)
// Desc:    This function prints the values in an array.
// Input:   int array of scores, size_t (number of elements in array).
// Output:  int array of scores
// Return:  None
void PrintScores(int arr[], size_t size) {
   size_t i = 0;
   for (i = 0; i < size; i++) {
      cout << "Score #" << i + 1 << ": " << arr[i] << endl;
   }
}

// Name:    FindMax(int arr[], size_t size)
// Desc:    This function finds the maximum integer in an array.
// Input:   int array, size_t (number of elements in array).
// Output:  None
// Return:  int maximum
int FindMax(int arr[], size_t size) {
   size_t i = 0;
   int max = -1;
   for (i = 0; i < size; i++) {
      if (arr[i] > max) {
         max = arr[i];
      }
   }
   return max;
}

// Name:    FindMin(int arr[], size_t size)
// Desc:    This function finds the minimum integer in an array.
// Input:   int array, size_t (number of elements in array).
// Output:  None
// Return:  int minimum
int FindMin(int arr[], size_t size) {
   size_t i = 0;
   int min = -1;

   if (size > 0) {
      min = arr[0];
   }

   for (i = 1; i < size; i++) {
      if (arr[i] < min) {
         min = arr[i];
      }
   }
   return min;
}

// Name:    CalculateAverage(int arr[], size_t size)
// Desc:    This function finds the average value in an array.
// Input:   int array, size_t (number of elements in array).
// Output:  None
// Return:  double average
double CalculateAverage(int arr[], size_t size) {
   size_t i = 0;
   int sum = 0;
   double avg = 0.0;

   for (i = 0; i < size; sum += arr[i++]);
   
   avg = size < 1 ? NAN : static_cast<double>(sum) / static_cast<double>(size);
   return avg;
}

// Name:    CountAboveThreshold(int arr[], size_t size, int threshold)
// Desc:    This function finds the number of integers in an array greater than
//          some value.
// Input:   int array, size_t (number of elements in array),
//          int (minimum threshold, non-inclusive).
// Output:  None
// Return:  int count
int CountAboveThreshold(int arr[], size_t size, int threshold) {
   size_t i = 0;
   int count = 0;
   
   for (i = 0; i < size; i++) {
      if (arr[i] > threshold) {
         count++;
      }
   }

   return count;
}

// Name:    tests()
// Desc:    This function runs unit tests for the program, and will abort (with
//          a diagnostic) if one of the tests fails.
// Input:   None
// Output:  Diagnostic in stderr if a test fails
// Return:  None
void tests() {
   // Permissible amount of floating point error for test outcomes.
   const double FLOAT_ERROR = 0.01;
   int arr1[] = {4, 0, 9};
   int arr2[] = {1, 1, 3, 2};
   int arr3[] = {-1}; // We'll use this as a placeholer for an empty array.

   assert(FindMax(arr1, 3) == 9);
   assert(FindMax(arr2, 4) == 3);
   assert(FindMax(arr3, 0) == -1);
   
   assert(FindMin(arr1, 3) == 0);
   assert(FindMin(arr2, 4) == 1);
   assert(FindMin(arr3, 0) == -1);

   // Direct comparison of floating-point values is error-prone; let's give a
   // range of correct values instead.
   assert(
      CalculateAverage(arr1, 3) + FLOAT_ERROR > 13.0 / 3.0 &&
      CalculateAverage(arr1, 3) - FLOAT_ERROR < 13.0 / 3.0
   );
   assert(
      CalculateAverage(arr2, 4) + FLOAT_ERROR > 7.0 / 4 &&
      CalculateAverage(arr2, 4) - FLOAT_ERROR < 7.0 / 4
   );
   assert(isnan(CalculateAverage(arr3, 0)));

   assert(CountAboveThreshold(arr1, 3, 2) == 2);
   assert(CountAboveThreshold(arr2, 4, 1) == 2);
   assert(CountAboveThreshold(arr3, 0, 99) == 0);
}
