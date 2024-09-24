#include "graph.hpp"
#include "MSTFactory.hpp"
#include <vector>
#include <iostream>
#include <memory> // For std::shared_ptr
#include <list>
#include <string>
#include <sstream>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "reactor.hpp"

#define PORT "9034" // Port we're listening on

#define invalid 0
#define valid 1

using std::list;
using std::string;
using std::cerr;
using std::cin;
using std::cout;
using std::endl;
using std::vector;
using std::shared_ptr;
using std::make_shared;

int checkValid(int n, int m) {
    if (n <= 0 || m < 0) { return invalid; }  // Should be 1+ vertices and 0+ edges
    if (m > n * (n - 1)) { return invalid; }  // m should be less than n*(n-1)
    return valid;
}

void acceptConnection(int listener) {
    struct sockaddr_storage remoteaddr; // Client address
    socklen_t addrlen = sizeof remoteaddr;
    int newfd = accept(listener, (struct sockaddr *)&remoteaddr, &addrlen);
    if (newfd == -1) {
        perror("accept");
        return;
    }

    addFdToReactor(reactor, newfd, handleClient);

    int saved_stdout = dup(STDOUT_FILENO); // save stdout
    dup2(newfd, STDOUT_FILENO); // redirect stdout to client
    cout << "Welcome to the server!" << endl;
    cout << "Here is the commanads:" << endl;
    cout << "Newgraph n m: Create a new undirected weighted graph with n vertices and m edges." << endl;
    cout << "Newedge u v: Create a new edge" << endl;
    cout << "Removeedge u v: Remove an edge" << endl;
    cout << "--------------------------------------" << endl;
    cout << "-=+ Choosing a MST algorithm +=-" << endl;
    cout << "p: Prim's Algorithm" << endl;
    cout << "t: Tarjan's Algorithm" << endl;
    cout << "k: Kruskal's Algorithm" << endl;
    cout << "b: Boruvka's Algorithm" << endl;
    cout << "i: IntegerMST's Algorithm" << endl;
    cout << "--------------------------------------" << endl;
    cout << "-=+ Some data about the MST +=-" << endl;
    cout << "printGraph: Print the graph" << endl;
    cout << "printWeight: Print the weight of the graph" << endl;
    cout << "maxDistance: Print the maximum distance between any two vertices" << endl;
    cout << "avgDistance: Print the average distance between any two vertices" << endl;
    cout << "shortestPath s t: Print the shortest path between s and t" << endl;
    cout << "--------------------------------------" << endl;
    cout << "Don't forget- first you create a Graph, then you are choosing a MST algorithm and then you can collect some data on the MST." << endl;

    dup2(saved_stdout, STDOUT_FILENO); // restore stdout

    char remoteIP[INET6_ADDRSTRLEN];
    cout << "pollserver: new connection from "
         << inet_ntop(remoteaddr.ss_family,
                      get_in_addr((struct sockaddr *)&remoteaddr),
                      remoteIP, INET6_ADDRSTRLEN)
         << " on socket " << newfd << endl;
}

void *get_in_addr(struct sockaddr *sa) {
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

void handleClient(int fd) {
    char buf[256];
    int nbytes = recv(fd, buf, sizeof(buf), 0);
    if (nbytes <= 0) {
        if (nbytes == 0) {
            cout << "Connection closed on fd " << fd << endl;
        } else {
            perror("recv");
        }
        close(fd);
        removeFdFromReactor(reactor, fd);
        return;
    }

    buf[nbytes] = '\0';
    string choice = buf;
    cout << "Received: " << choice << endl;

    if (choice.find("Newgraph") != string::npos) { // Creating a new undirected weighted graph.
        vector<string> last = split(choice, ',');
        int m = stoi(last[1]);
        vector<string> first = split(last[0], ' ');
        int n = stoi(first[1]);
        if (checkValid(n, m) == invalid) {
            cout << "Invalid input" << endl;
            return;
        }
        int saved_stdin = dup(STDIN_FILENO); // save stdin
        dup2(fd, STDIN_FILENO); // redirect stdin to client

        for (int i = 0; i < n; ++i) {
            vertices.push_back(make_shared<Vertex>(i)); // Create new Vertex using shared_ptr
        }
        Graph graph(vertices);  // Pass vector of shared_ptr to Graph

        cout << "A graph of " << n << " vertices." << endl;
        cout << "Add " << m << " edges:" << endl;

        for (int i = 0; i < m; i++) { // input edges
            int u, v, w;
            cin >> u >> v;
            u--;
            v--;
            if (u < 0 || u >= n || v < 0 || v >= n) { // vertex should be in range
                cout << "Input between 1-n" << endl;
                i--;
                continue;
            }
            if (u == v) { // self loop
                cout << "No inside edge" << endl;
                i--;
                continue;
            }
            if (vertices[v]->hasNeighbor(vertices[u])) { // multiple edges
                cout << "Edge already exists" << endl;
                i--;
                continue;
            }
            cout << "Weight:" << endl;
            cin >> w;
            graph.addEdge(vertices[v], vertices[u], w);
        }
        dup2(saved_stdin, STDIN_FILENO); // restore stdin
        
    } else if (choice.find("p") != string::npos) { // Prim's Algorithm

        int saved_stdout = dup(STDOUT_FILENO); // save stdout
        dup2(fd, STDOUT_FILENO); // redirect stdout to client
        algo = MSTFactory::MST('p');
        mst = algo->findMST(graph);
        dup2(saved_stdout, STDOUT_FILENO); // restore stdout

    } else if (choice.find("t") != string::npos) { // Tarjan's Algorithm

        int saved_stdout = dup(STDOUT_FILENO); // save stdout
        dup2(fd, STDOUT_FILENO); // redirect stdout to client
        algo = MSTFactory::MST('t');
        mst = algo->findMST(graph);
        dup2(saved_stdout, STDOUT_FILENO); // restore stdout

    } else if (choice.find("k") != string::npos) { // Kruskal's Algorithm

        int saved_stdout = dup(STDOUT_FILENO); // save stdout
        dup2(fd, STDOUT_FILENO); // redirect stdout to client
        algo = MSTFactory::MST('k');
        mst = algo->findMST(graph);
        dup2(saved_stdout, STDOUT_FILENO); // restore stdout

    } else if (choice.find("b") != string::npos) { // Boruvka's Algorithm

        int saved_stdout = dup(STDOUT_FILENO); // save stdout
        dup2(fd, STDOUT_FILENO); // redirect stdout to client
        algo = MSTFactory::MST('b');
        mst = algo->findMST(graph);
        dup2(saved_stdout, STDOUT_FILENO); // restore stdout

    } else if (choice.find("i") != string::npos) { // IntegerMST's Algorithm

        int saved_stdout = dup(STDOUT_FILENO); // save stdout
        dup2(fd, STDOUT_FILENO); // redirect stdout to client
        algo = MSTFactory::MST('i');
        mst = algo->findMST(graph);
        dup2(saved_stdout, STDOUT_FILENO); // restore stdout

    } else if (choice.find("Newedge") != string::npos) { // Add weighted edge to the graph from client input

        int n = graph.numVertices();
        vector<string> last = split(choice, ',');
        int u = stoi(last[1]) -1;
        vector<string> first = split(last[0], ' ');
        int v = stoi(first[1]) -1;

        if (u < 0 || u >= n || v < 0 || v >= n) { // vertex should be in range
            cout << "Input between 1-n" << endl;
            return;
        }
        if(u == v) { // self loop
            cout << "No inside edge" << endl;
            return;
        }
        if (vertices[v]->hasNeighbor(vertices[u])) { // multiple edges
            cout << "Edge already exists" << endl;
            return;
        }
        cout << "Weight:" << endl;
        int w;
        
        int saved_stdin = dup(STDIN_FILENO); // save stdin
        dup2(fd, STDIN_FILENO); // redirect stdin to client
        cin >> w;
        dup2(saved_stdin, STDIN_FILENO); // restore stdin

        graph.addEdge(vertices[v], vertices[u], w);

    } else if (choice.find("Removeedge") != string::npos) { // Remove edge from the graph from client input

        int n = graph.numVertices();
        vector<string> last = split(choice, ',');
        int u = stoi(last[1]) - 1;
        vector<string> first = split(last[0], ' ');
        int v = stoi(first[1]) - 1;
        if(u < 0 || u >= n || v < 0 || v >= n) { // vertex should be in range
            cout << "Input between 1-n" << endl;
            return;
        }
        graph.getVertex(v)->removeNeighbor(graph.getVertex(u)); // Remove edge

    }else if (choice.find("printGraph") != string::npos) {

        int saved_stdout = dup(STDOUT_FILENO); // save stdout
        dup2(fd, STDOUT_FILENO); // redirect stdout to client
        mst->printGraph();
        dup2(saved_stdout, STDOUT_FILENO); // restore stdout

    }
    else if (choice.find("printWeight") != string::npos) {
        
        int saved_stdout = dup(STDOUT_FILENO); // save stdout
        dup2(fd, STDOUT_FILENO); // redirect stdout to client
        mst->printWeight();
        dup2(saved_stdout, STDOUT_FILENO); // restore stdout

    }
    else if (choice.find("maxDistance") != string::npos) {

        int saved_stdout = dup(STDOUT_FILENO); // save stdout
        dup2(fd, STDOUT_FILENO); // redirect stdout to client
        mst->maxDistance();
        dup2(saved_stdout, STDOUT_FILENO); // restore stdout

    }
    else if (choice.find("avgDistance") != string::npos) {

        int saved_stdout = dup(STDOUT_FILENO); // save stdout
        dup2(fd, STDOUT_FILENO); // redirect stdout to client
        mst->avgDistance();
        dup2(saved_stdout, STDOUT_FILENO); // restore stdout

    }
    else if (choice.find("shortestPath") != string::npos) {

        vector<string> last = split(choice, ',');
        int t = stoi(last[2]) - 1;
        vector<string> first = split(last[0], ' ');
        int s = stoi(first[1]) - 1;

        int saved_stdout = dup(STDOUT_FILENO); // save stdout
        dup2(fd, STDOUT_FILENO); // redirect stdout to client
        mst->shortestPath(s, t, mst);
        dup2(saved_stdout, STDOUT_FILENO); // restore stdout
    }
    else { // The client input is invalid

        cout << "Invalid command, closing the connection with the socket." << endl;
        close(fd);
        removeFdFromReactor(reactor, fd);

    }
}

int get_listener_socket() {
    int listener;
    int yes = 1;
    int rv;
    struct addrinfo hints, *ai, *p;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    if ((rv = getaddrinfo(NULL, PORT, &hints, &ai)) != 0) {
        cerr << "selectserver: " << gai_strerror(rv) << endl;
        exit(1);
    }

    for (p = ai; p != NULL; p = p->ai_next) {
        listener = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (listener < 0) {
            continue;
        }
        setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
        if (bind(listener, p->ai_addr, p->ai_addrlen) < 0) {
            close(listener);
            continue;
        }
        break;
    }
    freeaddrinfo(ai);

    if (p == NULL) {
        return -1;
    }

    if (listen(listener, 10) == -1) {
        return -1;
    }

    return listener;
}

vector<string> split(const string &s, char delimiter) {
    vector<string> tokens;
    string token;
    istringstream tokenStream(s);
    while (getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

void* reactor; // Reactor instance
Graph graph; // Graph instance
vector<shared_ptr<Vertex>> vertices;  // Use shared_ptr to manage Vertex objects
shared_ptr<MSTSolver> algo; // MST algorithm instance
shared_ptr<Tree> mst; // MST tree instance

int main() {
    int listener = get_listener_socket();
    if (listener == -1) {
        cerr << "Error getting listening socket" << endl;
        return 1;
    }

    reactor = startReactor();
    addFdToReactor(reactor, listener, acceptConnection);
    runReactor(reactor);
    stopReactor(reactor);
    delete static_cast<Reactor*>(reactor);

    return 0;
}

// 6 7 1 2 2 1 3 1 2 3 3 1 4 5 1 5 3 1 6 1 5 6 1 k