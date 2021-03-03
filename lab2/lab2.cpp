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


#include <vector>


using namespace std;


struct Date
{
    int year{};
    short month{};
    short day{};
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
};

class FixedDateList : public IDateList
{
public:

    static FixedDateList *create_empty(size_t max_szie)
    {
        return nullptr;
    }

private:

    Date *date_store;
    size_t last_element;

    FixedDateList()
    {}
};

class ArrayDateList : public IDateList
{
public:

    static ArrayDateList *create_empty()
    {
        return nullptr;
    }

private:

    vector<Date> date_store;

    ArrayDateList();
};

class DateList : public IDateList
{
public:

    static DateList *create_empty()
    {
        return nullptr;
    }

private:

    DateList();
};

int main()
{
    IDateList *p = FixedDateList::create_empty(100);

    return 0;
}
