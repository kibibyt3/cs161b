/******************************************************************************
# Author:
# Assignment:
# Date:
# Description:
# Input:
# Output:
# Sources: en.cppreference.org, en.wikipedia.org
#*****************************************************************************/
#include <iostream>
using namespace std;

struct Page {
   const char *brief;
   const char *syntax[5];
   size_t syntax_count;
   const char *see_also;
   const char *example;
};

size_t menu_option_prompt(const char *prompt);
bool prompt_for_example(const char *prompt);
void print_page(const Page &page);

const char *menu = "\
-------------------------------------------------------------------\
Please select which usage of the * symbol you would like explained:\
1 - Multiplication operator\
2 - Indirection operator\
3 - Pointer declaration\
4 - Quit";

const Page PAGES[3] = {
   // Multiplication operator
   {
      "\
The multiplication operator returns the product of the left-hand side (lhs) \
right-hand side (rhs).",
      {"lhs * rhs"},
      1,
      "https://en.cppreference.com/w/cpp/language/operator_arithmetic.html",
      "\
   int product = 4 * 5;\
   cout << product; // Prints \"20\""
   },

   // Indirection operator
   {
      "The indirection operator returns the contents of a memory address (p).",
      {"*p"},
      1,
      "https://en.cppreference.com/w/cpp/language/operator_member_access.html",
      "\
   int x = 3;\
   int *xp = &x;\
   cout << *xp; // Prints \"3\""
   },

   // Pointer declaration
   {
      "\
The asterisk can be used to declare a pointer (p) to some location in memory \
(a) containing data of some type (t).",
      {"t *p = a;", "t* p = a;"},
      2,
      "https://en.cppreference.com/w/cpp/language/pointer.html",
      "\
   char c = '\0';\
   char *cp = &c;\
   cout << sizeof(cp); // Prints the amount of bytes in an address"
   },
};

// Name:
// Desc:
// Input:
// Output:
// Return:
int main() {
   size_t menu_option = 0;

   cout << "Welcome to the pointer tutorial!";
   do {
      cout << menu << endl;
      menu_option = menu_option_prompt(">> ");
      if (menu_option != 4) {
         print_page(PAGES[menu_option - 1]);
      }
   } while (menu_option != 4);

   cout << "Thank you for using the pointer tutorial!" << endl;
}
