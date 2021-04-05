#ifndef POVIDOM_H_SEEN
#define POVIDOM_H_SEEN

#include <chrono>
#include <string>

enum MessageType {
    Unknown, News, Question, Answer, Invite, Comment, 
};

typedef unsigned long long povidom_id;
typedef std::chrono::time_point<std::chrono::system_clock> povidom_time;

struct Povidom
{
    // ціле число, яке буде унікальним для кожного елементу даних
    povidom_id id{};
    // текст повідомлення
    std::string povidom;
    // користувач-автор повідомлення
    std::string avtor;
    // адресат повідомлення
    std::string adresat;
    // оцінка повідомлення від системи боротьби зі спамом
    float spam{};
    // точна дата і час відправлення повідомлення
    povidom_time timestamp;
    // тип повідомлення
    MessageType message_type{};
    // відповідь на
    povidom_id reply_to{};
};

#endif /* !POVIDOM_H_SEEN */
