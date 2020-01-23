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
#include <stdio.h>
#include <math.h>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <list>
#include <set>
//include "feature_info.h"

//#include "util_class.h"
//#include "chk_attr_value_tbl.h"
//#include <crd_cnv.h>

using	namespace	std;

#ifndef	_NOTE_RELCHK_H_
#define	_NOTE_RELCHK_H_

// 注記データフィールド名
#define	NF_OBJECTID		"OBJECTID"
#define	NF_ANNOCLS_C	"ANNOCLS_C"
#define	NF_NAMESTR1		"NAMESTR1"
#define	NF_NAMESTR2		"NAMESTR2"
#define	NF_NAMESTR3		"NAMESTR3"
#define	NF_ANNOID		"ANNOID"
#define	NF_SCALE		"SCALE"
#define	NF_NAMEID		"NAMEID"
#define NF_LINEANNO_F	"LINEANNO_F"
#define	NF_FONTSIZE_C	"FONTSIZE_C"
#define	NF_POINT_F		"POINT_F"

#define	SHP_NULL_STR	" "	// スペース

/**
	注記レイヤ
*/
struct	note_layer
{
	int		e_iFID;
	int		e_iOBJECTID;
	int		e_iANNOCLS_C;
	string	e_sNAMESTR1;
	string	e_sNAMESTR2;
	string	e_sNAMESTR3;	// 都市地図は使わない

	note_layer() {
		e_iFID		= 0;
		e_iOBJECTID	= 0;
		e_iANNOCLS_C= 0;
		e_sNAMESTR1	= "";
		e_sNAMESTR2	= "";
		e_sNAMESTR3	= "";
	}
	~note_layer() {
	}

	/**
		比較演算子定義
		@return	bool
	*/
	bool	operator<(const note_layer c_aDat ) const {
		bool	a_bRet	= false;
		if( e_iFID != c_aDat.e_iFID ) {
			if( e_iFID < c_aDat.e_iFID ) {
				a_bRet	= true;
			}			
		}else		if( e_iOBJECTID != c_aDat.e_iOBJECTID ) {
			if( e_iOBJECTID < c_aDat.e_iOBJECTID ) {
				a_bRet	= true;
			}			
		}else
		if( e_iANNOCLS_C != c_aDat.e_iANNOCLS_C ) {
			if( e_iANNOCLS_C < c_aDat.e_iANNOCLS_C ) {
				a_bRet	= true;
			}			
		}else
		if( e_sNAMESTR1 != c_aDat.e_sNAMESTR1 ) {
			if( e_sNAMESTR1 < c_aDat.e_sNAMESTR1 ) {
				a_bRet	= true;
			}			
		}else
		if( e_sNAMESTR2 != c_aDat.e_sNAMESTR2 ) {
			if( e_sNAMESTR2 < c_aDat.e_sNAMESTR2 ) {
				a_bRet	= true;
			}
		}else
		if( e_sNAMESTR3 != c_aDat.e_sNAMESTR3 ) {
			if( e_sNAMESTR3 < c_aDat.e_sNAMESTR3 ) {
				a_bRet	= true;
			}			
		}
		return ( a_bRet );
	}

};

typedef	multimap<int, note_layer, less<int> >			mm_note_layer;
typedef	multimap<int, note_layer, less<int> >::iterator	imm_note_layer;

/**
	注記表示情報レイヤクラス
*/
struct	note_disp_info
{
	int		e_iFID;
	int		e_iANNOID;		// 対応する注記真位置のOID
	int		e_iSCALE;
	int		e_iNAMEID;
	int		e_iLINEANNO_F;	// 2006.04.03
	int		e_iFONTSIZE_C;

	note_disp_info() {
		e_iFID			= 0;
		e_iANNOID		= 0;
		e_iSCALE		= 0;
		e_iNAMEID		= 0;
		e_iLINEANNO_F	= 0;
		e_iFONTSIZE_C	= 0;
	}
	~note_disp_info() {
	}
};

typedef	multimap<int, note_disp_info, less<int> >			mm_note_disp_info;
typedef	multimap<int, note_disp_info, less<int> >::iterator	imm_note_disp_info;

/**
	記号表示情報レイヤクラス
*/
struct	mark_disp_info
{
	int		e_iFID;
	int		e_iANNOID;
	int		e_iSCALE;
	int		e_iPOINT_F;

	mark_disp_info() {
		e_iFID		= 0;
		e_iANNOID	= 0;
		e_iSCALE	= 0;
		e_iPOINT_F	= 0;	
	}
	~mark_disp_info() {
	}

};

typedef	multimap<int, mark_disp_info, less<int> >			mm_mark_disp_info;
typedef	multimap<int, mark_disp_info, less<int> >::iterator	imm_mark_disp_info;

#endif 
