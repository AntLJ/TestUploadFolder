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

	/// �C�x���g�o�^�p��DISPID 
	/// vsflex8n.tli �Œ��ׂ�
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
	* @brief �ĕ`��}�~�p�N���X
	* @note �X�R�[�v���ōĕ`���}�~���܂�
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
	* @brief IVSFlexGrid::FormatString ���b�p�֐�
	* @note �I���W�i���͓�̕����������N�����ꍇ���������̂œƎ��Ő擪�s��ǉ����A�w�b�_��ݒ肵�܂�
	* @param ipFlexGrid [in] �O���b�h
	* @param headerFormat [in] �w�b�_�p�t�H�[�}�b�g�iFormatString�̂��̂ɏ�����j
	* @param sep [in] �t�H�[�}�b�g�p�̃Z�p���[�^
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

		// ���X�g�{�b�N�X��������Ή�����R�[�h�l�Ԃ�
		// �iSetCombo�j�Őݒ肵�Ă���O��A�A�A�A�ł������ȁA�Ƃ肠���� 
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
	* @brief �O���b�h�̃T�C�Y�𒲐�����
	* @param ipFlexGrid [in] �O���b�h
	*/
	inline void AutoRowColSize( IVSFlexGrid* grid )
	{
		if( !grid )
			return;

		long lCols = -1;
		grid->get_Cols( &lCols );

		grid->put_AllowUserResizing( flexResizeColumns );
		grid->put_ExtendLastCol( VARIANT_TRUE ); // �O���b�h�����R���g���[���̕��ɏ���ɍ��킹��

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
	* @brief ���ǉ�����
	* @note �ŉE��̉E�ׂɐV�K���ǉ����܂�
	* @param grid [in] �O���b�h
	* @param header [in] �w�b�_������
	* @param dt [in] �Z���̃f�[�^�^�C�v
	* @param colData [in] ��ɕR�t����f�[�^
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
	* @brief  �Z���̔w�i�F��ݒ肷��֐�
	* 
	* @param	ipGrid		[in]	�w�i�F��ݒ肵����FlexGrid�̃R���g���[��
	* @param	color		[in]	�w�i�F
	* @param	nStartRow	[in]	�ݒ肵�����ŏ��̍s
	* @param	nStartCol	[in]	�ݒ肵�����ŏ��̗�
	* @param	nEndRow		[in]	�ݒ肵�����Ō�̍s(�f�t�H���g�l-1)
	* @param	nEndCol		[in]	�ݒ肵�����Ō�̍s(�f�t�H���g�l-1)
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
	* @brief MouseRow���L���Ȕ͈͂ɂ��邩���ׂ�
	* @note �O���b�h�s���N���b�N���ꂽ���Ƃ��m�F����ۂɎg�p���Ă�������
	* @param [in] grid �O���b�h
	* @retval true MouseRow���O���b�h�s�̏�ɑ��݂���
	* aretval false ��L�ȊO�i�w�b�_�s��A�O���b�h�s�������Ƃ���j
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

