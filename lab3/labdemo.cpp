#include <vector>
#include <sstream>
#include <iostream>
#include <string>

#include "labdemo.h"


using namespace std;


LabDemo::LabDemo(/* args */)
{
}

LabDemo::~LabDemo()
{
}

bool LabDemo::process_command(const string &command)
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

    if (!lab_commands(verb, args, ss))
    {
        if (verb == "quit" || verb == "die")
        {
            return false;
        }
        else
        {
            // Якщо нема команди - нічого не печатаємо
            if (!verb.empty())
            {
                cout << "Невідома команда (скористуйтеся help) " << command << endl;
            }
        }
    }

    return true;
}
