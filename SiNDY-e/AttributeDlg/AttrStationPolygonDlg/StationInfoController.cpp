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

#include "StationInfoController.h"

// WinLib
#include "WinLib/type_convert.h"


using namespace sindy::schema;


// ������
bool CStationInfoController::init( HWND hwnd1, HWND hwnd2, _IRowPtr& ipRow )
{
	// �O���b�h�R���g���[���ݒ�
	//   �O���b�h������
	CAxWindow axWnd1( hwnd1 );
	CAxWindow axWnd2( hwnd2 );
	axWnd1.QueryControl( &m_spStationGrid );
	axWnd2.QueryControl( &m_spSearchStationGrid );
	m_spStationGrid->put_Editable( flexEDKbdMouse );
	m_spSearchStationGrid->put_Editable( flexEDKbdMouse );

	//   �O���b�h�̃w�b�_�쐬
	grid_util::FormatString( m_spStationGrid, station_grid::HEADER );
	grid_util::FormatString( m_spSearchStationGrid, search_station_grid::HEADER );

	//   �O���b�h�̗񕝒���
	grid_util::AutoRowColSize( m_spStationGrid );
	grid_util::AutoRowColSize( m_spSearchStationGrid );

	//   �O���b�h�̃`�F�b�N�{�b�N�X��ݒ�
	m_spSearchStationGrid->put_ColDataType( search_station_grid::grid_cols::Check, flexDTBoolean );


	// �֘A�e�[�u������I�u�W�F�N�g�̏�����
	if( ! m_RelTableMgr.init( ipRow ))
		return false;


	// �w���R�t���e�[�u���̓ǂݍ���
	m_StationInfoData.clear();
	m_StationInfoData_Org.clear();
	m_RelTableMgr.readRelStationSite( m_StationInfoData );
	m_RelTableMgr.readRelStationSite( m_StationInfoData_Org );

	return true;
}


// �w���O���b�h�̕\�����X�V����
void CStationInfoController::updateStationInfoGrid()
{
	using namespace station_grid;
	using namespace uh;

	// �O���b�h�̃T�C�Y�ݒ�(+1�̓w�b�_��)
	m_spStationGrid->PutRows(m_StationInfoData.size() + 1);

	// �O���b�h�ɕ\��
	long row = 1;
	for( const auto& record : m_StationInfoData )
	{
		// �O���[�vID,�w���擾
		CString strGroupId     = str_util::ToString( record.lGroupId );
		CString strStationName = m_RelTableMgr.getStationName( record.lGroupId );

		// �O���b�h�Ɋi�[
		m_spStationGrid->put_TextMatrix( row, grid_cols::GroupId, CComBSTR(strGroupId));	
		m_spStationGrid->put_TextMatrix( row, grid_cols::StationName, CComBSTR(strStationName));	

		++row;
	}
	
	// �񕝒���
	grid_util::AutoRowColSize( m_spStationGrid );
	drawColor();
}


// �w�������O���b�h�̕\�����X�V����
void CStationInfoController::updateSearchStationInfoGrid()
{
	using namespace search_station_grid;
	using namespace uh;

	// �O���b�h�̃T�C�Y�ݒ�
	// ���ӘH���f�[�^�� - �H�����X�g�ɒǉ��ς݂̃f�[�^�� + �w�b�_(+1)
	m_spSearchStationGrid->PutRows(m_SearchStationNameData.size() - m_SearchStationNameData.getDupliCount(m_StationInfoData) + 1);

	// �O���b�h�ɕ\��
	long row = 1;
	for( const auto& record : m_SearchStationNameData )
	{
		// �w���O���b�h�ɑ��݂��郌�R�[�h�͕\�����Ȃ�
		if( m_StationInfoData.isExist( record ) )
			continue;
		
		// �`�F�b�N�{�b�N�X������
		m_spSearchStationGrid->put_Cell(flexcpChecked, CComVariant(row), CComVariant(0L), CComVariant(row), CComVariant(0L), CComVariant(flexUnchecked));

		// �O���[�vID�擾
		CString strGroupId = str_util::ToString( record.lGroupId );
		// �w���擾
		CString strStationName = m_RelTableMgr.getStationName( record.lGroupId );

		// �O���b�h�Ɋi�[
		m_spSearchStationGrid->put_TextMatrix( row, grid_cols::GroupId, CComBSTR( strGroupId ));	
		m_spSearchStationGrid->put_TextMatrix( row, grid_cols::StationName, CComBSTR( strStationName ));	

		++row;
	}

	// �񕝒���
	grid_util::AutoRowColSize( m_spSearchStationGrid );
}


// �O���b�h�ł̕ύX�_(�ǉ�/�폜)���֘A�e�[�u���ɔ��f
bool CStationInfoController::updateTable()
{
	CRelStationSiteData AddData;
	CRelStationSiteData DeleteData;

	// �ǉ��E�ύX�f�[�^�Q�擾
	for( const auto& rec : m_StationInfoData )
	{
		// �ǉ��f�[�^�擾
		const auto org_iter = m_StationInfoData_Org.find( rec );
		if( org_iter == m_StationInfoData_Org.end() )
		{
			AddData.add(rec);
		}
	}

	// �폜�f�[�^�Q�擾
	for( const auto& rec : m_StationInfoData_Org )
	{
		if( ! m_StationInfoData.isExist( rec ))
			DeleteData.add( rec );
	}
	

	// �ύX�𔽉f
	bool bResult = true;
	bResult &= m_RelTableMgr.insertData( AddData );
	bResult &= m_RelTableMgr.deleteData( DeleteData );

	return bResult;
}


// �`�F�b�N�̓������w��񌟍��f�[�^���w���f�[�^�֒ǉ�
void CStationInfoController::addCheckedData()
{
	using namespace station_grid;

	// �`�F�b�N�������Ă��郌�R�[�h�̃O���[�vID���擾
	std::vector<long> IDs = getCheckedId();
	if( IDs.empty() ) return;

	// �w���f�[�^�Ƀ��R�[�h��ǉ�
	for( int id : IDs )
	{
		const auto rec = m_SearchStationNameData.find( id );
		if( rec == m_SearchStationNameData.end() )
			return;

		m_StationInfoData.add( *rec );
	}

	updateChangeStatus();
}


// �w���O���b�h�̑I�𒆂̍s���폜����
void CStationInfoController::deleteSelectedRow()
{
	// �w���O���b�h�𑖍�
	for( int i = m_spStationGrid->FixedRows; i <= m_spStationGrid->Rows - m_spStationGrid->FixedRows; ++i )
	{
		// �I�𒆂łȂ���ɂ͉������Ȃ�
		VARIANT_BOOL vb = VARIANT_FALSE;
		m_spStationGrid->get_IsSelected(i, &vb );
		if( ! vb )
			continue;

		// �R���e�i����폜
		long lGroupId = getGroupIdFromStationGrid(i);
		m_StationInfoData.erase( lGroupId );
	}

	// �ύX�������ۂ��̏�Ԃ��X�V
	updateChangeStatus();
}


// �w�肵���w���̃��R�[�h��ǂݍ���
void CStationInfoController::searchStationName( const CString& strStationName )
{
	if( strStationName.IsEmpty() )
		return;

	// �w������ǂݍ���ŃR���e�i�Ɋi�[
	m_SearchStationNameData.clear();
	m_RelTableMgr.readRelStationSiteByStationName( strStationName, m_SearchStationNameData);
}


// �w���O���b�h�N���b�N���̏���
void CStationInfoController::OnStationInfoGridMouseUp(SHORT Button, SHORT Shift, FLOAT X, FLOAT Y, HWND hwnd, UINT menuId, bool bEnable )
{
	if( ! bEnable )
		return;

	// �N���b�N�s�擾
	long lRow = -1;
	m_spStationGrid->get_MouseRow(&lRow);
	if( lRow < 0 )
		return;

	// �s�I��
	for( int i = m_spStationGrid->FixedRows; i <= m_spStationGrid->Rows - m_spStationGrid->FixedRows; ++i )
	{
		m_spStationGrid->put_IsSelected(i, (i==lRow) ? VARIANT_TRUE : VARIANT_FALSE );
	}

	// �E�N���b�N�̂Ƃ��|�b�v�A�b�v���j���[�\��
	if( Button == clicked::kRight )
	{
		POINT p = {};
		GetCursorPos(&p);
		CMenu menu;
		menu.LoadMenu( menuId );
		menu.GetSubMenu(0).TrackPopupMenu( TPM_LEFTALIGN | TPM_TOPALIGN | TPM_LEFTBUTTON, p.x, p.y, hwnd);
	}
}


// �w���O���b�h�ҏW�O�̏���
void CStationInfoController::OnStationInfoGridBeforeEdit(long Row, long Col, VARIANT_BOOL* Cancel)
{
	*Cancel = VARIANT_TRUE;
}


// �w��񌟍��O���b�h�ҏW�O�̏���
void CStationInfoController::OnSearchStationInfoBeforeEdit(long Row, long Col, VARIANT_BOOL* Cancel, bool bCheckEnable)
{
	// �ҏW�J�n����Ă���ꍇ�̓`�F�b�N�{�b�N�X�̂ݕҏW��
	if( bCheckEnable && Col == search_station_grid::grid_cols::Check )
		return;

	*Cancel = VARIANT_TRUE;
}


// �w��񌟍��O���b�h����`�F�b�N�̓����Ă��郌�R�[�h�̃O���[�vID�Q���擾
std::vector<long> CStationInfoController::getCheckedId()
{
	std::vector<long> group_ids;

	for( int i = 0; i < m_spSearchStationGrid->GetRows(); ++i )
	{
		// �`�F�b�N�������Ă��Ȃ���΃X�L�b�v
		if( ! isCheckedSearchStationGrid(i) ) continue;

		// �O���[�vID�擾
		group_ids.push_back(getGroupIdFromSearchStationGrid(i));
	}

	return group_ids;
}


// �w��񌟍��O���b�h�̎w��s�Ƀ`�F�b�N�������Ă��邩�ǂ������擾
bool CStationInfoController::isCheckedSearchStationGrid(int index) const
{
	CComVariant vaCheck = grid_util::GetValue(m_spSearchStationGrid, index, search_station_grid::grid_cols::Check);
	vaCheck.ChangeType(VT_BOOL);
	return VB2bool(vaCheck.boolVal);
}


// �w��񌟍��O���b�h�̎w��s�̃O���[�vID���擾
long CStationInfoController::getGroupIdFromSearchStationGrid(int index) const
{
	CComVariant vaId = grid_util::GetValue( m_spSearchStationGrid, index, search_station_grid::grid_cols::GroupId );
	vaId.ChangeType(VT_I4);
	return vaId.lVal;
}


// �w���O���b�h�̎w��s�̃O���[�vID���擾
long CStationInfoController::getGroupIdFromStationGrid(int index) const
{
	CComVariant vaId = grid_util::GetValue( m_spStationGrid, index, station_grid::grid_cols::GroupId );
	vaId.ChangeType(VT_I4);
	return vaId.lVal;
}


// �ύX�������ۂ��̃t���O���X�V	 
void CStationInfoController::updateChangeStatus()
{
	if( m_StationInfoData.size() != m_StationInfoData_Org.size() )
	{
		m_bIsChange = true;
		return;
	}

	for( const auto& rec : m_StationInfoData )
	{
		// �ǉ����R�[�h�m�F
		if( ! m_StationInfoData_Org.isExist( rec ))
		{
			m_bIsChange = true;
			return;
		}
	}

	// �폜���R�[�h�m�F
	for( const auto& rec : m_StationInfoData_Org )
	{
		if( ! m_StationInfoData.isExist( rec ))
		{
			m_bIsChange = true;
			return;
		}
	}

	m_bIsChange = false;
}


// �w���O���b�g�̕ύX�s��F�t������ 
void CStationInfoController::drawColor()
{
	long idx = 1;
	for( const auto& rec : m_StationInfoData )
	{
		// �ύX������΃I�����W�A�Ȃ���Δ��Ƀ��R�[�h��F�Â�����
		COLORREF color = m_StationInfoData_Org.isExist( rec ) ? grid_util::color::sindyDefault : grid_util::color::sindyModified;

		m_spStationGrid->PutCell( flexcpBackColor, idx, 0, idx, m_spStationGrid->GetCols() - 1, color );

		++idx;
	}
}
