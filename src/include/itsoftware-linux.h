///////////////////////////////////////////////////////////////////
//: Title        : itsoftware-linux.h
//: Product:     : Cpp.Include.Windows
//: Date         : 2020-05-01
//: Author       : "Kjetil Kristoffer Solberg" <post@ikjetil.no>
//: Version      : 1.0.0.0
//: Descriptions : Implementation of Cpp.Include.Linux.
#pragma once
//
// #include
//
#include <stdio.h>
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <exception>
#include <algorithm>
#include <string>
#include <vector>
#include <codecvt>
#include <random>
#include <functional>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <type_traits>

//
// namespace
//
namespace ItSoftware
{
	namespace Linux
	{
		//
		// using
		//
		using std::string;
		using std::stringstream;
		using std::vector;
		using std::endl;
		using std::ends;

		//
		// Macros
		//
		#define DECIMALTOSTRING2(d) #d
		#define DECIMALTOSTRING1(d) DECIMALTOSTRING2(d)
		#define ERR_AT_LOCATION(e) __FILE__ "(" DECIMALTOSTRING1(__LINE__) "): " #e
		#define ERROR_HERE __FILE__ "(" DECIMALTOSTRING2(__LINE__) "): "
		#define CHAR_BIT 8
		#define BitsCount(val) (sizeof(val) * CHAR_BIT)
		#define Shift(val, steps) (steps % BitsCount(val))
		#define ROL(val, steps) ((val << Shift(val, steps)) | (val >> (BitsCount(val) - Shift(val, steps))))
		#define ROR(val, steps) ((val >> Shift(val, steps)) | (val << (BitsCount(val) - Shift(val, steps))))
		#define AddFlag(val, flag) (val | flag)
		#define RemoveFlag(val, flag) (val & ~flag)

		//
		// Function: ItsAddFlag
		//
		template <typename T>
		T ItsAddFlag(T value, T flag)
		{
			return value | flag;
		}

		//
		// Function:: ItsRemoveFlag
		//
		template <typename T>
		T ItsRemoveFlag(T value, T flag)
		{
			return value & ~flag;
		}

		//
		// struct: ItsTime
		//
		// (i): Split milliseconds or render milliseconds to a friendly string.
		//
		struct ItsTime
		{
			//
			// Split ms to its parts
			//
			static void SplitMsToHourMinuteSeconds(size_t time_ms, size_t *part_hours, size_t *part_minutes, size_t *part_seconds, size_t *part_ms)
			{
				size_t seconds = time_ms / 1000;

				*part_hours = 0;
				*part_minutes = 0;

				*part_seconds = 0;
				*part_ms = 0;

				*part_hours = seconds / 3600;
				*part_minutes = (seconds - (*part_hours * 3600)) / 60;
				*part_seconds = seconds - (*part_hours * 3600) - (*part_minutes * 60);
				*part_ms = time_ms - (*part_seconds * 1000) - (*part_minutes * 60 * 1000) - (*part_hours * 3600 * 1000);
			}

			static void SplitHourToYearWeekDayHour(size_t houIn, size_t *houRest, size_t *day, size_t *week, size_t *year)
			{
				*houRest = houIn;

				*day = houIn / 24;
				*week = *day / 7;
				*year = *week / 52;

				*day -= (*week * 7);

				*week -= (*year * 52);

				*houRest -= *week * 7 * 24;
				*houRest -= *day * 24;
				*houRest -= *year * 52 * 7 * 24;
			}

			static string RenderMsToFullString(size_t milliseconds, bool bWithMilliseconds)
			{
				size_t hou = 0;
				size_t min = 0;
				size_t sec = 0;
				size_t ms = 0;
				ItsTime::SplitMsToHourMinuteSeconds(milliseconds, &hou, &min, &sec, &ms);

				size_t houRest = 0;
				size_t day = 0;
				size_t week = 0;
				size_t year = 0;
				ItsTime::SplitHourToYearWeekDayHour(hou, &houRest, &day, &week, &year);

				stringstream ss;
				if (year > 0)
				{
					if (year == 1)
					{
						ss << year << " year ";
					}
					else
					{
						ss << year << " years ";
					}
				}
				if (week > 0 || year > 0)
				{
					if (week == 1 || week == 0)
					{
						ss << week << " week ";
					}
					else
					{
						ss << week << " weeks ";
					}
				}
				if (day > 0 || week > 0 || year > 0)
				{
					if (day == 1 || day == 0)
					{
						ss << day << " day ";
					}
					else
					{
						ss << day << " days ";
					}
				}
				if (houRest > 0 || day > 0 || week > 0 || year > 0)
				{
					if (houRest == 1 || houRest == 0)
					{
						ss << houRest << " hour ";
					}
					else
					{
						ss << houRest << " hours ";
					}
				}

				if (min < 10)
				{
					ss << "0" << min << ":";
				}
				else
				{
					ss << min << ":";
				}
				if (sec < 10)
				{
					ss << "0" << sec;
				}
				else
				{
					ss << sec;
				}

				if ( bWithMilliseconds ) {
					if (ms < 10 ) {
						ss << ".00" << ms;
					}
					else if ( ms < 100 ) {
						ss << ".0" << ms;
					}
					else {
						ss << "." << ms;
					}
				}

				string str = ss.str();

				return str;
			}
		};

		//
		// enum ItsExpandDirection
		//
		// (i): Where text in expanded string input text should be aligned.
		//
		enum class ItsExpandDirection
		{
			Left,
			Middle,
			Right
		};

		//
		// struct: ItsString
		//
		// (i): Misc. string routines in one place.
		//
		struct ItsString
		{
			static string WidthExpand(string source, size_t width, char fill, ItsExpandDirection direction)
			{
				if (source.size() == 0) {
					return string("");
				}
				if (width <= 0) {
					return string("");
				}

				if (source.size() >= width) {
					return source.substr(0, width);
				}

				stringstream result;
				if (direction == ItsExpandDirection::Left)
				{
					for (size_t i = 0; i < (width - source.size()); i++)
					{
						result << fill;
					}
					result << source;
				}
				else if (direction == ItsExpandDirection::Middle)
				{
					for (size_t i = 0; i < ((width - source.size()) / 2); i++)
					{
						result << fill;
					}

					result << source;

					for (size_t i = result.str().size(); i < width; i++)
					{
						result << fill;
					}
				}
				else if (direction == ItsExpandDirection::Right)
				{
					result << source;

					for (size_t i = 0; i < (width - source.size()); i++)
					{
						result << fill;
					}
				}
				else {
					return string("");
				}

				result << ends;

				return result.str();
			}

			static vector<string> Split(string& data, string& token)
			{
				vector<string> output;
				size_t pos = string::npos; // size_t to avoid improbable overflow
				do
				{
					pos = data.find(token);
					output.push_back(data.substr(0, pos));
					if (string::npos != pos)
						data = data.substr(pos + token.size());
				} while (string::npos != pos);

				return output;
			}
			// to lower case
			static string ToLowerCase(string &s)
			{
				std::transform(s.begin(), s.end(), s.begin(), tolower);
				return s;
			}

			// to upper case
			static string ToUpperCase(string &s)
			{
				std::transform(s.begin(), s.end(), s.begin(), toupper);
				return s;
			}

			// trim from left
			static string TrimLeft(string &s, const char *t = " \t\n\r\f\v")
			{
				s.erase(0, s.find_first_not_of(t));
				return s;
			}

			// trim from right
			static string TrimRight(string &s, const char *t = " \t\n\r\f\v")
			{
				s.erase(s.find_last_not_of(t) + 1);
				return s;
			}

			// trim from left & right
			static string Trim(string &s, const char *t = " \t\n\r\f\v")
			{
				auto val = TrimRight(s, t);
				return TrimLeft(val, t);
			}

			// left count chars
			static string Left(string &s, uint32_t count)
			{
				if (s.size() == 0 || count <= 0)
				{
					return string("");
				}

				if (count >= s.size())
				{
					return s;
				}

				stringstream ss;
				for (uint32_t i = 0; i < count; i++)
				{
					ss << s[i];
				}

				string str = ss.str();
				return str;
			}

			// mid index, count chars
			static string Mid(string &s, uint32_t index, uint32_t count)
			{
				if (s.size() == 0 || count <= 0 || index < 0 || index >= s.size())
				{
					return string("");
				}

				if (index + count >= s.size())
				{
					stringstream ss;
					for (size_t i = index; i < s.size(); i++)
					{
						ss << s[i];
					}
					string str = ss.str();
					return str;
				}

				stringstream ss;
				for (size_t i = index; i < index + count; i++)
				{
					ss << s[i];
				}

				string str = ss.str();
				return str;
			}

			// right count chars
			static string Right(string &s, uint32_t count)
			{
				if (s.size() == 0 || count <= 0)
				{
					return string("");
				}

				if (count >= s.size())
				{
					return s;
				}

				stringstream ss;
				for (size_t i = s.size() - count; i < s.size(); i++)
				{
					ss << s[i];
				}

				string str = ss.str();
				return str;
			}

			static string Replace(string& s, string& replace, string& replace_with)
			{
				if (s.size() == 0 || replace.size() == 0 || replace.size() > s.size())
				{
					return string("");
				}

				stringstream ss;
				size_t index = s.find(replace);
				if (index == string::npos)
				{
					return s;
				}
				while (index != string::npos)
				{
					ss << ItsString::Left(s, (int)index);
					ss << replace_with;
					ss << ItsString::Right(s, (int)(s.size() - index - replace.size()));

					index = s.find(replace, index + replace.size());
				}

				string str = ss.str();
				return str;
			}
		};

		//
		// enum: DataSizeStringType
		//
		// (i): Enum for string representation of ToDataSizeString ItsConvert function.
		//
		enum class ItsDataSizeStringType {
			Recommended,
			IEC
		};

		//
		// struct: ItsConvert
		//
		// (i): Misc. convertion routines in one place.
		//
		struct ItsConvert
		{
			static string ToDataSizeString(size_t size)
			{
				return ItsConvert::ToDataSizeString(size, 0);
			}
			static string ToDataSizeString(size_t size, int digits)
			{
				return ItsConvert::ToDataSizeString(size, digits, ItsDataSizeStringType::Recommended);
			}
			static string ToDataSizeString(size_t size, int digits, ItsDataSizeStringType type)
			{
				if (digits < 0)
				{
					digits = 0;
				}
				else if (digits > 3)
				{
					digits = 3;
				}

				stringstream ss;
				double dSize = (double)size;

				int index = 0;
				while (dSize >= 1024)
				{
					dSize /= 1024;
					index++;
				}

				vector<string> szSize;
				if (type == ItsDataSizeStringType::IEC) {
					szSize = { "Bi", "KiB", "MiB", "GiB", "TiB", "PiB", "EiB", "ZiB", "YiB", "BB", "GP" };
				}
				else {//(type == ItsDataSizeStringType::Recommended) { // defaults to this notation
					szSize = { "B", "KB", "MB", "GB", "TB", "PB", "EB", "ZB", "YB", "BB", "GP" };
				}

				size_t tst = (size_t)dSize;
				ss << tst;
				if (digits > 0) {
					double t = dSize - tst;
					string ws = ItsConvert::ToString<double>(t);
					if (ws[0] == '0' && t != 0.0) {
						ws += "000";
					}
					else {
						ws += ".000";
					}
					ss << ws.substr(1, digits+1);
				}
				ss << " ";
				ss << ((index > (szSize.size() - 1) || index < 0) ? "?" : szSize[index]);
				ss << ends;

				return ss.str();
			}

			template<typename Numeric>
			static Numeric ToNumber(const string& str)
			{
				if (std::is_same_v<Numeric, float>)
				{
					return std::stof(str);
				}
				else if (std::is_same_v<Numeric, double>)
				{
					return std::stod(str);
				}
				else if (std::is_same_v<Numeric, int>)
				{
					return std::stoi(str);
				}
				else if (std::is_same_v<Numeric, long>)
				{
					return std::stol(str);
				}
				else if (std::is_same_v<Numeric, long long>)
				{
					return std::stoll(str);
				}
		
				Numeric x = std::stoull(str);
				return x;
			}

			static int ToIntFromHex(const string& str)
			{
				char * p;
    			int n = strtol( str.c_str(), &p, 16 );
    			if ( *p != 0 ) {
        			return 0;
    			}
				return n;
			}

			static string ToString(const bool flag)
			{
				if (flag)
				{
					return string("True");
				}
				return string("False");
			}

			template<typename Numeric>
			static string ToString(Numeric number)
			{
				stringstream ss;
				if (std::is_same_v<Numeric,float>)
				{
					ss << std::setprecision(std::numeric_limits<float>::digits10);
				}
				else if (std::is_same_v<Numeric, double>)
				{
					ss << std::setprecision(std::numeric_limits<double>::digits10);
				}
				ss << number;

				string tos = ss.str();
				return tos;
			}

			static string ToStringFormatted(size_t number, char thousandSep = ' ')
			{
				string txt = ItsConvert::ToString(number);

				stringstream ss;

				int groupSize = 3;
				int i = groupSize;
				size_t iCount = 0;
				size_t iSize = txt.size();
				for (auto itr = txt.rbegin(); itr < txt.rend(); itr++)
				{
					if (i-- > 0)
					{
						ss << *itr;
						iCount++;
					}
					else
					{
						if (iCount < iSize)
						{
							ss << thousandSep;
							ss << *itr;
							iCount++;
							i = groupSize - 1;
						}
						else {
							i = groupSize;
						}
					}

				}

				txt = ss.str();
				stringstream ss2;
				for (auto itr = txt.rbegin(); itr < txt.rend(); itr++)
				{
					ss2 << *itr;
				}

				string retVal = ss2.str();

				return retVal;
			}

			static string ToString(tm& dateTime)
			{
				stringstream ss;
				ss << std::setfill('0') << std::setw(4) << (dateTime.tm_year + 1900);
				ss << "-" << std::setfill('0') << std::setw(2) << (dateTime.tm_mon + 1);
				ss << "-" << std::setfill('0') << std::setw(2) << dateTime.tm_mday;
				ss << "T";
				ss << std::setfill('0') << std::setw(2) << dateTime.tm_hour;
				ss << ":" << std::setfill('0') << std::setw(2) << dateTime.tm_min;
				ss << ":" << std::setfill('0') << std::setw(2) << dateTime.tm_sec;

				string tos = ss.str();
				return tos;
			}

			template<typename Numeric>
			static string ToHexString(Numeric number, bool uppercase = true, uint32_t width = 0, char fillChar = '0', bool showBase = false)
			{
				stringstream ss;
				ss << std::hex;
				if (showBase)
				{
					ss << "0x";
				}
				if (uppercase)
				{
					ss << std::uppercase;
				}
				if (width > 0)
				{
					ss << std::setfill(fillChar) << std::setw(width);
				}
				ss << number;

				string tos = ss.str();
				return tos;
			}

			static tm ToTM(const string& dateTime)
			{
				tm t = {0};

				t.tm_year = ItsConvert::ToNumber<int>(dateTime.substr(0, 4)) - 1900;
				t.tm_mon = ItsConvert::ToNumber<int>(dateTime.substr(5, 2)) - 1;
				t.tm_mday = ItsConvert::ToNumber<int>(dateTime.substr(8, 2));
				t.tm_hour = ItsConvert::ToNumber<int>(dateTime.substr(11, 2));
				t.tm_min = ItsConvert::ToNumber<int>(dateTime.substr(14, 2));
				t.tm_sec = ItsConvert::ToNumber<int>(dateTime.substr(17, 2));

				return t;
			}

			static string ToLowerCase(string& text)
			{
				std::transform(text.begin(), text.end(), text.begin(), tolower);
				return text;
			}

			static string ToUpperCase(string& text)
			{
				std::transform(text.begin(), text.end(), text.begin(), toupper);
				return text;
			}

			static bool ToBool(string& flag)
			{
				flag = ItsConvert::ToLowerCase(flag);

				if (!strcmp(flag.c_str(), "true"))
				{
					return true;
				}
				else if (!strcmp(flag.c_str(), "1"))
				{
					return true;
				}
				else if (!strcmp(flag.c_str(), "yes"))
				{
					return true;
				}
				else if (!strcmp(flag.c_str(), "on"))
				{
					return true;
				}
				return false;
			}

			static string ToString(vector<uint64_t>& pks)
			{
				stringstream ss;
				bool bAddSep = false;
				for (auto &pk : pks)
				{
					if (bAddSep)
					{
						ss << ";";
					}
					ss << pk;
					bAddSep = true;
				}
				string tos = ss.str();
				return tos;
			}

			static vector<uint64_t> ToPK(const string& pks)
			{
				stringstream ss;
				ss.str(string(pks.begin(), pks.end()));

				string item;
				vector<uint64_t> topk;
				while (getline(ss, item, ';'))
				{
					if (!item.empty())
					{
						topk.push_back(ItsConvert::ToNumber<long long>(string(item.begin(), item.end())));
					}
				}

				return topk;
			}
		};

		//
		// struct: ItsDateTime
		//
		// (i): Represents a date and time.
		//
		struct ItsDateTime
		{
		private:
			tm m_tm;

		public:
			ItsDateTime(tm timeDate)
			{
				this->m_tm = timeDate;
			}

			ItsDateTime(ItsDateTime& dateTime)
			{
				this->m_tm = dateTime.m_tm;
			}

			ItsDateTime(ItsDateTime&& dateTime) noexcept
			{
				this->m_tm = dateTime.m_tm;
			}

			static ItsDateTime Now()
			{
				time_t t;
				time(&t);
				tm tm2;
				::localtime_r(&t, &tm2);

				return ItsDateTime(tm2);
			}

			string ToString()
			{
				stringstream ss;
				ss << std::setfill('0') << std::setw(4) << (this->m_tm.tm_year + 1900);
				ss << "-" << std::setfill('0') << std::setw(2) << (this->m_tm.tm_mon + 1);
				ss << "-" << std::setfill('0') << std::setw(2) << this->m_tm.tm_mday;
				ss << " ";
				ss << std::setfill('0') << std::setw(2) << this->m_tm.tm_hour;
				ss << ":" << std::setfill('0') << std::setw(2) << this->m_tm.tm_min;
				ss << ":" << std::setfill('0') << std::setw(2) << this->m_tm.tm_sec;

				string tos = ss.str();
				return tos;
			}

			string ToString(string option)
			{
				if (option == "s" || option == "S")
				{
					stringstream ss;
					ss << std::setfill('0') << std::setw(4) << (this->m_tm.tm_year + 1900);
					ss << "-" << std::setfill('0') << std::setw(2) << (this->m_tm.tm_mon + 1);
					ss << "-" << std::setfill('0') << std::setw(2) << this->m_tm.tm_mday;
					ss << "T";
					ss << std::setfill('0') << std::setw(2) << this->m_tm.tm_hour;
					ss << ":" << std::setfill('0') << std::setw(2) << this->m_tm.tm_min;
					ss << ":" << std::setfill('0') << std::setw(2) << this->m_tm.tm_sec;

					string tos = ss.str();
					return tos;
				}

				// day
				int d = this->GetDay();
				string str_d = ItsConvert::ToString(d);
				string str_dd;
				if (d < 10)
				{
					str_dd = "0";
					str_dd += str_d;
				}
				else
				{
					str_dd = str_d;
				}

				// month
				int m = this->GetMonth();
				string str_m = ItsConvert::ToString(m);
				string str_mm;
				if (m < 10)
				{
					str_mm = "0";
					str_mm += str_m;
				}
				else
				{
					str_mm = str_m;
				}

				// year
				int y = this->GetYear();
				string str_yyyy = ItsConvert::ToString(y);
				string str_yy = ItsString::Right(str_yyyy, 2);

				// hour			
				int h = this->GetHour();
				string str_h = ItsConvert::ToString(h);
				string str_hh;
				if (h < 10)
				{
					str_hh = "0";
					str_hh += str_h;
				}
				else
				{
					str_hh = str_h;
				}

				// minute
				int min = this->GetMinute();
				string str_min = ItsConvert::ToString(min);
				string str_minmin;
				if (min < 10)
				{
					str_minmin = "0";
					str_minmin += str_min;
				}
				else
				{
					str_minmin = str_min;
				}

				// second
				int s = this->GetSecond();
				string str_s = ItsConvert::ToString(s);
				string str_ss;
				if (s < 10)
				{
					str_ss = "0";
					str_ss += str_s;
				}
				else
				{
					str_ss = str_s;
				}

				string wdd = string("dd");
				string wd = string("d");
				option = ItsString::Replace(option, wdd, str_dd);
				option = ItsString::Replace(option, wd, str_d);

				string wMM = string("MM");
				string wM = string("M");
				option = ItsString::Replace(option, wMM, str_mm);
				option = ItsString::Replace(option, wM, str_m);

				string wyyyy = string("yyyy");
				string wyy = string("yy");
				option = ItsString::Replace(option, wyyyy, str_yyyy);
				option = ItsString::Replace(option, wyy, str_yy);

				string wHH = string("HH");
				string wH = string("H");
				option = ItsString::Replace(option, wHH, str_hh);
				option = ItsString::Replace(option, wH, str_h);

				string wmm = string("mm");
				string wm = string("m");
				option = ItsString::Replace(option, wmm, str_minmin);
				option = ItsString::Replace(option, wm, str_min);

				string wss = string("ss");
				string ws = string("s");
				option = ItsString::Replace(option, wss, str_ss);
				option = ItsString::Replace(option, ws, str_s);

				return option;
			}

			tm TM()
			{
				return this->m_tm;
			}

			void AddYears(int year)
			{
				this->m_tm.tm_year += year;

				mktime(&this->m_tm);
			}

			void AddMonths(int month)
			{
				this->m_tm.tm_mon += month;

				mktime(&this->m_tm);
			}

			void AddDays(int day)
			{
				this->m_tm.tm_mday += day;

				mktime(&this->m_tm);
			}

			void AddHours(int hour)
			{
				this->m_tm.tm_hour += hour;

				mktime(&this->m_tm);
			}

			void AddMinutes(int min)
			{
				this->m_tm.tm_min += min;

				mktime(&this->m_tm);
			}

			void AddSeconds(int sec)
			{
				this->m_tm.tm_sec += sec;

				mktime(&this->m_tm);
			}

			void Add(int year, int month, int day, int hour, int min, int sec)
			{
				this->m_tm.tm_year += year;
				this->m_tm.tm_mon += month;
				this->m_tm.tm_mday += day;
				this->m_tm.tm_hour += hour;
				this->m_tm.tm_min += min;
				this->m_tm.tm_sec += sec;

				mktime(&this->m_tm);
			}

			void SubtractYears(int year)
			{
				this->m_tm.tm_year -= year;

				mktime(&this->m_tm);
			}

			void SubtractMonths(int month)
			{
				this->m_tm.tm_mon -= month;

				mktime(&this->m_tm);
			}

			void SubtractDays(int day)
			{
				this->m_tm.tm_mday -= day;

				mktime(&this->m_tm);
			}

			void SubtractHours(int hour)
			{
				this->m_tm.tm_hour -= hour;

				mktime(&this->m_tm);
			}

			void SubtractMinutes(int min)
			{
				this->m_tm.tm_min -= min;

				mktime(&this->m_tm);
			}

			void SubtractSeconds(int sec)
			{
				this->m_tm.tm_sec -= sec;

				mktime(&this->m_tm);
			}

			void Subtract(int year, int month, int day, int hour, int min, int sec)
			{
				this->m_tm.tm_year -= year;
				this->m_tm.tm_mon -= month;
				this->m_tm.tm_mday -= day;
				this->m_tm.tm_hour -= hour;
				this->m_tm.tm_min -= min;
				this->m_tm.tm_sec -= sec;

				mktime(&this->m_tm);
			}

			int GetYear()
			{
				return this->m_tm.tm_year + 1900;
			}

			int GetMonth()
			{
				return this->m_tm.tm_mon + 1;
			}

			int GetDay()
			{
				return this->m_tm.tm_mday;
			}

			int GetHour()
			{
				return this->m_tm.tm_hour;
			}

			int GetMinute()
			{
				return this->m_tm.tm_min;
			}

			int GetSecond()
			{
				return this->m_tm.tm_sec;
			}

			ItsDateTime& operator=(const tm& timeDate)
			{
				this->m_tm = timeDate;
				return *this;
			}
		};

		//
		// Function: ItsRandom
		//
		// (i): A random number wrapper.
		//
		template<typename Numeric, typename Generator = std::mt19937>
		Numeric ItsRandom(Numeric from, Numeric to)
		{
			thread_local static Generator gen(std::random_device{}());

			using dist_type = typename std::conditional
			<
				std::is_integral<Numeric>::value
				, std::uniform_int_distribution<Numeric>
				, std::uniform_real_distribution<Numeric>
			>::type;

			thread_local static dist_type dist;

			return dist(gen, typename dist_type::param_type{from, to});
		}

		//
		// ItsLogType
		//
		// (i): Log type for ItsLogItem
		//
		enum class ItsLogType
		{
			Information,
			Warning,
			Error,
			Other,
			Debug
		};

		//
		// struct: ItsLogItem
		//
		// (i): Log item for ItsLog
		//
		struct ItsLogItem
		{
			ItsLogType Type;
			string	Description;
			tm When;
		};

		//
		// struct: ItsLog
		//
		// (i): CUstom application event log.
		//
		struct ItsLog
		{
		private:
			vector<ItsLogItem> m_items;
		public:
			void LogInformation(string description)
			{
				ItsLogItem item;
				item.When = ItsDateTime::Now().TM();
				item.Description = description;
				item.Type = ItsLogType::Information;

				this->m_items.push_back(item);
			}

			void LogWarning(string description)
			{
				ItsLogItem item;
				item.When = ItsDateTime::Now().TM();
				item.Description = description;
				item.Type = ItsLogType::Warning;

				this->m_items.push_back(item);
			}

			void LogError(string description)
			{
				ItsLogItem item;
				item.When = ItsDateTime::Now().TM();
				item.Description = description;
				item.Type = ItsLogType::Error;

				this->m_items.push_back(item);
			}

			void LogOther(string description)
			{
				ItsLogItem item;
				item.When = ItsDateTime::Now().TM();
				item.Description = description;
				item.Type = ItsLogType::Other;

				this->m_items.push_back(item);
			}

			void LogDebug(string description)
			{
				ItsLogItem item;
				item.When = ItsDateTime::Now().TM();
				item.Description = description;
				item.Type = ItsLogType::Debug;

				this->m_items.push_back(item);
			}

			vector<ItsLogItem> GetItems()
			{
				vector<ItsLogItem> copy;

				for (auto i : this->m_items)
				{
					copy.push_back(i);
				}

				return copy;
			}

			void Clear()
			{
				this->m_items.clear();
			}

			string LogTypeToString(ItsLogType t)
			{
				string type;
				switch (t)
				{
				case ItsLogType::Information:
					type = "Information";
					break;
				case ItsLogType::Warning:
					type = "Warning";
					break;
				case ItsLogType::Error:
					type = "Error";
					break;
				case ItsLogType::Other:
					type = "Other";
					break;
				case ItsLogType::Debug:
					type = "Debug";
					break;
				default:
					type = "<UNKNOWN>";
					break;
				}
				return type;
			}

			string ToString()
			{
				stringstream ss;
				for (auto i : this->m_items) {
					ss << std::setiosflags(std::ios::left) << std::setw(12) << this->LogTypeToString(i.Type) << ItsDateTime(i.When).ToString() << " " << i.Description << endl;
				}
				ss << ends;

				return ss.str();
			}
		};


	} // namespace Linux
} // namespace ItSoftware
