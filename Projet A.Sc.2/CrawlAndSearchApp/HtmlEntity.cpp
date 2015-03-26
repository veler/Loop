#include "stdafx.h"
#include "HtmlEntity.h"


namespace Parser
{

#pragma region Constructors

	HtmlEntity::HtmlEntity(int t)
	{
		type = t;
		children = new vector<HtmlEntity*>();
	}

	HtmlEntity::~HtmlEntity()
	{
		type = 0;
	}

#pragma endregion

#pragma region Methods

	void HtmlEntity::SetChildren(vector<HtmlEntity*>* vector)
	{
		children = vector;
	}

	int HtmlEntity::GetType()
	{
		return type;
	}

#pragma endregion

#pragma region Functions

	vector<HtmlEntity*>* HtmlEntity::GetChildren()
	{
		return children;
	}

#pragma endregion
}