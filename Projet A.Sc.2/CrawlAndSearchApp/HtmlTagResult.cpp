#include "stdafx.h"
#include "HtmlTagResult.h"

namespace Crawler
{

	HtmlTagResult::HtmlTagResult(Parser::HtmlTag tag) : HtmlTag(tag)
	{
	}


	HtmlTagResult::~HtmlTagResult()
	{
		VectorExtension::FreeAll(innerText);
	}

	void HtmlTagResult::SetInnerText(vector<string> value)
	{
		innerText = value;
	}

	string HtmlTagResult::GetInnerText()
	{
		string result = "";
		for each (string item in innerText)
			result += item + " ";
		return result;
	}

}