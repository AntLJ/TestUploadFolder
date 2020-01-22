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

// AttrFacilInfoPointDlg.cpp: CAttrFacilInfoPointDlg �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WinLib/libwinlib.h"
#include "crd_cnv/coord_converter.h"
#include "AttrDlgLib.h"
#include "sindy/libschema.h"
#include "AttrFacilInfoPointDlg.h"

/// �E�B���h�E�}�l�[�W���z�u�}�b�v�i�����E�L�����H�����֌W�|�C���g�p�j
BEGIN_WINDOW_MAP(FACIL_INFO_POINT_MAP)
	BEGINROWS(WRCT_REST,0,RCMARGINS(2,2))
		BEGINROWS(WRCT_PCT,0,90)
			RCSPACE(7)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_ROADCLASS_REF)
				RCTOFIT(-1)
				RCREST(IDC_COMBO_ROADCLASS_REF)
			ENDGROUP()
			RCSPACE(5)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_ROADNAME_REF)
				RCTOFIT(-1)
				RCREST(IDC_COMBO_ROADNAME_REF)
			ENDGROUP()
			RCSPACE(5)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_ROSENCODE)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_ROSENCODE)
			ENDGROUP()
			RCSPACE(5)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_SEQUENCE)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_SEQUENCE)
			ENDGROUP()
			RCSPACE(12)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_FACILKANJI)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_FACILKANJI)
			ENDGROUP()
			RCSPACE(7)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_FACILKANA)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_FACILKANA)
			ENDGROUP()
			RCSPACE(7)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_CHECK_DUMMYFLAG)
			ENDGROUP()
			RCSPACE(7)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_FACILCLASS)
				RCTOFIT(-1)
				RCREST(IDC_COMBO_FACILCLASS)
			ENDGROUP()
			RCSPACE(7)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_SEARCHFACIL)
				RCTOFIT(-1)
				RCREST(IDC_COMBO_SEARCHFACIL)
			ENDGROUP()
			RCSPACE(7)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_CHECK_AUTOPOSITION_F)
			ENDGROUP()
			RCSPACE(7)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_CHECK_DUMMY)
			ENDGROUP()
		ENDGROUP()
		BEGINROWS(WRCT_PCT, 0, 10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(0,0))
				RCREST(IDC_STATIC_VAR)
			ENDGROUP()
			RCSPACE(5)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
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
 * @retval CAttrFacilInfoPointDlg��Ԃ��܂��B
 */
extern "C" __declspec(dllexport) CExportDlg * __stdcall CreateDlg()
{
    return new CAttrFacilInfoPointDlg();
}

/////////////////////////////////////////////////////////////////////////////
//
// CAttrFacilInfoPointDlg �����o�֐�
//
/////////////////////////////////////////////////////////////////////////////

/** 
 * @fn CAttrFacilInfoPointDlg::CAttrFacilInfoPointDlg()
 * @brief �R���X�g���N�^
 *
 * �u���V�̏������A����Ƀ^�u�ʒu�A�t�H���g�^�C�v�̏����������Ă����܂��B
 */
CAttrFacilInfoPointDlg::CAttrFacilInfoPointDlg()
	: ATTR_BASE_CLASS(FACIL_INFO_POINT_MAP, IDD_ATTRFACILINFOPOINTDLG),
		m_wnd0( _T("COMBOBOX"), this, IDC_COMBO_ROADCLASS_REF ),
		m_wnd1( _T("COMBOBOX"), this, IDC_COMBO_ROADNAME_REF ), 
		m_wnd2( _T("EDIT"), this, IDC_EDIT_SEQUENCE ), 
		m_wnd3( _T("EDIT"), this, IDC_EDIT_FACILKANJI ), 
		m_wnd4( _T("EDIT"), this, IDC_EDIT_FACILKANA ), 
		m_wnd5( _T("CHECKBOX"), this, IDC_CHECK_DUMMYFLAG ), 
		m_wnd6( _T("COMBOBOX"), this, IDC_COMBO_FACILCLASS ),
		m_wnd7( _T("CHECKBOX"), this, IDC_CHECK_AUTOPOSITION_F ),
		m_wnd8( _T("EDIT"), this, IDC_EDIT_SOURCE ),
		m_wnd9( _T("CHECK"), this, IDC_CHECK_DUMMY),
		m_wnd10( _T("COMBOBOX"), this, IDC_COMBO_SEARCHFACIL )
{
	// �t�H���g�쐬
	m_hFont = CreateFont(
		18, 0, 0, 0, 100,		// �����C���C�w�p�x�C�x�p�x�C����
		FALSE, FALSE, FALSE,    // �ΆC�����C�ŏ���
		SHIFTJIS_CHARSET,       // �L�����N�^�ݒ�
		OUT_DEFAULT_PRECIS,     // �o�͐��x
		CLIP_DEFAULT_PRECIS,    // �N���b�v���x
		DEFAULT_QUALITY,        // �i��
		FIXED_PITCH | FF_DONTCARE,// �s�b�`
		AheLoadString(IDS_FONT));   // �t�H���g���
}


/** 
 * @fn HWND CAttrFacilInfoPointDlg::Create(HWND hWndParent, LPARAM dwInitParam)
 * @brief �q�_�C�A���O���쐬���܂��B
 *
 * ATTR_BASE_CLASS::Create(HWND hWndParent, LPARAM dwInitParam = NULL)�Ń_�C�A���O���쐬���܂��B
 *
 * @param hWndParent	[in]	�e�E�B���h�E�n���h��
 * @param dwInitParam	[in]	�_�C�A���O�������p�����[�^�i���ݓ��Ɏg�p���Ă��܂���j
 *
 * @retval �쐬���ꂽ�q�_�C�A���O�̃n���h����Ԃ��܂��B
 */
HWND CAttrFacilInfoPointDlg::Create(HWND hWndParent, LPARAM dwInitParam)
{ 
	m_hChildWnd = ATTR_BASE_CLASS::Create(hWndParent,dwInitParam);
	return m_hChildWnd;
}

/**
 * @fn void CAttrFacilInfoPointDlg::Delete()
 * @brief �쐬�����q�_�C�A���O���폜���܂��B
 *
 * �_�C�A���O���폜����Ƃ��ɕK���Ă�ŉ������B�Ă΂Ȃ��ƃ��������[�N���Ă��܂��܂��B
 */
void CAttrFacilInfoPointDlg::Delete()
{
	ClearFeatureDefs();
	::DestroyWindow(m_hChildWnd);
    delete this; // �K��new�Ŋm�ۂ���O��
}

/**
 * @fn void CAttrFacilInfoPointDlg::SetArcHelper( IApplication* ipApp )
 * @brief ATTR_BASE_CLASS��ArcHelper��n���܂��A�܂��AHwyHelper�ɂ�ArcHelper���Z�b�g���܂��B
 */
void CAttrFacilInfoPointDlg::SetArcHelper( IApplication* ipApp )
{ 
    ATTR_BASE_CLASS::SetArcHelper( ipApp );
	m_cHwyHelper.SetArcHelper( ipApp ); 
}

/**
 * @fn BOOL CAttrFacilInfoPointDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
 * @brief ���ݕ\�������ׂ��t�B�[�`����FeatureDef�C���f�b�N�X��n���܂��B
 * ����ɁA�t�B�[�`���̑����Ƃ͊֌W�Ȃ��R���g���[���̐�������Ă����܂��B
 * �H���R�[�h�A�H�����V�[�P���X�̐ݒ�����Ă����܂��B
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
BOOL CAttrFacilInfoPointDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
{ 
	BOOL bRet = ATTR_BASE_CLASS::SetCurrentFeatureDefIndex( lFeatureIndex, lTableIndex, lRowIndex, 0, bForce, bEditable );

	// �H���R�[�h�A�H�����V�[�P���X�̏����l�m��
	CComVariant		vaRoadSequence;
	CFieldDef*		pFieldDef;
	CComVariant		vaValue;	
 	pFieldDef = GetCurrentFeatureDef()->GetFieldDef( FACILINFO_TABLE_NAME, _T("ROAD_SEQ") );
	vaValue= pFieldDef->m_vaValue;
	m_lOrgRoadSequence = vaValue.lVal;
	pFieldDef = GetCurrentFeatureDef()->GetFieldDef( FACILINFO_TABLE_NAME, _T("ROAD_CODE") );
	vaValue = pFieldDef->m_vaValue;
	m_lOrgRoadCode = vaValue.lVal;

	// �ŏ�����H���R�[�h���o������
	SetLineClassList(m_lOrgRoadCode);

	// ���ڃt�B�[�`���Ɋ֌W�Ȃ��G�f�B�b�g�{�b�N�X(��F�Q�Ɨp�̘H�����V�[�P���X�⌴�e�ԍ�����)�Ȃǂ𐧌䂷��
	// �ҏW�J�n�O�ƕҏW�J�n��ł��ҏW�s�̂Ƃ���DISABLE�ɂ���
	if( IsEditableFeature() && m_cArcHelper.IsStartEditing() && bEditable )
	{
		// �ҏW�J�n����Ă���Ƃ��ɂ��łɁASetFocus���Ă���
		::SetFocus( GetDlgItem(IDC_COMBO_ROADCLASS_REF) );
	}

	return bRet;
}

/**
 * @fn void CAttrFacilInfoPointDlg::CreateControlRelation()
 * @brief �q�_�C�A���O�����ۂɍ쐬���܂��B
 *
 * ���ۂ̍쐬��ControlRel�ɔC���܂��B
 */
void CAttrFacilInfoPointDlg::CreateControlRelation()
{
	ATLASSERT( IsWindow() == TRUE );

	if( m_cControlRel.size() > 0 )
	{
		// �e�E�C���h�E�n���h����ݒ肵�Ȃ���
		m_cControlRel.ResetHWND( m_hWnd );
	} 
	else {
		m_cControlRel.SetHWND( m_hWnd );

		// �H����ʎQ�Ɨp
		m_cControlRel.SetControl( IDC_COMBO_ROADCLASS_REF,	TYPE_COMBO,		0,	0 );
		// �H�����̎Q�Ɨp
		m_cControlRel.SetControl( IDC_COMBO_ROADNAME_REF,	TYPE_COMBO,		0,	0 );
		// �H���R�[�h
		m_cControlRel.SetControl( IDC_EDIT_ROSENCODE,		TYPE_EDIT, 		0,	0, _T("ROAD_CODE"),	NULL, FACILINFO_TABLE_NAME , NULL, -1, FOLLOW_READONLY );
		// �H�����V�[�P���X�Q�Ɨp 
		m_cControlRel.SetControl( IDC_EDIT_SEQUENCE,		TYPE_EDIT,		0,	0, _T("ROAD_SEQ"),		NULL, FACILINFO_TABLE_NAME);
		// �{�݊�������
		m_cControlRel.SetControl( IDC_EDIT_FACILKANJI,		TYPE_EDITSTRING,0,	0, _T("NAME_KANJI"),	NULL, FACILINFO_TABLE_NAME);
		// �{�݃J�i����
		m_cControlRel.SetControl( IDC_EDIT_FACILKANA,		TYPE_EDITSTRING,0,	0, _T("NAME_YOMI"),		NULL, FACILINFO_TABLE_NAME);
		// �_�~�[������
		m_cControlRel.SetControl( IDC_CHECK_DUMMYFLAG,		TYPE_CHECKBOX,	0,	0, _T("DUMMYNAME_F"),	NULL, FACILINFO_TABLE_NAME);
		// �{�ݎ�ʃR�[�h
		m_cControlRel.SetControl( IDC_COMBO_FACILCLASS,		TYPE_COMBO,		0,	0, _T("FACILCLASS_C"),	NULL, FACILINFO_TABLE_NAME);
		// �_�~�[�{�ݎ�ʃR�[�h
		m_cControlRel.SetControl( IDC_COMBO_SEARCHFACIL,	TYPE_COMBO,		0,	0, _T("SEARCHCLASS_C"),	NULL, FACILINFO_TABLE_NAME);
		// �{�ݑ�\�|�C���g�����v�Z�t���O
		m_cControlRel.SetControl( IDC_CHECK_AUTOPOSITION_F,	TYPE_CHECKBOX,	0,	0, _T("AUTOPOSITION_F"),NULL, FACILINFO_TABLE_NAME);
		// �_�~�[�{�݃t���O
		m_cControlRel.SetControl( IDC_CHECK_DUMMY,			TYPE_CHECKBOX,	0,	0, _T("DUMMYFACIL_F"),NULL, FACILINFO_TABLE_NAME);
		// ���\�[�X
		m_cControlRel.SetControl( IDC_EDIT_SOURCE,			TYPE_EDITSTRING,0,	0, _T("SOURCE"),		NULL, FACILINFO_TABLE_NAME );
	}	
}

/**
 * @fn bool CAttrFacilInfoPointDlg::SetLineClassList(LONG nLineClass)
 * @brief �_�C�A���O���Ă΂ꂽ�Ƃ��ɘH����ʎQ�Ɨp�̃R���{�{�b�N�X�ɒl���Z�b�g����
 *
 * @param nLineClass	[in]	�H���R�[�h�̒l
 *
 * @return	�H����ʂ��擾�ł�����TRUE�A�擾�ł��Ȃ�������FALSE
 */
bool CAttrFacilInfoPointDlg::SetLineClassList(LONG nLineClass)
{
	// �H����ʃ��X�g�̃R���{�{�b�N�X
	CComboBox cbLineClass = CComboBox(GetDlgItem(IDC_COMBO_ROADCLASS_REF));
	// �A�C�e���̃N���A
	cbLineClass.ResetContent();

	// �H���R�[�h�e�[�u�����擾���Ă��Ȃ���΁A�擾����
	ITablePtr ipRoadCodeListTable = GetRoadCodeTable();
	if(!ipRoadCodeListTable)
		return false;

	// �^����ꂽ�H���R�[�h�̘H���̘H����ʃR�[�h���擾
	_ICursorPtr		ipCursor;
	IQueryFilterPtr	ipQueryFilter( CLSID_QueryFilter );
	CString strWhereClause, strSubFields;

	strWhereClause.Format(_T("ROAD_CODE = %ld"), nLineClass );
	strSubFields.Format(_T("LINECLASS_C"));
	ipQueryFilter->put_WhereClause( CComBSTR(strWhereClause) );
	ipQueryFilter->put_SubFields( CComBSTR(strSubFields) );
	ipRoadCodeListTable->Search( ipQueryFilter, VARIANT_FALSE, &ipCursor );

	_IRowPtr ipRow;

	long aLineClass = -1;
	if( ipCursor )
	{
		while (ipCursor->NextRow(&ipRow) == S_OK)
		{
			// ipCursor�Ɋi�[���ꂽRoad_Code_List�̃��R�[�h�̂����A�ŏ��̂P����LineClass_c���擾
			aLineClass = ipRow->GetValue( ipRow->Fields->_FindField(sindy::schema::road_code_list::kLineClass) );
			break;
		}
	}

	// �H����ʃR�[�h�̃R�[�h�l�h���C�����擾
	ATLASSERT(ipRoadCodeListTable);
	IFieldsPtr ipFields(ipRoadCodeListTable->Fields);
	IDomainPtr ipDomain(ipFields->GetField(ipFields->_FindField(sindy::schema::road_code_list::kLineClass))->GetDomain());
	ICodedValueDomainPtr ipCodedValueDomain(ipDomain);

	// �R���{�{�b�N�X�ɃA�C�e����ǉ�
	int nCurIdx = 0;
	for (int i = 0; i < ipCodedValueDomain->GetCodeCount(); ++i)
	{
		long aValue	= ipCodedValueDomain->GetValue(i).lVal;
		CString aName = COLE2T(ipCodedValueDomain->GetName(i));

		int nIndex = cbLineClass.AddString(aName);
		if(nIndex != CB_ERR && nIndex != CB_ERRSPACE)
		{
			cbLineClass.SetItemData(nIndex, aValue);
			if(aValue == aLineClass)
				nCurIdx = nIndex;
		}
		else
		{
			AttrMessageBox( AheLoadString( IDS_FAILTOSETROUTECLASS ), AheLoadString( IDS_CAPTION ), MB_OK, NULL);
			return false;
		}
	}

	// �J�[�\���̐ݒ�
	cbLineClass.SetCurSel(nCurIdx);
	// ���݂̘H����ʂ̎擾
	m_nLineClass = cbLineClass.GetItemData(nCurIdx);

	CFieldDef* pFieldDef;
	CComVariant		vaRoadCode;	
	pFieldDef = GetCurrentFeatureDef()->GetFieldDef( FACILINFO_TABLE_NAME, _T("ROAD_CODE") );
	vaRoadCode = pFieldDef->m_vaValue;

	RedrawRoadNameList(vaRoadCode.lVal);
	return true;

}

/**
 * @fn bool CAttrFacilInfoPointDlg::GetLineClass(int* pClass)
 * @brief �ݒ肳��Ă���H����ʂ��擾����
 *
 * @param pClass	[out]	�H���R�[�h�̒l
 *
 * @return	�H����ʂ��擾�ł�����TRUE�A�擾�ł��Ȃ�������FALSE
 */
bool CAttrFacilInfoPointDlg::GetLineClass(int* pClass)
{
	if( !pClass )
		return false;
	// �R���{�{�b�N�X�̎擾
	CComboBox cbLineClass = CComboBox(GetDlgItem(IDC_COMBO_ROADCLASS_REF));
	// �C���f�b�N�X�̎擾
	int nIndex = cbLineClass.GetCurSel();
	if(nIndex == CB_ERR)
		return false;
	// �l�̎擾
	*pClass = cbLineClass.GetItemData(nIndex);
	return true;
}

/**
 * @fn bool CAttrFacilInfoPointDlg::SetLineClass(int nClass)
 * @brief �H����ʂ̒l����R���{�{�b�N�X��ݒ�
 *
 * @param nClass	[in]	�H���R�[�h�̒l
 *
 * @return	�H����ʂ��擾�ł�����TRUE�A�擾�ł��Ȃ�������FALSE
 */
bool CAttrFacilInfoPointDlg::SetLineClass(int nClass)
{
	// �R���{�{�b�N�X�̎擾
	CComboBox cbLineClass = CComboBox(GetDlgItem(IDC_COMBO_ROADCLASS_REF));
	// �C���f�b�N�X�̌v�Z
	int i = 0;
	int nMaxCount = cbLineClass.GetCount();
	for( ; i < nMaxCount; ++i)
	{
		int nLineClass = cbLineClass.GetItemData(i);
		if(nLineClass == nClass)
			break;
	}
	if(i == nMaxCount)
		return false;
	// �R���{�{�b�N�X�̐ݒ�
	cbLineClass.SetCurSel(i);

	return true;
}

/**
 * @fn bool CAttrFacilInfoPointDlg::RedrawRoadNameList(LONG nRoadCode)
 * @brief �H�����̃R���{�{�b�N�X�̍X�V
 *
 * @param nRoadCode	[in]	�H���R�[�h�̒l
 *
 * @return	�H�����̂��擾�ł�����TRUE�A�擾�ł��Ȃ�������FALSE
 */
bool CAttrFacilInfoPointDlg::RedrawRoadNameList(LONG nRoadCode)
{
	// �H�����̃R���{�{�b�N�X
	CComboBox cbRoadName = CComboBox(GetDlgItem(IDC_COMBO_ROADNAME_REF));
	// �R���{�{�b�N�X�̏��̃N���A
	cbRoadName.ResetContent();
	// �J�i���̂Ń}�b�v�̍쐬
	std::map<CString, int> cYomiMap;

	// �H���R�[�h���̍ēǂݍ���
	if(!GetRoadCodeSet(&m_cRoadCodeSet))
	{
		AttrMessageBox( AheLoadString( IDS_FAILTOGETROUTECODE ), AheLoadString( IDS_CAPTION ), MB_OK, NULL );
		return 0;
	}
	for(CHetRoadCodeSet::const_iterator aIter = m_cRoadCodeSet.begin(); aIter != m_cRoadCodeSet.end(); ++aIter)
	{
		if(m_nLineClass == 0 || aIter->second.m_nLineClass == m_nLineClass)
			cYomiMap.insert(std::map<CString, int>::value_type(aIter->second.m_strNameYomi, aIter->second.m_nRoadCode));
	}

	// ���X�g�{�b�N�X�ɃA�C�e���̒ǉ�
	int nCurIdx = 0;
	for(std::map<CString, int>::const_iterator aIter2 = cYomiMap.begin(); aIter2 != cYomiMap.end(); ++aIter2)
	{
		const CHetRoadCode* pRoadCode = m_cRoadCodeSet.FindRoadCode(aIter2->second);
		if(pRoadCode)
		{
			int nIndex = cbRoadName.AddString(pRoadCode->m_strNameKanji);
			if(nIndex != CB_ERR && nIndex != CB_ERRSPACE)
			{
				cbRoadName.SetItemData(nIndex, pRoadCode->m_nRoadCode);
				if(pRoadCode->m_nRoadCode == nRoadCode)
					nCurIdx = nIndex;
			}
			else
			{
				AttrMessageBox( AheLoadString( IDS_FAILTOSETROUTENAME ), AheLoadString( IDS_CAPTION ), MB_OK, NULL );
			}
		}
	}
	// ���݂̃J�[�\���̐ݒ�
	cbRoadName.SetCurSel(nCurIdx);
	// ���݂̘H���R�[�h�̎擾
	m_nRoadCode = cbRoadName.GetItemData(nCurIdx);

	return true;
}

/**
 * @fn bool CAttrFacilInfoPointDlg::GetRoadCode(int* pRoadCode)
 * @brief �H���R�[�h�G�f�B�b�g�{�b�N�X����H���R�[�h�̎擾
 *
 * @param nRoadCode	[out]	�H���R�[�h�̒l
 *
 * @return	�H���R�[�h���擾�ł�����TRUE�A�擾�ł��Ȃ�������FALSE
 */
bool CAttrFacilInfoPointDlg::GetRoadCode(int* pRoadCode)
{
	// �G�f�B�b�g�{�b�N�X�̎擾
	CEdit edRoadCode = CEditT<CWindow>(GetDlgItem(IDC_EDIT_ROSENCODE));
	// �l�̎擾
	TCHAR lpszRoadCode[32];
	if(edRoadCode.GetLine(0, lpszRoadCode, 32) == 0)
		return false;
	// ���l�ɕϊ�
	if(! CHwyHelper::StrToRoadCode(pRoadCode, lpszRoadCode))
		return false;

	return true;
}

/**
 * @fn bool CAttrFacilInfoPointDlg::GetRoadCode(int* pRoadCode)
 * @brief �H���R�[�h�G�f�B�b�g�{�b�N�X����H���R�[�h�̎擾
 *
 * @param nRoadCode	[out]	�H���R�[�h�̒l
 *
 * @return	�H���R�[�h���擾�ł�����TRUE�A�擾�ł��Ȃ�������FALSE
 */
bool CAttrFacilInfoPointDlg::SetRoadCode(int nRoadCode)
{
	// ������ϊ�
	TCHAR lpszRoadCode[8];
	if(! CHwyHelper::RoadCodeToStr(nRoadCode, lpszRoadCode))
		return true;
	// �G�f�B�b�g�{�b�N�X�̎擾
	CEdit edRoadCode = CEditT<CWindow>(GetDlgItem(IDC_EDIT_ROSENCODE));
	// �l�̐ݒ�
	edRoadCode.SetSel(0, -1);
	edRoadCode.ReplaceSel(lpszRoadCode);

	CFieldDef* pFieldDef;
	pFieldDef = GetCurrentFeatureDef()->GetFieldDef( FACILINFO_TABLE_NAME, _T("ROAD_CODE") );
	pFieldDef->m_vaValue = nRoadCode;
	if( nRoadCode != m_lOrgRoadCode)
	{
		pFieldDef->m_bChanged = TRUE;	
		SetButton( Changed() );
	}
	return true;
}


/**
 * @fn BOOL CAttrFacilInfoPointDlg::ErrorCheck()
 * @brief �G���[���Ȃ����ǂ������ׂ�
 *
 * �_�C�A���O�����ۂɁAAttributeDlgObj����Ă΂��
 * �{�ݏ�񖼏̂Ǝ{�ݏ��J�i���̂������Ƒ΂ɂȂ��ė��������Ă��邩�A�܂��͓����Ă��Ȃ����������߂�
 *
 * @return	�G���[���Ȃ�������TRUE�A�G���[�������FALSE
 */
BOOL CAttrFacilInfoPointDlg::ErrorCheck()
{
	for( std::list<CFeatureDef>::iterator it = m_pFeatureDefList->begin(); it != m_pFeatureDefList->end(); it++ )
	{
		CString strTableName = it->GetTableName();

		if( strTableName == FACILINFO_TABLE_NAME )
		{
			// ���ݕҏW���̒l���K��
			CFieldDef* pFieldDef;
			CComVariant vaName_Kanji, vaName_Yomi;
			pFieldDef = it->GetFieldDef( FACILINFO_TABLE_NAME, _T("NAME_KANJI") );
			vaName_Kanji = pFieldDef->m_vaValue;
			pFieldDef = it->GetFieldDef( FACILINFO_TABLE_NAME, _T("NAME_YOMI") );
			vaName_Yomi = pFieldDef->m_vaValue;

			CString strMessage;
			LONG lOID;
			it->GetFeature()->get_OID( &lOID );

			if( vaName_Kanji.vt != VT_NULL && vaName_Yomi.vt == VT_NULL )
			{
				strMessage.Format( AheLoadString( IDS_INVALIDFACILKANA ),lOID);
				AttrMessageBox( strMessage, AheLoadString( IDS_CAPTION ), MB_OK, NULL );
				return FALSE;
			}
			else if( vaName_Kanji.vt == VT_NULL && vaName_Yomi.vt != VT_NULL )
			{
				strMessage.Format(AheLoadString( IDS_INVALIDFACILKANJI ),lOID);
				AttrMessageBox( strMessage, AheLoadString( IDS_CAPTION ), MB_OK, NULL );
				return FALSE;
			}
		}
	}

	return TRUE;
}


/**
* @brief	�H���R�[�h�e�[�u���擾
*/
ITablePtr CAttrFacilInfoPointDlg::GetRoadCodeTable()
{
	ITablePtr ipRoadCodeListTable;
	IWorkspacePtr ipWorkspace;
	ITablePtr ipCurrentTable;
	IFeaturePtr ipCurrentFeature;
	ipCurrentFeature = GetCurrentFeatureDef()->GetFeature();
	if( !ipCurrentFeature )
		return NULL;
	ipCurrentFeature->get_Table( &ipCurrentTable );
	IDatasetPtr ipDataset( ipCurrentTable );
	if( !ipDataset )
		return NULL;
	ipDataset->get_Workspace( &ipWorkspace );
	if( !ipWorkspace )
		return NULL;
	
	// REFERENCE.ROAD_CODE_LIST�e�[�u���擾
	CString strOwnerName = AheGetFeatureClassOwnerName( ipCurrentFeature );
	if( strOwnerName.IsEmpty() )
		strOwnerName = _T("ROAD_CODE_LIST");
	else
		strOwnerName += _T(".ROAD_CODE_LIST");

	return AheOpenTableByTableName( ipWorkspace, strOwnerName );
}

/**
* @brief	�H���R�[�h�擾
*/
bool CAttrFacilInfoPointDlg::GetRoadCodeSet(CHetRoadCodeSet* pCodeSet)
{
	// �e�[�u���̎擾
	ITablePtr ipRoadCodeListTable = GetRoadCodeTable();
	if(!ipRoadCodeListTable)
		return false;
	// �s�擾�̃p�����[�^�̐ݒ�
	IQueryFilterPtr ipQFilter(CLSID_QueryFilter);
	// �s�̎擾
	_ICursorPtr ipCursor;
	HRESULT hr = ipRoadCodeListTable->Search(ipQFilter, VARIANT_FALSE, &ipCursor);
	if(FAILED(ipRoadCodeListTable->Search(ipQFilter, VARIANT_FALSE, &ipCursor))){
		::MessageBox(m_hWnd, AheLoadString( IDS_FAILTOGETROW ), AheLoadString( IDS_CAPTION ), MB_OK);
		return false;
	}
	// �e�s������̎擾
	_IRowPtr ipRow;
	while(ipCursor->NextRow(&ipRow) == S_OK){
		CHetRoadCode cRoadCode(ipRow);
		pCodeSet->Append(cRoadCode);
	}
	return true;
}
