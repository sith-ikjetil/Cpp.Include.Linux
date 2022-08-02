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
using ItSoftware::Linux::IPC::ItsSocketStreamServer;
using ItSoftware::Linux::ItsConvert;
using ItSoftware::Linux::ItsString;

//
// constexpr
//
constexpr auto MAX_BUF_SIZE = 4096;

//
// struct: AppSettings
//
// (i): Application runtime settings.
//
struct AppSettings {
    string      ServerAddress;
    uint16_t    ServerPort;
    string      ConnectionType;
};

//
// Functions prototypes
//
void UpdateAppSettings(int argc, char** argv, AppSettings& settings);
string GetArgVal(string arg, int argc, char** argv);
int MainSocketTCP(AppSettings& settings);
int MainSocketUDP(AppSettings& settings);

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
        .ServerPort = 5500,
        .ConnectionType = "UDP"
    };

    //
    // Update application settings from command line arg.
    //
    UpdateAppSettings(argc,argv,settings);

    //
    // Output app settings
    //
    cout << "##" << endl;
    cout << "## server (Cpp.Include.Linux)" << endl;
    cout << "##" << endl;
    cout << "## Usage: server --server-port=<port> --server-address=<server ip address>" << endl;
    cout << "##               --connection-type=<UDP/TCP>" << endl;
    cout << "##" << endl;
    cout << "Connection type : " << settings.ConnectionType << endl;
    cout << "Server port     : " << settings.ServerPort << endl;
    cout << "Server address  : " << settings.ServerAddress << endl;
    cout << endl;
    
    //
    // Run the UDP or TCP version of the application
    //
    if (settings.ConnectionType == "UDP") {
        return MainSocketUDP(settings);
    }
    return MainSocketTCP(settings);
}

//
// Function: UpdateAppSettings
//
// (i): Takes program args and sets server settings from them.
//
void UpdateAppSettings(int argc, char** argv, AppSettings& settings)
{
    string sport = GetArgVal("--server-port", argc, argv);
    string saddress = GetArgVal("--server-address", argc, argv);

    sport = ItsString::Trim(sport);
    saddress = ItsString::Trim(saddress);

    if ( sport.length() > 0 ) {
        try
        {
            settings.ServerPort = ItsConvert::ToNumber<uint16_t>(sport);
        }
        catch(const std::invalid_argument& e)
        {
            cout << "## server (Cpp.Include.Linux)" << endl;
            cout << "## ERROR ########################" << endl;
            cout << "--server-port invalid port number" << endl;
            exit(EXIT_FAILURE);
        }
    }

    if ( saddress.length() > 0 ) {
        settings.ServerAddress = saddress;
    }

    string connectionType = GetArgVal("--connection-type", argc, argv);
    if ( connectionType.length() > 0 ) {
        std::transform(begin(connectionType),end(connectionType), begin(connectionType), ::toupper);
        settings.ConnectionType = (connectionType == "TCP") ? "TCP" : "UDP";
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

//
// Function: MainSocketTCP
//
// (i): Main client logic for TCP.
//
int MainSocketTCP(AppSettings& settings)
{
    //
    // Create INET host address.
    //
    struct sockaddr_in addr_server{0};
    auto addr_host_server = ItsSocket::CreateSockAddrHostInet4(settings.ServerPort, settings.ServerAddress);
    addr_server = *addr_host_server;

    //
    // Make ItsSocketDatagramServer
    //
    auto server = make_unique<ItsSocketStreamServer>(ItsSocketDomain::INET, (struct sockaddr*)&addr_server, sizeof(addr_server), ItsSocketStreamServer::DefaultBackdrop, false);
    if ( server->GetInitWithError()) {
        cout << "> server initialized with error: " << strerror(server->GetInitWithErrorErrno()) << " <" << endl;
        cout << "> is this machine's ip address equal --server-address argument? <" << endl;
        return EXIT_FAILURE;
    }
    else {
        cout << "> server initialized successfully <" << endl;
    }

    struct sockaddr_in addr;
    socklen_t addr_length;

    cout << "Waiting to accept client ..." << endl;
    int fd = server->Accept((struct sockaddr*)&addr, &addr_length);
    if ( fd == -1 ) {
        cout << "> server accept error: " << strerror(errno) << " <" << endl;
        return EXIT_FAILURE;
    }
    cout << "> client accepted <" << endl;

    //
    // Main program logic loop
    //
    char buf[MAX_BUF_SIZE] = "";
    while (strcmp(buf, "quit") != 0) {
        cout << endl;

        strcpy(buf, "");

        cout << "Awaiting data ..." << endl;
        ssize_t nRecv = 0;

        nRecv = server->Read(fd, buf, MAX_BUF_SIZE);
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
// Function: MainSocketUDP
//
// (i): Main client logic for UDP.
//
int MainSocketUDP(AppSettings& settings)
{
//
    // Create INET host address.
    //
    struct sockaddr_in addr_server{0};
    auto addr_host_server = ItsSocket::CreateSockAddrHostInet4(settings.ServerPort, settings.ServerAddress);
    addr_server = *addr_host_server;

    //
    // Make ItsSocketDatagramServer
    //
    auto server = make_unique<ItsSocketDatagramServer>(ItsSocketDomain::INET, (struct sockaddr*)&addr_server, sizeof(addr_server), false);
    if ( server->GetInitWithError()) {
        cout << "> server initialized with error: " << strerror(server->GetInitWithErrorErrno()) << " <" << endl;
        cout << "> is this machine's ip address equal --server-address argument? <" << endl;
        return EXIT_FAILURE;
    }
    else {
        cout << "> server initialized successfully <" << endl;
    }

    struct sockaddr_in addr;
    socklen_t addr_length;

    //
    // Main program logic loop
    //
    char buf[MAX_BUF_SIZE] = "";
    while (strcmp(buf, "quit") != 0) {
        cout << endl;

        strcpy(buf, "");

        cout << "Awaiting data ..." << endl;

        ssize_t nRecv = 0;
        nRecv = server->RecvFrom(buf, MAX_BUF_SIZE, 0, NULL, 0);
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