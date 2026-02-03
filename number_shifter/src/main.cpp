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
#include <numeric>
#include <limits>
#include <iomanip>
#include <cassert>
#include <cctype>
using namespace std;

void shift_nums(int arr[], size_t count, size_t shift);
void shift_chain(int arr[], size_t count, size_t shift, size_t start);
void tests();

const size_t MAX_RECOMMENDED_NUM_COUNT = 100;

// Name:
// Desc:
// Input:
// Output:
// Return:
int main() {
   size_t num_count = 0;
   size_t shift = 0;
   size_t i;
   char confirm_char = '\0';
   int *nums = NULL;

   //tests();

   cout << "This program will shift a list of numbers to the right." << endl;
   do {
      cout << "To begin, please enter the amount of numbers in your list.";
      cout << endl;
      cout << ">> ";
      cin >> num_count;
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
      if (num_count > MAX_RECOMMENDED_NUM_COUNT) {
         cout << "Allocating this much memory may not be possible." << endl;
         cout << "Are you sure you want to create a list with ";
         cout << num_count << " numbers? (y/N)" << endl;
         cout << ">> ";
         cin.get(confirm_char);
         if (confirm_char != '\n') {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
         }
      }
   } while (num_count > MAX_RECOMMENDED_NUM_COUNT
         && (confirm_char != 'y' && confirm_char != 'Y'));
   nums = new int[num_count];

   confirm_char = '\0';
   cout << "Please enter the list of numbers, separated by spaces:" << endl;
   cout << ">> ";
   for (i = 0; i < num_count; i++) {
      cin >> nums[i];
   }
   cin.ignore(numeric_limits<streamsize>::max(), '\n');

   do {
      cout << "Please enter how many shifts should be performed:" << endl;
      cout << ">> ";
      cin >> shift;
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
      if (shift > MAX_RECOMMENDED_NUM_COUNT) {
         cout << "Are you sure you want to perform ";
         cout << shift << " shifts? (y/N)" << endl;
         cout << ">> ";
         cin.get(confirm_char);
         if (confirm_char != '\n') {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
         }
      }
   } while (shift > MAX_RECOMMENDED_NUM_COUNT
         && (confirm_char != 'y' && confirm_char != 'Y'));

   // There are only num_count many possible permutations.
   shift %= num_count;

   shift_nums(nums, num_count, shift);
   cout << "Here are your shifted numbers:" << endl;
   for (i = 0; i < num_count - 1; i++) {
      cout << nums[i] << ", ";
   }
   cout << nums[num_count - 1] << '.' << endl;
   
   delete[] nums;

   return 0;
}

// Name:    shift_nums(int arr[], size_t count, size_t shift)
// Desc:    Shift an array count many times.
//          Time complexity: O(n)
// Input:   int array, size_t (array count), size_t (shift degree)
// Output:  None
// Return:  None
void shift_nums(int arr[], size_t count, size_t shift) {
   size_t greatest_common_divisor = 0;
   size_t i = 0;

   
   greatest_common_divisor = gcd(count, shift);
   for (i = 0; i < greatest_common_divisor; i++) {
      shift_chain(arr, count, shift, i);
   }
}

void shift_chain(int arr[], size_t count, size_t shift, size_t start) {
   int temp_right = 0;
   int temp_left = 0;
   size_t i = 0;

   temp_right = arr[start];
   for (i = start + count - shift; i > start; i -= shift) {
      temp_left = arr[i];
      arr[i] = temp_right;
      temp_right = temp_left;
   }
   arr[start] = temp_right;
}

void tests() {
   int arr_chain[] = {1, 2, 3, 4, 5, 6};
   int arr_chain_expected[] = {3, 2, 5, 4, 1, 6};
   int arr[] = {1, 2, 3, 4, 5, 6};
   int arr_expected[] = {4, 5, 6, 1, 2, 3};
   int i = 0;

   shift_chain(arr_chain, 6, 2, 0);
   for (i = 0; i < 6; i += 1) {
      assert(arr_chain[i] == arr_chain_expected[i]);
   }
   
   shift_nums(arr, 6, 3);
   for (i = 0; i < 6; i += 1) {
      assert(arr[i] == arr_expected[i]);
   }
}
