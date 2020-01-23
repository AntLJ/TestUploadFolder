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
#include <set>
#include <string>
#include <boost/function.hpp>

/**
 * @brief 個人的によく書く関数群を、置き場に困ってとりあえずまとめたもの
 * @note	TODO: そういうものはWinLibに置くべきとの事だったので、そちらへ移動予定。
 */
namespace adlib
{
	//----- 汎用typedef -----//
	typedef long OID;					///< オブジェクトID
	typedef long NodeID;
	typedef long LinkID;
	typedef std::set<LinkID> LinkIDs;

	typedef long MeshCode;
	typedef std::set< MeshCode > MeshCodes;

	typedef std::basic_string<TCHAR> tstring;	///< std::stringのTCHAR版

	/**
	 * @brief 日付と回数を付与して既存ファイルと被らないファイル名を作成する
	 * @note	元のファイル名の拡張子の前に「_yyyy-mm-dd_n」を挿入。nは1～cMaxCountまでの数のうち、既存ファイル名と被らない最小の数。
	 *			出典：SiNDYQToMPQ::editLogFileName()
	 * @param[in,out]	cFilePath	対象ファイルパス
	 * @param[in]		cMaxCount	付与する番号の最大値（デフォルトは1000）
	 */
	// TODO: 回数ではなく、秒単位の時間を付与した方が便利。
	void identifiedFileName( boost::filesystem::path& cFilePath, long cMaxCount = 1000 );

	/**
	 * @brief FREESTYLELOGヘッダを出力
	 * @note	出典：SiNDYQToMPQプロジェクトのUtility.h
	 * @param[in]	出力ストリーム
	 * @param[in]	ヘッダの２行目に列挙するフィールド名文字列の集合
	 */
	void outputSindyFreeStyleLogHeader( std::ostream& co, std::vector<std::string> cFieldNames );

	/**
	 * @brief ロケールを「日本語ロケールの数値区切りファセットだけをCロケールに変更したロケール」に変更する
	 * @note	http://www.freeml.com/cppll_novice/1550/latest より。
	 *			日本語ロケールを使いたいけど数値がカンマで区切られては困るという場合に使用。
	 *			出典：SiNDYQToMPQプロジェクトのUtility.h
	 */
	void setJapaneseLocaleWithCNumpunctFacet(void);

	/**
	 * @brief ワークスペースの取得
	 * @note	出典：SiNDYQToMPQ
	 * @param[in]	cDBStr	接続先指定文字列
	 * @param[in]	cErrorFunc	失敗時に呼び出す関数
	 * @return	取得したワークスペース
	 */
	IWorkspacePtr getWorkspace( tstring cDBStr, boost::function<void (std::string)> cErrorFunc );

	/**
	 * @brief フィーチャクラスの取得
	 * @note	出典：SiNDYQToMPQ
	 * @param[in]	cpWorkspace	取得元ワークスペース
	 * @param[in]	cTableName	取得するフィーチャクラスのテーブル名
	 * @param[in]	cErrorFunc	失敗時に呼び出す関数
	 * @return	取得したフィーチャクラス
	 */
	IFeatureClassPtr getFeatureClass(IWorkspace* cpWorkspace, tstring cTableName, boost::function<void (std::string)> cErrorFunc);

	/**
	 * @brief 文字列が２次メッシュコードらしき数（6桁の数字）であるか否かを判定する
	 * @note	出典：SiNDYQToMPQ
	 * @param[in]	cStr	対象文字列
	 * @return	判定結果
	 */
	bool isBaseMeshCode( std::string cStr );

	typedef long BaseMeshCode;
	typedef std::set<BaseMeshCode> BaseMeshList;
	/**
	 * @brief ２次メッシュリスト読み込み
	 * @note	メッシュリストファイルからメッシュコード群を読み込み、コンテナに格納して返す
	 *			出典：SiNDYQToMPQ
	 * @param[in]	cBaseMeshListFilePath	対象メッシュリストのファイルパス
	 * @param[in]	cOpenErrorFunc			ファイルを開けなかった場合に呼び出す関数
	 * @param[in]	cFormatErrorFunc		ファイルの内容が不正な場合に呼び出す関数
	 */
	BaseMeshList loadMeshList(
		boost::filesystem::path cBaseMeshListFilePath,
		boost::function<void (std::string)> cOpenErrorFunc,
		boost::function<void (long)> cFormatErrorFunc );

	/**
	 * @brief メッシュフィーチャクラスから指定されたメッシュコードを持つメッシュフィーチャを取得する
	 * @note	出典：SiNDYQToMPQ::getMeshFeature()
	 * @param[in]	cpBaseMeshClass	対象BaseMeshフィーチャクラス
	 * @param[in]	cMeshCode		対象２次メッシュコード
	 * @return	メッシュフィーチャ（取得できなかった場合はNULL）
	 */
	IFeaturePtr getMeshFeature( IFeatureClassPtr cpBaseMeshClass, long cMeshCode );

	/**
	 * @brief フィールドインデックスを返す（[Bug8736]で追加）
	 * @note	出典：SindyHighwayCheckのhwy_check_utility::fieldIndex()
	 *			TODO: RowではなくTableを引数に取る版も多分必要。
	 *			TODO: 呼び出し元の可読性向上のため、テーブル名を指定しなくてもいいようにしたい。AheGetOwnerTableName()を使用？
	 * @param[in]	cpRow		対象Row
	 * @param[in]	cTableName	テーブル名
	 * @param[in]	cFieldName	フィールド名
	 * @return	フィールドインデックス
	 */
	long fieldIndex( _IRowPtr cpRow, tstring cTableName, tstring cFieldName );

	/**
	 * @brief 四捨五入した値を返す
	 * @note	「負の数の四捨五入」の定義は明確に決まっていないらしいが、ここでは正の場合と同じ絶対値になるように定義する。
	 * @param[in]	cNum	対象値
	 * @return	結果
	 */
	long roundOff( double cNum );

	/**
	 * @brief Road_Link.UpDownClass_CがHWYか否かを判定する
	 * @param[in]	cUpDownClass	対象UpDownClass_C
	 * @return	判定結果
	 */
	bool isHwy( long cUpDownClass );

	/**
	 * @brief 対象リンクがHWYか否かを判定する
	 * @note	判定基準はRoad_Link.UpDownClass_C。
	 * @param[in]	cpLink	対象リンク
	 * @return	判定結果
	 */
	bool isHwy( IFeaturePtr cpLink );

	/**
	 * @brief 指定されたメッシュの周囲9メッシュのメッシュコード群を返す
	 * @param[in]	cMeshCode	対象メッシュ
	 * @return	メッシュコード群
	 */
	MeshCodes surroundingMeshCodes( long cMeshCode );

	/// あるメッシュに対する、他のもう１つのメッシュの位置関係
	namespace mesh_relation
	{
		enum ECode
		{
			kSame,			///< 同一
			kEast,
			kWest,
			kNorth,
			kSouth,
			kNorthEast,
			kNorthWest,
			kSouthEast,
			kSouthWest,
			kNone,			///< 辺でも点でも接触していない
		};
	} // namespace mesh_relation

	/// ２次メッシュコード計算クラス
	class CMeshCode
	{
	private:
		long m_MeshCode;	///< ２次メッシュコード

	public:
		/// コンストラクタ
		CMeshCode(
			long cMeshCode
		) :
			m_MeshCode( cMeshCode )
		{}

		/// メッシュコードを返す
		long code() const { return m_MeshCode; }

		/**
		 * @brief 東へ1メッシュ移動
		 * @return	移動後の自身
		 */
		CMeshCode& east();

		/**
		 * @brief 西へ1メッシュ移動
		 * @return	移動後の自身
		 */
		CMeshCode& west();

		/**
		 * @brief 北へ1メッシュ移動
		 * @return	移動後の自身
		 */
		CMeshCode& north();

		/**
		 * @brief 南へ1メッシュ移動
		 * @return	移動後の自身
		 */
		CMeshCode& south();
#if 0	// TODO: 必要なら作る
		/**
		 * @brief 東へ移動
		 * @param[in]	cDelta	移動量
		 * @return	移動後の自身
		 */
		CMeshCode& east( long cDelta );

		/**
		 * @brief 北へ移動
		 * @param[in]	cDelta	移動量
		 * @return	移動後の自身
		 */
		CMeshCode& north( long cDelta );

		/**
		 * @brief 西へ移動
		 * @param[in]	cDelta	移動量
		 * @return	移動後の自身
		 */
		CMeshCode& west( long cDelta ) { return east( -cDelta ); }

		/**
		 * @brief 南へ移動
		 * @param[in]	cDelta	移動量
		 * @return	移動後の自身
		 */
		CMeshCode& south( long cDelta ) { return north( -cDelta ); }
#endif

		/**
		 * @brief 指定されたメッシュとの位置関係を判定する
		 * @param[in]	cMeshCode	対象メッシュ
		 * @return	位置関係
		 */
		mesh_relation::ECode relation( long cMeshCode );
	};


} // namespace adlib
