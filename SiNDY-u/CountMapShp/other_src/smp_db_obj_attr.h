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
#include <vector>
#include <list>
#include <set>
//#include "poly_class.h"

/**
	更新履歴
		2003.02.03	道路レイヤのフェリー処理が抜けていたので追加
*/

#ifndef	_SMP_DB_OBJ_ATTR_H_
#define	_SMP_DB_OBJ_ATTR_H_

using	namespace	std;

/**
	DB上でオブジェクトが持つ属性等の情報は、シェイプとして取り出した
	時点で属性と切り離されてしまうので、それをなんとかしたくて作成
	カラム名称についても、ここで定義しておくことにする
*/

#define	O_ID		"OBJECTID"
#define	OPE			"OPERATOR"
#define	PUR_C		"PURPOSE_C"
#define	MDA			"MODIFYDATE"
#define	UDA			"UPDATETYPE_C"
#define	CLA_F		"USERCLAIM_F"
#define	MESH		"TMP_MESHCODE"
#define	PRIM		"TMP_PRIMID"

// スケール別表示対応用
#define	DSC1_F		"DISPSCALE1_F"	///< 背景用スケール１フラグ
#define	DSC2_F		"DISPSCALE2_F"	///< 背景用スケール２フラグ
#define	DSC3_F		"DISPSCALE3_F"	///< 背景用スケール３フラグ
#define	DSC4_F		"DISPSCALE4_F"	///< 背景用スケール４フラグ
#define	SC1BGC_C	"SC1BGCLASS_C"	///< 背景種別コード（スケール１）
#define	SC2BGC_C	"SC2BGCLASS_C"	///< 背景種別コード（スケール２）
#define	SC3BGC_C	"SC3BGCLASS_C"	///< 背景種別コード（スケール３）
#define	SC4BGC_C	"SC4BGCLASS_C"	///< 背景種別コード（スケール４）
#define	SC1DT_C		"SC1DISPTYPE_C"	///< 注記用スケール１表示コード
#define	SC2DT_C		"SC2DISPTYPE_C"	///< 注記用スケール２表示コード
#define	SC3DT_C		"SC3DISPTYPE_C"	///< 注記用スケール３表示コード
#define	SC4DT_C		"SC4DISPTYPE_C"	///< 注記用スケール４表示コード
#define	DST_C		"DISPTYPE_C"	///< 都市地図注記表示コード

#define	UNDER_F		"UNDERGROUND_F"
#define	CNT_C		"CNTCLASS_C"
#define	BG_C		"BGCLASS_C"
#define	RAIL_C		"RAILWAYCLASS_C"
#define	STAT_C		"STATIONCLASS_C"
#define	BLD_C		"BLDCLASS_C"
#define	ROAD_C		"ROADCLASS_C"
#define	C_CODE		"CITYCODE"
#define	A_CODE		"ADDRCODE"
#define	T_CODE_F	"TMPCODE_F"
#define	GAI			"GAIKUFUGO"
#define	EXGAI		"EXTGAIKUFUGO"
#define	ADDR_C		"ADDRCLASS_C"
#define	MAN_F		"MANAGE_F"
#define	CORR_F		"CORRIDOR_F"

#define	FLR			"FLOORS"
#define	S_RF		"STEP_RF"
#define	SPEC_F		"SPEC_F"

#define	SHP			"SHAPE"
#define	SHP_A		"SHAPE.AREA"
#define	SHP_L		"SHAPE.LEN"
// 注記のみ
#define	N_STR_1		"NAME_STRING1"
#define	N_STR_2		"NAME_STRING2"
#define	N_STR_3		"NAME_STRING3"
#define	S_NUM_1		"STRING_NUM1"
#define	S_NUM_2		"STRING_NUM2"
#define	S_NUM_3		"STRING_NUM3"
#define	AN_CLS_C	"ANNOCLASS_C"
#define	TMP_AN_CLS	"TMP_ANNO_CLASS"
#define	M_TYPE		"MARKTYPE"
#define	F_SIZE		"FONTSIZE"
#define	A_ID		"ANNO_ID"
// 地番注記のみ
#define	CHINUM		"CHIBAN_NO"		///< 地番番号
#define	A_F			"ANNO_F"		///< 地番注記フラグ

#define	TARGET_S	"TARGET_STR_C"
#define	MESH_C		"MESHCODE"
//行政界のみ
#define	CLR_CODE	"COLORCODE"

//typedef	vector<Dpos>				v_Dpos;
//typedef	vector<Dpos>::iterator		iv_Dpos;
//typedef	vector<v_Dpos>				v_v_Dpos;
//typedef	vector<v_Dpos>::iterator	iv_v_Dpos;

struct	Smp_db_obj_attr
{
	string			e_sLayer_Name;		///< レイヤ名
	unsigned long	e_Obj_ID;			///< オブジェクトID
	string			e_Operator;			///< 作業者
	int				e_Purpose;			///< ?
	string			e_Modify_Date;		///< 作業日時
	int				e_Update_Type;		///< ?
	int				e_User_Claim_F;		///< ?
	int				e_Tmp_Meshcode;		///< テンポラリメッシュコード
	int				e_Tmp_Prim_ID;		///< テンポラリプリミィティブID
	int				e_Sc1_Disp_Type_C;	///< スケール１表示タイプコード(背景・注記共用）
	int				e_Sc2_Disp_Type_C;	///< スケール２表示タイプコード（〃）
	int				e_Sc3_Disp_Type_C;	///< スケール３表示タイプコード（〃）
	int				e_Sc4_Disp_Type_C;	///< スケール４表示タイプコード（〃）

	int				e_Sc1_Disp_Code_C;	///< スケール１表示種別コード(中縮尺・ミドル背景用）
	int				e_Sc2_Disp_Code_C;	///< スケール２表示種別コード（〃）
	int				e_Sc3_Disp_Code_C;	///< スケール３表示種別コード（〃）
	int				e_Sc4_Disp_Code_C;	///< スケール４表示種別コード（〃）

//	int				e_Disp_Type_C;		///< 都市地図注記表示タイプコード
	int				e_Under_Ground_F;

	string			e_Cur_Attr_Cls;		///< 現在の属性クラス名

//	int				e_Cnt_Class_C;		///< 段彩
//	int				e_Bg_Class_C;		///< 都市地図、中縮尺種別
//	int				e_Railway_Class_C;	///< 鉄道路線
//	int				e_Station_Class_C;	///< 駅舎
//	int				e_Bld_Class_C;		///< 建物ポリゴン
	int				e_Class_C;			///< 現在の属性

	// 行政界のみ
	string			e_City_Code;		///< 市区町村コード
	string			e_Addr_Code;		///< 町丁目コード
	int				e_Tmp_Code_F;		///< 仮コード
	int				e_Gaiku_Fugo;		///< 街区符号番号
	string			e_Ext_Gaiku_Fugo;	///< 拡張街区符号
	int				e_Addr_Class_C;
	int				e_Manage_F;			///< 管理フラグ
	int				e_Color_Code;		///< 街区色番号

	// 建物のみ
	int				e_Corridor_F;		///< 渡り廊下フラグ
	int				e_Step_RF;
	int				e_Floors;
	int				e_Spec_F;			///< 段差フラグ？ 2002.7.22

	double			e_Shape_Len;
	double			e_Shape_Area;

	// 注記のみ
	string			e_Str_1;			///< 名称文字列１
	string			e_Str_2;			///< 名称文字列２
	string			e_Str_3;			///< 名称文字列３
	int				e_Str_Num_1;		///< 名称文字数１
	int				e_Str_Num_2;		///< 名称文字数２
	int				e_Str_Num_3;		///< 名称文字数３
	int				e_Anno_Class_C;		///< 注記種別
	string			e_Tmp_Anno_Class;	///< テンポラリ種別
//	int				e_Mark_Type;		///< 図形IDにあたる 2002.7.12　コメントアウト

	// 地番レイヤのみ
	int				e_Chiban_No;		///< 地番番号 2002.7.29
	int				e_Anno_F;			///< 地番注記フラグ 2002.7.29

	int				e_FontSize;			///< フォントサイズ
	int				e_Note_ID;			///< 注記ＩＤ
	int				e_Target_Str_Num;	///< 表示文字列が１、２のどちらか

	int				e_MeshCode;			///< 都市地図、ベース、ミドル、トップマップのメッシュは

	IGeometry**		e_Geometry_Ptr;		///< シェイプへのポインタ
	
	// 図形処理のために点列にばらしてしまう
	enum	data_type {
		PTS_LINE	= 0,
		PTS_POLY,
		PTS_POINT
	};
	int				e_Pts_Type;			///< 点列のタイプ（ポリゴン、ポリライン）
//	int				e_iPt_Count;		///< 点列カウント
//	int				e_iPart_Count;		///< 点列のパート数
//	v_v_Dpos		e_vDPtsArray;		///< ジオメトリから持ってきた点列

	/**
		デフォルトコンストラクタ
	*/
	Smp_db_obj_attr	() {
		e_sLayer_Name		= "";
		e_Obj_ID			= 0;
		e_Purpose			= 0;
		e_Update_Type		= 0;
		e_User_Claim_F		= 0;
		e_Tmp_Meshcode		= 0;
		e_Tmp_Prim_ID		= 0;

		e_Sc1_Disp_Type_C	= 0;
		e_Sc2_Disp_Type_C	= 0;
		e_Sc3_Disp_Type_C	= 0;
		e_Sc4_Disp_Type_C	= 0;

		e_Sc1_Disp_Code_C	= 0;
		e_Sc2_Disp_Code_C	= 0;
		e_Sc3_Disp_Code_C	= 0;
		e_Sc4_Disp_Code_C	= 0;

		e_Under_Ground_F	= 0;

//		e_Cnt_Class_C	= 0;		///< 段彩
//		e_Bg_Class_C	= 0;		///< 都市地図、中縮尺種別
//		e_Railway_Class_C	= 0;	///< 鉄道路線
//		e_Station_Class_C	= 0;	///< 駅舎
//		e_Bld_Class_C		= 0;	///< 建物ポリゴン
		e_Class_C		= 0;

		// 行政界のみ
		e_City_Code		= "";
		e_Addr_Code		= "";
		e_Tmp_Code_F	= 0;		///< 仮コード
		e_Gaiku_Fugo	= 0;

		e_Addr_Class_C	= 0;
		e_Manage_F		= 0;		///< 管理フラグ
		e_Color_Code	= 0;		///< 街区色番号

		// 建物のみ
		e_Corridor_F	= 0;
		e_Step_RF		= 0;
		e_Floors		= 0;
		e_Spec_F		= 0;

		// 注記のみ
		e_Str_Num_1		= 0;		///< 名称文字数１
		e_Str_Num_2		= 0;		///< 名称文字数２
		e_Str_Num_3		= 0;		///< 名称文字数３
		e_Anno_Class_C	= 0;		///< 注記種別
//		e_Mark_Type		= 0;		///< 図形IDにあたる
		e_FontSize		= 0;		///< フォントサイズ
		e_Note_ID		= 0;
		e_Target_Str_Num= 0;

		// 地番注記のみ
		e_Chiban_No		= 0;
		e_Anno_F		= 0;

		e_Shape_Len		= 0.0;
		e_Shape_Area	= 0.0;

	//	e_Geometry_Ptr	= NULL;

		// 新規に追加
		e_Pts_Type		= 0;			///< 点列のタイプ（ポリゴン、ポリライン）
	//	e_vDPtsArray.clear();			///< ジオメトリから持ってきた点列
	}

	/**
		オブジェクトのクリーンアップ
	*/
/*	void	Obj_Clean	( void )
	{
		iv_v_Dpos	a_ivIndx;
		for( a_ivIndx = e_vDPtsArray.begin(); a_ivIndx != e_vDPtsArray.end(); a_ivIndx++ )
		{
			a_ivIndx->clear();
		}
	}
*/
	/**
		ジオメトリを内部点列に変換
		@return	bool
	*/
	bool			Inport_Geo	(
						IGeometryPtr	c_ipGeo		///< ジオメトリ
					);

	/**
		内部点列をジオメトリに変換
		@return	IGeometryPtr	成功
		@return	NULL	失敗
	*/
	IGeometryPtr	Export_Geo	( void );


	// 注記ラインの比較用
	struct less_Note_ID : public std::binary_function<Smp_db_obj_attr, Smp_db_obj_attr, bool>
	{
		// 注記ラインは、e_Note_IDでソートするとサーチが楽
		int operator () (const Smp_db_obj_attr c_Dat_1, const Smp_db_obj_attr c_Dat_2) const
		{
			int	ret	= 0;
			if( c_Dat_1.e_Note_ID != c_Dat_2.e_Note_ID ) {
				if( c_Dat_1.e_Note_ID < c_Dat_2.e_Note_ID ) {
					ret	= 1;
				}
			}
			return (ret);
		}
	};

	int	operator==(const Smp_db_obj_attr c_Dat) {
			int	ret	= 0;
			if( e_Obj_ID			== c_Dat.e_Obj_ID ) {		///< オブジェクトID
				ret	= 1;
			}
			return	( ret );
	/*			e_Operator			== c_Dat.e_Operator &&		///< 作業者
				e_Purpose			== c_Dat.e_Purpose &&		///< ?
				e_Modify_Date		== c_Dat.e_Modify_Date &&	///< 作業日時
				e_Update_Type		== c_Dat.e_Update_Type &&	///< ?
				e_User_Claim_F		== c_Dat.e_User_Claim_F;	///< ?
				e_Tmp_Meshcode		== c_Dat.e_Tmp_Meshcode;	///< テンポラリメッシュコード
				e_Tmp_Prim_ID		== c_Dat.e_Tmp_Prim_ID;		///< テンポラリプリミィティブID
				e_Sc1_Disp_Type_C	== c_Dat.	///< スケール１表示タイプコード(背景・注記共用）
				e_Sc2_Disp_Type_C	== c_Dat.	///< スケール２表示タイプコード（〃）
				e_Sc3_Disp_Type_C	== c_Dat.	///< スケール３表示タイプコード（〃）
				e_Sc4_Disp_Type_C	== c_Dat.	///< スケール４表示タイプコード（〃）
				e_Under_Ground_F	== c_Dat.
				e_Cur_Attr_Cls		== c_Dat.		///< 現在の属性クラス名
				e_Class_C			== c_Dat.			///< 現在の属性
				e_City_Code			== c_Dat.		///< 市区町村コード
				e_Addr_Code			== c_Dat.		///< 町丁目コード
				e_Tmp_Code_F		== c_Dat.		///< 仮コード
				e_Gaiku_Fugo		== c_Dat.		///< 街区符号番号
				e_Ext_Gaiku_Fugo	== c_Dat.	///< 拡張街区符号
				e_Addr_Class_C		== c_Dat.
				e_Manage_F			== c_Dat.			///< 管理フラグ
				e_Color_Code		== c_Dat.		///< 街区色番号
				e_Corridor_F		== c_Dat.		///< 渡り廊下フラグ
				e_Step_RF			== c_Dat.
				e_Floors			== c_Dat.
				e_Shape_Len			== c_Dat.
				e_Shape_Area		== c_Dat.
				e_Str_1				== c_Dat.			///< 名称文字列１
				e_Str_2				== c_Dat.			///< 名称文字列２
				e_Str_Num_1			== c_Dat.		///< 名称文字数１
				e_Str_Num_2			== c_Dat.		///< 名称文字数２
				e_Str_Num_3			== c_Dat.		///< 名称文字数３
				e_Anno_Class_C		== c_Dat.		///< 注記種別
				e_Tmp_Anno_Class	== c_Dat.	///< テンポラリ種別
				e_Mark_Type			== c_Dat.		///< 図形IDにあたる
				e_FontSize			== c_Dat.			///< フォントサイズ
				e_Note_ID			== c_Dat.			///< 注記ＩＤ
				e_Target_Str_Num	== c_Dat.	///< 表示文字列が１、２のどちらか
				e_MeshCode	== c_Dat.			///< 都市地図、ベース、ミドル、トップマップのメッシュは
				e_Geometry_Ptr	== c_Dat.		///< シェイプへのポインタ*/
		}

/*
	int	operator=(const Smp_db_obj_attr c_Dat) {
			e_Obj_ID			= c_Dat.e_Obj_ID;			///< オブジェクトID
			e_Operator			= c_Dat.e_Operator;			///< 作業者
			e_Purpose			= c_Dat.e_Purpose;			///< ?
			e_Modify_Date		= c_Dat.e_Modify_Date;		///< 作業日時
			e_Update_Type		= c_Dat.e_Update_Type;		///< ?
			e_User_Claim_F		= c_Dat.e_Under_Ground_F;	///< ?
			e_Tmp_Meshcode		= c_Dat.e_Tmp_Code_F;		///< テンポラリメッシュコード
			e_Tmp_Prim_ID		= c_Dat.e_Tmp_Prim_ID;		///< テンポラリプリミィティブID
			e_Disp_Scale_1_F	= c_Dat.e_Disp_Scale_1_F;	
			e_Disp_Scale_2_F	= c_Dat.e_Disp_Scale_2_F;
			e_Disp_Scale_3_F	= c_Dat.e_Disp_Scale_3_F;
			e_Disp_Scale_4_F	= c_Dat.e_Disp_Scale_4_F;
			e_Under_Ground_F	= c_Dat.e_Under_Ground_F;

			e_Cur_Attr_Cls		= c_Dat.e_Cur_Attr_Cls;		///< 現在の属性クラス名

			e_Class_C			= c_Dat.e_Class_C;			///< 現在の属性

			// 行政界のみ
			e_City_Code			= c_Dat.e_City_Code;		///< 市区町村コード
			e_Addr_Code			= c_Dat.e_Addr_Code;		///< 町丁目コード
			e_Tmp_Code_F		= c_Dat.e_Tmp_Code_F;		///< 仮コード
			e_Gaiku_Fugo		= c_Dat.e_Gaiku_Fugo;		///< 街区符号番号
			e_Ext_Gaiku_Fugo	= c_Dat.e_Ext_Gaiku_Fugo;	///< 拡張街区符号
			e_Addr_Class_C		= c_Dat.e_Addr_Class_C;
			e_Manage_F			= c_Dat.e_Manage_F;			///< 管理フラグ

			// 建物のみ
			e_Corridor_F		= c_Dat.e_Corridor_F;		///< 渡り廊下フラグ
			e_Step_RF			= c_Dat.e_Step_RF;
			e_Floors			= c_Dat.e_Floors;

			e_Shape_Len			= c_Dat.e_Shape_Len;
			e_Shape_Area		= c_Dat.e_Shape_Area;

			e_Shape_Ptr			= c_Dat.e_Shape_Ptr;		///< シェイプデータへのポインタ
		}*/

	/** 2006.04.28 高速化のために追加
		比較関数
	*/
	bool	operator<(const Smp_db_obj_attr c_Dat) const {
		bool	a_bRet	= false;
		if( e_Obj_ID != c_Dat.e_Obj_ID ) {
			if( e_Obj_ID < c_Dat.e_Obj_ID ) {
				a_bRet	= true;
			}
		}
		return	( a_bRet );
	}

	/**
		構造体にSdeRowからデータをセットする(ArcObjects対応版）
		@return	bool
	*/
	bool	Attr_Set_AOBJ	(
					IFeaturePtr		c_Feature		///< フェッチしてきたフィーチャ[IN]
				//	IGeometryPtr	c_Shape		///< シェイプデータ
			);

	/**
		レイヤ名称を指定する
		※	これを使わない場合は、レイヤ名フィールドは
	*/
	void	Set_Layer_Name	(	string	c_sLayer_Name	) {	///< レイヤ名称
				e_sLayer_Name	= c_sLayer_Name;
			}

	/**
		Unicodeの文字列からSJISにする
	*/
//	string	Str_to_SJIS		(	CComBSTR*	c_CStr	);	// 文字列(Unicode)

};

//typedef		vector<Smp_db_obj_attr>				v_Smp_db_obj_attr;
//typedef		vector<Smp_db_obj_attr>::iterator	iv_Smp_db_obj_attr;
typedef		list<Smp_db_obj_attr>				v_Smp_db_obj_attr;
typedef		list<Smp_db_obj_attr>::iterator		iv_Smp_db_obj_attr;
typedef		list<int>							l_int;
typedef		list<int>::iterator					il_int;

typedef		multiset<Smp_db_obj_attr, Smp_db_obj_attr::less_Note_ID>			s_Smp_db_obj_attr_NLine;
typedef		multiset<Smp_db_obj_attr, Smp_db_obj_attr::less_Note_ID>::iterator	is_Smp_db_obj_attr_NLine;


/**
	行政界ポリゴンは、行政コード毎にまとめて処理できるようにする
*/
struct	admin_geo
{
	string			e_lCode_5;	///< ５桁行政コード
	string			e_lCode_6;	///< ６桁行政コード
	long			e_lOID;		///< 代表オブジェクトID
	long			e_lColor;	///< 色
//	IGeometry**		e_ipGeo;	///< ジオメトリ
	admin_geo	() {
		e_lCode_5		= "";
		e_lCode_6		= "";
		e_lOID			= 0;
		e_lColor		= 0;
//		e_ipGeo			= NULL;
	}
	~admin_geo	() {}

	/**
		比較演算子定義
	*/
	int		operator<(const admin_geo&	cDat) const
			{
				int	ret	= 0;
				if( e_lCode_5 != cDat.e_lCode_5 ) {	// ポリゴン間の距離
					if( e_lCode_5 < cDat.e_lCode_5 )
						ret	= 1;
				}else
				if( e_lCode_6 != cDat.e_lCode_6 ) {	// 距離を調べたベクタ配列ポリゴンのインデックス
					if( e_lCode_6 < cDat.e_lCode_6 )
						ret	= 1;
				}else
				// 2006.07.20 街区色問題対応で追加
				if( e_lColor != cDat.e_lColor ) {
					if( e_lColor < cDat.e_lColor )
						ret	= 1;
				}
				return	( ret );
			}
};

typedef		vector<admin_geo>							v_admin_geo;
typedef		vector<admin_geo>::iterator					iv_admin_geo;
typedef		set<admin_geo, less<admin_geo> >			s_admin_geo;
typedef		set<admin_geo, less<admin_geo> >::iterator	is_admin_geo;


#endif
