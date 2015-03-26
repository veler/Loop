#include "stdafx.h"
#include "StringExtension.h"


namespace System
{

#pragma region Constructors

	StringExtension::StringExtension()
	{
	}


	StringExtension::~StringExtension()
	{
	}

#pragma endregion

#pragma region Functions

	int StringExtension::Occurences(const string& text, const string& query)
	{
		if (query.length() == 0)
			return 0;
		int count = 0;
		for (size_t offset = text.find(query); offset != string::npos; offset = text.find(query, offset + query.length()))
			++count;
		return count;
	}

	string StringExtension::Replace(string text, const string& search, const string& replace)
	{
		size_t pos = 0;
		int replaceLenght = replace.length();
		int searchLenght = search.length();
		while ((pos = text.find(search, pos)) != string::npos)
		{
			text.replace(pos, searchLenght, replace);
			pos += replaceLenght;
		}
		pos = 0;
		replaceLenght = 0;
		searchLenght = 0;
		return text;
	}

	void StringExtension::ReplacePtr(string* text, const string& search, const string& replace)
	{
		size_t pos = 0;
		int replaceLenght = replace.length();
		int searchLenght = search.length();
		while ((pos = text->find(search, pos)) != string::npos)
		{
			text->replace(pos, searchLenght, replace);
			pos += replaceLenght;
		}
		pos = 0;
		replaceLenght = 0;
		searchLenght = 0;
	}

	vector<string> StringExtension::Split(const string& text, char splitChar)
	{
		int firstLetter = 0;
		int lastLetter = 0;
		int cursor = 0;
		int temp = 0;
		vector<string> vector;
		for each (char c in text)
		{
			if (c == splitChar)
			{
				lastLetter = cursor;
				temp = lastLetter - firstLetter;
				vector.push_back(text.substr(firstLetter, temp));
				firstLetter = lastLetter + 1;
			}
			cursor++;
		}
		lastLetter = cursor;
		temp = lastLetter - firstLetter;
		vector.push_back(text.substr(firstLetter, temp + 1));
		firstLetter = 0;
		lastLetter = 0;
		cursor = 0;
		temp = 0;
		splitChar = 0;
		return vector;
	}

	string StringExtension::ToLower(const string& text)
	{
		locale loc;
		string result;
		for each(char elem in text)
			result += tolower(elem, loc);
		return result;
	}

	string StringExtension::TrimSpace(string text)
	{
		if (text.length() <= 1)
			return text;

		while (text[0] == ' ')
		{
			text = text.substr(1, text.length() - 1);
		}

		while (text[text.length() - 1] == ' ')
		{
			text = text.substr(0, text.length() - 1);
		}
		return text;
	}

	bool StringExtension::StartsWith(const string& text, const string& query)
	{
		return query.length() <= text.length() && text.compare(0, query.length(), query) == 0;
	}

	bool StringExtension::EndsWith(const string& text, const string& query)
	{
		return query.length() <= text.length() && text.compare(text.length() - query.length(), query.length(), query) == 0;
	}

	wstring StringExtension::StringToWString(const string& text)
	{
		size_t sz = text.length();
		int nd = MultiByteToWideChar(CP_UTF8, 0, &text[0], sz, NULL, 0);
		wstring ret(nd, 0);
		int w = MultiByteToWideChar(CP_UTF8, 0, &text[0], sz, &ret[0], nd);
		return ret;
	}

#pragma endregion

}