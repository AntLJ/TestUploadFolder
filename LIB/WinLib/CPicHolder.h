/*
 * Copyright (C) INCREMENT P CORP. All Rights Reserved.
 *
 * THIS SOFTWARE IS PROVIDED BY INCREMENT P CORP., WITHOUT WARRANTY OF
 * ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 * WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
 * AND NONINFRINGEMENT.
 *
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDER BE LIABLE FOR ANY
 * CLAIM, DAMAGES SUFFERED BY LICENSEE AS A RESULT OF USING, MODIFYING
 * OR DISTRIBUTING THIS SOFTWARE OR ITS DERIVATIVES.
*/

/////////////////////////////////////////////////////////////////////////////
// CPicHolder - ピクチャオブジェクトを扱うためのヘルプクラス
//

#ifndef _PICHOLDERDECLARED
#define _PICHOLDERDECLARED

class CPicHolder
{
// コンストラクター
public:
    CPicHolder() { m_pPict = NULL; }
    CPicHolder(LPPICTUREDISP pPict) { m_pPict = NULL; SetPictureDispatch(pPict); }
    ~CPicHolder() { Clear(); }

// オペレーション
	void Clear()
	{
		if (m_pPict) m_pPict->Release();
		m_pPict = NULL;
	}

	HINSTANCE GetInstanceHandle(HINSTANCE hInst = NULL)
	{
		if (hInst) return hInst;
#ifdef _MFC_VER
		return AfxGetResourceHandle();
#endif
#ifdef _ATL_VER
#if _ATL_VER <= 1200
		return _Module.GetResourceInstance();
#else
		return ATL::_AtlBaseModule.GetResourceInstance();
#endif // if _ATL_VER <= 1200
#endif // ifdef _ATL_VER

		return NULL;
	}

	BOOL HasPicture()
	{
		return m_pPict != NULL;
	}

	BOOL CreateFromMetafile(HMETAFILE hmf, int xExt, int yExt, BOOL bTransferOwnership = false)
    {
	    Clear();
	    PICTDESC pdesc;
	    pdesc.cbSizeofstruct = sizeof(pdesc);
	    pdesc.picType = PICTYPE_METAFILE;
	    pdesc.wmf.hmeta = hmf;
	    pdesc.wmf.xExt = xExt;
	    pdesc.wmf.yExt = yExt;
	    return SUCCEEDED(OleCreatePictureIndirect(&pdesc, IID_IPicture, 
                         bTransferOwnership, (LPVOID*)&m_pPict));
    }

	BOOL CreateFromBitmap(HBITMAP hbm, HPALETTE hpal = NULL, BOOL bTransferOwnership = false)
	{
		Clear();
		PICTDESC pdesc;
		pdesc.cbSizeofstruct = sizeof(pdesc);
		pdesc.picType = PICTYPE_BITMAP;
		pdesc.bmp.hbitmap = hbm;
		pdesc.bmp.hpal = hpal;
		return SUCCEEDED(OleCreatePictureIndirect(&pdesc, IID_IPicture,
						 bTransferOwnership, (LPVOID*)&m_pPict));
	}

	BOOL CreateFromIcon(HICON hicon, BOOL bTransferOwnership = false)
	{
		Clear();
		PICTDESC pdesc;
		pdesc.cbSizeofstruct = sizeof(pdesc);
		pdesc.picType = PICTYPE_ICON;
		pdesc.icon.hicon = hicon;
		return SUCCEEDED(OleCreatePictureIndirect(&pdesc, IID_IPicture,
						 bTransferOwnership, (LPVOID*)&m_pPict));
	}

	BOOL CreateFromIcon(UINT nID, HINSTANCE hInst = NULL)
	{
		// 任意のアイコンのサイズを取得するために、LoadIconの代わりにLoadImageを使用します。
		HICON hIcon = (HICON)LoadImage(GetInstanceHandle(hInst), MAKEINTRESOURCE(nID), IMAGE_ICON, 0, 0, 0);
		if (!hIcon) return false;
		return CreateFromIcon(hIcon, true);
	}

	BOOL CreateFromBitmap(UINT nID, HINSTANCE hInst = NULL)
	{
		HBITMAP hBmp = LoadBitmap(GetInstanceHandle(hInst), MAKEINTRESOURCE(nID));
		if (!hBmp) return false;
		return CreateFromBitmap(hBmp, NULL, true);
	}

	BOOL CreateFromClipboard()
	{
		Clear();

		HANDLE h;
		BOOL bRet = false;
		OpenClipboard(NULL);

		// メタファイルの取得を行います。
		h = GetClipboardData(CF_METAFILEPICT);
		if (h) {
			LPMETAFILEPICT pm = (LPMETAFILEPICT)GlobalLock(h);
			bRet = CreateFromMetafile(pm->hMF, pm->xExt, pm->yExt);
			GlobalUnlock(h);
		}

		// ビットマップの取得を行います。
		if (!bRet) {
			h = GetClipboardData(CF_BITMAP);
			if (h) bRet = CreateFromBitmap((HBITMAP)h);
		}

		// その他のファイルフォーマットを取得する場合、ここでその処理を行います。

		CloseClipboard();
		return bRet;
	}

    BOOL CreateFromFile(LPCTSTR szFileName)
    {
		Clear();

	    // メモリにファイルをロードします。
        HGLOBAL hGlobal = FileToGlobalHandle(szFileName);
	    if (!hGlobal) return false;

	    // メモリハンドル上にストリームを開きます。
	    IStream*      pStream   = NULL;
	    IPictureDisp* pPictDisp = NULL;
	    if (S_OK == CreateStreamOnHGlobal(hGlobal, false, &pStream))
	    {
		    // ストリームからピクチャをロードします。
		    OleLoadPicture(pStream, 0, false, IID_IPicture, (void**)&m_pPict);
		    pStream->Release();
	    }

	    GlobalFree(hGlobal);

	    return (m_pPict != NULL);
    }

	short GetType()
    {
	    short sPicType = (short)PICTYPE_UNINITIALIZED;
	    if (m_pPict != NULL) m_pPict->get_Type(&sPicType);
	    return sPicType;
    }
        
	LPPICTUREDISP GetPictureDispatch(BOOL bAddRef = true)
    {
	    LPPICTUREDISP pPictDisp = NULL;
	    if (m_pPict) m_pPict->QueryInterface(IID_IPictureDisp, (LPVOID*)&pPictDisp);
		if (pPictDisp && !bAddRef) pPictDisp->Release();
	    return pPictDisp;
    }
        
	void SetPictureDispatch(LPPICTUREDISP pDisp)
    {
	    LPPICTURE pPict = NULL;
	    Clear();
	    if (pDisp && SUCCEEDED(pDisp->QueryInterface(IID_IPicture, (LPVOID*)&pPict)))
		    m_pPict = pPict;
    }

    void Render(HDC hDC, LPRECT rcRender, LPRECT rcBounds)
    {
	    if (!m_pPict) return;
        long hmWidth;
		long hmHeight;
		m_pPict->get_Width(&hmWidth);
		m_pPict->get_Height(&hmHeight);
		m_pPict->Render(hDC, rcRender->left, rcRender->top,
			rcRender->right - rcRender->left, rcRender->bottom - rcRender->top,
            0, hmHeight-1, hmWidth, -hmHeight, rcBounds);
	}

// アトリビュート
	LPPICTURE m_pPict;

private:

    HGLOBAL FileToGlobalHandle(LPCTSTR szFileName)
    {
		USES_CONVERSION;

	    HFILE f = _lopen(T2CA(szFileName), OF_READ);
        
		// 失敗の場合、アプリケーションのディレクトリで見ます。
		if (HFILE_ERROR == f) {

			// 新しいファイル名を作成します。
			TCHAR szBuff[512];
			GetModuleFileName(GetInstanceHandle(), szBuff, 512);
			LPTSTR sz = _tcsrchr(szBuff, '\\');
			if (sz) sz[1] = 0;
			_tcscat(szBuff, szFileName);

			// 再度処理を行います。
			f = _lopen(T2CA(szBuff), OF_READ);
		
			if (HFILE_ERROR == f) {

				// 新しいファイル名を作成します。
				sz = _tcsrchr(szBuff, '\\');
				if (sz) *sz = 0;
				sz = _tcsrchr(szBuff, '\\');
				if (sz) sz[1] = 0;
				_tcscat(szBuff, szFileName);

				// 再度処理を行います。
				f = _lopen(T2CA(szBuff), OF_READ);
			}
		}

		if (HFILE_ERROR == f) return NULL;

	    // バッファを割り当てます。
	    long len = _llseek(f, 0, FILE_END);
	    HGLOBAL hGlobal = GlobalAlloc(GHND, len);

	    // バッファにファイルを読み込みます。
	    if (hGlobal) {
		    LPTSTR sz = (LPTSTR)GlobalLock(hGlobal);
    	    _llseek(f, 0, FILE_BEGIN);
		    _lread(f, sz, len);
		    GlobalUnlock(hGlobal);
	    }

	    // ファイルをクローズします。
        _lclose(f);

	    // ハンドルを返します。
	    return hGlobal;
    }
};

#endif // _PICHOLDERDECLARED
