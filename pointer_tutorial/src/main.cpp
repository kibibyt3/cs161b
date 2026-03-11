/******************************************************************************
# Author:
# Assignment:
# Date:
# Description:
# Input:
# Output:
# Sources: en.cppreference.org, en.wikipedia.org
#*****************************************************************************/
#include <limits>
#include <iostream>
#include <cctype>

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
-------------------------------------------------------------------\n\
Please select which usage of the * symbol you would like explained:\n\
1 - Multiplication operator\n\
2 - Indirection operator\n\
3 - Pointer declaration\n\
4 - Quit";

const Page PAGES[3] = {
   // Multiplication operator
   {
      "\
The multiplication operator returns the product of the left-hand side (lhs) \n\
   right-hand side (rhs).",
      {"lhs * rhs"},
      1,
      "https://en.cppreference.com/w/cpp/language/operator_arithmetic.html",
      "\
   int product = 4 * 5;\n\
   cout << product; // Prints \"20\""
   },

   // Indirection operator
   {
      "The indirection operator returns the contents of a memory address (p).",
      {"*p"},
      1,
      "https://en.cppreference.com/w/cpp/language/operator_member_access.html",
      "\
   int x = 3;\n\
   int *xp = &x;\n\
   cout << *xp; // Prints \"3\""
   },

   // Pointer declaration
   {
      "\
The asterisk can be used to declare a pointer (p) to some location in \n   \
memory (a) containing data of some type (t).",
      {"t *p = a;", "t* p = a;"},
      2,
      "https://en.cppreference.com/w/cpp/language/pointer.html",
      "\
   char c = '\\0';\n\
   char *cp = &c;\n\
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

   cout << "Welcome to the pointer tutorial!" << endl;
   do {
      cout << menu << endl;
      menu_option = menu_option_prompt(">> ");
      if (menu_option != 4) {
         print_page(PAGES[menu_option - 1]);
      }
   } while (menu_option != 4);

   cout << "Thank you for using the pointer tutorial!" << endl;
}

void print_page(const Page &page) {
   size_t i = 0;
   cout << "DESCRIPTION:" << endl;
   cout << "   " << page.brief << endl;
   cout << "SYNTAX:" << endl;
   for (i = 0; i < page.syntax_count; i++) {
      cout << "   " << page.syntax[i] << endl;
      if (i != page.syntax_count - 1) {
         cout << "or" << endl;
      }
   }
   cout << "SEE ALSO:" << endl;
   cout << "   " << page.see_also << endl;
   cout << "See an example? (y/N)" << endl;
   if (prompt_for_example(">> ")) {
      cout << "EXAMPLE:" << endl;
      cout << page.example << endl;
   }
}

size_t menu_option_prompt(const char *prompt) {
   size_t choice = 0;
   cout << prompt;
   cin >> choice;
   while ((choice < 1) || (choice > 4)) {
      cout << "Please choose an option between 1 and 4." << endl;
      cout << prompt;
   }
   cin.ignore(numeric_limits<streamsize>::max(), '\n');
   return choice;
}

bool prompt_for_example(const char *prompt) {
   char ch = '\0';
   cout << prompt;
   cin.get(ch);
   if (ch != '\n') {
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
   }
   ch = static_cast<char>(toupper(ch));
   while (ch != '\n'
         && ch != 'Y'
         && ch != 'N') {
      cout << "Please type 'y' or 'n'." << endl;
      cout << prompt;
      cin.get(ch);
      if (ch != '\n') {
         cin.ignore(numeric_limits<streamsize>::max(), '\n');
      }
      ch = static_cast<char>(toupper(ch));
   }
   return ch == 'Y';
}
