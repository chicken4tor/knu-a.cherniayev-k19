//

#include <iostream>
#include <vector>


using namespace std;


// Counting sort

struct counting_data
{
    int key{};

    string value;
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

void radix_sort(string *first, string *last)
{
    ;
}


int main()
{
    counting_data a1[] = {{1, "b"}, {3, "d"}, {2, "c"}, {0, "a"}, };

    counting_sort(begin(a1), end(a1));

    bool sep = false;

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
} 
