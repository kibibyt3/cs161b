/******************************************************************************
# Author:      Liv Callister
# Lab:         Discussion 5
# Date:        27 February 2026
# Description: This program calculates the average amount of men and women in
#              each major and the average salary of men and women by major
#              given an input file.
# Input:       ifstream& (input file)
# Output:      ofstream& (output file)
# Sources:     cppreference.com
#*****************************************************************************/
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <cstring>
#include <limits>
using namespace std;

// function prototypes
bool openFile(ifstream &inFile, string fileName);
void ratioCalc(ifstream& inFile, ofstream& outFile);
void averageSalaryCalc(ifstream& inFile, ofstream& outFile);

// main
int main() {
   ifstream inFile;
   ofstream outFile;
   string fileName;
  
   cout << "Enter name of input file:";
   cin >> fileName;
 
   if (!openFile(inFile, fileName)) {
      cout << "Input file did not open. Program terminating!!!";
      return 0;
   }
   
   outFile.open("stemout.txt");
   if (!outFile) {
      cout << "Output file did not open. Program terminating!!!";
      return 0;
   }

   ratioCalc(inFile, outFile);

   // Reset inFile for a second pass.
   inFile.clear();
   inFile.seekg(0);

   averageSalaryCalc(inFile, outFile);
 
   inFile.close();
   outFile.close();

   return 0;
}

// function to open file
// returns true if file opens
// returns false if file does not open for any reason
// parameters: file stream variable by reference and fileName

bool openFile(ifstream &inFile, string fileName) {
   inFile.open(fileName);
   if (!inFile) {
      return false;
   }
   return true;
}

// Name:    ratioCalc(ifstream& inFile, ofstream& outFile)
// Desc:    This function calculates the percentage of men and women in each
//          major given an input file, and records it in an output file.
// Input:   ifstream& (input file)
// Output:  ofstream& (output file)
// Return:  None
void ratioCalc(ifstream& inFile, ofstream& outFile) {
   int major_code = 0;
   char major[50];
   char major_category[50];
   int total = 0;
   int men = 0;
   int women = 0;
   int annual_salary = 0;

   inFile.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore header.
   outFile << "Major\t%_Men\t%_Women\n"; // Write header
   while (!inFile.fail()) {
      inFile >> major_code;
      inFile >> major;
      inFile >> major_category;
      inFile >> total;
      inFile >> men;
      inFile >> women;
      inFile >> annual_salary;
      if (!inFile.fail()) {
         outFile << major << '\t' << std::fixed << std::setprecision(2)
            << static_cast<double>(men) / static_cast<double>(total) * 100
            << '%' << '\t'
            << static_cast<double>(women) / static_cast<double>(total) * 100
            << '%' << '\n';
      }
   }

   outFile << "-------------------------\n";
}

// Name:    averageSalaryCalc(ifstream& inFile, ofstream& outFile)
// Desc:    This function calculates the average salary of men and women by
//          major, and writes it to an output file.
// Input:   ifstream& (input file)
// Output:  ofstream& (output file)
// Return:  None
void averageSalaryCalc(ifstream& inFile, ofstream& outFile) {
   int major_code = 0;
   char major[50];
   char major_category[50];
   int total = 0;

   // We use unsigned long long int in order to prevent overflows.
   unsigned long long int men = 0;
   unsigned long long int women = 0;
   unsigned long long int annual_salary = 0;

   unsigned long long int overall_men = 0;
   unsigned long long int overall_women = 0;
   unsigned long long int weighted_men_salary = 0;
   unsigned long long int weighted_women_salary = 0;

   inFile.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore header.
   while (!inFile.fail()) {
      inFile >> major_code;
      inFile >> major;
      inFile >> major_category;
      inFile >> total;
      inFile >> men;
      inFile >> women;
      inFile >> annual_salary;
      if (!inFile.fail()) {
         overall_men += men;
         overall_women += women;
         weighted_men_salary += (men * annual_salary);
         weighted_women_salary += (women * annual_salary);
      }
   }
   outFile << "Average salary for men:    $"
      << (weighted_men_salary / overall_men) << endl;
   outFile << "Average salary for women:  $"
      << (weighted_women_salary / overall_women) << endl;
}
