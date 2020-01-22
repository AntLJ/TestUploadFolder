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

// AttrHimaLogDlg.cpp: CAttrHimaLogDlg �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AttrHimaLogDlg.h"

/// �E�B���h�E�}�l�[�W���z�u�}�b�v�i�Ђ܂�胍�O�p�j
BEGIN_WINDOW_MAP(HIMALOG_MAP)
	BEGINROWS(WRCT_REST,0,RCMARGINS(2,2))
		BEGINROWS(WRCT_TOFIT,0,0)
			RCSPACE(3)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_LOG)
				RCTOFIT(IDC_EDIT_LOG)
				RCSPACE(10)
				RCTOFIT(IDC_STATIC_WORKER)
				RCTOFIT(IDC_EDIT_WORKER)
			ENDGROUP()
			RCSPACE(3)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_SHEETID)
				RCTOFIT(IDC_EDIT_SHEETID)
				RCSPACE(10)
				RCTOFIT(IDC_STATIC_KIGOU)
				RCTOFIT(IDC_EDIT_KIGOU)
			ENDGROUP()
			RCSPACE(3)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_GENTYOU_C)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_GENTYOU_C)
			ENDGROUP()
			RCSPACE(3)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_GENTYOU_NAME)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_GENTYOU_NAME)
			ENDGROUP()
			RCSPACE(3)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_KIZON_C)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_KIZON_C)
			ENDGROUP()
			RCSPACE(3)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_KIZON_NAME)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_KIZON_NAME)
			ENDGROUP()
			RCSPACE(3)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_DISP_C)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_DISP_C)
			ENDGROUP()
			RCSPACE(3)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_DISP_NAME)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_DISP_NAME)
			ENDGROUP()
			RCSPACE(3)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_TDCSTATUS)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_TDCSTATUS)
			ENDGROUP()
			RCSPACE(3)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_RESULT)
				RCTOFIT(IDC_EDIT_RESULT)
				RCSPACE(10)
				RCTOFIT(IDC_STATIC_MATCHING)
				RCTOFIT(IDC_EDIT_MATCHING)
			ENDGROUP()
			RCSPACE(3)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_RESULT_SHA)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_RESULT_SHA)
			ENDGROUP()
		ENDGROUP()
		BEGINROWS(WRCT_REST, 0, RCMARGINS(4,4))
			RCTOFIT(IDC_STATIC_REMARKS)
			RCTOFIT(-1)
			RCREST(IDC_EDIT_REMARKS)
		ENDGROUP()
	ENDGROUP()
END_WINDOW_MAP()

/**
 * �N���X�̖����I�����N���s�����߂ɁA���̃N���X��CExportDlg�N���X���p�����A�����_�C�A���O����CExportDlg���쐬���邱�Ƃɂ��
 * �����_�C�A���O������t���ɂ����邱�Ƃ��ł���N���X�{�̂��쐬����܂��B
 * �ڂ�����http://ptvi.hp.infoseek.co.jp/vc/p_0000010.html���Q�Ƃ̂���
 *
 * @retval CAttrHimaLogDlg��Ԃ��܂��B
 */
extern "C" __declspec(dllexport) CExportDlg * __stdcall CreateDlg()
{
    return new CAttrHimaLogDlg();
}

/////////////////////////////////////////////////////////////////////////////
//
// CAttrHimaLogDlg �����o�֐�
//
/////////////////////////////////////////////////////////////////////////////

/** 
 * @fn CAttrHimaLogDlg::CAttrHimaLogDlg()
 * @brief �R���X�g���N�^
 *
 * �u���V�̏����������Ă����܂��B
 */
CAttrHimaLogDlg::CAttrHimaLogDlg() 
	: ATTR_BASE_CLASS(HIMALOG_MAP, IDD_ATTRHIMALOGDLG),
		m_wnd0( _T("EDIT"), this, IDC_EDIT_LOG ),
		m_wnd1( _T("EDIT"), this, IDC_EDIT_WORKER ),
		m_wnd2( _T("EDIT"), this, IDC_EDIT_SHEETID ),
		m_wnd3( _T("EDIT"), this, IDC_EDIT_KIGOU ),
		m_wnd4( _T("EDIT"), this, IDC_EDIT_GENTYOU_C ),
		m_wnd5( _T("EDIT"), this, IDC_EDIT_GENTYOU_NAME ),
		m_wnd6( _T("EDIT"), this, IDC_EDIT_KIZON_C ),
		m_wnd7( _T("EDIT"), this, IDC_EDIT_KIZON_NAME ),
		m_wnd8( _T("EDIT"), this, IDC_EDIT_DISP_C ),
		m_wnd9( _T("EDIT"), this, IDC_EDIT_DISP_NAME ),
		m_wnd10( _T("EDIT"), this, IDC_EDIT_TDCSTATUS ),
		m_wnd11( _T("EDIT"), this, IDC_EDIT_RESULT ),
		m_wnd12( _T("EDIT"), this, IDC_EDIT_MATCHING ),
		m_wnd13( _T("EDIT"), this, IDC_EDIT_RESULT_SHA ),
		m_wnd14( _T("EDIT"), this, IDC_EDIT_REMARKS )
{
	// �t�H���g�쐬
	m_hFont1 = CreateFont(
		12, 0, 0, 0, 100,                           // �����C���C�w�p�x�C�x�p�x�C����
		FALSE, TRUE, FALSE,                        // �ΆC�����C�ŏ���
		SHIFTJIS_CHARSET,                           // �L�����N�^�ݒ�
		OUT_DEFAULT_PRECIS,                         // �o�͐��x
		CLIP_DEFAULT_PRECIS,                        // �N���b�v���x
		DEFAULT_QUALITY,                            // �i��
		FIXED_PITCH | FF_DONTCARE,                // �s�b�`
		_T("�l�r�o�S�V�b�N"));                       // �t�H���g���

	m_hFont2 = CreateFont(
		18, 0, 0, 0, 100,                           // �����C���C�w�p�x�C�x�p�x�C����
		FALSE, FALSE, FALSE,                        // �ΆC�����C�ŏ���
		SHIFTJIS_CHARSET,                           // �L�����N�^�ݒ�
		OUT_DEFAULT_PRECIS,                         // �o�͐��x
		CLIP_DEFAULT_PRECIS,                        // �N���b�v���x
		DEFAULT_QUALITY,                            // �i��
		FIXED_PITCH | FF_DONTCARE,                // �s�b�`
		_T("�l�r�o�S�V�b�N"));                       // �t�H���g���

}

/** 
 * @fn HWND CAttrHimaLogDlg::Create(HWND hWndParent, LPARAM dwInitParam)
 * @brief �q�_�C�A���O���쐬���܂��B
 *
 * ATTR_BASE_CLASS::Create(HWND hWndParent, LPARAM dwInitParam = NULL)�Ń_�C�A���O���쐬���܂��B
 *
 * @param hWndParent	[in]	�e�E�B���h�E�n���h��
 * @param dwInitParam	[in]	�_�C�A���O�������p�����[�^�i���ݓ��Ɏg�p���Ă��܂���j
 *
 * @retval �쐬���ꂽ�q�_�C�A���O�̃n���h����Ԃ��܂��B
 */
HWND CAttrHimaLogDlg::Create(HWND hWndParent, LPARAM dwInitParam)
{ 
	m_hChildWnd = ATTR_BASE_CLASS::Create(hWndParent,dwInitParam);
	return m_hChildWnd;
}

/**
 * @fn void CAttrHimaLogDlg::Delete()
 * @brief �쐬�����q�_�C�A���O���폜���܂��B
 *
 * �_�C�A���O���폜����Ƃ��ɕK���Ă�ŉ������B�Ă΂Ȃ��ƃ��������[�N���Ă��܂��܂��B
 */
void CAttrHimaLogDlg::Delete()
{
	ClearFeatureDefs();
	::DestroyWindow(m_hChildWnd);
    delete this; // �K��new�Ŋm�ۂ���O��
}

/**
 * @fn BOOL CAttrHimaLogDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
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
BOOL CAttrHimaLogDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
{ 
    BOOL bRet = ATTR_BASE_CLASS::SetCurrentFeatureDefIndex( lFeatureIndex, lTableIndex, lRowIndex, 0, bForce, bEditable );

	// ���ڃt�B�[�`���Ɋ֌W�Ȃ��G�f�B�b�g�{�b�N�X(��F�Q�Ɨp�̘H�����V�[�P���X�⌴�e�ԍ�����)�Ȃǂ𐧌䂷��
	// �ҏW�J�n�O�ƕҏW�J�n��ł��ҏW�s�̂Ƃ���DISABLE�ɂ���
	if( IsEditableFeature() && m_cArcHelper.IsStartEditing() && bEditable )
	{
		// �ҏW�J�n����Ă���Ƃ��ɂ��łɁASetFocus���Ă���
		::SetFocus( GetDlgItem(IDC_EDIT_LOG) );
	}

    return bRet;
}

/**
 * @fn void CAttrHimaLogDlg::CreateControlRelation()
 * @brief �q�_�C�A���O�����ۂɍ쐬���܂��B
 *
 * ���ۂ̍쐬��ControlRel�ɔC���܂��B
 */
void CAttrHimaLogDlg::CreateControlRelation()
{
	ATLASSERT( IsWindow() == TRUE );

	if( m_cControlRel.size() > 0 )
	{
		// �e�E�C���h�E�n���h����ݒ肵�Ȃ���
		m_cControlRel.ResetHWND( m_hWnd );
	} 
	else {
		m_cControlRel.SetHWND( m_hWnd );

		// �Ђ܂�胍�O

		// ���O
		m_cControlRel.SetControl( IDC_EDIT_LOG,			TYPE_EDITSTRING,0,	0,	_T("LOG"),			NULL, HIMA_LOG_TABLE_NAME );
		// ��ƎҖ�
		m_cControlRel.SetControl( IDC_EDIT_WORKER,		TYPE_EDITSTRING,0,	0,	_T("W_NAME"),		NULL, HIMA_LOG_TABLE_NAME );
		// �V�[�gID
		m_cControlRel.SetControl( IDC_EDIT_SHEETID,		TYPE_EDITSTRING,0,	0,	_T("SHEETID"),		NULL, HIMA_LOG_TABLE_NAME );
		// �L��
		m_cControlRel.SetControl( IDC_EDIT_KIGOU,		TYPE_EDITSTRING,0,	0,	_T("KIGOU"),		NULL, HIMA_LOG_TABLE_NAME );
		// �������
		m_cControlRel.SetControl( IDC_EDIT_GENTYOU_C,	TYPE_EDITSTRING,0,	0,	_T("CHOUSA_CODE"),	NULL, HIMA_LOG_TABLE_NAME );
		// ��������
		m_cControlRel.SetControl( IDC_EDIT_GENTYOU_NAME,TYPE_EDITSTRING,0,	0,	_T("GENCHO_NAME"),	NULL, HIMA_LOG_TABLE_NAME );
		// �������
		m_cControlRel.SetControl( IDC_EDIT_KIZON_C,		TYPE_EDITSTRING,0,	0,	_T("KIZON_CODE"),	NULL, HIMA_LOG_TABLE_NAME );
		// ��������
		m_cControlRel.SetControl( IDC_EDIT_KIZON_NAME,	TYPE_EDITSTRING,0,	0,	_T("KIZON_NAME"),	NULL, HIMA_LOG_TABLE_NAME );
		// �\�L�C�����
		m_cControlRel.SetControl( IDC_EDIT_DISP_C,		TYPE_EDITSTRING,0,	0,	_T("MODIFY_COD"),	NULL, HIMA_LOG_TABLE_NAME );
		// �\�L�C������
		m_cControlRel.SetControl( IDC_EDIT_DISP_NAME,	TYPE_EDITSTRING,0,	0,	_T("MODIFY_NAME"),	NULL, HIMA_LOG_TABLE_NAME );
		// TDC_STATUS
		m_cControlRel.SetControl( IDC_EDIT_TDCSTATUS,	TYPE_EDITSTRING,0,	0,	_T("TDC_STATUS"),	NULL, HIMA_LOG_TABLE_NAME );
		// �Ή�����
		m_cControlRel.SetControl( IDC_EDIT_RESULT,		TYPE_EDITSTRING,0,	0,	_T("RESULT"),		NULL, HIMA_LOG_TABLE_NAME );
		// ��v����
		m_cControlRel.SetControl( IDC_EDIT_MATCHING,	TYPE_EDITSTRING,0,	0,	_T("MATCHING"),		NULL, HIMA_LOG_TABLE_NAME );
		// ���ʑΉ�����
		m_cControlRel.SetControl( IDC_EDIT_RESULT_SHA,	TYPE_EDITSTRING,0,	0,	_T("RESULT_SHAPE"),	NULL, HIMA_LOG_TABLE_NAME );
		// ���l
		m_cControlRel.SetControl( IDC_EDIT_REMARKS,		TYPE_EDITSTRING,0,	0,	_T("MEMO_"),		NULL, HIMA_LOG_TABLE_NAME );
	}
}
