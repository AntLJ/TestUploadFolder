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

#include <stdio.h>
#include <vector>
#include <set>
#include <string>
#include <coord_converter.h>
#include "exec_time_handle.h"
#include <atlstr.h>
#include <SiNDYLib/Workspace.h>
#include "GeometryFinder.h"
#include "MakeShapeFile.h"
#include <boost/tokenizer.hpp>
#include <boost/foreach.hpp>


#ifndef	_MAIN_ROUTINE_H_
#define	_MAIN_ROUTINE_H_

#define		ON_LOGFILE  0x01 //!< ログファイル指定
#define		ON_DELETE2  0x02 //!< 2レイヤ目の関係オブジェクトの削除処理を実施
#define     ON_DELMODE  0x04 //!< 削除モード指定（デフォルトはチェックモード）

namespace {
	typedef boost::char_separator<char> char_separator;
	typedef boost::tokenizer<char_separator> tokenizer;
}

/**
* @brief エラーメッセージ
* @param c_cpFp     [in] 出力ファイルポインタ
* @param c_cpFp2    [in] 出力ファイルポインタ２(ファイルとstdoutに同時・・のような場合を想定）
* @param c_cpFormat [in] 出力変換書式
*/
void PrnMsg ( FILE* c_cpFp, FILE* c_cpFp2, const char* c_cpFormat, ... );

// typedef std::map<long, CAdapt<IFeaturePtr>, std::less<long> > mIFeatureContainer;
typedef std::map<long, CAdapt<IGeometryPtr>, std::less<long> > mIFeatureContainer;

/**
* @brief オブジェクトの当該メッシュで処理するかの判定で使用
*/
struct XYPt
{

	double mX; //!< 経度
	double mY; //!< 緯度

	/**
	* @brief コンストラクタ
	*/
	XYPt() {
		mX = 0;
		mY = 0;
	}
	/**
	* @brief デストラクタ
	*/
	~XYPt() {}

	/**
	* @brief 比較演算子
	* @return src<tgt true, それ以外=false
	*/
	bool operator<(const XYPt& cTgt) const {
		bool bRet = false;
		if( mX != cTgt.mX ) {
			if( mX < cTgt.mX ) { bRet = true; }
		}else {
			if( mY != cTgt.mY ) {
				if( mY < cTgt.mY ) { bRet = true; }
			}
		}
		return bRet;
	}
};

/**
 * @brief オブジェクトの優先度管理用クラス
 * @note 優先度は、POI・注記データ①＞不動産情報＞防火対象物情報＞POI・注記データ②＞POI・注記データ③ の順
 * @note データタイプ、注記種別、コンテンツコードをキーに、優先度を返す
 */
struct PriorityManager
{
	enum {
		TYPE_POI = 1,       //!< POI
		TYPE_REALESTATE = 2,//!< 不動産情報
		TYPE_FIREPROOF = 3, //!< 防火対象物情報
	};

	/**
	* @brief 優先度情報クラス
	*/
	struct PriorityInfo {
		long        mlDataType;      //!< データのタイプ
	//	long        mlAnnoAttr;      //!< 注記種別コード
		long        mlContentsCode; //!< コンテンツコード
		long        mlPriority;      //!< 全体としての優先度
		std::string msName;          //!< 属性の名称

		/**
		* @brief コンストラクタ
		*/
		PriorityInfo(){
			mlDataType = -1;
		//	mlAnnoAttr = -1;
			mlContentsCode = -1;
			mlPriority = -1;
		}
		/**
		* @brief デストラクタ
		*/
		~PriorityInfo() {
		}

		/**
		* @brief 比較演算子
		* @note  データタイプ、注記種別、コンテンツコードでソート
		* @return src<tgt = true 左記以外=false0
		*/
		bool operator<(const PriorityInfo& cTgt) const {
			if( mlDataType != cTgt.mlDataType ) {
				if( mlDataType < cTgt.mlDataType ) // データタイプ
					return true;
			}else if( mlContentsCode != cTgt.mlContentsCode ) {
				if( mlContentsCode < cTgt.mlContentsCode ) // コンテンツコード
					return true;
			}
			return false;
		}

	};

	// PriorityManagerのメンバ
	std::set<PriorityInfo> msPriorityInfo; //!< 優先度情報

	/**
	* @brief コンストラクタ
	*/
	PriorityManager(){}
	
	/**
	* @brief デストラクタ
	*/
	~PriorityManager(){}

	/**
	* @brief テキストファイルの行を解析して、優先度レコードに変換する
	* @return 成功=true,失敗=false
	*/
	bool analyze_line( const char*   c_cpTxt,  ///< [in] テキストの一行のデータ
	                   PriorityInfo& c_pinfo ) ///< [out] 優先度情報
	{
		char_separator csSep("\t", "", boost::keep_empty_tokens); // 分割設定
		string         sBuf;
		sBuf         = c_cpTxt;
		tokenizer      a_tTokens(sBuf, csSep); // 分割

		tokenizer::iterator iIter;
		long lLoop = 0;
		for (iIter = a_tTokens.begin(); iIter != a_tTokens.end(); ++iIter++, ++lLoop)
		{
			switch (lLoop)
			{
			case	0:
				if( (*iIter) == "POI" ) {
					c_pinfo.mlDataType = TYPE_POI;
				}else if( (*iIter) == "不動産" ) {
					c_pinfo.mlDataType = TYPE_REALESTATE;
				}else if( (*iIter) == "防火対象物" ) {
					c_pinfo.mlDataType = TYPE_FIREPROOF;
				}else {
					return false;
				}
				break;	// タイプ
			case	1:	c_pinfo.mlContentsCode	= atol((*iIter).c_str()); break; // コンテンツコード
			case	2:	c_pinfo.mlPriority      = atol((*iIter).c_str());	break;	// 出力FC名
			case	3:	c_pinfo.msName          = (*iIter);	break; // 種別名
			}
		}
		//-----------------------------------------------
		return true;
	}

	/**
	* @brief 優先度リスト読み込み
	* @return 成功=true, 失敗=false
	*/
	bool Init( std::string csPriorityList ) //!< [in] 優先度リスト
	{
		const int bmax	= 2048;
		FILE*     a_pFp;		
		char      a_cBuf[bmax];
		int       a_StrLen = 0;	

		a_pFp = fopen ( csPriorityList.c_str(), "r" );
		if(a_pFp == NULL) {
			return false;
		}

		msPriorityInfo.clear(); // 念のためクリア

		while( fgets ( a_cBuf, bmax, a_pFp ) != NULL)
		{
			a_StrLen	= strlen ( a_cBuf );
			// 最後の"\n"を取り除く
			if(a_cBuf[a_StrLen - 1] == '\n') {
				a_cBuf[a_StrLen - 1] = '\0';
			}
			// コメント文を飛ばす
			if(a_cBuf[0] == '#' || a_cBuf[0] == '\0') {
				continue;
			}
			PriorityInfo pCurInfo;
			if( !analyze_line( a_cBuf, pCurInfo ) ){
				return false;
			}
			// リストに追加
			msPriorityInfo.insert ( pCurInfo );
		}
		fclose ( a_pFp );
		return true;
	}

	/**
	* @brief 
	* @note 不動産情報、防火対象物情報については、「コンテンツコード」は'-1'を指定するものとする
	* @return 優先度(対象が見つからない場合は-1を返却する)
	*/
	long GetPriority( long& clDataType,      //!< [in] データタイプ
			          long& clContentsCode,  //!< [in] コンテンツコード
					  std::string& csName )  //!< [out] 種別名称
	{
		PriorityInfo pKey;
		pKey.mlDataType     = clDataType;
		pKey.mlContentsCode = clContentsCode;

		std::set<PriorityInfo>::iterator iIter;
		iIter = msPriorityInfo.find ( pKey );
		if( iIter != msPriorityInfo.end() ) {
			csName = (*iIter).msName; // 名称
			return (*iIter).mlPriority;
		}else {
			csName = ""; // 名称
			return -1;
		}
	}
};

typedef std::multiset<PriorityManager> msPriorityManager;

/**
* @brief ポイントの情報用クラス
*/
struct PointInfo {
	long         mlOID;      //!< ポイントのオブジェクトID
	long         mlDataType; //!< データタイプ(PriorityManagerで定義のもの)
	long         mlContentsCode; //!< コンテンツコード(POI以外は-1)
	long         mlPriority; //!< 対応表から得られた優先度
	CString      mcsName;    //!< 名称
	IGeometryPtr mipGeom;    //!< 位置情報
	/**
	* @brief コンストラクタ
	*/
	PointInfo(){
		mlPriority = -1;
		mlOID = 0;
		mlDataType = 0;
		mlContentsCode = -1;
	}
	/**
	* @brief デストラクタ
	*/
	~PointInfo(){}
	/**
	* @brief 比較演算子
	*/
	bool operator<(const PointInfo& cTgt) const {
		if( mlPriority != cTgt.mlPriority ) {
			if( mlPriority < cTgt.mlPriority )
				return true;
		}else if( mcsName != cTgt.mcsName ) { 
			if( mcsName < cTgt.mcsName )
				return true;
		}
		return false;
	}
};

typedef std::multiset<PointInfo> mPointInfo;

/**
* @brief ポイントの情報用クラス(XY優先ソート）
*/
struct PointInfo_XY : PointInfo {
	double	mX; // X座標値
	double	mY; // Y座標値
	/**
	* @brief コンストラクタ
	*/
	PointInfo_XY(){
		mX = 0;
		mY = 0;
	}
	/**
	* @brief デストラクタ
	*/
	~PointInfo_XY(){}
	/**
	* @brief 比較演算子
	*/
	bool operator<(const PointInfo_XY& cTgt) const {
		if( mX != cTgt.mX ) {
			if( mX < cTgt.mX )
				return true;
		}else if( mY != cTgt.mY ) { 
			if( mY < cTgt.mY )
				return true;
		}
		return false;
	}
	/**
	* @brief 代入演算子
	*/
/*	void operator=(const PointInfo& cTgt) {
		mlOID          = cTgt.mlOID; // ポイントのオブジェクトID
		mlDataType     = cTgt.mlDataType; // データタイプ
		mlContentsCode = cTgt.mlContentsCode; // コンテンツコード(POI以外は-1)
		mlPriority     = cTgt.mlPriority; // 対応表から得られた優先度
		mcsName        = cTgt.mcsName;    // 名称
		mipGeom        = cTgt.mipGeom;    // 位置情報
	}*/
};

typedef std::multiset<PointInfo_XY> mPointInfo_XY;

/**
 * @brief メイン処理クラス
 */
class	main_routine
{
	/**
	 * @brief 関係定義用
	 */
	enum {
		contains = 1,
		overlaps,
		within,
		touches,
		equals,
		crosses
	};

	public:
		//------------------------------------------------------------------------
		std::string   m_sDB_Building;   //!< 建物の接続DB
		std::string   m_sFC_Building;   //!< 建物のフィーチャクラス名
		std::string   m_sMeshList;      //!< メッシュリスト
		std::string   m_sPriorityList;  //!< POI・注記データ優先度設定リスト
		std::string   m_sShapeFName;    //!< 出力Shapeファイル名
		std::string	  m_sLog_File;      //!< エラーログデータ出力パス
		std::string   m_sExeLog_File;   //!< 実行ログファイル

		std::string   m_sDB_Mesh;       //!< メッシュの接続DB
		std::string   m_sFC_Mesh;       //!< メッシュのフィーチャクラス名
		std::string   m_sDB_POI;        //!< POI・注記の接続DB
		std::string   m_sFC_POI;        //!< POI・注記のフィーチャクラス名
		std::string   m_sDB_RealEstate; //!< 不動産情報の接続DB
		std::string   m_sFC_RealEstate; //!< 不動産情報のフィーチャクラス名
		std::string   m_sDB_FireProof;  //!< 防火対象物情報の接続DB
		std::string   m_sFC_FireProof;  //!< 防火対象物情報のフィーチャクラス名

		std::string   m_sDB_Connect1;   //!< DB接続情報１
		std::string   m_sFeatureClass1; //!< フィーチャクラス１名称
		std::string   m_sDB_Connect2;   //!< DB接続情報２
		std::string   m_sFeatureClass2; //!< フィーチャクラス２名称
		std::string   m_sObjectID_File; //!< 処理オブジェクトIDリスト
		std::string	  m_sRel_Str;       //!< 関係指定文字列
		long          m_lRel_Type;      //!< 指定したオブジェクト同士の関係の形式

		std::set<long>    m_sMESH_List; //!< メッシュコードリスト

		IFeatureClassPtr  m_ipFC_Building; //!< フィーチャクラス(建物)
		IFeatureClassPtr  m_ipFC_Mesh;     //!< フィーチャクラス(メッシュ)
		IFeatureClassPtr  m_ipFC_POI;      //!< フィーチャクラス(POI)
		IFeatureClassPtr  m_ipFC_RealEstate;//!< フィーチャクラス(不動産情報)
		IFeatureClassPtr  m_ipFC_FireProof;//!< フィーチャクラス(防火対象物情報)

		IWorkspacePtr     m_ipWSpace_Building; //!< ワークスペース(建物)
		IWorkspacePtr     m_ipWSpace_Mesh;     //!< ワークスペース(メッシュ)
		IWorkspacePtr     m_ipWSpace_POI;      //!< ワークスペース(POI)
		IWorkspacePtr     m_ipWSpace_RealEstate;//!< ワークスペース(不動産情報)
		IWorkspacePtr     m_ipWSpace_FireProof;//!< ワークスペース(防火対象物情報)

		IWorkspaceEditPtr m_ipWorkspaceEdit_1; //!< エディット用
		IWorkspaceEditPtr m_ipWorkspaceEdit_2; //!< エディット用

		ISpatialReferencePtr m_ipMeshSpRef;    //!< 空間参照

		MakeShapeFile     m_mShpFile; //!< シェイプファイル作成用クラス

		PriorityManager   m_pPriorityMgr; //!< 優先度管理クラス

		//------------------------------------------------------------------------
		long					m_lExe_Flag;
		FILE*					m_fErr_Fp;	//!< エラーログファイル用ファイルポインタ
		FILE*                   m_fExe_Fp;  //!< 実行ログファイル用
	private:
		exec_time_handle		m_eExeTime;	//!< 実行時間管理

		crd_cnv					m_cCrd;
	private:
		CString       m_sVersion;  //!< ツールバージョン

	public:
		//!< コンストラクタ
		main_routine () {
			m_lExe_Flag		= 0;
			m_sRel_Str      = "contains";
			m_lRel_Type     = contains;
			m_fErr_Fp		= stderr; // デフォルトは標準エラー出力へ
			m_fExe_Fp       = stdout;
		}
		//!< デストラクタ
		~main_routine () {
			if( m_fErr_Fp != NULL && m_fErr_Fp != stderr && m_fErr_Fp != stdout ) {
				fclose ( m_fErr_Fp );
			}
			if( m_fExe_Fp != NULL && m_fExe_Fp != stderr && m_fExe_Fp != stderr ) {
				fclose ( m_fExe_Fp );
			}
		}

		/**
		 * @brief DBへの接続オープン
		 * @return  true   成功
		 * @return  false  失敗
		 */
		bool	DB_Open	( void );

		/**
		 * @brief エラーログファイルをオープンする
		 * @return true 成功
		 * @return  false  失敗
		 */
		bool    Open_Log (
		            std::string& csFName, //!< [in] ファイル名
		            FILE**       cFp      //!< [out] ファイルポインタ
		        );

		/**
		 * @brief ツールの使用法の表示
		 */
		void    print_usage (
		            char** argv //!< [in] 引数列
		        );

		/**
		 * @brief 実行情報表示
		 */
		void    PrintExecInfo( void );

		/**
		 * @brief 引数チェック
		 * @return	成功	true
		 * @return	成功	false
		 */
		bool    arg_check (
		            int    argc, //!< [in] 引数個数
		            char** argv  //!< [in] 引数列
		        );

		/**
		 * @brief リストの読み込み
		 * @return	成功	true
		 * @return	成功	false
		 */
		bool    Read_List (
		            const std::string& c_sFileName, //!< [in] ファイル名
		            std::set<long>&    c_sList      //!< [in] リスト（ID等に使用）
		        );

		/**
		 * @brief メッシュ矩形をGeoDBから取得する
		 * @return IGeometryPtr(失敗時はNULL)
		 */
		IGeometryPtr    GetMeshRect (
		                    int   c_iMeshCode //!< メッシュコード
		                );

		/**
		 * @brief 指定フィーチャに対してesriSpatialRelEnumで指定した関係を持つデータをとってくる
		 * @return IFeatureCursorPtr
		 */
		IFeatureCursorPtr   Search_Feature (
		                        const IFeatureClassPtr& c_ipFeCls,     //!< [in] フィーチャクラス
		                        const IGeometryPtr&     c_ipGeo,       //!< [in] 検索用ジオメトリ
		                        esriSpatialRelEnum      c_ipSpatialRel,//!< [in] 関係指定
		                        std::string             c_sWhere,      //!< [in] Where句指定
		                        long&                   c_iCount       //!< [out] 検索数
							);

		/**
		 * @brief 代表点を取得
		 * @return 
		 * @return	成功	true
		 * @return	成功	false
		 */
		bool   GetPoint (
		           const IGeometryPtr& c_ipGeom, //!< [in] ジオメトリ
		           double&             c_dX,     //!< [out] 経度
		           double&             c_dY      //!< [out] 経度
		       );

		/**
		 * @brief 実行部
		 * @note ※内部でエラーファイルの処理等も行う
		 * @return	成功	true
		 * @return	成功	false
		 */
		bool exec ( void );
		
		/**
		* @brief データのコンテナを作成する
		* @note  
		* @return	データ数（失敗の場合は-1）
		*/
		int    MakeDataContainer (
		           long                     cMeshCode,     //!< [in] 現メッシュコード
		           const IFeatureCursorPtr& cipFCursor,    //!< [in] フィーチャカーソル
		           mIFeatureContainer&      cmWork_Data,   //!< [in] 作業用データ
				   bool                     bLeftDownPtMode//!< [in] 一番左下のポイントが
		       );

		/**
		* @brief ポイント情報のコンテナを作成する
		* @note  データタイプが-1の場合は、建物名称ポイント情報の「」フィールドのタイプで判別する
		* @return	データ数（失敗の場合は-1）
		*/
		int    MakePointContainer (
		           const IFeatureCursorPtr& cipFCursor,    //!< [in] フィーチャカーソル
				   CString&                 cNameFidldName,//!< [in] 名称フィールド名指定
		           mPointInfo_XY&           cmWork_Data,   //!< [out] 作業用データ
		           long                     clDataType=-1  //!< [in] データタイプ(PriorityManagerで定義のもの)
		       );

		/**
		* @brief 建物形状から外接矩形を作成する
		* @note  
		* @return	IGeometryPtr
		*/
		IGeometryPtr    MakeBuildingsEnvelope (
		                    const mIFeatureContainer& cmWork_Data //!< [in] 作業用データ
		                );

		/**
		* @brief XYソートしたポイント列から、ポリゴンに含まれるポイントの、優先度ソートのコンテナを作成
		* @return	成功=true,失敗=false
		*/
		bool   GetInnerPoint (
		           IGeometryPtr&   cipPolygon,   //!< [in] ポリゴンジオメトリ
		           mPointInfo_XY&  cmPoint_Data, //!< [in] ポイント情報コンテナ(XYソート)
		           mPointInfo&     cmOutput      //!< [out] ポイント情報コンテナ(優先度ソート)
			   );

		/**
		* @brief データ出力
		* @note  
		* @return	成功=true,失敗=false
		*/
		bool   PrintOut (
		           mIFeatureContainer&      cmBuilding_Data, //!< [in] 建物データコンテナ
		           mPointInfo_XY&           cmPoint_Data     //!< [out] ポイント情報コンテナ
		       );

};
//------------------------------------------------------<ここまで>

#endif	//_MAIN_ROOTIN_H_
