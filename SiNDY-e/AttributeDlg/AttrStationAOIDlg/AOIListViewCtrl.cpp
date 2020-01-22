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
* @brief 指定行の指定列のテキストを取得
* @param  lineNum   [in]  テキストを取得する行番号
* @param  columnNum [in]  テキストを取得する列番号
* @param  lvCtrl    [in]  テキストを取得するリストビュー
* @return retText   取得したテキスト
*/
CString getLVText( int lineNum , int columnNum , const CListViewCtrl& lvCtrl)
{
	_TCHAR retText[128];
	lvCtrl.GetItemText( lineNum, columnNum, retText, 128);
	return retText;
}

// 初期化処理
void CAOIListViewCtrl::Init( HWND hwnd, UINT id, const COLUMN_NAMES& columns, DWORD option )
{
	// リストビューにカラムを追加
	const UINT COLUMN_MASK = LVCF_FMT|LVCF_WIDTH|LVCF_TEXT;  //!< 列の有効メンバフラグ
	const int  COLUMN_FMT  = LVCFMT_LEFT;                    //!< 列の配置
	const int  COLUMN_CX   = 150;                            //!< 列の幅

	m_lvCtrl = CListViewCtrl(GetDlgItem(hwnd,id));

	int insertIndex = 0; //!< 要素を挿入するインデックス番号
	for(auto& column : columns)
	{
		m_lvCtrl.AddColumn( column, insertIndex, -1, COLUMN_MASK, LVCFMT_LEFT );
		m_lvCtrl.SetColumnWidth(insertIndex, COLUMN_CX);
		++insertIndex;
	}

	// リストビューのスタイルを指定
	// 選択されたときに行全体を選択状態にし、グリッド線を入れる
	DWORD word = LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES;
	word |= option;
	m_lvCtrl.SetExtendedListViewStyle( word , word );
}

// リストビューの表示をクリアする
void CAOIListViewCtrl::Clear()
{
	m_lvCtrl.DeleteAllItems();
}

// リストビュー上の表示を内部で保持しているデータに合わせる
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

// リストビューの表示をクリアし、内部で保持しているデータも全て消す
void CAOIListViewCtrl::DeleteAll()
{
	Clear();
	m_OID2Values.clear();
}

// 内部で保持されているデータを消す
void CAOIListViewCtrl::Delete( long lOID )
{
	m_OID2Values[lOID].clear();
}

// 指定行のチェックボックスがチェックされているか
bool CAOIListViewCtrl::IsChecked( int lineCount ) const
{
	return m_lvCtrl.GetCheckState( lineCount ) == TRUE;
}

// リストビューで表示されている行の数を取得
int CAOIListViewCtrl::GetLineCount() const
{
	return m_lvCtrl.GetItemCount();
}

// リストビューで表示されているデータがあるか判定
bool CAOIListViewCtrl::IsEmpty() const
{
	return m_lvCtrl.GetItemCount()==0;
}

// リストビュー上の指定カラムに指定の値が存在するか否か
bool CAOIListViewCtrl::IsExist( const CString& value, const CString& columnName ) const
{
	AOI_COLUMN tempList;
	tempList.push_back(value);
	return IsExist( tempList, columnName );
}

// リストビューに一行分のデータを追加する
void CAOIListViewCtrl::AddItem( const AOI_LINE& lList )
{
	// リストビュー上にデータを表示
	addData( lList );

	// 内部的にも持っておく
	m_OID2Values[m_OID].push_back(lList);
}

// 指定カラムのリストビュー上でのインデックス番号を取得
int CAOIListViewCtrl::GetColumnIndex( const CString& columnName ) const
{
	for(int i=0; i<m_lvCtrl.GetHeader().GetItemCount(); ++i)
	{
		// テキスト取得用のバッファ
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

// 各行の指定カラムの値をリスト化したものを返す
AOI_COLUMN CAOIListViewCtrl::MakeTargetColumnList(const CString& columnName, bool isCheckOnly) const
{
	AOI_COLUMN retList;
	// リストビュー上のカラムのインデックス番号を取得
	int targetIndex = GetColumnIndex( columnName );

	// リスト内の全ての行のシリアルIDを取得
	for(int i=0; i< m_lvCtrl.GetItemCount(); ++i )
	{
		if( isCheckOnly && !m_lvCtrl.GetCheckState(i) )
			continue;
		retList.push_back( getLVText(i, targetIndex, m_lvCtrl) );
	}
	return retList;
}

// 指定行にチェックを入れる
void CAOIListViewCtrl::ChangeChecked( unsigned int lineIndex, BOOL checkOn )
{
	m_lvCtrl.SetCheckState( lineIndex, checkOn);
}

// 指定カラムの値がlist内に存在するなら、その行にチェックを入れる
void CAOIListViewCtrl::SetCheckedOn( const CString& columnName, const AOI_COLUMN& lList )
{
	int UniqueIDIndex = GetColumnIndex( columnName );

	for(int i=0; i< m_lvCtrl.GetItemCount(); ++i )
	{
		if( std::find(lList.begin(), lList.end(), getLVText(i, UniqueIDIndex, m_lvCtrl)) != lList.end() )
			m_lvCtrl.SetCheckState( i, TRUE);
	}
}

// 指定カラムの値がlist内に存在する行を削除する
bool CAOIListViewCtrl::DeleteItem( const CString& keyColumnName, const AOI_COLUMN& lList )
{
	int UniqueIDIndex = GetColumnIndex( keyColumnName );
	if( UniqueIDIndex == -1 )
		return false;

	bool isChanged = false;

	// 消すべき要素を一つ一つ見る
	for( auto& elem : lList )
	{
		// リストビューの全ての行を走査
		for( int i=0;i<m_lvCtrl.GetItemCount();++i )
		{
			// ユニークIDが一致するなら、その行は削除
			if( elem.Compare( getLVText(i, UniqueIDIndex, m_lvCtrl) ) == 0 )
			{
				m_lvCtrl.DeleteItem(i);
				// 内部的なデータも削除
				auto it = m_OID2Values[m_OID].begin();
				std::advance( it, i );
				if( it != m_OID2Values[m_OID].end())
					m_OID2Values[m_OID].erase(it);
				// ユニークIDはユニークなので、削除したらbreakする
				isChanged = true;
				break;
			}
		}
	}
	return isChanged;
}

// リストビュー上の指定カラムにlist内の値が存在するか否か
bool CAOIListViewCtrl::IsExist( const AOI_COLUMN& values, const CString& columnName ) const
{
	int targetColumnIndex = GetColumnIndex( columnName );
	if( targetColumnIndex == -1 )
		return false;

	// 行の数だけ繰り返す
	for( int lineCount = 0; lineCount < m_lvCtrl.GetItemCount(); ++lineCount )
	{
		// 対象のカラムにlist内の値が存在するならtrue
		if( std::find(values.begin(),values.end(), getLVText(lineCount, targetColumnIndex, m_lvCtrl) ) != values.end() )
			return true;
	}
	return false;
}

// リストビュー上の指定カラムにlist内の値が存在しない行を取得
AOI_MATRIX CAOIListViewCtrl::GetNonExistValue(
	const AOI_COLUMN& values,
	bool eraseNonChecked ,
	const CString& columnName ) const
{
	AOI_MATRIX retList;
	int targetColumnIndex = GetColumnIndex( columnName );
	if( targetColumnIndex == -1 )
		return retList;

	// 行の数だけ繰り返す
	for( int lineCount = 0; lineCount < m_lvCtrl.GetItemCount(); ++lineCount )
	{
		if( eraseNonChecked && !m_lvCtrl.GetCheckState( lineCount ) )
			continue;

		AOI_LINE addList; //!< 各カラムの値を保持するリスト
		//各行の列を見る
		for( int columnCount=0; columnCount < m_lvCtrl.GetHeader().GetItemCount(); ++columnCount)
		{
			// 列の値を取得
			CString strBuf = getLVText( lineCount, columnCount, m_lvCtrl );
			// 対象のカラムで、かつ、list内に要素があるならそれは追加しない
			if( columnCount == targetColumnIndex &&
				std::find(values.begin(),values.end(), strBuf) != values.end() )
			{
				break;
			}
			addList.push_back( strBuf );
		}
		// ループが最後まで回っていたなら、返り値に追加
		if( addList.size()== m_lvCtrl.GetHeader().GetItemCount() )
			retList.push_back( addList );
	}
	return retList;
}

// リストビューに一行分のデータを追加する
// 追加されたデータを内部的に保持することはない
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
