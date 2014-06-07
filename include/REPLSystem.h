#include <queue>
#include <string>
#include <map>
#include <functional>
#include <poll.h>
#include <lua.hpp>
#include "Engine/System.h"
#include "Engine/EntityManager.h"
#include "Engine/ComponentManager.h"

using namespace std;

/* A system for interacting with a running game through stdin.
 * Available commands:
 * print n			prints information about entity n
 * remove n			remove entity n
 * rmcomp n addr	remove the component at address addr from entity n
 */
class REPLSystem : public System
{
public:
	REPLSystem(EntityManager *em, ComponentManager *cm);
	virtual ~REPLSystem();
	void step(unsigned int dt, EntityManager* em);

private:
	/* Check if stdin is readable, and if so, read it and return the input */
	vector<string> get_input();

	/* array to hold the file descriptors we want to poll */
	struct pollfd poll_fds[1];

	lua_State *_L;
};
