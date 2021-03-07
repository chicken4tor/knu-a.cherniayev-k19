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

// Необхідно реалізувати наступні режими роботи для кожної задачі:
// 1. Інтерактивний діалоговий режим – коли користувач може вибирати, які операції
//    виконувати та задавати необхідні параметри.
// 2. Демонстраційний режим – задаються фіксовані значення та послідовності виконання
//    операцій, що демонструють правильність роботи структур даних та операцій
//    в різних ситуаціях. В цьому режимі користувач не має нічого вводити.


#include <iostream>
#include <vector>
#include <iterator>
#include <fstream>
#include <sstream>


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

class ServerDemo
{
public:
    ServerDemo(/* args */);
    ~ServerDemo();

    bool process_command(const string &command)
    {
        // Команда для виконання
        string verb;
        // Параметри команди
        vector<string> args;

        // Знаходимо команду та виділяємо аргументи
        stringstream ss(command);
        string arg;

        // Ігноруємо початкові пробели
        ss >> ws;
        // Перевіряемо початок строки на '#' або ';'
        auto first_char = ss.peek();
        if (first_char == '#' || first_char == ';')
        {
            // Ця команда - коментар
            return true;
        }

        ss >> verb;
        args.clear();

        while ((ss >> arg))
        {
            args.push_back(arg);
        }

        if (verb == "create")
        {
            if (args.size() >= 2)
            {
                try
                {
                    delete p;
                    p = nullptr;

                    size_t n = stoul(args.at(0));

                    p = new StepsRing(n);

                    for (size_t i = 1; i < args.size(); ++i)
                    {
                        p->append(stoi(args[i]));
                    }
                }
                catch(const std::exception& e)
                {
                    std::cerr << e.what() << '\n';
                }
            }
            else
            {
                cout << "Вкажіть кількість елементів, та послідовність кроків" << endl;
            }
        }
        else if (verb == "go")
        {
            if (p != nullptr)
            {
                p->lets_go();
            }
            else
            {
                cout << "Спочатку створи" << endl;
            }
        }
        else if (verb == "steps")
        {
            if (p != nullptr)
            {
                for (int n : p->removal_order())
                {
                    cout << n << " ";
                }

                cout << endl;
            }
            else
            {
                cout << "Спочатку створи" << endl;
            }
        }
        else if (verb == "step")
        {
            if (p != nullptr)
            {
                try
                {
                    int k = stoi(args.at(0));

                    auto s = p->removal_order();

                    if (s.size() > k)
                    {
                        cout << *(s.rbegin() + k) << endl;
                    }
                    else
                    {
                        cout << "Дуже великий номер кроку" << endl;
                    }
                }
                catch(const std::exception& e)
                {
                    std::cerr << e.what() << '\n';
                }
                
            }
            else
            {
                cout << "Спочатку створи" << endl;
            }
        }
        else if (verb == "demo")
        {
            if (args.size() == 1)
            {
                try
                {
                    ifstream scenario_to_play(args[0]);

                    if (!scenario_to_play.is_open())
                    {
                        throw runtime_error("Ой не можу відкрити файл");
                    }

                    string demo_cmd;

                    while (getline(scenario_to_play, demo_cmd))
                    {
                        if (!process_command(demo_cmd))
                        {
                            break;
                        }
                    }
                }
                catch(const std::exception& e)
                {
                    std::cerr << e.what() << '\n';
                }
            }
        }
        else if (verb == "quit" || verb == "die")
        {
            return false;
        }
        else if (verb == "help")
        {
            cout << "Акт творіння - create N <кроки по колу>\n"
                    "Додати елемент - go\n"
                    "Всі видалення - steps\n"
                    "Видаленний на кроці елемент - step k\n"
                    "Демонстрацийний режим - demo <ім'я файла з командами>\n"
                    "Вихід - quit\n"
                    "Ця інформація - help\n";
        }
        else
        {
            // Якщо нема команди - нічого не печатаємо
            if (!verb.empty())
            {
                cout << "Невідома команда (скористуйтеся help) " << command << endl;
            }
        }

        return true;
    }

private:
    StepsRing *p{};
};

ServerDemo::ServerDemo(/* args */)
{
}

ServerDemo::~ServerDemo()
{
    delete p;
}

int main()
{
    // StepsRing kk(10);

    // kk.append(2);
    // kk.append(3);
    // kk.append(5);

    // kk.lets_go();

    // for (int n : kk.removal_order())
    // {
    //     cout << n << " ";
    // }

    // cout << "\n";

    ServerDemo demo;

    // https://askubuntu.com/questions/558280/changing-colour-of-text-and-background-of-terminal
    const string user_input = "$ \e[31m";  // 31 - Red
    const string reset_color = "\e[0m";

    string command;

    cout << "Lab 2b 17\n";
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
