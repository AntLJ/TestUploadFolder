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

#include "LineInfoController.h"
#include "util.h"

using namespace sindy::schema;

// �O���b�h�̃w�b�_
static const _TCHAR* LINEINFO_GRID_HEADER   = _T("JOIN_LINE_INFO_OID|�w��|�H����|�w�ԍ�");
static const _TCHAR* AROUND_STA_GRID_HEADER = _T(" |JOIN_LINE_INFO_OID|�w��|�H����");


// �O���b�h�̏�����
bool CLineInfoController::init( HWND hwnd1, HWND hwnd2, _IRowPtr& ipRow, CArcHelper cArcHelper )
{
	// �O���b�h�R���g���[���쐬
	CAxWindow axWnd1( hwnd1 );
	CAxWindow axWnd2( hwnd2 );
	axWnd1.QueryControl( &m_spLineInfoGrid );
	axWnd2.QueryControl( &m_spAroundStaGrid );

	m_spLineInfoGrid->put_Editable( flexEDKbdMouse );
	m_spAroundStaGrid->put_Editable( flexEDKbdMouse );

	// �w�b�_�쐬
	grid_util::FormatString( m_spLineInfoGrid, LINEINFO_GRID_HEADER );
	grid_util::FormatString( m_spAroundStaGrid, AROUND_STA_GRID_HEADER );
	// �񕝒���
	grid_util::AutoRowColSize( m_spLineInfoGrid );
	grid_util::AutoRowColSize( m_spAroundStaGrid );

	// �`�F�b�N�{�b�N�X��ݒ�
	m_spAroundStaGrid->put_ColDataType( around_sta_grid::grid_cols::Check, flexDTBoolean );

	m_cArcHelper = cArcHelper.GetApp();

	// �֘A�e�[�u������I�u�W�F�N�g������
	if( !m_RelTableMgr.init( ipRow, m_cArcHelper) )
		return false;

	// �H�����X�g�f�[�^������
	m_LineInfoData.clear();
	m_AroundStaData.clear();
	m_LineInfoData_Org.clear();
	// �H�����ǂݍ���
	m_RelTableMgr.read( m_LineInfoData );
	m_RelTableMgr.read( m_LineInfoData_Org );

	return true;
}


// �H�����X�g�O���b�h�̕\�����X�V����
void CLineInfoController::updateLineInfoGrid()
{
	using namespace lineinfo_grid;

	// �O���b�h�̃T�C�Y�ݒ�(+1�̓w�b�_��)
	m_spLineInfoGrid->PutRows(m_LineInfoData.size() + 1);

	// �O���b�h�ɕ\��
	long r = 1;
	for( auto& record : m_LineInfoData )
	{
		// JOIN_LINE_ID
		CString strJoinLineId = uh::str_util::format(_T("%d"), record.nJoinLineId);
		m_spLineInfoGrid->put_TextMatrix( r, grid_cols::JoinLineInfoOid, CComBSTR(strJoinLineId));	
		// �w��
		m_spLineInfoGrid->put_TextMatrix( r, grid_cols::StationName    , CComBSTR(record.strStationName));	
		// �H����
		m_spLineInfoGrid->put_TextMatrix( r, grid_cols::RailwayName    , CComBSTR(record.strRailwayName));	
		// �w�ԍ�
		m_spLineInfoGrid->put_TextMatrix( r, grid_cols::StationNumber  , CComBSTR(record.strStationNumber));	

		++r;
	}
	
	// �񕝒���
	grid_util::AutoRowColSize( m_spLineInfoGrid );
	drawColor();
}


// ���Ӊw���X�g�O���b�h�̕\�����X�V����
void CLineInfoController::updateAroundStaGrid()
{
	using namespace around_sta_grid;

	// �O���b�h�̃T�C�Y�ݒ�
	// ���Ӊw�f�[�^�� - �H�����X�g�ɒǉ��ς݂̃f�[�^�� + �w�b�_(+1)
	m_spAroundStaGrid->PutRows(m_AroundStaData.size() - m_AroundStaData.getDupliCount(m_LineInfoData) + 1);

	// �O���b�h�ɕ\��
	long row = 1;
	for( auto& record : m_AroundStaData )
	{
		// �u�H�����X�g�v���X�g�r���[�ɑ��݂��郌�R�[�h�͕\�����Ȃ�
		if( m_LineInfoData.isExist( record.nJoinLineId ) )
			continue;

		// �`�F�b�N�{�b�N�X������
		m_spAroundStaGrid->put_Cell(flexcpChecked, CComVariant(row), CComVariant(0L), CComVariant(row), CComVariant(0L), CComVariant(flexUnchecked));

		// JOIN_LINE_ID
		CString strJoinLineId = uh::str_util::format(_T("%d"), record.nJoinLineId);

		m_spAroundStaGrid->put_TextMatrix( row, grid_cols::JoinLineInfoOid, CComBSTR(strJoinLineId));	
		// �w��
		m_spAroundStaGrid->put_TextMatrix( row, grid_cols::StationName    , CComBSTR(record.strStationName));	
		// �H����
		m_spAroundStaGrid->put_TextMatrix( row, grid_cols::RailwayName    , CComBSTR(record.strRailwayName));	

		++row;
	}

	// �񕝒���
	grid_util::AutoRowColSize( m_spAroundStaGrid );

}


// ���Ӊw���X�g�O���b�h�̃`�F�b�N�̓��������R�[�h��H�����X�g�֒ǉ�
void CLineInfoController::addCheckedRecord()
{
	// �`�F�b�N�������Ă���ID���擾
	std::vector<long> IDs = getCheckedId();
	if( IDs.empty() ) return;

	for( int id : IDs )
	{
		// �f�[�^����ID�Ō���
		auto rec = m_AroundStaData.find( id );
		if( rec == m_AroundStaData.end() ) continue;
		m_LineInfoData.add(*rec);
	}

	// ���X�g�r���[�̕\���ɔ��f
	updateLineInfoGrid();
	updateAroundStaGrid();

	updateChangeStatus();
}


// �w�����������\��
void CLineInfoController::searchStationInfo(IFeaturePtr ipFeature, ILayerPtr ipLayer)
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
	m_AroundStaData.clear();
	for( auto& sta_point : cItems)
	{
		long lOid = AheGetOID( sta_point.GetFeature() );

		// STATION_POINT��OID���L�[�Ƀf�[�^�ǂݍ���
		m_RelTableMgr.readJoinLineInfo(lOid, m_AroundStaData);

	}

	// �O���b�h���X�V
	updateAroundStaGrid();
}


// ���X�g�r���[�̕ύX�_���֘A�e�[�u���ɔ��f
bool CLineInfoController::updateTable()
{
	CRelTableData AddData;
	std::vector<long> DeleteIds;
	CRelTableData ChangeStaNumData;

	// �ǉ��E�ύX�f�[�^�Q�擾
	for( auto iter = m_LineInfoData.begin(); iter != m_LineInfoData.end(); ++iter)
	{
		// �ǉ��f�[�^�擾
		auto org_iter = m_LineInfoData_Org.find(iter->nJoinLineId);
		if( org_iter == m_LineInfoData_Org.end() )
		{
			AddData.add(*iter);
			continue;
		}
		// �w�ԍ����ύX���ꂽID�i�[
		if( ! iter->equalStaNumber(*org_iter) )
		{
			ChangeStaNumData.add(*iter);
		}
	}

	// �폜�f�[�^�Q�擾
	for( auto org_iter = m_LineInfoData_Org.begin(); org_iter != m_LineInfoData_Org.end(); ++org_iter)
	{
		if( ! m_LineInfoData.isExist( org_iter->nJoinLineId ))
			DeleteIds.push_back(org_iter->nJoinLineId);
	}

	// �ύX�𔽉f
	bool bResult = true;
	bResult &= m_RelTableMgr.addData(AddData);
	bResult &= m_RelTableMgr.changeStationNumber(ChangeStaNumData);
	bResult &= m_RelTableMgr.deleteData(DeleteIds);

	return bResult;
}


// �O���b�g�̕ύX�ӏ���F�t������ 
void CLineInfoController::drawColor()
{
	long idx = 1;
	for( auto& rec : m_LineInfoData )
	{
		// �w�ԍ����ύX���ꂽ�Ȃ�w�ԍ��Z�������F�Â�
		auto iter = m_LineInfoData_Org.find( rec.nJoinLineId );
		if( iter != m_LineInfoData_Org.end() &&  ! rec.equalStaNumber(*iter)  )
		{
			m_spLineInfoGrid->PutCell( flexcpBackColor, idx, lineinfo_grid::grid_cols::StationNumber, 
										                idx, lineinfo_grid::grid_cols::StationNumber,
														RGB(255,218,185));
			++idx;
			continue;
		}

		// �ύX���Ȃ��Ȃ�F�Â��Ȃ��ɂ���
		if( m_LineInfoData_Org.isExist( rec.nJoinLineId ))
		{
			m_spLineInfoGrid->PutCell( flexcpBackColor, idx, 0, 
													idx, m_spLineInfoGrid->GetCols() - 1, 
													RGB(255,255,255));
			++idx;
			continue;
		}

		// �ǉ����ꂽ���R�[�h�Ȃ�F�Â�
		m_spLineInfoGrid->PutCell( flexcpBackColor, idx, 0, 
													idx, m_spLineInfoGrid->GetCols() - 1, 
													RGB(255,218,185));
		++idx;
	}
}


// �O���b�h��ҏW�s�ɂ��� 
void CLineInfoController::disable()
{
	m_spLineInfoGrid->Enabled = false;
	m_spAroundStaGrid->Enabled = false;
}


// �H�����X�g�O���b�h�N���b�N���̏���
void CLineInfoController::OnLineInfoGridMouseUp(SHORT Button, SHORT Shift, FLOAT X, FLOAT Y, HWND hwnd, UINT menuId)
{
	long lRow = -1;
	m_spLineInfoGrid->get_MouseRow(&lRow);

	if( lRow < 0 )
		return;

	// �s�I��
	for( int i = m_spLineInfoGrid->FixedRows; i <= m_spLineInfoGrid->Rows - m_spLineInfoGrid->FixedRows; ++i )
	{
		m_spLineInfoGrid->put_IsSelected(i, (i==lRow) ? VARIANT_TRUE : VARIANT_FALSE );
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


// �H�����X�g�O���b�h�ҏW��̏���
void CLineInfoController::OnLineInfoGridBeforeEdit(long Row, long Col, VARIANT_BOOL* Cancel)
{
	// �w�ԍ��ȊO�ҏW�s��
	if( Col != lineinfo_grid::grid_cols::StationNumber)
		*Cancel = VARIANT_TRUE;
}


// �H�����X�g�O���b�h�ҏW��̏���
void CLineInfoController::OnLineInfoGridAfterEdit(long Row, long Col)
{
	// �w�ԍ��ȊO�ҏW�s��
	if( Col != lineinfo_grid::grid_cols::StationNumber)
		return;

	// �R���e�i�X�V
	long lid = getIdFromLineInfoGrid(Row);
	auto iter = m_LineInfoData.find(lid);
	iter->strStationNumber = getStaNumFromLineInfoGrid(Row); 

	updateLineInfoGrid();

	updateChangeStatus();
}


// �H�����X�g�O���b�h�ҏW�O�̏���
void CLineInfoController::OnAroundStaBeforeEdit(long Row, long Col, VARIANT_BOOL* Cancel)
{
	// �`�F�b�N�{�b�N�X�͕ҏW��
	if( Col == around_sta_grid::grid_cols::Check)
		return;

	*Cancel = VARIANT_TRUE;

	// ������H��ID����w�|�C���g�̃t�B�[�`�����擾
	long lid = getIdFromAroundStaGrid(Row);
	IFeaturePtr ipFeature = m_RelTableMgr.getStationPointFeature(lid);
	if( ! ipFeature )
		return;

	// �w�|�C���g��_�ł�����
	for( int i = 0; i < 3; ++i )
	{
		AheDrawVertex( m_cArcHelper.GetFocusMapScreenDisplay(), ipFeature, FALSE, 5, 255, 69, 0, esriSMSSquare, TRUE );
		Sleep(50);
		m_cArcHelper.GetActiveView()->PartialRefresh( esriViewForeground, NULL, NULL );
	}
}


// �I�𒆂̍s���폜����
void CLineInfoController::deleteSelectedRow()
{
	// �I����JOIN_INFO_ID ���擾
	std::vector<long> DeleteIds;
	for( int i = m_spLineInfoGrid->FixedRows; i <= m_spLineInfoGrid->Rows - m_spLineInfoGrid->FixedRows; ++i )
	{
		VARIANT_BOOL vb = VARIANT_FALSE;
		m_spLineInfoGrid->get_IsSelected(i, &vb );
		if( vb )
		{
			long lid = getIdFromLineInfoGrid(i);
			DeleteIds.push_back(lid);
			// �R���e�i����폜
			m_LineInfoData.erase(lid);
		}
	}

	updateLineInfoGrid();
	updateAroundStaGrid();

	updateChangeStatus();
}


// �u���Ӊw���X�g�v���X�g�r���[����`�F�b�N�̓����Ă��郌�R�[�h��ID�Q���擾
std::vector<long> CLineInfoController::getCheckedId()
{
	using namespace around_sta_grid;

	std::vector<long> retVec;

	for( int i = 0; i < m_spAroundStaGrid->GetRows(); ++i )
	{
		// �`�F�b�N�������Ă��Ȃ���΃X�L�b�v
		if( ! isCheckedAroundStaGrid(i) ) continue;

		// ID�i�[
		retVec.push_back(getIdFromAroundStaGrid(i));
	}

	return retVec;
}


// �`�F�b�N�������Ă��邩�ǂ������擾
bool CLineInfoController::isCheckedAroundStaGrid(int index) const
{
	CComVariant vaCheck = grid_util::GetValue(m_spAroundStaGrid, index, around_sta_grid::grid_cols::Check);
	vaCheck.ChangeType(VT_BOOL);
	return (vaCheck.boolVal == -1);
}


//���Ӊw�O���b�h�̎w����JOIN_INFO_ID���擾 
long CLineInfoController::getIdFromAroundStaGrid(int index) const
{
	CComVariant vaId = grid_util::GetValue(m_spAroundStaGrid, index, around_sta_grid::grid_cols::JoinLineInfoOid);
	vaId.ChangeType(VT_I4);
	return vaId.lVal;
}


//���Ӊw�O���b�h�̎w����JOIN_INFO_ID���擾 
long CLineInfoController::getIdFromLineInfoGrid(int index) const
{
	CComVariant vaId = grid_util::GetValue(m_spLineInfoGrid, index, lineinfo_grid::grid_cols::JoinLineInfoOid);
	vaId.ChangeType(VT_I4);
	return vaId.lVal;
}


// �H�����X�g�O���b�h�̎w���̉w�ԍ����擾 
CString CLineInfoController::getStaNumFromLineInfoGrid(int index) const
{
	CComVariant vaString = grid_util::GetValue(m_spLineInfoGrid, index, lineinfo_grid::grid_cols::StationNumber);
	CString strStaNum(vaString);
	return strStaNum;
}


// �ύX�������ۂ��̃t���O���X�V	 
void CLineInfoController::updateChangeStatus()
{
	for( auto rec : m_LineInfoData )
	{
		auto iter = m_LineInfoData_Org.find(rec.nJoinLineId);
		// �ǉ����R�[�h�m�F
		if( iter == m_LineInfoData_Org.end() )
		{
			m_bIsChange = true;
			return;
		}
		// �w�ԍ��̕ύX�m�F
		if( ! rec.equalStaNumber(*iter) )
		{
			m_bIsChange = true;
			return;
		}
	}

	// �폜���R�[�h�m�F
	for( auto rec : m_LineInfoData_Org )
	{
		if( ! m_LineInfoData.isExist( rec.nJoinLineId ))
		{
			m_bIsChange = true;
			return;
		}
	}

	m_bIsChange = false;
}
