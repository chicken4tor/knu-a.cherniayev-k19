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
// 7.  Видалення елементів (додаткові бали).

// Для зберігання елементів треба реалізувати наступні механізми:
// 1.  Зберігання в пам’яті, без збереження на диск (можна використати довільну структуру даних, зокрема бібліотечні структури на зразок std::vector);
// 2.  Зберігання у вигляді текстового файлу;
// 3.  Зберігання у вигляді бінарного файлу.

// Для кожного елементу, окрім описаних у відповідному варіанті даних, треба також зберігати
// унікальний ID – ціле число, яке буде унікальним для кожного елементу даних.

#include <iostream>
#include <vector>

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
    long timestamp;
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
    vector <Povidom> message;
    public:
        ServerPovidom(/* args */);
        ~ServerPovidom();

        void add_pov(const string &povidom, const string &avtor, const string &adresat, MessageType message_type)
        {
            Povidom new_povidom;
            new_povidom.povidom = povidom;
            message.push_back(new_povidom);

            Povidom new_avtor;
            new_avtor.avtor = avtor;
            message.push_back(new_avtor);

            Povidom new_adresat;
            new_adresat.adresat = adresat;
            message.push_back(new_adresat);

            Povidom new_message_type;
            new_message_type.message_type = message_type;
            message.push_back(new_message_type);
        }

        void save_pov()
        {

        }

        void read_pov()
        {

        }

        void show_pov()
        {

        }

        void search_pov()
        {

        }

        void mod_pov()
        {

        }

        void del_data()
        {

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