
// CrawlAndSearchAppDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CrawlAndSearchApp.h"
#include "CrawlAndSearchAppDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCrawlAndSearchAppDlg dialog

#pragma region Dialog

CCrawlAndSearchAppDlg::CCrawlAndSearchAppDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCrawlAndSearchAppDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCrawlAndSearchAppDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST2, DomainsList);
	DDX_Control(pDX, IDC_EDIT1, AddUrlEdit);
	DDX_Control(pDX, IDC_COMMAND2, StartSearchServiceButton);
	DDX_Control(pDX, IDC_COMMAND3, StopSearchServiceButton);
	DDX_Control(pDX, IDC_COMMAND4, StartCrawlServiceButton);
	DDX_Control(pDX, IDC_COMMAND5, StopCrawlServiceButton);
	DDX_Control(pDX, IDC_STATIC_DOMAIN, DomainCountLabel);
	DDX_Control(pDX, IDC_STATIC_PAGE, PageCountLabel);
	DDX_Control(pDX, IDC_STATIC_IMAGE, ImageCountLabel);
	DDX_Control(pDX, IDC_LIST1, LastestSearchesList);
	DDX_Control(pDX, IDC_STATIC_Search, SearchGroupBox);
	DDX_Control(pDX, IDC_STATIC_Crawl, CrawlGroupBox);
}

BEGIN_MESSAGE_MAP(CCrawlAndSearchAppDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_COMMAND2, &CCrawlAndSearchAppDlg::SearchEngine_StartService_OnBnClicked)
	ON_BN_CLICKED(IDC_COMMAND4, &CCrawlAndSearchAppDlg::Crawler_StartService_OnBnClicked)
	ON_BN_CLICKED(IDC_COMMAND3, &CCrawlAndSearchAppDlg::SearchEngine_StopService_OnBnClicked)
	ON_BN_CLICKED(IDC_COMMAND5, &CCrawlAndSearchAppDlg::Crawler_StopService_OnBnClicked)
	ON_BN_CLICKED(IDC_BUTTON1, &CCrawlAndSearchAppDlg::Crawler_AddUrl_OnBnClicked)
	ON_WM_TIMER()
END_MESSAGE_MAP()

BOOL CCrawlAndSearchAppDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	sql = new SQL::SqlHelper();

#if defined(_PRODUCTION)
	if (FileUtility::DirectoryExists("M:\\"))
		sql->Connect();
#else
	sql->Connect();
#endif

	crawlerService = new CrawlerService(sql);
	searchService = new SearcherService(sql);
	AddUrlEdit.SetWindowTextW(TEXT("http://www.domain.com"));
	SearchEngine_StartService_OnBnClicked();
	Crawler_StartService_OnBnClicked();
	UpdateInformation();
	SetTimer(1, 5000, 0);
	thread socketListenerThread(&CCrawlAndSearchAppDlg::SocketListener, this);
	socketListenerThread.detach();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CCrawlAndSearchAppDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint(); 
	}
}

HCURSOR CCrawlAndSearchAppDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

#pragma endregion

#pragma region Search Engine

void CCrawlAndSearchAppDlg::SearchEngine_StartService_OnBnClicked()
{
	StartSearchServiceButton.EnableWindow(FALSE);
	StopSearchServiceButton.EnableWindow(TRUE);

	searchService->StartSearchService();
}

void CCrawlAndSearchAppDlg::SearchEngine_StopService_OnBnClicked()
{
	StartSearchServiceButton.EnableWindow(TRUE);
	StopSearchServiceButton.EnableWindow(FALSE);

	searchService->StopSearchService();
}

#pragma endregion

#pragma region Crawler

void CCrawlAndSearchAppDlg::Crawler_StartService_OnBnClicked()
{
	StartCrawlServiceButton.EnableWindow(FALSE);
	StopCrawlServiceButton.EnableWindow(TRUE);

	crawlerService->StartCrawlService();
}

void CCrawlAndSearchAppDlg::Crawler_StopService_OnBnClicked()
{				
	StartCrawlServiceButton.EnableWindow(TRUE);
	StopCrawlServiceButton.EnableWindow(FALSE);

	crawlerService->StopCrawlService();
}

void CCrawlAndSearchAppDlg::Crawler_AddUrl_OnBnClicked()
{
	CString sWindowText;
	AddUrlEdit.GetWindowTextW(sWindowText);

	if (sWindowText.Trim(TEXT(" ")) != TEXT(""))
	{
		wstring wst = (wstring)sWindowText.GetString();
		string url = string(wst.begin(), wst.end());
		AddUrlToCrawl(url);
	}
	AddUrlEdit.SetWindowTextW(TEXT(""));
}

void CCrawlAndSearchAppDlg::AddUrlToCrawl(string url)
{
	int i = 0;
	bool found = false;

	url = crawlerService->GetDomainName(url);
	if (url == "" || url.empty())
		return;

	while (i < DomainsList.GetCount() - 1 && !found)
	{
		CString item;
		DomainsList.GetText(i, item);
		wstring wst = (wstring)item.GetString();
		string itemString = string(wst.begin(), wst.end());
		if (itemString == url)
			found = true;
		i++;
	}

	if (found)
		return;

	sql->InsertDomain(url);
	crawlerService->SubmitUrlToCrawl("http://" + url);
	DomainsList.InsertString(0, StringExtension::StringToWString(url).c_str());
}

#pragma endregion

#pragma region Statistics

void CCrawlAndSearchAppDlg::OnTimer(UINT_PTR nIDEvent)
{
	UpdateInformation();
	CDialogEx::OnTimer(nIDEvent);
}

void CCrawlAndSearchAppDlg::UpdateInformation()
{
	int domainCount = sql->GetDomainRowCount();
	int pageCount = sql->GetPageInternetRowCount();
	int photoCount = sql->GetPhotoRowCount();
	vector<string> domains = sql->SelectDomain();

	DomainCountLabel.SetWindowTextW(StringExtension::StringToWString(to_string(domainCount)).c_str());
	PageCountLabel.SetWindowTextW(StringExtension::StringToWString(to_string(pageCount)).c_str());
	ImageCountLabel.SetWindowTextW(StringExtension::StringToWString(to_string(photoCount)).c_str());

	DomainsList.ResetContent();
	for each (string str in domains)
		DomainsList.InsertString(0, (LPWSTR)StringExtension::StringToWString(str).c_str());
}

#pragma endregion

#pragma region Socket

void CCrawlAndSearchAppDlg::SocketListener()
{
	WSADATA WSAData;
	SOCKET sock;
	SOCKET csock;
	SOCKADDR_IN sockAddr;
	SOCKADDR_IN csin;
	WSAStartup(MAKEWORD(2, 0), &WSAData);
	sock = socket(AF_INET, SOCK_STREAM, 0);
	sockAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	sockAddr.sin_family = AF_INET;
	sockAddr.sin_port = htons(11000);
	::bind(sock, (SOCKADDR *)&sockAddr, sizeof(sockAddr));
	listen(sock, 0);

	while (true)
	{
		int sinsize = sizeof(csin);
		if ((csock = accept(sock, (SOCKADDR *)&csin, &sinsize)) != INVALID_SOCKET)
		{
			char queryChar[1024] = "";
			recv(csock, queryChar, sizeof(queryChar), 0);
			string result;
			string query = string(queryChar).substr(4);
			if (StringExtension::StartsWith(queryChar, "{T}|")) // RECHERCHE TEXTUELLE
			{
				result = searchService->DoSearch(query).c_str();
				LastestSearchesList.InsertString(0, StringExtension::StringToWString(query).c_str());
			}
			else if (StringExtension::StartsWith(queryChar, "{I}|")) // RECHERCHE IMAGE
			{
				vector<string> splittedQuery = StringExtension::Split(query, '|');
				result = searchService->DoImageSearch(splittedQuery[0], splittedQuery[1], splittedQuery[2]).c_str();
				LastestSearchesList.InsertString(0, StringExtension::StringToWString(query).c_str());
			}
			else if (StringExtension::StartsWith(queryChar, "{C}|")) // CRAWL
			{
				AddUrlToCrawl(query);
				result = "OK";
			}
			else if (StringExtension::StartsWith(queryChar, "{S}|")) // SHARE PAGE
			{
				sql->IncrementShareCount(query);
				result = "OK";
			}
			else if (StringExtension::StartsWith(queryChar, "{L}|")) // LIKE PAGE
			{
				sql->IncrementLikeCount(query);
				result = "OK";
			}
			else if (StringExtension::StartsWith(queryChar, "{V}|")) // VIEW PAGE
			{
				sql->IncrementViewCount(query);
				result = "OK";
			}
			else if (StringExtension::StartsWith(queryChar, "{W}|")) // SHARE Photo
			{
				sql->IncrementPhotoShareCount(query);
				result = "OK";
			}
			else if (StringExtension::StartsWith(queryChar, "{X}|")) // LIKE Photo
			{
				sql->IncrementPhotoLikeCount(query);
				result = "OK";
			}
			else if (StringExtension::StartsWith(queryChar, "{Y}|")) // VIEW Photo
			{
				sql->IncrementPhotoViewCount(query);
				result = "OK";
			}
			const char* resultChar = result.c_str();
			send(csock, resultChar, strlen(resultChar), 0);
		}
	}

	closesocket(sock);
	WSACleanup();
}

#pragma endregion
