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


using namespace std;



// Shapeデータ保持用のクラス(1レコードごとに1つ)
class	shp1
{
public:
	// コンストラクタ
	shp1()
	{
		shp_ID		= 0;
		point_num	= 0;
		check_flag	= 0;
		shp_dX		= 0;
		shp_dY		= 0;
		iMeshCode	= 0;
		iField_num	= 0;
	}

	// デクストラクタ
	~shp1()
	{
	}

	long					shp_ID;					// オブジェクトID or FID（FIDモード指定した時）
	int						point_num;				// 補間点個数
	double					shp_dX;					// ポイントのX座標値
	double					shp_dY;					// ポイントのY座標値
	int						check_flag;				// チェックされたかどうかのフラグ
	int						iMeshCode;				// メッシュコード
	int						iField_num;				// フィールド数
	vector<string>			vsField_name;			// フィールド名を順番に格納
	map<string, wstring>	Fields;					// フィールド数が増えても格納できるようにmapでフィールド名と要素で格納
		

	bool operator < (const shp1& right) const{	//オペレータ設定
		bool	ret	= false;
		if( shp_ID != right.shp_ID ) {			//オブジェクトIDの昇順で並び替える
			if( shp_ID < right.shp_ID ) {
				ret	= true;
			}
		}
		return	( ret );
	}
};

typedef multimap<long, shp1>			mmShp1;		// shp1の1メッシュ分保持のためのmultimap
typedef	multimap<long, shp1>::iterator	itrmmShp1;

// バイナリモード時のShapeデータ保持用のクラス(1meshごとに1つ)
class	shp2
{
public:
	shp2()
	{
		iRec_num			= 0;
		iHeadSize			= 0;
		iRecSize			= 0;
		iMeshCode			= 0;
	}

	~shp2()
	{
	}

	int									iRec_num;			// レコード数
	int									iHeadSize;			// ヘッダサイズ
	int									iRecSize;			// レコードサイズ
	int									iMeshCode;			// メッシュコード
	vector<string>						vsField_name;		// フィールド名を順番に格納
	//vector<string>					vsBinData;			// 1レコードごとのバイナリデータを順番に格納
	multimap<int, pair<int, string>>	msBinData;			// 1レコードごとのバイナリデータをオブジェクトIDを
															//キーにチェック済みフラグとともにmultimapに格納
};

// 処理用クラス
class	CCompShape2Shape
{
public:
	// コンストラクタ
	CCompShape2Shape()
	{
		m_iExeMode		= 0;
		m_dDistBorder	= 0;
		m_bAttr			= false;
		m_bShp			= false;
	}
	
	// デクストラクタ
	~CCompShape2Shape()
	{
	}

	/**
	 * @brief	使用法
	 */
	void	printUsage();

	/**
	 * @brief	引数チェック
	 * @param	argc	[in]	引数の数
	 * @param	argv	[in]	引数
	 * @return	bool
	 */
	bool	arg_check( int argc, char* argv[] );

	/**
	 * @brief	バージョン出力
	 */
	void	print_ver();

	/**
	 * @brief	Shapeファイル基点パス出力
	 */
	void	print_pass();

	/**
	 * @brief	開始時間出力
	 */
	void	print_begin_time();

	/**
	 * @brief	終了時間出力
	 */
	void	print_end_time();

	/**
	 * @brief	ログヘッダ(フィールド名)出力
	 */
	void	print_log_head();

	/**
	 * @brief	実行
	 * @return	bool
	 */
	int	execute();

private:
	/**
	 * @brief	リストファイル読み込み
	 * @param	lpszFile	[in]	読み込みたいリスト名
	 * @param	vsVectorList	[out]	読込先Vector(1フィールドごと)
	 * @return	bool
	 */
	bool	loadlist( LPCTSTR lpszFile, vector<string>& vsVectorList );

	/**
	 * @brief	メッシュ単位ファイル名の作成
	 * @param	iMeshCode	[in]	メッシュコード
	 * @param	cPathName	[in]	ファイル基点パス
	 * @return	string		[out]	ファイルのフルパス
	 */
	string	make_mesh_fname( int iMeshCode,	char* cpPathName );

	/**
	 * @brief	Shapeファイルを読み込み、multimap<ID,shp1>に落とし込む
	 * @param	sFullPass	[in]	読み込みShapeファイルのフルパス
	 * @param	mmShp1		[out]	Shapeファイル中身格納用クラス集合コンテナ
	 * @param	iMesh		[in]	メッシュコード
	 * @return	bool
	 */
	bool	load_shape_info( string sFullPass, int iMesh, mmShp1& mmShp1 );

	/**
	 * @brief	二つのShapeファイルの属性と補間点数を比較し、異なればエラー出力
	 * @param	mmShpOri	[in]	比較元Shapeファイル中身格納用クラス集合コンテナ<ID, shp1本体>
	 * @param	mmShpDest	[in]	比較先Shapeファイル中身格納用クラス集合コンテナ<ID, shp1本体>
	 */
	bool	comp_shape_info( mmShp1 mmShpOri, mmShp1 mmShpDest );

	/**
	 * @brief	Shapeファイルをバイナリで読み込み、shp2に落とし込む
	 * @param	sFullPass	[in]	読み込みShapeファイルのフルパス
	 * @param	iMesh		[in]	メッシュコード
	 * @param	cShp2		[out]	Shapeファイル中身格納用クラス（バイナリ）
	 * @return	bool
	 */
	bool	load_shape_info_bin( string sFullPass, int iMesh, shp2& cShp2 );

	/**
	 * @brief	二つのShapeファイルのレコードをバイナリで比較し、異なればエラー出力
	 * @param	cShp2Ori	[in]	比較元Shapeファイル中身格納用クラス（バイナリ）
	 * @param	cShp2Dest	[in]	比較先Shapeファイル中身格納用クラス（バイナリ）
	 */
	bool	comp_shape_info_bin( shp2 cShp2Ori, shp2 cShp2Dest );


	int					m_iExeMode;					// ON_FID_MODEのときFIDで紐付け、それ以外はOBJECTIDで紐付け
	vector<string>		m_vsMesh;					// メッシュリスト1行(メッシュコード+エリアコード)ずつ格納
	string				m_sOriFPass;				// 比較元Shapeファイルパス
	string				m_sDestFPass;				// 比較先Shapeファイルパス
	string				m_sLayerName;				// 比較対象レイヤ名					
	string				m_sDataSet;					// データセット名(Citymap等)
	bool				m_bAttr;					// 属性チェックを行うか判断するbool値
	bool				m_bShp;						// 形状チェックを行うか判断するbool値
	double				m_dDistBorder;				// 形状チェックの際、距離の許容誤差
	vector<string>		m_vsFSpecify;				// 比較フィールド指定時、指定フィールド名格納用

	ofstream			m_ofErrorLog;				// ログ出力用

	crd_cnv				m_cCrd;						
	
	SYSTEMTIME			m_st;						// 開始・終了時間取得用
	string				m_sVersion;					// バージョン情報出力用

//	shp1				m_clOriShp;					// 比較元Shapeハンドルクラス
//	shp1				m_clDestShp;				// 比較先Shapeハンドルクラス

};
