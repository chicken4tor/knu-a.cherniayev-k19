// Реалізувати інтерпретатор підмножини стекової мови Forth.

// Вирази цієї мови складаються з (цілочисельних) констант,
// які записуються на стек, і операторів,
// які забирають якусь кількість елементів зі стеку,
// обчислюють результат і викладають його на стек.

// Реалізувати оператори для арифметичних дій (‘+’ ‘-‘ ‘*’ ‘/’ MOD),
// оператор виводу на екран ‘.’,
// оператори маніпулювання елементами стеку:
//  n PICK – повторює на вершині стеку елемент з глибини n ( a0 .. an n -- a0 .. an a0 ),
//  n ROLL – виносить на вершину стеку елемент з глибини n ( a0 .. an n -- a1 .. an a0 ).
// Також реалізувати можливість визначення нових операторів
//  :<new-word> op1 .. opn ;
// Якщо новий оператор зустрічається у виразі,
// він замінюється на відповідну послідовність операторів.

// Необхідно реалізувати наступні режими роботи:
// 1. Інтерактивний діалоговий режим – коли користувач може вибирати, які операції
//    виконувати та задавати необхідні параметри.
// 2. Демонстраційний режим – задаються фіксовані значення та послідовності виконання
//    операцій, що демонструють правильність роботи структур даних та операцій
//    в різних ситуаціях. В цьому режимі користувач не має нічого вводити.


#include <iostream>
#include <vector>
#include <sstream>
#include <map>


using namespace std;

void report_error()
{
    cerr << "Something is wrong\n";
    exit(1);
}

// Grammar

/*
    <program>     --> (<statement> ';')*
    <statement>   --> <assignment> | <expression>
    <assignement> --> ID '=' <expression>
    <expression>  --> <term> ( <term> ('+'|'-') )*
    <term>        --> <factor> ( <factor> ('*'|'/'|'%') )*
    <factor>      --> ('+'|'-')* ID | NUMBER | '(' <expression> ')'
 */


// Scaner

enum TokenType
{
    NUMBER,
    PLUS,      // +
    MINUS,     // -
    MULTIPLY,  // *
    DIVIDE,    // /
    MODULE,    // %
    // RPAREN,    // )
    // LPAREN,    // (
    ASSIGN,    // =
    SEMICOLON, // ;
    ID,        // "Variable" name
    END,
};

string TokenString(TokenType tt)
{
    switch (tt)
    {
    case PLUS:
        return "+";
    case MINUS:
        return "-";
    case MULTIPLY:
        return "*";
    case DIVIDE:
        return "/";
    case ASSIGN:
        return "=";
    case MODULE:
        return "%";
    }

    return "UNKNOWN";
}

struct Token
{
    TokenType type{};
    long value{};
    string name;
};

int tindex = 0;
vector<Token> tokens;

Token scanner();

void lexus(const string &expr);

vector<long> values_stack;

map<string, long> variables;

Token lookahead;

// Parser

bool match(TokenType tt);

void prog();
void statement();
void assignment();
void expression();
void term();
void factor();

bool eva(const string &expr);


int main()
{
    string data1;

    // https://askubuntu.com/questions/558280/changing-colour-of-text-and-background-of-terminal
    const string user_input = "$ \e[31m";  // 31 - Red
    const string reset_color = "\e[0m";

    cout << user_input;

    while (getline(cin, data1))
    {
        // Повертаємо колір
        cout << reset_color;
        if (eva(data1))
        {
            // Виводимо останій вираз який був посчитан, але не був збережений в змінній
            cout << data1 << " == " << values_stack.back() << endl;
        }

        cout << user_input;
    }

    // Повертаємо колір
    cout << reset_color;

    return 0;
}

Token scanner()
{
    const Token *tp;

    tp = tokens.data() + tindex++;

    // if (tindex >= tokens.size())
    // {
    //     tindex = tokens.size() - 1;
    // }

    return *tp;
}

void lexus(const string &expr)
{
    string number;
    string name;

    char c{};

    stringstream strm(expr);

    while (strm.get(c))
    {
        Token token;

        if (isalpha(c))
        {
            name.append(1, c);

            while (strm.get(c))
            {
                if (isalnum(c))
                {
                    name.append(1, c);
                }
                else
                {
                    token.type = ID;
                    token.name = name;
                    tokens.push_back(token);

                    if (variables.find(name) == end(variables))
                    {
                        variables[name] = 0;
                    }

                    name.clear();
                    strm.unget();
                    break;
                }
            }
        }
        else if (isdigit(c))
        {
            number.append(1, c);

            while (strm.get(c))
            {
                if (isdigit(c))
                {
                    number.append(1, c);
                }
                else
                {
                    token.type = NUMBER;
                    token.value = atol(number.c_str());
                    tokens.push_back(token);

                    number.clear();
                    strm.unget();
                    break;
                }
            }
        }
        else if (isspace(c))
        {
            continue;
        }
        else
        {
            switch (c)
            {
            case '+':
                token.type = PLUS;
                tokens.push_back(token);
                break;
            case '-':
                token.type = MINUS;
                tokens.push_back(token);
                break;
            case '*':
                token.type = MULTIPLY;
                tokens.push_back(token);
                break;
            case '/':
                token.type = DIVIDE;
                tokens.push_back(token);
                break;
            case '%':
                token.type = MODULE;
                tokens.push_back(token);
                break;
            // case '(':
            //     token.type = LPAREN;
            //     tokens.push_back(token);
            //     break;
            // case ')':
            //     token.type = RPAREN;
            //     tokens.push_back(token);
            //     break;
            case '=':
                token.type = ASSIGN;
                tokens.push_back(token);
                break;
            case ';':
                token.type = SEMICOLON;
                tokens.push_back(token);
                break;
            default:
                report_error();
            }
        }
    }

    if (!number.empty())
    {
        Token token;

        token.type = NUMBER;
        token.value = atol(number.c_str());
        tokens.push_back(token);

        number.clear();
    }

    if (!name.empty())
    {
        Token token;

        token.type = ID;
        token.name = name;
        tokens.push_back(token);

        name.clear();
    }

    // Щоб було як у Пітоні - останній ';' можна не писати
    if (tokens.back().type != SEMICOLON)
    {
        Token token;

        token.type = SEMICOLON;
        tokens.push_back(token);
    }
}

bool match(TokenType tt)
{
    if (lookahead.type != tt)
    {
        report_error();
    }

    lookahead = scanner();

    return true;
}

void prog()
{
    do
    {
        statement();
    } while (tindex < tokens.size() && match(SEMICOLON));
}

void statement()
{
    // Не кожен може подивитися у майбутне, перевіряємо, чи є тут "присваювання"
    string var_name;
    bool is_assign = lookahead.type == ID && (tindex < tokens.size() && tokens[tindex].type == ASSIGN);

    if (is_assign)
    {
        // Assignment
        var_name = lookahead.name;
        match(ID);
        match(ASSIGN);
    }

    expression();

    if (is_assign)
    {
        variables[var_name] = values_stack.back();

        cout << "  " << values_stack.back() << " --> " << var_name << "\n";

        values_stack.pop_back();
    }
}

void expression()
{
    term();

    while (lookahead.type == NUMBER || lookahead.type == ID)
    {
        term();
    }

    while (lookahead.type == PLUS || lookahead.type == MINUS)
    {
        auto tt = lookahead.type;
        match(lookahead.type);

        cout << TokenString(tt) << '\n';

        if (tt == PLUS)
        {
            long arg2 = values_stack.back();
            values_stack.pop_back();
            long arg1 = values_stack.back();
            values_stack.pop_back();
            values_stack.push_back(arg1 + arg2);
            cout << "  --> " << values_stack.back() << "\n";
        }
        else if (tt == MINUS)
        {
            long arg2 = values_stack.back();
            values_stack.pop_back();
            long arg1 = values_stack.back();
            values_stack.pop_back();
            values_stack.push_back(arg1 - arg2);
            cout << "  --> " << values_stack.back() << "\n";
        }
    }
}

void term()
{
    factor();

    while (lookahead.type == NUMBER || lookahead.type == ID)
    {
        factor();
    }

    while (lookahead.type == MULTIPLY || lookahead.type == DIVIDE || lookahead.type == MODULE)
    {
        auto tt = lookahead.type;
        match(lookahead.type);

        cout << TokenString(tt) << '\n';

        if (tt == MULTIPLY)
        {
            long arg2 = values_stack.back();
            values_stack.pop_back();
            long arg1 = values_stack.back();
            values_stack.pop_back();
            values_stack.push_back(arg1 * arg2);
            cout << "  --> " << values_stack.back() << "\n";
        }
        else if (tt == DIVIDE)
        {
            long arg2 = values_stack.back();
            values_stack.pop_back();
            long arg1 = values_stack.back();
            values_stack.pop_back();
            values_stack.push_back(arg1 / arg2);
            cout << "  --> " << values_stack.back() << "\n";
        }
        else if (tt == MODULE)
        {
            long arg2 = values_stack.back();
            values_stack.pop_back();
            long arg1 = values_stack.back();
            values_stack.pop_back();
            values_stack.push_back(arg1 % arg2);
            cout << "  --> " << values_stack.back() << "\n";
        }
    }
}

void factor()
{
    switch (lookahead.type)
    {
    // case PLUS:
    //     {
    //         match(PLUS);
    //         factor();
    //         cout << "  --> " << values_stack.back() << "\n";
    //     }
    //     break;
    // case MINUS:
    //     {
    //         match(MINUS);
    //         factor();
    //         long arg = values_stack.back();
    //         values_stack.pop_back();
    //         values_stack.push_back(-arg);
    //         cout << "  --> " << values_stack.back() << "\n";
    //     }
    //     break;
    case NUMBER:
        {
            auto value = lookahead.value;
            match(NUMBER);
            values_stack.push_back(value);
            cout << value << '\n';
        }
        break;
    // case LPAREN:
    //     match(LPAREN);
    //     expression();
    //     match(RPAREN);
    //     break;
    // case ID:
    //     {
    //         auto name = lookahead.name;
    //         match(ID);
    //         values_stack.push_back(variables[name]);
    //         cout << "  " << name << " --> " << values_stack.back() << "\n";
    //     }
    //     break;
    default:
        report_error();
    }
}

bool eva(const string &expr)
{
    tindex = 0;
    values_stack.clear();
    tokens.clear();

    lexus(expr);

    lookahead = scanner();

    prog();

    return !values_stack.empty();
}
