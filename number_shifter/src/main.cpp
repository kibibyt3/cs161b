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

   tests();

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

   // By using a non-naive implementation, we bring the time complexity from
   // O(n^2) down to O(n) (unless gcd is implemented very poorly.)
   // The GCD of count and shift decide how many unique, neighboring "chains"
   // of numbers we have in arr. From there, we just shift each chain the
   // requisite amount. In this way we only move each element in arr once,
   // while also keeping a small memory footprint.
   // Here's a demonstration with shift = 3 and GCD = 3.
   //  ______________________________________________
   // |    |    |    |    |    |    |     |     |    |
   // | x1 | x2 | x3 | x4 | x5 | x6 | x7  | ... | xn |
   // |    |    |    |    |    |    |     |     |    |
   // ------------------------------------------------
   //   |              |              |
   // --.              |              |
   //     .-------------  width = GCD |
   //     |             .--------------    
   //     |             |              .---- ...
   //     v             v              v
   //  ______________________________________________
   // |    |    |    |    |    |    |     |     |    |
   // | x4 | x2 | x3 | x7 | x5 | x6 | x10 | ... | xn |
   // |    |    |    |    |    |    |     |     |    |
   // ------------------------------------------------
   //
   // We repeat the above process as many times as needed to bridge the
   // distance of shift. In the above example, we'd repeat 2 more times.
   greatest_common_divisor = gcd(count, shift);
   for (i = 0; i < greatest_common_divisor; i++) {
      shift_chain(arr, count, shift, i);
   }
}

// Name:    shift_chain(int arr[], size_t count, size_t shift, size_t start)
// Desc:    Shift one "chain" of numbers in an array.
// Input:   int array, size_t (array count), size_t (shift degree)
//          size_t (chain's start index)
// Output:  None
// Return:  None
void shift_chain(int arr[], size_t count, size_t shift, size_t start) {
   int temp_right = 0;
   int temp_left = 0;
   size_t i = 0;

   temp_right = arr[start];
   // DEBUG: change this to "i = start + count * n" where n is some number
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
   int arr_weird[] = {1, 2, 3, 4, 5, 6};
   int arr_weird_expected[] = {5, 6, 1, 2, 3, 4};
   int i = 0;

   shift_chain(arr_chain, 6, 2, 0);
   for (i = 0; i < 6; i++) {
      assert(arr_chain[i] == arr_chain_expected[i]);
   }
   
   shift_nums(arr, 6, 3);
   for (i = 0; i < 6; i++) {
      assert(arr[i] == arr_expected[i]);
   }

   shift_nums(arr, 6, 4);
   for (i = 0; i < 6; i++) {
      assert(arr_weird[i] == arr_weird_expected[i]);
   }

}
