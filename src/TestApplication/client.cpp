///////////////////////////////////////////////////////////////////
//: Title        : client.cpp
//: Product:     : Simple client example
//: Date         : 2022-07-30
//: Author       : "Kjetil Kristoffer Solberg" <post@ikjetil.no>
//: Version      : 1.0.0.0
//: Descriptions : Implementation of client simple example.
//
// #include
//
#include <iostream>
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
using ItSoftware::Linux::IPC::ItsSocketDatagramClient;

//
// constexpr
//
constexpr auto MAX_BUF_SIZE = 4096;

//
// Function: main
//
// (i): Application entry point
//
int main(int argc, char** argv)
{
    struct sockaddr_in addr_server{0};
    struct sockaddr_in addr_client{0};
    auto addr_host_server = ItsSocket::CreateSockAddrHostInet4(5500, "192.168.0.103");
    addr_server = *addr_host_server;
    
    auto addr_host_client = ItsSocket::CreateSockAddrHostInet4(5501, "192.168.0.100");
    addr_client = *addr_host_client;

    auto client = make_unique<ItsSocketDatagramClient>(ItsSocketDomain::INET, (struct sockaddr*)&addr_client, sizeof(addr_client));
    if ( client->GetInitWithError()) {
        cout << "Client, Init with error: " << strerror(client->GetInitWithErrorErrno()) << endl;
        return EXIT_FAILURE;
    }
    else {
        cout << "Client, Init Ok!" << endl;
    }

    char buf[MAX_BUF_SIZE] = "";
    while (strcmp(buf, "quit") != 0) {
        cout << endl;
        cout << "(enter message <quit> to exit both client and server)" << endl;
        cout << "Message: ";
        cin.getline(buf,MAX_BUF_SIZE);

        ssize_t nSent = client->SendTo(buf, strlen(buf)+1, 0, (struct sockaddr*)&addr_server, sizeof(addr_server));
        cout << "> " << nSent << " bytes sent <" << endl;
    }

    cout << endl << "exiting client" << endl;

    return EXIT_SUCCESS;
}