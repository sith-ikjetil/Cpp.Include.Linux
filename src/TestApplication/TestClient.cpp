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
using ItSoftware::Linux::IPC::ItsSocketStreamClient;
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
    string      ClientAddress;
    uint16_t    ClientPort;
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
// (i): Application entry point
//
int main(int argc, char** argv)
{
    //
    // Application settings. Set default.
    //
    AppSettings settings {
        .ServerAddress = "192.168.0.103",
        .ServerPort = 5500,
        .ClientAddress = "192.168.0.100",
        .ClientPort = 5501,
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
    cout << "## client (Cpp.Include.Linux)" << endl;
    cout << "##" << endl;
    cout << "## Usage: client --server-port=<port> --server-address=<server ip address>" << endl;
    cout << "##               --client-port=<port> --client-address=<client ip address>" << endl;
    cout << "##               --connection-type=<UDP/TCP>" << endl;
    cout << "##" << endl;
    cout << "Connection type : " << settings.ConnectionType << endl;
    cout << "Server port     : " << settings.ServerPort << endl;
    cout << "Server address  : " << settings.ServerAddress << endl;
    cout << "Client port     : " << settings.ClientPort << endl;
    cout << "Client address  : " << settings.ClientAddress << endl;
    cout << endl;
    
    //
    // Run the UDP or TCP version of the application
    //
    if ( settings.ConnectionType == "UDP" ) {
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
            cout << "## client (Cpp.Include.Linux)" << endl;
            cout << "## ERROR ########################" << endl;
            cout << "--server-port invalid port number" << endl;
            exit(EXIT_FAILURE);
        }
    }

    if ( saddress.length() > 0 ) {
        settings.ServerAddress = saddress;
    }

    string cport = GetArgVal("--client-port", argc, argv);
    string caddress = GetArgVal("--client-address", argc, argv);

    cport = ItsString::Trim(cport);
    caddress = ItsString::Trim(caddress);

    if ( cport.length() > 0 ) {
        try
        {
            settings.ClientPort = ItsConvert::ToNumber<uint16_t>(cport);
        }
        catch(const std::invalid_argument& e)
        {
            cout << "## client (Cpp.Include.Linux)" << endl;
            cout << "## ERROR ########################" << endl;
            cout << "--client-port invalid port number" << endl;
            exit(EXIT_FAILURE);
        }
    }

    if ( caddress.length() > 0 ) {
        settings.ClientAddress = caddress;
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
int MainSocketTCP(AppSettings& settings) {
    //
    // Create INET server host address.
    //
    struct sockaddr_in addr_server{0};
    auto addr_host_server = ItsSocket::CreateSockAddrHostInet4(settings.ServerPort, settings.ServerAddress);
    addr_server = *addr_host_server;
    
    //
    // CREATE INET client host address
    //
    struct sockaddr_in addr_client{0};
    auto addr_host_client = ItsSocket::CreateSockAddrHostInet4(settings.ClientPort, settings.ClientAddress);
    addr_client = *addr_host_client;

    //
    // Make ItsSocketStreamClient
    //
    auto client = make_unique<ItsSocketStreamClient>(ItsSocketDomain::INET, (struct sockaddr*)&addr_server, sizeof(addr_server));
    if ( client->GetInitWithError()) {
        cout << "> client initialized with error: " << strerror(client->GetInitWithErrorErrno()) << " <" << endl;
        return EXIT_FAILURE;
    }
    else {
        cout << "> client initialized successfully <" << endl;
    }

    if ( client->Connect() == -1 ) {
        cout << "> connect failed to remote server <" << endl;
    }

    //
    // Main program logic loop
    //
    char buf[MAX_BUF_SIZE] = "";
    while (strcmp(buf, "quit") != 0) {
        cout << endl;
        cout << "(enter message <quit> to exit both client and server)" << endl;
        cout << "Message: ";
        cin.getline(buf,MAX_BUF_SIZE);
        
        ssize_t nSent = client->Write(buf, strlen(buf)+1);
        cout << "> " << nSent << " bytes sent <" << endl;
    }

    cout << endl << "> exiting client <" << endl;

    return EXIT_SUCCESS;
}

//
// Function: MainSocketUDP
//
// (i): Main client logic for UDP.
//
int MainSocketUDP(AppSettings& settings) {
    //
    // Create INET server host address.
    //
    struct sockaddr_in addr_server{0};
    auto addr_host_server = ItsSocket::CreateSockAddrHostInet4(settings.ServerPort, settings.ServerAddress);
    addr_server = *addr_host_server;
    
    //
    // CREATE INET client host address
    //
    struct sockaddr_in addr_client{0};
    auto addr_host_client = ItsSocket::CreateSockAddrHostInet4(settings.ClientPort, settings.ClientAddress);
    addr_client = *addr_host_client;

    //
    // Make ItsSocketDatagramClient
    //
    auto client = make_unique<ItsSocketDatagramClient>(ItsSocketDomain::INET, (struct sockaddr*)&addr_client, sizeof(addr_client), false);

    if ( client->GetInitWithError()) {
        cout << "> client initialized with error: " << strerror(client->GetInitWithErrorErrno()) << " <" << endl;
        cout << "> is this machine's ip address equal --client-address argument? <" << endl;
        return EXIT_FAILURE;
    }
    else {
        cout << "> client initialized successfully <" << endl;
    }

    //
    // Main program logic loop
    //
    char buf[MAX_BUF_SIZE] = "";
    while (strcmp(buf, "quit") != 0) {
        cout << endl;
        cout << "(enter message <quit> to exit both client and server)" << endl;
        cout << "Message: ";
        cin.getline(buf,MAX_BUF_SIZE);
        
        ssize_t nSent = client->SendTo(buf, strlen(buf)+1, 0, (struct sockaddr*)&addr_server, sizeof(addr_server));
        cout << "> " << nSent << " bytes sent <" << endl;
    }

    cout << endl << "> exiting client <" << endl;

    return EXIT_SUCCESS;
}