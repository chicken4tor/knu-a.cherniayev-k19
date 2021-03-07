// Реалізувати циклічний журнал повідомлень.

// Задається фіксований розмір журналу N.
// Доступна операція для додавання текстового повідомлення, яка записує
// повідомлення і час надходження. Якщо журнал вже заповнений – новий запис
// замінює найдавніший. Також доступна операція отримання
// останніх K повідомлень (K<=N).

// Необхідно реалізувати наступні режими роботи:
// 1. Інтерактивний діалоговий режим – коли користувач може вибирати, які операції
//    виконувати та задавати необхідні параметри.
// 2. Демонстраційний режим – задаються фіксовані значення та послідовності виконання
//    операцій, що демонструють правильність роботи структур даних та операцій
//    в різних ситуаціях. В цьому режимі користувач не має нічого вводити.


#include <iostream>
#include <vector>
#include <chrono>
#include <iomanip>
#include <fstream>


using namespace std;

struct Message
{
    chrono::time_point<chrono::system_clock> timestamp;
    string message;
};


// Щось подібне до ArrayDateList
class MessagesList
{
public:

    MessagesList(size_t n) : N(n)
    {
        messages.reserve(n);
    }

    void log(const string &message)
    {
        Message povidom;

        povidom.timestamp = chrono::system_clock::now();
        povidom.message = message;

        if (messages.size() == N)
        {
            tail_index %= N;
            messages[tail_index] = povidom;
            ++tail_index;
        }
        else
        {
            ++tail_index;
            messages.push_back(povidom);
        }
    }

    void tail(size_t k, vector<Message> &tr)
    {
        if (k <= N)
        {
            tr.clear();

            for (size_t i = 1; i <= k; ++i)
            {
                tr.push_back(messages.at((tail_index - i + N) % N));
            }
        }
    }

private:
    vector<Message> messages;
    size_t N;
    size_t tail_index{};
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
            try
            {
                delete p;
                p = nullptr;

                size_t n = stoul(args.at(0));

                p = new MessagesList(n);
            }
            catch(const std::exception& e)
            {
                std::cerr << e.what() << '\n';
            }
        }
        else  if (verb == "log")
        {
            if (p != nullptr)
            {
                // Знайдемо початок повідомлення
                ss.clear();  // Убираємо признак кінця файлу
                ss.seekg(0, ios_base::beg);  // На початок стріму
                ss >> arg >> ws;  // Ігноруємо команду
                getline(ss, arg);

                p->log(arg);
            }
            else
            {
                cout << "Спочатку створи" << endl;
            }
        }
        else  if (verb == "tail")
        {
            if (p != nullptr)
            {
                try
                {
                    size_t n = stoul(args.at(0));

                    vector<Message> tr;
                    p->tail(n, tr);

                    for (Message &povidom : tr)
                    {
                        time_t message_time = chrono::system_clock::to_time_t(povidom.timestamp);

                        cout << put_time(localtime(&message_time), "%c") << ": " << povidom.message << endl;
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
            cout << "Акт творіння - create N\n"
                    "Додати елемент - log повідомлення\n"
                    "Останні повідомлення - tail N\n"
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
    MessagesList *p{};
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
    // MessagesList ml(5);

    // ml.log("1aaaaa!!!!");
    // ml.log("2ououo!!!!");
    // ml.log("3eyeye!!!!");
    // ml.log("4eyeye!!!!");
    // ml.log("5eyeye!!!!");
    // ml.log("6eyeye!!!!");
    // ml.log("7eyeye!!!!");

    // vector<Message> tr;

    // ml.tail(4, tr);

    // for (Message &povidom : tr)
    // {
    //     time_t message_time = chrono::system_clock::to_time_t(povidom.timestamp);

    //     cout << put_time(localtime(&message_time), "%c") << ": " << povidom.message << endl;
    // }

    ServerDemo demo;

    // https://askubuntu.com/questions/558280/changing-colour-of-text-and-background-of-terminal
    const string user_input = "$ \e[31m";  // 31 - Red
    const string reset_color = "\e[0m";

    string command;

    cout << "Lab 2b 16\n";
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
