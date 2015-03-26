#pragma once

#include <string>

#include "HtmlEntity.h"
#include "VectorExtension.h"

using namespace std;
using namespace System;

namespace Parser
{
	class HtmlInnerText :
		public HtmlEntity
	{
	public:
		HtmlInnerText(string textArg);
		~HtmlInnerText();
		string GetText();

	private:
		string text;
	};
}
