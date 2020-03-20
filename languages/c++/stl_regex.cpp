#include <regex>
#include <iostream>

namespace
{

void regexIterator()
{
    const std::string s {"sss"};

    std::regex regex(R"(ss)");
    auto begin = std::sregex_iterator(s.begin(), s.end(), regex);
    auto end = std::sregex_iterator();

    std::cout << std::distance(begin, end) << " matches found. \n";

    for (std::sregex_iterator i = begin; i != end; ++i) {
        std::smatch match = *i;
        std::string matchString = match.str();
        std::cout << matchString << '\n';
    }
}   

}

void testRegex()
{
    regexIterator();
}
