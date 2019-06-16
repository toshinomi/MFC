
// ImageView.h : PROJECT_NAME アプリケーションのメイン ヘッダー ファイルです
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'pch.h' をインクルードしてください"
#endif

#include "resource.h"		// メイン シンボル


// CImageViewApp:
// このクラスの実装については、ImageView.cpp を参照してください
//

class CImageViewApp : public CWinApp
{
public:
	CImageViewApp();

// オーバーライド
public:
	virtual BOOL InitInstance();

// 実装

	DECLARE_MESSAGE_MAP()
};

extern CImageViewApp theApp;
