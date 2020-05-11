#ifndef TKOM_TRACER
#define TKOM_TRACER

#include <string>

class Tracer
{
public:
    Tracer(const bool& enabled = false);

    void reset();

    void enter(const std::string& message = "");
    void leave(const std::string& message = "leave");
    void info(const std::string& message);

private:
    const bool enabled;
    unsigned int nestingLevel = 0;

    void printNesting();
};


#endif
