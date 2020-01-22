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

/** @file
 * CDoc クラスのインプリメンテーション
 *
 * @author	shimano
 * @date	2003/04/09
 *
<<<<<<< Doc.h
 * $Log: Doc.h,v $
 * Revision 1.5  2006/10/02 11:46:17  shimano
 * Arc9.1対応（F.2.0.0.9）
 *
 * Revision 1.4  2005/08/02 02:12:53  shimano
 * FILEVERSION 1,0,8,1 PRODUCTVERSION 1,0,8,8 対応。
 *
 * Revision 1.3  2003/11/17 04:18:38  shimano
 * Ver.1.6（アクセス道路リスト出力フォーマット修正）対応
 *
 * Revision 1.2  2003/11/15 01:35:57  shimano
 * アクセス道路属性確認ファイル出力対応（Debugモード）
 *
=======
 * $Log: Doc.h,v $
 * Revision 1.5  2006/10/02 11:46:17  shimano
 * Arc9.1対応（F.2.0.0.9）
 *
 * Revision 1.4  2005/08/02 02:12:53  shimano
 * FILEVERSION 1,0,8,1 PRODUCTVERSION 1,0,8,8 対応。
 *
 * Revision 1.3  2003/11/17 04:18:38  shimano
 * Ver.1.6（アクセス道路リスト出力フォーマット修正）対応
 *
 * Revision 1.2  2003/11/15 01:35:57  shimano
 * アクセス道路属性確認ファイル出力対応（Debugモード）
 *
 * Revision 1.1.1.2  2003/04/24 09:46:44  shimano
 * 新規作成
 *
>>>>>>> 1.1.1.2
 * Revision 1.0  2003/04/09 15:13:43  shimano
 * beta8対応。
 *
>>>>>>> 1.1.1.3
 * Revision 1.0  2003/05/04 14:31:41  shimano
 * beta9対応。
 */

#if !defined(AFX_DOC_H__0E3F8A46_D0F3_4183_98D3_BFDA6E880BCA__INCLUDED_)
#define AFX_DOC_H__0E3F8A46_D0F3_4183_98D3_BFDA6E880BCA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include	"FacilPoint.h"
#include	"NwObject.h"
#include	"road.h"

typedef	vector<CFacilPoint>		FacilArray;
typedef	vector<CNode>			NodeArray;
typedef	set<long>			    LinkArray;

class CAccessRoad
{
public:
	CAccessRoad() :m_Meshcode(0),m_FromId(0),m_ToId(0) {}
	CAccessRoad(int cMesh,long cFromID,long cToID){ m_Meshcode=cMesh;
													m_FromId=cFromID;
													m_ToId=cToID;}
	int			getMeshcode(void){	return( m_Meshcode );		}
	long		getFromId(void){	return( m_FromId );			}
	long		getToId(void){		return( m_ToId );			}
	
	// 比較演算子
	bool operator == (const CAccessRoad& cRoad) const{       
		bool bEqual =( m_Meshcode == cRoad.m_Meshcode
					&& m_FromId  == cRoad.m_FromId
					&& m_ToId  == cRoad.m_ToId );
		return bEqual;
	}
	bool operator < (const CAccessRoad& cRoad) const{
		bool bStatus;
		if(m_Meshcode != cRoad.m_Meshcode){
			bStatus = (m_Meshcode < cRoad.m_Meshcode);
		}
		else if( m_FromId != cRoad.m_FromId ){
			bStatus = (m_FromId < cRoad.m_FromId);
		}
		else{
			bStatus = (m_ToId < cRoad.m_ToId);
		}
		return bStatus;
	}
	bool operator > (const CAccessRoad& cRoad) const{
		bool bStatus;
		if(m_Meshcode != cRoad.m_Meshcode){
			bStatus = (m_Meshcode > cRoad.m_Meshcode);
		}
		else if( m_FromId != cRoad.m_FromId ){
			bStatus = (m_FromId > cRoad.m_FromId);
		}
		else{
			bStatus = (m_ToId > cRoad.m_ToId);
		}
		return bStatus;
	}
// メンバ変数
protected:
	int		m_Meshcode;	//メッシュコード
	long	m_FromId;	//リンク始点ノードOBJECTID
	long	m_ToId;		//リンク終点ノードOBJECTID
};

class CDoc  
{
public:
	CDoc();
	virtual ~CDoc();
	
	// 出力オブジェクト
	enum EnwLinkColor{
		VIEW_ARC_DISP	= 0x0001,		//道路種別
		VIEW_ARC_NAVI	= 0x0002,		//経路種別
		VIEW_ARC_FERRY	= 0x0004,		//道路種別チェック
		VIEW_ARC_WIDTH	= 0x0008		//道路幅員
	};
	void deleteMap(void);
	bool convertFacilPoint(LPCTSTR);

	/**
	 * @brief 施設情報を取得する一連の流れを実行
	 * @note コンテンツリストの取得からSiNDY-kの情報抽出までを行う。	[bug 8187]対応 2012/04/17 ta_suga
	 * @retval true 読み込み成功
	 * @retval false 読み込み失敗
	 */
	bool getFacilPoint(void);
	/**
	 * @brief 施設情報のXML(Excelのスプレッドシート形式)を出力する
	 * @note SiNDY-kの施設情報から、リリースデータ「access_list.xls」と同一内容のデータを出力する	[bug 8187]対応 2012/04/17 ta_suga
	 * @param なし
	 * @retval true 読み込み成功
	 * @retval false 読み込み失敗
	 */
	bool writeFacilList(void);
	/**
	 * @brief Excelの行を書く
	 * @note 文字列の羅列分だけExcelの行を埋めていく	[bug 8187]対応 2012/04/17 ta_suga
	 * @param rStrAry [in] 入力文字列の羅列
	 * @param rColNum [in] 入力行の列数
	 * @param pOfstream [in/out] 出力ストリーム
	 * @return	なし
	 */
	void writeExcelRow(const CString* rStrAry, const int& rColNum, ofstream* pOfstream) const;
	/**
	 * @brief Excelのセルの中身を書く
	 * @note Excelの中身をString形式で書く	[bug 8187]対応 2012/04/17 ta_suga
	 * @param strValue [in] セルに入力する値
	 * @param pOfstream [in/out] 出力ストリーム
	 * @return	なし
	 */
	void writeExcelCell(LPCTSTR strValue, ofstream* pOfstream) const;
	/**
	 * @brief コンテンツリストを読み込む
	 * @note 書式は「-」区切りで、前の値を10万倍した値と後ろの値を足して、コンテンツコードとする
	 * @note 例えば「2220-1」だったらコンテンツコードは「222000001」を返す。	[bug 8187]対応 2012/04/17 ta_suga
	 * @param rContentsVec [out] コンテンツコードのvector
	 * @retval true 読み込み成功
	 * @retval false 読み込み失敗
	 */
	bool readContentsList(std::vector<long>& rContentsVec);
	bool makeFacilPoint(LPCTSTR);
	bool makeCheckArea(LPCTSTR);
	int checkFacilPoint(void);
	bool makeAccessRoadList(void);
	bool showAccessRoad(int,CFacilPoint *);

	void setDBRoadStr(LPCTSTR cStr)			{	m_strRoadDB.Format("%s",cStr);	}		//(_T("SINDY2012@onyx(SDE.DEFAULT)"))
	void setDBPOIStr(LPCTSTR cStr)			{	m_strPOIDB.Format("%s",cStr);	}		//(_T("ANNOPOI2011B@coral2(SDE.DEFAULT)"))
	void setContentsListStr(LPCTSTR cStr)	{	m_strContentsList.Format("%s",cStr);	}	//(_T("NONAME"))
	void setLogFileStr(LPCTSTR cStr)		{	m_strLogFile.Format("%s",cStr);		}	//(_T("access_road_err.txt"))
	void setLogDirStr(LPCTSTR cStr)			{	m_mkFileDir.Format("%s",cStr);		}	//(_T(".\\log"))
	void setDistance(int cVal)				{ if( cVal>0 ) m_CheckDist = cVal;	}
	CString getLogDirStr(void)				{	return( m_mkFileDir );				}
	CString getLogFileStr(void)				{	return( m_strLogFile );				}
	FacilArray	getFacilArray(void)			{	return( m_FacilPoints );			}	//施設ポイント情報
	void setDebugMode(void)					{	m_DebugMode = true;					}	//ディバグモードを設定
	void setFacilListWrite(const bool bFlag){	m_FacilListWrite =	bFlag;			}	//施設リスト書き込みモード設定 [bug 8187]対応 2012/04/17 ta_suga

	bool			Init(void);
	void			Close(void);
	void			CheckStart(void);
	void			CheckStop(void);
	CComVariant		GetValue( const IFeaturePtr, const LPCTSTR);
	CComVariant		GetValue( const _IRowPtr, const LPCTSTR);
	void			GetValueStringWithDomain(const IFeaturePtr, LPCTSTR, CString* coString);
	bool			variantToDomainName(ICodedValueDomain*, const _variant_t&, BSTR*);
	long			SelectByShape(IGeometryPtr, IFeatureClassPtr, IFeatureCursor**, esriSpatialRelEnum);

	/**
	 * @brief WHERE句でフィーチャ群を取ってくる
	 * @note POIポイントを取得するときに使用する。戻り値のvectorはOBJECTIDでソートされる	[bug 8187]対応 2012/04/17 ta_suga
	 * @param r_ipFeatureClass [in] 対象のフィーチャクラス
	 * @param strWhereClause [in] WHERE句
	 * @param rFeatureVec [out] フィーチャのvector
	 * @return 条件に一致したフィーチャの数
	 */
	long SelectByWhereClause(IFeatureClassPtr& r_ipFeatureClass, LPCTSTR strWhereClause, std::vector< CAdapt<IFeaturePtr> >& rFeatureVec ) const;
	/**
	 * @brief フィーチャからWKSPointを取得する
	 * @note 一連の処理を1行で実行したかったため作成	[bug 8187]対応 2012/04/17 ta_suga
	 * @param r_ipFeature [in] 対象のフィーチャ
	 * @param rWKSPoint [out] 戻り値としてのWKSPoint
	 * @retval true 形状取得成功
	 * @retval false 形状取得失敗
	 */
	bool GetFeaturePoint(const IFeaturePtr& r_ipFeature, WKSPoint& rWKSPoint) const;
	/**
	 * @brief WKSPointを1/256秒緯度経度のペアに変換
	 * @note 戻り値のペアはfirstが経度、secondが緯度。　[bug 8187]対応 2012/04/17 ta_suga
	 * @param pWKSPoint [out] 戻り値としてのWKSPoint
	 * @return 1/256秒緯度経度のペア
	 */
	std::pair<int, int> getLL256(const WKSPoint& rWKSPoint) const;

	/**
	 * @brief フィーチャクラスのオープン
	 * @note オープンに失敗したときは標準エラー出力にエラーを出力する。　[bug 8187]対応 2012/04/17 ta_suga
	 * @param r_ipFeatureWorkspace [in] フィーチャワークスペース
	 * @param strFeatureClassName [in] フィーチャクラス名
	 * @param ppFeatureClass [out] フィーチャクラス
	 * @retval true 読み込み成功
	 * @retval false 読み込み失敗
	 */
	bool OpenFeatureClass(const IFeatureWorkspacePtr& r_ipFeatureWorkspace, LPCTSTR strFeatureClassName, IFeatureClass** ppFeatureClass);

	/**
	 * @brief テーブルのオープン
	 * @note オープンに失敗したときは標準エラー出力にエラーを出力する。　[bug 8187]対応 2012/04/17 ta_suga
	 * @param r_ipFeatureWorkspace [in] フィーチャワークスペース
	 * @param strTableName [in] テーブル名
	 * @param ppTable [out] テーブル
	 * @retval true 読み込み成功
	 * @retval false 読み込み失敗
	 */
	bool OpenTable(const IFeatureWorkspacePtr& r_ipFeatureWorkspace, LPCTSTR strTableName, ITable** ppTable);

	bool			isNaviLink(const int,const IFeaturePtr);
	bool			isKendoClassLink(const IFeaturePtr);
	bool			isNaviRoadWidth(const int,const int);
	bool			isLinkQueRoute(IFeaturePtr);
	bool			isCrossSuikei(CFacilPoint *);
	bool			isTraceOverLink(IFeaturePtr, LinkArray &);
	int				CheckAccess(const IFeaturePtr piFeature, const IGeometryPtr piGeometry, LinkArray& rLink);
	int				LinkTrace(const IFeaturePtr,const IGeometryPtr,LinkArray &);
	bool			CheckConnectLink(const IPointPtr piFacilPoint, const IGeometryPtr piNearestGeometry, long cTracedLinkOID);

	//2005年度ならエラー判定しない	Add 2005/8/2
	bool is_2005_L3Road(IFeaturePtr ipLinkFeature){
		using namespace sindy::schema;

		if( ipLinkFeature!= NULL ){
			CComVariant vaRoad_C = GetValue(ipLinkFeature,road_link::kRoadClass);
			if( vaRoad_C.intVal==road_link::road_class::kNarrowL3 ){
				CComVariant vaRoad_C = GetValue(ipLinkFeature,road_link::kAlleyMainte);
				if( vaRoad_C.intVal==road_link::alley_mainte::k2005 ){
					return true;
				}
			}
		}
		return false;
	}
	//チェック用ラインの作成
	bool CreateCheckLineShape(CFacilPoint *cFacil);

	//判定しない(無視する)リンクかどうか  bug 7208 
	bool isIgnoreLink(_IRowPtr ipRow);

	//最近傍ポイントとの距離を取得
	double GetNearestDist(const IGeometryPtr piTargetGeom, const IPointPtr piSorcePoint, IPointPtr& rNearestPoint);

	//通行禁止リンクかどうか bug 7208
	bool isNopassLink(_IRowPtr ipRow);

	//一方通行リンクかどうか bug 7208
	bool isOnewayLink(int cOneway);

	//追跡の末に到達したリンクが一方通行かどうかを判定し、一方通行の場合は
	//一方通行ではない基本道路に到達するまで追跡する
	int IsTraceSuccessLink(IFeaturePtr ipFeature, IGeometryPtr ipGeom, LinkArray& rLink);

// メンバ変数
protected:
	CSHPHdl				m_SHPHdl;				//Shape SHPハンドル
	CDBFHdl				m_DBFHdl;				//Shape DBFハンドル

	IWorkspacePtr		m_ipWorkspace;
	IFeatureClassPtr	m_ipLinkFeatureClass;
	IFeatureClassPtr	m_ipNodeFeatureClass;
	IFeatureClassPtr	m_ipAdmFeatureClass;
	IFeatureClassPtr	m_ipBaseSiteFeatureClass;
	IFeatureClassPtr	m_ipBaseLineFeatureClass;
	ITablePtr			m_ipLQ_Route;
	ITablePtr			m_ipINF_Route;
	FILE				*m_Fp;					//アクセス道路チェック結果ファイルへのポインタ
	CString				m_strRoadDB;			//道路の接続DB
	CString				m_strPOIDB;				//POIの接続DB
	CString				m_strContentsList;		//コンテンツリストファイル
	CString				m_strLogFile;			//アクセス道路エラーログファイル名
	CString				m_mkFileDir;			//出力ファイルフォルダ名
	FacilArray			m_FacilPoints;			//施設情報ポイント
	int					m_CheckDist;			//アクセス道までの距離
	bool				m_DebugMode;			//ディバグモード（アクセス道路属性ファイル出力）
	bool				m_FacilListWrite;		//施設情報リスト(access_list.xml)を出力するフラグ
};

#endif // !defined(AFX_DOC_H__0E3F8A46_D0F3_4183_98D3_BFDA6E880BCA__INCLUDED_)
