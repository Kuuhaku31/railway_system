
// controler.cpp

#include "controler.h"

Controler* Controler::instance = nullptr;
Controler&
Controler::Instance()
{
    if(instance == nullptr) instance = new Controler();
    return *instance;
}
