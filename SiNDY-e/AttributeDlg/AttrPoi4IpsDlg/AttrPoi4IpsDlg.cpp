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

// AttrPoi4IpsDlg.cpp : DLL �A�v���P�[�V�����p�ɃG�N�X�|�[�g�����֐����`���܂��B
//

#include "stdafx.h"
#include "AttrPoi4IpsDlg.h"
#include <boost/assign/list_of.hpp>
#include <boost/foreach.hpp>
#include <TDC/useful_headers/registry_util.h>
#include "TDC/sindylib_core/FieldMap.h"
#include "AccTable.h"
#include <fstream>

using namespace sindy::schema;

// �萔��`
namespace
{
	// ���W�X�g���֘A
	const CString REG_PATH = _T("Software\\INCREMENT P CORPORATION\\SiNDY-e\\Tools\\AttributeTool\\AttrPoi4IpsDlg");
	const CString REG_CONTENTS_MASTER = _T("ContentsMasterFilePath");
	const CString REG_INFOPROVIDE_MASTER = _T("InfoProvideMasterFilePath");
	
	// �R���g���[���֘A
	const CString DELETE_BUTTON_STR = _T("�폜");
	const CString DELETE_BUTTON_REDO_STR = _T("�폜����");

	// �G���[���b�Z�[�W�֘A
	const CString TOOL_NAME = _T("iPS����POI���L�����c�[��");
	const CString REG_KEY_ERR = _T("���W�X�g���ݒ肪�s���ł�");
	const CString FILE_PATH_ERR = _T("���W�X�g���ɐݒ肳��Ă���t�@�C���p�X���s���ł�");

	// �ݒ�l�֘A

	// �����R�@���E�p�~
	const CString CLOSE_C_CLOSE = _T("a");
	// ���񋟌��R�[�h�@�C���N�������gP�i���j
	const CString PST_IP_C_CLOSE_INCREMENT_P = _T("00");
	// ���x�@�s�s�n�}���x�� �ƌ`
	const CString ACC_C_WALL = _T("1S");
}

// �E�B���h�E�}�l�[�W���z�u�}�b�v
BEGIN_WINDOW_MAP(CPOIPOINT_MAP)
	BEGINROWS(WRCT_FIXED,0,RCMARGINS(2,2))
		BEGINROWS(WRCT_FIXED,5,5)
			RCSPACE(5)
			BEGINCOLS(WRCT_FIXED, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_CONTENTS_CODE)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_CONTENTS_CODE)
			ENDGROUP()
			RCSPACE(5)
			BEGINCOLS(WRCT_FIXED, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_CONTENTS_NAME)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_CONTENTS_NAME)
			ENDGROUP()
			RCSPACE(5)
			BEGINCOLS(WRCT_FIXED, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_NAME)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_NAME)
			ENDGROUP()
			RCSPACE(5)
			BEGINCOLS(WRCT_FIXED, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_BUTTON_PP_FOUND)
			ENDGROUP()
			RCSPACE(5)
			BEGINCOLS(WRCT_FIXED, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_PST_IP_C)
				RCTOFIT(-1)
				RCREST(IDC_COMBO_PST_IP_C)
			ENDGROUP()
			RCSPACE(5)
			BEGINCOLS(WRCT_FIXED, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_AAC_C)
				RCTOFIT(-1)
				RCREST(IDC_COMBO_AAC_C)
			ENDGROUP()
			RCSPACE(5)
			BEGINCOLS(WRCT_FIXED, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_BUTTON_DELETE)
			ENDGROUP()
			RCSPACE(5)
			BEGINCOLS(WRCT_FIXED, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_CLOSE_C)
				RCTOFIT(-1)
				RCTOFIT(IDC_EDIT_CLOSE_C)
				RCSPACE(10)
				RCTOFIT(IDC_STATIC_DELETE_C)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_DELETE_C)
			ENDGROUP()
			RCSPACE(5)
			BEGINCOLS(WRCT_FIXED, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_ANNO_DISP_C)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_ANNO_DISP_C)
			ENDGROUP()
		ENDGROUP()
		RCSPACE(10)
		BEGINROWS(WRCT_FIXED, 0, RCMARGINS(10,0))
			BEGINCOLS(WRCT_FIXED, 0, RCMARGINS(0,0))
				RCREST(IDC_STATIC_VAR)
			ENDGROUP()
			RCSPACE(5)
			BEGINCOLS(WRCT_FIXED, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_SOURCE)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_SOURCE)
			ENDGROUP()
		ENDGROUP()
	ENDGROUP()
END_WINDOW_MAP()

/**
 * �N���X�̖����I�����N���s�����߂ɁA���̃N���X��CExportDlg�N���X���p�����A�����_�C�A���O����CExportDlg���쐬���邱�Ƃɂ��
 * �����_�C�A���O������t���ɂ����邱�Ƃ��ł���N���X�{�̂��쐬����܂��B
 * �ڂ�����http://ptvi.hp.infoseek.co.jp/vc/p_0000010.html���Q�Ƃ̂���
 *
 * @retval CAttrPoi4IpsDlg��Ԃ��܂��B
 */
extern "C" __declspec(dllexport) CExportDlg * __stdcall CreateDlg()
{
	return new CAttrPoi4IpsDlg();
}

/////////////////////////////////////////////////////////////////////////////
//
// CAttrPoi4IpsDlg �����o�֐�
//
/////////////////////////////////////////////////////////////////////////////

/** 
 * @fn CAttrPoi4IpsDlg::CAttrPoi4IpsDlg()
 * @brief �R���X�g���N�^
 *
 * �u���V�̏����������Ă����܂��B
 */
CAttrPoi4IpsDlg::CAttrPoi4IpsDlg() 
	: ATTR_BASE_CLASS(CPOIPOINT_MAP, IDD_POIPOINTDLG),
	m_isDelete(true), m_contentsType(LONG_MAX), m_isNewlyPoi(false)
{
	// �e�R���g���[�����T�u�N���X�p�ɏ���
	m_mapSubClass = boost::assign::map_list_of
		( IDC_EDIT_SOURCE, CContainedWindow( _T("EDIT"), this, IDC_EDIT_SOURCE ) )
		( IDC_EDIT_CONTENTS_CODE, CContainedWindow( _T("EDIT"), this, IDC_EDIT_CONTENTS_CODE ) )
		( IDC_EDIT_CONTENTS_NAME, CContainedWindow( _T("EDIT"), this, IDC_EDIT_CONTENTS_NAME ) )
		( IDC_EDIT_NAME, CContainedWindow( _T("EDIT"), this, IDC_EDIT_NAME ) )
		( IDC_BUTTON_PP_FOUND, CContainedWindow( _T("BUTTON"), this, IDC_BUTTON_PP_FOUND ) )
		( IDC_COMBO_PST_IP_C, CContainedWindow( _T("COMBO"), this, IDC_COMBO_PST_IP_C ) )
		( IDC_COMBO_AAC_C, CContainedWindow( _T("COMBO"), this, IDC_COMBO_AAC_C ) )
		( IDC_BUTTON_DELETE, CContainedWindow( _T("BUTTON"), this, IDC_BUTTON_DELETE ) )
		( IDC_EDIT_CLOSE_C, CContainedWindow( _T("EDIT"), this, IDC_EDIT_CLOSE_C ) )
		( IDC_EDIT_DELETE_C, CContainedWindow( _T("EDIT"), this, IDC_EDIT_DELETE_C ) )
		( IDC_EDIT_ANNO_DISP_C, CContainedWindow( _T("EDIT"), this, IDC_EDIT_ANNO_DISP_C ) );
}

/** 
 * @fn HWND CAttrPoi4IpsDlg::Create(HWND hWndParent, LPARAM dwInitParam)
 * @brief �q�_�C�A���O���쐬���܂��B
 *
 * ATTR_BASE_CLASS::Create(HWND hWndParent, LPARAM dwInitParam = NULL)�Ń_�C�A���O���쐬���܂��B
 *
 * @param hWndParent	[in]	�e�E�B���h�E�n���h��
 * @param dwInitParam	[in]	�_�C�A���O�������p�����[�^�i���ݓ��Ɏg�p���Ă��܂���j
 *
 * @retval �쐬���ꂽ�q�_�C�A���O�̃n���h����Ԃ��܂��B
 */
HWND CAttrPoi4IpsDlg::Create(HWND hWndParent, LPARAM dwInitParam)
{ 
	return ATTR_BASE_CLASS::Create(hWndParent,dwInitParam);
}

/**
 * @fn void CAttrPoi4IpsDlg::Delete()
 * @brief �쐬�����q�_�C�A���O���폜���܂��B
 *
 * �_�C�A���O���폜����Ƃ��ɕK���Ă�ŉ������B�Ă΂Ȃ��ƃ��������[�N���Ă��܂��܂��B
 */
void CAttrPoi4IpsDlg::Delete()
{
	ClearFeatureDefs();
	if( IsWindow() )
		DestroyWindow();
	delete this; // �K��new�Ŋm�ۂ���O��
}

/**
 * @fn BOOL CAttrPoi4IpsDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
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
BOOL CAttrPoi4IpsDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
{ 
	BOOL bRet = ATTR_BASE_CLASS::SetCurrentFeatureDefIndex( lFeatureIndex, lTableIndex, lRowIndex, 0, bForce, bEditable );

	// read only
	CEdit(GetDlgItem(IDC_EDIT_CONTENTS_CODE)).SetReadOnly(TRUE);
	CEdit(GetDlgItem(IDC_EDIT_CONTENTS_NAME)).SetReadOnly(TRUE);
	CEdit(GetDlgItem(IDC_EDIT_NAME)).SetReadOnly(TRUE);
	CEdit(GetDlgItem(IDC_EDIT_CLOSE_C)).SetReadOnly(TRUE);
	CEdit(GetDlgItem(IDC_EDIT_DELETE_C)).SetReadOnly(TRUE);
	CEdit(GetDlgItem(IDC_EDIT_ANNO_DISP_C)).SetReadOnly(TRUE);

	// �I������Ă���t�B�[�`���̏����擾�������o�ϐ��ɃZ�b�g
	getCurrentFeatureInfo();
	// ���񋟌��R�[�h�R���{�{�b�N�X�쐬
	createPstIpCombo();
	// ���x�R���{�{�b�N�X�쐬
	createAcc();
	// �{�_�C�A���O�Ŏg�p����h���C�����쐬�������o�ϐ��ɃZ�b�g
	createStrKeyDomain();
	
	// GRPCODE-CHAINCODE-ContentsSeq�̌`�ŃR���e���c�R�[�h��\��
	GetDlgItem(IDC_EDIT_CONTENTS_CODE).SetWindowText( uh::str_util::format(_T("%s-%s-%s"), m_grpcode, m_chainCode, m_contentsSeq));
	// �R���e���c���̂�\��
	GetDlgItem(IDC_EDIT_CONTENTS_NAME).SetWindowText( uh::str_util::format(_T("%s"), m_contentsName ));

	// ���O�łŊǗ����Ă���EditBox�Q�̏����\�������s
	setEditBoxByDomainVal(IDC_EDIT_CLOSE_C, sindyk::poi_point::kCloseCode);
	setEditBoxByDomainVal(IDC_EDIT_DELETE_C, sindyk::poi_point::kDelete );
	setEditBoxByDomainVal(IDC_EDIT_ANNO_DISP_C, sindyk::poi_point::kAnnoDispCode);

	// ���ڃt�B�[�`���Ɋ֌W�Ȃ��G�f�B�b�g�{�b�N�X(��F�Q�Ɨp�̘H�����V�[�P���X�⌴�e�ԍ�����)�Ȃǂ𐧌䂷��
	// �ҏW�J�n�O�ƕҏW�J�n��ł��ҏW�s�̂Ƃ���DISABLE�ɂ���
	if( IsEditableFeature() && m_cArcHelper.IsStartEditing() && bEditable )
	{
		// �ŏ��Ƀt�H�[�J�X�����R���g���[��
		::SetFocus( GetDlgItem(IDC_EDIT_SOURCE) );

		// ���ʒu�����{�^���A�N�e�B�u������
		// �R���e���c�^�C�v�����p�E�o�n�h��p�̏ꍇ�A�{�^���A���񋟌��R�[�h�A���x���A�N�e�B�u�Ƃ���B
		GetDlgItem(IDC_BUTTON_PP_FOUND).EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_AAC_C).EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_PST_IP_C).EnableWindow(TRUE);
		if ( !( m_contentsType == sindyk::contents_master::contents_type_code::kCommon ||
			m_contentsType == sindyk::contents_master::contents_type_code::kPOI )
			)
		{
			GetDlgItem(IDC_BUTTON_PP_FOUND).EnableWindow(FALSE);
			GetDlgItem(IDC_COMBO_AAC_C).EnableWindow(FALSE);
			GetDlgItem(IDC_COMBO_PST_IP_C).EnableWindow(FALSE);
		}

		// �폜�{�^���A�N�e�B�u������
		// �R���e���c�^�C�v���ȉ��̏ꍇ�A�N�e�B�u�Ƃ���B
		// �E���p
		// �E���L��p���V�KPOI�ȊO
		GetDlgItem(IDC_BUTTON_DELETE).EnableWindow(TRUE);
		m_isDelete = judgeDeleteFlg();
		if (m_isDelete)
			GetDlgItem(IDC_BUTTON_DELETE).SetWindowText(DELETE_BUTTON_STR);
		else
			GetDlgItem(IDC_BUTTON_DELETE).SetWindowText(DELETE_BUTTON_REDO_STR);

		if ( !( m_contentsType == sindyk::contents_master::contents_type_code::kCommon ||
			    (m_contentsType == sindyk::contents_master::contents_type_code::kAnno && !m_isNewlyPoi))
			)
		{
			GetDlgItem(IDC_BUTTON_DELETE).SetWindowText(DELETE_BUTTON_STR);
			GetDlgItem(IDC_BUTTON_DELETE).EnableWindow(FALSE);
		}
	}
	else
	{
		GetDlgItem(IDC_COMBO_AAC_C).EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_PST_IP_C).EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_DELETE).EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_PP_FOUND).EnableWindow(FALSE);
	}

	return bRet;
}

/**
 * @fn void CAttrPoi4IpsDlg::CreateControlRelation()
 * @brief �q�_�C�A���O�����ۂɍ쐬���܂��B
 *
 * ���ۂ̍쐬��ControlRel�ɔC���܂��B
 */
void CAttrPoi4IpsDlg::CreateControlRelation()
{
	ATLASSERT( IsWindow() );

	if( !m_cControlRel.empty() )
	{
		// �e�E�C���h�E�n���h����ݒ肵�Ȃ���
		m_cControlRel.ResetHWND( m_hWnd );
	} 
	else {
		m_cControlRel.SetHWND( m_hWnd );

		// ControlRel�Ƀt�B�[���h���ƃe�[�u������o�^
		// ����
		m_cControlRel.SetControl( IDC_EDIT_NAME, TYPE_EDITSTRING, 0, 0, sindyk::poi_point::kName, NULL, sindyk::poi_point::kTableName );
		// ���\�[�X
		m_cControlRel.SetControl( IDC_EDIT_SOURCE, TYPE_EDITSTRING, 0, 0, ipc_table::kSource, 0, sindyk::poi_point::kTableName );

		// �����񋟌��R�[�h�A���x�A�����R�A�폜�R�[�h�A���L�\���R�[�h�͎��O�ōs��
	}
}

// �e�[�u���̒�����idx�Ԗڂ̒l���擾
std::pair<CString, CString> getValByIdx(const StringMasterTbl& tbl , int idx)
{
	ASSERT(tbl.size() < idx);
	int cnt(0);
	for( const auto&  rec :  tbl)
	{
		if (cnt == idx)
			return rec.second;

		++cnt;
	}

	return std::pair<CString, CString>();
}

// �R���{�{�b�N�X�̃A�C�e����FieldDef�ɃZ�b�g
void CAttrPoi4IpsDlg::setFieldDef4Combo(const StringMasterTbl& tbl, UINT id,  const LPCTSTR lpszFieldName)
{
	int idx = CComboBox(GetDlgItem(id)).GetCurSel();
	std::pair<CString, CString> pair = getValByIdx( tbl, idx);
	CFieldDef* pFieldDef = GetCurrentFeatureDef()->GetFieldDef(sindyk::poi_point::kTableName, lpszFieldName );
	if( 0 != _tcscmp( pFieldDef->m_vaValue.bstrVal, pair.first) )
	{
		pFieldDef->m_vaValue = pair.first;
		pFieldDef->m_bChanged = TRUE;
	}

	// �X�V���ύX
	updateChange(pFieldDef);

	// �X�V���t�^
	UpdateHistory(GetCurrentFeatureDef());
}

LRESULT  CAttrPoi4IpsDlg::setPstIPCode(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// ���񋟌��R�[�h�R���{�{�b�N�X�̒l��FieldDef��ύX
	setFieldDef4Combo(m_pstIpCodeMaster, IDC_COMBO_PST_IP_C, sindyk::poi_point::kPSTIPCode);

	return 0;
}

LRESULT  CAttrPoi4IpsDlg::setAccCode(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// ���x�R���{�{�b�N�X�̒l��FieldDef��ύX
	setFieldDef4Combo(m_AccCodeMaster, IDC_COMBO_AAC_C, sindyk::poi_point::kACCCode);

	return 0;
}

// �C�ӂ̕�����̓Y�������擾����
size_t getIdx(std::vector<CString> src, const CString target )
{
	std::vector<CString>::iterator iter = std::find(src.begin(), src.end(), target);
	return std::distance(src.begin(), iter);
}

// ���W�X�g���l���擾
bool CAttrPoi4IpsDlg::getRegKey(const CString& path , const CString& key, CString& out)
{
	// �p�X���擾
	out = uh::reg_util::read( key, path );

	// �L�[���Ȃ��A�܂��͒l����Ȃ狖��
	if( out.IsEmpty() )
	{
		CString msg = uh::str_util::format( _T("%s\n���W�X�g���p�X:%s"), REG_KEY_ERR.GetString(), key.GetString() );
		AttrMessageBox( msg.GetString(), TOOL_NAME.GetString(), MB_OK, NULL);
		return false;
	}

	return true;
}

// ���݂̑I������Ă���t�B�[�`������K�v�ȏ����擾����
void CAttrPoi4IpsDlg::getCurrentFeatureInfo()
{
	// �I������Ă���t�B�[�`�����擾
	IFeaturePtr feature = GetCurrentFeatureDef()->GetFeature();
	// �t�B�[���h�}�b�v�쐬�������o�ϐ��ɃZ�b�g
	m_fieldMap = sindy::CFieldMap( feature->GetFields() );

	//�R���e���c�}�X�^���ǂݍ��܂�Ă��Ȃ��ꍇ�̓��^�[��
	if(m_contentsMaster.empty())
		return;

	// GRPCODE,CHAINCODE,�R���e���c����,�R���e���c�^�C�v�̈ʒu�����
	long grpcodeIdx = getIdx(m_contentsMaster[0], sindyk::contents_master::kGroupCode );
	long chainCodeIdx = getIdx(m_contentsMaster[0], sindyk::contents_master::kChainCode );
	long contentsTypeIdx = getIdx(m_contentsMaster[0], sindyk::contents_master::kContentsType);
	long nameIdx = getIdx(m_contentsMaster[0], sindyk::contents_master::kName);

	// �R���e���c�R�[�h����GRPCODE,CHAINCODE���擾
	CString tmpContents = feature->GetValue( m_fieldMap.FindField( sindyk::poi_point::kContentsCode ) );
	long tmpGrpcode = _ttoi( tmpContents.Left( 4 ) ); // �R���e���c�R�[�h�̏�4����GRPCODE
	long tmpChainCode = _ttoi( tmpContents.Right( 5 ) ); // �R���e���c�R�[�h�̏�5����CHAINCODE

	// �R���e���c�}�X�^����R���e���c�R�[�h�ɕR�Â����R�[�h�����
	bool isHeader = true;
	bool isExist = false;
	for( const auto&  line :  m_contentsMaster )
	{

		// �w�b�_�͏ȗ�
		if ( isHeader )
		{
			isHeader = false;
			continue;
		}

		// �R���e���c�R�[�h�ɕR�Â����R�[�h���擾����
		if ( _ttoi(line[grpcodeIdx]) == tmpGrpcode && _ttoi(line[chainCodeIdx]) == tmpChainCode )
		{
			isExist = true;
			// GRPCODE���Z�b�g
			m_grpcode = line[grpcodeIdx];
			// CHAINCODE���Z�b�g
			m_chainCode = line[chainCodeIdx];
			// �R���e���c�^�C�v���Z�b�g
			m_contentsType = _ttoi(line[contentsTypeIdx]);
			// �R���e���c���̂��Z�b�g
			m_contentsName = line[nameIdx];

			break;
		}
	}

	if (!isExist)
		return;

	// ContentsSeq���Z�b�g
	m_contentsSeq = feature->GetValue( m_fieldMap.FindField( sindyk::poi_point::kContentsSeq ) );

	// �V�KPOI���𔻒�
	m_isNewlyPoi = m_contentsSeq.Find(_T("-")) != -1;
}

// �R���{�{�b�N�X�쐬(�ėp)
void CAttrPoi4IpsDlg::createComboBox(UINT id, const StringMasterTbl& tbl, LPCTSTR lpszTableName, LPCTSTR lpszFieldName)
{

	// �����I���ׂ̈�FieldDef���擾���Ă���
	CFieldDef *pFieldDef = GetCurrentFeatureDef()->GetFieldDef( lpszTableName, lpszFieldName );

	// �R���{�{�b�N�X�ɃA�C�e����ǉ�
	CComboBox cCombo = CComboBox(GetDlgItem(id));
	cCombo.ResetContent();
	int hitPos = INT_MAX;
	for ( const auto& line : tbl )
	{
		const std::pair<CString, CString>& masterRec = line.second;

		// �\����������쐬
		CString disp;
		disp.Format(_T("%s(%s)"), masterRec.second , masterRec.first);

		INT iCurPos = cCombo.AddString( disp );
		cCombo.SetItemData( iCurPos, line.first );

		// �I������Ă���t�B�[�`���̒l�������ꍇ�A���̈ʒu��I���s�Ƃ���
		if( pFieldDef->m_vaValue == masterRec.first )
			cCombo.SetCurSel( iCurPos );
	}
}

// ���񋟌��R�[�h�R���{�{�b�N�X�쐬
void CAttrPoi4IpsDlg::createPstIpCombo()
{
	// �R���{�{�b�N�X�ɓ����鍀�ڂ����݂��Ȃ��ꍇ�쐬
	if ( m_pstIpCodeMaster.empty() )
	{
		// ���W�X�g��������񋟌��R�[�h�}�X�^�̃p�X���擾
		CString regString;
		if( !getRegKey( REG_PATH, REG_INFOPROVIDE_MASTER, regString) )
			return;

		// ���񋟌��R�[�h�}�X�^�ǂݍ���
		const std::vector<std::vector<CString>>& infoProvideMaster =  parseTsvFile( regString );
		if (infoProvideMaster.empty())
			return;

		// IP_CODE,NAME,�̈ʒu�����
		long ipCodeIdx = getIdx(infoProvideMaster[0], sindyk::infoprovide_master::kIPCode );
		long nameIdx = getIdx(infoProvideMaster[0],  sindyk::infoprovide_master::kName );
		long mntPriorityIdx = getIdx(infoProvideMaster[0],  sindyk::infoprovide_master::kMNTPriority );

		// ���񋟌��R�[�h�}�X�^����IP_CODE,NAME���擾
		bool isHeader = true;
		for( const auto&  line : infoProvideMaster )
		{
			// �w�b�_�͏ȗ�
			if ( isHeader )
			{
				isHeader = false;
				continue;
			}

			// �����D�揇�ʂ�-1�͏ȗ�
			if ( _ttoi( line[mntPriorityIdx] ) == -1 )
				continue;

			m_pstIpCodeMaster[ _ttoi(line[mntPriorityIdx]) ] = std::pair<CString,CString>(line[ipCodeIdx],line[nameIdx]);
		}
	}

	// �R���{�{�b�N�X�쐬
	createComboBox(IDC_COMBO_PST_IP_C, m_pstIpCodeMaster, sindyk::poi_point::kTableName, sindyk::poi_point::kPSTIPCode);
}

// ���x�R���{�{�b�N�X�쐬
void CAttrPoi4IpsDlg::createAcc()
{
	// �R���{�{�b�N�X�쐬
	createComboBox(IDC_COMBO_AAC_C, m_AccCodeMaster, sindyk::poi_point::kTableName, sindyk::poi_point::kACCCode);
}

// �t�B�[���h�̐ݒ�l���h���C���̒l�ɕϊ����G�f�B�b�g�{�b�N�X�ɓ����
void CAttrPoi4IpsDlg::setEditBoxByDomainVal(int id, LPCTSTR lpszFieldName)
{
	using namespace sindyk::poi_point;

	CFieldDef* pFieldDef = GetCurrentFeatureDef()->GetFieldDef(kTableName, lpszFieldName );
	if ( pFieldDef->m_vaValue.vt != VT_NULL )
	{
		CString dsp;
		dsp.Format(_T("%s(%s)"), m_strKeyDomainMap[lpszFieldName][CString(pFieldDef->m_vaValue)], CString(pFieldDef->m_vaValue));
		CEdit(GetDlgItem(id)).SetWindowText( dsp.GetString() );
	}
	else
		CEdit(GetDlgItem(id)).SetWindowText(_T("") );
}

// �폜�{�^�����������ꂽ�ꍇ�̏���
LRESULT CAttrPoi4IpsDlg::onPressDelete(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	using namespace sindyk::contents_master::contents_type_code;
	using namespace sindyk::poi_point;

	// �폜�̏ꍇ
	if ( m_isDelete )
	{
		// ���p�̏ꍇ
		if( m_contentsType == kCommon )
		{
			//�u���L�\���R�[�h�v�Ɂu��\���v��ݒ肷��B
			CFieldDef* pFieldDef = GetCurrentFeatureDef()->GetFieldDef(kTableName, kAnnoDispCode );
			pFieldDef->m_vaValue =  annodisp_code::kNone;
			// �X�V�t���O�ύX
			updateChange(pFieldDef);
			// ��ʕ\���X�V
			setEditBoxByDomainVal(IDC_EDIT_ANNO_DISP_C, kAnnoDispCode);
		}
		// ���L��p���V�KPOI�ȊO
		else
		{
			//�u�폜�R�[�h�v�Ɂu1�v�A�u�����R�v�Ɂu���E�p�~�v��ݒ肷��B
			CFieldDef* pFieldDef = GetCurrentFeatureDef()->GetFieldDef(kTableName, kDelete );
			pFieldDef->m_vaValue =  delete_code::kDead ;
			// �X�V�t���O�ύX
			updateChange(pFieldDef);
			// ��ʕ\���X�V
			setEditBoxByDomainVal(IDC_EDIT_DELETE_C, kDelete);
			
			pFieldDef = GetCurrentFeatureDef()->GetFieldDef(kTableName, kCloseCode );
			pFieldDef->m_vaValue =CLOSE_C_CLOSE;
			// �X�V�t���O�ύX
			updateChange(pFieldDef);
			// ��ʕ\���X�V
			setEditBoxByDomainVal( IDC_EDIT_CLOSE_C, kCloseCode );
		}
	} 
	// �폜�����̏ꍇ
	else
	{
		// ���p�̏ꍇ
		if( m_contentsType == kCommon )
		{
			//�u���L�\���R�[�h�v�Ɂu�W���v��ݒ肷��B
			CFieldDef* pFieldDef = GetCurrentFeatureDef()->GetFieldDef(kTableName, kAnnoDispCode );
			pFieldDef->m_vaValue =  annodisp_code::kNomal;
			// �X�V�t���O�ύX
			updateChange(pFieldDef);
			// ��ʕ\���X�V
			setEditBoxByDomainVal(IDC_EDIT_ANNO_DISP_C, kAnnoDispCode);

		}
		// ���L��p���V�KPOI�ȊO
		else
		{
			//�u�폜�R�[�h�v�Ɂu0�v�A�u�����R�v�ɁuNULL�v��ݒ肷��B
			CFieldDef* pFieldDef = GetCurrentFeatureDef()->GetFieldDef(kTableName, kDelete );
			pFieldDef->m_vaValue =  delete_code::kAlive ;
			// �X�V�t���O�ύX
			updateChange(pFieldDef);
			// ��ʕ\���X�V
			setEditBoxByDomainVal(IDC_EDIT_DELETE_C, kDelete);

			pFieldDef = GetCurrentFeatureDef()->GetFieldDef(kTableName, kCloseCode );
			pFieldDef->m_vaValue.vt = VT_NULL;
			// �X�V�t���O�ύX
			updateChange(pFieldDef);
			// ��ʕ\���X�V
			setEditBoxByDomainVal( IDC_EDIT_CLOSE_C, kCloseCode );
		}
	}

	//�{�^������p�t���O�𔽓]
	m_isDelete = !m_isDelete;

	// �폜�{�^���̖��̂�ύX
	CButton cButton = CButton(GetDlgItem(IDC_BUTTON_DELETE));
	if (m_isDelete)
		cButton.SetWindowText(DELETE_BUTTON_STR);
	else
		cButton.SetWindowText(DELETE_BUTTON_REDO_STR);

	// �X�V���t�^
	UpdateHistory(GetCurrentFeatureDef());
	// �_�C�A���O���X�V�̈�ɒǉ����܂�
	InvalidateRect( NULL, FALSE );
	// �ύX���ꂽ�ꍇ�̓{�^���� Enable ��
	SetButton( Changed() );

	return 0;
}

// ���ʒu�����{�^�����������ꂽ�ꍇ�̏���
LRESULT CAttrPoi4IpsDlg::onPressPpFound(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	// ���񋟌��R�[�h���u00 �C���N�������gP�i���j�ɕύX
	CComboBox pstIpCombo = CComboBox( GetDlgItem(IDC_COMBO_PST_IP_C) );
	pstIpCombo.SetCurSel( getComboIdx(PST_IP_C_CLOSE_INCREMENT_P,  m_pstIpCodeMaster) );
	CFieldDef* pFieldDef = GetCurrentFeatureDef()->GetFieldDef(sindyk::poi_point::kTableName, sindyk::poi_point::kPSTIPCode );
	if(0 != _tcscmp( pFieldDef->m_vaValue.bstrVal, PST_IP_C_CLOSE_INCREMENT_P))
	{
		pFieldDef->m_vaValue = PST_IP_C_CLOSE_INCREMENT_P;
		// �X�V�t���O�ύX
		updateChange(pFieldDef);
	}

	// �u���x�v���u1S�v�ɕύX
	CComboBox accCombo = CComboBox( GetDlgItem(IDC_COMBO_AAC_C) );
	accCombo.SetCurSel( getComboIdx(ACC_C_WALL,  m_AccCodeMaster) );
	pFieldDef = GetCurrentFeatureDef()->GetFieldDef(sindyk::poi_point::kTableName, sindyk::poi_point::kACCCode );
	if(0 != _tcscmp( pFieldDef->m_vaValue.bstrVal, ACC_C_WALL))
	{
		pFieldDef->m_vaValue = ACC_C_WALL;
		// �X�V�t���O�ύX
		updateChange(pFieldDef);
	}

	// �X�V���t�^
	UpdateHistory(GetCurrentFeatureDef());

	// �_�C�A���O���X�V�̈�ɒǉ����܂�
	InvalidateRect( NULL, FALSE );
	// �ύX���ꂽ�ꍇ�̓{�^���� Enable ��
	SetButton( Changed() );

	return 0;
}

// �}�X�^����R���{�{�b�N�X�̃V�[�P���X���擾����
INT CAttrPoi4IpsDlg::getComboIdx(const CString& ipCode, const StringMasterTbl& tbl)
{
	// �R���{�{�b�N�X�̃V�[�P���X��map�̃\�[�g��(���Ȃ킿�\���D�揇�ʏ�)�̂͂�
	UINT ret(UINT_MAX);
	UINT cnt(0);
	for( const auto& line : tbl )
	{
		if (line.second.first == ipCode )
		{
			ret = cnt;
			break;
		}
		++cnt;
	}
	return ret;
}

// ��������؂蕶���ŕ���
std::vector<CString> spritStr(CString str, CString delim)
{
	std::vector<CString> ret;

	CString st;
	int indexback = 0;
	int i;

	// �ꕶ�������
	for( i = 0; i < str.GetLength(); i++)
	{
		// ��؂蕶��������������
		if (str.GetAt(i) == delim.GetAt(0))
		{
			if ((i - indexback) == 0)
				ret.push_back(_T(""));
			else
			{
				// ��؂蕶������O�̕�������擾
				st = str.Mid(indexback, i - indexback);
				ret.push_back(st);
			}
			indexback = i + 1;
		}
	}

	if ((i - indexback) != 0)
	{
		st = str.Mid(indexback, i - indexback);
		ret.push_back(st);
	}

	return ret;
}

// TSV�t�@�C����2�����z��ŕԋp����
std::vector<std::vector<CString>> CAttrPoi4IpsDlg::parseTsvFile(const CString& file)
{
	std::vector< std::vector<CString> > ret;
	std::list<CString> lists;
	if( !uh::isFile(file.GetString()) )
	{
		CString msg = uh::str_util::format( _T("%s\n�t�@�C���p�X:%s"),FILE_PATH_ERR.GetString(), file.GetString() );
		AttrMessageBox( msg.GetString(), TOOL_NAME.GetString(), MB_OK, NULL);

		return ret;
	}

	// �t�@�C������1�s���ǂݍ���Ŋi�[
	std::ifstream ifs(file.GetString());
	if( !(ifs && ifs.good()) )
		return ret;

	std::string buf;
	while(std::getline(ifs, buf))
	{
		lists.push_back( uh::toTStr(buf).c_str() );
	}

	for( const auto&  line : lists)
	{
		// �^�u��؂���΂炵�Ċi�[
		ret.push_back( spritStr( line,_T("\t")));
	}
	return ret;
}

// �X�V����t�^
bool CAttrPoi4IpsDlg::UpdateHistory(CFeatureDef* featureDef)
{
	//�@ �X�V�����������񏉊�������

	// WORK_DATE�����ɖ߂�
	CFieldDef* pFieldDef = featureDef->GetFieldDef(sindyk::poi_point::kTableName, sindyk::poi_point::kWorkDate );
	pFieldDef->m_vaValue = pFieldDef->m_vaInitValue;
	pFieldDef->m_bChanged = FALSE;
	// WORK_DATE�����ɖ߂�
	pFieldDef = featureDef->GetFieldDef(sindyk::poi_point::kTableName, sindyk::poi_point::kVerifyDate );
	pFieldDef->m_vaValue  = pFieldDef->m_vaInitValue;
	pFieldDef->m_bChanged = FALSE;

	// WORKER�����ɖ߂�
	pFieldDef = featureDef->GetFieldDef(sindyk::poi_point::kTableName, sindyk::poi_point::kWorker );
	pFieldDef->m_vaValue  = pFieldDef->m_vaInitValue;
	pFieldDef->m_bChanged = FALSE;
	// VERIFIER�����ɖ߂�
	pFieldDef = featureDef->GetFieldDef(sindyk::poi_point::kTableName, sindyk::poi_point::kVerifier );
	pFieldDef->m_vaValue = pFieldDef->m_vaInitValue;
	pFieldDef->m_bChanged = FALSE;

	// ��������ύX���������ꍇ�X�V���s��
	if (featureDef->Changed())
	{
		// �ŏI�v���O�����X�V����
		SYSTEMTIME st;
		::GetLocalTime( &st );
		CString strTime;
		strTime.Format( _T("%04d/%02d/%02d %02d:%02d:%02d"), 
			st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond );

		// WORK_DATE���X�V
		CFieldDef* pFieldDef = featureDef->GetFieldDef(sindyk::poi_point::kTableName, sindyk::poi_point::kWorkDate );
		pFieldDef->m_vaValue = strTime;
		pFieldDef->m_bChanged = TRUE;
		//VERIFY_DATE���X�V
		pFieldDef = featureDef->GetFieldDef(sindyk::poi_point::kTableName, sindyk::poi_point::kVerifyDate );
		pFieldDef->m_vaValue = strTime;
		pFieldDef->m_bChanged = TRUE;

		// �X�V��
		CComBSTR bstrOperator;
		m_ipRule->get_OperatorName( &bstrOperator );
		// WORKER���X�V
		pFieldDef = featureDef->GetFieldDef(sindyk::poi_point::kTableName, sindyk::poi_point::kWorker );
		pFieldDef->m_vaValue = bstrOperator;
		pFieldDef->m_bChanged = TRUE;
		// VERIFIER���X�V
		pFieldDef = featureDef->GetFieldDef(sindyk::poi_point::kTableName, sindyk::poi_point::kVerifier );
		pFieldDef->m_vaValue = bstrOperator;
		pFieldDef->m_bChanged = TRUE;
	}

	return true;
}

// �폜�{�^���t���O�����݂̏󋵂��画�f����
bool CAttrPoi4IpsDlg::judgeDeleteFlg()
{
	using namespace sindyk::contents_master::contents_type_code;
	using namespace sindyk::poi_point;

	// ���p�̏ꍇ
	if( m_contentsType == kCommon )
	{
		//�u���L�\���R�[�h�v�Ɂu��\���v�ȊO���H
		CFieldDef* annoDispCodeDef = GetCurrentFeatureDef()->GetFieldDef(kTableName, kAnnoDispCode );
		if (annoDispCodeDef->m_vaValue.lVal != annodisp_code::kNone)
			return true;
	}
	// ���L��p���V�KPOI�ȊO
	else
	{
		//�u�폜�R�[�h�v�Ɂu1�v�ȊO �܂��́u�����R�v���u���E�p�~�v�ȊO���H
		CFieldDef* deleteCDef = GetCurrentFeatureDef()->GetFieldDef(kTableName, kDelete );
		CFieldDef* closeCDef = GetCurrentFeatureDef()->GetFieldDef(kTableName, kCloseCode );
		if (deleteCDef->m_vaValue.lVal != delete_code::kDead ||
			!( closeCDef->m_vaValue.vt != VT_NULL && 0 == _tcscmp( closeCDef->m_vaValue.bstrVal,CLOSE_C_CLOSE))
			)
			return true;
	}

	return false;
}

// �X�V�t���O��ύX����
void CAttrPoi4IpsDlg::updateChange(CFieldDef* pFieldDef)
{
	if ( pFieldDef->m_vaValue == pFieldDef->m_vaInitValue)
		pFieldDef->m_bChanged = FALSE;
	else
		pFieldDef->m_bChanged = TRUE;
}

// �L�[��String�ł���h���C�����擾����
std::map< CString, CString> CAttrPoi4IpsDlg::getStrKeyDomein(const LPCTSTR lpszFieldName)
{
	std::map< CString, CString> ret;
	IDomainPtr ipDomain;

	GetCurrentFeatureDef()->GetFieldDef(sindyk::poi_point::kTableName, lpszFieldName )->m_ipField->get_Domain( &ipDomain );

	ICodedValueDomainPtr ipCodedValueDomain( ipDomain );

	long lCodeNum = 0;
	ipCodedValueDomain->get_CodeCount( &lCodeNum );

	for( int i = 0; i < lCodeNum; ++i )
	{
		CComVariant vaValue;
		CComBSTR bstrName;
		ipCodedValueDomain->get_Value( i, &vaValue );
		ipCodedValueDomain->get_Name( i, &bstrName );
		ret[ CString(vaValue) ] = CString(bstrName);
	}

	return ret;
}

// �{�_�C�A���O�Ŏg�p����h���C�����擾����
void CAttrPoi4IpsDlg::createStrKeyDomain()
{
	using namespace sindyk::poi_point;
	
	if ( m_strKeyDomainMap.end() == m_strKeyDomainMap.find( kAnnoDispCode) )
		m_strKeyDomainMap[ kAnnoDispCode ] = getStrKeyDomein( kAnnoDispCode );
	if ( m_strKeyDomainMap.end() == m_strKeyDomainMap.find( kDelete) )
		m_strKeyDomainMap[ kDelete ] = getStrKeyDomein( kDelete );
	if ( m_strKeyDomainMap.end() == m_strKeyDomainMap.find( kCloseCode) )
		m_strKeyDomainMap[ kCloseCode ] = getStrKeyDomein( kCloseCode );

}

// �R���e���c�}�X�^�ǂݍ���
bool  CAttrPoi4IpsDlg::readContentsMaster()
{
	// ���W�X�g������R���e���c�}�X�^�̃p�X���擾
	CString regString;
	if( !getRegKey(REG_PATH, REG_CONTENTS_MASTER, regString) )
		return false;

	// �R���e���c�}�X�^�ǂݍ���
	m_contentsMaster = parseTsvFile( regString );
	if ( m_contentsMaster.empty() )
		return false;

	return true;
}
