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

//
// Function Prototypes
//
void TestToNumber();
void TestToString();
void TestRandom();
void TestTime();
void TestString();
void TestLog();

//
// Function: main
//
int main()
{
    cout << "### Cpp.Include.Linux - Test Application ###" << endl << endl;

    TestToNumber();
    TestToString();
    TestRandom();
    TestTime();
    TestString();
	TestLog();

    return EXIT_SUCCESS;
}

//
// Function: TestToNumber
//
// (i) Test numerics as string converted to primitive data types.
//
void TestToNumber()
{
    cout << endl;

    cout << "## Test ToNumber ________________________________________________" << endl;
    cout << "ItsConvert::ToNumber<int>(L\"-1234\") = " << ItsConvert::ToNumber<int>("-1234") << endl;
    cout << "ItsConvert::ToNumber<unsigned int>(L\"1234\") = " << ItsConvert::ToNumber<unsigned int>("1234") << endl;
    cout << "ItsConvert::ToNumber<long>(L\"-1234\") = " << ItsConvert::ToNumber<long>("-1234") << endl;
    cout << "ItsConvert::ToNumber<unsigned long>(L\"123456789\") = " << ItsConvert::ToNumber<unsigned long>("12346789") << endl;
    cout << "ItsConvert::ToNumber<float>(L\"-12.34\") = " << ItsConvert::ToNumber<float>("-12.34") << endl;
    cout << "ItsConvert::ToNumber<double>(L\"1.234\") = " << ItsConvert::ToNumber<double>("1.234") << endl;
    cout << "ItsConvert::ToNumber<short>(L\"1234\") = " << ItsConvert::ToNumber<short>("1234") << endl;
    cout << "ItsConvert::ToNumber<unsigned short>(L\"40001\") = " << ItsConvert::ToNumber<unsigned short>("40001") << endl;

    cout << endl;
}

//
// Function: TestToString
//
// (i) Tests primitive data types to string. I.e. numeric types.
//
void TestToString()
{
    cout << endl;

    cout << "## Test ToString ________________________________________________" << endl;
    cout << "ItsConvert::ToString<int>(-1234) = \"" << ItsConvert::ToString<int>(-1234) << "\"" << endl;
    cout << "ItsConvert::ToString<unsigned int>(1234) = \"" << ItsConvert::ToString<unsigned int>(1234) << "\"" << endl;
    cout << "ItsConvert::ToString<long>(-1234) = \"" << ItsConvert::ToString<long>(-1234) << "\"" << endl;
    cout << "ItsConvert::ToString<unsigned long>(123456789) = \"" << ItsConvert::ToString<unsigned long>(12346789) << "\"" << endl;
    cout << "ItsConvert::ToString<float>(-12.34) = \"" << ItsConvert::ToString<float>(-12.34f) << "\"" << endl;
    cout << "ItsConvert::ToString<double>(1.234) = \"" << ItsConvert::ToString<double>(1.234) << "\"" << endl;
    cout << "ItsConvert::ToString<short>(1234) = \"" << ItsConvert::ToString<short>(1234) << "\"" << endl;
    cout << "ItsConvert::ToString<unsigned short>(40001) = \"" << ItsConvert::ToString<unsigned short>(40001) << "\"" << endl;
    cout << "ItsConvert::ToStringFormatted(256810246) = \"" << ItsConvert::ToStringFormatted(256810246) << "\"" << endl;
	cout << "ItsConvert::ToDataSizeString(0, 0) = \"" << ItsConvert::ToDataSizeString(0, 0) << "\"" << endl;
    cout << "ItsConvert::ToDataSizeString(1024, 2) = \"" << ItsConvert::ToDataSizeString(1024, 2) << "\"" << endl;
	cout << "ItsConvert::ToDataSizeString(200100400, 0) = \"" << ItsConvert::ToDataSizeString(200100400, 0) << "\"" << endl;
	cout << "ItsConvert::ToDataSizeString(200100400, 2) = \"" << ItsConvert::ToDataSizeString(200100400, 2) << "\"" << endl;

    cout << endl;
}

//
// Function: TestRandom
//
// (i) Test random integers and floats.
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
// (i) Test rendering of time in milliseconds.
//
void TestTime()
{
    cout << endl;

    cout << "## Test Time ___________________________________________________" << endl;
    cout << "ItsTime::RenderMsToFullString(92481379, false)" << " = " << ItsTime::RenderMsToFullString(92481379, false) << endl;
    cout << "ItsTime::RenderMsToFullString(92481379, true)" << " = " << ItsTime::RenderMsToFullString(92481379, true) << endl;
    cout << "ItsDateTime::Now().ToString(\"s\")" << " = " << ItsDateTime::Now().ToString() << endl;

    cout << endl;
}

//
// Function: TestString
//
// (i) Test string manipulation routines.
//
void TestString()
{
    string test("Ab12Cd");
    string testTrim("  Ab12Cd  ");
    cout << endl;

    cout << "## Test String ________________________________________________" << endl;
    cout << "test = \"" << test << "\"" << endl;
    cout << "testTrim = \"" << testTrim << "\"" << endl;
    cout << "ItsString::Left(test,4) = \"" << ItsString::Left(test, 4) << "\"" << endl;
    cout << "ItsString::Right(test,4) = \"" << ItsString::Right(test, 4) << "\"" << endl;
    cout << "ItsString::Mid(test,2,2) = \"" << ItsString::Mid(test, 2, 2) << "\"" << endl;
    cout << "ItsString::ToLowerCase(test) = \"" << ItsString::ToLowerCase(test) << "\"" << endl;
    cout << "ItsString::ToUpperCase(test) = \"" << ItsString::ToUpperCase(test) << "\"" << endl;
    cout << "ItsString::Trim(testTrim) = \"" << ItsString::Trim(testTrim) << "\"" << endl;

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
