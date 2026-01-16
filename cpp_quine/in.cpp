#include <iostream>
int main() { char q = 34; char n = 10; std::string s1 = "#include <iostream>int main() { char q = 34; char n = 10; std::string s1 = ; std::cout << s1.substr(0, 19) << n << s1.substr(19, 56) << q << s1 << q << s1.substr(75, s1.size()); }"; std::cout << s1.substr(0, 19) << n << s1.substr(19, 56) << q << s1 << q << s1.substr(75, s1.size()); }
