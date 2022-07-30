///////////////////////////////////////////////////////////////////
//: Title        : server.cpp
//: Product:     : Simple server example
//: Date         : 2022-07-30
//: Author       : "Kjetil Kristoffer Solberg" <post@ikjetil.no>
//: Version      : 1.0.0.0
//: Descriptions : Implementation of server simple example.
//
// #include
//
#include <iostream>
#include <chrono>
#include "../include/itsoftware-linux-core.h"
#include "../include/itsoftware-linux-ipc.h"
#include "../include/itsoftware-linux.h"

//
// using
//
using std::cout;
using std::cin;
using std::endl;
using ItSoftware::Linux::IPC::ItsSocket;
using ItSoftware::Linux::IPC::ItsSocketDomain;
using ItSoftware::Linux::IPC::ItsSocketDatagramServer;

//
// constexpr
//
constexpr auto MAX_BUF_SIZE = 4096;

//
// Function: main
//
// (i): Application entry point.
//
int main(int argc, char** argv)
{
    struct sockaddr_in addr_server{0};
    auto addr_host_server = ItsSocket::CreateSockAddrHostInet4(5500, "192.168.0.103");
    addr_server = *addr_host_server;

    auto server = make_unique<ItsSocketDatagramServer>(ItsSocketDomain::INET, (struct sockaddr*)&addr_server, sizeof(addr_server));
    if ( server->GetInitWithError()) {
        cout << "Server, Init with error: " << strerror(server->GetInitWithErrorErrno()) << endl;
    }
    else {
        cout << "Server, Init Ok!" << endl;
    }

    char buf[MAX_BUF_SIZE] = "";
    while (strcmp(buf, "quit") != 0) {
        cout << endl;

        struct sockaddr_in addr;
        socklen_t addr_length;

        strcpy(buf, "");

        cout << "Awaiting data ..." << endl;
        ssize_t nRecv = 0;
        nRecv = server->RecvFrom(buf, MAX_BUF_SIZE, 0, NULL, 0);
        while ( nRecv == 0 || (nRecv == -1 && errno == EAGAIN) ) {
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
            nRecv = server->RecvFrom(buf, 4096, 0, (struct sockaddr*)&addr, &addr_length);
        }
        
        if ( nRecv == -1 ) {
            cout << "> Server error: " << strerror(errno) << endl;
            return EXIT_FAILURE;
        }

        cout << "> " << nRecv << " bytes received <" << endl;
        cout << "> " << buf << " <" << endl;
    }

    cout << endl << "exiting server" << endl;
    
    return EXIT_SUCCESS;
}