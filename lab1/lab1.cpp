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

// Необхідно реалізувати наступні режими роботи для реалізованої програми:
// 1.  Інтерактивний діалоговий режим – коли користувач може вибирати,
//     які операції виконувати та задавати необхідні параметри.
// 2.  Демонстраційний режим – задаються фіксовані значення параметрів та
//     послідовності виконання операцій, що демонструють правильність роботи
//     операцій в різних ситуаціях. В цьому режимі користувач не має нічого вводити.
// 3.  Автоматичний режим “benchmark” з вимірами ефективності роботи різних
//     механізмів зберігання.


#include <iostream>
#include <vector>
#include <ctime>
#include <fstream>
#include <chrono>
#include <algorithm>
#include <sstream>


using namespace std;

enum MessageType {
    Unknown, News, Question, Answer, Invite, Comment, 
};

MessageType from_str(const string &str)
{
    MessageType result = Unknown;

    if (str == "news")
    {
        result = News;
    }
    else if (str == "question")
    {
        result = Question;
    }
    else if (str == "answer")
    {
        result = Answer;
    }
    else if (str == "invite")
    {
        result = Invite;
    }
    else if (str == "comment")
    {
        result = Comment;
    }

    return result;
}

// спеціальний адресат, що позначає повідомлення для всіх
const std::string EVERYBODY = "h311_666_da3m0n";

// Імена файлів
const std::string DATA_TEXT = "data_txt";
const std::string DATA_BINARY = "data_bin";

// Роздільник для текстового формату
const char FIELD_DELIM = '\t';

typedef unsigned long long povidom_id;
typedef chrono::time_point<chrono::system_clock> povidom_time;

// Дата у форматі - 2003-10-19T04:45:34
// https://en.wikipedia.org/wiki/ISO_8601
povidom_time parse_date(const string &date);


struct Povidom
{
    // ціле число, яке буде унікальним для кожного елементу даних
    povidom_id id{};
    // текст повідомлення
    string povidom;
    // користувач-автор повідомлення
    string avtor;
    // адресат повідомлення
    string adresat;
    // оцінка повідомлення від системи боротьби зі спамом
    float spam{};
    // точна дата і час відправлення повідомлення
    povidom_time timestamp;
    // тип повідомлення
    MessageType message_type{};
    // відповідь на
    povidom_id reply_to{};
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
    vector <Povidom> messages;

    povidom_id new_id{};

    const Povidom no_povidom;

    // Чи є зміни у векторі
    bool dirty_data{};

public:
    ServerPovidom(/* args */);
    ~ServerPovidom();

    povidom_id add_pov(const string &povidom, const string &avtor, const string &adresat, MessageType message_type)
    {
        Povidom new_povidom;

        new_povidom.id = ++new_id;
        new_povidom.povidom = povidom;
        new_povidom.avtor = avtor;
        new_povidom.adresat = adresat;
        new_povidom.message_type = message_type;
        new_povidom.timestamp = chrono::system_clock::now();
        new_povidom.spam = 1.0;

        dirty_data = true;
        messages.push_back(new_povidom);

        return new_povidom.id;
    }

    const Povidom &get_pov(povidom_id id)
    {
        for (const Povidom &povidom : messages)
        {
            if (povidom.id == id)
            {
                return povidom;
            }
        }

        // Пусте повідомлення якщо нічого не знайдео
        return no_povidom;
    }

    void show_pov(const Povidom &povidom)
    {
        time_t message_time = chrono::system_clock::to_time_t(povidom.timestamp);

        cout << "id - " << povidom.id
            << "\n  sender: " << povidom.avtor
            << "\n  receiver: " << povidom.adresat
            << "\n  type: " << povidom.message_type
            << "\n  spam: " << povidom.spam
            << "\n  time: " << ctime(&message_time)
            << "  message: " << povidom.povidom
            << "\n";
    }

    // повідомлення, що починаються із заданого фрагменту тексту
    int search_pov_text(const string &poshuk, vector<povidom_id> &result)
    {
        result.clear();

        for (const Povidom &povidom : messages)
        {
            if (povidom.povidom.rfind(poshuk, 0) == 0)
            {
                result.push_back(povidom.id);
            }
        }

        return 0;
    }

    // повідомлення заданого типу з оцінкою не менше заданої
    int search_pov_ocin(MessageType message_type, int spam, vector<povidom_id> &result)
    {
        result.clear();

        for (const Povidom &povidom : messages)
        {
            if (povidom.message_type == message_type && povidom.spam > spam)
            {
                result.push_back(povidom.id);
            }
        }

        return 0;
    }

    // повідомлення від заданого автора, надіслані в заданому діапазоні часу
    int search_pov_avtor_chas(const string &avtor, const povidom_time &start_date, const povidom_time &end_date, vector<povidom_id> &result)
    {
        // Перевіремо дати
        if (start_date == povidom_time() || end_date == povidom_time())
        {
            // Шось не те
            return 1;
        }

        if (start_date > end_date)
        {
            // Початок піздніший за кінець
            return 2;
        }

        result.clear();

        for (const auto &povidom : messages)
        {
            if (povidom.timestamp <= end_date && povidom.timestamp >= start_date)
            {
                if (avtor == povidom.avtor)
                {
                    result.push_back(povidom.id);
                }
            }
        }

        return 0;
    }

    bool mod_pov(povidom_id id, const Povidom &updated)
    {
        if (id != updated.id)
        {
            // Спроба редагування іншого повідомлення
            return false;
        }

        for (Povidom &povidom : messages)
        {
            if (povidom.id == id)
            {
                povidom = updated;
                // Змінюємо дату після редагування
                povidom.timestamp = chrono::system_clock::now();
                dirty_data = true;
                return true;
            }
        }

        return false;
    }

    bool del_pov(povidom_id id)
    {
        auto it = remove_if(begin(messages), end(messages), [id](const Povidom &x){ return x.id == id; });
        auto num_elements = distance(it, end(messages));
        messages.erase(it, end(messages));

        if (num_elements != 0)
        {
            dirty_data = true;
            return true;
        }
        else
        {
            return false;
        }
    }

    void dump()
    {
        for (const Povidom &povidom : messages)
        {
            show_pov(povidom);
        }
    }

    bool save_txt()
    {
        ofstream db(DATA_TEXT);

        // Зберігаємо, на майбутне
        db << new_id << '\n';

        for (const Povidom &povidom : messages)
        {
            db << povidom.id << FIELD_DELIM
               << povidom.avtor << FIELD_DELIM
               << povidom.adresat << FIELD_DELIM
               << povidom.message_type << FIELD_DELIM
               << povidom.spam << FIELD_DELIM
               << chrono::duration_cast<chrono::milliseconds>(povidom.timestamp.time_since_epoch()).count() << FIELD_DELIM
               << povidom.povidom <<'\n';
        }

        dirty_data = false;

        return true;
    }

    bool save_bin()
    {
        ofstream db(DATA_BINARY);

        db.write(reinterpret_cast<const char *>(&new_id), sizeof(new_id));

        for (const Povidom &povidom : messages)
        {
            string::size_type len;

            // id
            db.write(reinterpret_cast<const char *>(&povidom.id), sizeof(povidom.id));

            // avtor
            len = povidom.avtor.length();
            db.write(reinterpret_cast<const char *>(&len), sizeof(len));
            db.write(povidom.avtor.c_str(), len);

            // adresat
            len = povidom.adresat.length();
            db.write(reinterpret_cast<const char *>(&len), sizeof(len));
            db.write(povidom.adresat.c_str(), len);

            // message_type
            db.write(reinterpret_cast<const char *>(&povidom.message_type), sizeof(povidom.message_type));

            // spam
            db.write(reinterpret_cast<const char *>(&povidom.spam), sizeof(povidom.spam));

            // timestamp
            db.write(reinterpret_cast<const char *>(&povidom.timestamp), sizeof(povidom.timestamp));

            // povidom
            len = povidom.povidom.length();
            db.write(reinterpret_cast<const char *>(&len), sizeof(len));
            db.write(povidom.povidom.c_str(), len);
        }

        dirty_data = false;

        return true;
    }

    bool load_txt()
    {
        ifstream db(DATA_TEXT);

        db >> new_id;
        db >> ws;  // Дочитаємо перший рядок до кінця

        string temp;
        vector<string> fields;

        while (getline(db, temp))
        {
            string::size_type prev_pos{};
            string::size_type pos{};

            fields.clear();

            while ((pos = temp.find(FIELD_DELIM, prev_pos)) != string::npos)
            {
                fields.push_back(temp.substr(prev_pos, pos - prev_pos));
                prev_pos = pos + 1;
            }

            fields.push_back(temp.substr(prev_pos));

            if (fields.size() == 7)
            {
                Povidom povidom;

                povidom.id = stoull(fields[0]);
                povidom.avtor = fields[1];
                povidom.adresat = fields[2];
                povidom.message_type = static_cast<MessageType>(stoi(fields[3]));
                povidom.spam = stof(fields[4]);
                povidom.timestamp = povidom_time(chrono::milliseconds(stol(fields[5])));
                povidom.povidom = fields[6];

                messages.push_back(povidom);
            }
        }

        dirty_data = false;

        return true;
    }

    bool load_bin()
    {
        ifstream db(DATA_BINARY);

        db.read(reinterpret_cast<char *>(&new_id), sizeof(new_id));

        povidom_id id{};

        // Читаємо ід, поки не доберемося до кінця файлу
        while (db.read(reinterpret_cast<char *>(&id), sizeof(id)))
        {
            Povidom povidom;

            string::size_type len;

            // id
            povidom.id = id;

            // avtor
            db.read(reinterpret_cast<char *>(&len), sizeof(len));
            povidom.avtor.resize(len);
            db.read(&povidom.avtor[0], len);

            // adresat
            db.read(reinterpret_cast<char *>(&len), sizeof(len));
            povidom.adresat.resize(len);
            db.read(&povidom.adresat[0], len);

            // message_type
            db.read(reinterpret_cast<char *>(&povidom.message_type), sizeof(povidom.message_type));

            // spam
            db.read(reinterpret_cast<char *>(&povidom.spam), sizeof(povidom.spam));

            // timestamp
            db.read(reinterpret_cast<char *>(&povidom.timestamp), sizeof(povidom.timestamp));

            // povidom
            db.read(reinterpret_cast<char *>(&len), sizeof(len));
            povidom.povidom.resize(len);
            db.read(&povidom.povidom[0], len);

            messages.push_back(povidom);
        }

        dirty_data = false;

        return true;
    }

    bool is_dirty()
    {
        return dirty_data;
    }
};

ServerPovidom::ServerPovidom(/* args */)
{
}

ServerPovidom::~ServerPovidom()
{
}

povidom_time parse_date(const string &date)
{
    povidom_time timestamp;

    int year{};
    int month{};
    int day{};
    int hours{};
    int minutes{};
    int seconds{};

    int assigned = sscanf(date.c_str(), "%4d-%2d-%2dT%2d:%2d:%2d",
        &year, &month, &day, &hours, &minutes, &seconds);

    // Якщо у нас є рік, місяць та день
    if (assigned >= 3)
    {
        tm tempus{};

        tempus.tm_year = year - 1900;
        tempus.tm_mon = month - 1;
        tempus.tm_mday = day;
        tempus.tm_hour = hours;
        tempus.tm_min = minutes;
        tempus.tm_sec = seconds;

        time_t tt = mktime(&tempus);
        if (tt != -1)
        {
            timestamp = chrono::system_clock::from_time_t(tt);
        }
    }

    return timestamp;
}


class ServerDemo
{
    ServerPovidom s1;

    // Відправник повідомлення
    string avtor;
    // Діапазон часу
    povidom_time from;
    povidom_time to;
    //
    bool binary_mode{};
    // Повідомлення для редагування або видалення
    povidom_id up_id{};
    Povidom up;
    // Результат пошуку
    vector<povidom_id> results;

    povidom_id id_from_str(const string &id_str)
    {
        int special_id = -1;
        povidom_id id = strtoull(id_str.c_str(), NULL, 10);

        if (id == 0)
        {
            // Можливо маємо справу із спец-id для відносної адресації
            for (char c : id_str)
            {
                if (c == '_')
                {
                    ++special_id;
                }
                else
                {
                    // спец-id тільки якщо цілком із ________
                    special_id = -1;
                    break;
                }
            }

            if (special_id != -1 && special_id < results.size())
            {
                id = results[special_id];
            }
        }

        return id;
    }

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

        if (verb == "load")
        {
            bool loaded{};
            if (binary_mode)
            {
                loaded = s1.load_bin();
            }
            else
            {
                loaded = s1.load_txt();
            }
            cout << (loaded ? "Успіх" : "Невдача") << endl;
        }
        else if (verb == "save")
        {
            bool saved{};
            if (binary_mode)
            {
                saved = s1.save_bin();
            }
            else
            {
                saved = s1.save_txt();
            }
            cout << (saved ? "Успіх" : "Невдача") << endl;
        }
        else if (verb == "mode")
        {
            bool success{};
            if (args.size() == 1)
            {
                if (args[0] == "bin")
                {
                    binary_mode = true;
                    success = true;
                }
                else if (args[0] == "txt")
                {
                    binary_mode = false;
                    success = true;
                }
            }

            if (!success)
            {
                // Якщо режим не змінився - показуемо, те що є
                cout << "mode is binary - " << boolalpha << binary_mode << endl;
            }
        }
        else if (verb == "login")
        {
            bool success{};

            if (args.size() == 1)
            {
                if (args[0] != EVERYBODY)
                {
                    avtor = args[0];
                    success = true;
                }
            }

            if (!success)
            {
                cout << "Невдала спроба" << endl;
            }
        }
        else if (verb == "from")
        {
            povidom_time t;

            if (args.size() == 1)
            {
                t = parse_date(args[0]);
            }

            if (t == povidom_time())
            {
                cout << "Можливо невірні дата та час" << endl;
            }
            else
            {
                from = t;

                if (to < from)
                {
                    cout << "Невірний діапазон часу (кінець раніше початку)" << endl;
                }
            }
        }
        else if (verb == "to")
        {
            povidom_time t;

            if (args.size() == 1)
            {
                t = parse_date(args[0]);
            }

            if (t == povidom_time())
            {
                cout << "Можливо невірні дата та час" << endl;
            }
            else
            {
                to = t;

                if (to < from)
                {
                    cout << "Невірний діапазон часу (кінець раніше початку)" << endl;
                }
            }
        }
        else if (verb == "send")
        {
            if (args.size() > 2)
            {
                MessageType message_type = from_str(args[0]);

                if (message_type != Unknown)
                {
                    // Знайдемо початок повідомлення
                    ss.clear();  // Убираємо признак кінця файлу
                    ss.seekg(0, ios_base::beg);  // На початок стріму
                    ss >> arg >> arg >> arg >> ws;  // Ігноруємо команду, тип повідомлення та адресата
                    getline(ss, arg);
                    povidom_id id = s1.add_pov(arg, avtor, args[1], message_type);
                    if (id != 0)
                    {
                        results.clear();
                        results.push_back(id);
                    }
                }
                else
                {
                    cout << "Перевірте тип повідомлення" << endl;
                }
            }
            else
            {
                cout << "Скористуйтеся допомогою" << endl;
            }
        }
        else if (verb == "search_pov")
        {
            if (!args.empty())
            {
                // Знайдемо початок повідомлення
                ss.clear();  // Убираємо признак кінця файлу
                ss.seekg(0, ios_base::beg);  // На початок стріму
                ss >> arg >> ws;  // Ігноруємо команду
                getline(ss, arg);

                if (s1.search_pov_text(arg, results) == 0)
                {
                    cout << "Знайдено - " << results.size() << endl;
                }
            }
        }
        else if (verb == "search_ocin")
        {
            if (args.size() == 2)
            {
                try
                {
                    MessageType message_type = from_str(args[0]);

                    float ocinka = stof(args[1]);

                    if (s1.search_pov_ocin(message_type, ocinka, results) == 0)
                    {
                        cout << "Знайдено - " << results.size() << endl;
                    }
                }
                catch(const std::exception& e)
                {
                    std::cerr << e.what() << '\n';
                }
            }
        }
        else if (verb == "search_chas")
        {
            if (args.size() == 1)
            {
                if (s1.search_pov_avtor_chas(args[0], from, to, results) == 0)
                {
                    cout << "Знайдено - " << results.size() << endl;
                }
            }
        }
        else if (verb == "show")
        {
            bool success{};

            if (args.empty())
            {
                // показуємо нове повідомлення, або результати пошуку
                for (povidom_id id : results)
                {
                    const Povidom &povidom = s1.get_pov(id);
                    s1.show_pov(povidom);
                    success = true;
                }
            }
            else
            {
                for (const string &id_str : args)
                {
                    povidom_id id = id_from_str(id_str);

                    if (id != 0)
                    {
                        const Povidom &povidom = s1.get_pov(id);
                        s1.show_pov(povidom);
                        success = true;
                    }
                }
            }

            if (!success)
            {
                cout << "Повідомлення не знайдено" << endl;
            }
        }
        else if (verb == "delete")
        {
            for (const string &id_str : args)
            {
                povidom_id id = id_from_str(id_str);

                if (id != 0)
                {
                    if (s1.del_pov(id))
                    {
                        cout << "Deleted - " << id << endl;
                    }
                }
            }
        }
        else if (verb == "dump")
        {
            s1.dump();
        }
        else if (verb == "demo")
        {
            if (args.size() == 1)
            {
                try
                {
                    ifstream scenario_to_play(args[0]);

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
        else if (verb == "edit")
        {
            if (args.size() == 1)
            {
                povidom_id id = id_from_str(args[0]);

                if (id != 0)
                {
                    up = s1.get_pov(id);
                    if (up.id == id)
                    {
                        up_id = id;
                    }
                }
            }
            else
            {
                up_id = 0;
                cout << "Редагування завершено" << endl;
            }
        }
        else if (verb == "update")
        {
            if (args.size() >= 2)
            {
                if (up_id != 0)
                {
                    bool updated{};

                    if (args[0] == "type")
                    {
                        MessageType message_type = from_str(args[1]);

                        if (message_type != Unknown)
                        {
                            up.message_type = message_type;
                            updated = true;
                        }
                    }
                    else if (args[0] == "spam")
                    {
                        try
                        {
                            float ocinka = stof(args[1]);

                            if (ocinka >= 0.f)
                            {
                                up.spam = ocinka;
                                updated = true;
                            }
                        }
                        catch(const std::exception& e)
                        {
                            std::cerr << e.what() << '\n';
                        }
                    }
                    else if (args[0] == "message")
                    {
                        // Знайдемо початок повідомлення
                        ss.clear();  // Убираємо признак кінця файлу
                        ss.seekg(0, ios_base::beg);  // На початок стріму
                        ss >> arg >> ws;  // Ігноруємо команду
                        getline(ss, arg);

                        up.povidom = arg;
                        updated = true;
                    }
                    else
                    {
                        cout << "Невідоме поле - " << args[0] << endl;
                    }

                    if (updated)
                    {
                        if (!s1.mod_pov(up_id, up))
                        {
                            cout << "Оновлення не прошло" << endl;
                        }
                    }
                }
                else
                {
                    cout << "Виберить повідомлення за допомогою команди edit" << endl;
                }
            }
            else
            {
                up_id = 0;
                cout << "Редагування завершено" << endl;
            }
        }
        else if (verb == "benchmark")
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
        return avtor;
    }
};

ServerDemo::ServerDemo()
{
}

ServerDemo::~ServerDemo()
{
}


int main()
{
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
