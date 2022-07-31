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
#include <string>
#include <chrono>
#include "../include/itsoftware-linux-core.h"
#include "../include/itsoftware-linux-ipc.h"
#include "../include/itsoftware-linux.h"

//
// #define
//
// (i): Uncomment USE_STREAM to use datagram
//
//#define USE_STREAM

//
// using
//
using std::cout;
using std::cin;
using std::endl;
using std::string;
using ItSoftware::Linux::IPC::ItsSocket;
using ItSoftware::Linux::IPC::ItsSocketDomain;
using ItSoftware::Linux::IPC::ItsSocketDatagramServer;
using ItSoftware::Linux::IPC::ItsSocketStreamServer;
using ItSoftware::Linux::ItsConvert;

//
// constexpr
//
constexpr auto MAX_BUF_SIZE = 4096;

//
//
//
struct AppSettings {
    string      ServerAddress;
    uint16_t    ServerPort;
};

//
// Functions prototypes
//
void UpdateAppSettings(int argc, char** argv, AppSettings& settings);
string GetArgVal(string arg, int argc, char** argv);

//
// Function: main
//
// (i): Application entry point.
//
int main(int argc, char** argv)
{
    //
    // Application settings. Set default.
    //
    AppSettings settings {
        .ServerAddress = "192.168.0.103",
        .ServerPort = 5500
    };

    //
    // Update application settings from command line arg.
    //
    UpdateAppSettings(argc,argv,settings);

    //
    // Output app settings
    //
    cout << "##" << endl;
    cout << "## server" << endl;
    cout << "##" << endl;
    cout << "## Usage: server --server-port=<port> --server-address=<server ip address>" << endl;
    cout << "##" << endl;
    cout << "> server port    : " << settings.ServerPort << " <" << endl;
    cout << "> server address : " << settings.ServerAddress << " <" << endl;

    //
    // Create INET host address.
    //
    struct sockaddr_in addr_server{0};
    auto addr_host_server = ItsSocket::CreateSockAddrHostInet4(settings.ServerPort, settings.ServerAddress);
    addr_server = *addr_host_server;

    //
    // Make ItsSocketDatagramServer
    //
#ifdef USE_STREAM
    auto server = make_unique<ItsSocketStreamServer>(ItsSocketDomain::INET, (struct sockaddr*)&addr_server, sizeof(addr_server), ItsSocketStreamServer::DefaultBackdrop, false);
#else
    auto server = make_unique<ItsSocketDatagramServer>(ItsSocketDomain::INET, (struct sockaddr*)&addr_server, sizeof(addr_server), false);
#endif
    if ( server->GetInitWithError()) {
        cout << "> server initialized with error: " << strerror(server->GetInitWithErrorErrno()) << " <" << endl;
        return EXIT_FAILURE;
    }
    else {
        cout << "> server initialized successfully <" << endl;
    }

    struct sockaddr_in addr;
    socklen_t addr_length;

#ifdef USE_STREAM
    cout << "Waiting to accept client ..." << endl;
    int fd = server->Accept((struct sockaddr*)&addr, &addr_length);
    if ( fd == -1 ) {
        cout << "> server accept error: " << strerror(errno) << " <" << endl;
        return EXIT_FAILURE;
    }
    cout << "> client accepted <" << endl;
#endif

    //
    // Main program logic loop
    //
    char buf[MAX_BUF_SIZE] = "";
    while (strcmp(buf, "quit") != 0) {
        cout << endl;

        strcpy(buf, "");

        cout << "Awaiting data ..." << endl;
        ssize_t nRecv = 0;
#ifdef USE_STREAM
        nRecv = server->Read(fd, buf, MAX_BUF_SIZE);
#else
        nRecv = server->RecvFrom(buf, MAX_BUF_SIZE, 0, NULL, 0);
#endif
        if ( nRecv == -1 ) {
            cout << "> server error: " << strerror(errno) << " <" << endl;
            return EXIT_FAILURE;
        }

        cout << "> " << nRecv << " bytes received <" << endl;
        cout << "> " << buf << " <" << endl;
    }

    cout << endl << "> exiting server <" << endl;
    
    return EXIT_SUCCESS;
}

//
// Function: UpdateAppSettings
//
// (i): Takes program args and sets server settings from them.
//
void UpdateAppSettings(int argc, char** argv, AppSettings& settings)
{
    string port = GetArgVal("--server-port", argc, argv);
    string address = GetArgVal("--server-address", argc, argv);

    if ( port.length() > 0 ) {
        settings.ServerPort = ItsConvert::ToNumber<uint16_t>(port);
    }

    if ( address.length() > 0 ) {
        settings.ServerAddress = address;
    }
}

//
// Function: GetArgVal
//
// (i): Gets argument value if there is one.
//
string GetArgVal(string arg, int argc, char** argv)
{
    arg += "=";

    for ( int i = 0; i < argc; i++ ) {
        if ( string(argv[i]).find(arg) == 0 ) {
            return string(argv[i]).substr(arg.length());
        }
    }

    return string("");
}