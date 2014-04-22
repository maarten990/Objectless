#include <functional>
#include <map>
#include <typeinfo>
#include <typeindex>
#include "Component.h"

using namespace std;

class ComponentManager {
public:
	ComponentManager();
	~ComponentManager();
	void register_component(type_index type,
			function<Component*(void*)> func);
	Component *construct(type_index type, void *data);
	
private:
	map<type_index, function<Component*(void*)> > _constructors;
};
