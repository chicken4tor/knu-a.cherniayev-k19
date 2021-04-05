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
#include <vector>
#include <algorithm>
#include <chrono>
#include <random>

#include "labdemo.h"


using namespace std;

// Будемо показувати проміжні кроки?
static bool trace_on = true;

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

void print_array(string *first, string *last)
{
    bool sep = false;

    for (string *it = first; it != last; ++it)
    {
        if (sep)
        {
            cout << ", ";
        }

        cout << *it;

        sep = true;
    }

    cout << "\n";
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

            if (trace_on)
            {
                print_array(first, last);
            }
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

typedef vector<string> mrg_list;

mrg_list merge(mrg_list &left, mrg_list &right)
{
    mrg_list result;

    while (!left.empty() && !right.empty())
    {
        if (compare_greater(left.front(), right.front()))
        {
            result.push_back(right.front());
            right.erase(right.begin());
        }
        else
        {
            result.push_back(left.front());
            left.erase(left.begin());
        }
    }

    result.insert(end(result), begin(left), end(left));
    result.insert(end(result), begin(right), end(right));

    return result;
}

mrg_list mergesort(mrg_list &m)
{
    // Якщо у списку один елемент - він відсортован
    if (m.size() <= 1)
    {
        return m;
    }

    // Розділимо на два рівних підсписка
    int mid = m.size() / 2;

    mrg_list left(begin(m), begin(m) + mid);
    mrg_list right(begin(m) + mid, end(m));

    left = mergesort(left);
    right = mergesort(right);

    return merge(left, right);
}

void merge_sort(string *first, string *last)
{
    mrg_list m;

    m.reserve(last - first);

    m.insert(end(m), first, last);

    m = mergesort(m);

    copy(begin(m), end(m), first);
}

void combi_sort(string *first, string *last)
{}

class Lab3Demo : public LabDemo
{
protected:

    bool lab_commands(const std::string &verb, const std::vector<std::string> &args, stringstream &ss) override
    {
        if (verb == "sort")
        {
            long n{};

            if (args.size() == 1)
            {
                n = strtol(args[0].c_str(), nullptr, 10);
            }

            if (n != 0)
            {
                sort_alg(n);
            }
            else
            {
                cout << "Потрібно вказати кількість елементів" << endl;
            }
        }
        else if (verb == "trace")
        {}
        else if (verb == "benchmark")
        {}
        else if (verb == "help")
        {
            cout << "Демонстрація реалізованніх алгоритмів - sort N\n"
                    "Показувати проміжні кроки - trace on або off\n"
                    "Режим вимірювання ефективності - benchmark <кількість елементів>\n"
                    "Вихід - quit\n"
                    "Ця інформація - help\n";
        }
        else
        {
            return false;
        }

        return true;
    }

    void sort_alg(long n)
    {
        vector<string> data;

        data.reserve(n);

        // Генеруємо випадкові данні
        default_random_engine reng;

        const char alphabet[] = "0123456789QWERTYUIOPASDFGHJKLZXCVBNMqwertyuiopasdfghjklzxcvbnm!+;:)(_";

        // Довжина слів
        uniform_int_distribution<string::size_type> word_len(1, 18);
        // Випадкові літери із абетки
        uniform_int_distribution<size_t> litera(0, end(alphabet) - begin(alphabet) - 1);

        for (long i = 0; i < n; ++i)
        {
            string::size_type len{};
            string word;

            len = word_len(reng);
            word.reserve(len);

            for (int j = 0; j < len; ++j)
            {
                word += alphabet[litera(reng)];
            }

            data.push_back(word);
        }

        vector<string> dup;

        dup = data;

        cout << "Insertion sort\n";
        insertion_sort(&dup[0], &dup[0] + dup.size());

        dup = data;
        cout << "Quick sort\n";
        quick_sort(&dup[0], &dup[0] + dup.size());

        dup = data;
        cout << "Merge sort\n";
        merge_sort(&dup[0], &dup[0] + dup.size());

        dup = data;
        cout << "Combi sort\n";
        combi_sort(&dup[0], &dup[0] + dup.size());
    }

private:
};

int main()
{
    // string a1[] = { "AA", "B", "A", "AB", "X", "SSS", "Nnn", "a", "AAAA", "NNN"};

    // insertion_sort(begin(a1), end(a1));
    // quick_sort(begin(a1), end(a1));
    // merge_sort(begin(a1), end(a1));

    // bool sep = false;

    // for (auto& s : a1)
    // {
    //     if (sep)
    //     {
    //         cout << ", ";
    //     }

    //     cout << s;

    //     sep = true;
    // }

    // cout << "\n";

    Lab3Demo demo;

    // https://askubuntu.com/questions/558280/changing-colour-of-text-and-background-of-terminal
    const string user_input = "$ \e[31m";  // 31 - Red
    const string reset_color = "\e[0m";

    string command;

    cout << "Lab 3a\n";
    cout << user_input;

    while (getline(cin, command))
    {
        // Повертаємо колір
        cout << reset_color;

        if (!demo.process_command(command))
        {
            break;
        }

        cout << user_input;
    }

    // Повертаємо колір
    cout << reset_color;

    return 0;
}
