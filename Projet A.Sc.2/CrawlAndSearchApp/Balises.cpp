#include "stdafx.h"
#include "Balises.h"

namespace SQL
{

#pragma region Constructeur
	Balises::Balises()
	{

	}

	Balises::Balises(int FK_PageInternet, string type, string content)
	{
		SetFK_PageInternet(FK_PageInternet);
		SetType(type);
		SetContent(content);
	}

	Balises::~Balises()
	{
		VectorExtension::FreeAll(type);
		VectorExtension::FreeAll(content);
		FK_PageInternet = 0;
		PK_Balises = 0;
	}

#pragma endregion

#pragma region Setter

	void Balises::SetPK_Balises(int value)
	{
		PK_Balises = value;
	}

	void Balises::SetFK_PageInternet(int value)
	{
		FK_PageInternet = value;
	}

	void Balises::SetType(string value)
	{
		type = value;
	}

	void Balises::SetContent(string value)
	{
		content = value;
	}

#pragma endregion

#pragma region Getter
	
	int Balises::GetPK_Balises()
	{
		return PK_Balises;
	}

	int Balises::GetFK_PageInternet()
	{
		return FK_PageInternet;
	}

	string Balises::GetType()
	{
		return type;
	}

	string Balises::GetContent()
	{
		return content;
	}

#pragma endregion
}