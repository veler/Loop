#include "stdafx.h"
#include "HtmlInnerText.h"

namespace Parser
{

#pragma region Constructors

	HtmlInnerText::HtmlInnerText(string textArg) : HtmlEntity(2)
	{
		text = textArg;
		VectorExtension::FreeAll(textArg);
	}


	HtmlInnerText::~HtmlInnerText()
	{
	}

#pragma endregion

#pragma region Functions

	string HtmlInnerText::GetText()
	{
		return text;
	}

#pragma endregion
}