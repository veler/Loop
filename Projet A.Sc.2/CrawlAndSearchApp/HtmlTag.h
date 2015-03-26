#pragma once

#include <string>
#include <map>
#include <vector>
#include <thread>
#include <algorithm>

#include "HtmlEntity.h"
#include "StringExtension.h"
#include "VectorExtension.h"

using namespace std;
using namespace System;

namespace Parser
{
	class HtmlTag :
		public HtmlEntity
	{
	public:
		HtmlTag();
		HtmlTag(string tag, string line);
		~HtmlTag();
		string				GetTagType();
		map<string, string>	GetAttributs();	

	private:
		string				tagType;
		map<string, string> attributs;

		void ParseAttribut(string line);
	};
}
