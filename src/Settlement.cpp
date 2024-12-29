#include "Settlement.h"
#include <iostream>


Settlement::Settlement(const string &name, SettlementType type) : name(name) , type(type)
{}

const string &Settlement::getName() const
{
    return name;
}

SettlementType Settlement::getType() const
{
    return type;
}

const string Settlement::toString() const
{
    string typeToStr[3] = {"Village", "City", "Metropolin"};
    return "Name: " + getName() + " Type: " + typeToStr[int(type)];
   // return std::string "Name: " + getName() + "Type:" + getType();
}

