// Числа від 1 до N записано по колу.
// Задається послідовність кроків KK.
// Коло обходять і послідовно видаляють кожен KK[i]-ий елемент.
// Коли послідовність кроків доходить кінця, вона запускається заново з початку.
// (Наприклад, якщо KK=[2,3,5], то починаючи з початку кола видаляють
// один елемент через 2, потім один елемент через 3, потім один елемент через 5,
// і потім знову один елемент через 2 і так далі).
// Вивести список елементів в порядку видалення.
// Вивести, яким по порядку буде видалено задане число.
// Вивести, яке число буде видалено на заданому кроці, починаючи з останнього
// (тобто якщо задано крок 0 – яке число видалиться останнім,
// якщо задано 1 – яке видалиться передостаннім і т.д.)


#include <iostream>
#include <vector>
#include <iterator>


using namespace std;


// Знову, щось подібне до ArrayDateList
class StepsRing
{
public:

    StepsRing(size_t n) : N(n)
    {
        // Заповнюємо коло
        ring.resize(n);
        for (size_t i = 0; i < n; ++i)
        {
            ring[i] = i + 1;
        }
    }

    void append(int value)
    {
        steps.push_back(value);
    }

    void lets_go()
    {
        while (!ring.empty())
        {
            for (int step : steps)
            {
                auto it = begin(ring);

                // -1 тому що ітератор вже вказує на елемент
                int first_step = step - 1;

                // Якщо приходится робити більш кроків ниж є у колі
                if (first_step > ring.size())
                {
                    first_step = first_step % ring.size();
                }

                advance(it, first_step);

                while (it < end(ring))
                {
                    del_order.push_back(*it);
                    it = ring.erase(it);
                    // елемент видалено, тому -1 з кроку
                    advance(it, step - 1);
                }

                if (ring.empty())
                {
                    break;
                }
            }
        }
    }

    const vector<int> &removal_order()
    {
        return del_order;
    }

private:
    // Це коло
    size_t N;
    vector<int> ring;

    // Це кроки
    vector<int> steps;

    // Порядок видалення
    vector<int> del_order;
};

int main()
{
    StepsRing kk(10);

    kk.append(2);
    kk.append(3);
    kk.append(5);

    kk.lets_go();

    for (int n : kk.removal_order())
    {
        cout << n << " ";
    }

    cout << "\n";
}
