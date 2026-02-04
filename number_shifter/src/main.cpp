/******************************************************************************
# Author:      Liv Callister
# Assignment:  Discussion 4
# Date:        3 February 2026
# Description: This program shifts a list of numbers to the right.
# Input:       size_t (amount of numbers), int arr[] (list of numbers),
#              size_t (amount of shifts)
# Output:      int arr[] (list of shifted numbers)
# Sources:     en.cppreference.com, Discussion 4 Specifications
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

// Name:    int main()
// Desc:    This function controls the high-level control of the program.
// Input:   None
// Output:  None
// Return:  0 on a successful execution
int main() {
   size_t num_count = 0;
   size_t shift = 0;
   size_t i;
   char confirm_char = '\0';
   int *nums = NULL;

   // Uncomment this line to run unit tests.
   // tests();

   cout << "This program will shift a list of numbers to the right." << endl;
   do {
      cout << "To begin, please enter the amount of numbers in your list.";
      cout << endl;
      cout << ">> ";
      cin >> num_count;
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
      if (num_count > MAX_RECOMMENDED_NUM_COUNT) {
         cout << "Note: Allocating this much memory may not be possible." << endl;
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
   shift = (num_count == 0) ? 0 : shift % num_count;

   shift_nums(nums, num_count, shift);
   cout << "Here are your shifted numbers:" << endl;
   if (num_count > 0) {
      for (i = 0; i < num_count - 1; i++) {
         cout << nums[i] << ", ";
      }
      cout << nums[num_count - 1] << '.' << endl;
   }
   else {
      cout << "Your list was empty!" << endl;
   }
   
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
   // O(n^2) down to O(n), where n = count (unless gcd is implemented poorly.)
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
   // We repeat the above process as many times as needed to shift every value.
   // In the above example, we'd repeat 2 more times, beginning on x2 and then
   // on x3.
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
   size_t distance = 0;

   temp_right = arr[start];
   for (
         // distance must never be < 0, and is thus set to a theoretical
         // maximum. This value could be lower, but we'd need to know the value
         // of gcd(count, shift).
         distance = start + (count * shift) - shift;
         distance % count != start;
         distance -= shift
   ) {
      // distance % count is the actual index we work with.
      i = distance % count;
      temp_left = arr[i];
      arr[i] = temp_right;
      temp_right = temp_left;
   }
   arr[start] = temp_right;
}


// Name:    void tests()
// Desc:    This function performs unit tests on shift_nums and shift_chain.
// Input:   None
// Output:  None
// Return:  None; if a test fails, this function will abort the program.
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

   shift_nums(arr_weird, 6, 4);
   for (i = 0; i < 6; i++) {
      assert(arr_weird[i] == arr_weird_expected[i]);
   }

}
