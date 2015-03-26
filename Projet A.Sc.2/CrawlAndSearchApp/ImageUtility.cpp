#include "stdafx.h"
#include "ImageUtility.h"


namespace Crawler
{

#pragma region Constructors

	ImageUtility::ImageUtility()
	{
		GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
	}


	ImageUtility::~ImageUtility()
	{
	}

#pragma endregion

#pragma region Functions

	void ImageUtility::GetPhotoInfo(string url, string title, string description, int id, SqlHelper*& sql)
	{
#if defined(_PRODUCTION)
		if (!FileUtility::DirectoryExists("M:\\"))
			return;
#endif

		url = StringExtension::TrimSpace(url);
		if (url.length() >= 4)
		{
			string lastChar = url.substr(url.length() - 4, 4);
			lastChar = StringExtension::ToLower(lastChar);
			if (lastChar != ".bmp" && lastChar != ".gif" && lastChar != ".jpg" && lastChar != "jpeg" && lastChar != ".png")
				return;
			VectorExtension::FreeAll(lastChar);
		}

		if (url == "")
			return;

		unsigned int red = 0;
		unsigned int green = 0;
		unsigned int blue = 0;
		int total = 0;
		bool majorRed;
		bool majorGreen;
		bool majorBlue;
		bool parse = false;
		int min = 10000;
		int max = 99999;
		Color pixel;
		Photo result;

		string filePath = "C:\\CrawlerTemp\\" + to_string(id) + ".img";

		HRESULT res = URLDownloadToFile(NULL, StringExtension::StringToWString(url).c_str(), StringExtension::StringToWString(filePath).c_str(), 0, NULL);

		if (FileUtility::FileExists(filePath))
		{
			if (res == S_OK)
				parse = true;
			else if (res == E_OUTOFMEMORY)
				cout << "Buffer length invalid, or insufficient memory" << endl;
			else if (res == INET_E_DOWNLOAD_FAILURE)
				cout << "URL is invalid" << endl;
			else
				cout << "Other error: " << res << endl; // ERROR 404
		}

		if (parse)
		{
			Bitmap* image = Bitmap::FromFile(StringExtension::StringToWString(filePath).c_str());

			if (image == NULL)
				return;
			else if (image->GetHeight() == 0)
			{
				delete image;
				image = 0;
				if (FileUtility::FileExists(filePath))
					remove(filePath.c_str());
				return;
			}

			result.SetUrl(url);
			result.SetHeight(image->GetHeight());
			result.SetWidth(image->GetWidth());

			if (result.GetHeight() < 200 && result.GetWidth() < 200)
				result.SetResolution("Small");
			else if (result.GetHeight() < 500 && result.GetWidth() < 500)
				result.SetResolution("Medium");
			else
				result.SetResolution("Large");

			for (unsigned int x = 0; x < image->GetWidth(); x++)
			{
				for (unsigned int y = 0; y < image->GetHeight(); y++)
				{
					image->GetPixel(x, y, &pixel);
					red += pixel.GetR();
					green += pixel.GetG();
					blue += pixel.GetB();
					total++;
				}
			}

			if (total > 0 && result.GetWidth() > 0 && result.GetHeight() > 0)
			{
				red /= total;
				green /= total;
				blue /= total;

				majorRed = red > 127.5;
				majorGreen = green > 127.5;
				majorBlue = blue > 127.5;

				if (majorRed && majorGreen && majorBlue) // Blanc
					result.SetColor("White");
				else if (!majorRed && !majorGreen && !majorBlue) // Noir
					result.SetColor("Black");
				else if (majorRed && !majorGreen && !majorBlue) // Rouge
					result.SetColor("Red");
				else if (!majorRed && !majorGreen && majorBlue) // Bleu
					result.SetColor("Blue");
				else if (!majorRed && majorGreen && !majorBlue) // Vert
					result.SetColor("Green");
				else if (majorRed && majorGreen && !majorBlue) // Jaune
					result.SetColor("Yellow");
				else if (majorRed && !majorGreen && majorBlue) // Magenta
					result.SetColor("Magenta");
				else if (!majorRed && majorGreen && majorBlue) // Cyan
					result.SetColor("Cyan");
				else
					result.SetColor("Unknow");
			}

			delete image;
			image = 0;
		}

		if (result.GetUrl() != "")
		{
			result.SetTitle(title);
			result.SetDescription(description);
			sql->InsertPhoto(result);
		}

		if (FileUtility::FileExists(filePath))
			remove(filePath.c_str());

		VectorExtension::FreeAll(filePath);
		VectorExtension::FreeAll(url);
		VectorExtension::FreeAll(title);
		VectorExtension::FreeAll(description);
	}

#pragma endregion

}