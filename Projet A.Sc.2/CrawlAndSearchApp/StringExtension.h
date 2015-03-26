#pragma once

#include <string>
#include <vector>
#include <locale>
#include <windows.h>
#include <algorithm>  

using namespace std;

namespace System
{
	class StringExtension
	{
	public:
		StringExtension();
		~StringExtension();
		static int				Occurences(const string& text, const string& query);
		static vector<string>	Split(const string& text, char splitChar);
		static string			Replace(string text, const string& search, const string& replace);
		static void				ReplacePtr(string* text, const string& search, const string& replace);
		static string			ToLower(const string& text);
		static string			TrimSpace(string text);
		static bool				StartsWith(const string& text, const string& query);
		static bool				EndsWith(const string& text, const string& query);
		static wstring			StringToWString(const string& text);
	};
}
