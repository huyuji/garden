#include <iostream>
#include <vector>

using namespace std;

int nonDivisibleSubset(int k, vector<int> s) {
    vector<int> mods(k, 0);
    for (int number : s)
    {
        mods[number % k]++;
    }
    
    for (int i : mods)
    {
        cout << i << " ";
    }
    cout << "\n";

    int count = mods[0] ? 1 : 0;
    cout << count << "\n";
    for (int i = 1; 2 * i < k; i++)
    {
        count += mods[i] > mods[k - i] ? mods[i] : mods[k - i];
        cout << count << "\n";
    }
    
    if (k % 2 == 0 && mods[k / 2])
    {
        count++;
        cout << count << "\n";
    }
    
    return count;
}

void test(int k, vector<int> s)
{
    cout << nonDivisibleSubset(k, s) << "\n";
}

int main()
{
    test(3, {1,7,2,4});
    return 0;
}
