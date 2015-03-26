#pragma once

#include <string>
#include "VectorExtension.h"

using namespace std;
using namespace System;

namespace SQL
{
	class Balises
	{
	public:
		Balises();
		Balises(int FK_PageInternet, string type, string content);
		~Balises();
		void	SetPK_Balises(int value);
		void	SetFK_PageInternet(int value);
		void	SetType(string value);
		void	SetContent(string value);
		int		GetPK_Balises();
		int		GetFK_PageInternet();
		string	GetType();
		string	GetContent();

	private:
		int		PK_Balises;
		int		FK_PageInternet;
		string	type;
		string	content;
	};
}
