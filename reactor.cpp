#include "reactor.hpp"
#include <iostream>
#include <unistd.h>

// Starts a new reactor and returns a pointer to it
void *startReactor() {
    Reactor *reactor = new Reactor;
    reactor->running = true;
    return reactor;
}

// Adds fd to Reactor (for reading); returns 0 on success
int addFdToReactor(void *reactor_ptr, int fd, reactorFunc func) {
    Reactor *reactor = static_cast<Reactor *>(reactor_ptr);
    struct pollfd pfd;
    pfd.fd = fd;
    pfd.events = POLLIN;
    reactor->pfds.push_back(pfd);
    reactor->callbacks[fd] = func;
    return 0;
}

// Removes fd from reactor
int removeFdFromReactor(void *reactor_ptr, int fd) {
    Reactor *reactor = static_cast<Reactor *>(reactor_ptr);
    for (auto it = reactor->pfds.begin(); it != reactor->pfds.end(); ++it) {
        if (it->fd == fd) {
            reactor->pfds.erase(it);
            break;
        }
    }
    reactor->callbacks.erase(fd);
    return 0;
}

// Stops reactor
int stopReactor(void *reactor_ptr) {
    Reactor *reactor = static_cast<Reactor *>(reactor_ptr);
    reactor->running = false;
    return 0;
}

// Runs the reactor event loop
void runReactor(void *reactor_ptr) {
    Reactor *reactor = static_cast<Reactor *>(reactor_ptr);
    while (reactor->running) {
        int poll_count = poll(reactor->pfds.data(), reactor->pfds.size(), -1);
        if (poll_count == -1) {
            perror("poll");
            break;
        }

        for (size_t i = 0; i < reactor->pfds.size(); i++) {
            if (reactor->pfds[i].revents & POLLIN) {
                int fd = reactor->pfds[i].fd;
                reactorFunc func = reactor->callbacks[fd];
                if (func) {
                    func(fd);
                }
            }
        }
    }
}