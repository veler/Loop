#pragma once

#include "stdafx.h"
#include <iostream>
#include <string>
#include <windows.h>
#include <sql.h>
#include <sqlext.h>
#include <vector>
#include <thread>
#include <ppltasks.h>

#include "PageInternet.h"
#include "StringExtension.h"
#include "VectorExtension.h"
#include "FileUtility.h"

using namespace std;
using namespace concurrency;
using namespace System;

namespace SQL
{
	class SqlHelper
	{
	public:
		SqlHelper();
		~SqlHelper();
		void					Connect();
		void					Disconnect();

		vector<PageInternet>	SelectPageInternet();
		vector<PageInternet>	SelectPageInternet(const vector<string>& wordSearched);
		void					InsertPageInternet(PageInternet& page);
		void					DeletePageInternet(int pk);

		void					IncrementShareCount(string url);
		void					IncrementLikeCount(string url);
		void					IncrementViewCount(string url);
		void					IncrementPhotoShareCount(string url);
		void					IncrementPhotoLikeCount(string url);
		void					IncrementPhotoViewCount(string url);

		vector<Balises>			SelectBalises(int pkPageInternet = -1);
		void					InsertBalises(Balises& page);

		vector<Photo>			SelectPhoto(string colorToSearch = "All", string sizeToSearch = "All");
		void					InsertPhoto(Photo& page);

		vector<string>			SelectDomain();
		void					InsertDomain(const string& domainName);

		bool					UrlExists(const string& url, bool isPhoto = false);
		int						GetPageInternetPK(const string& url);
		int						GetPageInternetRowCount();
		int						GetPhotoRowCount();
		int						GetDomainRowCount();

		bool					GetConnected();

	private:

#if defined(_PRODUCTION)
		SQLWCHAR*		connectionString = TEXT("DRIVER={SQL Server Native Client 11.0};SERVER=VSQL;DATABASE=tweekbrosdatabase;UID=RobotApp;PWD=Supinf0;Trusted_Connection=yes");
#else
		SQLWCHAR*		connectionString = TEXT("DRIVER={SQL Server Native Client 11.0};SERVER=(localDb)\\v11.0;DATABASE=tweekbrosdatabase;UID=RobotApp;PWD=Supinf0;Trusted_Connection=yes");
#endif

		SQLHENV			handleEnvrironment;
		SQLHDBC			handleConnection;
		SQLWCHAR		errorMessage[512];
		SQLWCHAR		errorCode[8];
		SQLINTEGER		nativeErrorPtr;
		SQLSMALLINT		textLenghtPtr;
		bool			connected;

		void		DoInjection(const string& query);
		int			GetRowCount(const string& tableName);
		string		SqlCharToString(SQLTCHAR value[], size_t size);
	};
}
