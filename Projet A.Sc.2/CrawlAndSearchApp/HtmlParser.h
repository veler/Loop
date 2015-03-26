#pragma once

#include <string>
#include <fstream>
#include <vector>
#include <algorithm>

#include "VectorExtension.h"
#include "HtmlEntity.h"
#include "HtmlInnerText.h"
#include "HtmlTag.h"

using namespace std;
using namespace System;

namespace Parser
{
	class HtmlParser
	{
	public:
		HtmlParser(string filePath);
		~HtmlParser();
		vector<HtmlEntity*>* Parse();
	private:
		string*				fileContent;
		char				forbiddenChar[8];

		void					SplitTags(vector<string>& vector, string* str);
		vector<HtmlEntity*>*	CreateTreeMap(vector<string>& vector, unsigned int& index, string tag);

	};
}

