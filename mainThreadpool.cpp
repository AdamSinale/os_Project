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
#include "Threadpool.hpp"

#define PORT "9034" // Port we're listening on

#define invalid 0
#define valid 1

using std::istringstream;
using std::list;
using std::string;
using std::cerr;
using std::cin;
using std::cout;
using std::endl;
using std::vector;
using std::shared_ptr;
using std::make_shared;

// Function declarations
void handleClient(int fd);
void acceptConnection(int listener);
vector<string> split(const string &s, char delimiter);
int checkValid(int n, int m);
int get_listener_socket();
void *get_in_addr(struct sockaddr *sa);


ThreadPool threadPool(4); // Create a thread pool with 4 threads
std::mutex graphMutex;

void* reactor; // Reactor instance
Graph graph; // Graph instance
vector<shared_ptr<Vertex>> vertices;  // Use shared_ptr to manage Vertex objects
shared_ptr<MSTSolver> algo; // MST algorithm instance
shared_ptr<Tree> mst; // MST tree instance
int newfd;

void serverSend(string message){
    int saved_stdout = dup(STDOUT_FILENO); // save stdout
    dup2(newfd, STDOUT_FILENO);            // redirect stdout to client
    cout << message << endl;               // send message
    dup2(saved_stdout, STDOUT_FILENO);     // restore stdout
}
int checkValid(int n, int m) {
    if (n <= 0 || m < 0) { return invalid; }  // Should be 1+ vertices and 0+ edges
    if (m > n * (n - 1)) { return invalid; }  // m should be less than n*(n-1)
    return valid;
}

void acceptConnection(int listener) {
    struct sockaddr_storage remoteaddr; // Client address
    socklen_t addrlen = sizeof remoteaddr;
    newfd = accept(listener, (struct sockaddr *)&remoteaddr, &addrlen);
    if (newfd == -1) {
        perror("accept");
        return;
    }

    addFdToReactor(reactor, newfd, handleClient);

    int saved_stdout = dup(STDOUT_FILENO); // save stdout
    dup2(newfd, STDOUT_FILENO); // redirect stdout to client
    cout << "Welcome to the server!" << endl;
    cout << "Here is the commanads:" << endl;
    cout << "Newgraph n,m: Creates a new undirected weighted graph with n vertices and m edges." << endl;
    cout << "Newedge u,v,w: Creates a new edge between vertex u and v of weight w" << endl;
    cout << "Removeedge u,v: Removes an edge between vertex u and v" << endl;
    cout << "--------------------------------------" << endl;
    cout << "-=+ Choose a MST algorithm +=-" << endl;
    cout << "p: Prim's Algorithm" << endl;
    cout << "t: Tarjan's Algorithm" << endl;
    cout << "k: Kruskal's Algorithm" << endl;
    cout << "b: Boruvka's Algorithm" << endl;
    cout << "i: IntegerMST's Algorithm" << endl;
    cout << "--------------------------------------" << endl;
    cout << "-=+ Some data about the MST +=-" << endl;
    cout << "printGraph: Prints the graph" << endl;
    cout << "printMST: Prints the MST" << endl;
    cout << "printWeight: Prints the weight of the MST" << endl;
    cout << "maxDistance: Prints the maximum distance between any two vertices" << endl;
    cout << "avgDistance: Prints the average distance between any two vertices" << endl;
    cout << "shortestPath s,t: Prints the shortest path between vertex s and vertex t with an edge in the MST" << endl;
    cout << "--------------------------------------" << endl;
    cout << "Don't forget- first you create a Graph, then you choose a MST algorithm and then you can collect some data on the MST." << endl;
    dup2(saved_stdout, STDOUT_FILENO); // restore stdout

    char remoteIP[INET6_ADDRSTRLEN];
    cout << "pollserver: new connection from "
         << inet_ntop(remoteaddr.ss_family, get_in_addr((struct sockaddr *)&remoteaddr), remoteIP, INET6_ADDRSTRLEN)
         << " on socket " << newfd << endl;
}
void *get_in_addr(struct sockaddr *sa) {
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

void handleClient(int fd) {
    threadPool.enqueueTask([fd]() {
        std::lock_guard<std::mutex> lock(graphMutex);

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

            int saved_stdout = dup(STDOUT_FILENO); // save stdout
            dup2(fd, STDOUT_FILENO); // redirect stdout to client

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

            vertices.clear(); // Clear the vertices vector
            for (int i = 0; i < n; ++i) {
                vertices.push_back(make_shared<Vertex>(i)); // Create new Vertex using shared_ptr
            }

            graph = Graph(vertices); // Pass vector of shared_ptr to Graph

            cout << "A graph of " << n << " vertices." << endl;
            cout << "Add " << m << " edges:" << endl;

            for (int i = 0; i < m; i++) { // input edges
                int u, v, w;
                cin >> u >> v;
                u--;
                v--;
                if (u < 0 || u >= n || v < 0 || v >= n) { // vertex should be in range
                    cout << "The input should be between 1-" <<n << endl;
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
                cout << "Give a weight for the edge:" << endl;
                cin >> w;
                graph.addEdge(vertices[v], vertices[u], w);
            }
            cout << "Graph created" << endl;
            dup2(saved_stdin, STDIN_FILENO); // restore stdin
            dup2(saved_stdout, STDOUT_FILENO); // restore stdout
            
        } else if (choice.find("Newedge") != string::npos) { // Add weighted edge to the graph from client input
            int n = graph.numVertices();
            vector<string> last = split(choice, ',');
            int w = stoi(last[2]);
            int u = stoi(last[1]) -1;
            vector<string> first = split(last[0], ' ');
            int v = stoi(first[1]) -1;

            if(u<0 || u>=n || v<0 || v>=n){ serverSend("Input between 1-n"); return; }              // vertex should be in range
            if(u == v){ serverSend("No inside edge"); return; }                                     // self loop
            if(vertices[v]->hasNeighbor(vertices[u])){ serverSend("Edge already exists"); return; } // multiple edges
            serverSend("Edge added");
            graph.addEdge(vertices[v], vertices[u], w);

        } else if (choice.find("Removeedge") != string::npos) { // Remove edge from the graph from client input
            int n = graph.numVertices();
            vector<string> last = split(choice, ',');
            int u = stoi(last[1]) - 1;
            vector<string> first = split(last[0], ' ');
            int v = stoi(first[1]) - 1;
            if(!graph.hasEdge(v,u)){ serverSend("no edge to remove"); return; }
            if(u<0 || u>=n || v<0 || v>=n){ serverSend("Input between 1-n"); return; }  // vertex should be in range
            serverSend("Edge removed");
            graph.removeEdge(graph.getVertex(v),graph.getVertex(u)); // Remove edge

        } else if (choice.find("printGraph") != string::npos) {
            int saved_stdout = dup(STDOUT_FILENO); // save stdout
            dup2(fd, STDOUT_FILENO); // redirect stdout to client
            graph.printGraph();
            dup2(saved_stdout, STDOUT_FILENO); // restore stdout
        }
        else if (choice.find("printWeight") != string::npos) {
            if(mst == nullptr){
                serverSend("You didn't give an algorithm - prim as default:");
                algo = MSTFactory::MST('p');  // Create Prim's MST solver
                mst = algo->findMST(graph);   // Find MST for the current graph
            }
            
            int saved_stdout = dup(STDOUT_FILENO); // save stdout
            dup2(fd, STDOUT_FILENO); // redirect stdout to client
            mst->printWeight();
            dup2(saved_stdout, STDOUT_FILENO); // restore stdout

        }
        else if (choice.find("maxDistance") != string::npos) {
            if(mst == nullptr){
                serverSend("You didn't give an algorithm - prim as default:");
                algo = MSTFactory::MST('p');  // Create Prim's MST solver
                mst = algo->findMST(graph);   // Find MST for the current graph
            }

            int saved_stdout = dup(STDOUT_FILENO); // save stdout
            dup2(fd, STDOUT_FILENO); // redirect stdout to client
            mst->maxDistance();
            dup2(saved_stdout, STDOUT_FILENO); // restore stdout

        }
        else if (choice.find("avgDistance") != string::npos) {
            if(mst == nullptr){
                serverSend("You didn't give an algorithm - prim as default:");
                algo = MSTFactory::MST('p');  // Create Prim's MST solver
                mst = algo->findMST(graph);   // Find MST for the current graph
            }

            int saved_stdout = dup(STDOUT_FILENO); // save stdout
            dup2(fd, STDOUT_FILENO); // redirect stdout to client
            mst->avgDistance();
            dup2(saved_stdout, STDOUT_FILENO); // restore stdout

        }
        else if (choice.find("shortestPath") != string::npos) {
            vector<string> last = split(choice, ',');
            int t = stoi(last[1]);
            vector<string> first = split(last[0], ' ');
            int s = stoi(first[1]);
            if(mst == nullptr){
                serverSend("You didn't give an algorithm - prim as default:");
                algo = MSTFactory::MST('p');  // Create Prim's MST solver
                mst = algo->findMST(graph);   // Find MST for the current graph
            }

            int saved_stdout = dup(STDOUT_FILENO); // save stdout
            dup2(fd, STDOUT_FILENO); // redirect stdout to client
            mst->shortestPath(s, t, mst);
            dup2(saved_stdout, STDOUT_FILENO); // restore stdout
        } 
        
        else if (choice.find("printMST") != string::npos) {
            if(mst == nullptr){
                serverSend("You didn't give an algorithm - prim as default:");
                algo = MSTFactory::MST('p');  // Create Prim's MST solver
                mst = algo->findMST(graph);   // Find MST for the current graph
            }
            int saved_stdout = dup(STDOUT_FILENO); // save stdout
            dup2(fd, STDOUT_FILENO); // redirect stdout to client
            mst->printGraph();
            dup2(saved_stdout, STDOUT_FILENO); // restore stdout

        }
        // Algorithm options.
        else if (choice.find("p") != string::npos) { // Prim's Algorithm
            int saved_stdout = dup(STDOUT_FILENO);
            dup2(fd, STDOUT_FILENO);
            algo = MSTFactory::MST('p');  // Create Prim's MST solver
            mst = algo->findMST(graph);   // Find MST for the current graph
            dup2(saved_stdout, STDOUT_FILENO); // Restore stdout
        }
        // Tarjan's Algorithm using thread pool
        else if (choice.find("t") != string::npos) { // Tarjan's Algorithm
            int saved_stdout = dup(STDOUT_FILENO);
            dup2(fd, STDOUT_FILENO);
            algo = MSTFactory::MST('t');  // Create Tarjan's MST solver
            mst = algo->findMST(graph);   // Find MST for the current graph
            dup2(saved_stdout, STDOUT_FILENO);
        }
        // Kruskal's Algorithm using thread pool
        else if (choice.find("k") != string::npos) { // Kruskal's Algorithm
            int saved_stdout = dup(STDOUT_FILENO);
            dup2(fd, STDOUT_FILENO);
            algo = MSTFactory::MST('k');  // Create Kruskal's MST solver
            mst = algo->findMST(graph);   // Find MST for the current graph
            dup2(saved_stdout, STDOUT_FILENO);
        }
        // Borůvka's Algorithm using thread pool
        else if (choice.find("b") != string::npos) { // Borůvka's Algorithm
            int saved_stdout = dup(STDOUT_FILENO);
            dup2(fd, STDOUT_FILENO);
            algo = MSTFactory::MST('b');  // Create Borůvka's MST solver
            mst = algo->findMST(graph);   // Find MST for the current graph
            dup2(saved_stdout, STDOUT_FILENO);
        }
        // Integer MST Algorithm using thread pool
        else if (choice.find("i") != string::npos) { // Integer MST Algorithm
            int saved_stdout = dup(STDOUT_FILENO);
            dup2(fd, STDOUT_FILENO);
            algo = MSTFactory::MST('i');  // Create Integer MST solver
            mst = algo->findMST(graph);   // Find MST for the current graph
            dup2(saved_stdout, STDOUT_FILENO);
        }
        else {  // The client input is invalid
            cout << "Invalid command, closing the connection with the socket." << endl;
            close(fd);
            removeFdFromReactor(reactor, fd);
        }
    });
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

    if(p == NULL){ return -1; }

    if(listen(listener, 10) == -1){ return -1; }

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

int main() {

    // Create a listener socket
    int listener = get_listener_socket();
    if (listener == -1) {
        cerr << "Error getting listening socket" << endl;
        return 1;
    }

    cout << "In order to connect, write: nc 127.0.0.1 9034 in another terminal" << endl;

    reactor = startReactor();
    addFdToReactor(reactor, listener, acceptConnection);
    runReactor(reactor);
    stopReactor(reactor);
    delete static_cast<Reactor*>(reactor);

    return 0;
}
