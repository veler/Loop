#pragma once

#include <string>
#include "VectorExtension.h"

using namespace std;
using namespace System;

namespace SQL 
{
	class Photo
	{
	public:
		Photo();
		Photo(string url, string title, string description, int height, int width, string resolution, string color);
		~Photo();
		void	SetPK_Photo(int value);
		void	SetUrl(string value);
		void	SetTitle(string value);
		void	SetDescription(string value);
		void	SetHeight(int value);
		void	SetWidth(int value);
		void	SetResolution(string value);
		void	SetColor(string value);
		void	SetViewCount(int value);
		void	SetShareCount(int value);
		void	SetLikeCount(int value);
		void	SetRelevance(int value);
		int		GetPK_Photo();
		string	GetUrl();
		string	GetTitle();
		string	GetDescription();
		int		GetHeight();
		int		GetWidth();
		string	GetResolution();
		string	GetColor();
		int		GetViewCount();
		int		GetShareCount();
		int		GetLikeCount();
		int		GetRelevance();

		bool	operator< (Photo &value);
		Photo&	operator=(Photo value);

	private:
		int		PK_Photo;
		string	url;
		string	title;
		string	description;
		int		height;
		int		width;
		string	resolution;
		string	color;
		int		relevance;
		int		viewCount;
		int		shareCount;
		int		likeCount;
	};
}
