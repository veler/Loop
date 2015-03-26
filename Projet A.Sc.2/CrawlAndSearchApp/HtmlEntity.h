#pragma once

#include <vector>


using namespace std;

namespace Parser
{
	class HtmlEntity
	{
	public:
		HtmlEntity(int t);
		~HtmlEntity();
		void				SetChildren(vector<HtmlEntity*>* vector);
		vector<HtmlEntity*>*	GetChildren();
		int					GetType();

	private:
		int					type; // 1 = tag; 2 = inner text ### Pas très propre mais plus performant qu'un Cast ici
		vector<HtmlEntity*>* children;
	};
}
