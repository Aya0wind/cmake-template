#include "formatters.hpp"
#include "regex.hpp"





int main() {
   auto regex = Regex("ab*c.*");
   tools::println("{}", regex.isMatch("abc"));
}
