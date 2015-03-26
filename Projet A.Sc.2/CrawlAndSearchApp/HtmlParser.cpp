#include "stdafx.h"
#include "HtmlParser.h"
#include <iostream>   
#include <string>     
#include <locale>   

namespace Parser
{

#pragma region Constructors

	HtmlParser::HtmlParser(string filePath)
	{
		ifstream file = ifstream(filePath);
		if (!file)
			throw exception("File not found / Could not open file");
		fileContent = new string((istreambuf_iterator<char>(file)), (istreambuf_iterator<char>()));
		file.close();
		forbiddenChar[0] = '!';
		forbiddenChar[1] = '-';
		forbiddenChar[2] = '/';
		forbiddenChar[3] = '[';
		forbiddenChar[4] = '>';
		forbiddenChar[5] = '+';
		forbiddenChar[6] = ';';
		forbiddenChar[7] = '\\';
	}

	HtmlParser::~HtmlParser()
	{
		delete fileContent;
	}

#pragma endregion

#pragma region Methods

	vector<HtmlEntity*>* HtmlParser::Parse()
	{
		vector<HtmlEntity*>* document = new vector<HtmlEntity*>();
		if (fileContent->empty())
			return document;

		unsigned int index = 0;
		vector<string> VecStr;
		SplitTags(VecStr, fileContent);
		document = CreateTreeMap(VecStr, index, "html");
		VectorExtension::FreeAll(VecStr);
		VectorExtension::FreeAll(*fileContent);
		return document;
	}

	void HtmlParser::SplitTags(vector<string>& vector, string* str)
	{
		string::size_type cursor = 0;
		string::size_type openChevronIndex = 0;
		string::size_type closeChevronIndex = 0;
		string strTemp;

		VectorExtension::FreeAll(vector);
		replace(str->begin(), str->end(), '\n', ' ');
		replace(str->begin(), str->end(), '\r', ' ');
		replace(str->begin(), str->end(), '\t', ' ');

		for each(char c in *str)
		{
			if (c == '<')
			{
				openChevronIndex = cursor;
				if (openChevronIndex > 0 && openChevronIndex - closeChevronIndex - 1 > 0 && openChevronIndex >= closeChevronIndex)
				{
					strTemp = str->substr(closeChevronIndex + 1, openChevronIndex - closeChevronIndex - 1);
					if (strTemp.find_first_not_of(' ') != string::npos)
						vector.push_back(strTemp);
				}
			}
			else if (c == '>')
			{
				closeChevronIndex = cursor;
				if (closeChevronIndex > 0 && closeChevronIndex >= openChevronIndex)
				{
					strTemp = str->substr(openChevronIndex, closeChevronIndex - openChevronIndex + 1);
					if (strTemp.find_first_not_of(' ') != string::npos)
						vector.push_back(strTemp);
				}
			}
			cursor++;
		}
		cursor = 0;
		openChevronIndex = 0;
		closeChevronIndex = 0;
		VectorExtension::FreeAll(strTemp);
	}

#pragma endregion

#pragma region Functions

	vector<HtmlEntity*>* HtmlParser::CreateTreeMap(vector<string>& tabStr, unsigned int& index, string tag)
	{
		vector<HtmlEntity*>* result = new vector<HtmlEntity*>();
		int indexChar;
		int indexCharChevron;
		int i;
		bool closingTag = false;
		string line;

		while (closingTag == false && index < tabStr.size())
		{
			line = tabStr[index];

			if (line.length() == 0)
			{
				index++;
				continue;
			}

			if (line.length() > 2 && line[0] == '<')
			{
				indexChar = line.find_first_of(' ');
				indexCharChevron = line.find_last_of('>');
				if (indexChar == -1 || indexChar > indexCharChevron)
					indexChar = indexCharChevron;

				tag = line.substr(1, indexChar - 1);
				if (line[1] != '/') // Balise ouvrante
				{
					i = 0;
					indexChar = -1;
					while (indexChar == -1 && i < sizeof(forbiddenChar))
					{
						indexChar = tag.find_first_of(forbiddenChar[i]);
						i++;
					}

					if (indexChar == -1) // Si le tag semble correct
					{
						tag = StringExtension::ToLower(tag);
						tag = StringExtension::TrimSpace(tag);
						HtmlTag* htmlTagValue = new HtmlTag(tag, line);
						if (line[line.length() - 2] != '/') // Si le tag n'est pas unique
							htmlTagValue->SetChildren(CreateTreeMap(tabStr, ++index, tag));
						result->push_back(htmlTagValue);
					}
				}
				else
					closingTag = true; // Balise fermante, on quitte
				VectorExtension::FreeAll(tag);
			}
			else
			{
				result->push_back(new HtmlInnerText(line));
			}
			index++;
		}
		index--;
		indexChar = 0;
		indexCharChevron = 0;
		i = 0;
		VectorExtension::FreeAll(line);
		return result;
	}

#pragma endregion
}