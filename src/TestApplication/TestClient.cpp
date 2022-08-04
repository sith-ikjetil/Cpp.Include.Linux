///////////////////////////////////////////////////////////////////
//: Title        : TestClient.cpp
//: Product:     : Simple client example
//: Date         : 2022-07-30
//: Author       : "Kjetil Kristoffer Solberg" <post@ikjetil.no>
//: Version      : 1.0.0.0
//: Descriptions : Implementation of client simple example.
//
// #include
//
#include <iostream>
#include <string>
#include "../include/itsoftware-linux-core.h"
#include "../include/itsoftware-linux-ipc.h"
#include "../include/itsoftware-linux.h"

//
// using
//
using std::cout;
using std::cin;
using std::endl;
using std::stringstream;
using ItSoftware::Linux::IPC::ItsSocket;
using ItSoftware::Linux::IPC::ItsSocketDomain;
using ItSoftware::Linux::IPC::ItsSocketDatagramClient;
using ItSoftware::Linux::IPC::ItsSocketStreamClient;
using ItSoftware::Linux::ItsConvert;
using ItSoftware::Linux::ItsString;

//
// constexpr
//
static constexpr auto MAX_BUF_SIZE = 4096;
static constexpr auto COLOR_CYAN = "\033[36;1m";
static constexpr auto COLOR_GREEN = "\033[32m";
static constexpr auto COLOR_WHITE = "\033[37;1m";
static constexpr auto COLOR_RESET = "\033[0m";
static constexpr auto ARG_SERVER_PORT = "--server-port";
static constexpr auto ARG_SERVER_ADDRESS = "--server-address";
static constexpr auto ARG_CLIENT_PORT = "--client-port";
static constexpr auto ARG_CLIENT_ADDRESS = "--client-address";
static constexpr auto ARG_CONNECTION_TYPE = "--connection-type";
static constexpr auto ARG_NO_COLOR_OUTPUT = "--no-color-output";

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
    bool        NoColorOutput;
};

//
// Functions prototypes
//
void UpdateAppSettings(int argc, char** argv, AppSettings& settings);
string GetArgVal(string arg, int argc, char** argv);
bool GetHasArg(string arg, int argc, char** argv);
int MainSocketTCP(AppSettings& settings);
int MainSocketUDP(AppSettings& settings);
void PrintProlog(const AppSettings& settings);
void PrintError(const AppSettings& settings, const string& msg);
void PrintEvent(const AppSettings& settings, const string& msg);

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
        .ConnectionType = "UDP",
        .NoColorOutput = false
    };

    //
    // Update application settings from command line arg.
    //
    UpdateAppSettings(argc,argv,settings);

    //
    // Output app settings
    //
    PrintProlog(settings);
    
    //
    // Run the UDP or TCP version of the application
    //
    if ( settings.ConnectionType == "UDP" ) {
        return MainSocketUDP(settings);
    }
    
    return MainSocketTCP(settings);
}

//
// Function: PrintProlog
//
// (i): prints prolog message to stdout
//
void PrintProlog(const AppSettings& settings)
{
    if (!settings.NoColorOutput) { cout << COLOR_RESET << COLOR_GREEN; }
    cout << "##" << endl;
    cout << "## TestClient (Cpp.Include.Linux)" << endl;
    cout << "##" << endl;
    cout << "## Usage: TestClient" << endl; 
    cout << "##        --server-port=<port> --server-address=<server ip address>" << endl;
    cout << "##        --client-port=<port> --client-address=<client ip address>" << endl;
    cout << "##        --connection-type=<UDP/TCP>" << endl;
    cout << "##        --no-color-output" << endl;
    cout << "##" << endl;
    cout << "Connection type : " << settings.ConnectionType << endl;
    cout << "Server port     : " << settings.ServerPort << endl;
    cout << "Server address  : " << settings.ServerAddress << endl;
    cout << "Client port     : " << settings.ClientPort << endl;
    cout << "Client address  : " << settings.ClientAddress << endl;
    cout << endl;
    if (!settings.NoColorOutput) { cout << COLOR_RESET << COLOR_WHITE; }
}

//
// Function: PrintError
//
// (i): prints error message to stdout
//
void PrintError(const AppSettings& settings, const string& msg)
{
    if (!settings.NoColorOutput) { cout << COLOR_RESET << COLOR_GREEN; }
    cout << std::left << std::setw(36) << std::setfill('#') << "## ERROR ##" << endl;
    if (!settings.NoColorOutput) { cout << COLOR_RESET << COLOR_WHITE; }
    cout << msg << endl;
}

//
// Function: PrintEvent
//
// (i): prints event message to stdout
//
void PrintEvent(const AppSettings& settings, const string& msg)
{
    if (!settings.NoColorOutput) { cout << COLOR_RESET << COLOR_CYAN; }
    cout << "> " << msg << " <" << endl;
    if (!settings.NoColorOutput) { cout << COLOR_RESET << COLOR_WHITE; }
}

//
// Function: UpdateAppSettings
//
// (i): Takes program args and sets server settings from them.
//
void UpdateAppSettings(int argc, char** argv, AppSettings& settings)
{
    string sport = GetArgVal(ARG_SERVER_PORT, argc, argv);
    string saddress = GetArgVal(ARG_SERVER_ADDRESS, argc, argv);

    sport = ItsString::Trim(sport);
    saddress = ItsString::Trim(saddress);

    if ( sport.length() > 0 ) {
        try
        {
            settings.ServerPort = ItsConvert::ToNumber<uint16_t>(sport);
        }
        catch(const std::invalid_argument& e)
        {
            PrintError(settings, "--server-port invalid port number");
            exit(EXIT_FAILURE);
        }
    }

    if ( saddress.length() > 0 ) {
        settings.ServerAddress = saddress;
    }

    string cport = GetArgVal(ARG_CLIENT_PORT, argc, argv);
    string caddress = GetArgVal(ARG_CLIENT_ADDRESS, argc, argv);

    cport = ItsString::Trim(cport);
    caddress = ItsString::Trim(caddress);

    if ( cport.length() > 0 ) {
        try
        {
            settings.ClientPort = ItsConvert::ToNumber<uint16_t>(cport);
        }
        catch(const std::invalid_argument& e)
        {
            PrintError(settings,"--client-port invalid port number");
            exit(EXIT_FAILURE);
        }
    }

    if ( caddress.length() > 0 ) {
        settings.ClientAddress = caddress;
    }

    string connectionType = GetArgVal(ARG_CONNECTION_TYPE, argc, argv);
    if ( connectionType.length() > 0 ) {
        std::transform(begin(connectionType),end(connectionType), begin(connectionType), ::toupper);
        settings.ConnectionType = (connectionType == "TCP") ? "TCP" : "UDP";
    }

    settings.NoColorOutput = GetHasArg(ARG_NO_COLOR_OUTPUT, argc, argv);
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
// Function: GetHasArg
//
// (i): Gets if argument is set on command line.
//
bool GetHasArg(string arg, int argc, char** argv)
{
    for ( int i = 0; i < argc; i++ ) {
        if ( string(argv[i]).find(arg) == 0 ) {
            return true;
        }
    }

    return false;
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
        stringstream ss;
        ss << "client initialized with error: " << strerror(client->GetInitWithErrorErrno());
        PrintError(settings, ss.str());
        return EXIT_FAILURE;
    }
    else {
        PrintEvent(settings,"client initialized successfully");
    }

    if ( client->Connect() == -1 ) {
        stringstream ss;
        ss << "connect failed to remote server: " << strerror(errno) << endl;
        ss << "have you started server yet?";
        PrintError(settings, ss.str());
        return EXIT_FAILURE;
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
        stringstream ss;
        ss << nSent << " bytes sent";
        PrintEvent(settings,ss.str());
    }

    cout << endl;
    PrintEvent(settings, "exiting client");
    cout << endl;

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
        stringstream ss;
        ss << "> client initialized with error: " << strerror(client->GetInitWithErrorErrno()) << " <" << endl;
        ss << "> is this machine's ip address equal --client-address argument? <" << endl;
        PrintError(settings, ss.str());
        return EXIT_FAILURE;
    }
    else {
        PrintEvent(settings, "client initialized successfully");
    }

    //
    // Main progm); logic loop
    //
    char buf[MAX_BUF_SIZE] = "";
    while (strcmp(buf, "quit") != 0) {
        cout << endl;
        cout << "(enter message <quit> to exit both client and server)" << endl;
        cout << "Message: ";
        cin.getline(buf,MAX_BUF_SIZE);
        
        ssize_t nSent = client->SendTo(buf, strlen(buf)+1, 0, (struct sockaddr*)&addr_server, sizeof(addr_server));
        stringstream ss;
        ss << nSent << " bytes sent";
        PrintEvent(settings,ss.str());
    }

    cout << endl;
    PrintEvent(settings, "exiting client");
    cout << endl;

    return EXIT_SUCCESS;
}