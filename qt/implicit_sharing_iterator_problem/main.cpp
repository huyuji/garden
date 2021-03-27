#include <QVector>
#include <iostream>

int main()
{
    QVector<int> a, b;
    a.resize(100000);
    a[0] = 5;

    QVector<int>::iterator i = a.begin();
    b = a;
    b.clear();
    int j = *i;

    std::cout << a[0] << "\n" << b.size() << "\n" << j << "\n";
}
