//

#include <iostream>
#include <vector>
#include <limits>

#include "../lab1/povidom.h"

using namespace std;


// Counting sort

struct counting_data
{
    int key{};

    string value;

    // Це для Radix sort
    long val2{};
};

void countingsort(counting_data *A, counting_data *B, int len, int k)
{
    vector<int> C(k + 1);

    int x{};

    for (int j = 0; j < len; ++j)
    {
        x = A[j].key;
        ++C[x];
    }
    // C[i] - кількість елементів що дорівнює i

    for (int i = 1; i <= k; ++i)
    {
        C[i] += C[i - 1];
    }
    // C[i] - кількість елементів що менше або дорівнює i

    for (int j = len - 1 ; j >= 0; --j)
    {
        x = A[j].key;
        B[C[x] - 1] = A[j];
        --C[x];
    }
}

void counting_sort(counting_data *first, counting_data *last)
{
    int len = last - first;
    vector<counting_data> output(len);

    int k = 0;

    // Знайдемо наїбільше k
    for (int i = 0; i < len; ++i)
    {
        if (k < first[i].key)
        {
            k = first[i].key;
        }
    }

    countingsort(first, &output[0], len, k);

    copy(begin(output), end(output), first);
}

// Radix sort

void radix_sort(long *first, long *last)
{
    int len = last - first;

    vector<counting_data> A(len);

    for (int j = 0; j < len; ++j)
    {
        A[j].val2 = first[j];
    }

    // Максимальна кількість десятичних розрядів у long
    int d = numeric_limits<long>::digits10;

    int dgt_pos = 1;

    for (int i = 1; i <= d; ++i)
    {
        for (int j = 0; j < len; ++j)
        {
            // Беремо i-ту цифру числа
            A[j].key = (A[j].val2 / dgt_pos) % 10;
        }

        counting_sort(&A[0], &A[0] + A.size());

        dgt_pos *= 10;
    }

    for (int j = 0; j < len; ++j)
    {
        first[j] = A[j].val2;
    }
}


int main()
{
    counting_data a1[] = {{1, "b"}, {3, "d"}, {2, "c"}, {0, "a"}, };

    counting_sort(begin(a1), end(a1));

    bool sep = false;

    cout << "Counting sort: ";

    for (auto& s : a1)
    {
        if (sep)
        {
            cout << ", ";
        }

        cout << s.key <<": " << s.value;

        sep = true;
    }

    cout << "\n";

    long a2[] = {435435, 5333, 5645645, 333424, 2, 44, 121, 9, 0, 77, 9, 100, 33, };

    radix_sort(begin(a2), end(a2));

    sep = false;

    cout << "Radix sort: ";

    for (auto& s : a2)
    {
        if (sep)
        {
            cout << ", ";
        }

        cout << s;

        sep = true;
    }

    cout << "\n";
}
