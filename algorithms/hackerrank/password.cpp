#include <bits/stdc++.h>

using namespace std;

string ltrim(const string &);
string rtrim(const string &);
vector<string> split(const string &);


/*
 * Complete the 'authEvents' function below.
 *
 * The function is expected to return an INTEGER_ARRAY.
 * The function accepts 2D_STRING_ARRAY events as parameter.
 */

vector<int> authEvents(vector<vector<string>> events) {
    vector<int> results;
    vector<int> mods(9);
    mods[0] = 1;
    int p = 131;
    int m = 1000000007;
    vector<char> hashIndexes;
    hashIndexes.push_back(0);
    for (char c = 'a'; c <= 'z'; ++c)
    {
        hashIndexes.push_back(c);
    }
    for (char c = 'A'; c <= 'Z'; ++c)
    {
        hashIndexes.push_back(c);
    }
    for (char c = '0'; c <= '9'; ++c)
    {
        hashIndexes.push_back(c);
    }
    int eventIndex = 0;
    while (eventIndex < events.size())
    {
        string password = "";
        while(eventIndex < events.size() && events[eventIndex][0] == "setPassword")
        {
            password = events[eventIndex][1];
            cout << "password " << password << "\n";
            ++eventIndex;
        }
        
        if (eventIndex == events.size())
        {
            break;
        }
        
        unsigned long hash = password[password.size() - 1];
        for (int i = password.size() - 2, factor = 1; i >= 0; --i, ++factor)
        {
            if (mods[factor] == 0)
            {
                mods[factor] = (mods[factor - 1] * p) % m;
            }
            
            hash = (hash + (password[i] * mods[factor])) % m;
            cout << "hash " << hash << "\n";
        }
        cout << "hash " << hash << "\n";
        unsigned long hashPlus = (hash * p) % m;
        cout << "hashPlus " << hashPlus << "\n";
        
        map<char, int> hashMap;
        hashMap[0] = hash;
        
        while(eventIndex < events.size() && events[eventIndex][0] == "authorize")
        {   
            int input = atoi(events[eventIndex][1].c_str());
            cout << "input " << input << "\n";
            bool match = false;
            for (char c : hashIndexes)
            {
                if (hashMap.find(c) == hashMap.end())
                {
                    hashMap[c] = (hashPlus + c) % m;
                    cout << hashMap[c] << "\n";
                }
                if (input == hashMap[c])
                {
                    match = true;
                    break;                        
                }
            }
            cout << match << "\n";
            if (match)
            {
                results.push_back(1);
            }
            else
            {
                results.push_back(0);
            }
            
            ++eventIndex;
        }
    }
    
    return results;
}
int main()