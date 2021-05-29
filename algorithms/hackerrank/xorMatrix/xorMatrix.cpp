#include <vector>
#include <iostream>
#include <fstream>

using namespace std;

vector<string> split_string(string);

vector<vector<unsigned long long>> init(unsigned long long n)
{
    vector<vector<unsigned long long>> result(n);
    result[0] = vector<unsigned long long>(n);
    result[0][0] = 1;
    result[0][n-1] = 1;
    for (int i = 1; i < n; ++i)
    {
        result[i] = vector<unsigned long long>(n);
        result[i][i-1] = 1;
        result[i][i] = 1;
    }
    return result;
}

vector<vector<unsigned long long>> multiply(
    const vector<vector<unsigned long long>> & m1,
    const vector<vector<unsigned long long>> & m2)
{
    int n = m1.size();
    vector<vector<unsigned long long>> result(n);
    for (int i = 0; i < n; ++i)
    {
        result[i] = vector<unsigned long long>(n);
    }
    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            result[i][0] += m1[i][j] * m2[j][0];
        }
    }
    for (int j = 1; j < n; ++j)
    {
        result[0][j] = result[n-1][j-1];
        for (int i = 1; i < n; ++i)
        {
            result[i][j] = result[i-1][j-1];
        }
    }
    return result;
}

vector<vector<unsigned long long>> square(
    const vector<vector<unsigned long long>> & m)
{
    return {};
}

void print(const vector<vector<unsigned long long>> & m)
{
    for (int i = 0; i < m.size(); ++i)
    {
        for (int j = 0; j < m.size(); ++j)
        {
            cout << m[i][j] << " ";
        }
        cout << "\n";
    }
    cout << "\n\n";
}
/*
 * Complete the xorMatrix function below.
 */
vector<int> xorMatrix(unsigned long long m, vector<int> first_row) {
    if (m == 1)
    {
        return first_row;
    }
    --m;
    
    int n = first_row.size();
    if (n == 1)
    {
        return {0};
    }
    
    auto factor = std::move(init(n));
    print(factor);
    auto resultFactor = vector<vector<unsigned long long>>(n);
    for (int i = 0; i < n; ++i)
    {
        resultFactor[i] = vector<unsigned long long>(n);
        resultFactor[i][i] = 1;
    }
    while(m)
    {
        if (m & 1)
        {
            resultFactor = std::move(multiply(resultFactor, factor));           
            print(resultFactor);                          
        }
        m >>= 1;
        if (m)
        {
            factor = std::move(multiply(factor, factor));       
        }
    }
    print(resultFactor);
    
    vector<int> result(n);
    for (int i = 0; i < n; ++i)
    {
        int value = 0;
        for (int j = 0; j < n; ++j)
        {
            if (resultFactor[j][i] % 2)
            {
                value ^= first_row[j];
            }
        }
        result[i] = value;
    }
    
    return result;
}

int main()
{
    ofstream fout("output.txt");
    ifstream fin("input09.txt");

    string nm_temp;
    getline(fin, nm_temp);

    vector<string> nm = split_string(nm_temp);

    int n = stoi(nm[0]);

    unsigned long long m = atol(nm[1].c_str());

    string first_row_temp_temp;
    getline(fin, first_row_temp_temp);

    vector<string> first_row_temp = split_string(first_row_temp_temp);

    vector<int> first_row(n);

    for (int first_row_itr = 0; first_row_itr < n; first_row_itr++) {
        int first_row_item = stoi(first_row_temp[first_row_itr]);

        first_row[first_row_itr] = first_row_item;
    }

    vector<int> result = xorMatrix(m, first_row);

    for (int result_itr = 0; result_itr < result.size(); result_itr++) {
        fout << result[result_itr];

        if (result_itr != result.size() - 1) {
            fout << " ";
        }
    }

    fout << "\n";

    fout.close();

    return 0;
}

vector<string> split_string(string input_string) {
    string::iterator new_end = unique(input_string.begin(), input_string.end(), [] (const char &x, const char &y) {
        return x == y and x == ' ';
    });

    input_string.erase(new_end, input_string.end());

    while (input_string[input_string.length() - 1] == ' ') {
        input_string.pop_back();
    }

    vector<string> splits;
    char delimiter = ' ';

    size_t i = 0;
    size_t pos = input_string.find(delimiter);

    while (pos != string::npos) {
        splits.push_back(input_string.substr(i, pos - i));

        i = pos + 1;
        pos = input_string.find(delimiter, i);
    }

    splits.push_back(input_string.substr(i, min(pos, input_string.length()) - i + 1));

    return splits;
}
