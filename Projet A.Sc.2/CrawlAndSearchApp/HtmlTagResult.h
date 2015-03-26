#pragma once

#include <string>
#include <vector>
#include "HtmlTag.h"
#include "StringExtension.h"
#include "VectorExtension.h"

using namespace std;
using namespace System;

namespace Crawler
{

	class HtmlTagResult :
		public Parser::HtmlTag
	{
	public:
		HtmlTagResult(Parser::HtmlTag tag);
		~HtmlTagResult();
		void	SetInnerText(vector<string> value);
		string	GetInnerText();
	private:
		vector<string>	innerText;
	};

}