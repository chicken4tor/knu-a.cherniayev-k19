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
            throw out_of_range("Видаляемо не те");
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
    {}

    static DateList *create_empty()
    {
        return nullptr;
    }

private:

    DateList()
    {}
};

int main()
{
    IDateList *p{};

    // p = FixedDateList::create_empty(100);
    p = ArrayDateList::create_empty();
    // p = DateList::create_empty();

    Date a = {7, 7, 777};
    Date b = {9, 3, 2021};

    p->append(a);

    cout << p->length() << endl;

    p->append(b);

    cout << p->length() << endl;

    const Date &c = p->get(0);

    cout << c.year << endl;

    p->remove(0);

    cout << p->length() << endl;

    const Date &d = p->get(0);

    cout << d.year << endl;

    // Фатальна помилка

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
        while (true)
        {
            p->append(a);
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }

    delete p;

    return 0;
}
