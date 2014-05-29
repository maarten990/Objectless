#include <iostream>
#include <sstream>
#include <string>
#include <algorithm>
#include <poll.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include "REPLSystem.h"

using namespace std;

REPLSystem::REPLSystem()
	: poll_fds { {.fd = STDIN_FILENO, .events = POLLIN} }
{
	_functions["print"] = [this](EntityManager *em) {
		int entity = atoi(pop_queue().c_str());
		return em->pretty(entity);
	};

	_functions["remove"] = [this](EntityManager *em) {
		string arg = pop_queue();
		int entity = atoi(arg.c_str());
		stringstream output;

		em->remove(entity);
		output << "Removed entity " << entity;

		return output.str();
	};

	_functions["rmcomp"] = [this](EntityManager *em) {
		string entstr = pop_queue();
		string compstr = pop_queue();
		int entity;
		Component *component;
		char output[64];

		entity = atoi(entstr.c_str());
		sscanf(compstr.c_str(), "%lx", &component);

		em->remove_component(entity, (Component *)component);
		sprintf(output, "Removed component %#lx from entity %d\n", component,
				entity);

		return string(output, find(output, output + 64, '\0'));
	};
}

REPLSystem::~REPLSystem()
{
}

void REPLSystem::step(unsigned int dt, EntityManager *em)
{
	tokenize_input();

	while (!_messages.empty()) {
		string cmd = pop_queue();
		parse(cmd, em);
	}
}

void REPLSystem::tokenize_input()
{
	char cmd[256];

	if (poll(poll_fds, 1, 0) > 0 && (poll_fds[0].revents & POLLIN)) {
		while (fgets(cmd, 256, stdin) != 0)
			tokenize_into_queue(string(cmd));
	}
}

void REPLSystem::tokenize_into_queue(string msg)
{
	char cstr[msg.length() + 1];
	char delim[] = " ";
	strcpy(cstr, msg.c_str());

	char *tokens = strtok(cstr, delim);

	while (tokens) {
		_messages.push(string(tokens));
		tokens = strtok(NULL, " ");
	}
}

string REPLSystem::pop_queue()
{
	string str = _messages.front();

	_messages.pop();
	return str;
}

void REPLSystem::parse(string cmd, EntityManager *em)
{
	auto func_pair = _functions.find(cmd);
	if (func_pair != _functions.end()) {
		cout << func_pair->second(em) << endl;
	} else {
		cout << "Error: invalid function: " << cmd << endl;
	}
}
