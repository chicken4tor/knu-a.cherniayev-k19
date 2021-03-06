// Реалізувати циклічний журнал повідомлень.

// Задається фіксований розмір журналу N.
// Доступна операція для додавання текстового повідомлення, яка записує
// повідомлення і час надходження. Якщо журнал вже заповнений – новий запис
// замінює найдавніший. Також доступна операція отримання
// останніх K повідомлень (K<=N).

#include <iostream>
#include <vector>
#include <chrono>
#include <iomanip>


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
private:
    /* data */
};

ServerDemo::ServerDemo(/* args */)
{
}

ServerDemo::~ServerDemo()
{
}

int main()
{
    MessagesList ml(5);

    ml.log("1aaaaa!!!!");
    ml.log("2ououo!!!!");
    ml.log("3eyeye!!!!");
    ml.log("4eyeye!!!!");
    ml.log("5eyeye!!!!");
    ml.log("6eyeye!!!!");
    ml.log("7eyeye!!!!");

    vector<Message> tr;

    ml.tail(4, tr);

    for (Message &povidom : tr)
    {
        time_t message_time = chrono::system_clock::to_time_t(povidom.timestamp);

        cout << put_time(localtime(&message_time), "%c") << ": " << povidom.message << endl;
    }
}
