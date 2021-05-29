#include <string>
#include <vector>
#include <iostream>
#include <set>

using namespace std;

int stringSimilarity(string s)
{
    set<int> heads;
    int totalCount = 0;
    for (int i = 1; i < s.size(); ++i)
    {
        for ()
        {
            if (s[i] == s[i - heads[j]])
            {

            }
        }
    }
}

int stringSimilarity2(string s)
{
    vector<int> tailIndex;
    int i = 1;
    int total = 0;
    while (i < s.size())
    {
        vector<int> newIndex;
        for (int index : tailIndex)
        {
            if (s[i] == s[index + 1])
            {
                ++total;
                newIndex.push_back(index + 1);
            }
        }

        if (s[i] == s[0])
        {
            ++total;
            newIndex.push_back(0);
        }
        tailIndex = std::move(newIndex);
        ++i;
    }
    return total + s.size();
}

int stringSimilarity1(string s)
{
    vector<int> count(s.size());
    int i = 1;
    int m = 0;

    while (i < s.size())
    {
        if (s[i] == s[m])
        {
            count[i] = count[m] + 1;
            ++i;
            ++m;
        }
        else if (count[m])
        {
            if (m == 0)
            {
                ++i;
            }
            else
            {
                m = 0;
            }
        }
        else
        {
            ++i;
        }
    }

    int total = 0;
    for (int number : count)
    {
        total += number;
    }

    return total + s.size();
}

void test(string s)
{
    cout << stringSimilarity(s) << "\n";
}

int main()
{
    test("ababaa");     // 11
    test("aa");         // 3
    test("aaaa");       // 10
    test("acabacacd");  // 15
    test("abacababa");  // 17
    return 0;
}
