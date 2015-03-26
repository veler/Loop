#pragma once

#include <WinSock2.h>
#pragma comment(lib, "WS2_32.Lib")

#include <string>
#include <vector>
#include <algorithm>
#include <thread>
#include <ppltasks.h>
#include <concurrent_vector.h>
#include <array>
#include "PageInternet.h"  
#include "Photo.h"
#include "StringExtension.h"
#include "SqlHelper.h"
#include "Json.h"

using namespace std;
using namespace concurrency;
using namespace System;

namespace Searcher
{
	class SearcherService
	{
	public:
		SearcherService(SQL::SqlHelper*& sqlArg);
		~SearcherService();
		void	StartSearchService();
		void	StopSearchService();
		string	DoSearch(string query);
		string	DoImageSearch(string query, string color, string size);

	private:
		const int viewCountCoef = 1;
		const int favoriteCountCoef = 1;
		const int shareCountCoef = 3;
		const int likeCountCoef = 3;
		const int keywordCoef = 10;
		const int titleCorrectCoef = 100;
		const int titleCoef = 10;
		const int descriptionCorrectCoef = 1000;
		const int descriptionCoef = 10;
		const int urlCorrectCoef = 1000;
		const int urlCoef = 100;
		const int h1Coef = 7;
		const int h2Coef = 6;
		const int h3Coef = 5;
		const int h4Coef = 4;
		const int h5Coef = 3;
		const int h6Coef = 2;
		const int bStrongCoef = 4;
		const int spanCoef = 3;

		SQL::SqlHelper* sql;
		bool			socketRunning;

		int		CalculateRelevance(const string& query, const vector<string>& wordSearched, SQL::PageInternet& page);
		int		CalculateImageRelevance(const string& query, const vector<string>& wordSearched, SQL::Photo& photo);
	};
}