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

#pragma once

#import <VSFLEXGRID/VSFLEX8N.OCX> no_namespace named_guids
#include<TDC/useful_headers/str_util.h>
#include<boost/assign/list_of.hpp>

namespace grid_util{

	/// イベント登録用のDISPID 
	/// vsflex8n.tli で調べる
	namespace disp_id{
		const long SelChange = 0x1;
		const long BeforeMouseDown = 0x5;
		const long BeforeEdit = 0x15;
		const long AfterEdit = 0x18;
		const long KeyPressEdit = 0x19;
		const long ChangeEdit = 0x1c;
		const long BeforeRowColChange = 0x6;
		const long BeforeSelChange = 0x8;
		const long AfterSelChange = 0x9;
		const long StartEdit = 0x16;
		const long AfterMove = 0x19;
	} // disp_id
	
	/**
	* @brief 再描画抑止用クラス
	* @note スコープ中で再描画を抑止します
	*/
	struct CSuppressRedraw
	{
	private:
		CSuppressRedraw(){}
	public:
		CSuppressRedraw( IVSFlexGridPtr grid ):
			m_org(flexRDNone)
		{
			RedrawSettings rd = flexRDNone;
			grid->get_Redraw( &rd );
			m_org = rd; 
			grid->put_Redraw( flexRDNone );
			m_grid = grid;
		}
		~CSuppressRedraw(){ m_grid->put_Redraw( m_org ); }
	private:
		RedrawSettings m_org;
		IVSFlexGridPtr m_grid;
	};
	
	/**
	* @brief IVSFlexGrid::FormatString ラッパ関数
	* @note オリジナルは謎の文字化けを起こす場合があったので独自で先頭行を追加し、ヘッダを設定します
	* @param ipFlexGrid [in] グリッド
	* @param headerFormat [in] ヘッダ用フォーマット（FormatStringのものに準ずる）
	* @param sep [in] フォーマット用のセパレータ
	*/
	inline void FormatString( IVSFlexGrid* grid, const CString& headerFormat, const CString& sep = _T("|")  )
	{
		if( !grid )
			return;

		static std::map< CString, AlignmentSettings > aligns =
			boost::assign::map_list_of
				( _T("<"), flexAlignLeftCenter )
				( _T("^"), flexAlignCenterCenter )
				( _T(">"), flexAlignRightCenter );
		
		auto& cols = uh::str_util::split( headerFormat, sep );
		grid->put_Cols( cols.size() );
		grid->put_Rows(1);
		for( int i=0; i < cols.size(); ++i )
		{
			_bstr_t col;
			for( const auto& algn : aligns )
			{
				if( cols[i][0] == algn.first )
				{
					grid->put_ColAlignment( i, algn.second );
					col = cols[i].TrimLeft( algn.first );
					break;
				}
			}

			if( col.length() == 0 )
				col = cols[i];
			
			grid->PutTextMatrix( 0, i, col );
		}
	}

	inline void SetCombo( IVSFlexGridPtr grid, long col, const std::map<long, CString>& items )
	{
		CString format;
		for( const auto& item : items )
		{
			format.AppendFormat( _T("#%ld;%s|"), item.first, item.second );
		}
		grid->put_ColComboList(_variant_t(col), _bstr_t(format.TrimRight( _T("|") )));
	}

	inline CComVariant GetValue( IVSFlexGridPtr grid, long row, long col )
	{
		CComVariant val;

		// リストボックスだったら対応するコード値返す
		// （SetCombo）で設定している前提、、、、でいいかな、とりあえず 
		if(grid->GetColComboList(col).length() > 0)
		{
			auto txt = grid->GetTextMatrix( row, col );
			if( txt.length() == 0 )
				return val;

			return CComVariant(_ttol(txt));
		}

		DataTypeSettings dt = flexDTEmpty;
		grid->get_ColDataType( col, &dt );
		
		switch( dt )
		{
		case flexDTLong:
			val = (long)grid->GetValueMatrix( row, col );
			break;
		case flexDTBoolean:
			val = !grid->GetValueMatrix( row, col ) ? 0L : 1L;
			break;
		case flexDTSingle:
		case flexDTDouble:
			val = grid->GetValueMatrix( row, col );
			break;

		default:
			val = (LPCTSTR)grid->GetTextMatrix( row, col );
			break;
		}

		return val;
	}

	/**
	* @brief グリッドのサイズを調整する
	* @param ipFlexGrid [in] グリッド
	*/
	inline void AutoRowColSize( IVSFlexGrid* grid )
	{
		if( !grid )
			return;

		long lCols = -1;
		grid->get_Cols( &lCols );

		grid->put_AllowUserResizing( flexResizeColumns );
		grid->put_ExtendLastCol( VARIANT_TRUE ); // グリッド幅をコントロールの幅に勝手に合わせる

		if( lCols >= 1 )
		{
			grid->put_Redraw( flexRDNone );
			grid->put_AutoSizeMode( flexAutoSizeColWidth );
			grid->AutoSize( 0, lCols -1 );
			grid->put_AutoSizeMode( flexAutoSizeRowHeight );
			grid->AutoSize( 0, lCols -1 );
			grid->put_Redraw( flexRDDirect );
		}
	}
	
	/**
	* @brief 列を追加する
	* @note 最右列の右隣に新規列を追加します
	* @param grid [in] グリッド
	* @param header [in] ヘッダ文字列
	* @param dt [in] セルのデータタイプ
	* @param colData [in] 列に紐付けるデータ
	*/
	inline void AddCol( 
		IVSFlexGrid* grid,
		const CString& header,
		DataTypeSettings dt = flexDTString,
		const _variant_t& colData = _variant_t() )
	{
		long col = 0;
		grid->get_Cols( &col );
		grid->put_Cols( col+1 );
		grid->put_TextMatrix( 0, col, _bstr_t( header ) );
		grid->put_ColDataType( col, dt );
		grid->put_ColData( col, colData );
	}


	namespace color{
		const COLORREF sindyDefault = RGB(255,255,255);
		const COLORREF sindyModified = RGB(255,168,126);
		const COLORREF sindyUnselectable = RGB(128,128,128);
		const COLORREF sindyUnselectableBrighter = RGB(224,224,224);
	}
	/**
	* @brief  セルの背景色を設定する関数
	* 
	* @param	ipGrid		[in]	背景色を設定したいFlexGridのコントロール
	* @param	color		[in]	背景色
	* @param	nStartRow	[in]	設定したい最初の行
	* @param	nStartCol	[in]	設定したい最初の列
	* @param	nEndRow		[in]	設定したい最後の行(デフォルト値-1)
	* @param	nEndCol		[in]	設定したい最後の行(デフォルト値-1)
	*/
	inline void SetCellBackColor( 
		IVSFlexGrid* ipGrid, COLORREF color, 
		long nStartRow, long nStartCol, long nEndRow=-1, long nEndCol=-1 )
	{
		if ( ipGrid ) {
			long nEndRowTmp = nEndRow;
			if ( nEndRow == -1 )
				nEndRowTmp = nStartRow;

			long nEndColTmp = nEndCol;
			if ( nEndCol == -1 )
				nEndColTmp = nStartCol;

			ipGrid->put_Cell( flexcpBackColor, variant_t(nStartRow), variant_t(nStartCol), variant_t(nEndRowTmp), variant_t(nEndColTmp), _variant_t((long)color) );
		}
	}

	/**
	* @brief MouseRowが有効な範囲にあるか調べる
	* @note グリッド行がクリックされたことを確認する際に使用してください
	* @param [in] grid グリッド
	* @retval true MouseRowがグリッド行の上に存在する
	* aretval false 上記以外（ヘッダ行や、グリッド行が無いところ）
	*/
	inline bool IsValidMouseRow( IVSFlexGrid* grid )
	{
		long mouseRow = grid->GetMouseRow();
		return( (grid->GetFixedRows() < mouseRow) && (mouseRow < grid->GetRows()) );
	}

	inline DataTypeSettings GetGridDataType(esriFieldType field_type)
	{
		switch (field_type)
		{
		case esriFieldTypeSmallInteger:
			return flexDTShort;
			break;
		case esriFieldTypeInteger:
		case esriFieldTypeOID:
			return flexDTLong;
			break;
		case esriFieldTypeSingle:
			return flexDTSingle;
			break;
		case esriFieldTypeDouble:
			return flexDTDouble;
			break;
		case esriFieldTypeString:
		case esriFieldTypeGUID:
		case esriFieldTypeGlobalID:
			return flexDTString;
			break;
		case esriFieldTypeDate:
			return flexDTDate;
			break;
		case esriFieldTypeGeometry:
		case esriFieldTypeBlob:
		case esriFieldTypeRaster:
		case esriFieldTypeXML:
			break;
		default:
			break;
		}

		return flexDTEmpty;
	}
}

