#ifndef SYSTEM_H
#define SYSTEM_H

class System {
public:
	virtual void step(unsigned int dt) = 0;
};

#endif
