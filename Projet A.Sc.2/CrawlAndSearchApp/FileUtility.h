#pragma once

#include <string>
#include "StringExtension.h"

using namespace std;

namespace System
{
	class FileUtility
	{
	public:
		FileUtility();
		~FileUtility();
		static bool	DirectoryExists(const string& path);
		static int	FileExists(const string& path);
		static void	DeleteAllFiles(const string& path);
	};
}
