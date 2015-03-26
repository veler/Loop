
// CrawlAndSearchAppDlg.h : header file
//

#pragma once

#include <thread>
#include <ppltasks.h>

#include "SearcherService.h"
#include "CrawlerService.h"
#include "SQlHelper.h"
#include "StringExtension.h"
#include "FileUtility.h"
#include "afxwin.h"

using namespace Crawler;
using namespace Searcher;
using namespace SQL;
using namespace concurrency;
using namespace std;

// CCrawlAndSearchAppDlg dialog
class CCrawlAndSearchAppDlg : public CDialogEx
{
// Construction
public:
	CCrawlAndSearchAppDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_CRAWLANDSEARCHAPP_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	SqlHelper* sql = NULL;
	CrawlerService* crawlerService = NULL;
	SearcherService* searchService = NULL;


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void SearchEngine_StartService_OnBnClicked();
	afx_msg void Crawler_StartService_OnBnClicked();
	afx_msg void SearchEngine_StopService_OnBnClicked();
	afx_msg void Crawler_StopService_OnBnClicked();
	afx_msg void Crawler_AddUrl_OnBnClicked();
	CListBox DomainsList;
	CListBox LastestSearchesList;
	CEdit AddUrlEdit;
	CButton StartSearchServiceButton;
	CButton StopSearchServiceButton;
	CButton StartCrawlServiceButton;
	CButton StopCrawlServiceButton;
	CStatic DomainCountLabel;
	CStatic PageCountLabel;
	CStatic ImageCountLabel;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	void UpdateInformation();
	void SocketListener();
	void AddUrlToCrawl(string url);
	CStatic SearchGroupBox;
	CStatic CrawlGroupBox;
};
