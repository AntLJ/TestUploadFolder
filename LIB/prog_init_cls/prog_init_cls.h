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

// 2002.06.05
// ツールの初期化及び設定に関しての処理を行う。
/**
	2002.10.18	行政界が中縮尺、都市地図に分かれたので対応
*/

/// 接続定義
#define		DB_MAP		"DB_MAP"			///< 地図系DB接続
#define		DB_ADDRESS	"DB_ADDRESS"		///< 住所系DB接続 
#define		DB_BASE		"DB_BASE"			///< 中縮尺DB接続
#define		DB_ROAD		"DB_ROAD"			///< 道路系DB接続 [bug 7066]への対応 2009.07.02
#define		DB_ANNO		"DB_ANNO"			///< 注記系DB接続 cityとanno,poiの分離対応 2014.05.07
#define		DB_ANY		"DB_ANY"			///< 汎用

/// レイヤ定義
#define		INI_UN		"USERNAME"			///< ユーザ名
#define		INI_PW		"PASSWORD"			///< パスワード
#define		INI_IT		"INSTANCE"			///< インスタンス
#define		INI_SV		"DB_CONNECTION_PROPERTIES"			///< サーバ名
#define		INI_VER		"VERSION"			///< バージョン

#define		INI_OPATH	"OUTPUT_PATH"		///< データ出力パス名

#define		INI_CMESH	"CITYMESH"			///< 都市地図メッシュレイヤ

#define		INI_CNOTE	"CITY_ANNOTATION"	///< 都市地図注記ポイントレイヤ
#define		INI_CNLINE	"CITY_DISP_LINE"	///< 都市地図注記ラインレイヤ
#define		INI_CHIBAN	"CITY_REP_POINT"	///< 地番注記レイヤ

#define		INI_CLINE	"CITY_LINE"			///< 都市地図背景アークレイヤ
#define		INI_CRWAY	"CITY_RAILWAY"		///< 都市地図鉄道レイヤ
#define		INI_CSITE	"CITY_SITE"			///< 都市地図街区レイヤ
#define		INI_CSTA	"CITY_STATION"		///< 都市地図駅舎レイヤ
#define		INI_BLD		"BUILDING"			///< 都市地図建物レイヤ
#define		INI_BLDL	"BUILDING_LINE"		///< 都市地図建物アークレイヤ
#define		INI_STEP	"BUILDING_STEP"		///< 都市地図kplレイヤ
#define		INI_ROOF	"BUILDING_ROOF"		///< 都市地図建物屋上施設形状レイヤ
#define		INI_TUNNEL	"CITY_TUNNEL"		///< 都市地図トンネル坑口形状レイヤ

#define		INI_GOU		"GOU_POINT"			///< 号ポイント

#define		INI_ADM		"ADMIN"				///< 行政界レイヤ いずれ使用？
#define		INI_BADM	"BASE_ADMIN"		///< 中縮尺行政界
#define		INI_CADM	"CITY_ADMIN"		///< 都市地図行政界

#define		INI_BMESH	"BASEMESH"			///< 中縮尺メッシュレイヤ

#define		INI_BNL1	"B_SC1DISP_LINE"	///< 中縮尺注記ラインレイヤ１
#define		INI_BNL2	"B_SC2DISP_LINE"	///< 中縮尺注記ラインレイヤ２
#define		INI_BNL3	"B_SC3DISP_LINE"	///< 中縮尺注記ラインレイヤ３
#define		INI_BNL4	"B_SC4DISP_LINE"	///< 中縮尺注記ラインレイヤ４
#define		INI_BNOTE	"BASE_ANNOTATION"	///< 中縮尺注記

#define		INI_BCNT	"BASE_CONTOUR"		///< 段彩レイヤ
#define		INI_BLINE	"BASE_LINE"			///< 中縮尺背景アークレイヤ
#define		INI_BRWAY	"BASE_RAILWAY"		///< 中縮尺鉄道レイヤ
#define		INI_BSITE	"BASE_SITE"			///< 中縮尺背景、施設レイヤ
#define		INI_BSTA	"BASE_STATION"		///< 中縮尺駅舎レイヤ

#define		INI_MMESH	"MIDDLEMESH"		///< ミドルメッシュレイヤ

#define		INI_MNL1	"M_SC1DISP_LINE"	///< 中縮尺注記ラインレイヤ１
#define		INI_MNL2	"M_SC2DISP_LINE"	///< 中縮尺注記ラインレイヤ２
#define		INI_MNL3	"M_SC3DISP_LINE"	///< 中縮尺注記ラインレイヤ３
#define		INI_MNL4	"M_SC4DISP_LINE"	///< 中縮尺注記ラインレイヤ４
#define		INI_MNOTE	"MIDDLE_ANNOTATION"	///< 中縮尺注記

#define		INI_MCNT	"MIDDLE_CONTOUR"	///< 段彩レイヤ
#define		INI_MLINE	"MIDDLE_LINE"		///< 中縮尺背景アークレイヤ
#define		INI_MSITE	"MIDDLE_SITE"		///< 中縮尺背景、施設レイヤ

#define		INI_TMESH	"TOPMESH"

// トップマップ注記用
#define		INI_TNL1	"T_SC1DISP_LINE"	///< 中縮尺注記ラインレイヤ１
#define		INI_TNL2	"T_SC2DISP_LINE"	///< 中縮尺注記ラインレイヤ２
#define		INI_TNL3	"T_SC3DISP_LINE"	///< 中縮尺注記ラインレイヤ３
#define		INI_TNL4	"T_SC4DISP_LINE"	///< 中縮尺注記ラインレイヤ４
#define		INI_TNOTE	"TOP_ANNOTATION"	///< 中縮尺注記
// トップマップ用 2003.01.05
#define		INI_TCNT	"TOP_CONTOUR"		///< 段彩レイヤ
#define		INI_TLINE	"TOP_LINE"			///< トップマップ背景アークレイヤ
#define		INI_TSITE	"TOP_SITE"			///< トップマップ背景、施設レイヤ

#define		INI_RLINK	"ROAD_LINK"			///< 道路リンクレイヤ

#define		INI_PAREA	"PAREA"				///< PAREA代表点レイヤ
#define		INI_BRP		"BASE_REP_POINT"	///< 中縮尺Addrポイント

#define		INI_POI		"POI_POINT"			///< POI_POINT

// 属性テーブル用
#define		INI_BACK_ATTR	"BACKGROUND_CLASS"	///< 背景種別用テーブル
#define		INI_ANNO_ATTR	"ANNOTATION_CLASS"	///< 注記種別用テーブル

typedef	std::set<int>												s_int;		///< メッシュリスト用
typedef	std::set<int>::iterator										is_int;		///< メッシュリスト用イテレータ
typedef	std::set<std::string, std::less<std::string> >				s_charp;	///< 汎用リスト用(文字列リスト）
typedef	std::set<std::string, std::less<std::string> >::iterator	is_charp;	///< 汎用リスト用(文字列リスト）

/**
	よく使用する、メッシュや行政コード等のリストを読み込む用に作成した
	クラス
	2002.8.19 行政コード等の読み込みも出来るように改造
*/
class	mk_input_list
{
	private:
		int			mesh_digit;	///< メッシュの桁数
		s_int		mesh_set;	///< メッシュのリスト
		is_int		mesh_indx;	///< メッシュリストのインデックス
		s_charp		data_set;	///< 読み込んだデータ列
		is_charp	data_indx;	///< データ列のインデックス
	public:
		/**
			コンストラクタ
		*/
		mk_input_list	()	{
			mesh_digit		= 0;
			mesh_set.clear();
			mesh_indx		= mesh_set.begin();
			data_set.clear();
			data_indx		= data_set.begin();
		}

		/**
			デストラクタ
		*/
		~mk_input_list	()	{
			// 汎用リストは、メモリのアロケートをやっているので、解放しないとだめ
		/*	for(data_indx = data_set.begin(); data_indx != data_set.end(); data_indx++)
			{
				if(*data_indx != NULL) {
					delete [] *data_indx;	// メモリの解放
				}
			}*/
		}

		/**
			メッシュリストの読み込み
		*/
		int		read_mesh_list	(	char	*c_cFile	);

		/**
			メッシュリストの読み込み
			※指定したメッシュを１つ１つ読み込んでいく
		*/
		int		read_mesh_list	(	int		c_iMesh	);

		/**
			サイズを返す
			@return		成功	0　失敗　-1
		*/
		int		get_size	() {
					return	(mesh_set.size());
				}

		/**
			メッシュリストの最初のメッシュを返す
		*/
		int		get_mesh_first	() {
					mesh_indx	= mesh_set.begin();
					int	mesh	= *mesh_indx;
					return	(mesh);
				}
		/**
			メッシュリストの次のメッシュを返す
		*/
		int		get_mesh_next	() {
					mesh_indx++;
					if(mesh_indx == mesh_set.end()) {
						return	(-1);
					}
					int	mesh	= *mesh_indx;
					return	(mesh);
				}
		/**
			メッシュリストの桁数を返す
		*/
		int		get_mesh_digit	() {
					return	( mesh_digit );
				}

		//--------------------------------------------------
		/**
			データリストの読み込み
		*/
		int		read_data_list	(	char	*c_cFile	);

		/** 2007.06.28 追加
			文字列レコードの追加
		*/
		void	put_str_record	(	char	*c_cStr	);

		/**
			データ列の最初のデータを返す
		*/
		is_charp	get_indx_first	() {
						data_indx	= data_set.begin();
						return	( data_indx );
					}

		/**
			データ列の次のデータを返す
		*/
		is_charp	get_indx_next	() {
						data_indx++;
						if( data_indx == data_set.end() ) {
							return	( data_indx );
						}
						return	( data_indx );
					}
		/**
			データ列の終了判定
		*/
		bool		is_indx_end	() {
						if( data_indx == data_set.end() ) {
							return	( true );
						}
						return	( false );
					}
		//--------------------------------------------------

};

/**
	プログラム初期化用のクラス
*/

class	prog_init
{
	///=共通DB接続設定に対応するために追加
	std::string		e_sUserName_Map;		///< ユーザ名
	std::string		e_sPassword_Map;		///< パスワード
	std::string		e_sVersion_Map;			///< バージョン
	std::string		e_sInstance_Map;		///< インスタンス
	std::string		e_sServer_Map;			///< サーバ名
	std::string		e_sDataFile_Path_Map;	///< データファイル(PGDB等)のパス 2007.03.01

	std::string		e_sUserName_Address;	///< ユーザ名
	std::string		e_sPassword_Address;	///< パスワード
	std::string		e_sVersion_Address;		///< バージョン
	std::string		e_sInstance_Address;	///< インスタンス
	std::string		e_sServer_Address;		///< サーバ名
	std::string		e_sDataFile_Path_Address;///< データファイル(PGDB等)のパス 2007.03.01

	std::string		e_sUserName_Base;		///< ユーザ名
	std::string		e_sPassword_Base;		///< パスワード
	std::string		e_sVersion_Base;		///< バージョン
	std::string		e_sInstance_Base;		///< インスタンス
	std::string		e_sServer_Base;			///< サーバ名
	std::string		e_sDataFile_Path_Base;	///< データファイル(PGDB等)のパス 2007.03.01

	std::string		e_sUserName_Road;		///< ユーザ名 [bug 7066]への対応 2009.07.02
	std::string		e_sPassword_Road;		///< パスワード [bug 7066]への対応 2009.07.02
	std::string		e_sVersion_Road;		///< バージョン [bug 7066]への対応 2009.07.02
	std::string		e_sInstance_Road;		///< インスタンス [bug 7066]への対応 2009.07.02
	std::string		e_sServer_Road;			///< サーバ名 [bug 7066]への対応 2009.07.02
	std::string		e_sDataFile_Path_Road;	///< データファイル(PGDB等)のパス [bug 7066]への対応 2009.07.02

	std::string		e_sUserName_Anno;		///< ユーザ名
	std::string		e_sPassword_Anno;		///< パスワード
	std::string		e_sVersion_Anno;		///< バージョン
	std::string		e_sInstance_Anno;		///< インスタンス
	std::string		e_sServer_Anno;			///< サーバ名
	std::string		e_sDataFile_Path_Anno;	///< データファイル(PGDB等)のパス

	std::string		e_sUserName_Any;		///< ユーザ名
	std::string		e_sPassword_Any;		///<パスワード
	std::string		e_sVersion_Any;			///< バージョン
	std::string		e_sInstance_Any;		///< インスタンス
	std::string		e_sServer_Any;			///< サーバ名
	std::string		e_sDataFile_Path_Any;	///< データファイル(PGDB等)のパス 2007.03.01

	///=[ここの設定は既存を生かすための設定========================
	std::string		e_sUserName;		///< ユーザ名
	std::string		e_sPassword;		///<パスワード
	std::string		e_sVersion;			///< バージョン
	std::string		e_sInstance;		///< インスタンス
	std::string		e_sServer;			///< サーバ名
	std::string		e_sDataFile_Path;	///< データファイル(PGDB等)のパス 2007.03.01

//	std::string		e_sOutput_Path;		///< データ出力パス名

	std::string		e_sCity_Mesh;		///< 都市地図メッシュレイヤ

	std::string		e_sCity_Note;		///< 都市地図注記ポイントレイヤ
	std::string		e_sCity_Note_Line;	///< 都市地図注記ラインレイヤ
	std::string		e_sChiban;			///< 地番注記レイヤ

	std::string		e_sCity_Line;		///< 都市地図背景アークレイヤ
	std::string		e_sCity_RailWay;	///< 都市地図鉄道レイヤ
	std::string		e_sCity_Site;		///< 都市地図街区レイヤ
	std::string		e_sCity_Station;	///< 都市地図駅舎レイヤ
	std::string		e_sCity_Building;	///< 都市地図建物レイヤ
	std::string		e_sCity_Build_Line;	///< 都市地図建物アークレイヤ
	std::string		e_sCity_Build_Step;		///< 都市地図kplレイヤ
	std::string     e_sCity_Building_Roof;  ///< 都市建物屋上施設ポリゴン
	std::string     e_sCity_Tunnel;     ///< 都市トンネル坑口ポリゴン

	std::string		e_sGou_Point;		///< 号ポイント

	std::string		e_sAdmin;			///< 行政界レイヤ
	std::string		e_sBase_Admin;		///< 中縮尺行政界レイヤ
	std::string		e_sCity_Admin;		///< 都市地図行政界レイヤ

	std::string		e_sBase_Mesh;		///< 中縮尺メッシュレイヤ

	std::string		e_sBase_Note_Line1;	///< 中縮尺注記ラインレイヤ１
	std::string		e_sBase_Note_Line2;	///< 中縮尺注記ラインレイヤ２
	std::string		e_sBase_Note_Line3;	///< 中縮尺注記ラインレイヤ３
	std::string		e_sBase_Note_Line4;	///< 中縮尺注記ラインレイヤ４

	std::string		e_sBase_Note;		///< 中縮尺注記
	std::string		e_sBase_Contour;	///< 段彩レイヤ
	std::string		e_sBase_Line;		///< 中縮尺背景アークレイヤ
	std::string		e_sBase_Railway;	///< 中縮尺鉄道レイヤ
	std::string		e_sBase_Site;		///< 中縮尺背景、施設レイヤ
	std::string		e_sBase_Station;	///< 中縮尺駅舎レイヤ

	std::string		e_sMiddle_Mesh;		///< ミドルメッシュレイヤ

	std::string		e_sMiddle_Note_Line1;	///< ミドル注記ラインレイヤ１
	std::string		e_sMiddle_Note_Line2;	///< ミドル注記ラインレイヤ２
	std::string		e_sMiddle_Note_Line3;	///< ミドル注記ラインレイヤ３
	std::string		e_sMiddle_Note_Line4;	///< ミドル注記ラインレイヤ４

	std::string		e_sMiddle_Note;		///< ミドル注記
	std::string		e_sMiddle_Contour;	///< 段彩レイヤ
	std::string		e_sMiddle_Line;		///< ミドル背景アークレイヤ
	std::string		e_sMiddle_Site;		///< ミドル背景、施設レイヤ

	// トップマップ用 2004.01.05

	std::string		e_sTop_Mesh;

	std::string		e_sTop_Contour;		///< 段彩レイヤ
	std::string		e_sTop_Line;		///< トップ背景アークレイヤ
	std::string		e_sTop_Site;		///< トップ背景、施設レイヤ
	// トップマップ注記用 2004.02.13
	std::string		e_sTop_Note_Line1;	///< トップ注記ラインレイヤ１
	std::string		e_sTop_Note_Line2;	///< トップ注記ラインレイヤ２
	std::string		e_sTop_Note_Line3;	///< トップ注記ラインレイヤ３
	std::string		e_sTop_Note_Line4;	///< トップ注記ラインレイヤ４
	std::string		e_sTop_Note;		///< トップ注記

	std::string		e_sRoad_Link;		///< 道路リンクレイヤ
	std::string		e_sParea;			///< PAREA代表点レイヤ
	std::string		e_sBase_Rep_Point;	///< 中縮尺Addrポイント

	std::string		e_sPOI_Point;		///< POI_Point [bug 9264]対応

	std::string		e_sBackground_Class;	///< 背景種別用テーブル
	std::string		e_sAnnotation_Class;	///< 注記種別用テーブル
	///=[ここまでの設定は既存を生かすための設定====================

	/** 2007.10.25
		FCをつけた環境変数に変換
	*/
	char*	Add_FC	( char*	c_cpEnvStr )	///< 環境変数
	{
		static char	a_cpTmp[256];
		memset	( a_cpTmp, '\0', 256 );
		sprintf	( a_cpTmp, "FC_%s", c_cpEnvStr );
		return	( a_cpTmp );
	}

public:
	prog_init	()	{}	///< コンストラクタ
	~prog_init	()	{}	///< デストラクタ
	

	//　共通設定用に追加 ================================
	//----<地図系DB用>----------------------------------
	char*	get_UserName_Map() {		///< ユーザ名
				return	(&e_sUserName_Map[0]);
			}
	char*	get_Password_Map() {		///<パスワード
				return	(&e_sPassword_Map[0]);
			}
	char*	get_Instance_Map() {		///< インスタンス
				return	(&e_sInstance_Map[0]);
			}
	char*	get_Server_Map	() {		///< サーバ名
				return	(&e_sServer_Map[0]);
			}
	char*	get_Version_Map	() {		///< バージョン
				return	(&e_sVersion_Map[0]);
			}
	char*	get_DataFile_Path_Map	() {///< データファイル(PGDB等)のパス 2007.03.01
				return	(&e_sDataFile_Path_Map[0]);
			}
	//----<住所系DB用>----------------------------------
	char*	get_UserName_Address() {		///< ユーザ名
				return	(&e_sUserName_Address[0]);
			}
	char*	get_Password_Address() {		///<パスワード
				return	(&e_sPassword_Address[0]);
			}
	char*	get_Instance_Address() {		///< インスタンス
				return	(&e_sInstance_Address[0]);
			}
	char*	get_Server_Address	() {		///< サーバ名
				return	(&e_sServer_Address[0]);
			}
	char*	get_Version_Address	() {		///< バージョン
				return	(&e_sVersion_Address[0]);
			}
	char*	get_DataFile_Path_Address () {	///< データファイル(PGDB等)のパス 2007.03.01
				return	(&e_sDataFile_Path_Address[0]);
			}
	//----<中縮尺系DB用>----------------------------------
	char*	get_UserName_Base() {		///< ユーザ名
				return	(&e_sUserName_Base[0]);
			}
	char*	get_Password_Base() {		///<パスワード
				return	(&e_sPassword_Base[0]);
			}
	char*	get_Instance_Base() {		///< インスタンス
				return	(&e_sInstance_Base[0]);
			}
	char*	get_Server_Base	() {		///< サーバ名
				return	(&e_sServer_Base[0]);
			}
	char*	get_Version_Base	() {	///< バージョン
				return	(&e_sVersion_Base[0]);
			}
	char*	get_DataFile_Path_Base () {	///< データファイル(PGDB等)のパス 2007.03.01
				return	(&e_sDataFile_Path_Base[0]);
			}
	//----<道路系DB用>----------------------------------
	// [Bug 7066]への対応 2009.07.02
	char*	get_UserName_Road() {		///< ユーザ名
				return	(&e_sUserName_Road[0]);
			}
	char*	get_Password_Road() {		///<パスワード
				return	(&e_sPassword_Road[0]);
			}
	char*	get_Instance_Road() {		///< インスタンス
				return	(&e_sInstance_Road[0]);
			}
	char*	get_Server_Road	() {		///< サーバ名
				return	(&e_sServer_Road[0]);
			}
	char*	get_Version_Road	() {	///< バージョン
				return	(&e_sVersion_Road[0]);
			}
	char*	get_DataFile_Path_Road () {	///< データファイル(PGDB等)のパス
				return	(&e_sDataFile_Path_Road[0]);
			}
	//----<注記系DB用>----------------------------------
	// cityとanno,poiの分離対応 2014/05/07
	char*	get_UserName_Anno() {		///< ユーザ名
				return	(&e_sUserName_Anno[0]);
			}
	char*	get_Password_Anno() {		///<パスワード
				return	(&e_sPassword_Anno[0]);
			}
	char*	get_Instance_Anno() {		///< インスタンス
				return	(&e_sInstance_Anno[0]);
			}
	char*	get_Server_Anno	() {		///< サーバ名
				return	(&e_sServer_Anno[0]);
			}
	char*	get_Version_Anno	() {	///< バージョン
				return	(&e_sVersion_Anno[0]);
			}
	char*	get_DataFile_Path_Anno () {	///< データファイル(PGDB等)のパス
				return	(&e_sDataFile_Path_Anno[0]);
			}
	//==================================================
	//----<既存設定又は汎用接続DB用>----------------------------------
	char*	get_UserName	() {	///< ユーザ名
				return	(&e_sUserName[0]);
			}
	char*	get_Password	() {		///<パスワード
				return	(&e_sPassword[0]);
			}
	char*	get_Instance	() {		///< インスタンス
				return	(&e_sInstance[0]);
			}
	char*	get_Server	() {			///< サーバ名
				return	(&e_sServer[0]);
			}
	char*	get_Version	() {			///< バージョン
				return	(&e_sVersion[0]);
			}
	char*	get_DataFile_Path	() {	///< データファイル(PGDB等)のパス 2007.03.01
				return	(&e_sDataFile_Path[0]);
			}
	char*	get_City_Mesh	() {		///< 都市地図メッシュレイヤ
				return	(&e_sCity_Mesh[0]);
			}
	char*	get_City_Note	() {		///< 都市地図注記ポイントレイヤ
				return	(&e_sCity_Note[0]);
			}
	char*	get_City_Note_Line	() {	///< 都市地図注記ラインレイヤ
				return	(&e_sCity_Note_Line[0]);
			}
	char*	get_Chiban		() {
				return	(&e_sChiban[0]);
			}
	char*	get_City_Line	() {		///< 都市地図背景アークレイヤ
				return	(&e_sCity_Line[0]);
			}
	char*	get_City_RailWay	() {	///< 都市地図鉄道レイヤ
				return	(&e_sCity_RailWay[0]);
			}
	char*	get_City_Site	() {		///< 都市地図街区レイヤ
				return	(&e_sCity_Site[0]);
			}
	char*	get_City_Station	() {	///< 都市地図駅舎レイヤ
				return	(&e_sCity_Station[0]);
			}
	char*	get_City_Building() {		///< 都市地図建物レイヤ
				return	(&e_sCity_Building[0]);
			}
	char*	get_City_Building_Roof() {  ///< 都市建物屋上施設ポリゴン
	            return ( &e_sCity_Building_Roof[0]);
	        }
	char*	get_City_Tunnel() {         ///< 都市トンネル坑口ポリゴン
	            return ( &e_sCity_Tunnel[0]);
	        }
	char*	get_Gou_Point() {			///< 号ポイントレイヤ 2003.7.18
				return	(&e_sGou_Point[0]);
			}
	char*	get_City_Build_Line() {		///< 都市地図建物アークレイヤ
				return	(&e_sCity_Build_Line[0]);
			}
	char*	get_City_Build_Step	() {	///< 都市地図kplレイヤ
				return	(&e_sCity_Build_Step[0]);
			}
	char*	get_Admin	() {			///< 行政界レイヤ
				return	(&e_sAdmin[0]);
			}
	char*	get_Base_Admin	() {		///< 中縮尺行政界レイヤ
				return	(&e_sBase_Admin[0]);
			}
	char*	get_City_Admin	() {		///< 都市地図行政界レイヤ
				return	(&e_sCity_Admin[0]);
			}
	char*	get_Base_Mesh	() {		///< 中縮尺メッシュレイヤ
				return	(&e_sBase_Mesh[0]);
			}
	char*	get_Base_Note_Line1	() {	///< 中縮尺注記ラインレイヤ１
				return	(&e_sBase_Note_Line1[0]);
			}
	char*	get_Base_Note_Line2	() {	///< 中縮尺注記ラインレイヤ２
				return	(&e_sBase_Note_Line2[0]);
			}
	char*	get_Base_Note_Line3	() {	///< 中縮尺注記ラインレイヤ３
				return	(&e_sBase_Note_Line3[0]);
			}
	char*	get_Base_Note_Line4	() {	///< 中縮尺注記ラインレイヤ４
				return	(&e_sBase_Note_Line4[0]);
			}
	char*	get_Base_Note	() {		///< 中縮尺注記
				return	(&e_sBase_Note[0]);
			}
	char*	get_Base_Contour	() {	///< 段彩レイヤ
				return	(&e_sBase_Contour[0]);
			}
	char*	get_Base_Line	() {		///< 中縮尺背景アークレイヤ
				return	(&e_sBase_Line[0]);
			}
	char*	get_Base_Railway	() {	///< 中縮尺鉄道レイヤ
				return	(&e_sBase_Railway[0]);
			}
	char*	get_Base_Site	() {		///< 中縮尺背景、施設レイヤ
				return	(&e_sBase_Site[0]);
			}
	char*	get_Base_Station	() {	///< 中縮尺駅舎レイヤ
				return	(&e_sBase_Station[0]);
			}
	char*	get_Middle_Mesh	() {		///< ミドルメッシュレイヤ
				return	(&e_sMiddle_Mesh[0]);
			}
	char*	get_Middle_Note_Line1	() {	///< ミドル注記ラインレイヤ１
				return	(&e_sMiddle_Note_Line1[0]);
			}
	char*	get_Middle_Note_Line2	() {	///< ミドル注記ラインレイヤ２
				return	(&e_sMiddle_Note_Line2[0]);
			}
	char*	get_Middle_Note_Line3	() {	///< ミドル注記ラインレイヤ３
				return	(&e_sMiddle_Note_Line3[0]);
			}
	char*	get_Middle_Note_Line4	() {	///< ミドル注記ラインレイヤ４
				return	(&e_sMiddle_Note_Line4[0]);
			}
	char*	get_Middle_Note	() {		///< ミドル注記
				return	(&e_sMiddle_Note[0]);
			}
	char*	get_Middle_Contour	() {	///< 段彩レイヤ
				return	(&e_sMiddle_Contour[0]);
			}
	char*	get_Middle_Line	() {		///< ミドル背景アークレイヤ
				return	(&e_sMiddle_Line[0]);
			}
	char*	get_Middle_Site	() {		///< ミドル背景、施設レイヤ
				return	(&e_sMiddle_Site[0]);
			}
	// トップマップ用 2003.01.05
	char*	get_Top_Mesh	() {		///< ミドルメッシュレイヤ
				return	(&e_sTop_Mesh[0]);
			}
	char*	get_Top_Contour	() {		///< 段彩レイヤ
				return	(&e_sTop_Contour[0]);
			}
	char*	get_Top_Line	() {		///< トップ背景アークレイヤ
				return	(&e_sTop_Line[0]);
			}
	char*	get_Top_Site	() {		///< トップ背景、施設レイヤ
				return	(&e_sTop_Site[0]);
			}
	char*	get_Top_Note_Line1	() {	///< トップ注記ラインレイヤ１
				return	(&e_sTop_Note_Line1[0]);
			}
	char*	get_Top_Note_Line2	() {	///< トップ注記ラインレイヤ２
				return	(&e_sTop_Note_Line2[0]);
			}
	char*	get_Top_Note_Line3	() {	///< トップ注記ラインレイヤ３
				return	(&e_sTop_Note_Line3[0]);
			}
	char*	get_Top_Note_Line4	() {	///< トップ注記ラインレイヤ４
				return	(&e_sTop_Note_Line4[0]);
			}
	char*	get_Top_Note	() {		///< トップ注記
				return	(&e_sTop_Note[0]);
			}

	char*	get_Road_Link	() {		///< 道路リンクレイヤ
				return	(&e_sRoad_Link[0]);
			}
	char*	get_Parea		() {		///< PAREA代表点レイヤ
				return	(&e_sParea[0]);
			}
	char*	get_Base_Rep_Point	() {	///< 背景種別用テーブル
				return	(&e_sBase_Rep_Point[0]);
			}
	char*	get_Background_Class	() {	///< 中縮尺Addrポイント
				return	(&e_sBackground_Class[0]);
			}
	char*	get_Annotation_Class	() {	///< 注記種別用テーブル
				return	(&e_sAnnotation_Class[0]);
			}
	char*	get_POI_Point () {	///< POI_POINTテーブル [bug 9264]
				return	(&e_sPOI_Point[0]);
			}

	/**
		プログラム初期化ファイルの読み込み
		@return 成功　0　失敗　-1
	*/
	int		read_init_file	(
				char	*c_cFile,	///< 読み込みファイル名
				int		c_iMode=0	///< 読み込みモード 0=SDE用 1=SiNDY-C用 2007.03.01
			);
	/**
		プログラム初期化環境変数の読み込み
		@return 成功　0　失敗　-1
	*/
	int		read_init_env	(
				int		c_iMode=0	///< 読み込みモード 0=SDE用 1=SiNDY-C用 2007.03.01
			);

	/**
		接続情報の取得
		@return	成功	true
		@return	失敗	false
	*/
	bool	get_Connect_Info	(
				char*			c_cpConnect_Str,	///< 接続文字列
				std::string*	c_UserName,			///< ユーザ名[out]
				std::string*	c_Password,			///< パスワード[out]
				std::string*	c_Version,			///< バージョン[out]
				std::string*	c_Instance,			///< インスタンス[out]
				std::string*	c_Server			///< サーバ名[out]
			);

	/**	2007.06.27 追加 from CrossRoadsNameChk から
		初期化文字列の読み込み
		@return	成功	true
		@return	失敗	false
	*/
	bool	read_init_str	(
				char	*c_cpConnect_Str
			);

};
