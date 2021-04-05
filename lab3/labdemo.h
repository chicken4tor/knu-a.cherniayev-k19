#ifndef LABDEMO_H_SEEN
#define LABDEMO_H_SEEN

#include <string>
#include <vector>
#include <sstream>


class LabDemo
{
public:
    LabDemo(/* args */);
    ~LabDemo();

    bool process_command(const std::string &command);

protected:

    virtual bool lab_commands(const std::string &verb, const std::vector<std::string> &args, std::stringstream &ss) = 0;
};

#endif /* !LABDEMO_H_SEEN */
