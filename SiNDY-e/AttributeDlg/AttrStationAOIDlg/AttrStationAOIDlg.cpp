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

// AttrStationAOIDlg.cpp : DLL �A�v���P�[�V�����p�ɃG�N�X�|�[�g�����֐����`���܂��B
//

#include "stdafx.h"
#include "AttrStationAOIDlg.h"
#include <ArcHelperEx/Map.h>
#include <boost/assign/list_of.hpp>

using namespace sindy::schema;

/// ���X�g�r���[��ɕ\������郆�j�[�N�R�[�h�̃J������
static const _TCHAR UNIQUE_CODE[] = _T("JOIN_LINE_INFO_OID");

/// ���X�g�r���[��ɕ\�������J������
static const _TCHAR* COLUMNS[] = {
	UNIQUE_CODE,
	_T("�w��"),
	_T("�H����"),
	_T("��Ж�")
};

// �E�B���h�E�}�l�[�W���z�u�}�b�v
BEGIN_WINDOW_MAP(STATIONAOI_MAP)
	BEGINROWS(WRCT_TOFIT, 0, RCMARGINS(4,4))
		BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(0,0))
			RCTOFIT(IDC_RELATION_POI_TEXT)
		ENDGROUP()
		RCSPACE(5)
		BEGINCOLS(WRCT_PCT, 0, 30)
			RCREST(IDC_RELATIONLIST)
		ENDGROUP()
		RCSPACE(5)
		BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(0,0))
			RCTOFIT(IDC_SEARCH_RAILWAY_TEXT)
			RCTOFIT(IDC_SEARCH_AREA_TEXT)
			RCPERCENT(IDC_METER_BOX,30)
			RCTOFIT(IDC_LESSTHAN_METER_TEXT)
			RCTOFIT(IDC_BUTTON_SEARCH_POI)
		ENDGROUP()
		RCSPACE(5)
		BEGINCOLS(WRCT_PCT, 0, 40)
			RCREST(IDC_SEARCHLIST)
		ENDGROUP()
		RCSPACE(5)
		BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(0,0))
			RCTOFIT(IDC_BUTTON_APPLY)
		ENDGROUP()
		RCSPACE(5)
	ENDGROUP()
END_WINDOW_MAP()

/**
* @brief TOC����w�背�C�����擾
* @param layerName [in]     TOC����擾���郌�C����
* @param stCount   [in,out] TOC��ɑ��݂��郌�C�����̐����擾
* @param ipMap     [in]     IMapPtr
*/
ILayerPtr getLayer( const CString& layerName, int& stCount, const IMapPtr& ipMap )
{
	// TOC����layerName���擾
	CMap cMap( ipMap );
	// TOC���layerName�̐����J�E���g
	stCount = 0;
	ILayerPtr ipLayer;
	for( auto& it : cMap )
	{
		if( AheGetFeatureClassName( it ).Compare( layerName ) == 0 )
		{
			ipLayer = it;
			++stCount;
		}
	}
	return ipLayer;
}

/**
 * �N���X�̖����I�����N���s�����߂ɁA���̃N���X��CExportDlg�N���X���p�����A�����_�C�A���O����CExportDlg���쐬���邱�Ƃɂ��
 * �����_�C�A���O������t���ɂ����邱�Ƃ��ł���N���X�{�̂��쐬����܂��B
 * �ڂ�����http://ptvi.hp.infoseek.co.jp/vc/p_0000010.html���Q�Ƃ̂���
 *
 * @retval CAttrStationAOIDlg��Ԃ��܂��B
 */
extern "C" __declspec(dllexport) CExportDlg * __stdcall CreateDlg()
{
	return new CAttrStationAOIDlg();
}

/////////////////////////////////////////////////////////////////////////////
//
// CAttrStationAOIDlg �����o�֐�
//
/////////////////////////////////////////////////////////////////////////////

/**
 * @brief �R���X�g���N�^
 *
 * �u���V�̏����������Ă����܂��B
 */
CAttrStationAOIDlg::CAttrStationAOIDlg()
	: ATTR_BASE_CLASS(STATIONAOI_MAP, IDD_STATIONAOIDLG),
	  m_IsCheckedChangeFromProg(false)
{
	// �e�R���g���[�����T�u�N���X�p�ɏ���
	m_mapSubClass = boost::assign::map_list_of
		(IDC_METER_BOX, CContainedWindow(  _T("EDIT"), this, IDC_METER_BOX ) )
		(IDC_BUTTON_APPLY, CContainedWindow(  _T("BUTTON"), this, IDC_BUTTON_APPLY ) )
		(IDC_BUTTON_SEARCH_POI, CContainedWindow(  _T("BUTTON"), this, IDC_BUTTON_SEARCH_POI ) );
}

/**
 * @brief �q�_�C�A���O���쐬���܂��B
 *
 * ATTR_BASE_CLASS::Create(HWND hWndParent, LPARAM dwInitParam = NULL)�Ń_�C�A���O���쐬���܂��B
 *
 * @param hWndParent	[in]	�e�E�B���h�E�n���h��
 * @param dwInitParam	[in]	�_�C�A���O�������p�����[�^�i���ݓ��Ɏg�p���Ă��܂���j
 *
 * @retval �쐬���ꂽ�q�_�C�A���O�̃n���h����Ԃ��܂��B
 */
HWND CAttrStationAOIDlg::Create(HWND hWndParent, LPARAM dwInitParam)
{
	return ATTR_BASE_CLASS::Create(hWndParent,dwInitParam);
}

/**
 * @brief �쐬�����q�_�C�A���O���폜���܂��B
 *
 * �_�C�A���O���폜����Ƃ��ɕK���Ă�ŉ������B�Ă΂Ȃ��ƃ��������[�N���Ă��܂��܂��B
 */
void CAttrStationAOIDlg::Delete()
{
	ClearFeatureDefs();
	if( IsWindow() )
		DestroyWindow();
	delete this; // �K��new�Ŋm�ۂ���O��
}

/**
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
BOOL CAttrStationAOIDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
{
	BOOL bRet = ATTR_BASE_CLASS::SetCurrentFeatureDefIndex( lFeatureIndex, lTableIndex, lRowIndex, 0, bForce, bEditable );

	// ���X�g�r���[���A�R�t���Ɏg�p������̂�������
	static bool initListView = initializeListView();

	if( !initListView )
	{
		SetDlgStatus( IDC_RELATIONLIST, FALSE );
		SetDlgStatus( IDC_SEARCHLIST, FALSE );
		SetDlgStatus( IDC_BUTTON_SEARCH_POI, FALSE );
		SetDlgStatus( IDC_METER_BOX, FALSE );
		SetDlgStatus( IDC_BUTTON_APPLY, FALSE );
		return bRet;
	}

	// �K�p�{�^���ȊO�͑���ł���悤�ɂ��Ă���
	SetDlgStatus( IDC_RELATIONLIST, TRUE );
	SetDlgStatus( IDC_SEARCHLIST, TRUE );
	SetDlgStatus( IDC_BUTTON_SEARCH_POI, TRUE );
	SetDlgStatus( IDC_METER_BOX, TRUE );
	SetDlgStatus( IDC_BUTTON_APPLY, FALSE );

	// �K���{�^�����������ۂɎg�p����폜�p�o�b�t�@�͏���
	m_CheckedRemove.clear();

	long lOID = AheGetOID(GetCurrentFeatureDef()->GetFeature());
	// ���X�g�r���[��œ����I�ɕێ����Ă���f�[�^������Ȃ�A�������\������
	// �����_�C�A���O���J������ɁA�t�B�[�`���[��؂�ւ����ꍇ�A
	if(! m_RelationView.Review( lOID ) )
	{
		// �R�t������Ă���POI�����X�g�ɕ\��
		dispPOIInfo( IDC_RELATIONLIST );
	}
	m_SearchView.Review( lOID );

	// �����[�V�������X�g�ɂ�����̂ɂ̓`�F�b�N��t����
	m_IsCheckedChangeFromProg = true;
	auto relationIDList = m_RelationView.MakeTargetColumnList( UNIQUE_CODE );
	m_SearchView.SetCheckedOn( UNIQUE_CODE, relationIDList );
	m_IsCheckedChangeFromProg = false;

	// ���ڃt�B�[�`���Ɋ֌W�Ȃ��G�f�B�b�g�{�b�N�X(��F�Q�Ɨp�̘H�����V�[�P���X�⌴�e�ԍ�����)�Ȃǂ𐧌䂷��
	// �ҏW�J�n�O�ƕҏW�J�n��ł��ҏW�s�̂Ƃ���DISABLE�ɂ���
	if( IsEditableFeature() && m_cArcHelper.IsStartEditing() && bEditable )
	{
		SetDlgStatus( IDC_BUTTON_APPLY, TRUE );
	}

	// �G�f�B�b�g�{�b�N�X�ɏ����l��ݒ�
	SetDlgItemText( IDC_METER_BOX,_T("50") );

	// �ŏ��Ƀt�H�[�J�X�����R���g���[��
	::SetFocus( GetDlgItem(IDC_METER_BOX) );

	return bRet;
}

/**
 * @brief �q�_�C�A���O�����ۂɍ쐬���܂��B
 *
 * ���ۂ̍쐬��ControlRel�ɔC���܂��B
 */
void CAttrStationAOIDlg::CreateControlRelation()
{
	ATLASSERT( IsWindow() == TRUE );

	if( m_cControlRel.size() > 0 )
	{
		// �e�E�C���h�E�n���h����ݒ肵�Ȃ���
		m_cControlRel.ResetHWND( m_hWnd );
	}
	else {
		m_cControlRel.SetHWND( m_hWnd );
	}
}

LRESULT CAttrStationAOIDlg::OnClickedSearchButton(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if( wID != IDC_BUTTON_SEARCH_POI ) return FALSE;

	// �I�����Ă���t�B�[�`���[�̌`����擾
	IFeaturePtr ipFeature = GetCurrentFeatureDef()->GetFeature();
	IGeometryPtr ipGeom;
	ipFeature->get_Shape(&ipGeom);
	ITopologicalOperatorPtr ipTopo( ipGeom );
	if( !ipTopo )
		return FALSE;

	// �G�f�B�b�g�{�b�N�X�ɓ��͂���Ă���l�͈̔͂ɂ�����̂�����
	CWindow edit( GetDlgItem( IDC_METER_BOX ) );

	// �l���擾
	CString value;
	edit.GetWindowText(value);
	int meter = std::stoi(value.GetString());
	if(meter < 0) meter = 0;

	// �P�ʂ����[�g������x�ɕϊ�
	IUnitConverterPtr ipUnitConv( CLSID_UnitConverter );
	double dConvRange = 0.0;
	ipUnitConv->ConvertUnits( meter, esriMeters, esriDecimalDegrees, &dConvRange );

	// �G�f�B�b�g�{�b�N�X�ɓ��͂���Ă���l�͈̔͂ɂ�����̂�����
	// �o�b�t�@�����O�`��ŋ�Ԍ���
	IGeometryPtr ipGeoBuffer;
	ipTopo->Buffer( dConvRange, &ipGeoBuffer );
	if( !ipGeoBuffer )
		return FALSE;

	// TOC����STATION_POINT���擾
	int stCount = 0; //!< TOC���STATION_POINT�̐����J�E���g
	ILayerPtr ipLayer = getLayer( station_point::kTableName, stCount, m_cArcHelper.GetFocusMap() );
	// STATION_POINT��TOC��ɓ�ȏ㑶�݂���ꍇ
	if( stCount >= 2 )
	{
		MessageBox( _T("TOC��STATION_POINT����ȏ㑶�݂��邽��\n�R�t����STATION_POINT�����ł��܂���"), _T("STATION_POI�̓���Ɏ��s") );
		return FALSE;
	}
	if( !ipLayer)
		return FALSE;

	CTargetItems cItems;
	AheSelectByShapeFromLayer( ipGeoBuffer, cItems, ipLayer );

	// �\������Ă��郊�X�g�Ɗ֘A����f�[�^������
	m_SearchView.Delete( AheGetOID(ipFeature) );
	m_SearchView.Clear();
	for( auto& item : cItems )
	{
		long lOID = AheGetOID( item.GetFeature() );
		CString strOID;
		strOID.Format(_T("%ld"), lOID);

		AOI_MATRIX dispList = getViewList( lOID );
		for( auto& elem : dispList )
		{
			m_SearchView.AddItem( elem );
		}
	}

	// �����[�V�������X�g�ɂ�����̂ɂ̓`�F�b�N��t����
	m_IsCheckedChangeFromProg = true;
	auto relationIDList = m_RelationView.MakeTargetColumnList( UNIQUE_CODE );
	m_SearchView.SetCheckedOn( UNIQUE_CODE, relationIDList );
	m_IsCheckedChangeFromProg = false;
	return TRUE;
}

// �K���{�^���������ꂽ���ɌĂ΂�鏈��
// SearchList�Ń`�F�b�N�̕t���Ă���s��RelationList�ɑ}�����A�`�F�b�N�̂Ȃ��s���폜����
LRESULT CAttrStationAOIDlg::OnClickedApplyButton(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if( wID != IDC_BUTTON_APPLY ) return FALSE;

	bool isChanged = false; //!< �ύX�����������ǂ����̃t���O

	// �`�F�b�N���O���ꂽ���̂����X�g�r���[����폜
	isChanged |= removePOIFromRelationLV();

	// �`�F�b�N������ꂽ���̂����X�g�r���[�ɒǉ�
	isChanged |= addPOIToRelationLV();

	// �ύX�Ȃ��Ȃ�X�V�̕K�v���Ȃ��̂�FALSE��Ԃ�
	if( !isChanged ) return FALSE;

	// �R���g���[����ŕύX�������Ƃɂ��邽�߂�m_bChanged�t���O��TRUE�ɂ���
	GetCurrentFeatureDef()->GetFieldDef( sj::station_aoi::kTableName, _T("OBJECTID") )->m_bChanged = TRUE;
	SetButton( TRUE );

	return TRUE;
}

// ���X�g�̍s���I�����ꂽ���ɌĂ΂�鏈��(����̃|�C���g���n�C���C�g����)
LRESULT CAttrStationAOIDlg::OnClickedList(LPNMHDR pnmd)
{
	LPNMLISTVIEW pnmlist = (LPNMLISTVIEW)pnmd;
	if(! ( (pnmlist->uChanged & LVIF_STATE) && (pnmlist->uNewState & LVIS_SELECTED) && !(pnmlist->uOldState & LVIS_SELECTED) ) )
		return FALSE;

	auto lList = ((pnmd->idFrom==IDC_SEARCHLIST)? m_SearchView:m_RelationView).MakeTargetColumnList(UNIQUE_CODE);

	// �ړI�̍s�܂ŃC�e���[�^��i�߂�
	auto it = lList.begin();
	std::advance( it, pnmlist->iItem );
	if( it== lList.end() )
		return FALSE;

	long stationOID = -1; //<! �n�C���C�g����STATION_POINT��OBJECTID���i�[����ϐ�
	ITablePtr ipTable = AheOpenSameOwnerTable( (ITablePtr)m_ipStationClass, join_line_info::kTableName );
	if( !ipTable )
		return FALSE;

	CString strSubField;
	strSubField.Format(_T("%s,%s"), join_line_info::kStationID, _T("OBJECTID") );
	_ICursorPtr ipCursor;

    if( FAILED(ipTable->Search(
        AheInitQueryFilter(nullptr, strSubField, _T("%s=%s"), _T("OBJECTID"), *it ),
		VARIANT_TRUE, &ipCursor) ) )
		return FALSE;

	long index = 0;
	if( FAILED(ipCursor->FindField(CComBSTR(join_line_info::kStationID),&index) ) )
		return FALSE;

	// ���������v�f��1�����Ȃ��͂�
	_IRowPtr ipRow;
	while( ipCursor->NextRow( &ipRow )==S_OK && ipRow )
	{
		CComVariant value = AheGetAttribute( ipRow, index );
		if( value.vt == VT_EMPTY )
			return FALSE;
		// lVal�ȊO�ɂ͒l�������Ă��Ȃ��͂�
		stationOID = value.lVal;
	}

	// �������ꂽ�v�f���n�C���C�g�\������
	setHighLight( stationOID );

	// FALSE��Ԃ��Ȃ��Ɛe(?)�̏���(�Ⴆ�΁A���X�g���N���b�N�����Ƃ��ɁA���̍s�̐F��ς���A�Ƃ���������)�����s����Ȃ�
	return FALSE;
}

//�`�F�b�N�{�b�N�X���`�F�b�N���ꂽ���̏���
LRESULT CAttrStationAOIDlg::OnClickedCheckBox(LPNMHDR pnmd)
{
	// �����{�^���������ꂽ�Ƃ��̏����ł͉������Ȃ�
	if( m_IsCheckedChangeFromProg ) return FALSE;

	LPNMLISTVIEW pnmlist = (LPNMLISTVIEW)pnmd;
	// uOldState��0�̂Ƃ��͏������̂Ƃ��Ȃ̂ŁA�������Ȃ�
	if(!pnmlist->uOldState)
		return FALSE;

	UINT newState = (pnmlist->uNewState & LVIS_STATEIMAGEMASK);
	UINT oldState = (pnmlist->uOldState & LVIS_STATEIMAGEMASK);

	CString JOIN_OID = m_SearchView.GetValue( pnmlist->iItem, UNIQUE_CODE );
	// �`�F�b�N�L�����ł���ꍇ
	if( (oldState == INDEXTOSTATEIMAGEMASK(2)) && (newState == INDEXTOSTATEIMAGEMASK(1))  )
	{
		// �ҏW�J�n����Ă���ꍇ
		if(m_cArcHelper.IsStartEditing())
		{
			// RelationList�ɒl�����݂��A�`�F�b�N���O���ꂽ�Ȃ�A�L�^���Ă���
			if( m_RelationView.IsExist( JOIN_OID, UNIQUE_CODE ) )
				m_CheckedRemove.insert( JOIN_OID );
		}
		// �ҏW�J�n����Ă��Ȃ��ꍇ�A�`�F�b�N�{�b�N�X�̏�Ԃ����ɖ߂�
		// ���X�g�r���[��̃`�F�b�N�{�b�N�X�����𑀍�ł��Ȃ����邽�߂̏����B
		// TODO:�`�F�b�N����ENABLE�ɂł���Ȃ�A������𗘗p�̂���
		else
		{
			m_IsCheckedChangeFromProg = true;
			m_SearchView.ChangeChecked( pnmlist->iItem, TRUE );
			m_IsCheckedChangeFromProg = false;
		}
	}
	// �`�F�b�N�����L�̏ꍇ
	else if( (oldState == INDEXTOSTATEIMAGEMASK(1)) && (newState == INDEXTOSTATEIMAGEMASK(2))  )
	{
		// �ҏW�J�n����Ă���ꍇ
		if(m_cArcHelper.IsStartEditing())
		{
			auto it = m_CheckedRemove.find( JOIN_OID );
			if( it!= m_CheckedRemove.end() )
				m_CheckedRemove.erase( it );
		}
		// �ҏW�J�n����Ă��Ȃ��ꍇ�A�`�F�b�N�{�b�N�X�̏�Ԃ����ɖ߂�
		// ���X�g�r���[��̃`�F�b�N�{�b�N�X�����𑀍�ł��Ȃ����邽�߂̏����B
		// TODO:�`�F�b�N����ENABLE�ɂł���Ȃ�A������𗘗p�̂���
		else
		{
			m_IsCheckedChangeFromProg = true;
			m_SearchView.ChangeChecked( pnmlist->iItem, FALSE );
			m_IsCheckedChangeFromProg = false;
		}
	}

	return FALSE;
}

/// STATOIN_POINT���n�C���C�g���鏈��
LRESULT CAttrStationAOIDlg::OnRefresh( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	if( m_ipFeature )
		AheDrawVertex( m_cArcHelper.GetScreenDisplay(), m_ipFeature, FALSE, 5, 10, 10, 255, esriSMSSquare, TRUE );
	return FALSE;
}

// OK�{�^���������ꂽ��ɌĂяo�����̂ŁA�����Ŋ֘A�e�[�u�����X�V����
BOOL CAttrStationAOIDlg::ErrorCheck()
{
	std::map< long, AOI_MATRIX >& relationData = m_RelationView.getRelationData();

	std::list<long> eraseList;
	BOOL isFailed = TRUE;
	for( auto& elem : relationData )
	{
		if( update(elem.first, elem.second ) )
		{
			eraseList.push_back( elem.first );
		}
		else
		{
			isFailed = FALSE;
		}
	}
	for( auto& elem : eraseList )
	{
		// �I�u�W�F�N�g�w���I�ɍl���Đl�̂��̂��������ŏ����̂͂�����ł��傤...
		// ���ǁA�ʓ|�Ȃ̂ł������Ă���
		relationData.erase( relationData.find(elem) );
	}
	return isFailed;
}

// ���X�g�r���[�̏���������
bool CAttrStationAOIDlg::initializeListView()
{
	// �֘A�e�[�u�����Ǘ�����C���X�^���X�̏�����
	FIELDNAMES subFields;
	subFields.push_back( sj::rel_station_poi::kJoinLineInfo );
	m_RelTableM.Initialize( sj::rel_station_poi::kTableName, sj::rel_station_poi::kAOI_ID,
		subFields, GetCurrentFeatureDef()->GetFeature() );

	// ���X�g�r���[�ɃJ������ݒ�
	CAOIListViewCtrl::COLUMN_NAMES columns;
	columns.push_back(COLUMNS[0]);
	columns.push_back(COLUMNS[1]);
	columns.push_back(COLUMNS[2]);
	columns.push_back(COLUMNS[3]);

	m_RelationView.Init(m_hWnd, IDC_RELATIONLIST, columns, 0);
	m_SearchView.Init(m_hWnd, IDC_SEARCHLIST, columns, LVS_EX_CHECKBOXES);

	// TOC����STATION_POINT���擾
	int stCount = 0; //!< TOC���STATION_POINT�̐����J�E���g
	ILayerPtr ipPoi = getLayer( station_point::kTableName, stCount, m_cArcHelper.GetFocusMap() );
	if( !ipPoi )
	{
		MessageBox( _T("STATION_POINT�ƕR�t�����邽�߂ɂ�\nSTATION_POINT��TOC�ɒǉ�����K�v������܂�"), _T("STATION_POI�̎擾�Ɏ��s") );
		return false;
	}
	// STATION_POINT��TOC��ɓ�ȏ㑶�݂���ꍇ
	if( stCount >= 2 )
	{
		MessageBox( _T("TOC��STATION_POINT����ȏ㑶�݂��邽��\n�R�t����STATION_POINT�����ł��܂���"), _T("STATION_POI�̓���Ɏ��s") );
		return false;
	}

	m_ipStationClass = AheGetFeatureClass( ipPoi );
	if( !m_ipStationClass )
	{
		MessageBox( _T("STATION_POINT�t�B�[�`���[���J���܂���"), _T("STATION_POI�̃I�[�v���Ɏ��s") );
		return false;
	}

	m_LineCorpManager.setTable( (ITablePtr)m_ipStationClass );
	m_StationNameManager.setTable( (ITablePtr)m_ipStationClass );

	// �ȉ��A�e�[�u���̑Ή��֌W��ݒ肷��
	std::list<std::pair<CString,CString>> tempList;
	tempList.push_back( std::make_pair( join_line_info::kTableName, _T("OBJECTID") ) );
	tempList.push_back( std::make_pair( railway_line::kTableName, railway_line::kName ) );
	tempList.push_back( std::make_pair( railway_corp::kTableName, railway_corp::kName ) );
	m_LineCorpManager.SetReturnData( tempList );

	//STATION_POINT�ɑ΂���JOIN_LINE_INFO�̊֌W
	m_LineCorpManager.Join( station_point::kTableName, join_line_info::kTableName, _T("OBJECTID"), join_line_info::kStationID );
	//JOIN_LINE_INFO�ɑ΂���RAILWAY_LINE�̊֌W
	m_LineCorpManager.Join( join_line_info::kTableName, railway_line::kTableName, join_line_info::kLineID, _T("OBJECTID") );
	//RAILWAY_LINE�ɑ΂���RAILWAY_CORP�̊֌W
	m_LineCorpManager.Join( railway_line::kTableName, railway_corp::kTableName, railway_line::kCorpID, _T("OBJECTID") );

	//// �ȉ��A�w���擾�p
	// �ė��p
	tempList.clear();
	tempList.push_back( std::make_pair( join_line_info::kTableName, _T("OBJECTID") ) );
	tempList.push_back( std::make_pair( station_info::kTableName, station_info::kName ) );
	m_StationNameManager.SetReturnData( tempList );

	////STATION_POINT�ɑ΂���JOIN_LINE_INFO�̊֌W
	m_StationNameManager.Join( station_point::kTableName, join_line_info::kTableName, _T("OBJECTID"), join_line_info::kStationID );
	//JOIN_LINE_INFO�ɑ΂���STATION_INFO�̊֌W
	m_StationNameManager.Join( join_line_info::kTableName, station_info::kTableName, join_line_info::kInfoID, _T("OBJECTID") );

	return true;
}

// �w�肳�ꂽ�I�u�W�F�N�gID�̊֘A�e�[�u�����X�V����
bool CAttrStationAOIDlg::update( long lOID, const AOI_MATRIX& lList )
{
	// ���X�g�r���[���JOIN_LINE_INFO_ID�̈ꗗ���擾
	auto lViewList = getUniqueData( lList );
	// DB���JOIN_LINE_INFO_ID�̈ꗗ���擾
	auto tableList = m_RelTableM.GetRelationData( lOID, sj::rel_station_poi::kJoinLineInfo );

	// ���X�g�r���[�ɑ��݂��ADB��ɑ��݂��Ȃ����̂͒ǉ�
	auto addIDList = getDiffList( lViewList, tableList );
	// DB��ɑ��݂��A���X�g�r���[�ɑ��݂��Ȃ����͍̂폜
	auto deleteIDList = getDiffList( tableList, lViewList ); 

	// �e�[�u������R�t�����O���ꂽ�s���폜
	if( !m_RelTableM.DeleteData( lOID, deleteIDList ) )
	{
		CString strErr;
		strErr.Format(_T("%s����R�t�����O���ꂽ�f�[�^���폜���邱�Ƃ��ł��܂���ł���"), sj::rel_station_poi::kTableName);
		MessageBox(strErr);
		return false;
	}
	// �e�[�u���ɍs��ǉ�
	if( !m_RelTableM.AddData( lOID, addIDList ) )
	{
		CString strErr;
		strErr.Format(_T("%s�ɐV���ɕR�t�����ꂽ�f�[�^��ǉ����邱�Ƃ��ł��܂���ł���"), sj::rel_station_poi::kTableName);
		MessageBox(strErr);
		return false;
	}
	return true;
}

// �w�肳�ꂽ�I�u�W�F�N�gID������STATION_POINT���n�C���C�g�\������
bool CAttrStationAOIDlg::setHighLight( long lOID )
{
	// ���������ƃT�u�t�B�[���h��ݒ�
	CString strSubField;
	strSubField.Format(_T("%s,%s"), _T("SHAPE"),_T("OBJECTID"));

	IFeatureCursorPtr ipFeatureCursor;
	if( FAILED(m_ipStationClass->Search(
		AheInitQueryFilter( nullptr, strSubField, _T("%s=%ld"), _T("OBJECTID"), lOID ),
		VARIANT_TRUE, &ipFeatureCursor)) )
		return false;

	// 1�����Ȃ��͂�
	if( ipFeatureCursor->NextFeature(&m_ipFeature)!=S_OK || !m_ipFeature )
		return false;

	// �ĕ`�施�߂��o���đI��_���ĕ`�悳����
	m_cArcHelper.GetActiveView()->PartialRefresh( esriViewForeground, NULL, NULL );

	return true;
}

// �I�����ꂽ�t�B�[�`���[�Ɋ֘A�t�����Ă���POI���擾���A���X�g�r���[�ɕ\��
void CAttrStationAOIDlg::dispPOIInfo( UINT nClassID )
{
	/// ���ݑI�𒆂̃t�B�[�`���[��OID���i�[
	long lOID = AheGetOID( GetCurrentFeatureDef()->GetFeature() );

	/// �f�[�^�x�[�X�ォ��A�I�𒆂̉wAOI�|���S����OID��AOI_ID�Ƃ��Ă��s��JOIN_LINE_INFO_OID�̈ꗗ���擾
	auto joinIDList = m_RelTableM.GetRelationData( lOID, sj::rel_station_poi::kJoinLineInfo );

	// AOI_ID��lOID�����s������
	for( auto& joinID : joinIDList )
	{
		auto railCorpList = m_LineCorpManager.GetValue(joinID, _T("OBJECTID"), join_line_info::kTableName );
		auto stationInfoList = m_StationNameManager.GetValue(joinID, _T("OBJECTID"), join_line_info::kTableName );
		auto dispList = getViewList( railCorpList, stationInfoList );
		for( auto& elem : dispList )
		{
			m_RelationView.AddItem( elem );
		}
	}
}


//�ȉ��A��L�֐����ŕK�v�ȏ�����ʊ֐��Ƃ��Ď�����������

// update
//���X�g���烆�j�[�NID�̃J���������𒊏o���A�擾
AOI_COLUMN CAttrStationAOIDlg::getUniqueData( const AOI_MATRIX& lList ) const
{
	int uniqueIndex = m_RelationView.GetColumnIndex( UNIQUE_CODE );
	AOI_COLUMN retList;
	for( auto& elem : lList )
	{
		auto it = elem.begin();
		std::advance( it, uniqueIndex );
		if( it!= elem.end())
			retList.push_back( *it );
	}
	return retList;
}

std::list<CString> CAttrStationAOIDlg::getDiffList( const std::list<CString>& argList1, const std::list<CString>& argList2 ) const
{
	std::list<CString> retList;
	for(auto& val : argList1)
	{
		auto it = find( argList2.begin(), argList2.end() ,val );
		if( it==argList2.end() )
			retList.push_back( val );
	}
	return retList;
}

// OnClickedApplyButton
// �R�t�����Ă���POI�̃��X�g�r���[����A�`�F�b�N���O���ꂽPOI������
bool CAttrStationAOIDlg::removePOIFromRelationLV()
{
	// �폜�Ώۂɉ����Ȃ��Ȃ牽�����Ȃ�
	if( m_CheckedRemove.empty() )
		return false;

	// �폜�p�֐��Ɉ����n����`���ɕϊ�
	AOI_COLUMN deleteList;
	for( auto& elem : m_CheckedRemove )
		deleteList.push_back( elem );

	// �����n������폜�ΏۂƂ��ĕێ����Ă������̂͏���
	m_CheckedRemove.clear();

	return m_RelationView.DeleteItem( UNIQUE_CODE, deleteList );
}

// �R�t�����Ă���POI�̃��X�g�r���[�ɁA�`�F�b�N������ꂽPOI�𑫂�
bool CAttrStationAOIDlg::addPOIToRelationLV()
{
	auto relationIDList = m_RelationView.MakeTargetColumnList( UNIQUE_CODE );
	int UniqueIDIndex = m_SearchView.GetColumnIndex( UNIQUE_CODE );

	auto lineList = m_SearchView.GetNonExistValue( relationIDList, true, UNIQUE_CODE );
	for( auto& elem : lineList )
	{
		m_RelationView.AddItem( elem );
	}
	return !lineList.empty();
}

// OnClickedSearchButton dispPOIInfo
// ���X�g�r���[�ɑ}������f�[�^���擾
AOI_MATRIX CAttrStationAOIDlg::getViewList( long lOID )
{
	CString strOID;
	strOID.Format(_T("%ld"), lOID);
	auto railCorpList = m_LineCorpManager.GetValue(strOID , _T("OBJECTID"), station_point::kTableName );
	auto stationInfoList = m_StationNameManager.GetValue(strOID , _T("OBJECTID"), station_point::kTableName );

	return getViewList(railCorpList,stationInfoList);
}

AOI_MATRIX CAttrStationAOIDlg::getViewList( AOI_MATRIX& railCorp, AOI_MATRIX& stationInfo ) const
{
	AOI_MATRIX dispList;

	// ��̃��X�g�̃T�C�Y���قȂ�=�f�[�^���s���S�Ȃ̂ŁA�������������͕̂\�����Ȃ�
	if( railCorp.size() != stationInfo.size() )
		return dispList;

	auto corpIt = railCorp.begin();

	for(; corpIt != railCorp.end(); ++corpIt)
	{
		// �f�[�^�}�����Ɏg�p����C�e���[�^���`
		auto insertIt = corpIt->begin();

		AOI_LINE addList;
		// JOIN_LINE_INFO��OBJECTID��}��
		addList.push_back( *insertIt );

		// �w����}��(�����܂ŗ��Ă���Ȃ�A��ɂȂ邱�Ƃ͂Ȃ��͂�)
		for( auto& elem : stationInfo )
		{
			// ��Ԗڂ̗v�f==JOIN_LINE_INFO��OBJECTID�������Ȃ�
			if( *elem.begin() == *insertIt )
			{
				addList.push_back( *(++elem.begin()) );
				break;
			}
		}
		++insertIt;

		// �H������}��
		addList.push_back( *insertIt );
		++insertIt;

		// ��Ж���}��
		addList.push_back( *insertIt );
		++insertIt;

		dispList.push_back( addList );
	}
	return dispList;
}
