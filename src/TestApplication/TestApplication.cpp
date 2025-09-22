///////////////////////////////////////////////////////////////////
//: Title        : TestApplication.cpp
//: Product:     : Cpp.Include.Windows
//: Date         : 2020-05-01
//: Author       : "Kjetil Kristoffer Solberg" <post@ikjetil.no>
//: Version      : 1.0.0.0
//: Descriptions : Implementation of Cpp.Include.Window Test Application.
//
// #include
//
#include <iostream>
#include <string>
#include <chrono>
#include <vector>
#include <thread>
#include <memory>
#include "../include/itsoftware-linux.h"
#include "../include/itsoftware-linux-core.h"
#include "../include/itsoftware-linux-ipc.h"

namespace ItSoftware::CppIncludeLinux::TestApplication
{
    //
    // using
    //
    using std::cout;
    using std::endl;
    using std::vector;
    using std::thread;
    using std::string;
    using std::stringstream;
    using std::to_string;
    using std::unique_ptr;
    using std::make_unique;
    using ItSoftware::Linux::ItsString;
    using ItSoftware::Linux::ItsExpandDirection;
    using ItSoftware::Linux::ItsTime;
    using ItSoftware::Linux::ItsConvert;
    using ItSoftware::Linux::ItsRandom;
    using ItSoftware::Linux::ItsDateTime;
    using ItSoftware::Linux::ItsLog;
    using ItSoftware::Linux::ItsLogType;
    using ItSoftware::Linux::ItsDataSizeStringType;
    using ItSoftware::Linux::ItsID;
    using ItSoftware::Linux::ItsCreateIDOptions;
    using ItSoftware::Linux::Core::ItsTimer;
    using ItSoftware::Linux::Core::ItsFile;
    using ItSoftware::Linux::Core::ItsGuid;
    using ItSoftware::Linux::Core::ItsGuidFormat;
    using ItSoftware::Linux::Core::ItsPath;
    using ItSoftware::Linux::Core::ItsDirectory;
    using ItSoftware::Linux::Core::ItsError;
    using ItSoftware::Linux::Core::ItsFileMonitor;
    using ItSoftware::Linux::Core::ItsFileMonitorMask;
    using ItSoftware::Linux::IPC::ItsSocket;
    using ItSoftware::Linux::IPC::ItsSocketStreamServer;
    using ItSoftware::Linux::IPC::ItsSocketStreamClient;
    using ItSoftware::Linux::IPC::ItsSocketConType;
    using ItSoftware::Linux::IPC::ItsSocketDomain;
    using ItSoftware::Linux::IPC::ItsSocketDatagramServer;
    using ItSoftware::Linux::IPC::ItsSocketDatagramClient;
    using ItSoftware::Linux::IPC::ItsPipe;
    using ItSoftware::Linux::IPC::ItsSvMsgQueue;
    using ItSoftware::Linux::IPC::ItsSvMsgFlags;
    using ItSoftware::Linux::IPC::ItsSvMsg1k;
    using ItSoftware::Linux::IPC::ItsFifoServer;
    using ItSoftware::Linux::IPC::ItsFifoClient;
    using ItSoftware::Linux::IPC::ItsFifoHeader;
    using ItSoftware::Linux::Core::ItsTimeTracker;

    //
    // Function Prototypes
    //
    void TestItsSocketDatagramClientServerStart();
    void TestItsSocketStreamClientServerStart();
    void TestItsConvert();
    void TestItsRandom();
    void TestItsTime();
    void TestItsString();
    void TestItsLog();
    void TestItsTimerStart();
    void TestItsTimerStop();
    void TestItsFile();
    void TestItsDateTime();
    void TestItsID();
    void TestItsGuid();
    void TestItsPath();
    void TestItsDirectory();
    void TestItsFileMonitorStart();
    void TestItsFileMonitorStop();
    void ExitFn();
    void PrintHeader(const string& txt);
    void PrintSubHeader(const string& txt);
    void PrintApplicationEvent(const string& event);
    void HandleFileEvent(inotify_event& event);
    void TestItsSocketStreamClientServerStop();
    void TestItsSocketDatagramClientServerStop();
    void TestItsPipe();
    void TestItsSvMsgQueue();
    void TestItsFifo();
    void InitializeTestDirectory();
    void CleanupTestDirectory();

    //
    // constexpr
    //
    constexpr auto COLOR_GREEN = "\033[32m";
    constexpr auto COLOR_WHITE = "\033[37;1m";
    constexpr auto COLOR_RESET = "\033[0m";

    //
    // global data
    //
    ItsTimer g_timer;
    unique_ptr<ItsFileMonitor> g_fm;
    vector<string> g_fileMonNames;
    char g_filename[] = "/tmp/CppIncludeLinux/test.txt";
    char g_copyToFilename[] = "/tmp/CppIncludeLinux/test2.txt";
    char g_shredFilename[] = "/tmp/CppIncludeLinux/test2shred.txt";
    string g_path1("/tmp");
    string g_path2("/CppIncludeLinux/test.txt");
    string g_invalidPath("home\0/kjetilso");
    string g_directoryRoot("/tmp/CppIncludeLinux/");
    string g_creatDir("/tmp/CppIncludeLinux/testdir");

    //
    // global data, IPC
    //
    unique_ptr<ItsSocketStreamServer> g_socket_stream_server;
    unique_ptr<ItsSocketStreamClient>  g_socket_stream_client;
    unique_ptr<ItsSocketDatagramServer> g_socket_dg_server;
    unique_ptr<ItsSocketDatagramClient>  g_socket_dg_client;
    vector<string> g_socket_stream_traffic;
    vector<string> g_socket_dg_traffic;
    unique_ptr<thread> g_socket_stream_thread1;
    unique_ptr<thread> g_socket_stream_thread2;
    unique_ptr<thread> g_socket_dg_thread1;
    unique_ptr<thread> g_socket_dg_thread2;
    struct sockaddr_un g_addr{0};
    struct sockaddr_un g_saddr{0};
    struct sockaddr_un g_caddr{0};

    //
    // Function: ExitFn
    //
    // (i): Print exit message.
    // 
    void ExitFn()
    {
        cout << endl;
        PrintApplicationEvent("Completed");
    }

    //
    // Function: main
    //
    // (i): Application entry point
    //
    int main(int argc, const char* argv[])
    {
        ItsTimeTracker tt("main");

        // Set exit handler
        atexit(ExitFn);

        //
        // Initialize test directory.
        //
        InitializeTestDirectory();

        //
        // Start.
        //
        PrintApplicationEvent("Started");

        //
        // Do all tests.
        //
        TestItsTimerStart();
        TestItsSocketStreamClientServerStart();
        TestItsSocketDatagramClientServerStart();
        TestItsFileMonitorStart();
        TestItsConvert();
        TestItsRandom();
        TestItsTime();
        TestItsString();
        TestItsLog();
        TestItsFile();
        TestItsDateTime();
        TestItsID();
        TestItsGuid();
        TestItsPath();
        TestItsDirectory();
        TestItsFileMonitorStop();
        TestItsSocketDatagramClientServerStop();
        TestItsSocketStreamClientServerStop();
        TestItsPipe();
        TestItsSvMsgQueue();
        TestItsFifo();
        TestItsTimerStop();
        
        //
        // Cleanup test directory.
        //
        CleanupTestDirectory();

        //
        // Exit all ok
        //
        return EXIT_SUCCESS;
    }

    //
    // Function: InitializeTestDirectory
    //
    // (i): Initialize the test directory.
    //
    void InitializeTestDirectory()
    {
        CleanupTestDirectory();
        ItsDirectory::CreateDirectory(g_directoryRoot, ItsFile::CreateMode("rwx","rwx","rwx"));
    }

    //
    // Function: CleanupTestDirectory
    //
    // (i): Remove test directory if exists.
    //
    void CleanupTestDirectory()
    {
        if ( ItsDirectory::Exists(g_directoryRoot) ) {
            ItsFile::Delete(g_directoryRoot);
        }
    }

    //
    // Function: PrintApplicationEvent
    //
    // (i): prints application event string.
    //
    void PrintApplicationEvent(const string& event)
    {
        cout << COLOR_RESET << COLOR_GREEN;
        cout << std::setw(80) << std::setfill('#') << std::left << "## Test Application " << endl;
        cout << COLOR_RESET << COLOR_WHITE;
        cout << "> "<< event << " <" << endl;
    }

    //
    // Function: PrintHeader
    //
    // (i): Prints a tests header.
    //
    void PrintHeader(const string& txt)
    {
        cout << COLOR_RESET << COLOR_GREEN;
        cout << endl;

        stringstream ss;
        ss << " " << txt << " ";
        cout << ItsString::WidthExpand(ss.str(), 80, '_', ItsExpandDirection::Middle) << endl;

        cout << COLOR_RESET << COLOR_WHITE;
    }

    //
    // Function: PrintSubHeader
    //
    // (i): Prints a tests sub header.
    //
    void PrintSubHeader(const string& txt)
    {
        cout << COLOR_RESET << COLOR_GREEN;

        cout << endl;
        cout << "__ " << txt << " __" << endl;

        cout << COLOR_RESET << COLOR_WHITE;
    }

    //
    // Function: TestToNumber
    //
    // (i): Test numerics as string converted to primitive data types.
    //
    void TestItsConvert()
    {
        PrintHeader("ItsConvert");

        PrintSubHeader("ToNumber");
        cout << R"(ItsConvert::ToNumber<int>("-1234"))" << endl;
        cout << "> " << ItsConvert::ToNumber<int>("-1234") << endl;
        cout << R"(ItsConvert::ToNumber<unsigned int>("1234"))" << endl;
        cout << "> " << ItsConvert::ToNumber<unsigned int>("1234") << endl;
        cout << R"(ItsConvert::ToNumber<long>("-1234"))" << endl;
        cout << "> " << ItsConvert::ToNumber<long>("-1234") << endl;
        cout << R"(ItsConvert::ToNumber<unsigned long>("123456789"))" << endl;
        cout << "> " << ItsConvert::ToNumber<unsigned long>("12346789") << endl;
        cout << R"(ItsConvert::ToNumber<long long>("12345678900"))" << endl;
        cout << "> " << ItsConvert::ToNumber<long long>("12345678900") << endl;
        cout << R"(ItsConvert::ToNumber<unsigned long long>("12345678900"))" << endl;
        cout << "> " << ItsConvert::ToNumber<unsigned long long>("12345678900") << endl;
        cout << R"(ItsConvert::ToNumber<float>("-12.34"))" << endl;
        cout << "> " << ItsConvert::ToNumber<float>("-12.34") << endl;
        cout << R"(ItsConvert::ToNumber<double>("1.234"))" << endl;
        cout << "> " << ItsConvert::ToNumber<double>("1.234") << endl;
        cout << R"(ItsConvert::ToNumber<long double>("1.234"))" << endl;
        cout << "> " << ItsConvert::ToNumber<long double>("1.234") << endl;
        cout << R"(ItsConvert::ToNumber<short>("1234"))" << endl;
        cout << "> " << ItsConvert::ToNumber<short>("1234") << endl;
        cout << R"(ItsConvert::ToNumber<unsigned short>("40001"))" << endl;
        cout << "> " << ItsConvert::ToNumber<unsigned short>("40001") << endl;

        PrintSubHeader("ToString");
        cout << R"(ItsConvert::ToString<int>(-1234))" << endl;
        cout << R"(> ")" << ItsConvert::ToString<int>(-1234) << R"(")" << endl;
        cout << R"(ItsConvert::ToString<unsigned int>(1234))" << endl;
        cout << R"(> ")" << ItsConvert::ToString<unsigned int>(1234) << R"(")" << endl;
        cout << R"(ItsConvert::ToString<long>(-1234))" << endl;
        cout << R"(> ")" << ItsConvert::ToString<long>(-1234) << R"(")" << endl;
        cout << R"(ItsConvert::ToString<unsigned long>(123456789))" << endl;
        cout << R"(> ")" << ItsConvert::ToString<unsigned long>(12346789) << R"(")" << endl;
        cout << R"(ItsConvert::ToString<long long>(12345678900))" << endl;
        cout << R"(> ")" << ItsConvert::ToString<long long>(12345678900) << R"(")" << endl;
        cout << R"(ItsConvert::ToString<unsigned long long>(12345678900))" << endl;
        cout << R"(> ")" << ItsConvert::ToString<unsigned long long>(12345678900) << R"(")" << endl;
        cout << R"(ItsConvert::ToString<float>(-12.34))" << endl;
        cout << R"(> ")" << ItsConvert::ToString<float>(-12.34f) << R"(")" << endl;
        cout << R"(ItsConvert::ToString<double>(1.234))" << endl;
        cout << R"(> ")" << ItsConvert::ToString<double>(1.234) << R"(")" << endl;
        cout << R"(ItsConvert::ToString<long double>(1.234))" << endl;
        cout << R"(> ")" << ItsConvert::ToString<long double>(1.234) << R"(")" << endl;
        cout << R"(ItsConvert::ToString<short>(1234))" << endl;
        cout << R"(> ")" << ItsConvert::ToString<short>(1234) << R"(")" << endl;
        cout << R"(ItsConvert::ToString<unsigned short>(40001))" << endl;
        cout << R"(> ")" << ItsConvert::ToString<unsigned short>(40001) << R"(")" << endl;
        
        PrintSubHeader("ToStringFormatted");
        cout << R"(ItsConvert::ToStringFormatted(256810246))" << endl;
        cout << R"(> ")" << ItsConvert::ToStringFormatted(256810246) << R"(")" << endl;
        cout << R"(ItsConvert::ToStringFormatted(256810246, L' '))" << endl;
        cout << R"(> ")" << ItsConvert::ToStringFormatted(256810246, L' ') << R"(")" << endl;
        
        PrintSubHeader("ToDataSizeString");
        cout << R"(ItsConvert::ToDataSizeString(1024, 2))" << endl;
        cout << R"(> ")" << ItsConvert::ToDataSizeString(1024, 2) << R"(")" << endl;
        cout << R"(ItsConvert::ToDataSizeString(200100400, 0))" << endl;
        cout << R"(> ")" << ItsConvert::ToDataSizeString(200100400, 0) << R"(")" << endl;
        cout << R"(ItsConvert::ToDataSizeString(200100400, 2))" << endl;
        cout << R"(> ")" << ItsConvert::ToDataSizeString(200100400, 2) << R"(")" << endl;
        cout << R"(ItsConvert::ToDataSizeString(9807804606, 3, ItsDataSizeStringType::IEC))" << endl;
        cout << R"(> ")" << ItsConvert::ToDataSizeString(9807804606, 3, ItsDataSizeStringType::IEC) << R"(")" << endl;
        cout << R"(ItsConvert::ToDataSizeString(1000, 3, ItsDataSizeStringType::IEC))" << endl;
        cout << R"(> ")" << ItsConvert::ToDataSizeString(1000, 3, ItsDataSizeStringType::IEC) << R"(")" << endl;
        cout << R"(ItsConvert::ToDataSizeString(size_t max, 2))" << endl;
        cout << R"(> ")" << ItsConvert::ToDataSizeString(SIZE_MAX, 2) << R"(")" << endl;
        cout << R"(ItsConvert::ToDataSizeString(size_t max, 2, ItsDataSizeStringType::IEC))" << endl;
        cout << R"(> ")" << ItsConvert::ToDataSizeString(SIZE_MAX, 2, ItsDataSizeStringType::IEC) << R"(")" << endl;

        PrintSubHeader("ToLongFromHex");
        cout << R"(ItsConvert::ToLongFromHex("0xFF3333"))" << endl;
        cout << "> " << ItsConvert::ToLongFromHex("0xFF3333") << endl;

        cout << endl;
    }

    //
    // Function: TestRandom
    //
    // (i): Test random integers and floats.
    //
    void TestItsRandom()
    {
        PrintHeader("ItsRandom");

        cout << "ItsRandom<long>(10'000, 1'000'000)" << endl;
        cout << "> " << ItsRandom<long>(10'000, 1'000'000) << endl;
        cout << "ItsRandom<long>(10'000, 1'000'000)" << endl;
        cout << "> " << ItsRandom<long>(10'000, 1'000'000) << endl;
        cout << "ItsRandom<float>(1, 10)" << endl;
        cout << "> " << ItsRandom<float>(1, 10) << endl;
        cout << "ItsRandom<float>(1, 10)" << endl;
        cout << "> " << ItsRandom<float>(1, 10) << endl;
        cout << "ItsRandom<double>(1, 2)" << endl;
        cout << "> " << ItsRandom<double>(1, 2) << endl;
        cout << "ItsRandom<double>(1, 2)" << endl;
        cout << "> " << ItsRandom<double>(1, 2) << endl;

        cout << endl;
    }

    //
    // Function: TestTime
    //
    // (i): Test rendering of time from milliseconds.
    //
    void TestItsTime()
    {
        PrintHeader("ItsTime");

        PrintSubHeader("RenderMsToFullString");
        cout << "ItsTime::RenderMsToFullString(92481379, false)" << endl;
        cout << R"(> ")" << ItsTime::RenderMsToFullString(92481379, false) << R"(")" << endl;
        cout << "ItsTime::RenderMsToFullString(92481379, true)" << endl;
        cout << R"(> ")" << ItsTime::RenderMsToFullString(92481379, true) << R"(")" << endl;
        
        PrintSubHeader("Now + ToString");
        cout << "ItsDateTime::Now().ToString()" << endl;
        cout << R"(> ")" << ItsDateTime::Now().ToString() << R"(")" << endl;
        cout << R"(ItsDateTime.Now().ToString("s"))" << endl;
        cout << R"(> ")" << ItsDateTime::Now().ToString("s") << R"(")" << endl;

        cout << endl;
    }

    //
    // Function: TestString
    //
    // (i): Test string manipulation routines.
    //
    void TestItsString()
    {
        PrintHeader("ItsString");

        cout << R"(ItsString::Left("Ab12Cd",4))" << endl;
        cout << R"(> ")" << ItsString::Left("Ab12Cd", 4) << R"(")" << endl;
        cout << R"(ItsString::Right("Ab12Cd",4))" << endl;
        cout << R"(> ")" << ItsString::Right("Ab12Cd", 4) << R"(")" << endl;
        cout << R"(ItsString::Mid("Ab12Cd",2,2))" << endl;
        cout << R"(> ")" << ItsString::Mid("Ab12Cd", 2, 2) << R"(")" << endl;
        cout << R"(ItsString::ToLowerCase("Ab12Cd"))" << endl;
        cout << R"(> ")" << ItsString::ToLowerCase("Ab12Cd") << R"(")" << endl;
        cout << R"(ItsString::ToUpperCase("Ab12Cd"))" << endl;
        cout << R"(> ")" << ItsString::ToUpperCase("Ab12Cd") << R"(")" << endl;
        cout << R"(ItsString::Trim("  Ab12Cd  "))" << endl;
        cout << R"(> ")" << ItsString::Trim("  Ab12Cd  ") << R"(")" << endl;
        cout << R"(ItsString::Replace("__ ABCDEF __"))" << endl;
        cout << R"(> ")" << ItsString::Replace("__ ABCDEF __", "__", "##") << R"(")" << endl;
        cout << R"(ItsString::Split("A;BC;DEF;G",";"))" << endl;
        auto vs = ItsString::Split("A;BC;DEF;G", ";");
        stringstream ss;
        ss << "{";
        bool bFirst{true};
        for (auto s : vs) {
            if ( !bFirst ) {
                ss << ",";
            }
            ss << R"(")" << s << R"(")";

            bFirst = false;            
        }
        ss << "}";
        
        string temp = ss.str();
        cout << "> " << temp << endl;
        cout << R"(ItsString::WidthExpand ("Kjetil", 30, L'_', ItsExpandDirection:Left))" << endl;
        cout << R"(> ")" << ItsString::WidthExpand("Kjetil", 30, L'_', ItsExpandDirection::Left) << R"(")" << endl;
        cout << R"(ItsString::WidthExpand ("Kjetil", 30, L'_', ItsExpandDirection:Middle))" << endl;
        cout << R"(> ")" << ItsString::WidthExpand("Kjetil", 30, L'_', ItsExpandDirection::Middle) << R"(")" << endl;
        cout << R"(ItsString::WidthExpand ("Kjetil", 30, L'_', ItsExpandDirection:Right))" << endl;
        cout << R"(> ")" << ItsString::WidthExpand("Kjetil", 30, L'_', ItsExpandDirection::Right) << R"(")" << endl;

        cout << endl;
    }

    //
    // Function: TestLog
    //
    // (i): Test ItsLog.
    //
    void TestItsLog()
    {
        PrintHeader("ItsLog");

        ItsLog log{"ItsTestApp",true};
        log.LogInformation("This is an information log item");
        log.LogWarning("This is an warning log item");
        log.LogError("This is an error log item");
        log.LogOther("This is an other log item");
        log.LogDebug("This is an debug log item");

        PrintSubHeader("ToString");
        cout << log.ToString() << endl;

        PrintSubHeader("ToString(tailN=1)");
        cout << log.ToString(1) << endl;
        
        cout << endl;
    }

    //
    // Function: TestFile
    //
    // (i): Test of ItsFile.
    //
    void TestItsFile()
    {        
        PrintHeader("ItsFile");

        ItsFile file;
        cout << R"(file.OpenOrCreate(g_filename,"rwt",ItsFile::CreateMode("rw","rw","rw")))" << endl;
        if (!file.OpenOrCreate(g_filename,"rwt",ItsFile::CreateMode("rw","rw","rw")))
        {
            cout << "> FAILED: " << ItsError::GetLastErrorDescription() << endl;
            cout << endl;
            return;
        }
        cout << "> Success" << endl;

        char text[] = "Test Line 1\nTest Line 2\n";
        size_t written(0);
        cout << "file.Write((void*)text,strlen(text), &written)" << endl;
        if ( !file.Write(reinterpret_cast<void*>(text),strlen(text), &written) )
        {
            cout << "> FAILED: " << ItsError::GetLastErrorDescription() << endl;
            cout << endl;
            return;
        }
        cout << "> Success. " << written << " bytes written to " << g_filename << endl;
        

        string str;
        cout << "file.ReadAllText(str)" << endl;
        if (!file.ReadAllText(str)) {
            cout << "> FAILED: " << ItsError::GetLastErrorDescription() << endl;
            cout << endl;
            return;
        }
        cout << "> Success. Read all text from file " << g_filename << endl;
                

        cout << "file.Close()" << endl;
        file.Close();
        cout << "> Success" << endl;

        string content;
        cout << "ItsFile::ReadAllText(str)" << endl;
        if (!ItsFile::ReadAllText(g_filename, content)) {
            cout << "> FAILED: " << ItsError::GetLastErrorDescription() << endl;
            cout << endl;
            return;
        }
        cout << "> Success. Read all text from file " << g_filename << endl;
        cout << "> Length: " << content.size() << endl;

        vector<string> contentLines;
        cout << "ItsFile::ReadAllTextLines(str)" << endl;
        if (!ItsFile::ReadTextAllLines(g_filename, contentLines)) {
            cout << "> FAILED: " << ItsError::GetLastErrorDescription() << endl;
            cout << endl;
            return;
        }
        cout << "> Success. Read all text lines from file " << g_filename << endl;
        cout << "> Line#: " << contentLines.size() << endl;
        
        cout << "ItsFile::Copy(g_filename, g_shredFilename, true)" << endl;
        if (!ItsFile::Copy(g_filename, g_shredFilename, true)) {
            cout << "> FAILED: " << ItsError::GetLastErrorDescription() << endl;
            cout << endl;
            return;
        }
        cout << "> Success. File " << g_filename << " successfully copied to " << g_shredFilename << endl;
        
        cout << "ItsFile::Copy(g_filename, g_copyToFilename, true)" << endl;
        if (!ItsFile::Copy(g_filename, g_copyToFilename, true)) {
            cout << "> FAILED: " << ItsError::GetLastErrorDescription() << endl;
            cout << endl;
            return;
        }
        cout << "> Success. File " << g_filename << " successfully copied to " << g_copyToFilename << endl;
        
        cout << "ItsFile::Exists(g_copyToFilename)" << endl;
        if (!ItsFile::Exists(g_copyToFilename)) {
            cout << "> File " << g_copyToFilename << " does not exist" << endl;
        }
        else {
            cout << "> File " << g_copyToFilename << " exists" << endl;
        }

        cout << "ItsFile::Delete(g_copyToFilename)" << endl;
        if (!ItsFile::Delete(g_copyToFilename)) {
            cout << "> FAILED: " << ItsError::GetLastErrorDescription() << endl;
            cout << endl;
            return;
        }
        cout << "> Success. File " << g_copyToFilename << " deleted" << endl;

        cout << "ItsFile::Shred(g_shredFilename, false)" << endl;
        if (!ItsFile::Shred(g_shredFilename, false)) {
            cout << "> FAILED: " << ItsError::GetLastErrorDescription() << endl;
            cout << endl;
            return;
        }
        cout << "> Success. File " << g_shredFilename << " shreded" << endl;


        cout << endl;
    }

    //
    // Function: TestStartTimer
    //
    // (i): Test of ItsTimer.
    //
    void TestItsTimerStart()
    {
        PrintHeader("ItsTimer::Start");

        g_timer.Start();
        cout << "Timer started..." << endl;

        cout << endl;
    }

    //
    // Function: TestStopTimer
    //
    // (i): Test of ItsTimer.
    //
    void TestItsTimerStop()
    {
        PrintHeader("ItsTimer::Stop");

        g_timer.Stop();
        cout << "Elapsed Time: " << ItsTime::RenderMsToFullString(g_timer.GetMilliseconds(),true) << endl;

        cout << endl;
    }

    //
    // Function: TestDateTime
    //
    // (i): Test ItsDateTime
    //
    void TestItsDateTime()
    {
        PrintHeader("ItsDateTime");

        auto now = ItsDateTime::Now();
        cout << "ItsDateTime.Now(): " << now.ToString() << endl;

        now.AddDays(7);
        now.AddHours(1);
        now.AddMinutes(1);
        now.AddSeconds(1);
        cout << "Added Days(7), Hours(1), Minutes(1) and Seconds(1): " << now.ToString() << endl;

        now.SubtractDays(7);
        now.SubtractHours(1);
        now.SubtractMinutes(1);
        now.SubtractSeconds(1);
        cout << "Subtracted Days(7), Hours(1), Minutes(1) and Seconds(1): " << now.ToString() << endl;

        cout << "Custom: " << now.ToString("dd.MM.yyyy HH:mm:ss") << endl;

        cout << endl;
    }

    //
    // Function: TestID
    //
    // (i): Tests ItsID.
    //
    void TestItsID()
    {
        PrintHeader("ItsID");

        cout << "ItsID::CreateID(12, ItsCreateIDOptions::LowerAndUpperCase, false)" << endl;
        cout << R"(> ")" << ItsID::CreateID(12, ItsCreateIDOptions::LowerAndUpperCase, false) << R"(")" << endl;
        cout << "ItsID::CreateID(10, ItsCreateIDOptions::LowerAndUpperCase, true)" << endl;
        cout << R"(> ")" << ItsID::CreateID(10, ItsCreateIDOptions::LowerAndUpperCase, true) << R"(")" << endl;
        cout << "ItsID::CreateID(8, ItsCreateIDOptions::UpperCase, false)" << endl;
        cout << R"(> ")" << ItsID::CreateID(8, ItsCreateIDOptions::UpperCase, false) << R"(")" << endl;
        cout << "ItsID::CreateID(6, ItsCreateIDOptions::UpperCase, true)" << endl;
        cout << R"(> ")" << ItsID::CreateID(6, ItsCreateIDOptions::UpperCase, true) << R"(")" << endl;
        cout << "ItsID::CreateID(14, ItsCreateIDOptions::LowerCase, false)" << endl;
        cout << R"(> ")" << ItsID::CreateID(14, ItsCreateIDOptions::LowerCase, false) << R"(")" << endl;
        cout << "ItsID::CreateID(16, ItsCreateIDOptions::LowerCase, true)" << endl;
        cout << R"(> ")" << ItsID::CreateID(16, ItsCreateIDOptions::LowerCase, true) << R"(")" << endl;
        cout << "ItsID::CreateID(64, ItsCreateIDOptions::LowerAndUpperCase, false)" << endl;
        cout << R"(> ")" << ItsID::CreateID(64, ItsCreateIDOptions::LowerAndUpperCase, false) << R"(")" << endl;
        cout << "ItsID::CreateID(64, ItsCreateIDOptions::LowerAndUpperCase, true)" << endl;
        cout << R"(> ")" << ItsID::CreateID(64, ItsCreateIDOptions::LowerAndUpperCase, true) << R"(")" << endl;

        cout << endl;
    }

    //
    // Function: TestItsGuid
    //
    // (i): Tests ItsGuid.
    //
    void TestItsGuid()
    {
        PrintHeader("ItsGuid");

        cout << "ItsGuid::CreateGuid()" << endl;
        cout << R"(> ")" << ItsGuid::CreateGuid() << R"(")" << endl;
        
        uuid_t guid{ 0 };
        cout << "ItsGuid::CreateGuid(guid)" << endl;
        ItsGuid::CreateGuid(guid);
        {
            cout << "> Success" << endl;
            
            cout << "ItsGuid::ToString(guid,ItsGuidFormat::MicrosoftRegistryFormat, true)" << endl;
            cout << R"(> ")" << ItsGuid::ToString(guid, ItsGuidFormat::MicrosoftRegistryFormat, true) << R"(")" << endl;
            cout << "ItsGuid::ToString(guid, ItsGuidFormat::MicrosoftRegistryFormatStripped, true)" << endl;
            cout << R"(> ")" << ItsGuid::ToString(guid, ItsGuidFormat::MicrosoftRegistryFormatStripped, true) << R"(")" << endl;
            cout << "ItsGuid::ToString(guid,ItsGuidFormat::MicrosoftConstFormat, true)" << endl;
            cout << R"(> ")" << ItsGuid::ToString(guid, ItsGuidFormat::MicrosoftConstFormat, true) << R"(")" << endl;
            cout << "ItsGuid::ToString(guid,ItsGuidFormat::MicrosoftCompactFormat, true)" << endl;
            cout << R"(> ")" << ItsGuid::ToString(guid, ItsGuidFormat::MicrosoftCompactFormat, true) << R"(")" << endl;
            cout << "ItsGuid::ToString(guid,ItsGuidFormat::MicrosoftPrefixedCompactFormat, true)" << endl;
            cout << R"(> ")" << ItsGuid::ToString(guid, ItsGuidFormat::MicrosoftPrefixedCompactFormat, true) << R"(")" << endl;
        }

        cout << endl;
    }

    //
    // Function: TestItsPath
    //
    // (i): Test ItsPath
    //
    void TestItsPath()
    {
        PrintHeader("ItsPath");

        string path = ItsPath::Combine(g_path1, g_path2);
        cout << R"(ItsPath::Exists(path))" << endl;
        if (ItsPath::Exists(path)) {
            cout << "> Path: " << path << " exists" << endl;
        }
        else {
            cout << "> Path: " << path << " does not exist" << endl;
        }

        cout << R"(ItsPath::GetDirectory(path))" << endl; 
        cout << R"(> ")" << ItsPath::GetDirectory(path) << R"(")" << endl;
        cout << R"(ItsPath::GetFilename(path))" << endl; 
        cout << R"(> ")" << ItsPath::GetFilename(path) << R"(")" << endl;
        cout << R"(ItsPath::GetExtension(path))" << endl; 
        cout << R"(> ")" << ItsPath::GetExtension(path) << R"(")" << endl;
        cout << R"(ItsPath::HasExtension(path, ".txt"))" << endl; 
        cout << R"(> )" << ((ItsPath::HasExtension(path, ".txt")) ? "true" : "false") << endl;
        cout << R"(ItsPath::HasExtension(path, ".js"))" << endl; 
        cout << R"(> )" << ((ItsPath::HasExtension(path, ".js")) ? "true" : "false") << endl;
        cout << R"(ItsPath::ChangeExtension(path,".js"))" << endl; 
        cout << R"(> ")" << ItsPath::ChangeExtension(path, ".js") << endl;
        cout << R"(ItsPath::IsPathValid(path))" << endl; 
        cout << R"(> )" << ((ItsPath::IsPathValid(path)) ? "true" : "false") << endl;
        cout << R"(ItsPath::IsPathValid(g_invalidPath))" << endl; 
        cout << R"(> )" << ((ItsPath::IsPathValid(g_invalidPath)) ? "true" : "false") << endl;
        cout << R"(ItsPath::GetParentDirectory("path"))" << endl;
        cout << R"(> )" << ItsPath::GetParentDirectory(path) << endl;

        cout << endl;
    }

    //
    // Function: TestItsDirectory
    //
    // (i): Tests ItsDirectory.
    //
    void TestItsDirectory()
    {
        PrintHeader("ItsDirectory");

        cout << R"(ItsDirectory::Exists(g_directoryRoot))" << endl;
        if (!ItsDirectory::Exists(g_directoryRoot)) {
            cout << "> NOT EXISTS" << endl;
        }
        else {
            cout << "> Directory " << g_directoryRoot << " EXISTS" << endl;
        }

        cout << R"(ItsDirectory::GetDirectories(g_directoryRoot))" << endl;
        auto result = ItsDirectory::GetDirectories(g_directoryRoot);
        if (result.size() > 0) {
            cout << "> Success. Found " << result.size() << " sub-directories under " << g_directoryRoot << endl;
            for (auto r : result) {
                cout << ">> " << r << endl;
            }
        }
        else {
            cout << "> FAILED. No directories found under " << g_directoryRoot << endl;
        }

        cout << R"(ItsDirectory::GetFiles(g_directoryRoot))" << endl;
        auto result2 = ItsDirectory::GetFiles(g_directoryRoot);
        if (result2.size() > 0) {
            cout << "> Success. Found " << result2.size() << " files under " << g_directoryRoot << endl;
            for (auto r : result2) {
                cout << ">> " << r << endl;
            }
        }

        auto cdir = g_creatDir;
        cout << R"(ItsDirectory::CreateDirectory(cdir))" << endl;
        bool bResult = ItsDirectory::CreateDirectory(cdir, ItsFile::CreateMode("rw","rw","rw"));
        if (!bResult) {
            cout << "> FAILED. Error: " << ItsError::GetLastErrorDescription() << endl;
            cout << endl;
            return;
        }
        cout << "> Success creating " << cdir << endl;

        cout << R"(ItsDirectory::RemoveDirectory(cdir))" << endl;
        bResult = ItsDirectory::RemoveDirectory(cdir);
        if (!bResult) {
            cout << "> FAILED: " << ItsError::GetLastErrorDescription() << endl;;
            cout << endl;
            return;
        }
        cout << "> Success removing directory " << cdir << endl;

        cout << endl;
    }

    //
    // Function: TestItsFileMonitor
    //
    // (i): Tests ItsFileMonitor. 
    //
    void TestItsFileMonitorStart()
    {
        PrintHeader("ItsFileMonitor Start");

        g_fm = make_unique<ItsFileMonitor>(g_directoryRoot, (ItsFileMonitorMask::Modify | ItsFileMonitorMask::Open | ItsFileMonitorMask::Access | ItsFileMonitorMask::Create | ItsFileMonitorMask::CloseWrite | ItsFileMonitorMask::CloseNoWrite), HandleFileEvent);  
        if (g_fm->GetInitWithError()) {
            cout << "ItsFileMonitor error on init: " << strerror(g_fm->GetInitWithErrorErrno()) << endl;
            cout << endl;
            return;
        }

        cout << "File monitor monitoring directory '" << g_directoryRoot << "' with mask 'ItsFileMonitorMask::Modify,Open,Access,Create,CloseWrite,CloseNoWrite'" << endl;
        
        cout << endl;
    }

    //
    // Function: TestItsFileMonitor
    //
    // (i): Tests ItsFileMonitor. 
    //
    void TestItsFileMonitorStop()
    {
        PrintHeader("ItsFileMonitor Stop");

        if ( !g_fm->GetInitWithError() ) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            g_fm->Stop();

            cout << "File monitor monitoring directory '" << g_directoryRoot << "' with mask 'ItsFileMonitorMask::Modify,Open,Access,Create,CloseWrite,CloseNoWrite'" << endl;
            cout << "Events:" << endl;
            for ( auto i : g_fileMonNames ) {
                cout << ">> " << i << endl;
            }
        }

        cout << endl;
    }

    //
    // Function: HandleFileEvent
    //
    // (i): handle file event.
    //
    void HandleFileEvent(inotify_event& event)
    {
        stringstream ss;
        ss << "Name: " << ((event.len != 0) ? event.name : ".") << ", Mask: ";
        if ( event.mask & IN_ISDIR ) {
            ss << "[IN_ISDIR] ";
        }
        if (event.mask & IN_ACCESS) {
            ss << "[ItsFileMonitorMask::Access] ";
        }
        if (event.mask & IN_ATTRIB) {
            ss << "[ItsFileMonitorMask::Attrib] ";
        }
        if (event.mask & IN_CLOSE_WRITE) {
            ss << "[ItsFileMonitorMask::CloseWrite] ";
        }
        if (event.mask & IN_CLOSE_NOWRITE) {
            ss << "[ItsFileMonitorMask::CloseNoWrite] ";
        }
        if (event.mask & IN_CREATE) {
            ss << "[ItsFileMonitorMask::Create] ";
        }
        if (event.mask & IN_DELETE) {
            ss << "[ItsFileMonitorMask::Delete] ";
        }
        if (event.mask & IN_DELETE_SELF) {
            ss << "[ItsFileMonitorMask::DeleteSelf] ";
        }
        if (event.mask & IN_MODIFY) {
            ss << "[ItsFileMonitorMask::Modify] ";
        }
        if (event.mask & IN_MOVE_SELF) {
            ss << "[ItsFileMonitorMask::MoveSelf] ";
        }
        if (event.mask & IN_MOVED_FROM) {
            ss << "[ItsFileMonitorMask::MovedFrom] ";
        }
        if (event.mask & IN_MOVED_TO) {
            ss << "[ItsFileMonitorMask::MovedTo] ";
        }
        if (event.mask & IN_OPEN) {
            ss << "[ItsFileMonitorMask::Open] ";
        }

        g_fileMonNames.push_back(ss.str());
    }

    //
    // Function: TestItsSocketStreamClientServerStart
    //
    // (i): Starts testing socket stream client server.
    //
    void TestItsSocketStreamClientServerStart()
    {
        PrintHeader("ItsSocketStream[Client/Server] Start");

        g_addr.sun_family = AF_UNIX;
        
        g_socket_stream_server = make_unique<ItsSocketStreamServer>(ItsSocketDomain::UNIX, reinterpret_cast<sockaddr*>(&g_addr), sizeof(g_addr), ItsSocketStreamServer::DefaultBackdrop, true);
        if ( g_socket_stream_server->GetInitWithError()) {
            cout << "ItsSocketStreamServer, Init with error: " << strerror(g_socket_stream_server->GetInitWithErrorErrno()) << endl;
        }
        cout << "ItsSocketStreamServer, Init Ok!" << endl;

        g_socket_stream_client = make_unique<ItsSocketStreamClient>(ItsSocketDomain::UNIX, reinterpret_cast<sockaddr*>(&g_addr), sizeof(g_addr));
        if ( g_socket_stream_client->GetInitWithError()) {
            cout << "ItsSocketStreamClient, Init with error: " << strerror(g_socket_stream_client->GetInitWithErrorErrno()) << endl;
        }
        cout << "ItsSocketStreamClient, Init Ok!" << endl;

        if ( g_socket_stream_server->GetInitWithError() || g_socket_stream_client->GetInitWithError() ) {
            return;
        }

        g_socket_stream_thread1 = make_unique<thread>([] () {
            struct sockaddr_un accept_addr{0};
            socklen_t accept_addr_len(0);
            char buf[1000];
            bool quit = false;
            while (!quit) {
                auto fd = g_socket_stream_server->Accept(reinterpret_cast<sockaddr*>(&accept_addr),&accept_addr_len);
                if ( fd >= 0 ) {
                    g_socket_stream_traffic.push_back("g_socket_stream_server.Accept OK");
                    
                    auto nr = g_socket_stream_server->Read(fd, buf, 1000);
                    g_socket_stream_traffic.push_back("g_socket_stream_server.Read " + to_string(nr) + " bytes, " + buf);
                    
                    strcpy(buf, "This is a response ECHO ONE!");
                    auto nw = g_socket_stream_server->Write(fd, buf, strlen(buf)+1);
                    g_socket_stream_traffic.push_back("g_socket_stream_server.Write " + to_string(nw) + " bytes, " + buf);
                    quit = true;
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(200));
            }
        });
        g_socket_stream_thread2 = make_unique<thread>([] () {   
            std::this_thread::sleep_for(std::chrono::seconds(1));

            if (g_socket_stream_client->Connect() == 0) {
                g_socket_stream_traffic.push_back("g_socket_stream_client.Connect OK");

                char buf[1000] = "This is a test string.";
                auto nw = g_socket_stream_client->Write(buf, strlen(buf)+1);
                g_socket_stream_traffic.push_back("g_socket_stream_client.Write " + to_string(nw) + " bytes, " + buf);
                auto nr = g_socket_stream_client->Read(buf, 1000);
                g_socket_stream_traffic.push_back("g_socket_stream_client.Read " + to_string(nr) + " bytes, " + buf);
            
                std::this_thread::sleep_for(std::chrono::seconds(1));
            }
            else {
                g_socket_stream_traffic.push_back("ItsSocketStreamClient, failed to connect.");
            }
        });
    }

    //
    // Function: TestItsSocketStreamClientServerStop
    //
    // (i): Stop testing socket stream client server.
    //
    void TestItsSocketStreamClientServerStop()
    {
        PrintHeader("ItsSocketStream[Client/Server] Stop");

        for ( auto s : g_socket_stream_traffic) {
            cout << s << endl;
        }

        cout << "Closing net..." << endl;
        g_socket_stream_client->Close();
        g_socket_stream_server->Close();
        cout << "... net closed" << endl;

        if ( g_socket_stream_thread1 != nullptr ) {
            cout << "Thread joining..." << endl;
            g_socket_stream_thread1->join();
            g_socket_stream_thread2->join();
            cout << "... threads joined" << endl;
        }
    }

    //
    // Function: TestItsSocketDatagramClientServerStart
    //
    // (i): Starts testing socket datagram client server.
    //
    void TestItsSocketDatagramClientServerStart()
    {
        PrintHeader("ItsSocketDatagram[Client/Server] Start");
        
        auto pid = getpid();

        const char SV_SOCK_PATH[] = "/tmp/its-server";
        g_saddr.sun_family = AF_UNIX;
        snprintf(g_saddr.sun_path, sizeof(g_saddr.sun_path), "%s", SV_SOCK_PATH);
        remove(g_saddr.sun_path);

        const char CL_SOCK_PATH[] = "/tmp/its-client";
        g_caddr.sun_family = AF_UNIX;
        snprintf(g_caddr.sun_path, sizeof(g_caddr.sun_path), "%s.%i", CL_SOCK_PATH, pid);
        remove(g_caddr.sun_path);
        
        g_socket_dg_server = make_unique<ItsSocketDatagramServer>(ItsSocketDomain::UNIX, reinterpret_cast<sockaddr*>(&g_saddr), sizeof(g_saddr), true);
        if ( g_socket_dg_server->GetInitWithError()) {
            cout << "ItsSocketDatagramServer, Init with error: " << strerror(g_socket_dg_server->GetInitWithErrorErrno()) << endl;
        }
        else {
            cout << "ItsSocketDatagramServer, Init Ok!" << endl;
        }

        g_socket_dg_client = make_unique<ItsSocketDatagramClient>(ItsSocketDomain::UNIX, reinterpret_cast<sockaddr*>(&g_caddr), sizeof(g_caddr), true);
        if ( g_socket_dg_client->GetInitWithError()) {
            cout << "ItsSocketDatagramClient, Init with error: " << strerror(g_socket_dg_client->GetInitWithErrorErrno()) << endl;
        }
        else {
            cout << "ItsSocketDatagramClient, Init Ok!" << endl;
        }

        if ( g_socket_dg_server->GetInitWithError() || g_socket_dg_client->GetInitWithError() ) {
            return;
        }

        g_socket_dg_thread1 = make_unique<thread>([] () {
            struct sockaddr_un accept_addr{0};
            socklen_t accept_addr_len(0);
            char buf[1000];
            bool quit = false;
            while (!quit) {
                auto nr = g_socket_dg_server->RecvFrom(buf, 1000, 0, reinterpret_cast<sockaddr*>(&accept_addr),&accept_addr_len);
                if ( nr > 0 ) {
                    g_socket_dg_traffic.push_back("g_socket_dg_server.RecvFrom " + to_string(nr) + " bytes, " + buf);
                
                    strcpy(buf, "This is a response ECHO ONE!");
                    auto nw = g_socket_dg_server->SendTo(buf, strlen(buf)+1, 0, reinterpret_cast<sockaddr*>(&g_caddr), sizeof(g_caddr));
                    g_socket_dg_traffic.push_back("g_socket_dg_server.SendTo " + to_string(nw) + " bytes, " + buf);
                    
                    quit = true;
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(200));
            }
        });

        g_socket_dg_thread2 = make_unique<thread>([] () {   
            std::this_thread::sleep_for(std::chrono::seconds(1));

            struct sockaddr_un accept_addr{0};
            socklen_t accept_addr_len(0);

            char buf[1000] = "This is a test string.";
            auto nw = g_socket_dg_client->SendTo(buf, strlen(buf)+1, 0, reinterpret_cast<sockaddr*>(&g_saddr), sizeof(g_saddr));
            g_socket_dg_traffic.push_back("g_socket_dg_client.SendTo " + to_string(nw) + " bytes, " + buf);

            std::this_thread::sleep_for(std::chrono::milliseconds(200));

            bool quit = false;
            while (!quit)
            { 
                auto nr = g_socket_dg_client->RecvFrom(buf, 1000, 0, reinterpret_cast<sockaddr*>(&accept_addr),&accept_addr_len);
                if ( nr > 0 ) {
                    g_socket_dg_traffic.push_back("g_socket_dg_client.RecvFrom " + to_string(nr) + " bytes, " + buf);
                    quit = true;
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(200));
            }
        });
    }

    //
    // Function: TestItsSocketDatagramClientServerStop
    //
    // (i): Stop testing socket datagram client server.
    //
    void TestItsSocketDatagramClientServerStop()
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1200));

        PrintHeader("ItsSocketDatagram[Client/Server] Stop");

        for ( auto s : g_socket_dg_traffic) {
            cout << s << endl;
        }

        cout << "Closing net..." << endl;
        g_socket_dg_client->Close();
        g_socket_dg_server->Close();
        cout << "... net closed" << endl;

        if ( g_socket_dg_thread1 != nullptr ) {
            cout << "Thread joining..." << endl;
            g_socket_dg_thread1->join();
            g_socket_dg_thread2->join();
            cout << "... threads joined" << endl;
        }

        remove(g_saddr.sun_path);
        remove(g_caddr.sun_path);
    }

    //
    // Function: TestItsPipe
    //
    // (i): Test ItsPipe
    //
    void TestItsPipe()
    {
        PrintHeader("ItsPipe");

        ItsPipe pipe;

        if ( pipe.GetInitWithError() ) {
            cout << "ItsPipe, Init with error: " << strerror(pipe.GetInitWithErrorErrno()) << endl;
            return;
        }
        cout << "ItsPipe, Init Ok" << endl;

        cout << "ItsPipe, fork() called" << endl;
        switch(fork()) {
            case -1:
            {
                cout << "ItsPipe fork call failed with error: " << strerror(errno) << endl;
                break;
            }
            case 0:
            {
                // child
                pipe.CloseRead();
                
                char wbuf[ItsPipe::MaxBufferSize] = "This is a testing message!";
                auto nw = pipe.Write(wbuf, strlen(wbuf)+1);
                if ( nw < 0 ) {
                    cout << "ItsPipe, Child::Write Error, " << strerror(errno) << endl;
                }
                else {
                    cout << "ItsPipe, Child::Wrote " << to_string(nw) << " bytes, " << wbuf << endl;
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(2000));
                pipe.Close();
                _exit(0);
                break;
            }
            default:
            {
                // parent
                pipe.CloseWrite();

                std::this_thread::sleep_for(std::chrono::milliseconds(1200));

                char rbuf[ItsPipe::MaxBufferSize];
                auto nr = pipe.Read(rbuf, ItsPipe::MaxBufferSize);
                if ( nr < 0 ) {
                    cout << "ItsPipe, Parent::Read Error, " << strerror(errno) << endl;
                }
                else {
                    cout << "ItsPipe, Parent::Read " << to_string(nr) << " bytes, " << rbuf << endl;
                }

                pipe.Close();
            }
        }
    }

    //
    // Function: TesTItsSvMsgQueue
    //
    // (i): Tests ItsSvMsgQueue.
    //
    void TestItsSvMsgQueue()
    {
        PrintHeader("ItsSvMessageQueue");
        
        ItsSvMsgQueue queue(IPC_PRIVATE, ItsSvMsgQueue::CreateQueueFlags(true, false, "rw", "rw", "rw"));
        if ( queue.GetInitWithError() ) {
            cout << "ItsSvMsgQueue, Init with error: " << strerror(queue.GetInitWithErrorErrno()) << endl;
            return;
        }
        cout << "ItsSvMsgQueue, Init Ok with id: " << to_string(queue.GetMessageQueueId()) << endl;

        cout << "ItsSvMsgQueue, fork() called" << endl;
        switch(fork()) {
            case -1:
            {
                cout << "ItsSvMsgQueue fork call failed with error: " << strerror(errno) << endl;
                break;
            }
            case 0:
            {
                // child
                ItsSvMsg1k tmp{0};
                tmp.mtype = 1;
                strcpy(tmp.mtext, "This is a test message queue message!");

                auto nw = queue.MsgSnd(&tmp, sizeof(tmp), ItsSvMsgQueue::CreateMsgFlags(true,false,false,false));
                if ( nw == 0 ) {
                    cout << "ItsSvMsgQueue, Child::MsgSnd Ok: " << tmp.mtext << endl;
                }
                else {
                    cout << "ItsSvMsgQueue, Child::MsgSnd with error: " << strerror(errno) << endl;
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(2000));
                _exit(0);
                break;
            }
            default:
            {
                // parent
                ItsSvMsg1k tmp{0};

                std::this_thread::sleep_for(std::chrono::milliseconds(1200));

                auto nr = queue.MsgRcv(&tmp, sizeof(tmp), 0, ItsSvMsgQueue::CreateMsgFlags(true,false,false,false));
                if ( nr < 0 ) {
                    cout << "ItsSvMsgQueue, Parent::MsgRcv with error: " << strerror(errno) << endl;
                }
                else {
                    cout << "ItsSvMsgQueue, Parent::MsgRcv Ok: " << to_string(nr) << " bytes: " << tmp.mtext << endl;
                }

                if ( queue.Delete() == 0 ) {
                    cout << "ItsSvMessageQueue, Parent::Delete Ok" << endl;
                }
                else {
                    cout << "ItsSvMessageQueue, Parent::Delete with error: " << strerror(errno) << endl;
                }
            }
        }
    }

    //
    // Function: TestItsFifo
    //
    // (i): Tests ItsFifo.
    //
    void TestItsFifo()
    {
        PrintHeader("ItsFifo");

        ItsFifoServer fifoServer("/tmp/its-fifo-src", "/tmp/its-fifo_cl", ItsFifoServer::CreateFifoFlags("rw","rw","rw"));
        if ( fifoServer.GetInitWithError() ) {
            cout << "ItsFifoServer, Init with error: " << strerror(fifoServer.GetInitWithErrorErrno()) << endl;
            return;
        }
        cout << "ItsFifoServer, Init Ok" << endl;

        cout << "ItsFifo[Server/Client], fork() called" << endl;
        switch(fork()) {
            case -1:
            {
                cout << "ItsSvMsgQueue fork call failed with error: " << strerror(errno) << endl;
                break;
            }
            case 0:
            {
                ItsFifoClient fifoClient("/tmp/its-fifo-src", "/tmp/its-fifo_cl", ItsFifoClient::CreateFifoFlags("rw","rw","rw"));
                if ( fifoClient.GetInitWithError() ) {
                    cout << "ItsFifoClient, Init with error: " << strerror(fifoClient.GetInitWithErrorErrno()) << endl;
                    return;
                }
                cout << "ItsFifoClient, Init Ok" << endl;

                // child
                char buf[] = "This is a sample message for fifo transmission!";
                ItsFifoHeader tmp{0};
                tmp.length = strlen(buf)+1;
                tmp.pid = getpid();
                
                auto nw = fifoClient.Write(reinterpret_cast<void*>(buf), &tmp);
                if ( nw > 0 ) {
                    cout << "ItsFifoClient, Child::Write Ok: pid=" << to_string(tmp.pid) << ", content=" << buf << endl;
                }
                else if (nw == 0)
                {
                    cout << "ItsFifoClient, Child::Write with return 0" << endl;
                }
                else {
                    cout << "ItsFifoClient, Child::Write with error: " << strerror(errno) << endl;
                }

                std::this_thread::sleep_for(std::chrono::milliseconds(2000));
                fifoClient.Close();
                _exit(0);
                break;
            }
            default:
            {
                // parent
                unique_ptr<unsigned char[]> buf;

                ItsFifoHeader tmp{0};
                tmp.length = 0;
                tmp.pid = getpid();
                
                std::this_thread::sleep_for(std::chrono::milliseconds(1200));

                auto nr = fifoServer.Read(&buf, &tmp);
                
                if ( nr < 0 ) {
                    cout << "ItsFifoServer, Parent::Read with error: " << strerror(errno) << endl;
                }
                else if ( nr == 0 ) {
                    cout << "ItsFifoServer, Parent::Read with return 0" << endl;
                }
                else {
                    cout << "ItsFifoServer, Parent::Read Ok: pid=" << to_string(tmp.pid) << ", content=" << buf.get() << endl;
                }

                fifoServer.Close();
            }
        }
    }
}

//
// Function: main
//
// (i): Application entry point.
//      Redirects to ItSoftware::CppIncludeLinux::TestApplication::main.
//
int main(int argc, const char* argv[])
{
    return ItSoftware::CppIncludeLinux::TestApplication::main(argc, argv);
}