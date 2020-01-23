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

/*
**	SiINClassTable
**
**		IN class table class
**
**		Programmed by H.Miyahara
*/
#ifdef WIN32
#pragma warning(disable:4786)
#endif

//#include "stdafx.h"
#include "SiINClassTable.h"
#include "Smp_db_obj_attr.h"


/*
**	static variables
*/

multiset<SiINClassTable::Peace, SiINClassTable::lessC>	SiINClassTable::mTable;


/*
**	inner data
*/
/*
struct INClass
{
	char	eSiLayerName[11];	///< SiNDYレイヤ名称
	int		eSiClass;			///< SiNDY種別コード
	int		eINClass;			///< IN種別
	char*	eName;
};
*/


/**
	種別名称を取り出す
*/
/*
const char* SiINClassTable::
GetName	(	char*	cFieldName,	/// SiNDYフィールド名称
			int		cClass)		///<　種別コード
{
	/*
	if(0 == mTable.size())
	{
		for(int i=0 ; gClasses[i].eSiClass ; ++i)
		{
			mTable.insert(Peace(gClasses[i].eSiClass, i));
		}
	}
	*//*
	Peace	aSearch_Tmp;
	aSearch_Tmp.setData	(cFieldName, cClass);
#ifdef	WIN32
	set<Peace, lessC>::iterator	i = mTable.find(aSearch_Tmp);
#else
	set<Peace>::iterator			i = mTable.find(aSearch_Tmp);
#endif
	if(i == mTable.end()) return 0;

	//return gClasses[(*i).eIndex].eName;
	Peace	ret_tbl	= *i;			// 返すテーブル
	return	(ret_tbl.eName);
}
*/


/**
	フィーチャからデータをセットする
	※全テーブル = 0, 都市地図 = 3x, ベース = 2x, ミドル = 1x
	@return	成功	true
	@return	失敗	false
*/
bool	SiINClassTable::Peace::
Set_Data	(	_IRowPtr	c_Feature,			///< フェッチしてきたフィーチャ[IN]
			 	int			c_iData_Level	)	///< データのレベル
{
	LONG			a_Col_Size	= 0; 
	IFieldsPtr		a_Col_Defs;					// カラムの定義
	c_Feature->get_Fields		( &a_Col_Defs );
	a_Col_Defs->get_FieldCount	( &a_Col_Size );
	
	LONG		i = 0;
	for( i = 0 ; i < a_Col_Size ; i++ )
	{
		IFieldPtr	a_Col_Def;
		CComBSTR	a_Col_Name;
		CComVariant	a_Value;
		a_Col_Defs->get_Field	( i, &a_Col_Def );
		a_Col_Name.Empty();

		esriFieldType	a_Col_Type;				// フィールドタイプ
		a_Col_Def->get_Type	( &a_Col_Type );	// カラムタイプ
		a_Col_Def->get_Name	( &a_Col_Name );	// カラム名称
		c_Feature->get_Value( i, &a_Value );	// 属性値の取得

		a_Col_Name.ToUpper(); // postgre対応

		if(a_Col_Name == "OBJECTID") {			// オブジェクトID
			eIndex			= a_Value.intVal;
		}else
		if(a_Col_Name == "CLASS"){				// SiNDY種別
			eSiClass		= a_Value.intVal;
		}else
		if(a_Col_Name == "CLASS_NAME") {		// 種別名称
			string			a_sName_Str;
			a_sName_Str		= Str_to_SJIS ((CComBSTR *)(&a_Value.bstrVal));
			strcpy_s		( eName, &a_sName_Str[0] );
		}else
		if(a_Col_Name == "LAYERID") {			// レイヤID
			int	a_iLID		= a_Value.intVal;
			int a_iModLID	= a_iLID / 10;
			if( c_iData_Level != 0 && (a_iModLID != c_iData_Level) ) {	// 目的のデータレベル以外は飛ばす
				// トップマップ用 2003.01.09
				if( !(c_iData_Level == top && a_iModLID == 0 ) ){
					// トップマップ用ここまで
					return	( false );
				}
			}
			/////////////////////////////////////////
			string		a_sFName;
			int			a_iTmpType;
			if( !Get_Field_DType	( a_iLID, &a_sFName, &a_iTmpType ) ) {
			//	fprintf	( stderr, "属性取得に失敗\n");
			//	fprintf	( stderr, "未対応のレイヤです<%s>\n", &a_stmpstr[0]);
				return	( false );
			}
			eData_Type		= a_iTmpType;
			eSiFieldName	= a_sFName;
			// 2010.08.18 追加
			switch (c_iData_Level)
			{
			case	top:	m_Top_ExF	= 1;	break;
			case	mid:	m_Mid_ExF	= 1;	break;
			case	base:	m_Base_ExF	= 1;	break;
			case	city:	m_City_ExF	= 1;	break;
			}

			/////////////////////////////////////////
		/*	IDomainPtr	a_ipDomain;
			a_Col_Def->get_Domain(&a_ipDomain);
			ICodedValueDomainPtr ipCodeDomain(a_ipDomain);
			long		aCount;
			ipCodeDomain->get_CodeCount(&aCount);
			for(int j = 0; j < aCount; j++)
			{
				CComVariant	a_cTmpValue;
				CComBSTR	aName;
				string		a_sFName;
				int			a_iTmpType;

				ipCodeDomain->get_Value	( j, &a_cTmpValue );
				ipCodeDomain->get_Name	( j, &aName );
				if( a_iLID == a_cTmpValue.intVal ) {
					string	a_stmpstr	= Str_to_SJIS ( (CComBSTR *)(&aName) );
					if( !Get_Field_DType	( a_iLID, &a_sFName, &a_iTmpType ) ) {
					//	fprintf	( stderr, "属性取得に失敗\n");
						fprintf	( stderr, "未対応のレイヤです<%s>\n", &a_stmpstr[0]);
						return	( false );
					}
					eData_Type		= a_iTmpType;
					eSiFieldName	= a_sFName;
					break;
				}
			}*/
			////////////////////////////////////////
		}else
		if(a_Col_Name == "UNDERGROUND_F") {		// 地上地下フラグ
			eUnder_Flag		= a_Value.intVal;
		}else
		if(a_Col_Name == "IN_CLASS") {			// IN種別
			string			a_sIN_Class_Str;
			char**			gomistr = NULL;
			a_sIN_Class_Str	= Str_to_SJIS ((CComBSTR *)(&a_Value.bstrVal));
			eINClass		= strtoul(&a_sIN_Class_Str[0], gomistr, 16);
			// 0x00000000 の種別のものはテーブルには追加しない 2003.04.14
			if( eINClass == 0x00000000 ) {
			//	fprintf	( stderr, "属性名称:<%s>\t0x%08x\tIN属性値が無いのでとばします\n", &eName[0], eINClass ); 
			//	return	( false ); 2007.01.11 コメントアウト
			}
		}else
		if(a_Col_Name == "TOP_FLAG") {
			m_Top_ExF	=	a_Value.intVal;
		}else
		if(a_Col_Name == "MIDDLE_FLAG") {
			m_Mid_ExF	=	a_Value.intVal;
		}else
		if(a_Col_Name == "BASE_FLAG") {
			m_Base_ExF	=	a_Value.intVal;
		}else
		if(a_Col_Name == "CITY_FLAG") {
			m_City_ExF	=	a_Value.intVal;
		}

#ifdef	DB_ROT_RULE	// 2006.10.12
		else	// 2006.10.11 注記の回転ルールをDBから取得するため修正
		if(a_Col_Name == "ROT_RULE" ) {
			string			a_sRot_Rule;
			a_sRot_Rule		= Str_to_SJIS ((CComBSTR *)(&a_Value.bstrVal));
			if( a_sRot_Rule.size() != 12 ) {
				return	( false );
			}
			strncpy ( eRot_Rule, (char*)a_sRot_Rule.c_str(), 12 );
		}
#endif
	}
	return	( true );
}

/**
	Unicodeの文字列からSJISにする
*/
string	SiINClassTable::Peace::
Str_to_SJIS		(	CComBSTR*	c_CStr	)		// 文字列(Unicode)
{
	string		ret_str;

	USES_CONVERSION;
	if (*c_CStr != 0) {
		_bstr_t		a_bTmp1	( (*c_CStr), false );
		if( (char *)a_bTmp1 == NULL ) {
			ret_str	= "";
		}else {
			ret_str	= (char *)a_bTmp1;
		}
	}
	return	(ret_str);
}

/**
	レイヤIDから、属性のフィールド名とデータタイプ獲得する
	@return	成功	true
	@return	失敗	false
*/
bool	SiINClassTable::Peace::
Get_Field_DType	(	int			c_sLayer_ID,	///< レイヤID[IN]
					string*		c_spFName,		///< フィールド名[OUT]
					int*		c_ipDType	)	///< データタイプ[OUT]
{
	switch	( c_sLayer_ID )
	{
	case	0:		// "common"
		*c_spFName	= "";
		*c_ipDType	= 0;
		break;
	case	1:		// "top_line" 2003.01.05 追加
	case	11:		// "middle_line"
	case	21:		// "base_line" 
	case	31:		// "city_line"
		*c_spFName	= BG_C;
		*c_ipDType	= TYPE_LINE;
		break;
	case	12:		// "middle_railway"
	case	22:		// "base_railway"
	case	32:		// "city_railway" 
		*c_spFName	= RAIL_C;
		*c_ipDType	= TYPE_LINE;
		break;
	case	13:		// "middle_station"
	case	23:		// "base_station"
	case	33:		// "city_station" 
		*c_spFName	= STAT_C;
		*c_ipDType	= TYPE_POLY;
		break;
	case	4:		// "top_site" 2003.01.05 追加
	case	14:		// "middle_site"
	case	24:		// "base_site"
	case	34:		// "city_site"
		*c_spFName	= BG_C;
		*c_ipDType	= TYPE_POLY;
		break;
	case	5:		// "top_contour" 2003.01.05 追加
	case	15:		// "middle_contour"
	case	25:		// "base_contour"
		*c_spFName	= CNT_C;
		*c_ipDType	= TYPE_POLY;
		break;
	case	35:		// "building"
		*c_spFName	= BLD_C;
		*c_ipDType	= TYPE_POLY;
		break;
	case	36:		// building_line
		*c_spFName	= BLD_C;
		*c_ipDType	= TYPE_LINE;
		break;
	default:
		return	( false );
	}
	return	( true );
}

//===================================================================
// ここから　SiINClassTableの定義
//===================================================================

/**
	種別名称を取り出す
*/
const int SiINClassTable::
GetINClassCode	(	string	cFieldName,	// SiNDYフィールド名称
					int		cData_Type,	// データタイプ
					int		cUnder_Flag,// 地上・地下フラグ(0:地上、1:地下）
					int		cSiClass )	// 種別コード
{
	/*
	if(0 == mTable.size())	{
		for(int i=0 ; gClasses[i].eSiClass ; ++i)
		{
			mTable.insert(Peace(gClasses[i].eSiClass, i));
		}
	}
	*/
	Peace	aSearch_Tmp;
	/*char	pName[256];
	int		lsize = cName.size();
	if(lsize > 256) {
		return 0;
	}
	strncpy(pName, &cName[0], 256);
	*/
	 aSearch_Tmp.setData	(cFieldName, cData_Type, cUnder_Flag, cSiClass);
#ifdef	WIN32
	multiset<Peace, lessC>::iterator	i = mTable.find(aSearch_Tmp);
#else
	multiset<Peace>::iterator			i = mTable.find(aSearch_Tmp);
#endif
	if(i == mTable.end()) {
		return 0;
	}
	Peace	ret_tbl	= *i;			// 返すテーブル
	return	(ret_tbl.eINClass);
}

/**
	SiNDYの情報からIN属性の対応情報を取り出す
*/
SiINClassTable::Peace		SiINClassTable::
GetSiToINInfo	(	string	cFieldName,	// SiNDYフィールド名称
					int		cData_Type,	// データタイプ
					int		cUnder_Flag,// 地上・地下フラグ(0:地上、1:地下）
					int		cSiClass )	// 種別コード
{
	Peace	aSearch_Tmp;
	aSearch_Tmp.setData	(cFieldName, cData_Type, cUnder_Flag, cSiClass);
#ifdef	WIN32
	multiset<Peace, lessC>::iterator	i = mTable.find(aSearch_Tmp);
#else
	multiset<Peace>::iterator			i = mTable.find(aSearch_Tmp);
#endif
	Peace	ret_tbl;			// 返すテーブル
	if(i != mTable.end()) {
		// 2007.01.10
		ret_tbl	= *i;
	}
	return	(ret_tbl);
}

/**
	対応テーブル読み込み
*/

/**
	Sindy、IN属性対応テーブルの配列を作成する
	@return 返り値	成功 = 0, 失敗 = -1
*/
int		SiINClassTable::
Read_Table	(	char	*c_RFName)	// Sindy、IN属性対応テーブルファイル名
{
	const	int		bmax		= 256;
	FILE			*pRFp;		
	char			pRBuf[bmax];
	int				aStrLen		= 0;

	pRFp	= fopen(c_RFName, "r");
	if(pRFp == NULL) {
		fprintf	(stderr, "Sindy、IN属性対応テーブルファイル <%s> が開けません!!\n", c_RFName);
		return	(-1);
	}
/**
	if(0 == mTable.size())
	{
		for(int i=0 ; gClasses[i].eSiClass ; ++i)
		{
			mTable.insert(Peace(gClasses[i].eSiClass, i));
		}
	}
	*/
	
	int		i = 0;
	char*	cut_str	= ",\t";
	while(fgets(pRBuf, bmax, pRFp) != NULL)
	{
#ifdef	DB_ROT_RULE	// 2006.10.12
		char		*eDivStr[7] = {NULL, NULL, NULL, NULL, NULL, NULL, NULL};	// SiNDY上のレイヤ名
#else
		char		*eDivStr[6] = {NULL, NULL, NULL, NULL, NULL, NULL};	// SiNDY上のレイヤ名
#endif
		Peace		cur_tbl;	// 追加する属性テーブル

		aStrLen		= strlen(pRBuf);
		// 最後の"\n"を取り除く
		if(pRBuf[aStrLen - 1] == '\n') {
			pRBuf[aStrLen - 1] = '\0';
		}

		// コメント文を飛ばす
		if(pRBuf[0] == '#' || pRBuf[0] == '\0') {
			continue;
		}

		// 文字列を','で分割する
		char**				gomistr = NULL;
		eDivStr[0]			= strtok	(pRBuf, cut_str);
		eDivStr[1]			= strtok	(NULL, cut_str);
		eDivStr[2]			= strtok	(NULL, cut_str);
		eDivStr[3]			= strtok	(NULL, cut_str);
		eDivStr[4]			= strtok	(NULL, cut_str);
		eDivStr[5]			= strtok	(NULL, cut_str);
#ifdef	DB_ROT_RULE	// 2006.10.12
		eDivStr[6]			= strtok	(NULL, cut_str);
#endif

	//	strcpy				(cur_tbl.eSiFieldName, eDivStr[0]);
		cur_tbl.eSiFieldName	= eDivStr[0];
		if(strcmp(eDivStr[1], "POLYGON") == 0) {
			cur_tbl.eData_Type	= TYPE_POLY;
		}else
		if(strcmp(eDivStr[1], "LINE") == 0) {
			cur_tbl.eData_Type	= TYPE_LINE;
		}else
		if(strcmp(eDivStr[1], "POINT") == 0) {
			cur_tbl.eData_Type	= TYPE_POINT;
		}else{
			fprintf	(stderr, "属性対応定義ファイルフォーマットエラー\n");
			return	(-1);
		}
		cur_tbl.eUnder_Flag	= atoi(eDivStr[2]);
		cur_tbl.eSiClass	= atoi(eDivStr[3]);
		cur_tbl.eINClass	= strtoul(eDivStr[4], gomistr, 16);
		strcpy				(cur_tbl.eName, eDivStr[5]);
#ifdef	DB_ROT_RULE	// 2006.10.13
		if( eDivStr[6] != NULL ) {
			strcpy				(cur_tbl.eRot_Rule, eDivStr[6]);
		}
#endif
		cur_tbl.eIndex		= i;

		// セットに追加
		mTable.insert	(cur_tbl);
	}
	fclose(pRFp);
	return(0);
}

/**
	DBから属性対応テーブルを読み込む
	Sindy、IN属性対応テーブルの配列を作成する
	※データレベルを指定して必要とするレコードだけ読み込む
	　全テーブル = 0, 都市地図 = 3x, ベース = 2x, ミドル = 1x
	@return 返り値	成功 = 0, 失敗 = -1
*/
int	SiINClassTable::
Read_DB_Table	(	char*					c_cpLayer_Name,		///< DBのレイヤ名
					IFeatureWorkspacePtr	c_ipWorkSpace,		///< フィーチャワークスペース
					int						c_iData_Level	)	///< データのレベル
{
	IFeatureWorkspacePtr	a_F_Workspace	( c_ipWorkSpace );	// フィーチャワークスペース
//	IFeatureClassPtr		a_ipF_Class;
	ITablePtr				a_ipTable;

	CComBSTR	a_bOpen_Layer_Name	( c_cpLayer_Name );

	// レイヤのオープン
	if( FAILED(a_F_Workspace->OpenTable ( a_bOpen_Layer_Name, &a_ipTable )) ) {
		fprintf	(stderr,	"<%s> レイヤのオープンに失敗\n", c_cpLayer_Name);
		return	(false);
	}else {
		fprintf	(stdout,	"<%s> レイヤをオープン\n", c_cpLayer_Name);
	}

	char		a_tmpstr[30];	//　ここはなんとかしたい気がする

	sprintf	(	a_tmpstr, "OBJECTID <> 0");		// メッシュコードを文字列に

	_bstr_t		a_Where_Str ( a_tmpstr );

	IQueryFilterPtr	a_Sp_Filter		( CLSID_SpatialFilter );		// 空間検索フィルタ
	a_Sp_Filter->put_WhereClause	( a_Where_Str );				// Where句の設定
	
	// クリップ矩形作成
	IEnvelopePtr	a_ipMesh_Rect;	// メッシュ矩形

	// サーチしてくる
//	IFeatureCursorPtr	a_F_Cursor;
	_ICursorPtr			a_ipCursor;
	if( FAILED	(a_ipTable->Search	( a_Sp_Filter, VARIANT_FALSE, &a_ipCursor )) ) {
		fprintf	(stderr, "データの属性検索に失敗\n");
		return	( -1 );
	}else {
		_IRowPtr				a_ipRow;
		long					a_lSp_InitFlag	= 0;	// 空間参照を初期化したかどうかのフラグ
		int i = 0;
		for(i=0;;i++)
		{
			if( a_ipCursor->NextRow ( &a_ipRow ) != S_OK) {	// データをひとつ取り出す
				break;
			}

			Peace	cur_attr;
			
			//　属性と、フェッチしたシェイプをとってくる
			if( !cur_attr.Set_Data	( a_ipRow, c_iData_Level ) ) continue;
			// 注記記号用の処理
			if( cur_attr.eData_Type	== TYPE_POINT ) {
				cur_attr.eSiFieldName	= AN_CLS_C;	// 注記記号用のフィールド名を追加しておく
			}
			// セットに追加
			mTable.insert	(cur_attr);
		}
	}

	// 都市地図背景のときだけ
	if ( c_iData_Level == city ) {
		// 街区用のテーブルを追加で用意
		int i = 0;
		for( i = 0; i < 6; i++ )
		{
			Peace					a_ptmpattr;
			a_ptmpattr.eSiFieldName	= "BGCLASS_C";
			a_ptmpattr.eIndex		= i+100;
			a_ptmpattr.eINClass		= 0x01015000 + 0x100*(i+1);
			strcpy (a_ptmpattr.eName, "街区色");
			a_ptmpattr.eData_Type	= TYPE_POLY;
			a_ptmpattr.eSiClass		= i+1;
			a_ptmpattr.eUnder_Flag	= 0;
			mTable.insert ( a_ptmpattr );
		}
	}

	// ベースマップの場合フェリー用のテーブルを作成 2003.08.02
	if ( c_iData_Level == base ) {
		int	i = 0;
		for ( i = 2; i < 5; i++ )
		{
			Peace					a_ptmpattr;
			a_ptmpattr.eSiFieldName	= "ROADCLASS_C";
			a_ptmpattr.eIndex		= i+300;	// 値はダミー
			a_ptmpattr.eINClass		= 0x0500f100;
			strcpy (a_ptmpattr.eName, "");
			a_ptmpattr.eData_Type	= TYPE_LINE;
			a_ptmpattr.eSiClass		= 200+i;
			a_ptmpattr.eUnder_Flag	= 0;
			mTable.insert ( a_ptmpattr );
		}
	}

	//行政界用ダミー
	{
		Peace					a_ptmpattr;
		a_ptmpattr.eSiFieldName	= "D_ADMCLASS_C";
		a_ptmpattr.eIndex		= 350;	// 値はダミー
		a_ptmpattr.eINClass		= 0x00000000;
		strcpy (a_ptmpattr.eName, "市区町村界／字・町丁目界");
		a_ptmpattr.eData_Type	= TYPE_POLY;
		a_ptmpattr.eSiClass		= 0;
		a_ptmpattr.eUnder_Flag	= 0;
		mTable.insert ( a_ptmpattr );
	}

	// 建物装飾線用ダミー
	{
		Peace					a_ptmpattr;
		a_ptmpattr.eSiFieldName	= "D_DCLCLASS_C";
		a_ptmpattr.eIndex		= 360;	// 値はダミー
		a_ptmpattr.eINClass		= 0x00000000;
		strcpy (a_ptmpattr.eName, "建物装飾線");
		a_ptmpattr.eData_Type	= TYPE_LINE;
		a_ptmpattr.eSiClass		= 0;
		a_ptmpattr.eUnder_Flag	= 0;
		mTable.insert ( a_ptmpattr );
	}

	// 建物用
	{
		Peace					a_ptmpattr;
		a_ptmpattr.eSiFieldName	= "BLDCLASS_C";
		a_ptmpattr.eIndex		= 370;	// 値はダミー
		a_ptmpattr.eINClass		= 0x00000000;
		strcpy (a_ptmpattr.eName, "未調査・作業中");
		a_ptmpattr.eData_Type	= TYPE_POLY;
		a_ptmpattr.eSiClass		= 0;
		a_ptmpattr.eUnder_Flag	= 0;
		mTable.insert ( a_ptmpattr );

	}

	return	( a_ipMesh_Rect );
}
