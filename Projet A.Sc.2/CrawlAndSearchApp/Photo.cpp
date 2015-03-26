#include "stdafx.h"
#include "Photo.h"

namespace SQL
{

#pragma region Constructors

	Photo::Photo()
	{
		relevance = -1;
	}

	Photo::Photo(string url, string title, string description, int height, int width, string resolution, string color)
	{
		SetUrl(url);
		SetTitle(title);
		SetDescription(description);
		SetHeight(height);
		SetWidth(width);
		SetResolution(resolution);
		SetColor(color);
		viewCount = 0;
		shareCount = 0;
		likeCount = 0;
		relevance = -1;
	}

	Photo::~Photo()
	{
		VectorExtension::FreeAll(url);
		VectorExtension::FreeAll(title);
		VectorExtension::FreeAll(description);
		VectorExtension::FreeAll(resolution);
		VectorExtension::FreeAll(color);
		PK_Photo = 0;
		height = 0;
		width = 0;
		viewCount = 0;
		shareCount = 0;
		likeCount = 0;
		relevance = -1;
	}

#pragma endregion

#pragma region Setter

	void Photo::SetPK_Photo(int value)
	{
		PK_Photo = value;
	}

	void Photo::SetUrl(string value)
	{
		url = value;
	}

	void Photo::SetTitle(string value)
	{
		title = value;
	}

	void Photo::SetDescription(string value)
	{
		description = value;
	}

	void Photo::SetHeight(int value)
	{
		height = value;
	}

	void Photo::SetWidth(int value)
	{
		width = value;
	}

	void Photo::SetResolution(string value)
	{
		resolution = value;
	}

	void Photo::SetColor(string value)
	{
		color = value;
	}

	void Photo::SetViewCount(int value)
	{
		viewCount = value;
	}

	void Photo::SetShareCount(int value)
	{
		shareCount = value;
	}

	void Photo::SetLikeCount(int value)
	{
		likeCount = value;
	}

	void Photo::SetRelevance(int value)
	{
		relevance = value;
	}

#pragma endregion

#pragma region Getter

	int	Photo::GetPK_Photo()
	{
		return PK_Photo;
	}

	string Photo::GetUrl()
	{
		return url;
	}

	string Photo::GetTitle()
	{
		return title;
	}

	string Photo::GetDescription()
	{
		return description;
	}

	int	Photo::GetHeight()
	{
		return height;
	}

	int	Photo::GetWidth()
	{
		return width;
	}

	string Photo::GetResolution()
	{
		return resolution;
	}

	string Photo::GetColor()
	{
		return color;
	}

	int	Photo::GetViewCount()
	{
		return viewCount;
	}

	int	Photo::GetShareCount()
	{
		return shareCount;
	}

	int	Photo::GetLikeCount()
	{
		return likeCount;
	}

	int	Photo::GetRelevance()
	{
		return relevance;
	}

#pragma endregion 

#pragma region Operators

	bool Photo::operator< (Photo &value)
	{
		return GetRelevance() > value.GetRelevance();
	};

	Photo& Photo::operator = (Photo value)
	{
		SetDescription(value.GetDescription());
		SetViewCount(value.GetViewCount());
		SetShareCount(value.GetShareCount());
		SetLikeCount(value.GetLikeCount());
		SetPK_Photo(value.GetPK_Photo());
		SetTitle(value.GetTitle());
		SetUrl(value.GetUrl());
		SetHeight(value.GetHeight());
		SetWidth(value.GetWidth());
		SetResolution(value.GetResolution());
		SetColor(value.GetColor());
		relevance = value.GetRelevance();
		return *this;
	}

#pragma endregion

}


