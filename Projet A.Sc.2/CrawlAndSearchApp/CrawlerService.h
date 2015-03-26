#pragma once

#include <string>
#include <vector>
#include <algorithm>
#include <concurrent_vector.h>
#include <Windows.h>

#include <urlmon.h>
#pragma comment(lib, "urlmon.lib")

#include "ThreadManager.h"
#include "SqlHelper.h"
#include "PageInternet.h"
#include "Balises.h"
#include "Photo.h"
#include "HtmlParser.h"
#include "HtmlTagResult.h"
#include "StringExtension.h"
#include "VectorExtension.h"
#include "FileUtility.h"
#include "ImageUtility.h"

using namespace concurrency;
using namespace std;
using namespace Parser;
using namespace SQL;
using namespace System;

namespace Crawler
{

	class CrawlerService
	{
	public:
		CrawlerService(SqlHelper*& sqlArg);
		~CrawlerService();
		void	SubmitUrlToCrawl(string url);
		void	StartCrawlService();
		void	StopCrawlService();
		string	GetDomainName(const string& url);

	private:
		SqlHelper*					sql;
		vector<string>				tagWords;
		concurrent_vector<string>	urlInCrawling;
		concurrent_vector<string>	urlPhotoInCrawling;
		concurrent_vector<string>	urlInWait;
		concurrent_vector<string>	urlPhotoInWait;
		concurrent_vector<string>	urlBlackList;
		ThreadManager				threadManager;
		mutex						threadMutex;
		bool						serviceRunning = false;
		ImageUtility				imageUtility;

		void						Service();
		void						ServicePeriodic();
		void						SetTagWords();
		void						Crawl(string url, int id, int viewCount = 0, int shareCount = 0, int favoriteCount = 0, int likeCount = 0);
		vector<HtmlTagResult*>*		Explore(vector<HtmlEntity*>* document);
		string						GetUrlFromSameDomain(HtmlTagResult tag, const string& domainName, const string& currentUrl);
		string						AddDomainToString(string urlPart, string currentUrl);
		bool						FindConcurrencyVector(concurrent_vector<string> vect, const string& query);
	};

}