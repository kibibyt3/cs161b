#include <iostream>
#include <cmath>

using namespace std;

void PrintScores(int arr[], size_t size);
int FindMax(int arr[], size_t size);
int FindMin(int arr[], size_t size);
double CalculateAverage(int arr[], size_t size);
int CountAboveThreshold(int arr[], size_t size, int threshold);

int main() {
   int scores[100];
   int score = 0;
   size_t score_count = 0;
   int max = 0;
   int min = 0;
   double avg = 0.0;
   int threshold = 0;
   
   int menu_option = 0;
   
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
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
      cin.get();
   } while (menu_option != 0);
   
   
   return 0;
}


void PrintScores(int arr[], size_t size) {
   size_t i = 0;
   for (i = 0; i < size; i++) {
      cout << "Score #" << i + 1 << ": " << arr[i] << endl;
   }
}

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

double CalculateAverage(int arr[], size_t size) {
   size_t i = 0;
   int sum = 0;
   double avg = 0.0;

   for (i = 0; i < size; sum += arr[i++]);
   
   // use the trinary operator here
   if (size < 1) {
      sum = NAN;
   }

   return static_cast<double>(sum) / static_cast<double>(size);
}

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
