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

#include "stdafx.h"

#include "RailwayLineController.h"
#include "WinLib/type_convert.h"


using namespace sindy::schema;

// �O���b�h�̃w�b�_
static const _TCHAR* RAILWAYLINE_GRID_HEADER    = _T("RAILWAY_LINE_ID|�H����|��Ж�");
static const _TCHAR* SEARCH_RAILWAY_GRID_HEADER = _T(" |RAILWAY_LINE_ID|�H����|��Ж�");


// �O���b�h�̏�����
bool CRailwayLineController::init( HWND hwnd1, HWND hwnd2, _IRowPtr& ipRow, CArcHelper cArcHelper )
{
	// �O���b�h�R���g���[���쐬
	CAxWindow axWnd1( hwnd1 );
	CAxWindow axWnd2( hwnd2 );
	axWnd1.QueryControl( &m_spRailwayLineGrid );
	axWnd2.QueryControl( &m_spSearchRailwayGrid );

	m_spRailwayLineGrid->put_Editable( flexEDKbdMouse );
	m_spSearchRailwayGrid->put_Editable( flexEDKbdMouse );

	// �w�b�_�쐬
	grid_util::FormatString( m_spRailwayLineGrid, RAILWAYLINE_GRID_HEADER );
	grid_util::FormatString( m_spSearchRailwayGrid, SEARCH_RAILWAY_GRID_HEADER );
	// �񕝒���
	grid_util::AutoRowColSize( m_spRailwayLineGrid );
	grid_util::AutoRowColSize( m_spSearchRailwayGrid );

	// �`�F�b�N�{�b�N�X��ݒ�
	m_spSearchRailwayGrid->put_ColDataType( search_railway_grid::grid_cols::Check, flexDTBoolean );

	m_cArcHelper = cArcHelper.GetApp();

	// �֘A�e�[�u������I�u�W�F�N�g������
	if( ! m_RelTableMgr.init( ipRow, m_cArcHelper ))
		return false;

	// �H�����X�g�f�[�^������
	m_RailwayLineData.clear();
	m_SearchFromAroundStaData.clear();
	m_RailwayLineData_Org.clear();
	// �H�����ǂݍ���
	m_RelTableMgr.readRelRailwayLink( m_RailwayLineData );
	m_RelTableMgr.readRelRailwayLink( m_RailwayLineData_Org );

	return true;
}


// �H�����X�g�O���b�h�̕\�����X�V����
void CRailwayLineController::updateRailwayLineGrid()
{
	using namespace railwayline_grid;

	// �O���b�h�̃T�C�Y�ݒ�(+1�̓w�b�_��)
	m_spRailwayLineGrid->PutRows(m_RailwayLineData.size() + 1);

	// �O���b�h�ɕ\��
	long row = 1;
	for( const auto& record : m_RailwayLineData )
	{
		// RAILWAY_LINE_ID
		CString strJoinLineId = uh::str_util::format(_T("%d"), record.lRailwayLineOid);
		m_spRailwayLineGrid->put_TextMatrix( row, grid_cols::RailwayLineOid, CComBSTR(strJoinLineId));	
		// �H����
		m_spRailwayLineGrid->put_TextMatrix( row, grid_cols::RailwayName    , CComBSTR(record.strRailwayName));	
		// ��Ж�
		m_spRailwayLineGrid->put_TextMatrix( row, grid_cols::CorpName    , CComBSTR(record.strCorpName));	

		++row;
	}
	
	// �񕝒���
	grid_util::AutoRowColSize( m_spRailwayLineGrid );
	drawColor();
}


// �H�������O���b�h�̕\�����X�V����
void CRailwayLineController::updateSearchRailwayGrid()
{
	using namespace search_railway_grid;

	CRelTableData* data = nullptr;

	// �������[�h�ŕ\���f�[�^����
	switch(m_SearchMode)
	{
		case kAroundSearch: data = &m_SearchFromAroundStaData;    break;
		case kStarionNameSearch: data = &m_SearchFromStationNameData; break;
		default: 
			return; 
			break;
	}

	putIntoSearchGrid(*data);

}


// �H�������O���b�h�̃`�F�b�N�̓��������R�[�h��H�����X�g�֒ǉ�
void CRailwayLineController::addCheckedData()
{
	using namespace search_railway_grid;

	// �`�F�b�N�������Ă���ID���擾
	std::vector<long> IDs = getCheckedId();
	if( IDs.empty() ) return;

	// �������[�h�ɂ��i�[�ΏەύX
	for( int id : IDs )
	{
		if( m_SearchMode == kAroundSearch)
			addCheckedRecord(id, m_SearchFromAroundStaData);
		else if( m_SearchMode == kStarionNameSearch)
			addCheckedRecord(id, m_SearchFromStationNameData);
	}

	updateChangeStatus();
}


// �w��t�B�[�`��������͈͂̉w�|�C���g����ǂݍ���
void CRailwayLineController::searchStationInfo(IFeaturePtr ipFeature, ILayerPtr ipLayer)
{
	if( !ipFeature || !ipLayer)
		return;

	// �I�����Ă���t�B�[�`���̌`����擾
	IGeometryPtr ipGeom;
	ipFeature->get_Shape( &ipGeom );
	ITopologicalOperatorPtr ipTopo( ipGeom );
	if( !ipTopo )
		return;

	// �����͈͂�x�Ŏ擾
	IUnitConverterPtr ipUnitConv( CLSID_UnitConverter );
	double dConvRange = 0.0;
	ipUnitConv->ConvertUnits( SEARCH_METER, esriMeters, esriDecimalDegrees, &dConvRange );

	// ��Ԍ���
	IGeometryPtr ipGeoBuffer;
	ipTopo->Buffer( dConvRange, &ipGeoBuffer );
	if( !ipGeoBuffer )
		return;

	// ���G���A���̃t�B�[�`���Q�擾
	CTargetItems cItems;
	AheSelectByShapeFromLayer( ipGeoBuffer, cItems, ipLayer );

	// �i�[
	m_SearchFromAroundStaData.clear();
	for( const auto& sta_point : cItems)
	{
		long lOid = AheGetOID( sta_point.GetFeature() );

		// STATION_POINT��OID���L�[�Ƀf�[�^�ǂݍ���
		m_RelTableMgr.readRailLineFromStaPtOid(lOid, m_SearchFromAroundStaData);
	}
}


// ��Ж����������O���b�h�ɕ\�� 
void CRailwayLineController::searchStaName(CString strStaName)
{
	if( strStaName.IsEmpty() )
		return;

	// �w������ & �\��
	m_SearchFromStationNameData.clear();
	m_RelTableMgr.readRailwayLineFromStaName(strStaName, m_SearchFromStationNameData);
}


// ���X�g�r���[�̕ύX�_���֘A�e�[�u���ɔ��f
bool CRailwayLineController::updateTable()
{
	CRelTableData AddData;
	std::vector<long> DeleteIds;

	// �ǉ��E�ύX�f�[�^�Q�擾
	for( const auto& rec : m_RailwayLineData )
	{
		// �ǉ��f�[�^�擾
		const auto org_iter = m_RailwayLineData_Org.find(rec.lRailwayLineOid);
		if( org_iter == m_RailwayLineData_Org.end() )
		{
			AddData.add(rec);
			continue;
		}
	}

	// �폜�f�[�^�Q�擾
	for( const auto& rec : m_RailwayLineData_Org )
	{
		if( ! m_RailwayLineData.isExist( rec.lRailwayLineOid ))
			DeleteIds.push_back(rec.lRailwayLineOid);
	}
	

	// �ύX�𔽉f
	bool bResult = true;
	bResult &= m_RelTableMgr.addData(AddData);
	bResult &= m_RelTableMgr.deleteData(DeleteIds);

	return bResult;
}


// �O���b�g�̕ύX�ӏ���F�t������ 
void CRailwayLineController::drawColor()
{
	long idx = 1;
	for( const auto& rec : m_RailwayLineData )
	{
		// �ύX������΃I�����W�A�Ȃ���Δ��Ƀ��R�[�h��F�Â�����
		COLORREF color = m_RailwayLineData_Org.isExist(rec.lRailwayLineOid) ? RGB(255,255,255) : RGB(255,218,185);

		m_spRailwayLineGrid->PutCell( flexcpBackColor, idx, 0, idx, m_spRailwayLineGrid->GetCols() - 1, color );

		++idx;
	}
}


// �H�����X�g�O���b�h�N���b�N���̏���
void CRailwayLineController::OnLineInfoGridMouseUp(SHORT Button, SHORT Shift, FLOAT X, FLOAT Y, HWND hwnd, UINT menuId, bool bEnable )
{
	if( ! bEnable )
		return;

	long lRow = -1;
	m_spRailwayLineGrid->get_MouseRow(&lRow);

	if( lRow < 0 )
		return;

	// �s�I��
	for( int i = m_spRailwayLineGrid->FixedRows; i <= m_spRailwayLineGrid->Rows - m_spRailwayLineGrid->FixedRows; ++i )
	{
		m_spRailwayLineGrid->put_IsSelected(i, (i==lRow) ? VARIANT_TRUE : VARIANT_FALSE );
	}

	// �E�N���b�N�Ȃ�|�b�v�A�b�v���j���[�\��
	if( 2 == Button )
	{
		POINT p = {};
		GetCursorPos(&p);
		CMenu menu;
		menu.LoadMenu( menuId );
		menu.GetSubMenu(0).TrackPopupMenu( TPM_LEFTALIGN | TPM_TOPALIGN | TPM_LEFTBUTTON, p.x, p.y, hwnd);
	}
}


// �H�����X�g�O���b�h�ҏW�O�̏���
void CRailwayLineController::OnRailwayLineGridBeforeEdit(long Row, long Col, VARIANT_BOOL* Cancel)
{
	// RAILWAY_LINE_ID�Ɉ�v����RAILWAY_LINK�̃t�B�[�`���[�Q���擾����(�����N�̃n�C���C�g����)
	long lid = getIdFromRailwayLineGrid(Row);
	m_HighlightFeatures = m_RelTableMgr.getRailwayLinkFeatures(lid);

	*Cancel = VARIANT_TRUE;
}


// �H�������O���b�h�ҏW�O�̏���
void CRailwayLineController::OnSearchRailwayBeforeEdit(long Row, long Col, VARIANT_BOOL* Cancel, bool bCheckEnable)
{
	// �ҏW�J�n����Ă���ꍇ�̃`�F�b�N�{�b�N�X�̂ݕҏW��
	if( bCheckEnable && Col == search_railway_grid::grid_cols::Check ) {
		return;
	}

	// RAILWAY_LINE_ID�Ɉ�v����RAILWAY_LINK�̃t�B�[�`���[�Q���擾����(�����N�̃n�C���C�g����)
	long lid = getIdFromSearchRailwayGrid(Row);
	m_HighlightFeatures = m_RelTableMgr.getRailwayLinkFeatures(lid);

	*Cancel = VARIANT_TRUE;
}


// �I�𒆂̍s���폜����
void CRailwayLineController::deleteSelectedRow()
{
	// �I����JOIN_INFO_ID ���擾
	for( int i = m_spRailwayLineGrid->FixedRows; i <= m_spRailwayLineGrid->Rows - m_spRailwayLineGrid->FixedRows; ++i )
	{
		VARIANT_BOOL vb = VARIANT_FALSE;
		m_spRailwayLineGrid->get_IsSelected(i, &vb );
		if( vb )
		{
			long lid = getIdFromRailwayLineGrid(i);
			// �R���e�i����폜
			m_RailwayLineData.erase(lid);
		}
	}

	updateChangeStatus();
}


// �t�B�[�`���[���n�C���C�g������
void CRailwayLineController::highlightFeatures()
{
	for( const auto& feature : m_HighlightFeatures )
	{
		IGeometryPtr ipGeo;
		feature->get_Shape(&ipGeo);
		if( FAILED( feature->get_Shape(&ipGeo)) || !ipGeo )
			continue;

		AheDrawPolyline( m_cArcHelper.GetFocusMapScreenDisplay(), ipGeo, RGB( 255, 255, 0 ), 7, esriSLSSolid, TRUE );
	}
}


// �H���������X�g�O���b�h����`�F�b�N�̓����Ă��郌�R�[�h��ID�Q���擾
std::vector<long> CRailwayLineController::getCheckedId()
{
	std::vector<long> retVec;

	for( int i = 0; i < m_spSearchRailwayGrid->GetRows(); ++i )
	{
		// �`�F�b�N�������Ă��Ȃ���΃X�L�b�v
		if( ! isCheckedSearchRailwayGrid(i) ) continue;

		// ID�i�[
		retVec.push_back(getIdFromSearchRailwayGrid(i));
	}

	return retVec;
}


// �`�F�b�N�������Ă��邩�ǂ������擾
bool CRailwayLineController::isCheckedSearchRailwayGrid(int index) const
{
	CComVariant vaCheck = grid_util::GetValue(m_spSearchRailwayGrid, index, search_railway_grid::grid_cols::Check);
	vaCheck.ChangeType(VT_BOOL);
	return VB2bool(vaCheck.boolVal);
}


// �H�������O���b�h�̎w��s��RAILWAY_LINE_ID���擾 
long CRailwayLineController::getIdFromSearchRailwayGrid(int index) const
{
	CComVariant vaId = grid_util::GetValue(m_spSearchRailwayGrid, index, search_railway_grid::grid_cols::RailwayLineOid);
	vaId.ChangeType(VT_I4);
	return vaId.lVal;
}


// �H�����X�g�O���b�h�̎w��s��RAILWAY_LINE_ID���擾
long CRailwayLineController::getIdFromRailwayLineGrid(int index) const
{
	CComVariant vaId = grid_util::GetValue(m_spRailwayLineGrid, index, railwayline_grid::grid_cols::RailwayLineOid);
	vaId.ChangeType(VT_I4);
	return vaId.lVal;
}


// �ύX�������ۂ��̃t���O���X�V	 
void CRailwayLineController::updateChangeStatus()
{
	if( m_RailwayLineData.size() != m_RailwayLineData_Org.size() )
	{
		m_bIsChange = true;
		return;
	}

	for( const auto& rec : m_RailwayLineData )
	{
		// �ǉ����R�[�h�m�F
		if( ! m_RailwayLineData_Org.isExist(rec.lRailwayLineOid) )
		{
			m_bIsChange = true;
			return;
		}
	}

	// �폜���R�[�h�m�F
	for( const auto& rec : m_RailwayLineData_Org )
	{
		if( ! m_RailwayLineData.isExist( rec.lRailwayLineOid ))
		{
			m_bIsChange = true;
			return;
		}
	}

	m_bIsChange = false;
}


// �H�������O���b�h�֒l���i�[����
void CRailwayLineController::putIntoSearchGrid(const CRelTableData& cData)
{
	using namespace search_railway_grid;

	// �O���b�h�̃T�C�Y�ݒ�
	// ���ӘH���f�[�^�� - �H�����X�g�ɒǉ��ς݂̃f�[�^�� + �w�b�_(+1)
	m_spSearchRailwayGrid->PutRows(cData.size() - cData.getDupliCount(m_RailwayLineData) + 1);

	// �O���b�h�ɕ\��
	long row = 1;
	for( const auto& record : cData )
	{
		// �u�H�����X�g�v�O���b�h�ɑ��݂��郌�R�[�h�͕\�����Ȃ�
		if( m_RailwayLineData.isExist( record.lRailwayLineOid ) )
			continue;
		
		// �`�F�b�N�{�b�N�X������
		m_spSearchRailwayGrid->put_Cell(flexcpChecked, CComVariant(row), CComVariant(0L), CComVariant(row), CComVariant(0L), CComVariant(flexUnchecked));

		// RAILWAY_LINE_ID
		CString strJoinLineId = uh::str_util::format(_T("%d"), record.lRailwayLineOid);

		m_spSearchRailwayGrid->put_TextMatrix( row, grid_cols::RailwayLineOid, CComBSTR(strJoinLineId));	
		// �w��
		m_spSearchRailwayGrid->put_TextMatrix( row, grid_cols::RailwayName    , CComBSTR(record.strRailwayName));	
		// �H����
		m_spSearchRailwayGrid->put_TextMatrix( row, grid_cols::CorpName    , CComBSTR(record.strCorpName));	

		++row;
	}

	// �񕝒���
	grid_util::AutoRowColSize( m_spSearchRailwayGrid );
}


//�w��R���e�i�̎w��S���H��ID�̃��R�[�h��H�����X�g�f�[�^�R���e�i�֊i�[����
void CRailwayLineController::addCheckedRecord(long lId, CRelTableData& cData)
{
	const auto rec = cData.find( lId );
	if( rec == cData.end() )
		return;

	m_RailwayLineData.add(*rec);
}


// �H�����X�g�f�[�^�𓯊�����
void CRailwayLineController::syncRailwayLineData(CRailwayLineController& cRailwayLineCtlr)
{
	m_RailwayLineData.copyData( cRailwayLineCtlr.m_RailwayLineData );
	cRailwayLineCtlr.updateChangeStatus();
}

