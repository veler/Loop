#include "stdafx.h"
#include "SearcherService.h"

namespace Searcher
{

#pragma region Constructors

	SearcherService::SearcherService(SQL::SqlHelper*& sqlArg)
	{
		sql = sqlArg;
	}


	SearcherService::~SearcherService()
	{
	}

#pragma endregion

#pragma region Methods

	void SearcherService::StartSearchService()
	{
		if (socketRunning)
			return;
		socketRunning = true;
	}

	void SearcherService::StopSearchService()
	{
		socketRunning = false;
	}

#pragma endregion

#pragma region Functions

	bool RelevanceIsZero(SQL::PageInternet page)
	{
		return page.GetRelevance() == 0;
	}

	bool RelevanceImageIsZero(SQL::Photo photo)
	{
		return photo.GetRelevance() == 0;
	}
	
	string SearcherService::DoSearch(string query)
	{
		if (!socketRunning)
			return "";

		query = StringExtension::ToLower(StringExtension::TrimSpace(query));
		vector<string> wordSearched = StringExtension::Split(query, ' ');

		vector<task<void>> tasks;
		vector<SQL::PageInternet> result;

		// Recherche sur la base de données
		result = sql->SelectPageInternet(wordSearched);

		if (result.size() > 0)
		{
			concurrent_vector<int> relevances;
			for (unsigned int i = 0; i <= result.size() - 1; i++)
			{
				task<void> t = create_task([this, &relevances, &result, i, &query, &wordSearched]
				{
					int relevance = CalculateRelevance(query, wordSearched, result[i]);
					relevances.push_back(relevance);
				});
				tasks.push_back(t);
			}
			auto joinTask = when_all(begin(tasks), end(tasks));
			joinTask.wait();

			for (unsigned int i = 0; i <= result.size() - 1; i++)
				result[i].SetRelevance(relevances[i]);

			sort(result.begin(), result.end()); // Trie les données selon l'attribut "relevance" (voir l'opérateur < de la class PageInternet)
			
			if (result.size() > 500)
			{
				for (int i = result.size() - 1; i >= 500; i--)
				{
					auto iterator = result.begin() + i;
					result.erase(iterator);
				}
			}

			auto it = find_if(result.begin(), result.end(), RelevanceIsZero);
			result.erase(it, result.end());
		}

		// Conversion en Json
		Json::Object root;
		Json::Array listOfPageInternet;

		for each (SQL::PageInternet page in result)
		{
			Json::Object pageJson;
			pageJson.Add("Url", page.GetUrl());
			pageJson.Add("Title", page.GetTitle());
			pageJson.Add("Description", page.GetDescription());
			pageJson.Add("ViewCount", page.GetViewCount());
			pageJson.Add("FavoriteCount", page.GetFavoriteCount());
			pageJson.Add("ShareCount", page.GetShareCount());
			pageJson.Add("LikeCount", page.GetLikeCount());
			listOfPageInternet.Add(pageJson.AsObject());
		}
		root.Add("Pages", listOfPageInternet);

		Json::Writer writer(root, Json::NoFormat);
		writer.Write();
		return writer.GetResult();
	}

	string SearcherService::DoImageSearch(string query, string color, string size)
	{
		if (!socketRunning)
			return "";

		query = StringExtension::ToLower(StringExtension::TrimSpace(query));
		vector<string> wordSearched = StringExtension::Split(query, ' ');

		vector<task<void>> tasks;
		vector<SQL::Photo> result;

		// Recherche sur la base de données
		result = sql->SelectPhoto(color, size);

		if (result.size() > 0)
		{
			concurrent_vector<int> relevances;
			for (unsigned int i = 0; i <= result.size() - 1; i++)
			{
				task<void> t = create_task([this, &relevances, &result, i, &query, &wordSearched]
				{
					int relevance = CalculateImageRelevance(query, wordSearched, result[i]);
					relevances.push_back(relevance);
				});
				tasks.push_back(t);
			}
			auto joinTask = when_all(begin(tasks), end(tasks));
			joinTask.wait();

			for (unsigned int i = 0; i <= result.size() - 1; i++)
				result[i].SetRelevance(relevances[i]);

			sort(result.begin(), result.end()); // Trie les données selon l'attribut "relevance" (voir l'opérateur < de la class PageInternet)

			if (result.size() > 500)
			{
				for (int i = result.size() - 1; i >= 500; i--)
				{
					auto iterator = result.begin() + i;
					result.erase(iterator);
				}
			}

			auto it = find_if(result.begin(), result.end(), RelevanceImageIsZero);
			result.erase(it, result.end());
		}										   	   

		// Conversion en Json
		Json::Object root;
		Json::Array listOfPhotos;

		for each (SQL::Photo photo in result)
		{
			Json::Object pageJson;
			pageJson.Add("Url", photo.GetUrl());
			pageJson.Add("Title", photo.GetTitle());
			pageJson.Add("Description", photo.GetDescription());
			pageJson.Add("ViewCount", photo.GetViewCount());
			pageJson.Add("ShareCount", photo.GetShareCount());
			pageJson.Add("LikeCount", photo.GetLikeCount());
			listOfPhotos.Add(pageJson.AsObject());
		}
		root.Add("Photos", listOfPhotos);

		Json::Writer writer(root, Json::NoFormat);
		writer.Write();
		return writer.GetResult();
	}

	int SearcherService::CalculateRelevance(const string& query, const vector<string>& wordSearched, SQL::PageInternet& page)
	{
		if (page.GetRelevance() > -1)
			return page.GetRelevance();

		vector<task<void>> tasks;
		int opinion = 0;
		int keywordsIndice = 0;
		int descriptionIndice = 0;
		int titleIndice = 0;
		int urlIndice = 0;
		int innerTextIndice = 0;
		string keywords = regex_replace(StringExtension::ToLower(page.GetKeywords()), regex("\\s+"), ""); // supprime ' ' et '/t'
		string description = StringExtension::ToLower(page.GetDescription());
		string title = StringExtension::ToLower(page.GetTitle());

		// Notation
		opinion += page.GetViewCount() * viewCountCoef;
		opinion += page.GetFavoriteCount() * favoriteCountCoef;
		opinion += page.GetShareCount() * shareCountCoef;
		opinion += page.GetLikeCount() * likeCountCoef;


		// Mots clés, description, titre, url
		for (auto &keyword : wordSearched)
		{
			keywordsIndice += StringExtension::Occurences(keywords, keyword) * keywordCoef;
			descriptionIndice += StringExtension::Occurences(description, keyword) * descriptionCoef;
			titleIndice += StringExtension::Occurences(title, keyword) * titleCoef;
			urlIndice += StringExtension::Occurences(page.GetUrl(), keyword) * urlCoef;
		}

		descriptionIndice += StringExtension::Occurences(description, query) * descriptionCorrectCoef;
		titleIndice += StringExtension::Occurences(title, query) * titleCorrectCoef;
		urlIndice += StringExtension::Occurences(page.GetUrl(), query) * urlCorrectCoef;

		// Balises
		for (auto &tag : page.GetBalises())
		{
			int coef = 1;
			if (tag.GetType() == "h1")
				coef = h1Coef;
			else if (tag.GetType() == "h2")
				coef = h2Coef;
			else if (tag.GetType() == "h3")
				coef = h3Coef;
			else if (tag.GetType() == "h4")
				coef = h4Coef;
			else if (tag.GetType() == "h5")
				coef = h5Coef;
			else if (tag.GetType() == "h6")
				coef = h6Coef;
			else if (tag.GetType() == "b" || tag.GetType() == "strong")
				coef = bStrongCoef;
			else if (tag.GetType() == "span")
				coef = spanCoef;

			string text = StringExtension::ToLower(tag.GetContent());
			innerTextIndice += StringExtension::Occurences(text, query) * coef;
			for (auto &keyword : wordSearched)
				innerTextIndice += StringExtension::Occurences(text, keyword) * coef;
		}
		return opinion + keywordsIndice + descriptionIndice + titleIndice + urlIndice + innerTextIndice;
	}

	int SearcherService::CalculateImageRelevance(const string& query, const vector<string>& wordSearched, SQL::Photo& photo)
	{
		if (photo.GetRelevance() > -1)
			return photo.GetRelevance();

		vector<task<void>> tasks;
		int opinion = 0;
		int keywordsIndice = 0;
		int descriptionIndice = 0;
		int titleIndice = 0;
		int urlIndice = 0;
		int innerTextIndice = 0;
		string description = StringExtension::ToLower(photo.GetDescription());
		string title = StringExtension::ToLower(photo.GetTitle());

		// Notation
		opinion += photo.GetViewCount() * viewCountCoef;
		opinion += photo.GetShareCount() * shareCountCoef;
		opinion += photo.GetLikeCount() * likeCountCoef;


		// description, titre, url
		for (auto &keyword : wordSearched)
		{
			descriptionIndice += StringExtension::Occurences(description, keyword) * descriptionCoef;
			titleIndice += StringExtension::Occurences(title, keyword) * titleCoef;
			urlIndice += StringExtension::Occurences(photo.GetUrl(), keyword) * urlCoef;
		}

		descriptionIndice += StringExtension::Occurences(description, query) * descriptionCorrectCoef;
		titleIndice += StringExtension::Occurences(title, query) * titleCorrectCoef;
		urlIndice += StringExtension::Occurences(photo.GetUrl(), query) * urlCorrectCoef;

		return opinion + keywordsIndice + descriptionIndice + titleIndice + urlIndice + innerTextIndice;
	}

#pragma endregion

}