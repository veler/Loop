#pragma once

#include <vector>
#include <concurrent_vector.h>

using namespace std;

namespace System
{
	class VectorExtension
	{
	public:
		VectorExtension();
		~VectorExtension();

		template <typename T>
		static void FreeAll(T& to_clear)
		{
			T v;
			v.swap(to_clear);
		};
	};
}
