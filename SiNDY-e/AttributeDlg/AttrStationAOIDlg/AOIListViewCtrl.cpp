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
#include "AOIListViewCtrl.h"

/**
* @brief �w��s�̎w���̃e�L�X�g���擾
* @param  lineNum   [in]  �e�L�X�g���擾����s�ԍ�
* @param  columnNum [in]  �e�L�X�g���擾�����ԍ�
* @param  lvCtrl    [in]  �e�L�X�g���擾���郊�X�g�r���[
* @return retText   �擾�����e�L�X�g
*/
CString getLVText( int lineNum , int columnNum , const CListViewCtrl& lvCtrl)
{
	_TCHAR retText[128];
	lvCtrl.GetItemText( lineNum, columnNum, retText, 128);
	return retText;
}

// ����������
void CAOIListViewCtrl::Init( HWND hwnd, UINT id, const COLUMN_NAMES& columns, DWORD option )
{
	// ���X�g�r���[�ɃJ������ǉ�
	const UINT COLUMN_MASK = LVCF_FMT|LVCF_WIDTH|LVCF_TEXT;  //!< ��̗L�������o�t���O
	const int  COLUMN_FMT  = LVCFMT_LEFT;                    //!< ��̔z�u
	const int  COLUMN_CX   = 150;                            //!< ��̕�

	m_lvCtrl = CListViewCtrl(GetDlgItem(hwnd,id));

	int insertIndex = 0; //!< �v�f��}������C���f�b�N�X�ԍ�
	for(auto& column : columns)
	{
		m_lvCtrl.AddColumn( column, insertIndex, -1, COLUMN_MASK, LVCFMT_LEFT );
		m_lvCtrl.SetColumnWidth(insertIndex, COLUMN_CX);
		++insertIndex;
	}

	// ���X�g�r���[�̃X�^�C�����w��
	// �I�����ꂽ�Ƃ��ɍs�S�̂�I����Ԃɂ��A�O���b�h��������
	DWORD word = LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES;
	word |= option;
	m_lvCtrl.SetExtendedListViewStyle( word , word );
}

// ���X�g�r���[�̕\�����N���A����
void CAOIListViewCtrl::Clear()
{
	m_lvCtrl.DeleteAllItems();
}

// ���X�g�r���[��̕\��������ŕێ����Ă���f�[�^�ɍ��킹��
bool CAOIListViewCtrl::Review( long lOID )
{
	m_OID = lOID;
	const auto& values = m_OID2Values[m_OID];

	Clear();
	bool isChange = false;
	for( auto& elem : values )
	{
		addData( elem );
		isChange = true;
	}
	return isChange;
}

// ���X�g�r���[�̕\�����N���A���A�����ŕێ����Ă���f�[�^���S�ď���
void CAOIListViewCtrl::DeleteAll()
{
	Clear();
	m_OID2Values.clear();
}

// �����ŕێ�����Ă���f�[�^������
void CAOIListViewCtrl::Delete( long lOID )
{
	m_OID2Values[lOID].clear();
}

// �w��s�̃`�F�b�N�{�b�N�X���`�F�b�N����Ă��邩
bool CAOIListViewCtrl::IsChecked( int lineCount ) const
{
	return m_lvCtrl.GetCheckState( lineCount ) == TRUE;
}

// ���X�g�r���[�ŕ\������Ă���s�̐����擾
int CAOIListViewCtrl::GetLineCount() const
{
	return m_lvCtrl.GetItemCount();
}

// ���X�g�r���[�ŕ\������Ă���f�[�^�����邩����
bool CAOIListViewCtrl::IsEmpty() const
{
	return m_lvCtrl.GetItemCount()==0;
}

// ���X�g�r���[��̎w��J�����Ɏw��̒l�����݂��邩�ۂ�
bool CAOIListViewCtrl::IsExist( const CString& value, const CString& columnName ) const
{
	AOI_COLUMN tempList;
	tempList.push_back(value);
	return IsExist( tempList, columnName );
}

// ���X�g�r���[�Ɉ�s���̃f�[�^��ǉ�����
void CAOIListViewCtrl::AddItem( const AOI_LINE& lList )
{
	// ���X�g�r���[��Ƀf�[�^��\��
	addData( lList );

	// �����I�ɂ������Ă���
	m_OID2Values[m_OID].push_back(lList);
}

// �w��J�����̃��X�g�r���[��ł̃C���f�b�N�X�ԍ����擾
int CAOIListViewCtrl::GetColumnIndex( const CString& columnName ) const
{
	for(int i=0; i<m_lvCtrl.GetHeader().GetItemCount(); ++i)
	{
		// �e�L�X�g�擾�p�̃o�b�t�@
		_TCHAR buff[128];
		LV_COLUMN col;
		ZeroMemory(&col, sizeof(LV_COLUMN));
		col.mask = LVCF_TEXT;
		col.pszText = buff;
		col.cchTextMax = 128;
		m_lvCtrl.GetColumn( i, &col);
		if( columnName.Compare( col.pszText )!=0 )
			continue;
		return i;
	}
	return -1;
}

CString CAOIListViewCtrl::GetValue( unsigned int lineIndex , const CString& columnName) const
{
	auto columnList = MakeTargetColumnList( columnName );
	auto it = columnList.begin();
	std::advance( it, lineIndex );
	if( it==columnList.end() )
		return _T("");
	return *it;
}

// �e�s�̎w��J�����̒l�����X�g���������̂�Ԃ�
AOI_COLUMN CAOIListViewCtrl::MakeTargetColumnList(const CString& columnName, bool isCheckOnly) const
{
	AOI_COLUMN retList;
	// ���X�g�r���[��̃J�����̃C���f�b�N�X�ԍ����擾
	int targetIndex = GetColumnIndex( columnName );

	// ���X�g���̑S�Ă̍s�̃V���A��ID���擾
	for(int i=0; i< m_lvCtrl.GetItemCount(); ++i )
	{
		if( isCheckOnly && !m_lvCtrl.GetCheckState(i) )
			continue;
		retList.push_back( getLVText(i, targetIndex, m_lvCtrl) );
	}
	return retList;
}

// �w��s�Ƀ`�F�b�N������
void CAOIListViewCtrl::ChangeChecked( unsigned int lineIndex, BOOL checkOn )
{
	m_lvCtrl.SetCheckState( lineIndex, checkOn);
}

// �w��J�����̒l��list���ɑ��݂���Ȃ�A���̍s�Ƀ`�F�b�N������
void CAOIListViewCtrl::SetCheckedOn( const CString& columnName, const AOI_COLUMN& lList )
{
	int UniqueIDIndex = GetColumnIndex( columnName );

	for(int i=0; i< m_lvCtrl.GetItemCount(); ++i )
	{
		if( std::find(lList.begin(), lList.end(), getLVText(i, UniqueIDIndex, m_lvCtrl)) != lList.end() )
			m_lvCtrl.SetCheckState( i, TRUE);
	}
}

// �w��J�����̒l��list���ɑ��݂���s���폜����
bool CAOIListViewCtrl::DeleteItem( const CString& keyColumnName, const AOI_COLUMN& lList )
{
	int UniqueIDIndex = GetColumnIndex( keyColumnName );
	if( UniqueIDIndex == -1 )
		return false;

	bool isChanged = false;

	// �����ׂ��v�f��������
	for( auto& elem : lList )
	{
		// ���X�g�r���[�̑S�Ă̍s�𑖍�
		for( int i=0;i<m_lvCtrl.GetItemCount();++i )
		{
			// ���j�[�NID����v����Ȃ�A���̍s�͍폜
			if( elem.Compare( getLVText(i, UniqueIDIndex, m_lvCtrl) ) == 0 )
			{
				m_lvCtrl.DeleteItem(i);
				// �����I�ȃf�[�^���폜
				auto it = m_OID2Values[m_OID].begin();
				std::advance( it, i );
				if( it != m_OID2Values[m_OID].end())
					m_OID2Values[m_OID].erase(it);
				// ���j�[�NID�̓��j�[�N�Ȃ̂ŁA�폜������break����
				isChanged = true;
				break;
			}
		}
	}
	return isChanged;
}

// ���X�g�r���[��̎w��J������list���̒l�����݂��邩�ۂ�
bool CAOIListViewCtrl::IsExist( const AOI_COLUMN& values, const CString& columnName ) const
{
	int targetColumnIndex = GetColumnIndex( columnName );
	if( targetColumnIndex == -1 )
		return false;

	// �s�̐������J��Ԃ�
	for( int lineCount = 0; lineCount < m_lvCtrl.GetItemCount(); ++lineCount )
	{
		// �Ώۂ̃J������list���̒l�����݂���Ȃ�true
		if( std::find(values.begin(),values.end(), getLVText(lineCount, targetColumnIndex, m_lvCtrl) ) != values.end() )
			return true;
	}
	return false;
}

// ���X�g�r���[��̎w��J������list���̒l�����݂��Ȃ��s���擾
AOI_MATRIX CAOIListViewCtrl::GetNonExistValue(
	const AOI_COLUMN& values,
	bool eraseNonChecked ,
	const CString& columnName ) const
{
	AOI_MATRIX retList;
	int targetColumnIndex = GetColumnIndex( columnName );
	if( targetColumnIndex == -1 )
		return retList;

	// �s�̐������J��Ԃ�
	for( int lineCount = 0; lineCount < m_lvCtrl.GetItemCount(); ++lineCount )
	{
		if( eraseNonChecked && !m_lvCtrl.GetCheckState( lineCount ) )
			continue;

		AOI_LINE addList; //!< �e�J�����̒l��ێ����郊�X�g
		//�e�s�̗������
		for( int columnCount=0; columnCount < m_lvCtrl.GetHeader().GetItemCount(); ++columnCount)
		{
			// ��̒l���擾
			CString strBuf = getLVText( lineCount, columnCount, m_lvCtrl );
			// �Ώۂ̃J�����ŁA���Alist���ɗv�f������Ȃ炻��͒ǉ����Ȃ�
			if( columnCount == targetColumnIndex &&
				std::find(values.begin(),values.end(), strBuf) != values.end() )
			{
				break;
			}
			addList.push_back( strBuf );
		}
		// ���[�v���Ō�܂ŉ���Ă����Ȃ�A�Ԃ�l�ɒǉ�
		if( addList.size()== m_lvCtrl.GetHeader().GetItemCount() )
			retList.push_back( addList );
	}
	return retList;
}

// ���X�g�r���[�Ɉ�s���̃f�[�^��ǉ�����
// �ǉ����ꂽ�f�[�^������I�ɕێ����邱�Ƃ͂Ȃ�
void CAOIListViewCtrl::addData( const AOI_LINE& lList )
{
	int itemCount = m_lvCtrl.GetItemCount();
	int index = 0;
	for( const auto& elem : lList )
	{
		m_lvCtrl.AddItem( itemCount, index, elem );
		++index;
	}
	m_lvCtrl.SetItemData( itemCount, 0);
}
