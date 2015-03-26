#include "stdafx.h"
#include "HtmlTag.h"

namespace Parser
{

#pragma region Constructors

	HtmlTag::HtmlTag(string tag, string line) : HtmlEntity(1)
	{
		tagType = tag;
		if (tag == "meta" || tag == "a" || tag == "img")
		{
			ParseAttribut(line);
		}
		VectorExtension::FreeAll(line);
	}

	HtmlTag::~HtmlTag()
	{
		VectorExtension::FreeAll(attributs);
		VectorExtension::FreeAll(tagType);
	}

#pragma endregion

#pragma region Methods

	void HtmlTag::ParseAttribut(string line)
	{
		line = StringExtension::Replace(line, "<" + tagType, "");
		line = StringExtension::TrimSpace(line);
		line = line.substr(0, line.size() - 1); // suppression de ">" à la fin de la ligne
		if (line != "")
		{
			vector<string> quoteSplit = StringExtension::Split(line, '\"');
			for (unsigned int i = 1; i < quoteSplit.size(); i++)
			{
				if (i % 2 != 0)
				{
					string previousValue = quoteSplit[i - 1];
					string currentValue = quoteSplit[i];
					previousValue = StringExtension::Replace(previousValue, " ", "");
					previousValue = StringExtension::Replace(previousValue, "=", "");
					previousValue = StringExtension::ToLower(previousValue);
					attributs[previousValue] = currentValue;
					VectorExtension::FreeAll(currentValue);
					VectorExtension::FreeAll(previousValue);
				}
			}
			VectorExtension::FreeAll(quoteSplit);
		}
	}

#pragma endregion

#pragma region Functions

	string HtmlTag::GetTagType()
	{
		return tagType;
	}

	map<string, string> HtmlTag::GetAttributs()
	{
		return attributs;
	}

#pragma endregion

}