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
// #define
//
// (i): uncomment USE_STREAM to use datagram.
//
//#define USE_STREAM

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
};

//
// Functions prototypes
//
void UpdateAppSettings(int argc, char** argv, AppSettings& settings);
string GetArgVal(string arg, int argc, char** argv);

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
        .ClientPort = 5501
    };

    //
    // Update application settings from command line arg.
    //
    UpdateAppSettings(argc,argv,settings);

    //
    // Output app settings
    //
    cout << "##" << endl;
    cout << "## client" << endl;
    cout << "##" << endl;
    cout << "## Usage: client --server-port=<port> --server-address=<server ip address>" << endl;
    cout << "##               --client-port=<port> --client-address=<client ip address>" << endl;
    cout << "##" << endl;
    cout << "> server port    : " << settings.ServerPort << " <" << endl;
    cout << "> server address : " << settings.ServerAddress << " <" << endl;
    cout << "> client port    : " << settings.ClientPort << " <" << endl;
    cout << "> client address : " << settings.ClientAddress << " <" << endl;

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
#ifdef USE_STREAM
    auto client = make_unique<ItsSocketStreamClient>(ItsSocketDomain::INET, (struct sockaddr*)&addr_server, sizeof(addr_server));
#else
    auto client = make_unique<ItsSocketDatagramClient>(ItsSocketDomain::INET, (struct sockaddr*)&addr_client, sizeof(addr_client), false);
#endif

    if ( client->GetInitWithError()) {
        cout << "> client initialized with error: " << strerror(client->GetInitWithErrorErrno()) << " <" << endl;
        return EXIT_FAILURE;
    }
    else {
        cout << "> client initialized successfully <" << endl;
    }

#ifdef USE_STREAM
    if ( client->Connect() == -1 ) {
        cout << "> connect failed to remote server <" << endl;
    }
#endif

    //
    // Main program logic loop
    //
    char buf[MAX_BUF_SIZE] = "";
    while (strcmp(buf, "quit") != 0) {
        cout << endl;
        cout << "(enter message <quit> to exit both client and server)" << endl;
        cout << "Message: ";
        cin.getline(buf,MAX_BUF_SIZE);
#ifdef USE_STREAM
        ssize_t nSent = client->Write(buf, strlen(buf)+1);
#else
        ssize_t nSent = client->SendTo(buf, strlen(buf)+1, 0, (struct sockaddr*)&addr_server, sizeof(addr_server));
#endif
        cout << "> " << nSent << " bytes sent <" << endl;
    }

    cout << endl << "> exiting client <" << endl;

    return EXIT_SUCCESS;
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

    if ( sport.length() > 0 ) {
        settings.ServerPort = ItsConvert::ToNumber<uint16_t>(sport);
    }

    if ( saddress.length() > 0 ) {
        settings.ServerAddress = saddress;
    }

    string cport = GetArgVal("--client-port", argc, argv);
    string caddress = GetArgVal("--client-address", argc, argv);

    if ( cport.length() > 0 ) {
        settings.ClientPort = ItsConvert::ToNumber<uint16_t>(cport);
    }

    if ( caddress.length() > 0 ) {
        settings.ClientAddress = caddress;
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