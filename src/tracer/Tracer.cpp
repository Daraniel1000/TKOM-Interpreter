#include "Tracer.hpp"
#include <iostream>
#include <exception>

Tracer::Tracer(const bool& enabled):
enabled(enabled)
{}

void Tracer::reset()
{
    this->nestingLevel = 0;
}

void Tracer::enter(const std::string& message)
{
    if (!this->enabled)
    {
        return;
    }
    if (message.length() > 0)
    {
        this->printNesting();
        std::cout<<message<<std::endl;
    }
    this->nestingLevel++;
}

void Tracer::leave(const std::string& message)
{
    if (!this->enabled)
    {
        return;
    }
    if (this->nestingLevel == 0)
    {
        std::cout<<"Tried to leave top tracking level, leave aborted...";
        return;
    }
    this->nestingLevel--;
    if (message.length() > 0)
    {
        this->printNesting();
        std::cout<<message<<std::endl;
    }
}

void Tracer::info(const std::string& message)
{
    if (!this->enabled)
    {
        return;
    }
    this->printNesting();
    std::cout<<message<<std::endl;
}

void Tracer::printNesting()
{
    for (int i = this->nestingLevel - 1; i > 0; --i)
    {
        std::cout << "| ";
    }
    std::cout << "|-";
}
