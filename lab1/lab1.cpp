// Інформація про повідомлення у децентралізованій соціальній мережі (є багато серверів,
// на кожному сервері до 40000 користувачів, повідомлення в рамках одного серверу).
// Для кожного повідомлення зберігаються такі дані:
//   текст повідомлення;
//   точна дата і час відправлення повідомлення;
//   користувач-автор повідомлення;
//   адресат повідомлення (існує спеціальний адресат, що позначає повідомлення для всіх);
//   тип повідомлення – новини, питання, відповідь на питання, запрошення на подію, коментар до іншого повідомлення;
//   оцінка повідомлення від системи боротьби зі спамом (дійсне невід’ємне число, чим ближче до 0 – тим менше шансів, що це спам).

// Критерії пошуку:
//  повідомлення, що починаються із заданого фрагменту тексту;
//  повідомлення заданого типу з оцінкою не менше заданої;
//  повідомлення від заданого автора, надіслані в заданому діапазоні часу.

// Необхідно реалізувати підтримку наступних операцій:
// 1.  Додавання елементів;
// 2.  Зберігання даних (запис даних у файл);
// 3.  Відновлення даних (зчитування даних з файлу);
// 4.  Вивід всіх збережених даних;
// 5.  Пошук за заданими критеріями;
// 6.  Модифікація елементів (додаткові бали);
// 7.  Видалення елементів (додаткові бали)

// Для зберігання елементів треба реалізувати наступні механізми:
// 1.  Зберігання в пам’яті, без збереження на диск (можна використати довільну структуру даних, зокрема бібліотечні структури на зразок std::vector);
// 2.  Зберігання у вигляді текстового файлу;
// 3.  Зберігання у вигляді бінарного файлу.

// Для кожного елементу, окрім описаних у відповідному варіанті даних, треба також зберігати
// унікальний ID – ціле число, яке буде унікальним для кожного елементу даних.

#include <iostream>
#include <vector>
#include <ctime>
#include <fstream>
#include <time.h>

using namespace std;

enum MessageType {
    News, Question, Answer, Invite, Comment, 
}; 

struct Povidom
{
    string povidom;
    string avtor;
    string adresat;
    int spam;
    time_t timestamp;
    MessageType message_type;
};

struct Server
{
    /* data */
};

struct User
{
    /* data */
};

class ServerPovidom
{
private:
    vector <Povidom> messages;

public:
    ServerPovidom(/* args */);
    ~ServerPovidom();

    void add_pov(const string &povidom, const string &avtor, const string &adresat, MessageType message_type)
    {
        Povidom new_povidom;

        new_povidom.povidom = povidom;
        new_povidom.avtor = avtor;
        new_povidom.adresat = adresat;
        new_povidom.message_type = message_type;

        messages.push_back(new_povidom);
    }

    void get_pov()
    {

    }

    void show_pov()
    {

    }

    void search_pov_text(string poshuk)
    {
        short resus = poshuk.length();
        string obrezysh;
        for (Povidom povidom : messages)
        {
            obrezysh = povidom.povidom;
            obrezysh.resize(resus);

            if (poshuk == obrezysh)
            {
                cout << povidom.povidom << endl;
            }
        }
    }

    void search_pov_ocin(int spam)
    {
        for (Povidom povidom : messages)
        {
            if (spam == povidom.spam)
            {
                cout << povidom.povidom << endl;
            }
            
        }
        
    }

    void search_pov_avtor_chas(string avtor, const char* date, short h1, short m1, short s1, short h2, short m2, short s2)
    {
        tm da;   
        strptime(date, "%F", &da);
        time_t d = mktime(&da);

        Povidom povidom;

        cout << da.tm_year << endl;
        cout << "d = " << d << endl;
        cout << avtor << " " << povidom.avtor;

        time_t chas1 = d + h1 * 3600 + m1 * 60 + s1;
        time_t chas2 = d + h2 * 3600 + m2 * 60 + s2;

        for (Povidom povidom : messages)
        {
            if (avtor == povidom.avtor && (povidom.timestamp <= chas2 && povidom.timestamp >= chas2))
            {
                cout << povidom.povidom << endl;
            }
        }
        
    }

    void mod_pov()
    {

    }

    void del_pov()
    {

    }

    void dump()
    {
        for (Povidom povidom : messages)
        {
            cout << /*povidom.povidom << " (" << povidom.avtor << ", " << povidom.adresat << ", " << povidom.message_type << ", " << */povidom.timestamp << ")" << endl;
        }
        
    }
};

ServerPovidom::ServerPovidom(/* args */)
{
}

ServerPovidom::~ServerPovidom()
{
}


int main()
{
    ServerPovidom s1;

}