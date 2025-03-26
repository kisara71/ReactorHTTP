// #ifndef SERVER_H
// #define SERVER_H
// #include <arpa/inet.h>
// #include <sys/epoll.h>
// #include "../HTTPRequest/HTTPRequest.h"
// #include <thread>
// #include <atomic>
// class Server{
// public:
//     Server();
//     ~Server();
//     void initServer(uint16_t port);
//     std::thread test;

// private:
//     bool initListenFd(uint16_t port);
//     bool acceptClient();
//     void epollRun();
// private:
//     int m_lfd;
//     int m_epfd;
//     std::atomic<bool> m_stop;
//     HTTPRequest http;

// };


// #endif