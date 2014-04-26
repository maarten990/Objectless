#ifndef COMPONENT_H
#define COMPONENT_H

#include <map>
#include <string>
#include <functional>
#include <typeindex>
#include <typeinfo>

using namespace std;

class Component
{
public:
    static map<int, function<Component*(void*)> > factory;
    type_index get_id() {
        return typeid(*this);
    }
    virtual ~Component() {}
};

#endif
