#ifndef REACTOR_H
#define REACTOR_H

#include <vector>
#include <unordered_map>
#include <poll.h>

// Typedef for the reactor function pointer
typedef void (*reactorFunc)(int fd);

// Reactor struct definition
struct Reactor {
    std::vector<struct pollfd> pfds;
    std::unordered_map<int, reactorFunc> callbacks;
    bool running;
};

void *startReactor();
int addFdToReactor(void *reactor, int fd, reactorFunc func);
int removeFdFromReactor(void *reactor, int fd);
int stopReactor(void *reactor);
void runReactor(void *reactor);

#endif // REACTOR_H