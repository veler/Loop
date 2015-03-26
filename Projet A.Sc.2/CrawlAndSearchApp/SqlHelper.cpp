#include "stdafx.h"
#include "SqlHelper.h"

#include "afxwin.h"
#include "afxdialogex.h"
namespace SQL
{

#pragma region Constructors

	SqlHelper::SqlHelper()
	{
		ZeroMemory(errorMessage, 512);
		ZeroMemory(errorCode, 8);
	}

	SqlHelper::~SqlHelper()
	{
	}

#pragma endregion

#pragma region Methods

	void SqlHelper::Connect()
	{
		try
		{
			unsigned int result = -1;

			SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &handleEnvrironment);
			SQLSetEnvAttr(handleEnvrironment, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3_80, SQL_IS_INTEGER);
			SQLAllocHandle(SQL_HANDLE_DBC, handleEnvrironment, &handleConnection);
			result = SQLDriverConnect(handleConnection, NULL, connectionString, SQL_NTS, NULL, 0, 0, SQL_DRIVER_NOPROMPT);

			if (result == SQL_SUCCESS || result == SQL_SUCCESS_WITH_INFO)
				connected = true;
			else
			{
				SQLGetDiagRec(SQL_HANDLE_DBC, handleConnection, 1, errorCode, &nativeErrorPtr, errorMessage, 512, &textLenghtPtr);
				cout << "SQL Connection error : " << SqlCharToString(errorMessage, 512) << endl;
				Disconnect();
			}
		}
		catch (exception)
		{

		}
	}

	void SqlHelper::Disconnect()
	{
		try
		{
			SQLDisconnect(handleConnection);
			SQLFreeHandle(SQL_HANDLE_DBC, handleConnection);
			SQLFreeHandle(SQL_HANDLE_ENV, handleEnvrironment);
		}
		catch (exception)
		{

		}
		ZeroMemory(errorMessage, 512);
		ZeroMemory(errorCode, 8);
		connected = false;
	}

	void SqlHelper::DoInjection(const string& query)
	{
		if (!connected)
		{
#if defined(_PRODUCTION)
			if (FileUtility::DirectoryExists("M:\\"))
				Connect();
			else
			{
				Disconnect();
				return;
			}
#else
			Connect();
#endif
		}
		if (!connected)
		{
			cout << "SQL Injection error : disconnected." << endl;
			return;
		}
		unsigned int result;
		bool restart;
		do
		{
			restart = false;
			SQLHSTMT hStmt;
			SQLAllocStmt(handleConnection, &hStmt);
			result = SQLExecDirect(hStmt, (SQLTCHAR *)StringExtension::StringToWString(query).c_str(), SQL_NTS);
			if (result != SQL_SUCCESS && result != SQL_SUCCESS_WITH_INFO)
			{
				SQLGetDiagRec(SQL_HANDLE_STMT, hStmt, 1, errorCode, &nativeErrorPtr, errorMessage, 512, &textLenghtPtr);
				if (errorCode == StringExtension::StringToWString("HY000"))
					restart = true;
				else
					cout << "SQL Injection error : " << SqlCharToString(errorMessage, 512) << endl;
			}
			SQLCloseCursor(hStmt);
			SQLFreeStmt(hStmt, SQL_DROP);
		} while (restart);
	}

	void SqlHelper::InsertPageInternet(PageInternet& page)
	{
		if ((page.GetUrl() == "") || (page.GetUrl().empty()))
		{
			std::cout << "Des données sont manquantes" << endl;
			std::cout << "Url : " << page.GetUrl() << endl;
		}
		else
		{
			string query = "INSERT INTO PageInternet (URL, Title, Description, Keywords, ViewCount, FavoriteCount, ShareCount, LikeCount, Date) VALUES ('" + page.GetUrl() + "', '" + StringExtension::Replace(page.GetTitle(), "'", "''") + "', '" + StringExtension::Replace(page.GetDescription(), "'", "''") + "', '" + StringExtension::Replace(page.GetKeywords(), "'", "''") + "', " + to_string(page.GetViewCount()) + ", " + to_string(page.GetFavoriteCount()) + ", " + to_string(page.GetShareCount()) + ", " + to_string(page.GetLikeCount()) + ", " + to_string(page.GetDate()) + ");";
			DoInjection(query);
			VectorExtension::FreeAll(query);
		}
	}

	void SqlHelper::InsertPhoto(Photo& page)
	{
		if ((page.GetUrl() == "") || (page.GetUrl().empty()))
		{
			std::cout << "Des données sont manquantes" << endl;
			std::cout << "Url : " << page.GetUrl() << endl;
		}
		else
		{
			string query = "INSERT INTO Photo (URL, Title, Description, Height, Width, Resolution, Color, ViewCount, ShareCount, LikeCount) VALUES ('" + page.GetUrl() + "', '" + StringExtension::Replace(page.GetTitle(), "'", "''") + "', '" + StringExtension::Replace(page.GetDescription(), "'", "''") + "', " + to_string(page.GetHeight()) + ", " + to_string(page.GetWidth()) + ", '" + page.GetResolution() + "', '" + page.GetColor() + "', 0, 0, 0);";
			DoInjection(query);
			VectorExtension::FreeAll(query);
		}
	}

	void SqlHelper::InsertBalises(Balises& page)
	{
		if ((page.GetFK_PageInternet() == 0) || (page.GetType() == "") || (page.GetType().empty()) || (page.GetContent() == "") || (page.GetContent().empty()))
		{
			std::cout << "Des données sont manquantes" << endl;
			std::cout << "FK_PageInternet : " << page.GetFK_PageInternet() << endl;
			std::cout << "Type : " << page.GetType() << endl;
			std::cout << "Contenu : " << page.GetContent() << endl;
		}
		else
		{
			string query = "INSERT INTO Balises (FK_PageInternet, type, [content]) VALUES (" + to_string(page.GetFK_PageInternet()) + ", '" + page.GetType() + "', '" + StringExtension::Replace(page.GetContent(), "'", "''") + "');";
			DoInjection(query);
			VectorExtension::FreeAll(query);
		}
	}

	void SqlHelper::InsertDomain(const string& domainName)
	{
		string query = "INSERT INTO Domains (Domain) VALUES ('" + domainName + "');";
		DoInjection(query);
		VectorExtension::FreeAll(query);
	}

	void SqlHelper::DeletePageInternet(int pk)
	{
		string query;
		query = "DELETE FROM Balises WHERE FK_PageInternet = " + to_string(pk) + ";";
		DoInjection(query);
		query = "DELETE FROM Photo WHERE FK_PageInternet = " + to_string(pk) + ";";
		DoInjection(query);
		query = "DELETE FROM PageInternet WHERE PK_PageInternet = " + to_string(pk) + ";";
		DoInjection(query);
		VectorExtension::FreeAll(query);
	}

	void SqlHelper::IncrementShareCount(string url)
	{
		string query = "UPDATE PageInternet SET ShareCount = ShareCount + 1 WHERE URL = '" + url + "';";
		DoInjection(query);
		VectorExtension::FreeAll(query);
	}

	void SqlHelper::IncrementLikeCount(string url)
	{
		string query = "UPDATE PageInternet SET LikeCount = LikeCount + 1 WHERE URL = '" + url + "';";
		DoInjection(query);
		VectorExtension::FreeAll(query);
	}

	void SqlHelper::IncrementViewCount(string url)
	{
		string query = "UPDATE PageInternet SET ViewCount = ViewCount + 1 WHERE URL = '" + url + "';";
		DoInjection(query);
		VectorExtension::FreeAll(query);
	}

	void SqlHelper::IncrementPhotoShareCount(string url)
	{
		string query = "UPDATE Photo SET ShareCount = ShareCount + 1 WHERE URL = '" + url + "';";
		DoInjection(query);
		VectorExtension::FreeAll(query);
	}

	void SqlHelper::IncrementPhotoLikeCount(string url)
	{
		string query = "UPDATE Photo SET LikeCount = LikeCount + 1 WHERE URL = '" + url + "';";
		DoInjection(query);
		VectorExtension::FreeAll(query);
	}

	void SqlHelper::IncrementPhotoViewCount(string url)
	{
		string query = "UPDATE Photo SET ViewCount = ViewCount + 1 WHERE URL = '" + url + "';";
		DoInjection(query);
		VectorExtension::FreeAll(query);
	}

#pragma endregion

#pragma region Functions

	vector<PageInternet> SqlHelper::SelectPageInternet()
	{
		vector<string> arg;
		return SelectPageInternet(arg);
	}

	vector<PageInternet> SqlHelper::SelectPageInternet(const vector<string>& wordSearched)
	{
		if (!connected)
		{
#if defined(_PRODUCTION)
			if (FileUtility::DirectoryExists("M:\\"))
				Connect();
			else
			{
				Disconnect();
				return vector<PageInternet>();
			}
#else
			Connect();
#endif
		}
		if (!connected)
		{
			cout << "SQL Query PageInternet error : disconnected." << endl;
			return vector<PageInternet>();
		}

		vector<PageInternet> resultVector;
		unsigned int result;
		SQLHSTMT hStmt;
		SQLINTEGER iJnk = NULL;
		SQLINTEGER pkPageInternet = NULL;
		char url[1024];
		char title[1024];
		char description[1024];
		char keywords[1024];
		char type[20];
		char content[1024];
		SQLINTEGER viewCount = NULL;
		SQLINTEGER favoriteCount = NULL;
		SQLINTEGER shareCount = NULL;
		SQLINTEGER likeCount = NULL;
		SQLINTEGER date = NULL;
		string query;
		memset(url, 0, 1024);
		memset(title, 0, 1024);
		memset(description, 0, 1024);
		memset(keywords, 0, 1024);

		query = "SELECT DISTINCT PK_PageInternet, PageInternet.URL, PageInternet.Title, PageInternet.Description, PageInternet.Keywords, PageInternet.ViewCount, PageInternet.FavoriteCount, PageInternet.ShareCount, PageInternet.LikeCount, PageInternet.Date, Balises.type, Balises.[content] FROM PageInternet INNER JOIN Balises ON PageInternet.PK_PageInternet = Balises.FK_PageInternet ORDER BY PageInternet.PK_PageInternet;";

		bool restart;
		do
		{
			restart = false;
			VectorExtension::FreeAll(resultVector);
			SQLAllocStmt(handleConnection, &hStmt);
			SQLBindCol(hStmt, 1, SQL_C_LONG, &pkPageInternet, sizeof(pkPageInternet), &iJnk);
			SQLBindCol(hStmt, 2, SQL_C_CHAR, &url, sizeof(url), &iJnk);
			SQLBindCol(hStmt, 3, SQL_C_CHAR, &title, sizeof(title), &iJnk);
			SQLBindCol(hStmt, 4, SQL_C_CHAR, &description, sizeof(description), &iJnk);
			SQLBindCol(hStmt, 5, SQL_C_CHAR, &keywords, sizeof(keywords), &iJnk);
			SQLBindCol(hStmt, 6, SQL_C_LONG, &viewCount, sizeof(viewCount), &iJnk);
			SQLBindCol(hStmt, 7, SQL_C_LONG, &favoriteCount, sizeof(favoriteCount), &iJnk);
			SQLBindCol(hStmt, 8, SQL_C_LONG, &shareCount, sizeof(shareCount), &iJnk);
			SQLBindCol(hStmt, 9, SQL_C_LONG, &likeCount, sizeof(likeCount), &iJnk);
			SQLBindCol(hStmt, 10, SQL_C_LONG, &date, sizeof(date), &iJnk);
			SQLBindCol(hStmt, 11, SQL_C_CHAR, &type, sizeof(type), &iJnk);
			SQLBindCol(hStmt, 12, SQL_C_CHAR, &content, sizeof(content), &iJnk);

			result = SQLExecDirect(hStmt, (SQLTCHAR *)StringExtension::StringToWString(query).c_str(), SQL_NTS);
			if (result == SQL_SUCCESS || result == SQL_SUCCESS_WITH_INFO)
			{
				int pkPageOld = -1;
				PageInternet item;
				while (SQLFetch(hStmt) != SQL_NO_DATA)
				{
					if (pkPageOld != pkPageInternet)
					{
						if (pkPageOld > -1)
							resultVector.push_back(item);
						item = PageInternet(url, title, description, keywords, viewCount, favoriteCount, shareCount, likeCount, date);
						item.SetPK_PageInternet(pkPageInternet);
						pkPageOld = pkPageInternet;
					}
					else
					{
						item.AddBalise(Balises(0, type, content));
					}
				}
			}
			else
			{
				SQLGetDiagRec(SQL_HANDLE_STMT, hStmt, 1, errorCode, &nativeErrorPtr, errorMessage, 512, &textLenghtPtr);
				if (errorCode == StringExtension::StringToWString("HY000"))
					restart = true;
				else
					cout << "SQL Query PageInternet error : " << SqlCharToString(errorMessage, 512) << endl;
			}
			SQLCloseCursor(hStmt);
			SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
		} while (restart);

		VectorExtension::FreeAll(query);
		memset(url, 0, 1024);
		memset(title, 0, 1024);
		memset(description, 0, 1024);
		memset(keywords, 0, 1024);

		return resultVector;
	}

	vector<Balises> SqlHelper::SelectBalises(int pkPageInternet)
	{
		if (!connected)
		{
#if defined(_PRODUCTION)
			if (FileUtility::DirectoryExists("M:\\"))
				Connect();
			else
			{
				Disconnect();
				return vector<Balises>();
			}
#else
			Connect();
#endif
		}
		if (!connected)
		{
			cout << "SQL Query Balises error : disconnected." << endl;
			return vector<Balises>();
		}

		vector<Balises> resultVector;
		unsigned int result;
		SQLHSTMT hStmt;
		SQLINTEGER iJnk = NULL;
		SQLINTEGER PK_Balises = NULL;
		SQLINTEGER FK_PageInternet = NULL;
		char type[20];
		char content[1024];
		string query;
		memset(type, 0, 20);
		memset(content, 0, 1024);

		if (pkPageInternet > -1)
			query = "SELECT PK_Balises, FK_PageInternet, type, [content] FROM Balises WHERE FK_PageInternet = " + to_string(pkPageInternet) + ";";
		else
			query = "SELECT PK_Balises, FK_PageInternet, type, [content] FROM Balises;";

		bool restart;
		do
		{
			restart = false;
			VectorExtension::FreeAll(resultVector);
			SQLAllocStmt(handleConnection, &hStmt);
			SQLBindCol(hStmt, 1, SQL_C_LONG, &PK_Balises, sizeof(PK_Balises), &iJnk);
			SQLBindCol(hStmt, 2, SQL_C_LONG, &FK_PageInternet, sizeof(FK_PageInternet), &iJnk);
			SQLBindCol(hStmt, 3, SQL_C_CHAR, &type, sizeof(type), &iJnk);
			SQLBindCol(hStmt, 4, SQL_C_CHAR, &content, sizeof(content), &iJnk);

			result = SQLExecDirect(hStmt, (SQLTCHAR *)StringExtension::StringToWString(query).c_str(), SQL_NTS);
			if (result == SQL_SUCCESS || result == SQL_SUCCESS_WITH_INFO)
			{
				while (SQLFetch(hStmt) != SQL_NO_DATA)
				{
					Balises item(FK_PageInternet, type, content);
					item.SetPK_Balises(PK_Balises);
					resultVector.push_back(item);
				}
			}
			else
			{
				SQLGetDiagRec(SQL_HANDLE_STMT, hStmt, 1, errorCode, &nativeErrorPtr, errorMessage, 512, &textLenghtPtr);
				if (errorCode == StringExtension::StringToWString("HY000"))
					restart = true;
				else
					cout << "SQL Query Balises error : " << SqlCharToString(errorMessage, 512) << endl;
			}
			SQLCloseCursor(hStmt);
			SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
		} while (restart);

		VectorExtension::FreeAll(query);
		memset(type, 0, 20);
		memset(content, 0, 1024);

		return resultVector;
	}

	vector<Photo> SqlHelper::SelectPhoto(string colorToSearch, string sizeToSearch)
	{
		if (!connected)
		{
#if defined(_PRODUCTION)
			if (FileUtility::DirectoryExists("M:\\"))
				Connect();
			else
			{
				Disconnect();
				return vector<Photo>();
			}
#else
			Connect();
#endif
		}
		if (!connected)
		{
			cout << "SQL Query Photo error : disconnected." << endl;
			return vector<Photo>();
		}

		vector<Photo> resultVector;
		unsigned int result;
		SQLHSTMT hStmt;
		SQLINTEGER iJnk = NULL;
		SQLINTEGER PK_Photo = NULL;
		char url[1024];
		char title[1024];
		char description[1024];
		SQLINTEGER height = NULL;
		SQLINTEGER width = NULL;
		SQLINTEGER viewCount = NULL;
		SQLINTEGER shareCount = NULL;
		SQLINTEGER likeCount = NULL;
		char resolution[20];
		char color[20];
		string query;
		memset(url, 0, 1024);
		memset(title, 0, 1024);
		memset(description, 0, 1024);
		memset(resolution, 0, 20);
		memset(color, 0, 20);

		if (colorToSearch == "All" && sizeToSearch == "All")
			query = "SELECT PK_Photo, URL, Title, Description, Height, Width, Resolution, Color FROM Photo ORDER BY PK_Photo;";
		else if (colorToSearch != "All" && sizeToSearch == "All")
			query = "SELECT PK_Photo, URL, Title, Description, Height, Width, Resolution, Color FROM Photo WHERE Color = '" + colorToSearch + "' ORDER BY PK_Photo;";
		else if (colorToSearch == "All" && sizeToSearch != "All")
			query = "SELECT PK_Photo, URL, Title, Description, Height, Width, Resolution, Color FROM Photo WHERE Resolution = '" + sizeToSearch + "' ORDER BY PK_Photo;";
		else if (colorToSearch != "All" && sizeToSearch != "All")
			query = "SELECT PK_Photo, URL, Title, Description, Height, Width, Resolution, Color FROM Photo WHERE Resolution = '" + sizeToSearch + "' AND Color = '" + colorToSearch + "' ORDER BY PK_Photo;";

		bool restart;
		do
		{
			restart = false;
			VectorExtension::FreeAll(resultVector);
			SQLAllocStmt(handleConnection, &hStmt);
			SQLBindCol(hStmt, 1, SQL_C_LONG, &PK_Photo, sizeof(PK_Photo), &iJnk);
			SQLBindCol(hStmt, 2, SQL_C_CHAR, &url, sizeof(url), &iJnk);
			SQLBindCol(hStmt, 3, SQL_C_CHAR, &title, sizeof(title), &iJnk);
			SQLBindCol(hStmt, 4, SQL_C_CHAR, &description, sizeof(description), &iJnk);
			SQLBindCol(hStmt, 5, SQL_C_LONG, &height, sizeof(height), &iJnk);
			SQLBindCol(hStmt, 6, SQL_C_LONG, &width, sizeof(width), &iJnk);
			SQLBindCol(hStmt, 7, SQL_C_CHAR, &resolution, sizeof(resolution), &iJnk);
			SQLBindCol(hStmt, 8, SQL_C_CHAR, &color, sizeof(color), &iJnk);
			SQLBindCol(hStmt, 9, SQL_C_LONG, &viewCount, sizeof(viewCount), &iJnk);
			SQLBindCol(hStmt, 10, SQL_C_LONG, &shareCount, sizeof(shareCount), &iJnk);
			SQLBindCol(hStmt, 11, SQL_C_LONG, &likeCount, sizeof(likeCount), &iJnk);

			result = SQLExecDirect(hStmt, (SQLTCHAR *)StringExtension::StringToWString(query).c_str(), SQL_NTS);
			if (result == SQL_SUCCESS || result == SQL_SUCCESS_WITH_INFO)
			{
				while (SQLFetch(hStmt) != SQL_NO_DATA)
				{
					Photo item(url, title, description, height, width, resolution, color);
					item.SetLikeCount(likeCount);
					item.SetShareCount(shareCount);
					item.SetViewCount(viewCount);
					item.SetPK_Photo(PK_Photo);
					resultVector.push_back(item);
				}
			}
			else
			{
				SQLGetDiagRec(SQL_HANDLE_STMT, hStmt, 1, errorCode, &nativeErrorPtr, errorMessage, 512, &textLenghtPtr);
				if (errorCode == StringExtension::StringToWString("HY000"))
					restart = true;
				else
					cout << "SQL Query PageInternet error : " << SqlCharToString(errorMessage, 512) << endl;
			}
			SQLCloseCursor(hStmt);
			SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
		} while (restart);

		VectorExtension::FreeAll(query);
		memset(url, 0, 1024);
		memset(title, 0, 1024);
		memset(description, 0, 1024);
		memset(resolution, 0, 20);
		memset(color, 0, 20);

		return resultVector;
	}

	vector<string> SqlHelper::SelectDomain()
	{
		if (!connected)
		{
#if defined(_PRODUCTION)
			if (FileUtility::DirectoryExists("M:\\"))
				Connect();
			else
			{
				Disconnect();
				return vector<string>();
}
#else
			Connect();
#endif
		}
		if (!connected)
		{
			cout << "SQL Query Domain error : disconnected." << endl;
			return vector<string>();
		}

		vector<string> resultVector;
		unsigned int result;
		SQLHSTMT hStmt;
		SQLINTEGER iJnk = NULL;
		char domain[1024];
		string query;
		memset(domain, 0, 1024);

		query = "SELECT Domain FROM Domains;";

		bool restart;
		do
		{
			restart = false;
			VectorExtension::FreeAll(resultVector);
			SQLAllocStmt(handleConnection, &hStmt);
			SQLBindCol(hStmt, 1, SQL_C_CHAR, &domain, sizeof(domain), &iJnk);

			result = SQLExecDirect(hStmt, (SQLTCHAR *)StringExtension::StringToWString(query).c_str(), SQL_NTS);
			if (result == SQL_SUCCESS || result == SQL_SUCCESS_WITH_INFO)
			{
				while (SQLFetch(hStmt) != SQL_NO_DATA)
					resultVector.push_back(domain);
			}
			else
			{
				SQLGetDiagRec(SQL_HANDLE_STMT, hStmt, 1, errorCode, &nativeErrorPtr, errorMessage, 512, &textLenghtPtr);
				if (errorCode == StringExtension::StringToWString("HY000"))
					restart = true;
				else
					cout << "SQL Query Domain error : " << SqlCharToString(errorMessage, 512) << endl;
			}
			SQLCloseCursor(hStmt);
			SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
		} while (restart);

		VectorExtension::FreeAll(query);
		memset(domain, 0, 1024);

		return resultVector;
	}

	bool SqlHelper::UrlExists(const string& url, bool isPhoto)
	{
		if (!connected)
		{
#if defined(_PRODUCTION)
			if (FileUtility::DirectoryExists("M:\\"))
				Connect();
			else
			{
				Disconnect();
				return true;
			}
#else
			Connect();
#endif
		}
		if (!connected)
		{
			cout << "SQL Query URL Exists error : disconnected." << endl;
			return true;
		}

		int resultCount = 0;
		unsigned int result;
		SQLHSTMT hStmt;
		SQLINTEGER iJnk = NULL;
		SQLINTEGER urlCount = NULL;
		string query;

		if (isPhoto)
			query = "SELECT DISTINCT COUNT(URL) FROM Photo WHERE URL LIKE '" + url + "';";
		else
			query = "SELECT DISTINCT COUNT(URL) FROM PageInternet WHERE URL LIKE '" + url + "';";

		bool restart;
		do
		{
			restart = false;
			SQLAllocStmt(handleConnection, &hStmt);
			SQLBindCol(hStmt, 1, SQL_C_LONG, &urlCount, sizeof(urlCount), &iJnk);

			result = SQLExecDirect(hStmt, (SQLTCHAR *)StringExtension::StringToWString(query).c_str(), SQL_NTS);
			if (result == SQL_SUCCESS || result == SQL_SUCCESS_WITH_INFO)
				while (SQLFetch(hStmt) != SQL_NO_DATA)
					resultCount = urlCount;
			else
			{
				SQLGetDiagRec(SQL_HANDLE_STMT, hStmt, 1, errorCode, &nativeErrorPtr, errorMessage, 512, &textLenghtPtr);
				if (errorCode == StringExtension::StringToWString("HY000"))
					restart = true;
				else
					cout << "SQL Query URL Exists error : " << SqlCharToString(errorMessage, 512) << endl;
			}
			SQLCloseCursor(hStmt);
			SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
		} while (restart);
		VectorExtension::FreeAll(query);

		return resultCount > 0;
	}

	int	SqlHelper::GetPageInternetPK(const string& url)
	{
		if (!connected)
		{
#if defined(_PRODUCTION)
			if (FileUtility::DirectoryExists("M:\\"))
				Connect();
			else
			{
				Disconnect();
				return 0;
			}
#else
			Connect();
#endif
		}
		if (!connected)
		{
			cout << "SQL Query PageInternet Get Next PK error : disconnected." << endl;
			return 0;
		}

		int resultPk = -1;
		unsigned int result;
		SQLHSTMT hStmt;
		SQLINTEGER iJnk = NULL;
		SQLINTEGER pK = NULL;
		string query;

		query = "SELECT TOP 1 PK_PageInternet FROM PageInternet WHERE URL LIKE '" + url + "' ORDER BY PK_PageInternet DESC;";

		bool restart;
		do
		{
			restart = false;
			SQLAllocStmt(handleConnection, &hStmt);
			SQLBindCol(hStmt, 1, SQL_C_LONG, &pK, sizeof(pK), &iJnk);

			result = SQLExecDirect(hStmt, (SQLTCHAR *)StringExtension::StringToWString(query).c_str(), SQL_NTS);
			if (result == SQL_SUCCESS || result == SQL_SUCCESS_WITH_INFO)
				while (SQLFetch(hStmt) != SQL_NO_DATA)
					resultPk = pK;
			else
			{
				SQLGetDiagRec(SQL_HANDLE_STMT, hStmt, 1, errorCode, &nativeErrorPtr, errorMessage, 512, &textLenghtPtr);
				if (errorCode == StringExtension::StringToWString("HY000"))
					restart = true;
				else
					cout << "SQL Query PageInternet Get Next PK error : " << SqlCharToString(errorMessage, 512) << endl;
			}
			SQLCloseCursor(hStmt);
			SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
		} while (restart);
		VectorExtension::FreeAll(query);

		return resultPk;
	}

	bool SqlHelper::GetConnected()
	{
		return connected;
	}

	int	SqlHelper::GetPageInternetRowCount()
	{
		return GetRowCount("PageInternet");
	}

	int	SqlHelper::GetPhotoRowCount()
	{
		return GetRowCount("Photo");
	}

	int	SqlHelper::GetDomainRowCount()
	{
		return GetRowCount("Domains");
	}

	int	SqlHelper::GetRowCount(const string& tableName)
	{
		if (!connected)
		{
#if defined(_PRODUCTION)
			if (FileUtility::DirectoryExists("M:\\"))
				Connect();
			else
			{
				Disconnect();
				return 0;
			}
#else
			Connect();
#endif
	}
		if (!connected)
		{
			cout << "SQL Query Get Row Count error : disconnected." << endl;
			return 0;
		}

		int resultCount = 0;
		unsigned int result;
		SQLHSTMT hStmt;
		SQLINTEGER iJnk = NULL;
		SQLINTEGER count = NULL;
		string query;

		query = "SELECT COUNT(*) FROM " + tableName + ";";

		bool restart;
		do
		{
			restart = false;
			SQLAllocStmt(handleConnection, &hStmt);
			SQLBindCol(hStmt, 1, SQL_C_LONG, &count, sizeof(count), &iJnk);

			result = SQLExecDirect(hStmt, (SQLTCHAR *)StringExtension::StringToWString(query).c_str(), SQL_NTS);
			if (result == SQL_SUCCESS || result == SQL_SUCCESS_WITH_INFO)
				while (SQLFetch(hStmt) != SQL_NO_DATA)
					resultCount = count;
			else
			{
				SQLGetDiagRec(SQL_HANDLE_STMT, hStmt, 1, errorCode, &nativeErrorPtr, errorMessage, 512, &textLenghtPtr);
				if (errorCode == StringExtension::StringToWString("HY000"))
					restart = true;
				else
					cout << "SQL Query Get Row Count error : " << SqlCharToString(errorMessage, 512) << endl;
			}
			SQLCloseCursor(hStmt);
			SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
		} while (restart);
		VectorExtension::FreeAll(query);

		return resultCount;
}

	string SqlHelper::SqlCharToString(SQLTCHAR value[], size_t size)
	{
		string result;
		if (value != NULL)
		{
			unsigned int i = 0;
			do
			{
				result += (char)value[i];
				i++;
			} while (value[i] != '\0' && i < size);
		}
		return result;
	}

#pragma endregion

}