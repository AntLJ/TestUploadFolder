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
**	SiINClassTable.h
**
**		IN class table class  header file
**
**		Programmed by H.Miyahara
**
**		EDITION:
**			1999/4/13	zcl から編集
*/

#ifndef	___IN_CLASS_TABLE___
#define	___IN_CLASS_TABLE___

//#include "SiZIN.h"
#include <string>
#include <vector>
#include <set>
#include "stdafx.h"

using	namespace	std;

#define		TYPE_POLY	2
#define		TYPE_LINE	1
#define		TYPE_POINT	0

/**
	SiNDYの属性と、INデータ種別の対応表を作成するクラス
*/
class SiINClassTable
{
public:
	/**
		読み込みレベルの定義
	*/
	enum	Read_Lv	{
		all	= 0,
		mid,
		base,
		city,
		top
	};
public:
	struct Peace
	{
	//	char	eSiFieldName[11];	///< SiNDY上のレイヤ名
		string	eSiFieldName;
		int		eData_Type;			///< データタイプ
		int		eUnder_Flag;		///< 地上、地下フラグ
		int		eSiClass;			///< SiNDY種別コード
		int		eINClass;			// IN種別
		char	eName[128];			// 種別名称
		int		m_Top_ExF;			///< トップマップ存在フラグ 2010.08.18
		int		m_Mid_ExF;			///< ミドルマップ存在フラグ 2010.08.18
		int		m_Base_ExF;			///< ベースマップ存在フラグ 2010.08.18
		int		m_City_ExF;			///< 都市地図存在フラグ 2010.08.18
#ifdef	DB_ROT_RULE	// 2006.10.12
		char	eRot_Rule[12];		///< 注記の回転ルール 2006.10.11
#endif
		int		eIndex;

		Peace	(void) {
			eData_Type	= TYPE_POINT;	// ポイントで初期化
			eUnder_Flag	= 0;
			eSiClass	= 0;
			eINClass	= 0;
			memset ( eName, '\0', 128 );
#ifdef	DB_ROT_RULE	// 2006.10.12
			memset ( eRot_Rule, '\0', 12 );	// 2006.10.11
#endif
			m_Top_ExF	= 0;
			m_Mid_ExF	= 0;
			m_Base_ExF	= 0;
			m_City_ExF	= 0;
			eIndex		= 0;
		}
		Peace	(int cClass) : eSiClass(cClass) { }
		Peace	(int cClass, int cIndex) : eSiClass(cClass), eIndex(cIndex) { }

		void	setData	(string c_FieldName, int cData_Type, int cUnder_Flag, int cSiClass) {
				//strcpy		(eSiFieldName, c_FieldName);
				eSiFieldName= c_FieldName;
				eData_Type	= cData_Type;
				eUnder_Flag	= cUnder_Flag;
				eSiClass	= cSiClass;
		}
		void	setData (string c_FieldName, char* c_Name){
				eSiFieldName	= c_FieldName;
			//	strcpy	(eSiFieldName, c_FieldName);
				strcpy_s(eName, c_Name);
		}
		/**
			フィーチャからデータをセットする
			※全テーブル = 0, 都市地図 = 3x, ベース = 2x, ミドル = 1x
			@return	成功	true
			@return	失敗	false
		*/
		bool	Set_Data	(
					_IRowPtr		c_Feature,			///< フェッチしてきたフィーチャ[IN]
					int			c_iData_Level		///< データのレベル	　
				);
		/**
			Unicodeの文字列からSJISにする
		*/
		string	Str_to_SJIS		(
					CComBSTR*	c_CStr		// 文字列(Unicode)
				);
		/**
			レイヤIDから、属性のフィールド名とデータタイプ獲得する
			@return	成功	true
			@return	失敗	false
		*/
		bool	Get_Field_DType	(
					int			c_sLayer_ID,	///< レイヤID[IN]
					string*		c_spFName,		///< フィールド名[OUT]
					int*		c_ipDType		///< データタイプ[OUT]
				);
	};

	
	// 比較関数
	/*
	struct lessC : public binary_function<Peace, Peace, bool>
	{
		bool operator () (const Peace cPeace1, const Peace cPeace2) const
		{
			return cPeace1.eSiClass < cPeace2.eSiClass;
		}
	};
	*/

	struct lessC : public binary_function<Peace, Peace, bool>
	{
		// SiNDY上のレイヤ名と、SiNDY上の種別コードだけの比較でユニークになるはず
		int operator () (const Peace cPeace1, const Peace cPeace2) const
		{
			int	ret	= 0;
			/*
			int	cmp = strcmp(cPeace1.eSiFieldName, cPeace2.eSiFieldName);	// レイヤ名称
			if(cmp != 0) {
				if(cmp < 0) {
					ret	= 1;
				}
			}else
			*/
			if( cPeace1.eSiFieldName != cPeace2.eSiFieldName ) {
				if( cPeace1.eSiFieldName < cPeace2.eSiFieldName ) {
					ret	= 1;
				}
			}else
			if( cPeace1.eData_Type != cPeace2.eData_Type ) {		// データタイプ
				if( cPeace1.eData_Type < cPeace2.eData_Type ) {
					ret	= 1;
				}
			}else
			if( cPeace1.eUnder_Flag != cPeace2.eUnder_Flag ) {		// 地上、地下フラグ
				if( cPeace1.eUnder_Flag < cPeace2.eUnder_Flag ) {
					ret	= 1;
				}
			}else
			if(cPeace1.eSiClass != cPeace2.eSiClass) {
				if(cPeace1.eSiClass < cPeace2.eSiClass) {
					ret	= 1;
				}	
			}
			return (ret);
		}
	};
public:
	static multiset<Peace, lessC>	mTable;
	//static set<Peace>	mTable;
public:

	SiINClassTable	() {
	}
	/*
	static const char*	GetName			(char *cFieldName, int cClass );
	*/
	//static const int	GetINClassCode	(char *cFieldName, int cData_Type, int cUnder_Flag, int cSiClass );
	static const int	GetINClassCode	(string cFieldName, int cData_Type, int cUnder_Flag, int cSiClass );

	/**
		SiNDYの情報からIN属性の対応情報を取り出す
	*/
	Peace				GetSiToINInfo	(
							string		cFieldName,
							int			cData_Type,
							int			cUnder_Flag,
							int			cSiClass
						);

	/**
		Sindy、IN属性対応テーブルの配列を作成する
		@return 返り値	成功 = 0, 失敗 = -1
	*/
	 int	Read_Table		(
				char	*c_RFName
			);

	 /**
		DBから属性対応テーブルを読み込む
		Sindy、IN属性対応テーブルの配列を作成する
		※データレベルを指定して必要とするレコードだけ読み込む
		　全テーブル = 0, 都市地図 = 3x, ベース = 2x, ミドル = 1x
		@return 返り値	成功 = 0, 失敗 = -1
	 */
	int		Read_DB_Table	(
				char					*c_cpLayer_Name,		///< レイヤ名称
				IFeatureWorkspacePtr	c_ipWorkSpace,			///< フィーチャワークスペース
				int						c_iData_Level			///< データのレベル
			);

#ifdef	DB_ROT_RULE	// 2006.10.18
	/**
		属性テーブルがROT_RULEフィールドの情報を取得してきているかのチェック
		@return	bool
	*/
	bool	Read_ROT_RULE	( void ) {
				Peace	a_pFirstRec	= *(mTable.begin());
				if( a_pFirstRec.eRot_Rule[0] == '\0' ) {
					return ( false );
				}
				return ( true );
			}
#endif

};

#endif	// ___IN_CLASS_TABLE___

