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

// AttrCitySurveyPointDlg.cpp : DLL �A�v���P�[�V�����p�̃G���g�� �|�C���g���`���܂��B
//

#include "stdafx.h"
#include "AttrCitySurveyPointDlg.h"

/// �E�B���h�E�}�l�[�W���z�u�}�b�v�i�Ђ܂�胍�O�p�j
BEGIN_WINDOW_MAP(CITY_SURVEY_POINT_MAP)
	BEGINROWS(WRCT_REST,0,RCMARGINS(2,2))
		BEGINROWS(WRCT_REST,0,0)
			RCSPACE(2)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_MESH)
				RCTOFIT(IDC_EDIT_MESH)
				RCSPACE(10)
				RCTOFIT(IDC_STATIC_RESEARCHDAY)
				RCTOFIT(IDC_EDIT_RESEARCHDAY)
			ENDGROUP()
			RCSPACE(2)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_IDINMESH)
				RCTOFIT(IDC_EDIT_IDINMESH)
				RCSPACE(10)
				RCTOFIT(IDC_STATIC_TARGETDATA)
				RCTOFIT(IDC_EDIT_TARGETDATA)
			ENDGROUP()
			RCSPACE(2)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(0,0))
				RCREST(IDC_STATIC_VAR)
			ENDGROUP()
			RCSPACE(2)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_ATTRNAME)
				RCTOFIT(IDC_EDIT_ATTRNAME)
				RCSPACE(10)
				RCTOFIT(IDC_STATIC_LAYER)
				RCTOFIT(IDC_EDIT_LAYER)
			ENDGROUP()
			RCSPACE(2)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_PRENAME)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_PRENAME)
			ENDGROUP()
			RCSPACE(2)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(0,0))
				RCREST(IDC_STATIC_VAR2)
			ENDGROUP()
			RCSPACE(2)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_AFTNAME)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_AFTNAME)
			ENDGROUP()
			RCSPACE(2)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_SPACE)
				RCTOFIT(-1)
				RCREST(IDC_STATIC_COMMENT)
			ENDGROUP()
			RCSPACE(2)
			BEGINCOLS(WRCT_REST, 0, RCMARGINS(0,0))
				BEGINROWS(WRCT_TOFIT, 0, RCMARGINS(0,0))
					BEGINCOLS(WRCT_TOFIT, 0, 0)
						RCTOFIT(IDC_STATIC_POS)
						RCSPACE(10)
						RCTOFIT(IDC_CHECK_POS)
					ENDGROUP()
					RCSPACE(2)
					BEGINCOLS(WRCT_TOFIT, 0, 0)
						RCTOFIT(IDC_STATIC_NAMEOK)
						RCSPACE(10)
						RCTOFIT(IDC_CHECK_NAMEOK)
					ENDGROUP()
					RCSPACE(2)
					BEGINCOLS(WRCT_TOFIT, 0, 0)
						RCTOFIT(IDC_STATIC_CLOSE)
						RCSPACE(10)
						RCTOFIT(IDC_CHECK_CLOSE)
					ENDGROUP()
					RCSPACE(2)
					BEGINCOLS(WRCT_TOFIT, 0, 0)
						RCTOFIT(IDC_STATIC_CLEAR)
						RCSPACE(10)
						RCTOFIT(IDC_CHECK_CLEAR)
					ENDGROUP()
					RCSPACE(2)
					BEGINCOLS(WRCT_TOFIT, 0, 0)
						RCTOFIT(IDC_STATIC_CONSTRUCT)
						RCSPACE(10)
						RCTOFIT(IDC_CHECK_CONSTRUCT)
					ENDGROUP()
					RCSPACE(2)
					BEGINCOLS(WRCT_TOFIT, 0, 0)
						RCTOFIT(IDC_STATIC_UNCERTAIN)
						RCSPACE(10)
						RCTOFIT(IDC_CHECK_UNCERTAIN)
					ENDGROUP()
				ENDGROUP()
				BEGINCOLS(WRCT_REST, 0, 0)
					RCSPACE(10)
					BEGINROWS(WRCT_REST, 0, RCMARGINS(0,0))
						RCREST(IDC_STATIC_PIC)
						BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(0,0))
							RCREST(IDC_SLIDER_PIC)
							RCTOFIT(IDC_STATIC_PICNUM)
							RCTOFIT(IDC_BUTTON_DISPLAYPICTURE)
						ENDGROUP()
					ENDGROUP()
				ENDGROUP()
			ENDGROUP()
			RCSPACE(2)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(0,0))
				BEGINROWS(WRCT_TOFIT, 0, RCMARGINS(0,0))
					BEGINCOLS(WRCT_TOFIT, 0, 0)
						RCTOFIT(IDC_COMBO_CONFIRM)
						RCTOFIT(IDC_BUTTON_CONFIRM)
					ENDGROUP()
				ENDGROUP()
				RCSPACE(5)
				BEGINCOLS(WRCT_REST, 0, 0)
					RCREST(IDC_STATIC_MEMO)
				ENDGROUP()
			ENDGROUP()
		ENDGROUP()
	ENDGROUP()
END_WINDOW_MAP()

/**
 * �N���X�̖����I�����N���s�����߂ɁA���̃N���X��CExportDlg�N���X���p�����A�����_�C�A���O����CExportDlg���쐬���邱�Ƃɂ��
 * �����_�C�A���O������t���ɂ����邱�Ƃ��ł���N���X�{�̂��쐬����܂��B
 * �ڂ�����http://ptvi.hp.infoseek.co.jp/vc/p_0000010.html���Q�Ƃ̂���
 *
 * @retval CAttrCitySurveyPointDlg��Ԃ��܂��B
 */
//[Bug 5710]�̗v�]�ɂ��V���[�g�J�b�g�ŕ\���ł���悤�ɁACreateDlg����ύX
extern "C" __declspec(dllexport) CExportDlg * __stdcall CreateDlgByName( LPCTSTR lpcszTableName )
{
	CExportDlg* pDlg = NULL;
	pDlg = new CAttrCitySurveyPointDlg();
	if( pDlg )
		pDlg->SetFeatureClassName( lpcszTableName );

	return pDlg;
}

/////////////////////////////////////////////////////////////////////////////
//
// CAttrCitySurveyPointDlg �����o�֐�
//
/////////////////////////////////////////////////////////////////////////////

/** 
 * @fn CAttrCitySurveyPointDlg::CAttrCitySurveyPointDlg()
 * @brief �R���X�g���N�^
 *
 * �u���V�̏����������Ă����܂��B
 */
CAttrCitySurveyPointDlg::CAttrCitySurveyPointDlg() 
	: ATTR_BASE_CLASS(CITY_SURVEY_POINT_MAP, IDD_ATTRCITYSURVEYPOINTDLG),
		m_wnd0( _T("EDIT"), this, IDC_EDIT_MESH ),
		m_wnd1( _T("EDIT"), this, IDC_EDIT_RESEARCHDAY ),
		m_wnd2( _T("EDIT"), this, IDC_EDIT_IDINMESH ),
		m_wnd3( _T("EDIT"), this, IDC_EDIT_TARGETDATA ),
		m_wnd4( _T("EDIT"), this, IDC_EDIT_ATTRNAME ),
		m_wnd5( _T("EDIT"), this, IDC_EDIT_LAYER ),
		m_wnd6( _T("EDIT"), this, IDC_EDIT_PRENAME ),
		m_wnd7( _T("EDIT"), this, IDC_EDIT_AFTNAME ),
		m_wnd9( _T("CHECKBOX"), this, IDC_CHECK_POS ),
		m_wnd10( _T("CHECKBOX"), this, IDC_CHECK_NAMEOK ),
		m_wnd11( _T("CHECKBOX"), this, IDC_CHECK_CLOSE ),
		m_wnd12( _T("CHECKBOX"), this, IDC_CHECK_CLEAR ),
		m_wnd13( _T("CHECKBOX"), this, IDC_CHECK_CONSTRUCT ),
		m_wnd14( _T("CHECKBOX"), this, IDC_CHECK_UNCERTAIN ),
		m_wnd15( _T("BUTTON"), this, IDC_BUTTON_DISPLAYPICTURE ),
		m_wnd16( _T("COMBO"), this, IDC_COMBO_CONFIRM ),
		m_wnd17( _T("BUTTON"), this, IDC_BUTTON_CONFIRM )
{
	m_strBaseDir.Empty();
	m_strCode2Path.Empty();
	m_strToolPath.Empty();

	m_hFont = CreateFont(
		18, 0, 0, 0, 100,                           // �����C���C�w�p�x�C�x�p�x�C����
		FALSE, FALSE, FALSE,                        // �ΆC�����C�ŏ���
		SHIFTJIS_CHARSET,                           // �L�����N�^�ݒ�
		OUT_DEFAULT_PRECIS,                         // �o�͐��x
		CLIP_DEFAULT_PRECIS,                        // �N���b�v���x
		DEFAULT_QUALITY,                            // �i��
		FIXED_PITCH | FF_DONTCARE,                // �s�b�`
		AheLoadString(IDS_FONT) );                       // �t�H���g���
}

/** 
 * @fn HWND CAttrCitySurveyPointDlg::Create(HWND hWndParent, LPARAM dwInitParam)
 * @brief �q�_�C�A���O���쐬���܂��B
 *
 * ATTR_BASE_CLASS::Create(HWND hWndParent, LPARAM dwInitParam = NULL)�Ń_�C�A���O���쐬���܂��B
 *
 * @param hWndParent	[in]	�e�E�B���h�E�n���h��
 * @param dwInitParam	[in]	�_�C�A���O�������p�����[�^�i���ݓ��Ɏg�p���Ă��܂���j
 *
 * @retval �쐬���ꂽ�q�_�C�A���O�̃n���h����Ԃ��܂��B
 */
HWND CAttrCitySurveyPointDlg::Create(HWND hWndParent, LPARAM dwInitParam)
{ 
	m_hChildWnd = ATTR_BASE_CLASS::Create(hWndParent,dwInitParam);
	return m_hChildWnd;
}

/**
 * @fn void CAttrCitySurveyPointDlg::Delete()
 * @brief �쐬�����q�_�C�A���O���폜���܂��B
 *
 * �_�C�A���O���폜����Ƃ��ɕK���Ă�ŉ������B�Ă΂Ȃ��ƃ��������[�N���Ă��܂��܂��B
 */
void CAttrCitySurveyPointDlg::Delete()
{
	ClearFeatureDefs();
	::DestroyWindow(m_hChildWnd);
    delete this; // �K��new�Ŋm�ۂ���O��
}

/**
 * @fn BOOL CAttrCitySurveyPointDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
 * @brief ���ݕ\�������ׂ��t�B�[�`����FeatureDef�C���f�b�N�X��n���܂��B
 * ����ɁA�t�B�[�`���̑����Ƃ͊֌W�Ȃ��R���g���[���̐�������Ă����܂��B
 * �_�C�A���O���J�����Ƃ��ɍŏ��Ƀt�H�[�J�X�̓������Ă��ė~�����R���g���[���̐ݒ�����Ă����܂��B
 *
 * @param lFeatureIndex	[in]	�t�B�[�`����`�N���X�̃C���f�b�N�X�ԍ�
 * @param lTableIndex	[in]	�e�[�u����`�N���X�̃C���f�b�N�X�ԍ�
 * @param lRowIndex		[in]	���R�[�h��`�N���X�̃C���f�b�N�X�ԍ�
 * @param bForce		[in]	�����I�ɕҏW���邩�ǂ���
 * @param bEditable		[in]	�ҏW�\���ǂ���
 *
 * @retval �Ӗ����Ȃ��iTRUE�����Ԃ��Ȃ��j
 */
BOOL CAttrCitySurveyPointDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
{ 
    BOOL bRet = ATTR_BASE_CLASS::SetCurrentFeatureDefIndex( lFeatureIndex, lTableIndex, lRowIndex, 0, bForce, bEditable );

	// ���ڃt�B�[�`���Ɋ֌W�Ȃ��G�f�B�b�g�{�b�N�X(��F�Q�Ɨp�̘H�����V�[�P���X�⌴�e�ԍ�����)�Ȃǂ𐧌䂷��
	// �ҏW�J�n�O�ƕҏW�J�n��ł��ҏW�s�̂Ƃ���DISABLE�ɂ���
	if( IsEditableFeature() && m_cArcHelper.IsStartEditing() && bEditable )
	{
		SetDlgStatus(IDC_EDIT_MESH, FALSE, TRUE );
		SetDlgStatus(IDC_EDIT_RESEARCHDAY, FALSE, TRUE );
		SetDlgStatus(IDC_EDIT_IDINMESH, FALSE, TRUE );
		SetDlgStatus(IDC_EDIT_TARGETDATA, FALSE, TRUE );
		SetDlgStatus(IDC_EDIT_ATTRNAME, FALSE, TRUE );
		SetDlgStatus(IDC_EDIT_LAYER, FALSE, TRUE );
		SetDlgStatus(IDC_EDIT_PRENAME, FALSE, TRUE );
		SetDlgStatus(IDC_EDIT_AFTNAME, FALSE, TRUE );
		SetDlgStatus(IDC_CHECK_POS, FALSE );
		SetDlgStatus(IDC_CHECK_NAMEOK, FALSE );
		SetDlgStatus(IDC_CHECK_CLOSE, FALSE );
		SetDlgStatus(IDC_CHECK_CLEAR, FALSE );
		SetDlgStatus(IDC_CHECK_CONSTRUCT, FALSE );
		SetDlgStatus(IDC_CHECK_UNCERTAIN, FALSE );
		// �ҏW�J�n����Ă���Ƃ��ɂ��łɁASetFocus���Ă���
		::SetFocus( GetDlgItem(IDC_BUTTON_CONFIRM) );
	}
	FindPicture();

    return bRet;
}

/**
 * @fn void CAttrCitySurveyPointDlg::CreateControlRelation()
 * @brief �q�_�C�A���O�����ۂɍ쐬���܂��B
 *
 * ���ۂ̍쐬��ControlRel�ɔC���܂��B
 */
void CAttrCitySurveyPointDlg::CreateControlRelation()
{
	ATLASSERT( IsWindow() == TRUE );

	if( m_cControlRel.size() > 0 )
	{
		// �e�E�C���h�E�n���h����ݒ肵�Ȃ���
		m_cControlRel.ResetHWND( m_hWnd );
	} 
	else {
		m_cControlRel.SetHWND( m_hWnd );

		// ���b�V��
		m_cControlRel.SetControl( IDC_EDIT_MESH,		TYPE_EDIT,		0,	0,  _T("MESH"),			NULL, m_strFeatureClassName );
		// ������
		m_cControlRel.SetControl( IDC_EDIT_RESEARCHDAY,	TYPE_EDITSTRING,0,	0,  _T("RESERCH_DATE"),	NULL, m_strFeatureClassName );
		// ���b�V����ID
		m_cControlRel.SetControl( IDC_EDIT_IDINMESH,	TYPE_EDIT,		0,	0,  _T("ID_IN_MESH"),	NULL, m_strFeatureClassName );
		// �Ώۃf�[�^
		m_cControlRel.SetControl( IDC_EDIT_TARGETDATA,	TYPE_EDITSTRING,0,	0,  _T("TARGET_DATA"),	NULL, m_strFeatureClassName );
		// �����O���
		m_cControlRel.SetControl( IDC_EDIT_ATTRNAME,	TYPE_EDITSTRING,0,	0,  _T("ATTR_NAME"),	NULL, m_strFeatureClassName );
		// �f�[�^�̗p��
		m_cControlRel.SetControl( IDC_EDIT_LAYER,		TYPE_EDITSTRING,0,	0,  _T("LAYER"),		NULL, m_strFeatureClassName );
		// �����O����
		m_cControlRel.SetControl( IDC_EDIT_PRENAME,		TYPE_EDITSTRING,0,	0,  _T("PRE_NAME"),		NULL, m_strFeatureClassName );
		// �����㖼��
		m_cControlRel.SetControl( IDC_EDIT_AFTNAME,		TYPE_EDITSTRING,0,	0,  _T("AFT_NAME"),		NULL, m_strFeatureClassName );
		// ����
		m_cControlRel.SetControl( IDC_STATIC_MEMO,		TYPE_EDITSTRING,0,	0,  _T("MEMO"),			NULL, m_strFeatureClassName );
		// �ʒu�ړ�
		m_cControlRel.SetControl( IDC_CHECK_POS,		TYPE_CHECKBOX,	0,	0,  _T("POS_F"),		NULL, m_strFeatureClassName );
		// ����OK
		m_cControlRel.SetControl( IDC_CHECK_NAMEOK,		TYPE_CHECKBOX,	0,	0,  _T("NAME_F"),		NULL, m_strFeatureClassName );
		// ��
		m_cControlRel.SetControl( IDC_CHECK_CLOSE,		TYPE_CHECKBOX,	0,	0,  _T("CLOSE_F"),		NULL, m_strFeatureClassName );
		// �X�n
		m_cControlRel.SetControl( IDC_CHECK_CLEAR,		TYPE_CHECKBOX,	0,	0,  _T("CLEAR_F"),		NULL, m_strFeatureClassName );
		// ���ݒ�
		m_cControlRel.SetControl( IDC_CHECK_CONSTRUCT,	TYPE_CHECKBOX,	0,	0,  _T("CONST_F"),		NULL, m_strFeatureClassName );
		// ���̊m�F�s��
		m_cControlRel.SetControl( IDC_CHECK_UNCERTAIN,	TYPE_CHECKBOX,	0,	0,  _T("UNCERTAIN_F"),	NULL, m_strFeatureClassName );
		// �m�F�R�[�h
		m_cControlRel.SetControl( IDC_COMBO_CONFIRM,	TYPE_COMBO,	0,	0,  _T("CONFIRM_C"),	NULL, m_strFeatureClassName );
		// �m�F�{�^��
		m_cControlRel.SetControl( IDC_BUTTON_CONFIRM,	TYPE_BUTTON,	0,	0 );
	}
}

void CAttrCitySurveyPointDlg::GetRegCode()
{
 	CString strRegOpenKey( _T("SOFTWARE\\INCREMENT P CORPORATION\\SiNDY-e\\TOOLS\\ATTRIBUTETOOL" ) );

	HKEY hOpenKey = EmxCreateRegKey( strRegOpenKey, HKEY_CURRENT_USER );
	if ( hOpenKey ) {
		TCHAR strBaseDir[255], strCode2Path[255], strToolPath[255];

		if( EmxGetRegString( _T("SurveyDir"), strBaseDir, 255, hOpenKey ) == TRUE )
			m_strBaseDir = strBaseDir;

		if( EmxGetRegString( _T("SurveyCode2Path"), strCode2Path, 255, hOpenKey ) == TRUE )
			m_strCode2Path = strCode2Path;

		if( EmxGetRegString( _T("SurveyToolPath"), strToolPath, 255, hOpenKey ) == TRUE )
			m_strToolPath = strToolPath;

		RegCloseKey( hOpenKey );
	}
}

void CAttrCitySurveyPointDlg::FindPicture()
{
	m_vecPath.clear();
	m_lSlider = 0;

	CEdit cEditCtrl(GetDlgItem( IDC_EDIT_MESH ) );
	LPTSTR lpszStr = (LPTSTR)_alloca( ( cEditCtrl.GetWindowTextLength() + 1 ) * sizeof(TCHAR) );
	cEditCtrl.GetWindowText( lpszStr, cEditCtrl.GetWindowTextLength() + 1 );
	CString strMesh = lpszStr;

	CEdit cTargetEdit(GetDlgItem( IDC_EDIT_TARGETDATA ) );
	LPTSTR lpszStrTargetData = (LPTSTR)_alloca( ( cTargetEdit.GetWindowTextLength() + 1 ) * sizeof(TCHAR) );
	cTargetEdit.GetWindowText( lpszStrTargetData, cTargetEdit.GetWindowTextLength() + 1 );
	CString strTargetData = lpszStrTargetData;

	CEdit cIDInMeshEdit(GetDlgItem( IDC_EDIT_IDINMESH ) );
	LPTSTR lpszStrIDInMesh = (LPTSTR)_alloca( ( cIDInMeshEdit.GetWindowTextLength() + 1 ) * sizeof(TCHAR) );
	cIDInMeshEdit.GetWindowText( lpszStrIDInMesh, cIDInMeshEdit.GetWindowTextLength() + 1 );

	// ���n�����t�@�C����3���̐��l�Ńi���o�����O����Ă���
	long lIDInMesh = _ttol( lpszStrIDInMesh );
	CString strIDInMeshData;
	if( lIDInMesh < 10 )
		strIDInMeshData = _T("00") + CString(lpszStrIDInMesh);
	else if( lIDInMesh < 100 )
		strIDInMeshData = _T("0") + CString(lpszStrIDInMesh);
	else
		strIDInMeshData = lpszStrIDInMesh;

	/// Code2Path�����ƂɁA�ϊ�
	CString cStrPath;
	CString cStrFormatWork;
	CString cStrWork1 = m_strCode2Path;
	CString cStrWork2;
	int nIndex = 0;
	int nFind;
	do
	{
		/// \����؂�Ƃ��āA�����P�ʂ̎擾
		nFind = cStrWork1.Find( TEXT( '\\'));
		if( 0 <= nFind)
		{
			cStrWork2 = cStrWork1.Mid( nIndex, nFind + 1);
			cStrWork1 = cStrWork1.Mid( nFind + 1);
		}
		else
			cStrWork2 = cStrWork1.Mid( nFind + 1);

		/// �����P�ʂ�%���܂܂��ꍇ�AFormat(wsprintf)���āA����
		if( 0 <= cStrWork2.Find( TEXT( '%')))
		{
			cStrFormatWork.Format( cStrWork2, strMesh);
			cStrPath += cStrFormatWork;
		}
		else
			cStrPath += cStrWork2;
	}while( -1 != nFind);

	// �t�H���_���̃t�@�C����������
	HANDLE				hFile;
	WIN32_FIND_DATA		ffd;
	CString				strDirPath, strTmp;
	LONG				ItemCnt = 0;
	strDirPath = m_strBaseDir + strTargetData + _T("\\PHOTO\\") + cStrPath + _T("\\") + _T("*.JPG");

	hFile = FindFirstFile( strDirPath, &ffd );
	if ( hFile == INVALID_HANDLE_VALUE )
	{
		CString strErrorMessage;
		strErrorMessage = m_strBaseDir + strTargetData + _T("\\PHOTO\\") + cStrPath + AheLoadString( IDS_NOTEXISTPICTURE );
		AheMessageBox(strErrorMessage, _T("CitySurveyPoint.dll"), MB_OK );
		return ;	// �t�@�C�������݂��Ȃ��H
	}

	// �t�H���_�J������A���b�V����ID�̂��摜��������
	std::vector<CString> vecPath; 
	do {
		if ( ffd.cFileName[0] != '.' ) {
			// �f�B���N�g��
			if ( !(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) ) {
				TCHAR		strDrive[255], strDir[512], strFileName[255], strExt[255];

				_tsplitpath( ffd.cFileName, strDrive, strDir, strFileName, strExt );
				CString strCheckFileName = strFileName;
				if ( lstrcmpi( strExt, _T(".JPG") ) == 0 && strCheckFileName.Find(strIDInMeshData) != -1 ) {
					CString strParameters;
					strParameters += m_strBaseDir + strTargetData + _T("\\PHOTO\\") + cStrPath + _T("\\") + strCheckFileName + _T(".JPG ");
					m_vecPath.push_back( strParameters );
				}
			}
		}	
	} while ( FindNextFile(hFile, &ffd) );

	FindClose(hFile);

	CTrackBarCtrl cTrackBar( GetDlgItem( IDC_SLIDER_PIC ) );
	CStatic	cStatic( GetDlgItem( IDC_STATIC_PICNUM ) );
	CString strPicNum;
	strPicNum.Format( _T("%d"), m_vecPath.size() );
	cStatic.SetWindowText( strPicNum );
	if( m_vecPath.size() > 2 )
	{
		::ShowWindow(cTrackBar, true );
		cTrackBar.SetRange(0,m_vecPath.size()-2 );
	}
	else
		::ShowWindow(cTrackBar, false );
}

