#include "stdafx.h"
#include "PageInternet.h"


namespace SQL
{

#pragma region Constructors

	PageInternet::PageInternet()
	{
		relevance = -1;
	}

	PageInternet::PageInternet(string url, string title, string description, string keywords, int viewCount, int favoriteCount, int shareCount, int likeCount, long date)
	{
		SetUrl(url);
		SetTitle(title);
		SetDescription(description);
		SetKeywords(keywords);
		SetViewCount(viewCount);
		SetFavoriteCount(favoriteCount);
		SetShareCount(shareCount);
		SetLikeCount(likeCount);
		SetDate(date);
		relevance = -1;
	}

	PageInternet::~PageInternet()
	{
		VectorExtension::FreeAll(balises);
		VectorExtension::FreeAll(photo);
		VectorExtension::FreeAll(url);
		VectorExtension::FreeAll(title);
		VectorExtension::FreeAll(description);
		VectorExtension::FreeAll(keywords);
		PK_PageInternet = 0;
		favoriteCount = 0;
		shareCount = 0;
		likeCount = 0;
		date = 0;
		relevance = -1;
	}

#pragma endregion

#pragma region Setter

	void PageInternet::SetUrl(string value)
	{
		url = value;
	}

	void PageInternet::SetTitle(string value)
	{
		title = value;
	}

	void PageInternet::SetDescription(string value)
	{
		description = value;
	}

	void PageInternet::SetKeywords(string value)
	{
		keywords = value;
	}

	void PageInternet::SetViewCount(int value)
	{
		viewCount = value;
	}

	void PageInternet::SetFavoriteCount(int value)
	{
		favoriteCount = value;
	}

	void PageInternet::SetShareCount(int value)
	{
		shareCount = value;
	}

	void PageInternet::SetLikeCount(int value)
	{
		likeCount = value;
	}

	void PageInternet::SetDate(unsigned long long value)
	{
		date = value;
	}

	void PageInternet::SetBalises(vector<Balises> value)
	{
		balises = value;
	}

	void PageInternet::SetPhoto(vector<Photo> value)
	{
		photo = value;
	}

	void PageInternet::SetPK_PageInternet(int value)
	{
		PK_PageInternet = value;
	}

	void PageInternet::SetRelevance(int value)
	{
		relevance = value;
	}

	void PageInternet::AddBalise(Balises item)
	{
		balises.push_back(item);
	}

	void PageInternet::AddPhoto(Photo item)
	{
		photo.push_back(item);
	}

#pragma endregion

#pragma region Getter

	string PageInternet::GetUrl()
	{
		return url;
	}

	string PageInternet::GetTitle()
	{
		return title;
	}

	string PageInternet::GetDescription()
	{
		return description;
	}

	string PageInternet::GetKeywords()
	{
		return keywords;
	}

	int	PageInternet::GetViewCount()
	{
		return viewCount;
	}

	int	PageInternet::GetFavoriteCount()
	{
		return favoriteCount;
	}

	int	PageInternet::GetShareCount()
	{
		return shareCount;
	}

	int	PageInternet::GetLikeCount()
	{
		return likeCount;
	}

	unsigned long long PageInternet::GetDate()
	{
		return date;
	}

	int PageInternet::GetPK_PageInternet()
	{
		return PK_PageInternet;
	}

	vector<Balises> PageInternet::GetBalises()
	{
		return balises;
	}

	vector<Photo> PageInternet::GetPhoto()
	{
		return photo;
	}

	int	PageInternet::GetRelevance()
	{
		return relevance;
	}

#pragma endregion

#pragma region Operators

	bool PageInternet::operator < (PageInternet &value)
	{
		if (GetRelevance() == value.GetRelevance())
			return GetPK_PageInternet() > value.GetPK_PageInternet();
		else
			return GetRelevance() > value.GetRelevance();
	};

	PageInternet& PageInternet::operator = (PageInternet value)
	{
		SetBalises(value.GetBalises());
		SetDescription(value.GetDescription());
		SetKeywords(value.GetKeywords());
		SetViewCount(value.GetViewCount());
		SetFavoriteCount(value.GetFavoriteCount());
		SetShareCount(value.GetShareCount());
		SetLikeCount(value.GetLikeCount());
		SetDate(value.GetDate());
		SetPhoto(value.GetPhoto());
		SetPK_PageInternet(value.GetPK_PageInternet());
		SetTitle(value.GetTitle());
		SetUrl(value.GetUrl());
		relevance = value.GetRelevance();
		return *this;
	}

#pragma endregion

}