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
// using
//
using std::cout;
using std::cin;
using std::endl;
using std::string;
using ItSoftware::Linux::IPC::ItsSocket;
using ItSoftware::Linux::IPC::ItsSocketDomain;
using ItSoftware::Linux::IPC::ItsSocketDatagramServer;
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
    auto server = make_unique<ItsSocketDatagramServer>(ItsSocketDomain::INET, (struct sockaddr*)&addr_server, sizeof(addr_server));
    if ( server->GetInitWithError()) {
        cout << "> server initialized with error: " << strerror(server->GetInitWithErrorErrno()) << " <" << endl;
        return EXIT_FAILURE;
    }
    else {
        cout << "> server initialized successfully <" << endl;
    }

    //
    // Main program logic loop
    //
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