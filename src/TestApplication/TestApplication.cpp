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
#include "../include/itsoftware-linux.h"
#include "../include/itsoftware-linux-core.h"

//
// using
//
using std::cout;
using std::endl;
using std::ends;
using std::string;
using std::stringstream;
using ItSoftware::Linux::ItsString;
using ItSoftware::Linux::ItsExpandDirection;
using ItSoftware::Linux::ItsTime;
using ItSoftware::Linux::ItsConvert;
using ItSoftware::Linux::ItsRandom;
using ItSoftware::Linux::ItsDateTime;
using ItSoftware::Linux::ItsLog;
using ItSoftware::Linux::ItsDataSizeStringType;
using ItSoftware::Linux::ItsDateTime;
using ItSoftware::Linux::ItsID;
using ItSoftware::Linux::ItsCreateIDOptions;
using ItSoftware::Linux::Core::ItsTimer;
using ItSoftware::Linux::Core::ItsFile;

//
// Function Prototypes
//
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
void ExitFn();
void PrintTestHeader(string txt);

//
// global data
//
ItsTimer g_timer;
char g_filename[] = "/home/kjetilso/test.txt";
char g_copyToFilename[] = "/home/kjetilso/test2.txt";

//
// Function: ExitFn
//
// (i): Print exit message.
// 
void ExitFn()
{
    cout << endl;
    cout << "> Test Application - Exited <" << endl;
}

//
// Function: main
//
// (i): Application entry point
//
int main(int argc, char* argv[])
{
    atexit(ExitFn);

    cout << "> Test Application - Started <" << endl;

	TestItsTimerStart();
    TestItsConvert();
    TestItsRandom();
    TestItsTime();
    TestItsString();
	TestItsLog();
	TestItsFile();
    TestItsDateTime();
    TestItsID();
	TestItsTimerStop();

    return EXIT_SUCCESS;
}

//
// Function: PrintTestHeader
//
// (i): Prints a tests header.
//
void PrintTestHeader(string txt)
{
    cout << endl;
    cout << ItsString::WidthExpand(txt, 80, '_', ItsExpandDirection::Right) << endl;
}

//
// Function: PrintTestSubHeader
//
// (i): Prints a tests sub header.
//
void PrintTestSubHeader(string txt)
{
    cout << endl;
    cout << "__ " << txt << " __" << endl;
}

//
// Function: TestToNumber
//
// (i): Test numerics as string converted to primitive data types.
//
void TestItsConvert()
{
    PrintTestHeader("## Test ItsConvert ");

    PrintTestSubHeader("ToNumber");
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
    cout << R"(ItsConvert::ToNumber<float>("-12.34"))" << endl;
    cout << "> " << ItsConvert::ToNumber<float>("-12.34") << endl;
    cout << R"(ItsConvert::ToNumber<double>("1.234"))" << endl;
    cout << "> " << ItsConvert::ToNumber<double>("1.234") << endl;
    cout << R"(ItsConvert::ToNumber<short>("1234"))" << endl;
    cout << "> " << ItsConvert::ToNumber<short>("1234") << endl;
    cout << R"(ItsConvert::ToNumber<unsigned short>("40001"))" << endl;
    cout << "> " << ItsConvert::ToNumber<unsigned short>("40001") << endl;

    PrintTestSubHeader("ToString");
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
    cout << R"(ItsConvert::ToString<float>(-12.34))" << endl;
    cout << R"(> ")" << ItsConvert::ToString<float>(-12.34f) << R"(")" << endl;
    cout << R"(ItsConvert::ToString<double>(1.234))" << endl;
    cout << R"(> ")" << ItsConvert::ToString<double>(1.234) << R"(")" << endl;
    cout << R"(ItsConvert::ToString<short>(1234)" << endl;
    cout << R"(> ")" << ItsConvert::ToString<short>(1234) << R"(")" << endl;
    cout << R"(ItsConvert::ToString<unsigned short>(40001)" << endl;
    cout << R"(> ")" << ItsConvert::ToString<unsigned short>(40001) << R"(")" << endl;
    
    PrintTestSubHeader("ToStringFormatted");
    cout << R"(ItsConvert::ToStringFormatted(256810246))" << endl;
    cout << R"(> ")" << ItsConvert::ToStringFormatted(256810246) << R"(")" << endl;
    cout << R"(ItsConvert::ToStringFormatted(256810246, L' '))" << endl;
    cout << R"(> ")" << ItsConvert::ToStringFormatted(256810246, L' ') << R"(")" << endl;
    
    PrintTestSubHeader("ToDataSizeString");
    cout << R"(ItsConvert::ToDataSizeString(1024, 2))" << endl;
    cout << R"(> ")" << ItsConvert::ToDataSizeString(1024, 2) << R"(")" << endl;
    cout << R"(ItsConvert::ToDataSizeString(200100400, 0))" << endl;
    cout << R"(> ")" << ItsConvert::ToDataSizeString(200100400, 0) << R"(")" << endl;
    cout << R"(ItsConvert::ToDataSizeString(200100400, 2))" << endl;
    cout << R"(> ")" << ItsConvert::ToDataSizeString(200100400, 2) << R"(")" << endl;
    cout << R"(ItsConvert::ToDataSizeString(9807804606, 3))" << endl;
    cout << R"(> ")" << ItsConvert::ToDataSizeString(9807804606, 3, ItsDataSizeStringType::IEC) << R"(")" << endl;
    cout << R"(ItsConvert::ToDataSizeString(1000, 3))" << endl;
    cout << R"(> ")" << ItsConvert::ToDataSizeString(1000, 3, ItsDataSizeStringType::IEC) << R"(")" << endl;
    cout << R"(ItsConvert::ToDataSizeString(size_t max, 2))" << endl;
    cout << R"(> ")" << ItsConvert::ToDataSizeString(SIZE_MAX, 2) << R"(")" << endl;
    cout << R"(ItsConvert::ToDataSizeString(size_t max, 2))" << endl;
    cout << R"(> ")" << ItsConvert::ToDataSizeString(SIZE_MAX, 2, ItsDataSizeStringType::IEC) << R"(")" << endl;

    cout << endl;
}

//
// Function: TestRandom
//
// (i): Test random integers and floats.
//
void TestItsRandom()
{
    PrintTestHeader("## Test ItsRandom ");

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
    PrintTestHeader("## Test ItsTime ");

    PrintTestSubHeader("RenderMsToFullString");
    cout << "ItsTime::RenderMsToFullString(92481379, false)" << endl;
    cout << R"(> ")" << ItsTime::RenderMsToFullString(92481379, false) << R"(")" << endl;
    cout << "ItsTime::RenderMsToFullString(92481379, true)" << endl;
    cout << R"(> ")" << ItsTime::RenderMsToFullString(92481379, true) << R"(")" << endl;
    
    PrintTestSubHeader("Now + ToString");
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
    PrintTestHeader("## Test ItsString ");

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
    for (auto s : vs) {
        ss << s << L" ";
    }
    ss << ends;
    cout << R"(> ")" << ss.str() << endl;
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
	PrintTestHeader("## Test ItsLog ");

    ItsLog log;
    log.LogInformation("This is an information log item");
    log.LogWarning("This is an warning log item");
    log.LogError("This is an error log item");
    log.LogOther("This is an other log item");
    log.LogDebug("This is an debug log item");

    PrintTestSubHeader("ToFriendlyString");
    cout << log.ToFriendlyString() << endl;

    PrintTestSubHeader("ToString");
    cout << log.ToString() << endl;
    
    cout << endl;
}

//
// Function: TestFile
//
// (i): Test of ItsFile.
//
void TestItsFile()
{
	PrintTestHeader("## Test ItsFile ");

	ItsFile file;
	if (!file.OpenOrCreate(g_filename,"rwt",ItsFile::CreateMode("rw","rw","rw")))
	{
		cout << "> FAILED: " << strerror(errno) << endl;
		cout << endl;
		return;
	}
    cout << "> Success" << endl;

	char text[] = "Test Line 1\nTest Line 2\n";
	size_t written(0);
	cout << "file.Write((void*)text,strlen(text), &written)" << endl;
    if ( !file.Write((void*)text,strlen(text), &written) )
	{
		cout << "> FAILED: " << strerror(errno) << endl;
		cout << endl;
		return;
	}
    cout << "> Success. " << written << " bytes written to " << g_filename << endl;
    

    string str;
    cout << "file.ReadAllText(str)" << endl;
    if (!file.ReadAllText(str)) {
        cout << "> FAILED: " << strerror(errno) << endl;
        cout << endl;
        return;
    }
    cout << "> Success. Read all text from file " << g_filename << endl;
    

    cout << "file.Close()" << endl;
    if (!file.Close() ) {
        cout << "> FAILED: " << strerror(errno) << endl;;
    }
    cout << "> Success" << endl;


    cout << "ItsFile::Copy(g_filename, g_copyToFilename, true)" << endl;
    if (!ItsFile::Copy(g_filename, g_copyToFilename, true)) {
        cout << "> FAILED: " << strerror(errno) << endl;
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
        cout << "> FAILED: " << strerror(errno) << endl;
        cout << endl;
        return;
    }
    cout << "> Success. File " << g_copyToFilename << " deleted" << endl;

	cout << endl;
}

//
// Function: TestStartTimer
//
// (i): Test of ItsTimer.
//
void TestItsTimerStart()
{
	PrintTestHeader("## Test ItsTimer::Start ");

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
	PrintTestHeader("## Test ItsTimer::Stop ");

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
    PrintTestHeader("## Test ItsDateTime ");

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
    PrintTestHeader("## Test ItsID ");

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