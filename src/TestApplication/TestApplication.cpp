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
using std::string;
using ItSoftware::Linux::ItsString;
using ItSoftware::Linux::ItsExpandDirection;
using ItSoftware::Linux::ItsTime;
using ItSoftware::Linux::ItsConvert;
using ItSoftware::Linux::ItsRandom;
using ItSoftware::Linux::ItsDateTime;
using ItSoftware::Linux::ItsLog;
using ItSoftware::Linux::ItsDataSizeStringType;
using ItSoftware::Linux::ItsDateTime;
using ItSoftware::Linux::Core::ItsTimer;
using ItSoftware::Linux::Core::ItsFile;

//
// Function Prototypes
//
void TestToNumber();
void TestToString();
void TestRandom();
void TestTime();
void TestString();
void TestLog();
void TestStartTimer();
void TestStopTimer();
void TestFile();
void TestDateTime();

//
// global data
//
ItsTimer g_timer;
char g_filename[] = "/home/kjetilso/test.txt";

//
// Function: main
//
// (i): Application entry point
//
int main(int argc, char* argv[])
{
    cout << "### Cpp.Include.Linux - Test Application ###" << endl << endl;

	TestStartTimer();
    TestToNumber();
    TestToString();
    TestRandom();
    TestTime();
    TestString();
	TestLog();
	TestFile();
    TestDateTime();
	TestStopTimer();

    return EXIT_SUCCESS;
}

//
// Function: TestToNumber
//
// (i): Test numerics as string converted to primitive data types.
//
void TestToNumber()
{
    cout << endl;

    cout << "## Test ToNumber ________________________________________________" << endl;
    cout << R"(ItsConvert::ToNumber<int>("-1234") = )" << ItsConvert::ToNumber<int>("-1234") << endl;
    cout << R"(ItsConvert::ToNumber<unsigned int>("1234") = )" << ItsConvert::ToNumber<unsigned int>("1234") << endl;
    cout << R"(ItsConvert::ToNumber<long>("-1234") = )" << ItsConvert::ToNumber<long>("-1234") << endl;
    cout << R"(ItsConvert::ToNumber<unsigned long>("123456789") = )" << ItsConvert::ToNumber<unsigned long>("12346789") << endl;
    cout << R"(ItsConvert::ToNumber<float>("-12.34") = )" << ItsConvert::ToNumber<float>("-12.34") << endl;
    cout << R"(ItsConvert::ToNumber<double>("1.234") = )" << ItsConvert::ToNumber<double>("1.234") << endl;
    cout << R"(ItsConvert::ToNumber<short>("1234") = )" << ItsConvert::ToNumber<short>("1234") << endl;
    cout << R"(ItsConvert::ToNumber<unsigned short>("40001") = )" << ItsConvert::ToNumber<unsigned short>("40001") << endl;

    cout << endl;
}

//
// Function: TestToString
//
// (i): Tests primitive data types to string. I.e. numeric types.
//
void TestToString()
{
    cout << endl;

    cout << "## Test ToString ________________________________________________" << endl;
    cout << R"(ItsConvert::ToString<int>(-1234) = ")" << ItsConvert::ToString<int>(-1234) << R"(")" << endl;
    cout << R"(ItsConvert::ToString<unsigned int>(1234) = ")" << ItsConvert::ToString<unsigned int>(1234) << R"(")" << endl;
    cout << R"(ItsConvert::ToString<long>(-1234) = ")" << ItsConvert::ToString<long>(-1234) << R"(")" << endl;
    cout << R"(ItsConvert::ToString<unsigned long>(123456789) = ")" << ItsConvert::ToString<unsigned long>(12346789) << R"(")" << endl;
    cout << R"(ItsConvert::ToString<float>(-12.34) = ")" << ItsConvert::ToString<float>(-12.34f) << R"(")" << endl;
    cout << R"(ItsConvert::ToString<double>(1.234) = ")" << ItsConvert::ToString<double>(1.234) << R"(")" << endl;
    cout << R"(ItsConvert::ToString<short>(1234) = ")" << ItsConvert::ToString<short>(1234) << R"(")" << endl;
    cout << R"(ItsConvert::ToString<unsigned short>(40001) = ")" << ItsConvert::ToString<unsigned short>(40001) << R"(")" << endl;
    cout << R"(ItsConvert::ToStringFormatted(256810246) = ")" << ItsConvert::ToStringFormatted(256810246) << R"(")" << endl;
	cout << R"(ItsConvert::ToDataSizeString(0, 0) = ")" << ItsConvert::ToDataSizeString(0, 0) << R"(")" << endl;
    cout << R"(ItsConvert::ToDataSizeString(1024, 2) = ")" << ItsConvert::ToDataSizeString(1024, 2) << R"(")" << endl;
	cout << R"(ItsConvert::ToDataSizeString(200100400, 0) = ")" << ItsConvert::ToDataSizeString(200100400, 0) << R"(")" << endl;
	cout << R"(ItsConvert::ToDataSizeString(200100400, 2) = ")" << ItsConvert::ToDataSizeString(200100400, 2) << R"(")" << endl;
	cout << R"(ItsConvert::ToDataSizeString(9807804606, 3) = ")" << ItsConvert::ToDataSizeString(9807804606, 3, ItsDataSizeStringType::IEC) << R"(")" << endl;
    cout << R"(ItsConvert::ToDataSizeString(1000, 3) = ")" << ItsConvert::ToDataSizeString(1000, 3, ItsDataSizeStringType::IEC) << R"(")" << endl;

    cout << endl;
}

//
// Function: TestRandom
//
// (i): Test random integers and floats.
//
void TestRandom()
{
    cout << endl;

    cout << "## Test Random _________________________________________________" << endl;
    cout << "ItsRandom<long>(10'000, 1'000'000) = " << ItsRandom<long>(10'000, 1'000'000) << endl;
    cout << "ItsRandom<long>(10'000, 1'000'000) = " << ItsRandom<long>(10'000, 1'000'000) << endl;
    cout << "ItsRandom<float>(1, 10) = " << ItsRandom<float>(1, 10) << endl;
    cout << "ItsRandom<float>(1, 10) = " << ItsRandom<float>(1, 10) << endl;
    cout << "ItsRandom<double>(1, 2) = " << ItsRandom<double>(1, 2) << endl;
    cout << "ItsRandom<double>(1, 2) = " << ItsRandom<double>(1, 2) << endl;

    cout << endl;
}

//
// Function: TestTime
//
// (i): Test rendering of time from milliseconds.
//
void TestTime()
{
    cout << endl;

    cout << "## Test Time ___________________________________________________" << endl;
    cout << "ItsTime::RenderMsToFullString(92481379, false)" << " = " << ItsTime::RenderMsToFullString(92481379, false) << endl;
    cout << "ItsTime::RenderMsToFullString(92481379, true)" << " = " << ItsTime::RenderMsToFullString(92481379, true) << endl;
    cout << R"(ItsDateTime::Now().ToString("s"))" << " = " << ItsDateTime::Now().ToString("s") << endl;

    cout << endl;
}

//
// Function: TestString
//
// (i): Test string manipulation routines.
//
void TestString()
{
    string test("Ab12Cd");
    string testTrim("  Ab12Cd  ");
    cout << endl;

    cout << "## Test String ________________________________________________" << endl;
    cout << R"(test = ")" << test << R"(")" << endl;
    cout << R"(testTrim = ")" << testTrim << R"(")" << endl;
    cout << R"(ItsString::Left(test,4) = ")" << ItsString::Left(test, 4) << R"(")" << endl;
    cout << R"(ItsString::Right(test,4) = ")" << ItsString::Right(test, 4) << R"(")" << endl;
    cout << R"(ItsString::Mid(test,2,2) = ")" << ItsString::Mid(test, 2, 2) << R"(")" << endl;
    cout << R"(ItsString::ToLowerCase(test) = ")" << ItsString::ToLowerCase(test) << R"(")" << endl;
    cout << R"(ItsString::ToUpperCase(test) = ")" << ItsString::ToUpperCase(test) << R"(")" << endl;
    cout << R"(ItsString::Trim(testTrim) = ")" << ItsString::Trim(testTrim) << R"(")" << endl;

	string name("Kjetil");
    cout << "ItsString::WidthExpand (ItsExpandDirection:Left,Middle,Right) Below" << endl;
    cout << ItsString::WidthExpand(name, 30, '_', ItsExpandDirection::Left) << endl;
    cout << ItsString::WidthExpand(name, 30, '_', ItsExpandDirection::Middle) << endl;
    cout << ItsString::WidthExpand(name, 30, '_', ItsExpandDirection::Right) << endl;

    cout << endl;
}

//
// Function: TestLog
//
// (i): Test ItsLog.
//
void TestLog()
{
	cout << endl;
    cout << "## Test Log _________________________________________________" << endl;

    ItsLog log;
    log.LogInformation("This is an information log item");
    log.LogWarning("This is an warning log item");
    log.LogError("This is an error log item");
    log.LogOther("This is an other log item");
    log.LogDebug("This is an debug log item");
    cout << log.ToString();

    cout << endl;
}

//
// Function: TestFile
//
// (i): Test of ItsFile.
//
void TestFile()
{
	cout << endl;
	cout << "## Test File _______________________________________________" << endl;

	ItsFile file;
	if (!file.OpenOrCreate(g_filename,"rwt",ItsFile::CreateMode("rw","rw","rw")))
	{
		cout << "Error creating: " << g_filename << endl;
		cout << endl;
		return;
	}

	char text[] = "Test Line 1\nTest Line 2\n";
	size_t written(0);
	if ( !file.Write((void*)text,strlen(text), &written) )
	{
		cout << "Error writing: " << g_filename << endl;
		cout << endl;
		return;
	}

	cout << written << " bytes written to file " << g_filename << " successfully" << endl;

	cout << endl;
}

//
// Function: TestStartTimer
//
// (i): Test of ItsTimer.
//
void TestStartTimer()
{
	cout << endl;
	cout << "## Test Start Timer _________________________________________" << endl;

	g_timer.Start();
}

//
// Function: TestStopTimer
//
// (i): Test of ItsTimer.
//
void TestStopTimer()
{
	cout << endl;
	cout << "## Test Stop Timer __________________________________________" << endl;

	g_timer.Stop();
	cout << "Exceution time: " << ItsTime::RenderMsToFullString(g_timer.GetMilliseconds(),true) << endl;

	cout << endl;
}

//
// Function: TestDateTime
//
// (i): Test ItsDateTime
//
void TestDateTime()
{
    cout << endl;
    cout << "## Test DateTime ___________________________________________" << endl;

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

    cout << endl;
}