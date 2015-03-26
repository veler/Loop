#pragma once

#include <string>
#include <vector>
#include <regex>

#include "Balises.h"
#include "Photo.h"
#include "StringExtension.h"
#include "VectorExtension.h"

using namespace std;
using namespace System;

namespace SQL
{
	class PageInternet
	{
	public:
		PageInternet();
		PageInternet(string url, string title, string description, string keywords, int viewCount, int favoriteCount, int shareCount, int likeCount, long date);
		~PageInternet();
		void				SetPK_PageInternet(int value);
		void				SetUrl(string value);
		void				SetTitle(string value);
		void				SetDescription(string value);
		void				SetKeywords(string value);
		void				SetViewCount(int value);
		void				SetFavoriteCount(int value);
		void				SetShareCount(int value);
		void				SetLikeCount(int value);
		void				SetDate(unsigned long long value);
		void				SetBalises(vector<Balises> value);
		void				SetPhoto(vector<Photo> value);
		void				SetRelevance(int value);
		int					GetPK_PageInternet();
		string				GetUrl();
		string				GetTitle();
		string				GetDescription();
		string				GetKeywords();
		int					GetViewCount();
		int					GetFavoriteCount();
		int					GetShareCount();
		int					GetLikeCount();
		unsigned long long	GetDate();
		int					GetRelevance();
		vector<Balises>		GetBalises();
		vector<Photo>		GetPhoto();
		void				AddBalise(Balises item);
		void				AddPhoto(Photo item);

		bool				operator<(PageInternet &value);
		PageInternet&		operator=(PageInternet value);

	private:
		int					PK_PageInternet;
		string				url;
		string				title;
		string				description;
		string				keywords;
		int					viewCount;
		int					favoriteCount;
		int					shareCount;
		int					likeCount;
		int					relevance;
		unsigned long long	date;
		vector<Balises>		balises;
		vector<Photo>		photo;
	};
}
