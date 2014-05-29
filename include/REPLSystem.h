#include <queue>
#include <string>
#include <map>
#include <functional>
#include <poll.h>
#include "Engine/System.h"
#include "Engine/EntityManager.h"

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
	REPLSystem();
	virtual ~REPLSystem();
	void step(unsigned int dt, EntityManager* em);

private:
	/* Check if stdin is readable, and if so, read it and tokenize the input */
	void tokenize_input();

	/* Split a string at its spaces and insert each token into _messages */
	void tokenize_into_queue(string msg);

	/* Pop the head off _messages */
	string pop_queue();

	/* Execute a command according to the functions specified in _functions */
	void parse(string cmd, EntityManager *em);

	/* array to hold the file descriptors we want to poll */
	struct pollfd poll_fds[1];

	/* Queue holding the tokenized input.
	 * e.g. an input of "foo bar baz" will push "foo", "bar" and "baz" into the
	 * queue. */
	queue<string> _messages;

	/* A mapping of commands to functions.
	 * Commands that take arguments will pop them off the queue themselves. 
	 * Each function returns a string that gets printed to stdout. */
	map<string, function<string(EntityManager *em)> > _functions;
};
