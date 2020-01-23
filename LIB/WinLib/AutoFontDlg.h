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

#pragma once

#include <boost/shared_ptr.hpp>

namespace autodlg
{
	struct Observer
	{
		// �������z�֐�
		virtual void LoadColSort(IVSFlexGridPtr ipFlexGrid, long mode=0) = 0;
		virtual void SaveColSort(IVSFlexGridPtr ipFlexGrid, long lCol, SortSettings sortype = flexSortNone, long mode=0) = 0;
		virtual void LoadGridColWidth(IVSFlexGridPtr ipFlexGrid, long mode=0) = 0;
		virtual void SaveGridColWidth(IVSFlexGridPtr ipFlexGrid, long lCol, long width, long mode=0) = 0;
		virtual bool HasGridColWidth(IVSFlexGridPtr ipFlexGrid, long lCol, long mode=0) = 0;
		virtual void ResetGridColWidth(IVSFlexGridPtr ipFlexGrid, long lCol, long mode=0) = 0;
		virtual void LoadGridRowHeight(IVSFlexGridPtr ipFlexGrid, long mode=0) = 0;
		virtual void SaveGridRowHeight(IVSFlexGridPtr ipFlexGrid, long lRow, long height, long mode=0) = 0;
		virtual bool HasGridRowHeight(IVSFlexGridPtr ipFlexGrid, long lRow, long mode=0) = 0;
		virtual void ResetGridRowHeight(IVSFlexGridPtr ipFlexGrid, long lRow, long mode=0) = 0;
		virtual bool IsSaveGridSize(IVSFlexGridPtr ipFlexGrid, long nColRowMode, long id, long mode=0) = 0;
		virtual void LoadGridSelect(IVSFlexGridPtr ipFlexGrid, long mode=0) = 0;
		virtual void SaveGridSelect(IVSFlexGridPtr ipFlexGrid, long mode=0) = 0;
		virtual void ClearGridSelect(IVSFlexGridPtr ipFlexGrid=NULL, long mode=0) = 0;
		virtual void LoadGridPos(IVSFlexGridPtr ipFlexGrid, long mode=0) = 0;
		virtual void SaveGridPos(IVSFlexGridPtr ipFlexGrid, long mode=0) = 0;
		virtual void ClearGridPos(IVSFlexGridPtr ipFlexGrid=NULL, long mode=0) = 0;
	};

	enum FontSetMode
	{
		font_small = 0, //!< �t�H���g�T�C�Y�k��
		font_big   = 1, //!< �t�H���g�T�C�Y�g��
		font_init  = 2  //!< �t�H���g�T�C�Y����ݒ�
	};
};

typedef std::list< HFONT > HFONTS; //!< �t�H���g�̃��X�g
#define AUTOFONTDLG_INIT_FONTSIZE 9 //!< �R���g���[���̃f�t�H���g�̃t�H���g�T�C�Y
#define AUTOFONTDLG_INIT_ATLAXWINLICFONTSIZE 8.25 //!< FlexGrid�p�̃f�t�H���g�̃t�H���g�T�C�Y

class CControlsImpl : public std::list<HWND>
{
public:
	CControlsImpl() : m_idd(0), m_size(0.0f), m_size_min(8.0f), m_size_max(72.0f){};
	CControlsImpl(LPCTSTR lpcszKey, long idd, LPCTSTR lpcszName, double size) : m_strKey(lpcszKey), m_idd(idd), m_strCtlName(lpcszName), m_size(size), m_size_min(8.0f), m_size_max(72.0f) {};
	virtual ~CControlsImpl(){};

	LPCTSTR GetControlName() const { return m_strCtlName; };
	double GetSize() const { return m_size; };

	virtual void ChgFontSize(autodlg::FontSetMode nMode, std::map<HWND, HFONTS >& mapExFonts) = 0;

	void GetRegData()
	{
		// ���W�X�g���ǂݍ���
		CString strReg;

		strReg.Format( _T("%s\\AutoFont\\%d\\%s"), m_strKey, m_idd, m_strCtlName );

		CRegKey reg;
		if( ERROR_SUCCESS == reg.Open( HKEY_CURRENT_USER, strReg ) ) {
			m_size = GetRegSize( reg, _T("FontSize") );
			m_size_min = GetRegSize( reg, _T("FontMinSize") );
			m_size_max = GetRegSize( reg, _T("FontMaxSize"), 72.0f );
		}
	}

	void SetRegData()
	{
		if ( ! m_strKey.IsEmpty() ) {
			// ���W�X�g���ɕۑ�
			CString strReg;

			strReg.Format( _T("%s\\AutoFont\\%d\\%s"), m_strKey, m_idd, m_strCtlName );

			CRegKey reg;
			if( ERROR_SUCCESS == reg.Create( HKEY_CURRENT_USER, strReg ) )
				SetRegSize( reg, _T("FontSize"), m_size );
		}
	}

	bool ChgSize(autodlg::FontSetMode nMode)
	{
		double size = m_size;

		switch( nMode ) 
		{
		case autodlg::FontSetMode::font_small: size -= 1.0f; break;
		case autodlg::FontSetMode::font_big  : size += 1.0f; break;
		default:break;
		}

		if ( size >= m_size_min && size <= m_size_max ) {
			m_size = size;
			return true;
		}

		return false;
	}

protected:
	double GetRegSize(CRegKey& reg, LPCTSTR lpcszName, double defsize=8.0f)
	{
		CString strSize;
		ULONG BufSize = 0;
		double size = defsize;

		reg.QueryStringValue( lpcszName, NULL, &BufSize );
		LPTSTR lpszSize = strSize.GetBuffer( BufSize + 1 );
		if( ERROR_SUCCESS == reg.QueryStringValue( lpcszName, lpszSize, &BufSize ) )
			size = _tcstod( lpszSize, NULL );
		strSize.ReleaseBuffer();

		return size;
	}

	void SetRegSize(CRegKey& reg, LPCTSTR lpcszName, double size)
	{
		CString strSize;

		strSize.Format( _T("%.2lf"), size );
		reg.SetStringValue( lpcszName, strSize );
	}

protected:
	CString m_strKey;
	long m_idd;	// �_�C�A���OID
	double m_size;
	double m_size_min;
	double m_size_max;
	CString m_strCtlName;
};

class CNormalCtls : public CControlsImpl
{
public:
	CNormalCtls() {};
	CNormalCtls(LPCTSTR lpcszKey, long idd, LPCTSTR lpcszName, long size) : CControlsImpl(lpcszKey, idd, lpcszName, size){};
	~CNormalCtls()
	{
		for ( std::map<HWND, HFONTS>::iterator it = m_mapDefaultFonts.begin(); it != m_mapDefaultFonts.end(); ++it )
		{
			for( HFONTS::iterator font = it->second.begin(); font != it->second.end(); ++font )
				::DeleteObject( *font );
		}

		m_mapDefaultFonts.clear();
	};

	/**
	 * @brief  �R���g���[���ɕύX�����t�H���g��ݒ肷��֐�
	 * @note   ���̊֐����ł́A�t�H���g�̍폜�͍s���܂���
	 *         �����o�ō쐬���ꂽ�t�H���g���m�ۂ��Ă����A�_�C�A���O��������Ƃ���
	 *         �f�X�g���N�^�ňꊇ�폜����܂��B
	 *         ����Ȃ��Ȃ����t�H���g���폜����ƁA���낢��Ƃ��������Ȃ�
	 *         �ݒ��ɂ�����x�t�H���g�擾����ƃI�u�W�F�N�g�^�C�v���t�H���g����Ȃ��Ƃ�����B
	 *         �t�H���g�̍폜�͂��̃R���g���[�����̃t�H���g���ׂč폜�����悤�ȃC���[�W
	 *         �t�H���g�쐬������A�Ō�ɍ폜����̂�����@�̂悤���B
	 *         http://rarara.cafe.coocan.jp/cgi-bin/lng/vc/vclng.cgi?print+200701/07010038.txt
	 *         TODO:���̕��@���ƁA���\�ȗʂ̃t�H���g�I�u�W�F�N�g��������ނ��ƂɂȂ�̂ŁA
	 *              �t�H���g���쐬������폜����Ƃ������@�őΉ������ق����悢
	 *
	 * @param  nMode	  [in]  �t�H���g�T�C�Y�̐ݒ���@
	 * @param  mapExFonts [in]  �R���g���[�����L�[�Ƃ����t�H���g�̃��X�g
	 *
	 */
	void ChgFontSize(autodlg::FontSetMode nMode, std::map<HWND, HFONTS>& mapExFonts)
	{
		if ( ! empty() ) {
			GetRegData();

			if ( ChgSize(nMode) ) 
			{
				// �R���g���[���̃t�H���g�ύX
				for ( std::list<HWND>::const_iterator it = begin(); it != end(); ++it )
				{
					// ���݂̃t�H���g�擾�i��΂ɂ���j
					HFONT hCurrentFont = (HFONT)::SendMessage( (*it), WM_GETFONT, 0, 0 );
					// �Ȃ��Ȃ�A�ǂ����ō폜����Ă��܂��Ă���̂ł�������
					ATLASSERT( ::GetObjectType( hCurrentFont ) == OBJ_FONT);

					// CLogFont���쐬
					HDC hDC = ::GetDC((*it));
					CLogFont cChangedLogFont( hCurrentFont );

					HFONT hFont;
					// ����ݒ�t�H���g�������Ă���R���g���[���Ȃ�A������̐ݒ�̃t�H���g��ύX����
					std::map<HWND, HFONTS>::iterator itExFonts = mapExFonts.find( (*it) );
					if ( itExFonts != mapExFonts.end() ) 
					{
						// ����ݒ�t�H���g�̏ꍇ�ɂ�m_size���g���Ȃ��̂ŁA���ݒ肳��Ă���t�H���g
						// ����̊g��E�k�����s��
						long size = cChangedLogFont.GetHeight( hDC );
						// ���[�h�ɂ��t�H���g�T�C�Y�ύX
						switch( nMode ) 
						{
						case autodlg::FontSetMode::font_small: size -= 1; break;
						case autodlg::FontSetMode::font_big  : size += 1; break;
						default: size = (long)(m_size + ((double)size - m_size));break;
						}
						// �T�C�Y�Đݒ�
						cChangedLogFont.SetHeight( size, hDC );

						// ���łɂ����Ă���t�H���g���ǂ����m�F
						if( !HasFont( itExFonts->second, cChangedLogFont, hFont) )
						{
							// �Ȃ��Ȃ�t�H���g�쐬���āA�m�ۂ��Ă���
							HFONT hNewFont = cChangedLogFont.CreateFontIndirect();
							itExFonts->second.push_back( hNewFont );
							hFont = hNewFont;
						}
					} 
					else
					{
						cChangedLogFont.SetHeight( (long)m_size, hDC );

						HFONTS fonts = m_mapDefaultFonts[ *it ];
						// ���łɂ����Ă��邩�ǂ����m�F
						if( !HasFont( fonts, cChangedLogFont, hFont ) )
						{
							// �Ȃ��Ȃ�t�H���g�쐬���āA�m�ۂ��Ă���
							HFONT hNewFont = cChangedLogFont.CreateFontIndirect();
							m_mapDefaultFonts[(*it)].push_back( hNewFont );
							hFont = hNewFont;
						}
					}
					// �p�ς݃f�o�C�X�R���e�L�X�g�폜
					::ReleaseDC( (*it), hDC );

					// �R���g���[���փt�H���g�Đݒ�
					if ( hFont )
						::SendMessage( (*it), WM_SETFONT, (WPARAM)hFont, MAKELPARAM(TRUE,0) );
				}

				SetRegData();
			}
		}
	}

	/**
	 * @brief  ���łɕύX���悤�Ƃ��Ă���t�H���g�������Ă��邩�ǂ����𒲂ׂ�֐�
	 *
	 * @param  hFonts	     [in]   �`�F�b�N�������t�H���g�̃��X�g
	 * @param  cCheckLogFont [in]   �ύX���悤�Ƃ��Ă���t�H���g
	 * @param  hRetFont	     [out]  �����o�ŕ����Ă����t�H���g
	 * @return bool    true  ��1�����̃t�H���g�̃��X�g���ɑ�2�����̃t�H���g��������
	 * @return bool    false ��1�����̃t�H���g�̃��X�g���ɑ�2�����̃t�H���g���Ȃ�����
	 *
	 */
	bool HasFont( const HFONTS& hFonts, const CLogFont& cCheckLogFont, HFONT& hRetFont )
	{
		bool bHasFont = false;
		for( HFONTS::const_iterator font = hFonts.begin(); font != hFonts.end(); ++font )
		{
			CLogFont cCheckFont( *font );
			if( cCheckFont == cCheckLogFont )
			{
				hRetFont = *font;
				bHasFont = true;
			}
		}
		return bHasFont;
	}


protected:
	std::map<HWND, HFONTS> m_mapDefaultFonts;	//!< �W���t�H���g map<�R���g���[����,�t�H���g>
};

class CGridCtls : public CControlsImpl
{
public:
	CGridCtls() : m_pObserver(NULL) {};
	CGridCtls(LPCTSTR lpcszKey, long idd, LPCTSTR lpcszName, double size, autodlg::Observer* pObserver=NULL) : CControlsImpl(lpcszKey, idd, lpcszName, size), m_pObserver(pObserver) {};
	~CGridCtls(){};

	void ChgFontSize(autodlg::FontSetMode nMode, std::map<HWND, HFONTS >& mapExFonts)
	{
		if ( ! empty() ) {
			GetRegData();

			if ( ChgSize(nMode) ) {
				for ( std::list<HWND>::const_iterator it = begin(); it != end(); ++it )
				{
					IVSFlexGridPtr ipGrid = GetFlexGrid( (*it) );
					if ( ipGrid ) {
						// �t�H���g�T�C�Y
						ipGrid->put_FontSize( (float)m_size );

						ipGrid->put_Redraw( flexRDNone );

						long lCols = -1;

						ipGrid->get_Cols( &lCols );
						if ( lCols > 0 )
						{
							ipGrid->put_AutoSizeMode( flexAutoSizeColWidth );
							ipGrid->AutoSize( 0, lCols -1 );
							ipGrid->put_AutoSizeMode( flexAutoSizeRowHeight );
							ipGrid->AutoSize( 0, lCols -1 );
						}

						if ( m_pObserver )
						{
							m_pObserver->LoadGridColWidth( ipGrid );	// [Bug 7768][i][�v�]]�O���b�h�̗񕝕ێ��@�\
							m_pObserver->LoadGridRowHeight( ipGrid );	// [Bug 7768][i][�v�]]�O���b�h�̗񕝕ێ��@�\
						}

						ipGrid->put_Redraw( flexRDDirect );
					}
				}

				SetRegData();
			}
		}
	}

public:
	IVSFlexGridPtr GetFlexGrid(HWND hWnd)
	{
		IVSFlexGridPtr ipGrid;
		CComPtr<IUnknown> spUnk;

		HRESULT hr = AtlAxGetControl( hWnd, &spUnk );
		if ( SUCCEEDED(hr) && spUnk )
			hr = spUnk->QueryInterface( __uuidof(IVSFlexGrid), (void**)&ipGrid );

		return ipGrid;
	}

private:
	autodlg::Observer* m_pObserver;
};

template<class TDlg>
class CAutoFontDlgImpl : public CAxDialogImpl<TDlg>, public autodlg::Observer
{
public:
	CAutoFontDlgImpl() {};
	virtual ~CAutoFontDlgImpl(){ Clear(); };

BEGIN_MSG_MAP(CAutoFontDlgImpl)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	MESSAGE_HANDLER(WM_MOUSEWHEEL, OnWheel)
	MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
	CHAIN_MSG_MAP(CAxDialogImpl<TDlg>)
END_MSG_MAP()

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
	{
		Init();

		bHandled = FALSE;
		return 0;  // �V�X�e���Ńt�H�[�J�X��ݒ肵�܂��B
	}
	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		// �ʒu�E�T�C�Y�����W�X�g���ɕێ��ibug 6567�j
		OnPosition( uMsg, wParam, lParam, bHandled );
		Clear();

		bHandled = FALSE;
		return 0;
	}
	LRESULT OnWheel(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled)
	{
		Auto(wParam);

		bHandled = FALSE;
		return 0;
	}

	// [Bug 6567][i][�v�]]�o�^�_�C�A���O�̑O��\���ʒu�Ƒ傫����ێ��ł���悤�ɂ��Ăق����B
	LRESULT OnPosition(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& bHandled)
	{
		if ( ::IsWindow(m_hWnd) )
		{
			long width = LOWORD(lParam);
			long height = HIWORD(lParam);

			CRect rc;
			::GetWindowRect( m_hWnd, &rc );

			SavePosition( rc.left, rc.top, rc.Width(), rc.Height() );
		}

		bHandled = FALSE;
		return 0;
	}

	/**
	 * @brief ���̃R���g���[���Ƃ͈قȂ�t�H���g�T�C�Y��ݒ肷��֐�
	 *
	 * @note �p����̃_�C�A���O��OnInitDialog�œ��ʂɃt�H���g�T�C�Y��ς������R���g���[�����w�肵�܂��B
	 *       ����킵���̂ŁACreateExFont�Ƃ��ɖ��O���Ⴄ�ق�����������
	 *       ����ɁA�����Őݒ肷��t�H���g�T�C�Y�̓f�t�H���g�t�H���g�T�C�Y����̍��ł��邱�Ƃɒ���!
	 *       �Ⴆ�΁A�f�t�H���g�Ńt�H���g�T�C�Y��8�Ɛݒ肵�Ă��邪�A���ʂ�12�ɂ������ꍇ��
	 *       �p����̃_�C�A���O�ł́ACreateFont( hWnd, 12, ...)�ƃt�H���g�T�C�Y��ݒ肷��
	 *       �����A���̃N���X�ɂ���ă_�C�A���O�S�̂̃t�H���g�T�C�Y��15�Ɛݒ肳�ꂽ�ꍇ��
	 *       ����_�C�A���O�N�����i���W�X�g���ɓo�^����āA�����ǂݎ�邱�ƂɂȂ��Ă���j
	 *       �������������t�H���g�T�C�Y�ɂȂ��Ă��܂��i���ʐݒ肵���R���g���[����12�ŁA
	 *       �ق��̃R���g���[����15�ƂȂ�j�B���̂��߁A���̊֐��ŗ^����t�H���g�T�C�Y��
	 *       �f�t�H���g���炢���傫���A�܂��͏���������������^����ׂ������A���̃c�[��
	 *       ���C������̂��ʓ|�Ȃ̂ŁA���̊֐��ł́A�ݒ肳�ꂽ�t�H���g�T�C�Y���f�t�H���g
	 *       �T�C�Y�Ƃ����Ⴄ�����m�F���čs��
	 *
	 * @param	hWnd   [in] �t�H���g�T�C�Y��ς������E�B���h�E�n���h��
	 * @param	size   [in] �t�H���g�T�C�Y
	 * @param	Weight [in] �t�H���g�̑���
	 */
	void CreateFont(HWND hWnd, long size, long Weight)
	{
		// ���W�X�g���ɓo�^����Ă���R���g���[���̏��擾
		CNormalCtls cNormal( static_cast<TDlg*>(this)->GetRegKey(), GetIDD(), GetClassName( hWnd ), 8 );
		cNormal.GetRegData();

		// ���݂̃R���g���[���̃t�H���g�擾
		CLogFont cLogFont( (HFONT)::SendMessage( hWnd, WM_GETFONT, 0, 0 ) );

		// �f�t�H���g�̃R���g���[���T�C�Y���擾����
		CString strControlName = cNormal.GetControlName();
		double dInitFontSize = 0.0;
		if( strControlName.CompareNoCase( _T(ATLAXWINLIC_CLASS) ) == 0 )
			dInitFontSize = AUTOFONTDLG_INIT_ATLAXWINLICFONTSIZE;
		else
			dInitFontSize = AUTOFONTDLG_INIT_FONTSIZE;
		// ���̒l�����W�X�g���̒l���炢���傫���A�܂��͏��������������̒l�ƂȂ�
		double dDevFontSize = (double)size - dInitFontSize;

		HDC hDC = ::GetDC(hWnd);
		double dSetFontSize = 0.0;
		//���W�X�g���ɐݒ肳��Ă���l�擾���č��𑫂�
		if( cNormal.GetSize() > 0 )
			dSetFontSize = cNormal.GetSize() + dDevFontSize;
		else
			dSetFontSize = dInitFontSize;
		// �t�H���g�T�C�Y�ݒ�
		cLogFont.SetHeight( dSetFontSize, hDC );
		if ( Weight >= FW_BOLD )
			cLogFont.SetBold();
		// �p�ς݃f�o�C�X�R���e�L�X�g�폜
		::ReleaseDC( hWnd, hDC );

		// �t�H���g�쐬�������o�Ŋm�ۂ��Ă���
		HFONT hExFont = cLogFont.CreateFontIndirect();
		HFONTS fonts;
		fonts.push_back( hExFont );
		m_mapExFonts[hWnd] = fonts;
		// �R���g���[���փt�H���g�T�C�Y�ύX
		::SendMessage(hWnd, WM_SETFONT, (WPARAM)hExFont, MAKELPARAM(TRUE, 0));
	}

	/**
	 * @brief �쐬�����t�H���g���폜����֐�
	 *
	 * @note �r���Ńt�H���g�폜�����肷��Ƃ��������Ȃ�̂Ńt�H���g�̓_�C�A���O�I�����ɍ폜����
	 *       http://rarara.cafe.coocan.jp/cgi-bin/lng/vc/vclng.cgi?print+200701/07010038.txt
	 */
	void Clear()
	{
		for ( std::map<HWND, HFONTS>::iterator it = m_mapExFonts.begin(); it != m_mapExFonts.end(); ++it )
		{
			for( HFONTS::iterator font = it->second.begin(); font != it->second.end(); ++font )
				::DeleteObject( *font );
		}
		m_mapExFonts.clear();

		m_mapCtrls.clear();
	}

	void Init()
	{
		// ���O���X�g���W�X�g���ǂݍ���
		CString strReg;

		strReg.Format( _T("%s\\AutoFont\\%d"), static_cast<TDlg*>(this)->GetRegKey(), GetIDD() );

		CRegKey reg;
		if( ERROR_SUCCESS == reg.Open( HKEY_CURRENT_USER, strReg ) ) {
			CString strCtrlIDs;
			ULONG BufSize = 0;

			reg.QueryStringValue( _T("ExclutionCtrls"), NULL, &BufSize );
			if( ERROR_SUCCESS != reg.QueryStringValue( _T("ExclutionCtrls"), strCtrlIDs.GetBuffer( BufSize + 1 ), &BufSize ) )
			{
				strCtrlIDs.ReleaseBuffer();
				strCtrlIDs.Empty();
			} else
				strCtrlIDs.ReleaseBuffer();

			CString strToken( strCtrlIDs ), strID;
			int curPos=0;

			strID = strToken.Tokenize(_T(","), curPos);
			while ( strID != _T("") ) {
				m_listExclutions.push_back( _tcstol( strID, NULL, 10 ) );

				if ( curPos >= 0 )
					strID = strToken.Tokenize(_T(","), curPos);
				else
					break;
			}
		}

		m_listExclutions.sort();
		m_listExclutions.unique();

		InitControls();

		ChgFontSize( autodlg::FontSetMode::font_init );	// ���W�X�g��

		// [Bug 6567][i][�v�]]�o�^�_�C�A���O�̑O��\���ʒu�Ƒ傫����ێ��ł���悤�ɂ��Ăق����B
		strReg.Format( _T("%s\\DlgPosition\\%d"), static_cast<TDlg*>(this)->GetRegKey(), GetIDD() );

		if( ERROR_SUCCESS == reg.Open( HKEY_CURRENT_USER, strReg ) )
		{
			SetPosition(false);
		}
		else
		{
			// �f�t�H���g�T�C�Y���L��
			if( ERROR_SUCCESS == reg.Create( HKEY_CURRENT_USER, strReg ) )
			{
				CRect rc;

				::GetWindowRect( m_hWnd, &rc );

				CString strRect;
				strRect.Format( _T("%d,%d,%d,%d"), rc.left, rc.top, rc.Width(), rc.Height() );

				reg.SetStringValue( _T("Default_Position"), strRect );
			}
		}
	}

	/**
	 * @brief  �E�B���h�E�T�C�Y�ƃE�B���h�E�ʒu�ۑ�
	 *		   [Bug 6567][i][�v�]]�o�^�_�C�A���O�̑O��\���ʒu�Ƒ傫����ێ��ł���悤�ɂ��Ăق����B
	 * @param	x	[in]	X�ʒu
	 * @param	y	[in]	Y�ʒu
	 * @param	cx	[in]	��
	 * @param	cy	[in]	����
	 *
	 */
	void SavePosition(long x, long y, long cx, long cy)
	{
		CString strReg;

		strReg.Format( _T("%s\\DlgPosition\\%d"), static_cast<TDlg*>(this)->GetRegKey(), GetIDD() );

		CRegKey reg;
		if( ERROR_SUCCESS == reg.Create( HKEY_CURRENT_USER, strReg ) )
		{
			CString strRect;
			strRect.Format( _T("%d,%d,%d,%d"), x, y, cx, cy );

			reg.SetStringValue( _T("Position"), strRect );
		}
	}

	/**
	 * @brief  �E�B���h�E�T�C�Y�ƃE�B���h�E�ʒu�ݒ�
	 *		   [Bug 6567][i][�v�]]�o�^�_�C�A���O�̑O��\���ʒu�Ƒ傫����ێ��ł���悤�ɂ��Ăق����B
	 * @param	bDefSize	[in]	�f�t�H���g��K�p���邩�ǂ���
	 *
	 */
	void SetPosition(bool bDefSize)
	{
		// [Bug 6567][i][�v�]]�o�^�_�C�A���O�̑O��\���ʒu�Ƒ傫����ێ��ł���悤�ɂ��Ăق����B
		CString strReg;

		strReg.Format( _T("%s\\DlgPosition\\%d"), static_cast<TDlg*>(this)->GetRegKey(), GetIDD() );

		CRegKey reg;
		if( ERROR_SUCCESS == reg.Open( HKEY_CURRENT_USER, strReg ) )
		{
			CString strRect;
			ULONG BufSize = 0;

			CString strKey;

			if ( bDefSize )
				strKey += _T("Default_");

			strKey += _T("Position");

			reg.QueryStringValue( strKey, NULL, &BufSize );
			if( ERROR_SUCCESS != reg.QueryStringValue( strKey, strRect.GetBuffer( BufSize + 1 ), &BufSize ) )
			{
				strRect.ReleaseBuffer();
				strRect.Empty();
			}
			else
				strRect.ReleaseBuffer();

			if ( ! strRect.IsEmpty() )
			{
				CString strToken( strRect ), strSize;
				int curPos=0;
				long x=0,y=0,cx=0,cy=0;

				strSize = strToken.Tokenize(_T(","), curPos);
				x = _tcstol( strSize, NULL, 10 );

				strSize = strToken.Tokenize(_T(","), curPos);
				y = _tcstol( strSize, NULL, 10 );

				strSize = strToken.Tokenize(_T(","), curPos);
				cx = _tcstol( strSize, NULL, 10 );

				strSize = strToken.Tokenize(_T(","), curPos);
				cy = _tcstol( strSize, NULL, 10 );

				::SetWindowPos( m_hWnd, HWND_TOP, x, y, cx, cy, SWP_NOACTIVATE|SWP_NOZORDER );
			}
		}
	}

	void Auto(WPARAM wParam)
	{
		if ( GET_KEYSTATE_WPARAM(wParam) == MK_CONTROL ) {
			autodlg::FontSetMode nMode = autodlg::FontSetMode::font_big;	// �t�H���g�T�C�Y�g��

			if ( 0 < (short)GET_WHEEL_DELTA_WPARAM(wParam) )
				nMode =  autodlg::FontSetMode::font_small;	// �t�H���g�T�C�Y�k��

			ChgFontSize( nMode );
		}
	}

	void ChgFontSize(autodlg::FontSetMode nMode)
	{
		for ( std::map<long, boost::shared_ptr<CControlsImpl>>::iterator it = m_mapCtrls.begin(); it != m_mapCtrls.end(); ++it )
			it->second->ChgFontSize( nMode, m_mapExFonts );

		static_cast<TDlg*>(this)->FontResized();
	}

	void InitControls()
	{
		long idd = GetIDD();

		_InitControls( idd, WC_EDIT, AUTOFONTDLG_INIT_FONTSIZE );
		_InitControls( idd, WC_STATIC, AUTOFONTDLG_INIT_FONTSIZE );
		_InitControls( idd, WC_BUTTON, AUTOFONTDLG_INIT_FONTSIZE );
		_InitControls( idd, WC_COMBOBOX, AUTOFONTDLG_INIT_FONTSIZE );
		_InitControls( idd, WC_LISTBOX, AUTOFONTDLG_INIT_FONTSIZE );
		_InitControls( idd, WC_LISTVIEW, AUTOFONTDLG_INIT_FONTSIZE );
		_InitControls( idd, WC_TREEVIEW, AUTOFONTDLG_INIT_FONTSIZE );
		_InitControls( idd, WC_TABCONTROL, AUTOFONTDLG_INIT_FONTSIZE );
		_InitControls( idd, MONTHCAL_CLASS, AUTOFONTDLG_INIT_FONTSIZE );
		_InitControls( idd, _T(ATLAXWINLIC_CLASS), AUTOFONTDLG_INIT_ATLAXWINLICFONTSIZE );

		GetControls( m_hWnd );	// �S�R���g���[���擾
	}

	void FontResized(){};
	CString GetRegKey(){ return _T(""); };

	static CString GetClassName(HWND hWnd)
	{
		// �N���X���擾
		CString strClassName;

		::GetClassName( hWnd, strClassName.GetBuffer(513), 512 );
		strClassName.ReleaseBuffer();

		return strClassName;
	}

	// �\�[�g���ǂݍ���
	// [Bug 7767][i][�v�]]�₢���킹�_�C�A���O��̃\�[�g���ێ�
	void LoadColSort(IVSFlexGridPtr ipFlexGrid, long mode=0)
	{
		long id = GetGridID( ipFlexGrid );
		if ( id != -1 )
		{
			long r1=0, r2=0, c1=0, c2=0;
			ipFlexGrid->GetSelection(&r1, &c1, &r2, &c2);

			CString strReg;
			strReg.Format( _T("%s\\Sort\\%d\\%d\\%d"), static_cast<TDlg*>(this)->GetRegKey(), GetIDD(), id, mode );

			CRegKey reg;
			if( ERROR_SUCCESS == reg.Open( HKEY_CURRENT_USER, strReg ) )
			{
				long lCols = -1;

				ipFlexGrid->get_Cols( &lCols );
				for( int i = 0; i < lCols; ++i )
				{
					DWORD dwValue = 0;
					CString strCol;
					strCol.Format( _T("%d"), i );
					if ( ERROR_SUCCESS != reg.QueryDWORDValue( strCol, dwValue ) )
						dwValue = flexSortNone;
					else
						ipFlexGrid->Select( 0, i, 0, i );	// ���ꂪ�d�v�݂���

					ipFlexGrid->put_ColSort( i, (SortSettings)dwValue );
				}
			}

			ipFlexGrid->put_Sort( flexSortUseColSort );
			if( r1 >= 0 && c1 >= 0 )
				ipFlexGrid->Select( r1, c1 );
		}
	}

	// �\�[�g���ۑ�
	// [Bug 7767][i][�v�]]�₢���킹�_�C�A���O��̃\�[�g���ێ�
	void SaveColSort(IVSFlexGridPtr ipFlexGrid, long lCol, SortSettings sortype = flexSortNone, long mode=0)
	{
		long id = GetGridID( ipFlexGrid );
		if ( id != -1 && lCol >= 0 )
		{
			CString strReg;
			strReg.Format( _T("%s\\Sort\\%d\\%d\\%d"), static_cast<TDlg*>(this)->GetRegKey(), GetIDD(), id, mode );

			CRegKey reg;
			if( ERROR_SUCCESS == reg.Create( HKEY_CURRENT_USER, strReg ) )
			{
				// �w���ȊO�͍폜
				long lCols = -1;

				ipFlexGrid->get_Cols( &lCols );
				for( int i = 0; i < lCols; ++i )
				{
					if ( lCol != i )
					{
						CString strCol;
						strCol.Format( _T("%d"), i );
						DWORD dwValue=0;
						if ( ERROR_SUCCESS == reg.QueryDWORDValue( strCol, dwValue ) )
							reg.DeleteValue( strCol );
					}
				}

				CString strCol;
				strCol.Format( _T("%d"), lCol );
				reg.SetDWORDValue( strCol, sortype );
			}
		}
	}

	// �񕝓ǂݍ���
	// [Bug 7768][i][�v�]]�O���b�h�̗񕝕ێ��@�\
	void LoadGridColWidth(IVSFlexGridPtr ipFlexGrid, long mode=0)
	{
		long id = GetGridID( ipFlexGrid );
		if ( id != -1 && IsSaveGridSize(ipFlexGrid,1,id,mode) )
		{
			CString strReg;
			strReg.Format( _T("%s\\ColWidth\\%d\\%d\\%d"), static_cast<TDlg*>(this)->GetRegKey(), GetIDD(), id, mode );

			CRegKey reg;
			if( ERROR_SUCCESS == reg.Open( HKEY_CURRENT_USER, strReg ) )
			{
				long lCols = -1;
				bool bChg = false;

				ipFlexGrid->get_Cols( &lCols );
				for( int i = 0; i < lCols; ++i )
				{
					DWORD dwValue = 0;
					CString strCol;
					strCol.Format( _T("%d"), i );
					if ( ERROR_SUCCESS == reg.QueryDWORDValue( strCol, dwValue ) )
					{
						ipFlexGrid->Select( 0, i );
						ipFlexGrid->CellBorder( RGB(128, 128, 255 ), 0, 100, 0, 0, 0, 0 );
						ipFlexGrid->put_ColWidth( i, dwValue );
					}
				}
			}
		}
	}

	// �񕝕ۑ�
	// [Bug 7768][i][�v�]]�O���b�h�̗񕝕ێ��@�\
	void SaveGridColWidth(IVSFlexGridPtr ipFlexGrid, long lCol, long width, long mode=0)
	{
		long id = GetGridID( ipFlexGrid );
		if ( id != -1 && lCol >= 0 && IsSaveGridSize(ipFlexGrid,1,id,mode) )
		{
			CString strReg;
			strReg.Format( _T("%s\\ColWidth\\%d\\%d\\%d"), static_cast<TDlg*>(this)->GetRegKey(), GetIDD(), id, mode );

			CRegKey reg;
			if( ERROR_SUCCESS == reg.Create( HKEY_CURRENT_USER, strReg ) )
			{
				CString strCol;
				strCol.Format( _T("%d"), lCol );
				reg.SetDWORDValue( strCol, width );

				ipFlexGrid->Select( 0, lCol );
				ipFlexGrid->CellBorder( RGB(128, 128, 255 ), 0, 100, 0, 0, 0, 0 );
			}
		}
	}

	// �񕝑��݁H
	bool HasGridColWidth(IVSFlexGridPtr ipFlexGrid, long lCol, long mode=0)
	{
		bool bRet = false;
		long id = GetGridID( ipFlexGrid );
		if ( id != -1 && lCol >= 0 && IsSaveGridSize(ipFlexGrid,1,id,mode) )
		{
			CString strReg;
			strReg.Format( _T("%s\\ColWidth\\%d\\%d\\%d"), static_cast<TDlg*>(this)->GetRegKey(), GetIDD(), id, mode );

			CRegKey reg;
			if( ERROR_SUCCESS == reg.Open( HKEY_CURRENT_USER, strReg ) )
			{
				CString strCol;
				strCol.Format( _T("%d"), lCol );

				DWORD dwValue = 0;
				if ( ERROR_SUCCESS == reg.QueryDWORDValue( strCol, dwValue ) )
					bRet = true;
			}
		}

		return bRet;
	}

	// �񕝃��Z�b�g
	// [Bug 7768][i][�v�]]�O���b�h�̗񕝕ێ��@�\
	void ResetGridColWidth(IVSFlexGridPtr ipFlexGrid, long lCol, long mode=0)
	{
		long id = GetGridID( ipFlexGrid );
		if ( id != -1 && lCol >= 0 && IsSaveGridSize(ipFlexGrid,1,id,mode) )
		{
			CString strReg;
			strReg.Format( _T("%s\\ColWidth\\%d\\%d\\%d"), static_cast<TDlg*>(this)->GetRegKey(), GetIDD(), id, mode );

			CRegKey reg;
			if( ERROR_SUCCESS == reg.Open( HKEY_CURRENT_USER, strReg ) )
			{
				CString strCol;
				strCol.Format( _T("%d"), lCol );
				reg.DeleteValue( strCol );

				ipFlexGrid->Select( 0, lCol );
				ipFlexGrid->CellBorder( RGB(172, 168, 153 ), 0, 0, 0, 0, 0, 0 );

				long lCols = -1;
				ipFlexGrid->get_Cols( &lCols );
				if ( lCols >= 1 )
				{
					ipFlexGrid->put_Redraw( flexRDNone );
					ipFlexGrid->put_AutoSizeMode( flexAutoSizeColWidth );
					ipFlexGrid->AutoSize( lCol );
					ipFlexGrid->put_Redraw( flexRDDirect );

					LoadGridColWidth( ipFlexGrid, mode );
				}
			}
		}
	}

	// �s���ǂݍ���
	void LoadGridRowHeight(IVSFlexGridPtr ipFlexGrid, long mode=0)
	{
		long id = GetGridID( ipFlexGrid );
		if ( id != -1 && IsSaveGridSize(ipFlexGrid,2,id,mode) )
		{
			CString strReg;
			strReg.Format( _T("%s\\RowHeight\\%d\\%d\\%d"), static_cast<TDlg*>(this)->GetRegKey(), GetIDD(), id, mode );

			CRegKey reg;
			if( ERROR_SUCCESS == reg.Open( HKEY_CURRENT_USER, strReg ) )
			{
				long lRows = -1;

				ipFlexGrid->get_Rows( &lRows );
				for( int i = 0; i < lRows; ++i )
				{
					DWORD dwValue = 0;
					CString strRow;
					strRow.Format( _T("%d"), i );
					if ( ERROR_SUCCESS == reg.QueryDWORDValue( strRow, dwValue ) )
					{
						ipFlexGrid->Select( i, 0 );
						ipFlexGrid->CellBorder( RGB(128, 128, 255 ), 100, 0, 0, 0, 0, 0 );
						ipFlexGrid->put_RowHeight( i, dwValue );
					}
				}
			}
		}
	}

	// �s���ۑ�
	void SaveGridRowHeight(IVSFlexGridPtr ipFlexGrid, long lRow, long height, long mode=0)
	{
		long id = GetGridID( ipFlexGrid );
		if ( id != -1 && lRow >= 0 && IsSaveGridSize(ipFlexGrid,2,id,mode) )
		{
			CString strReg;
			strReg.Format( _T("%s\\RowHeight\\%d\\%d\\%d"), static_cast<TDlg*>(this)->GetRegKey(), GetIDD(), id, mode );

			CRegKey reg;
			if( ERROR_SUCCESS == reg.Create( HKEY_CURRENT_USER, strReg ) )
			{
				CString strRow;
				strRow.Format( _T("%d"), lRow );
				reg.SetDWORDValue( strRow, height );

				ipFlexGrid->Select( lRow, 0 );
				ipFlexGrid->CellBorder( RGB(128, 128, 255 ), 100, 0, 0, 0, 0, 0 );
			}
		}
	}

	// �s�����݁H
	bool HasGridRowHeight(IVSFlexGridPtr ipFlexGrid, long lRow, long mode=0)
	{
		bool bRet = false;
		long id = GetGridID( ipFlexGrid );
		if ( id != -1 && lRow >= 0 && IsSaveGridSize(ipFlexGrid,2,id,mode) )
		{
			CString strReg;
			strReg.Format( _T("%s\\RowHeight\\%d\\%d\\%d"), static_cast<TDlg*>(this)->GetRegKey(), GetIDD(), id, mode );

			CRegKey reg;
			if( ERROR_SUCCESS == reg.Open( HKEY_CURRENT_USER, strReg ) )
			{
				CString strRow;
				strRow.Format( _T("%d"), lRow );

				DWORD dwValue = 0;
				if ( ERROR_SUCCESS == reg.QueryDWORDValue( strRow, dwValue ) )
					bRet = true;
			}
		}

		return bRet;
	}

	// �s�����Z�b�g
	void ResetGridRowHeight(IVSFlexGridPtr ipFlexGrid, long lRow, long mode=0)
	{
		long id = GetGridID( ipFlexGrid );
		if ( id != -1 && lRow >= 0 && IsSaveGridSize(ipFlexGrid,2,id,mode) )
		{
			CString strReg;
			strReg.Format( _T("%s\\RowHeight\\%d\\%d\\%d"), static_cast<TDlg*>(this)->GetRegKey(), GetIDD(), id, mode );

			CRegKey reg;
			if( ERROR_SUCCESS == reg.Open( HKEY_CURRENT_USER, strReg ) )
			{
				CString strRow;
				strRow.Format( _T("%d"), lRow );
				reg.DeleteValue( strRow );

				ipFlexGrid->Select( lRow, 0 );
				ipFlexGrid->CellBorder( RGB(172, 168, 153 ), 0, 0, 0, 0, 0, 0 );

				long lRows = -1;
				ipFlexGrid->get_Rows( &lRows );
				if ( lRows >= 1 )
				{
					long lCols = -1;
					ipFlexGrid->get_Cols( &lCols );
					ipFlexGrid->put_Redraw( flexRDNone );
					ipFlexGrid->put_AutoSizeMode( flexAutoSizeRowHeight );
					ipFlexGrid->AutoSize( 0 );
					ipFlexGrid->put_Redraw( flexRDDirect );

					LoadGridRowHeight( ipFlexGrid, mode );
				}
			}
		}
	}

	// �񕝍s���ۑ����[�h�ǂݍ���
	bool IsSaveGridSize(IVSFlexGridPtr ipFlexGrid, long nColRowMode, long id, long mode=0)
	{
		bool bRet = false;
		CString strReg;
		//strReg.Format( _T("%s\\%s\\%d\\%d\\%d"), static_cast<TDlg*>(this)->GetRegKey(), bRow ? _T("RowHeight") : _T("ColWidth"), GetIDD(), id, mode );
		strReg.Format( _T("%s"), static_cast<TDlg*>(this)->GetRegKey() );

		CRegKey reg;
		if( ERROR_SUCCESS == reg.Open( HKEY_CURRENT_USER, strReg ) )
		{
			DWORD dwValue = 0;
			if ( ERROR_SUCCESS == reg.QueryDWORDValue( _T("SaveGridHeightWidth"), dwValue ) )
				bRet = dwValue == 1 ? true : false;
		}

		return bRet;
	}

	// �Z���I����ԓǂݍ���
	void LoadGridSelect(IVSFlexGridPtr ipFlexGrid, long mode=0)
	{
		long id = GetGridID( ipFlexGrid );
		if ( id != -1 && m_mapGridSelects[mode].find( id ) != m_mapGridSelects[mode].end() )
		{
			long lRows = 0, lCols = 0;
			ipFlexGrid->get_Rows( &lRows );
			ipFlexGrid->get_Cols( &lCols );

			long lRow = m_mapGridSelects[mode][id].first;
			long lCol = m_mapGridSelects[mode][id].second;

			if ( lRow >= 0 && lRow <= lRows - 1 )
			{
				if ( lCol >= 0 )
					lCol = 0;
				if( lRow < 0 )
					lRow = 0;

				ipFlexGrid->Select( lRow, lCol <= lCols - 1 ? lCol : 0 );
				if ( ! ipFlexGrid->GetRowIsVisible( lRow ) || ipFlexGrid->GetRowPos( lRow ) == ipFlexGrid->GetClientHeight() )
					ipFlexGrid->put_TopRow( lRow );	// �I���Z����������ʒu�ɃX�N���[��
			}
			else
			{
				// ��ԉ�or��ԏォ��
				;
			}
		}
	}

	// �Z���I����ԕێ�
	void SaveGridSelect(IVSFlexGridPtr ipFlexGrid, long mode=0)
	{
		long id = GetGridID( ipFlexGrid );
		if ( id != -1 )
		{
			long r1=0, r2=0, c1=0, c2=0;
			ipFlexGrid->GetSelection(&r1, &c1, &r2, &c2);

			m_mapGridSelects[mode][id] = std::pair<long,long>(r1, c1);
		}
	}

	// �Z���I����ԃN���A
	void ClearGridSelect(IVSFlexGridPtr ipFlexGrid=NULL, long mode=0)
	{
		if ( ! ipFlexGrid )
			m_mapGridSelects.clear();
		else
		{
			long id = GetGridID( ipFlexGrid );
			if ( id != -1 && m_mapGridSelects[mode].find( id ) != m_mapGridSelects[mode].end() )
				m_mapGridSelects[mode].erase( m_mapGridSelects[mode].find( id ) );
		}
	}

	// �O���b�h�ʒu�ǂݍ���
	void LoadGridPos(IVSFlexGridPtr ipFlexGrid, long mode=0)
	{
		long id = GetGridID( ipFlexGrid );
		if ( id != -1 && m_mapGridPos[mode].find( id ) != m_mapGridPos[mode].end() )
		{
			long lRows = 0, lCols = 0;
			ipFlexGrid->get_Rows( &lRows );
			ipFlexGrid->get_Cols( &lCols );

			long lRow = m_mapGridPos[mode][id].first;
			long lCol = m_mapGridPos[mode][id].second;

			if ( lRow >= 0 && lRow <= lRows - 1 )
			{
				if ( lCol >= 0 )
					lCol = 0;
				if( lRow < 0 )
					lRow = 0;

				ipFlexGrid->Select( lRow, lCol <= lCols - 1 ? lCol : 0 );
				ipFlexGrid->put_TopRow( lRow );	// �I���Z����������ʒu�ɃX�N���[��
			}
		}
	}

	// �O���b�h�ʒu�ێ�
	void SaveGridPos(IVSFlexGridPtr ipFlexGrid, long mode=0)
	{
		long id = GetGridID( ipFlexGrid );
		if ( id != -1 )
		{
			long r1=0, c1=0;
			ipFlexGrid->get_TopRow( &r1 );
			// �Ƃ肠�����A�g�b�v�ʒu�����ł����񂶂�ˁH
			// ��ʒu�����Ă��ƂɂȂ�����Aget_LeftCol�g���Ă�
			m_mapGridPos[mode][id] = std::pair<long,long>(r1, c1);
		}
	}

	// �O���b�h�ʒu�N���A
	void ClearGridPos(IVSFlexGridPtr ipFlexGrid=NULL, long mode=0)
	{
		if ( ! ipFlexGrid )
			m_mapGridPos.clear();
		else
		{
			long id = GetGridID( ipFlexGrid );
			if ( id != -1 && m_mapGridPos[mode].find( id ) != m_mapGridPos[mode].end() )
				m_mapGridPos[mode].erase( m_mapGridPos[mode].find( id ) );
			// ������Ɩ�����肩������Ȃ����A�擪�Ƀ|�W�V�����ڂ��Ă���
			ipFlexGrid->put_TopRow(0);
		}
	}

private:
	/**
	 * @brief  �S�R���g���[���̃E�B���h�E�n���h���擾
	 */
	void GetControls(HWND hWnd)
	{
		// �S�R���g���[���擾
		for ( HWND hWndChild = ::GetTopWindow(hWnd); hWndChild != NULL; hWndChild = ::GetNextWindow(hWndChild, GW_HWNDNEXT) )
		{
			long nCtrlID = ::GetDlgCtrlID(hWndChild);

			// ���O�R���g���[�����`�F�b�N
			std::list<long>::const_iterator itCtrl = std::find( m_listExclutions.begin(), m_listExclutions.end(), nCtrlID );
			if ( itCtrl == m_listExclutions.end() ) {
				// �N���X���擾
				CString strClassName = GetClassName( hWndChild );

				// ���N���X���̃N���X�Ɋi�[
				std::map<long, boost::shared_ptr<CControlsImpl>>::iterator it = m_mapCtrls.find( GetClassType(strClassName) );
				if ( it != m_mapCtrls.end() )
				{
					long nStyle = ::GetWindowLong( hWndChild, GWL_STYLE );

					bool bAdd = false;
					switch ( it->first )
					{
						case 2: // Static
							if ( nStyle & SS_ICON || nStyle & SS_BITMAP )
								bAdd = false;
							else
								bAdd = true;
							break;
						case 4: // Button
							if ( ! (nStyle & BS_ICON) && ! (nStyle & BS_BITMAP) )
								bAdd = true;
							break;
						default: bAdd = true; break;
					}

					if ( bAdd )
						it->second->push_back( hWndChild );
				}

				if ( ::GetTopWindow(hWndChild) )
					GetControls( hWndChild );
			}
		}
	}

	void _InitControls(long idd, LPCTSTR lpcszName, double dbInitsize=8.0)
	{
		long nCtrID = GetClassType( lpcszName );

		if ( m_mapCtrls.find( nCtrID ) == m_mapCtrls.end() ) {
			switch ( nCtrID ) {
				case 1024:	// Grid
					m_mapCtrls[nCtrID] = (boost::shared_ptr<CControlsImpl>)new CGridCtls(static_cast<TDlg*>(this)->GetRegKey(), idd, lpcszName, dbInitsize, this);	// ActiveX�R���g���[��(���C�Z���X�t���H)
					break;
				default:
					m_mapCtrls[nCtrID] = (boost::shared_ptr<CControlsImpl>)new CNormalCtls(static_cast<TDlg*>(this)->GetRegKey(), idd, lpcszName, (long)dbInitsize);
					break;
			}
		}
	}

	static long GetClassType(LPCTSTR lpcszName)
	{
		if ( _tcsicmp( WC_EDIT, lpcszName ) == 0 )
			return 1;
		else if ( _tcsicmp( WC_STATIC, lpcszName ) == 0 )
			return 2;
		else if ( _tcsicmp( WC_BUTTON, lpcszName ) == 0 )
			return 4;
		else if ( _tcsicmp( WC_COMBOBOX, lpcszName ) == 0 )
			return 8;
		else if ( _tcsicmp( WC_LISTBOX, lpcszName ) == 0 )
			return 16;
		else if ( _tcsicmp( WC_LISTVIEW, lpcszName ) == 0 )
			return 32;
		else if ( _tcsicmp( WC_TREEVIEW, lpcszName ) == 0 )
			return 64;
		else if ( _tcsicmp( WC_TABCONTROL, lpcszName ) == 0 )
			return 128;
		else if ( _tcsicmp( MONTHCAL_CLASS, lpcszName ) == 0 )
			return 256;
		else if ( _tcsicmp( _T(ATLAXWINLIC_CLASS), lpcszName ) == 0 )
			return 1024;

		return -1;
	}

	// �O���b�h��ID�擾
	// [Bug 7768][i][�v�]]�O���b�h�̗񕝕ێ��@�\
	// [Bug 7767][i][�v�]]�₢���킹�_�C�A���O��̃\�[�g���ێ�
	long GetGridID(IVSFlexGridPtr ipFlexGrid)
	{
		long id = -1;

		if ( m_mapCtrls.find( 1024 ) != m_mapCtrls.end() )
		{
			CGridCtls* pGrid = (CGridCtls*)m_mapCtrls[1024].get();

			for ( std::list<HWND>::const_iterator it = pGrid->begin(); it != pGrid->end(); ++it )
			{
				IVSFlexGridPtr ipGrid = pGrid->GetFlexGrid( (*it) );

				if ( ipGrid == ipFlexGrid )
				{
					id = ::GetDlgCtrlID((*it));
					break;
				}
			}
		}

		return id;
	}

private:
	std::map<long, boost::shared_ptr<CControlsImpl>> m_mapCtrls;	//!< �擾�N���X���Q map<�R���g���[��ID,�t�H���g�ύX�N���X>
	std::list<long> m_listExclutions;	//!< ���O�R���g���[���Q list<�R���g���[��ID>
	std::map<HWND, HFONTS> m_mapExFonts;	//!< �g���t�H���g map<�E�B���h�E�n���h��,�t�H���g>
	std::map<long,std::map<long, std::pair<long,long>>> m_mapGridSelects;
	std::map<long,std::map<long, std::pair<long,long>>> m_mapGridPos;
};
