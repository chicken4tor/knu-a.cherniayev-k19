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
#include <fstream>
#include <iomanip>
#include <chrono>


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

void print_list(IDateList *p)
{
    if (p != nullptr)
    {
        size_t len = p->length();
        bool comma = false;

        cout << "[";

        for (size_t i = 0; i < len; ++i)
        {
            if (comma)
            {
                cout << ", ";
            }

            const Date &d = p->get(i);

            cout << setfill('0') << setw(4) << d.year << "-" << setw(2) << d.month << "-" << setw(2) << d.day;

            comma = true;
        }

        cout << "]\n";
    }
}

const size_t BENCH_RAZIV = 50000;
typedef chrono::time_point<chrono::steady_clock> bench_time;

bench_time::duration benchmark_append(IDateList *p)
{
    Date d = {1, 2, 3};

    bench_time start = chrono::steady_clock::now();

    for (size_t i = 0; i < BENCH_RAZIV; ++i)
    {
        p->append(d);
    }

    bench_time end = chrono::steady_clock::now();

    return end - start;
}

bench_time::duration benchmark_insert(IDateList *p)
{
    Date d = {1, 2, 3};

    bench_time start = chrono::steady_clock::now();

    for (size_t i = 0; i < BENCH_RAZIV; ++i)
    {
        p->insert(0, d);
    }

    bench_time end = chrono::steady_clock::now();

    return end - start;
}

bench_time::duration benchmark_get_set(IDateList *p)
{
    bench_time start = chrono::steady_clock::now();

    for (size_t i = 0; i < BENCH_RAZIV; ++i)
    {
        Date d = p->get(i);
        p->set(i, d);
    }

    bench_time end = chrono::steady_clock::now();

    return end - start;
}

bench_time::duration benchmark_remove(IDateList *p)
{
    bench_time start = chrono::steady_clock::now();

    for (size_t i = 0; i < BENCH_RAZIV; ++i)
    {
        p->remove(0);
    }

    bench_time end = chrono::steady_clock::now();

    return end - start;
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

        if (verb == "create")
        {
            if (!args.empty())
            {
                if (p != nullptr)
                {
                    delete p;
                    p = nullptr;
                    list_type.clear();
                }

                if (args[0] == "fixed")
                {
                    try
                    {
                        size_t n{};

                        // Якщо нема аргументу, або там не число - спіймаємо помилку
                        n = stoul(args.at(1));

                        p = FixedDateList::create_empty(n);
                        list_type = "fixed";
                    }
                    catch(const std::exception& e)
                    {
                        std::cerr << e.what() << '\n';
                    }
                }
                else if (args[0] == "array")
                {
                    p = ArrayDateList::create_empty();
                    list_type = "array";
                }
                else if (args[0] == "list")
                {
                    p = ArrayDateList::create_empty();
                    list_type = "list";
                }
            }
            else
            {
                cout << "Потрібні аргументи" << endl;
            }
        }
        else if (verb == "append")
        {
            if (p != nullptr)
            {
                try
                {
                    Date d;

                    d.day = stoi(args.at(0));
                    d.month = stoi(args.at(1));
                    d.year = stoi(args.at(2));

                    p->append(d);
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
        else if (verb == "insert")
        {
            if (p != nullptr)
            {
                try
                {
                    Date d;

                    size_t k = stoul(args.at(0));

                    d.day = stoi(args.at(1));
                    d.month = stoi(args.at(2));
                    d.year = stoi(args.at(3));

                    p->insert(k, d);
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
        else if (verb == "remove")
        {
            if (p != nullptr)
            {
                try
                {
                    size_t k = stoul(args.at(0));

                    p->remove(k);
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
        else if (verb == "get")
        {
            if (p != nullptr)
            {
                try
                {
                    const Date &d = p->get(stoul(args.at(0)));

                    cout << setfill('0') << setw(4) << d.year << "-" << setw(2) << d.month << "-" << setw(2) << d.day << endl;
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
        else if (verb == "set")
        {
            if (p != nullptr)
            {
                try
                {
                    Date d;

                    size_t k = stoul(args.at(0));

                    d.day = stoi(args.at(1));
                    d.month = stoi(args.at(2));
                    d.year = stoi(args.at(3));

                    p->set(k, d);
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
        else if (verb == "length")
        {
            if (p != nullptr)
            {
                cout << p->length() << endl;
            }
            else
            {
                cout << "Спочатку створи" << endl;
            }
        }
        else if (verb == "dump")
        {
            if (p != nullptr)
            {
                print_list(p);
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
        else if (verb == "benchmark")
        {
            try
            {
                IDateList *l1{};

                cout << "Пристебніться, взлітаємо" << endl;

                // Результати - 3 типа по 4 теста
                bench_time::duration results[3][4]{};

                // FixedDateList
                l1 = FixedDateList::create_empty(BENCH_RAZIV);
                results[0][0] = benchmark_append(l1);
                delete l1;

                l1 = FixedDateList::create_empty(BENCH_RAZIV);
                results[0][1] = benchmark_insert(l1);
                results[0][2] = benchmark_get_set(l1);
                results[0][3] = benchmark_remove(l1);
                delete l1;

                cout << "... fixed" << endl;

                // ArrayDateList
                l1 = ArrayDateList::create_empty();
                results[1][0] = benchmark_append(l1);
                delete l1;

                l1 = ArrayDateList::create_empty();
                results[1][1] = benchmark_insert(l1);
                results[1][2] = benchmark_get_set(l1);
                results[1][3] = benchmark_remove(l1);
                delete l1;

                cout << ".. array" << endl;

                l1 = ArrayDateList::create_empty();

                // DateList
                l1 = DateList::create_empty();
                results[2][0] = benchmark_append(l1);
                delete l1;

                l1 = DateList::create_empty();
                results[2][1] = benchmark_insert(l1);
                results[2][2] = benchmark_get_set(l1);
                results[2][3] = benchmark_remove(l1);
                delete l1;

                cout << ". list" << endl;

                const char *impl_label[] = {"FixedDateList", "ArrayDateList", "DateList"};

                cout << "Results in milliseconds " << BENCH_RAZIV << " elements(iterations)" << "\n              |  append  |  insert  |  get/set |  remove\n";

                for (int i = 0; i < 3; ++i)
                {
                    cout << left << setw(14) << impl_label[i] << "|";
                    for (int j = 0; j < 4; ++j)
                    {
                        chrono::duration<double, milli> fp_ms = results[i][j];
                        cout << fixed << setw(10) << right << setprecision(4) << fp_ms.count() << "|";
                    }
                    cout << "\n";
                }
            }
            catch(const std::exception& e)
            {
                std::cerr << e.what() << '\n';
            }
        }
        else if (verb == "quit" || verb == "die")
        {
            return false;
        }
        else if (verb == "help")
        {
            cout << "Акт творіння - create fixed <кількість елементів> або array або list\n"
                    "Додати елемент - append день місяць рік\n"
                    "Вставити елемент - insert k день місяць рік\n"
                    "Видалити елемент - remove k\n"
                    "Виводимо елемент - get k\n"
                    "Обновляємо елемент - set k день місяць рік\n"
                    "Довжина списку - length\n"
                    "Всі елементи - dump\n"
                    "Демонстрацийний режим - demo <ім'я файла з командами>\n"
                    "Режим вимірювання ефективності - benchmark\n"
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

    cout << "Lab 2a 1.2\n";
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
