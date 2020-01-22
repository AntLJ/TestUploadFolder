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

// AttrDistancePostPointDlg.cpp: CAttrDistancePostPointDlg �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WinLib/libwinlib.h"
#include "crd_cnv/coord_converter.h"
#include "AttrDlgLib.h"
#include "sindy/libschema.h"
#include "AttrDistancePostPointDlg.h"

/// �E�B���h�E�}�l�[�W���z�u�}�b�v�i�����E�L�����H�����֌W�|�C���g�p�j
BEGIN_WINDOW_MAP(DISTANCE_POST_POINT_MAP)
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
				RCTOFIT(IDC_STATIC_DIRECTIONCODE)
				RCTOFIT(-1)
				RCREST(IDC_COMBO_DIRECTIONCODE)
			ENDGROUP()
			RCSPACE(5)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_KILOPOST)
				RCTOFIT(-1)
				RCFIXED(IDC_EDIT_KILOPOST2,20)
				RCTOFIT(IDC_EDIT_KILOPOST)
				RCTOFIT(IDC_STATIC_KILOPOST2)
			ENDGROUP()
			RCSPACE(5)
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
 * @retval CAttrDistancePostPointDlg��Ԃ��܂��B
 */
extern "C" __declspec(dllexport) CExportDlg * __stdcall CreateDlg()
{
    return new CAttrDistancePostPointDlg();
}

/////////////////////////////////////////////////////////////////////////////
//
// CAttrDistancePostPointDlg �����o�֐�
//
/////////////////////////////////////////////////////////////////////////////

/** 
 * @fn CAttrDistancePostPointDlg::CAttrDistancePostPointDlg()
 * @brief �R���X�g���N�^
 *
 * �u���V�̏������A����Ƀ^�u�ʒu�A�t�H���g�^�C�v�̏����������Ă����܂��B
 */
CAttrDistancePostPointDlg::CAttrDistancePostPointDlg()
	: ATTR_BASE_CLASS(DISTANCE_POST_POINT_MAP, IDD_ATTRDISTANCEPOSTPOINTDLG),
		m_wnd0( _T("COMBOBOX"), this, IDC_COMBO_ROADCLASS_REF ),
		m_wnd1( _T("COMBOBOX"), this, IDC_COMBO_ROADNAME_REF ), 
		m_wnd2( _T("COMBOBOX"), this, IDC_COMBO_DIRECTIONCODE ),
		m_wnd3( _T("EDIT"), this, IDC_EDIT_KILOPOST ), 
		m_wnd4( _T("EDIT"), this, IDC_EDIT_SOURCE ),
		m_wnd5( _T("EDIT"), this, IDC_EDIT_KILOPOST2 ), 
		m_lOrgRoadCode(-1),
		m_lOrgDirCode(-1),
		m_nLineClass(-1),
		m_nRoadCode(-1),
		m_nDirCode(-1),
		m_hChildWnd(NULL)
{
}

// �q�_�C�A���O���쐬���܂��B
HWND CAttrDistancePostPointDlg::Create(HWND hWndParent, LPARAM dwInitParam)
{ 
	m_hChildWnd = ATTR_BASE_CLASS::Create(hWndParent,dwInitParam);
	return m_hChildWnd;
}

// �쐬�����q�_�C�A���O���폜���܂��B
void CAttrDistancePostPointDlg::Delete()
{
	ClearFeatureDefs();
	::DestroyWindow(m_hChildWnd);
    delete this; // �K��new�Ŋm�ۂ���O��
}

// ATTR_BASE_CLASS��ArcHelper��n���܂��A�܂��AHwyHelper�ɂ�ArcHelper���Z�b�g���܂��B
void CAttrDistancePostPointDlg::SetArcHelper( IApplication* ipApp )
{ 
    ATTR_BASE_CLASS::SetArcHelper( ipApp );
}

// ���ݕ\�������ׂ��t�B�[�`����FeatureDef�C���f�b�N�X��n���܂�
BOOL CAttrDistancePostPointDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
{ 
	BOOL bRet = ATTR_BASE_CLASS::SetCurrentFeatureDefIndex( lFeatureIndex, lTableIndex, lRowIndex, 0, bForce, bEditable );

	// �H���R�[�h�̏����l�m��
	CFieldDef* pFieldDef = GetCurrentFeatureDef()->GetFieldDef( sindy::schema::distance_post_point::kTableName, sindy::schema::category::highway_road_relational_table::kRoadCode );
	CComVariant vaValue = pFieldDef->m_vaValue;
	m_lOrgRoadCode = vaValue.lVal;

	// �����R�[�h�̏����l�m��
	CFieldDef* pFieldDef2 = GetCurrentFeatureDef()->GetFieldDef( sindy::schema::distance_post_point::kTableName, sindy::schema::category::highway_direction_table::kDirection );
	CComVariant vaValueDir = pFieldDef2->m_vaValue;
	m_lOrgDirCode = vaValueDir.lVal;

	// �ŏ�����H���R�[�h���o������
	SetLineClassList(m_lOrgRoadCode);
	SetDirList(m_lOrgDirCode);

	// �H���R�[�h�̐ݒ�
	if(! SetRoadCode(m_nRoadCode))
		return 0;

	CFieldDef* pFieldDef3 = GetCurrentFeatureDef()->GetFieldDef( sindy::schema::distance_post_point::kTableName, sindy::schema::distance_post_point::kDistance );
	CComVariant vaDistance = pFieldDef3->m_vaValue;
	CEdit edtDistance = CEdit(GetDlgItem(IDC_EDIT_KILOPOST));

	CString strTmp;
	strTmp.Format( _T("%.1f"), vaDistance.fltVal );
	edtDistance.SetWindowText(strTmp);

	// ���ڃt�B�[�`���Ɋ֌W�Ȃ��G�f�B�b�g�{�b�N�X(��F�Q�Ɨp�̘H�����V�[�P���X�⌴�e�ԍ�����)�Ȃǂ𐧌䂷��
	// �ҏW�J�n�O�ƕҏW�J�n��ł��ҏW�s�̂Ƃ���DISABLE�ɂ���
	if( IsEditableFeature() && m_cArcHelper.IsStartEditing() && bEditable )
	{
		// �ҏW�J�n����Ă���Ƃ��ɂ��łɁASetFocus���Ă���
		::SetFocus( GetDlgItem(IDC_COMBO_ROADCLASS_REF) );
	}

	return bRet;
}

// �G���[���Ȃ����ǂ������ׂ�
BOOL CAttrDistancePostPointDlg::ErrorCheck()
{
	// �R���g���[���̓��e�`�F�b�N
	for ( const auto& ctl : m_cControlRel )
	{
		if ( IDC_EDIT_KILOPOST == ctl.m_nID )
		{
			CString strVal;
			::GetWindowText( ::GetDlgItem(m_hWnd, ctl.m_nID), strVal.GetBuffer(1026), 1026);
			strVal.ReleaseBuffer();

			CString strRet = strVal.SpanIncluding(_T("0123456789."));
			if(strVal != strRet)
			{
				CString strMessage;
				LONG lOID = 0;
				ctl.m_pFeatureDef->GetFeature()->get_OID( &lOID );
				strMessage.Format( AheLoadString( IDS_ERROR_DISTANCE2 ), lOID );
				AttrMessageBox( strMessage, AheLoadString( IDS_CAPTION ), MB_OK, NULL );
				return FALSE;
			}
		}
	}

	// �i�[����Ă�����e���`�F�b�N
	for( std::list<CFeatureDef>::iterator it = m_pFeatureDefList->begin(); it != m_pFeatureDefList->end(); it++ )
	{
		CString strTableName = it->GetTableName();

		if( strTableName == sindy::schema::distance_post_point::kTableName )
		{
			// ���ݕҏW���̒l���K��
			CFieldDef* pFieldDef;
			CComVariant vaDistance;
			pFieldDef = it->GetFieldDef( sindy::schema::distance_post_point::kTableName, sindy::schema::distance_post_point::kDistance );
			vaDistance = pFieldDef->m_vaValue;

			CString strMessage;
			LONG lOID = 0;
			it->GetFeature()->get_OID( &lOID );

			if( vaDistance.vt == VT_NULL )
			{
				strMessage.Format( AheLoadString( IDS_ERROR_DISTANCE ) ,lOID);
				AttrMessageBox( strMessage, AheLoadString( IDS_CAPTION ), MB_OK, NULL );
				return FALSE;
			}
			else
			{
				CString strTmp;
				strTmp.Format( _T("%.1f"), vaDistance.fltVal );
				pFieldDef->m_vaValue = CComVariant( (float)_tstof(strTmp) );
			}
		}
	}

	return TRUE;
}

// �q�_�C�A���O�����ۂɍ쐬���܂��B
void CAttrDistancePostPointDlg::CreateControlRelation()
{
	ATLASSERT( IsWindow() == TRUE );

	if( m_cControlRel.size() > 0 )
	{
		// �e�E�C���h�E�n���h����ݒ肵�Ȃ���
		m_cControlRel.ResetHWND( m_hWnd );
	} 
	else
	{
		m_cControlRel.SetHWND( m_hWnd );

		// �H����ʎQ�Ɨp
		m_cControlRel.SetControl( IDC_COMBO_ROADCLASS_REF, TYPE_COMBO, 0, 0 );
		// �H�����̎Q�Ɨp
		m_cControlRel.SetControl( IDC_COMBO_ROADNAME_REF, TYPE_COMBO, 0, 0 );
		// �H���R�[�h
		m_cControlRel.SetControl( IDC_EDIT_ROSENCODE, TYPE_EDIT, 0,	0, sindy::schema::category::highway_road_relational_table::kRoadCode, NULL, sindy::schema::distance_post_point::kTableName, NULL, -1, FOLLOW_READONLY );
		// �����R�[�h
		m_cControlRel.SetControl( IDC_COMBO_DIRECTIONCODE, TYPE_COMBO, 0, 0, sindy::schema::category::highway_direction_table::kDirection, NULL, sindy::schema::distance_post_point::kTableName );
		// �����W�l
		m_cControlRel.SetControl( IDC_EDIT_KILOPOST, TYPE_EDITFLOAT, 0, 0, sindy::schema::distance_post_point::kDistance, NULL, sindy::schema::distance_post_point::kTableName );
		// ���\�[�X
		m_cControlRel.SetControl( IDC_EDIT_SOURCE, TYPE_EDITSTRING, 0, 0, sindy::schema::ipc_table::kSource, NULL, sindy::schema::distance_post_point::kTableName );
		// �����W�l(������)
		m_cControlRel.SetControl( IDC_EDIT_KILOPOST2, TYPE_EDITSTRING, 0, 0, sindy::schema::distance_post_point::kDistanceMark, NULL, sindy::schema::distance_post_point::kTableName );
	}
}

// �_�C�A���O���Ă΂ꂽ�Ƃ��ɘH����ʎQ�Ɨp�̃R���{�{�b�N�X�ɒl���Z�b�g����
bool CAttrDistancePostPointDlg::SetLineClassList(int nLineClass)
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
	IQueryFilterPtr	ipQueryFilter( CLSID_QueryFilter );
	CString strWhereClause, strSubFields;

	strWhereClause.Format(_T("%s = %ld"), sindy::schema::category::highway_road_relational_table::kRoadCode, nLineClass );
	strSubFields.Format(_T("LINECLASS_C"));
	ipQueryFilter->put_WhereClause( CComBSTR(strWhereClause) );
	ipQueryFilter->put_SubFields( CComBSTR(strSubFields) );

	_ICursorPtr ipCursor;
	ipRoadCodeListTable->Search( ipQueryFilter, VARIANT_TRUE, &ipCursor );

	long aLineClass = -1;
	if( ipCursor )
	{
		_IRowPtr ipRow;
		while (ipCursor->NextRow(&ipRow) == S_OK)
		{
			// ipCursor�Ɋi�[���ꂽRoad_Code_List�̃��R�[�h�̂����A�ŏ��̂P����LineClass_c���擾
			aLineClass = ipRow->GetValue( ipRow->Fields->_FindField(sindy::schema::road_code_list::kLineClass) );
			break;
		}
	}
	// TODO:aLineClass�̒l���m�F���鏈����ǉ�����

	// �H����ʃR�[�h�̃R�[�h�l�h���C�����擾
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

	CFieldDef* pFieldDef = GetCurrentFeatureDef()->GetFieldDef( sindy::schema::distance_post_point::kTableName, sindy::schema::category::highway_road_relational_table::kRoadCode );
	CComVariant vaRoadCode = pFieldDef->m_vaValue;

	RedrawRoadNameList(vaRoadCode.lVal);
	return true;
}

// �ݒ肳��Ă���H����ʂ��擾����
bool CAttrDistancePostPointDlg::GetLineClass(int* pClass)
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

// �H����ʂ̒l����R���{�{�b�N�X��ݒ�
bool CAttrDistancePostPointDlg::SetLineClass(int nClass)
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

// �_�C�A���O���Ă΂ꂽ�Ƃ��ɕ����R�[�h�p�̃R���{�{�b�N�X�ɒl���Z�b�g����
void CAttrDistancePostPointDlg::SetDirList(int nDir)
{
	// �H����ʃ��X�g�̃R���{�{�b�N�X
	CComboBox cbDir = CComboBox(GetDlgItem(IDC_COMBO_DIRECTIONCODE));
	// �A�C�e���̃N���A
	cbDir.ResetContent();

	// �����R�[�h�̃R�[�h�l�h���C�����擾
	IFeaturePtr ipCurrentFeature = GetCurrentFeatureDef()->GetFeature();
	if( !ipCurrentFeature )
		return;

	IFieldsPtr ipFields;
	ipCurrentFeature->get_Fields(&ipFields);

	IDomainPtr ipDomain(ipFields->GetField(ipFields->_FindField(sindy::schema::highway_node::kDirection))->GetDomain());
	ICodedValueDomainPtr ipCodedValueDomain(ipDomain);

	// �R���{�{�b�N�X�ɃA�C�e����ǉ�
	int nCurIdx = 0;
	for (int i = 0; i < ipCodedValueDomain->GetCodeCount(); ++i)
	{
		long nValue	= ipCodedValueDomain->GetValue(i).lVal;
		CString sName = COLE2T(ipCodedValueDomain->GetName(i));

		int nIndex = cbDir.AddString(sName);
		if(nIndex != CB_ERR && nIndex != CB_ERRSPACE)
		{
			cbDir.SetItemData(nIndex, nValue);
			if(nValue == nDir)
				nCurIdx = nIndex;
		}
		else
		{
			AttrMessageBox( AheLoadString( IDS_FAILTOSETROUTECLASS ), AheLoadString( IDS_CAPTION ), MB_OK, NULL);
			return;
		}
	}

	// �J�[�\���̐ݒ�
	cbDir.SetCurSel(nCurIdx);
	// ���݂̘H����ʂ̎擾
	m_nDirCode = cbDir.GetItemData(nCurIdx);
}

// �H�����̃R���{�{�b�N�X�̍X�V
bool CAttrDistancePostPointDlg::RedrawRoadNameList(int nRoadCode)
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

	for(const auto& aIter : m_cRoadCodeSet)
	{
		if(m_nLineClass == 0 || aIter.second.m_nLineClass == m_nLineClass)
			cYomiMap.insert(std::map<CString, int>::value_type(aIter.second.m_strNameYomi, aIter.second.m_nRoadCode));
	}

	// ���X�g�{�b�N�X�ɃA�C�e���̒ǉ�
	int nCurIdx = 0;
	for(const auto& aIter2 : cYomiMap)
	{
		const CHetRoadCode* pRoadCode = m_cRoadCodeSet.FindRoadCode(aIter2.second);
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
				// TODO: ������return���Ȃ��đ��v���m�F
			}
		}
	}

	// ���݂̃J�[�\���̐ݒ�
	cbRoadName.SetCurSel(nCurIdx);
	// ���݂̘H���R�[�h�̎擾
	m_nRoadCode = cbRoadName.GetItemData(nCurIdx);

	return true;
}

// �H���R�[�h�G�f�B�b�g�{�b�N�X����H���R�[�h�̎擾
bool CAttrDistancePostPointDlg::GetRoadCode(int* pRoadCode)
{
	// �G�f�B�b�g�{�b�N�X�̎擾
	CEdit edRoadCode = CEditT<CWindow>(GetDlgItem(IDC_EDIT_ROSENCODE));
	// �l�̎擾
	TCHAR lpszRoadCode[32] = {};
	if(edRoadCode.GetLine(0, lpszRoadCode, 32) == 0)
		return false;
	// ���l�ɕϊ�
	if(! CHwyHelper::StrToRoadCode(pRoadCode, lpszRoadCode))
		return false;

	return true;
}

// �H���R�[�h�G�f�B�b�g�{�b�N�X����H���R�[�h�̎擾
bool CAttrDistancePostPointDlg::SetRoadCode(int nRoadCode)
{
	// ������ϊ�
	TCHAR lpszRoadCode[8] = {};
	if(! CHwyHelper::RoadCodeToStr(nRoadCode, lpszRoadCode))
		return true;
	// �G�f�B�b�g�{�b�N�X�̎擾
	CEdit edRoadCode = CEditT<CWindow>(GetDlgItem(IDC_EDIT_ROSENCODE));
	// �l�̐ݒ�
	edRoadCode.SetSel(0, -1);
	edRoadCode.ReplaceSel(lpszRoadCode);

	CFieldDef* pFieldDef = GetCurrentFeatureDef()->GetFieldDef( sindy::schema::distance_post_point::kTableName, sindy::schema::category::highway_road_relational_table::kRoadCode );
	pFieldDef->m_vaValue = nRoadCode;
	if(nRoadCode != m_lOrgRoadCode)
	{
		pFieldDef->m_bChanged = TRUE;	
		SetButton( Changed() );
	}
	return true;
}

// �H���R�[�h�e�[�u���擾
ITablePtr CAttrDistancePostPointDlg::GetRoadCodeTable()
{
	IFeaturePtr ipCurrentFeature = GetCurrentFeatureDef()->GetFeature();
	if( !ipCurrentFeature )
		return NULL;

	ITablePtr ipCurrentTable;
	ipCurrentFeature->get_Table( &ipCurrentTable );

	IDatasetPtr ipDataset( ipCurrentTable );
	if( !ipDataset )
		return NULL;

	IWorkspacePtr ipWorkspace;
	ipDataset->get_Workspace( &ipWorkspace );
	if( !ipWorkspace )
		return NULL;
	
	// REFERENCE.ROAD_CODE_LIST�e�[�u���擾
	// TODO: AheOpenSameOwnerTable()���g���悤�ɂ���
	CString strOwnerName = AheGetFeatureClassOwnerName( ipCurrentFeature );
	if( strOwnerName.IsEmpty() )
		strOwnerName = _T("ROAD_CODE_LIST");
	else
		strOwnerName += _T(".ROAD_CODE_LIST");

	return AheOpenTableByTableName( ipWorkspace, strOwnerName );
}

// �H���R�[�h�擾
bool CAttrDistancePostPointDlg::GetRoadCodeSet(CHetRoadCodeSet* pCodeSet)
{
	// �e�[�u���̎擾
	ITablePtr ipRoadCodeListTable = GetRoadCodeTable();
	if(!ipRoadCodeListTable)
		return false;

	// �s�擾�̃p�����[�^�̐ݒ�
	IQueryFilterPtr ipQFilter(CLSID_QueryFilter);

	// �s�̎擾
	_ICursorPtr ipCursor;
	HRESULT hr = ipRoadCodeListTable->Search(ipQFilter, VARIANT_TRUE, &ipCursor);
	if(FAILED(ipRoadCodeListTable->Search(ipQFilter, VARIANT_TRUE, &ipCursor))){
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
