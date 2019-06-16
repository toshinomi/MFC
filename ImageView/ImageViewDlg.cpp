
// ImageViewDlg.cpp : 実装ファイル
//

#include "pch.h"
#include "framework.h"
#include "ImageView.h"
#include "ImageViewDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// アプリケーションのバージョン情報に使われる CAboutDlg ダイアログ

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

// 実装
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CImageViewDlg ダイアログ



CImageViewDlg::CImageViewDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_IMAGEVIEW_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CImageViewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PICTURE, m_picture);
	DDX_Control(pDX, ID_FILE_OPEN, m_fileOpen);
}

BEGIN_MESSAGE_MAP(CImageViewDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(ID_FILE_OPEN, &CImageViewDlg::OnBnClickedFileOpen)
END_MESSAGE_MAP()


// CImageViewDlg メッセージ ハンドラー

BOOL CImageViewDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// "バージョン情報..." メニューをシステム メニューに追加します。

	// IDM_ABOUTBOX は、システム コマンドの範囲内になければなりません。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// このダイアログのアイコンを設定します。アプリケーションのメイン ウィンドウがダイアログでない場合、
	//  Framework は、この設定を自動的に行います。
	SetIcon(m_hIcon, TRUE);			// 大きいアイコンの設定
	SetIcon(m_hIcon, FALSE);		// 小さいアイコンの設定

	// TODO: 初期化をここに追加します。

	return TRUE;  // フォーカスをコントロールに設定した場合を除き、TRUE を返します。
}

void CImageViewDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// ダイアログに最小化ボタンを追加する場合、アイコンを描画するための
//  下のコードが必要です。ドキュメント/ビュー モデルを使う MFC アプリケーションの場合、
//  これは、Framework によって自動的に設定されます。

void CImageViewDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 描画のデバイス コンテキスト

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// クライアントの四角形領域内の中央
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// アイコンの描画
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
		ResizeGraphView(m_picture, nWidth, nHeight);
		//OnBnClickedFileOpen();
	}
}

// ユーザーが最小化したウィンドウをドラッグしているときに表示するカーソルを取得するために、
//  システムがこの関数を呼び出します。
HCURSOR CImageViewDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CImageViewDlg::OnBnClickedFileOpen()
{
	CString		strFilter("JPEG Files (*.jpg;*.jpeg)|*.jpg; *.jpeg||");
	CFileDialog fileDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, strFilter);

	if (fileDlg.DoModal() == IDOK)
	{
		CDC* pDC = m_picture.GetDC();
		strFileName = fileDlg.GetPathName();

		CImage* image = new CImage();
		image->Load(strFileName);

		nWidth = image->GetWidth();
		nHeight = image->GetHeight();

		image->Draw(*pDC, 0, 0, nWidth, nHeight, 0, 0, nWidth, nHeight);

		UpdateData(FALSE);
	}

	return;
}


void CImageViewDlg::ResizeGraphView(CStatic& _picture, int _nWidth, int _nHeight)
{
	CRect rect;

	_picture.GetClientRect(&rect);

	int nWInc = _nWidth - rect.Width();
	int nHInc = _nHeight - rect.Height();

	CRect rectThis;
	GetWindowRect(&rectThis);
	if (nWInc > 0)
	{
		rectThis.right += nWInc;
	}
	if (nHInc > 0)
	{
		rectThis.bottom += nHInc;
	}

	CRect rectViewWnd;
	_picture.GetWindowRect(&rectViewWnd);
	ScreenToClient(&rectViewWnd);
	rectViewWnd.right += nWInc;
	rectViewWnd.bottom += nHInc;

	MoveWindow(&rectThis);
	_picture.MoveWindow(&rectViewWnd);

	ReDraw();

	return;
}

void CImageViewDlg::ReDraw()
{
	CDC* pDC = m_picture.GetDC();

	CImage* image = new CImage();
	image->Load(strFileName);

	nWidth = image->GetWidth();
	nHeight = image->GetHeight();

	image->Draw(*pDC, 0, 0, nWidth, nHeight, 0, 0, nWidth, nHeight);

	UpdateData(FALSE);

	return;
}