#include "stdafx.h"
#include "FileUtility.h"


namespace System
{

#pragma region Constructors

	FileUtility::FileUtility()
	{
	}


	FileUtility::~FileUtility()
	{
	}

#pragma endregion

#pragma region Functions

	void FileUtility::DeleteAllFiles(const string& path)
	{
		CFileFind fnd;
		BOOL bfnd = fnd.FindFile(StringExtension::StringToWString(path).c_str() + CString("\\*.*"));
		while (bfnd)
		{
			bfnd = fnd.FindNextFile();
			Win32DeleteFile(fnd.GetFilePath());
		}
	}

	bool FileUtility::DirectoryExists(const string& path)
	{
		DWORD ftyp = GetFileAttributesA(path.c_str());
		if (ftyp == INVALID_FILE_ATTRIBUTES)
			return false;

		if (ftyp & FILE_ATTRIBUTE_DIRECTORY)
			return true;

		return false;
	}

	int FileUtility::FileExists(const string& path)
	{
		WIN32_FIND_DATA FindFileData;
		HANDLE handle = FindFirstFile(StringExtension::StringToWString(path).c_str(), &FindFileData);
		int found = handle != INVALID_HANDLE_VALUE;
		if (found)
			FindClose(handle);
		return found;
	}

#pragma endregion

}