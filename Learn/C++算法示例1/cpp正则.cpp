#include <iostream>

#include <regex>
#include <string>
 
int main()
{
   std::string text = "Quigck bro\nwn fox";
   std::regex re("\g|\n");
 
   // 写结果到输出迭代器
 //  std::regex_replace(std::ostreambuf_iterator<char>(std::cout), text.begin(), text.end(), vowel_re, "*");
 
   // 构造保有结果的字符串
   std::cout << '\n' << std::regex_replace(text,re, "嗯") << '\n';
}