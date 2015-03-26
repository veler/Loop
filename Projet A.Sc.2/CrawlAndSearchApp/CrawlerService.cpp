#include "stdafx.h"
#include "CrawlerService.h"

namespace Crawler
{

#pragma region Constructors

	CrawlerService::CrawlerService(SqlHelper*& sqlArg)
	{
		sql = sqlArg;
		SetTagWords();
		imageUtility = ImageUtility();
	}


	CrawlerService::~CrawlerService()
	{
	}

#pragma endregion

#pragma region Methods

	void CrawlerService::StartCrawlService()
	{
		if (serviceRunning)
			return;
		serviceRunning = true;

		if (!FileUtility::DirectoryExists("C:\\CrawlerTemp"))
			CreateDirectory(TEXT("C:\\CrawlerTemp"), NULL);

		FileUtility::DeleteAllFiles("C:\\CrawlerTemp");

		thread crawlServiceThread(&CrawlerService::Service, this);
		crawlServiceThread.detach();

		thread periodicCrawlServiceThread(&CrawlerService::ServicePeriodic, this);
		periodicCrawlServiceThread.detach();
	}

	void CrawlerService::StopCrawlService()
	{
		serviceRunning = false;
		VectorExtension::FreeAll(urlBlackList);
		VectorExtension::FreeAll(urlInWait);
		VectorExtension::FreeAll(urlInCrawling);
		VectorExtension::FreeAll(urlPhotoInWait);
		VectorExtension::FreeAll(urlPhotoInCrawling);
	}

	void CrawlerService::Service()
	{
		int min = 10000;
		int max = 99999;
		while (serviceRunning)
		{
#if defined(_PRODUCTION)
			if (FileUtility::DirectoryExists("M:\\"))
			{
#endif
				if (urlInWait.size() > 0) // CRAWLER PAGE
				{
					for (auto url = begin(urlInWait); url != end(urlInWait); ++url)
					{
						int id = min + (rand() % (int)(max - min + 1));
						urlInCrawling.push_back(*url);
						threadManager.AddTaskToRun(ThreadManager::NORMAL, &CrawlerService::Crawl, this, *url, id, 0, 0, 0, 0);
					}
					VectorExtension::FreeAll(urlInWait);
					threadManager.DoTasks(ThreadManager::NORMAL);
				}
				else if (threadManager.GetRunningCount() == 0)
				{
					VectorExtension::FreeAll(urlInCrawling);
					VectorExtension::FreeAll(urlPhotoInCrawling);
				}
#if defined(_PRODUCTION)
			}
#endif
			this_thread::sleep_for(std::chrono::milliseconds(5000));
		}
	}

	void CrawlerService::ServicePeriodic()
	{
		int min = 10000;
		int max = 99999;
		while (serviceRunning) // PERIODIC CRAWLER
		{
#if defined(_PRODUCTION)
			if (FileUtility::DirectoryExists("M:\\"))
			{
#endif
				vector<SQL::PageInternet> pages;
				pages = sql->SelectPageInternet();

				for each (SQL::PageInternet page in pages)
				{
					int score = page.GetFavoriteCount() + page.GetLikeCount() + page.GetShareCount() + page.GetViewCount();
					unsigned long long timeLapse = static_cast<long>(time(NULL)) - page.GetDate();

					if ((score >= 500 && timeLapse >= 60 * 60 * 24) || // Crawl quotidien
						(score >= 250 && timeLapse >= 60 * 60 * 24 * 7) || // Crawl hebdomadaire
						(timeLapse >= 60 * 60 * 24 * 30)) // Crawl mensuel
					{
						int id = min + (rand() % (int)(max - min + 1));
						sql->DeletePageInternet(page.GetPK_PageInternet());
						urlInCrawling.push_back(page.GetUrl());
						threadManager.AddTaskToRun(ThreadManager::IMPORTANT, &CrawlerService::Crawl, this, page.GetUrl(), id, page.GetViewCount(), page.GetShareCount(), page.GetFavoriteCount(), page.GetLikeCount());
					}
				}
				threadManager.DoTasks(ThreadManager::IMPORTANT);

#if defined(_PRODUCTION)
			}
#endif
			this_thread::sleep_for(std::chrono::hours(24));
		}
	}

	void CrawlerService::SubmitUrlToCrawl(string url)
	{
		url = StringExtension::TrimSpace(url);
		if (url.length() >= 4)
		{
			string lastChar = url.substr(url.length() - 4, 4);
			lastChar = StringExtension::ToLower(lastChar);
			if (lastChar == ".exe" || lastChar == ".pdf" || lastChar == ".zip" || lastChar == ".iso" || lastChar == ".dmg" || lastChar == ".rar" || lastChar == ".png" || lastChar == ".jpg" || lastChar == ".jpeg" || lastChar == ".gif" || lastChar == ".bmp")
				return;
			VectorExtension::FreeAll(lastChar);
		}

		if (url == "")
			return;

		urlInWait.push_back(url);
		VectorExtension::FreeAll(url);
	}

	void CrawlerService::Crawl(string url, int id, int viewCount, int shareCount, int favoriteCount, int likeCount)
	{
#if defined(_PRODUCTION)
		if (!FileUtility::DirectoryExists("M:\\"))
			return;
#endif

		int min = 10000;
		int max = 99999;
		bool parse = false;
		string filePath = "C:\\CrawlerTemp\\" + to_string(id) + ".html";
		string domainName = GetDomainName(url);

		HRESULT res = URLDownloadToFile(NULL, StringExtension::StringToWString(url).c_str(), StringExtension::StringToWString(filePath).c_str(), 0, NULL);

		if (FileUtility::FileExists(filePath))
		{
			if (res == S_OK)
				parse = true;
			else if (res == E_OUTOFMEMORY)
				cout << "Buffer length invalid, or insufficient memory" << endl;
			else if (res == INET_E_DOWNLOAD_FAILURE)
				cout << "URL is invalid" << endl;
			else
				cout << "Other error: " << res << endl; // ERROR 404
		}

		if (parse)
		{
			HtmlParser* parser = new HtmlParser(filePath);
			vector<HtmlEntity*>* document = parser->Parse();
			vector<HtmlTagResult*>* exploreResult = Explore(document);

			PageInternet pageDatabase;
			vector<Balises> tagsDatabase;
			int pkPageInternet;

			for each (HtmlTagResult* tag in *exploreResult)
			{
				if (tag->GetTagType() == "a") // si l'url est du même domaine que celui exploré et qu'il n'est pas déjà en base, on l'explore.
				{
					string urlToExplore = GetUrlFromSameDomain(*tag, domainName, url);
					if (urlToExplore != ""
						&& urlToExplore != url
						&& !FindConcurrencyVector(urlInCrawling, urlToExplore)
						&& !FindConcurrencyVector(urlInWait, urlToExplore)
						&& !FindConcurrencyVector(urlBlackList, urlToExplore)
						&& !sql->UrlExists(urlToExplore))
					{
						SubmitUrlToCrawl(urlToExplore);
					}
					VectorExtension::FreeAll(urlToExplore);
				}
				else if (tag->GetTagType() == "meta" && tag->GetAttributs().count("name") == 1 && tag->GetAttributs().count("content") == 1)
				{
					string nameAttribut = StringExtension::ToLower(tag->GetAttributs()["name"]);
					if (nameAttribut == "description" || nameAttribut == "og:description")
						pageDatabase.SetDescription(tag->GetAttributs()["content"]);
					else if (nameAttribut == "keywords")
						pageDatabase.SetKeywords(tag->GetAttributs()["content"]);
				}
				else if (tag->GetTagType() == "title")
				{
					pageDatabase.SetTitle(tag->GetInnerText());
				}
				else if (tag->GetTagType() == "img" && tag->GetAttributs().count("src") == 1)
				{
					string urlToExplore = AddDomainToString(tag->GetAttributs()["src"], url);
					if (urlToExplore != ""
						&& urlToExplore != url
						&& !FindConcurrencyVector(urlPhotoInCrawling, urlToExplore)
						&& !sql->UrlExists(urlToExplore, true))
					{
						urlPhotoInCrawling.push_back(url);
						string title;
						if (tag->GetAttributs().count("title") == 1)
							title = tag->GetAttributs()["title"];
						else if (tag->GetAttributs().count("alt") == 1)
							title = tag->GetAttributs()["alt"];
						else
							title = pageDatabase.GetTitle();
						int id = min + (rand() % (int)(max - min + 1));
						threadManager.AddTaskToRun(ThreadManager::NORMAL, &ImageUtility::GetPhotoInfo, imageUtility, urlToExplore, title, pageDatabase.GetDescription(), id, sql);
					}
					VectorExtension::FreeAll(urlToExplore);
				}
				else
				{
					string innerText = tag->GetInnerText();
					if (innerText != "")
						tagsDatabase.push_back(Balises(0, tag->GetTagType(), innerText));
					VectorExtension::FreeAll(innerText);
				}
			}

			pageDatabase.SetUrl(url);
			pageDatabase.SetFavoriteCount(favoriteCount);
			pageDatabase.SetViewCount(viewCount);
			pageDatabase.SetShareCount(shareCount);
			pageDatabase.SetLikeCount(likeCount);
			pageDatabase.SetDate(static_cast<long>(time(NULL)));

			sql->InsertPageInternet(pageDatabase);
			pkPageInternet = sql->GetPageInternetPK(url);

			for each (Balises tag in tagsDatabase)
			{
				tag.SetFK_PageInternet(pkPageInternet);
				sql->InsertBalises(tag);
			}


			VectorExtension::FreeAll(tagsDatabase);

			if (exploreResult->size() > 0)
			{
				for (int i = exploreResult->size(); i > 0; i--)
				{
					delete exploreResult->at(i - 1);
				}
				delete exploreResult;
			}
			delete parser;
		}
		else
		{
			urlBlackList.push_back(url);
		}

		if (FileUtility::FileExists(filePath))
			remove(filePath.c_str());

		VectorExtension::FreeAll(domainName);
		VectorExtension::FreeAll(filePath);

		if (parse)
			cout << "AJOUTE : " + url << endl;
		else
			cout << "ECHEC : " + url << endl;
	}

	void CrawlerService::SetTagWords()
	{
		tagWords.clear();
		tagWords.push_back("a");
		tagWords.push_back("meta");
		tagWords.push_back("title");
		tagWords.push_back("img");
		tagWords.push_back("h1");
		tagWords.push_back("h2");
		tagWords.push_back("h3");
		tagWords.push_back("h4");
		tagWords.push_back("h5");
		tagWords.push_back("h6");
		tagWords.push_back("strong");
		tagWords.push_back("b");
		tagWords.push_back("summary");
		tagWords.push_back("address");
		tagWords.push_back("article");
		tagWords.push_back("blockquote");
		tagWords.push_back("header");
		tagWords.push_back("big");
	}

#pragma endregion

#pragma region Functions

	vector<HtmlTagResult*>* CrawlerService::Explore(vector<HtmlEntity*>* document)
	{
		vector<HtmlTagResult*>* tagList = new vector<HtmlTagResult*>();
		for each (HtmlEntity* item in *document)
		{
			if (item->GetType() == 1)
			{
				HtmlTagResult* tag = new HtmlTagResult(static_cast<HtmlTag&>(*item));
				if (find(tagWords.begin(), tagWords.end(), tag->GetTagType()) != tagWords.end())
				{
					if (item->GetChildren() != NULL && item->GetChildren()->size() > 0)
					{
						vector<string> innerTextList;
						for each (HtmlEntity* ite in *item->GetChildren())
						{
							if (ite->GetType() == 2)
							{
								HtmlInnerText* tagInner = &static_cast<HtmlInnerText&>(*ite);
								innerTextList.push_back(tagInner->GetText());
							}
						}
						tag->SetInnerText(innerTextList);
						VectorExtension::FreeAll(innerTextList);
					}
					tagList->push_back(tag);
				}

				if (tag->GetChildren() != NULL && tag->GetChildren()->size() > 0)
				{
					vector<HtmlTagResult*>* subExploreResult = Explore(tag->GetChildren());
					tagList->insert(tagList->end(), subExploreResult->begin(), subExploreResult->end());
				}
			}
		}
		for (int i = document->size(); i > 0; i--)
		{
			delete document->at(i - 1);
		}
		delete document;
		return tagList;
	}

	string CrawlerService::GetUrlFromSameDomain(HtmlTagResult tag, const string& domainName, const string& currentUrl)
	{
		if (tag.GetTagType() != "a")
			return "";

		if (tag.GetAttributs().count("href") != 1)
			return "";

		string url = tag.GetAttributs()["href"];

		url = StringExtension::TrimSpace(url);
		if (url.find_first_of(' ') > -1)
			return "";

		int questionMarkIndex = url.find_first_of('?');
		if (questionMarkIndex > -1)
			url = url.substr(0, questionMarkIndex);

		int hashTagIndex = url.find_first_of('#');
		if (hashTagIndex > -1)
			url = url.substr(0, hashTagIndex);

		if (StringExtension::EndsWith(url, "/"))
			url = url.substr(0, url.length() - 1);

		if (StringExtension::EndsWith(url, ":{}"))
			return "";

		url = AddDomainToString(url, currentUrl);
		if (GetDomainName(url) == domainName)
			return url;
		return "";
	}

	string CrawlerService::GetDomainName(const string& url)
	{
		if (url.find_first_of(' ') > -1)
			return "";
		vector<string> slashSplit = StringExtension::Split(url, '/');
		if (slashSplit.size() >= 3)
		{
			string domainName = slashSplit[2];
			int questionMarkIndex = domainName.find_first_of('?');

			if (questionMarkIndex > -1)
				domainName = domainName.substr(0, questionMarkIndex);
			if (StringExtension::Occurences(domainName, ".") > 1)
			{
				int pointIndex = domainName.find_first_of('.');
				domainName = domainName.substr(pointIndex + 1, domainName.size() - pointIndex - 1);
				pointIndex = 0;
			}
			questionMarkIndex = 0;
			VectorExtension::FreeAll(slashSplit);
			return domainName;
		}
		VectorExtension::FreeAll(slashSplit);
		return "";
	}

	string CrawlerService::AddDomainToString(string urlPart, string currentUrl)
	{
		int charIndex = urlPart.find(' ');
		if (charIndex > -1)
			return "";
		charIndex = urlPart.find('\'');
		if (charIndex > -1)
			return "";
		charIndex = 0;
		if (urlPart == "")
			return currentUrl;
		if (StringExtension::StartsWith(urlPart, "//"))
			urlPart = "http:" + urlPart;
		if (!StringExtension::StartsWith(urlPart, "http://") && !StringExtension::StartsWith(urlPart, "https://") && !StringExtension::StartsWith(urlPart, "ftp://"))
		{
			if (StringExtension::StartsWith(urlPart, "/"))
				urlPart = currentUrl + urlPart;
			else
				urlPart = currentUrl + "/" + urlPart;
		}
		VectorExtension::FreeAll(currentUrl);
		return urlPart;
	}

	bool CrawlerService::FindConcurrencyVector(concurrent_vector<string> vect, const string& query)
	{
		if (vect.empty())
			return false;

		bool found = false;
		auto url = begin(vect);
		do
		{
			if (*url == query)
				found = true;
			++url;
		} while (url != end(vect) && !found);
		VectorExtension::FreeAll(vect);
		return found;
	}

#pragma endregion

}