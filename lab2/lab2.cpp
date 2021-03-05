// Реалізувати структуру даних «список» з наступними операціями:
// 1. create_empty – створення пустого списку;
// 2. append – додавання елементу в кінець списку;
// 3. insert – вставка елементу перед елементом з індексом k;
// 4. remove – видалення елементу з індексом k;
// 5. get, set – доступ до елементу за індексом;
// 6. length – довжина списку.

// В структурі зберігаються дати (день, місяць, рік)

// Використовуючи три різні підходи:
// 1. На основі масиву фіксованого розміру. При виході за межі масиву
//    видавати повідомлення про помилку.
// 2. На основі масиву змінного розміру (ArrayList, наприклад, std::vector).
// 3. На основі зв’язного списку. Самостійно обрати однозв’язний чи
//    двозв’язний список та інші деталі реалізації.

// Необхідно реалізувати наступні режими роботи для реалізованої програми:
// 1. Інтерактивний діалоговий режим – коли користувач може вибирати,
//    які операції виконувати та задавати необхідні параметри.
// 2. Демонстраційний режим – задаються фіксовані значення параметрів
//    та послідовності виконання операцій, що демонструють правильність роботи
//    структур даних та операцій в різних ситуаціях. В цьому режимі користувач
//    не має нічого вводити.
// 3. Автоматичний режим “benchmark” з вимірами ефективності роботи різних
//    версій структур даних.


#include <iostream>
#include <vector>
#include <stdexcept>
#include <sstream>


using namespace std;


struct Date
{
    short day{};
    short month{};
    int year{};
};


class IDateList
{
public:
    // додавання елементу в кінець списку
    virtual void append(const Date &value) = 0;
    // вставка елементу перед елементом з індексом k;
    virtual void insert(size_t k, const Date &value) = 0;
    // видалення елементу з індексом k
    virtual void remove(size_t k) = 0;
    // доступ до елементу за індексом k
    virtual const Date &get(size_t k) = 0;
    // доступ до елементу за індексом k
    virtual void set(size_t k, const Date &value) = 0;
    // довжина списку
    virtual size_t length() = 0;

    virtual ~IDateList()
    {}
};

class FixedDateList : public IDateList
{
public:

    ~FixedDateList()
    {
        delete[] date_store;
    }

    static FixedDateList *create_empty(size_t max_size)
    {
        return new FixedDateList(max_size);
    }

    void append(const Date &value)
    {
        if (tail_element < total_count)
        {
            date_store[tail_element] = value;
            ++tail_element;
        }
        else
        {
            // Помилка
            throw out_of_range("Вже не лізе");
        }
    }

    void insert(size_t k, const Date &value)
    {
        if (tail_element < total_count)
        {
            if (k < tail_element)
            {
                // Вставляємо десь по середині, звільнюємо місце для нового елементу
                for (size_t i = tail_element; i > k; --i)
                {
                    date_store[i] = move(date_store[i - 1]);
                }
            }
            date_store[k] = value;
            ++tail_element;
        }
        else
        {
            // Помилка
            throw out_of_range("Вставляємо не туди");
        }
    }

    void remove(size_t k)
    {
        if (k < tail_element)
        {
            --tail_element;  // Зменшуємо, щоб можна було використовувати (i + 1)
            for (size_t i = k; i < tail_element; ++i)
            {
                date_store[i] = move(date_store[i + 1]);
            }
        }
        else
        {
            // Помилка
            throw out_of_range("Видаляємо не те");
        }
    }

    const Date &get(size_t k)
    {
        if (k < tail_element)
        {
            return date_store[k];
        }
        else
        {
            // Помилка
            throw out_of_range("Ліземо не туди");
        }
    }

    void set(size_t k, const Date &value)
    {
        if (k < tail_element)
        {
            date_store[k] = value;
        }
        else
        {
            // Помилка
            throw out_of_range("Ліземо не туди");
        }
    }

    size_t length()
    {
        return tail_element;
    }

private:

    Date *date_store;
    size_t total_count;
    size_t tail_element{};

    FixedDateList(size_t max_size) : total_count(max_size)
    {
        date_store = new Date[max_size];
    }
};

class ArrayDateList : public IDateList
{
public:

    static ArrayDateList *create_empty()
    {
        return new ArrayDateList;
    }

    void append(const Date &value)
    {
        date_store.push_back(value);
    }

    void insert(size_t k, const Date &value)
    {
        date_store.insert(begin(date_store) + k, value);
    }

    void remove(size_t k)
    {
        date_store.erase(begin(date_store) + k);
    }

    const Date &get(size_t k)
    {
        return date_store.at(k);
    }

    void set(size_t k, const Date &value)
    {
        date_store.at(k) = move(value);
    }

    size_t length()
    {
        return date_store.size();
    }

private:

    vector<Date> date_store;

    ArrayDateList()
    {}
};

class DateList : public IDateList
{
public:

    ~DateList()
    {
        Node *node = head;
        Node *prev_node{};

        // Добіваєм виживших
        while (node != nullptr)
        {
            prev_node = node;
            node = node->next;

            delete prev_node;
        }
    }

    static DateList *create_empty()
    {
        return new DateList;
    }

    void append(const Date &value)
    {
        Node *p = new Node;
        p->date = value;

        if (head == nullptr)
        {
            // Ти мій перший
            head = p;
        }
        else
        {
            Node *node = head;

            // Йдемо до останнього
            while (node->next != nullptr)
            {
                node = node->next;
            }

            node->next = p;
        }
    }

    void insert(size_t k, const Date &value)
    {
        Node *p = new Node;
        p->date = value;

        if (k == 0)
        {
            // Вставимо в голову
            p->next = head;
            head = p;
        }
        else
        {
            Node *node = get_previous_k(k);

            if (node != nullptr)
            {
                // Вставляємо в проміжок
                p->next = node;
                node->next = p;
            }
            else
            {
                delete p;

                // Помилка
                throw out_of_range("Вставляємо не туди");
            }
        }
    }

    void remove(size_t k)
    {
        Node *node{};

        if (k == 0)
        {
            // Уберемо із голови
            node = head;
            head = head->next;
        }
        else
        {
            Node *prev_node = get_previous_k(k);

            if (prev_node != nullptr && prev_node->next != nullptr)
            {
                // Зберігаємо, для подальшого видалення з пам'яти
                node = prev_node->next;

                // Видаляємо з проміжку
                prev_node->next = prev_node->next->next;
            }
            else
            {
                // Помилка
                throw out_of_range("Видаляємо не те");
            }
        }

        delete node;
    }

    const Date &get(size_t k)
    {
        Node *node{};

        if (k == 0)
        {
            node = head;
        }
        else
        {
            node = get_k(k);
        }

        if (node != nullptr)
        {
            return node->date;
        }
        else
        {
            // Помилка
            throw out_of_range("Ліземо не туди");
        }
    }

    void set(size_t k, const Date &value)
    {
        Node *node{};

        if (k == 0)
        {
            node = head;
        }
        else
        {
            node = get_k(k);
        }

        if (node != nullptr)
        {
            node->date = move(value);
        }
        else
        {
            // Помилка
            throw out_of_range("Ліземо не туди");
        }
    }

    size_t length()
    {
        size_t l = 0;

        Node *node = head;

        while (node != nullptr)
        {
            node = node->next;
            ++l;
        }

        return l;
    }

private:

    // Обираємо однозв’язний, сподіваємось що це буде простіше.
    // https://en.wikipedia.org/wiki/Linked_list#Singly_linked_list
    struct Node
    {
        // Тут зберігаємо данні
        Date date;
        // Наступний елемент списку, nullptr якщо кінець
        Node *next{};
    };

    // Це голова
    Node *head{};

    DateList()
    {}

    // Цей метод визивається для k >= 1
    Node *get_previous_k(size_t k)
    {
        Node *prev_node{};
        Node *node = head;

        size_t pos = 0;

        for (; pos < k; ++pos)
        {
            if (node != nullptr)
            {
                prev_node = node;
                node = node->next;
            }
            else
            {
                break;
            }
        }

        // Якщо досягнуте k
        if (pos == k)
        {
            return prev_node;
        }
        else
        {
            return nullptr;
        }
    }

    Node *get_k(size_t k)
    {
        Node *node = get_previous_k(k);

        if (node != nullptr)
        {
            return node->next;
        }

        return nullptr;
    }
};

void test_list(IDateList *p)
{
    Date a = {7, 7, 777};
    Date b = {9, 3, 2021};

    p->append(a);

    cout << p->length() << endl;

    p->insert(0, b);

    p->append(b);

    cout << p->length() << endl;

    const Date &c = p->get(0);

    cout << c.year << endl;

    p->remove(0);

    cout << p->length() << endl;

    const Date &d = p->get(0);

    cout << d.year << endl;

    // Фатальні помилки

    try
    {
        p->get(10);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }

    try
    {
        p->get(-1);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }

    try
    {
        // Нескінченне додавання дуже повільно у DateList
        for (int i = 0; i < 1000; ++i)
        {
            p->append(a);
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
}

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

        if (verb == "")
        {

        }
        else if (verb == "quit")
        {
            return false;
        }
        else if (verb == "help")
        {
            cout << "Считування - load\n"
                    "Збереження - save\n"
                    "Тип бази - mode bin або txt\n"
                    "\"Авторизація\" - login <ім'я автора повідомлення>\n"
                    "Відправити повідомлення - send <тип - news, question, answer, invite, comment> адресат повідомлення\n"
                    "Пошук за початком повідомлення - search_pov <початок повідомлення>\n"
                    "Пошук за типом та оцінкою - search_ocin <тип - news, question, answer, invite, comment> <оцінка>\n"
                    "Пошук за автором та часом - search_chas <ім'я автора повідомлення>\n"
                    "Початок діапазону пошуку - from <час у форматі ISO:8601>\n"
                    "Кінець діапазону пошуку - to <час у форматі ISO:8601>\n"
                    "Показати - show <id повідомлення>\n"
                    "Редагування - edit <id повідомлення>\n"
                    "Оновлення данних - update <type або spam або message> <нове значення>\n"
                    "Видалити - delete <id повідомлення>\n"
                    "Всі повідомлення - dump\n"
                    "Демонстрацийний режим - demo <ім'я файла з командами>\n"
                    "Режим вимірювання ефективності - benchmark <кількість елементів>\n"
                    "Вихід - quit\n"
                    "Ця інформація - help\n";
        }
        else
        {
            cout << "Невідома команда (скористуйтеся help) " << command << endl;
        }

        return true;
    }

    const string &custom_prompt()
    {
        return list_type;
    }

private:
    IDateList *p{};

    string list_type;
};

ServerDemo::ServerDemo(/* args */)
{
}

ServerDemo::~ServerDemo()
{
}


int main()
{
    // IDateList *p{};

    // p = FixedDateList::create_empty(5);
    // test_list(p);
    // delete p;

    // p = ArrayDateList::create_empty();
    // test_list(p);
    // delete p;

    // p = DateList::create_empty();
    // test_list(p);
    // delete p;

    ServerDemo demo;

    // https://askubuntu.com/questions/558280/changing-colour-of-text-and-background-of-terminal
    const string user_input = "$ \e[31m";  // 31 - Red
    const string reset_color = "\e[0m";

    string command;

    cout << "Connected to server\n";
    cout << user_input;

    while (getline(cin, command))
    {
        // Повертаємо колір
        cout << reset_color;

        if (!demo.process_command(command))
        {
            break;
        }

        cout << demo.custom_prompt() << user_input;
    }

    // Повертаємо колір
    cout << reset_color;

    return 0;
}
