#pragma once

#include <string>
#include <iostream>
#include <windows.h>
#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")

#include "Photo.h"
#include "StringExtension.h"
#include "FileUtility.h"
#include "SqlHelper.h"

using namespace std;
using namespace Gdiplus;
using namespace SQL;
using namespace System;


namespace Crawler
{
	class ImageUtility
	{
	public:
		ImageUtility();
		~ImageUtility();
		void GetPhotoInfo(string url, string title, string description, int id, SqlHelper*& sql);

	private:
		GdiplusStartupInput gdiplusStartupInput;
		ULONG_PTR			gdiplusToken;
	};
}
