#ifndef SYSTEM_H
#define SYSTEM_H

class System {
public:
	virtual ~System() {};
	virtual void step(unsigned int dt) = 0;
};

#endif
