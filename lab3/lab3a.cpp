// Реалізувати алгоритми сортування для текстових рядків,
// спершу за довжиною, за зростанням ("A"<"B"<"AA"<"AB")

// Необхідно реалізувати наступні режими роботи: 
// 1. Демонстраційний режим – задається розмір масиву,
//    генерується масив відповідного розміру, масив сортується
//    різними алгоритмами, виводяться проміжні кроки кожного алгоритму.
// 2. Автоматичний режим “benchmark” з вимірами швидкості роботи
//    кожного з алгоритмів.

// Всі алгоритми мають працювати на однакових масивах.
// Варто розглянути як випадкові масиви, так і майже відсортовані
// в правильному та неправильному порядку (тобто якщо треба відсортувати
// за зростанням, то розглянути масиви, майже відсортовані
// за зростанням – правильний порядок, та за спаданням – неправильний порядок).

// Для комбінованого алгоритму, розглянути різні значення порогу,
// коли переходити від простого до більш ефективного алгоритму – і виміряти
// час виконання для різних значень порогу, а також підібрати
// оптимальне значення порогу. Алгоритм працює рекурсивно,
// і для маленьких підмасивів переключається на більш простий
// алгоритм (наприклад, якщо поріг=10 елементів, на вхід
// подається масив з 10000 елементів, то спершу використовується
// більш ефективний алгоритм, але на подальших кроках може бути
// використаний більш простий алгоритм для сортування підмасивів
// не більше 10 елементів).

#include <iostream>
#include <string>
#include <utility>


using namespace std;


// Порівняння рядків
bool compare_greater(const string &s1, const string &s2)
{
    if (s1.length() == s2.length())
    {
        return s1 > s2;
    }

    if (s1.length() < s2.length())
    {
        return false;
    }

    return true;
}

bool compare_lesser(const string &s1, const string &s2)
{
    return !compare_greater(s1, s2) && s1 != s2;
}

// Insertion sort
void insertion_sort(string *first, string *last)
{
    int len = last - first;

    for (int i = 1; i < len; ++i)
    {
        for( int j = i; j > 0 && compare_greater(first[j-1], first[j]); --j)
        {
            swap(first[j-1], first[j]);
        }
    }
}

// Quick sort. Hoare partition scheme, rightmost pivot

int hoare_partition(string *A, int lo, int hi)
{
    int p_i = (lo + hi)/2;  // Middle index, temporarly
    const string &pivot = A[p_i];

    for (int i = lo - 1, j = hi + 1;;)
    {
        do
        {
            ++i;
        } while (compare_lesser(A[i], pivot));

        do
        {
            --j;
        } while (compare_greater(A[j], pivot));

        if (i >= j)
        {
            return j;
        }

        swap(A[i], A[j]);
    }

    return -1;
}

void quicksort(string *A, int lo, int hi)
{
    if (lo < hi)
    {
        int p = hoare_partition(A, lo, hi);
        quicksort(A, lo, p);
        quicksort(A, p + 1, hi);
    }
}

void quick_sort(string *first, string *last)
{
    quicksort(first, 0, last - first - 1);
}

// Merge sort. Top-down, small lists

int main()
{
    string a1[] = { "AA", "B", "A", "AB"};

    //insertion_sort(begin(a1), end(a1));
    quick_sort(begin(a1), end(a1));

    bool sep = false;

    for (auto& s : a1)
    {
        if (sep)
        {
            cout << ", ";
        }

        cout << s;

        sep = true;
    }

    cout << "\n";

    return 0;
}
