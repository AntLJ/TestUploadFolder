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

// CheckJusyoDist.h: CheckJusyoDist クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHECKJUSYODIST_H__BF9CACAC_4A4E_4A68_9937_C1F4A4D98FEB__INCLUDED_)
#define AFX_CHECKJUSYODIST_H__BF9CACAC_4A4E_4A68_9937_C1F4A4D98FEB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define FD_ADDR_CODE		"AddrCode"	//!< エラー住所ポイントPGDB上の属性名

#define PROGRAM_ID				"H05xx"

#define ECT_MAX_RECORD			7

#define ERR_CANT_GET_VALUE		1
#define ERR_ILLEGAL_VALUE		2
#define ERR_QUERY_FAILED		3
#define ERR_CANT_INSERT			4
#define ERR_CANT_GET_SHAPE		5
#define ERR_NULL_GAIKU_FUGO		10
#define ERR_GP_DISTRIBUTION		100

#define	ON_FiveCode		0x01	// 市区町村単位(5桁)実行モード
#define	ON_ElevenCode	0x04	// 11桁単位実行モード
#define ON_Addrcode8    0x08    // 住所8桁＋地番単位実行モード

static ErrCodeRec gECTable[] = {
	{ ERR_CANT_GET_VALUE,	kError,		"フィールドから値が取得できない" },
	{ ERR_ILLEGAL_VALUE,	kError,		"値が不正" },
	{ ERR_QUERY_FAILED,		kError,		"クエリの実行に失敗" },
	{ ERR_CANT_INSERT,		kError,		"テーブルに挿入できません" },
	{ ERR_CANT_GET_SHAPE,	kError,		"形状を取得できません" },
	{ ERR_NULL_GAIKU_FUGO,	kWarning,	"住居表示整備済なのに街区符号がNULL" },
	{ ERR_GP_DISTRIBUTION,	kError,		"号ポイントの分布にばらつきがある" },
};

/// 市区町村リスト作成タイプ
enum	ECityListType {
	kCLNone			= 0,	// 無指定
	kCLAll			= 1,	// 全市区町村指定
	kCLList			= 2,	// リストファイルでの市区町村指定
	kCLSingle		= 3,	// 単一市区町村指定
	kCLRange		= 4,	// コード範囲指定
	kAdr8List       = 5		// 住所8桁コードリスト指定
};

struct AddrTmpRec
{
	string	eAddrCode;
	long	ePointID;
	double	eLongitude;
	double	eLatitude;
	long	eAddrClass;
	long	eGouType;
	string	eCharPntName;	//!< [Bug6516]文字地番名称

	AddrTmpRec() {
		ePointID	= 0;
		eLongitude	= 0;
		eLatitude	= 0;
		eAddrClass	= 0;
		eGouType	= 0;
	}

	void operator = (const AddrTmpRec &cATRec) {
		eAddrCode	= cATRec.eAddrCode;
		ePointID	= cATRec.ePointID;
		eLongitude	= cATRec.eLongitude;
		eLatitude	= cATRec.eLatitude;
		eAddrClass	= cATRec.eAddrClass;
		eGouType	= cATRec.eGouType;
		eCharPntName = cATRec.eCharPntName;
	}

	bool operator < (const AddrTmpRec &cATRec) const {
		if(eAddrCode == cATRec.eAddrCode) {
			if(eGouType == cATRec.eGouType) {
				return ePointID < cATRec.ePointID;
			}
			return eGouType < cATRec.eGouType;
		}
		return eAddrCode < cATRec.eAddrCode;
	}

};

typedef set<AddrTmpRec, less<AddrTmpRec> >	AddrTmpSet;

struct PointRec
{

	long	ePointID;
	double	eLongitude;
	double	eLatitude;
	long	eGouType;

	PointRec() {
		ePointID	= 0;
		eLongitude	= 0;
		eLatitude	= 0;
		eGouType	= 0;
	}

	void operator = (const PointRec &cPtRec) {
		ePointID	= cPtRec.ePointID;
		eLongitude	= cPtRec.eLongitude;
		eLatitude	= cPtRec.eLatitude;
		eGouType	= cPtRec.eGouType;
	}

	bool operator < (const PointRec &cPtRec) const {
		if(eGouType == cPtRec.eGouType) {
			return ePointID < cPtRec.ePointID;
		}
		return eGouType < cPtRec.eGouType;
	}

};

typedef list<PointRec>	PointList;

struct AddrRec
{
	string		eAddrCode;
	PointList	ePoints;
	string		eCharPntName;	//!< [Bug6516]文字地番名称

	AddrRec() {}

	AddrRec& operator = (const AddrRec &cAddrRec) {
		eAddrCode	= cAddrRec.eAddrCode;
		ePoints		= cAddrRec.ePoints;
		eCharPntName = cAddrRec.eCharPntName;
		return *this;
	}

	bool operator == (const AddrRec& addrRec)
	{
		return eAddrCode == addrRec.eAddrCode && eCharPntName == addrRec.eCharPntName;
	}

	bool operator < (const AddrRec &cAddrRec) const {
		return eAddrCode < cAddrRec.eAddrCode;
	}

};

typedef list<AddrRec>	AddrList;

struct GroupRec
{

	PointRec	eRepPoint;
	PointList	ePoints;

	GroupRec() {
	}

	GroupRec& operator = (const GroupRec &cGRec) {
		ePoints		= cGRec.ePoints;
		return *this;
	}

};

typedef list<GroupRec>	GroupList;

struct AC11Rec
{

	string	eAC11;
	set<long, less<long> >	eOIDSet;

	AC11Rec() {
	}

	AC11Rec(const AC11Rec &cAC11Rec) {
		eAC11		= cAC11Rec.eAC11;
		eOIDSet		= cAC11Rec.eOIDSet;
	}

	~AC11Rec() {
	}

	bool operator < (const AC11Rec &cAC11Rec) const {
		return eAC11 < cAC11Rec.eAC11;
	}

};

typedef set<AC11Rec, less<AC11Rec> >	AC11Set;

struct AC8Rec
{
	string	eAC8;
	AC11Set	eAC11Set;

	AC8Rec() {
	}

	AC8Rec(const AC8Rec &cAC8Rec) {
		eAC8		= cAC8Rec.eAC8;
		eAC11Set	= cAC8Rec.eAC11Set;
	}

	~AC8Rec() {
	}

	bool operator < (const AC8Rec &cAC8Rec) const {
		return eAC8 < cAC8Rec.eAC8;
	}

};

typedef set<AC8Rec, less<AC8Rec> >		AC8Set;

class ACSet : public AC8Set
{

public:

	void	Add(const string &cAC11, const long cOID )
	{
		AC8Rec	aAC8Rec;
		aAC8Rec.eAC8	= cAC11.substr(0, 8);
		AC8Set::iterator aAC8Itr = find(aAC8Rec);
		if( aAC8Itr != end() )
		{	// 同一８桁が見つかった
			aAC8Rec = *aAC8Itr;	// コピー
			erase(aAC8Itr);		// 一旦削除
			AC11Rec	aAC11Rec;
			aAC11Rec.eAC11	= cAC11;
			AC11Set::iterator aAC11Itr = aAC8Rec.eAC11Set.find(aAC11Rec);
			if( aAC11Itr != aAC8Rec.eAC11Set.end() )	// 同一11桁が見つかった
			{
				aAC11Rec = *aAC11Itr;				// コピー
				aAC8Rec.eAC11Set.erase(aAC11Itr);	// 一旦削除
				aAC11Rec.eOIDSet.insert(cOID);		// 登録（OID）
				aAC8Rec.eAC11Set.insert(aAC11Rec);	// 再登録（11桁）
				insert(aAC8Rec);					// 再登録（８桁）
			}
			else {										// 同一11桁が見つからない
				aAC11Rec.eOIDSet.insert(cOID);		// 登録（OID）
				aAC8Rec.eAC11Set.insert(aAC11Rec);	// 登録（11桁）
				insert(aAC8Rec);					// 再登録（８桁）
			}
		} else {				// 同一８桁が見つからない
			AC11Rec	aAC11Rec;
			aAC11Rec.eAC11	= cAC11;
			aAC11Rec.eOIDSet.insert(cOID);			// 登録（OID）
			aAC8Rec.eAC11Set.insert(aAC11Rec);		// 登録（11桁）
			insert(aAC8Rec);						// 登録（８桁）
		}
	}

};

struct AddrPolyRec
{

	long				eOID;
	IPolygonPtr			ipPolygon;

	AddrPolyRec& operator = (const AddrPolyRec &cAPRec) {
		eOID		= cAPRec.eOID;
		ipPolygon	= cAPRec.ipPolygon;
		return *this;
	}

};

typedef list<AddrPolyRec>	AddrPoly;
typedef list<AddrPoly>		AddrGroup;

struct AdminAttr
{
	string	eAddrCode;		//!< 住所コード
	long	eGaikuFugo;		//!< 街区符号
	long	eAddrClassC;	//!< 住所整備種別
	string	eAddrCode2;		//!< 住所コード2
	string	eExtGaiku;		//!< 拡張街区符号[Bug6516]

	AdminAttr() {
		eGaikuFugo	= 0;
		eAddrClassC	= 0;
	}

	AdminAttr& operator = (const AdminAttr &cAdmAttr) {
		eAddrCode	= cAdmAttr.eAddrCode;
		eGaikuFugo	= cAdmAttr.eGaikuFugo;
		eAddrClassC	= cAdmAttr.eAddrClassC;
		eAddrCode2	= cAdmAttr.eAddrCode2;
		eExtGaiku	= cAdmAttr.eExtGaiku;
		return *this;
	}

};

class CheckJusyoDist : public SDEAccess  
{
public:

	CheckJusyoDist();
	virtual ~CheckJusyoDist();

	/**
	 * @brief	使用法
	 */
	static void Usage();

	bool Execute(int cArgc, char **cArgv);

private:

	char	*mDBCon_Address;       //!< 住所系SDEサーバ接続先
	char	*mPGDB_Out;            //!< エラー住所ポイントPGDB

	char	*mFC_Target;           //!< エラー住所ポイントPGDB上のレイヤ名
	char	*mFC_CityAdmin;        //!< 都市地図行政界ポリゴンのフィーチャ名
	char	*mFC_GouPoint;         //!< 号ポイントのフィーチャ名
	char	*mFC_CSAddrPoint;      //!< クレームポイントのフィーチャ名
	char	*mFC_KlinkPoint;       //!< 家形リンクポイントのフィーチャ名

	IFeatureClassPtr	ipFC_CityAdmin;      //!< CITY_ADMINのフィーチャクラス
	IFeatureClassPtr	ipFC_GouPoint;       //!< GOU_POINTのフィーチャクラス
	IFeatureClassPtr	ipFC_CSAddrPoint;    //!< CS_ADDR_POINTのフィーチャクラス
	IFeatureClassPtr	ipFC_KlinkPoint;     //!< KLINK_POINTのフィーチャクラス

	IFeatureClassPtr	ipFC_TmpPoints;      //!< エラー住所ポイントPGDBのフィーチャクラス

	long	mFID_CityCode;       //!< CITY_ADMINのCITYCODEフィールドインデックス
	long	mFID_AddrCode;       //!< CITY_ADMINのADDRCODEフィールドインデックス
	long	mFID_GaikuFugo;      //!< CITY_ADMINのGAIKUFUGOフィールドインデックス 
	long	mFID_ExtGaikuFugo;   //!< CITY_ADMINのEXTGAIKUFUGOフィールドインデックス 
	long	mFID_AreaClassC;     //!< CITY_ADMINのAREACLASS_Cフィールドインデックス 
	long	mFID_AddrClassC;     //!< CITY_ADMINのADDRCLASS_Cフィールドインデックス
	long	mFID_AddrCode2;      //!< CITY_ADMINのADDRCODE2フィールドインデックス

	long	mFID_GouNo;          //!< GOU_POINTのGOU_NOフィールドインデックス
	long	mFID_GouType;        //!< GOU_POINTのGOU_TYPEフィールドインデックス
	long	mFID_PriorityF;      //!< GOU_POINTのPRIORITY_Fフィールドインデックス
	long	mFID_ExpGouNoF;      //!< GOU_POINTのEXPGOUNO_Fフィールドインデックス

	long	mFID_InfoSrcC;       //!< KLINK_POINTのINFOSRC_Cフィールドインデックス
	long	mFID_InputSeidoC;    //!< KLINK_POINTのINPUTSEIDO_Cフィールドインデックス
	long	mFID_AddrCode_KP;    //!< KLINK_POINTのADDRCODE_ORGフィールドインデックス

	long	mFID_AddrCode_CS;    //!< CS_ADDR_POINTのADDRCODEフィールドインデックス

	long	mFID_AddrCode_TP;    //!< エラー住所ポイントPGDBのADDRCODEフィールドインデックス

	bool	mIgnoreTPG;          //!< TPG除外オプション（（-t）指定判定フラグ

	char	*mFN_ChkLog;         //!< エラーログファイル名
	OutputErrLog	mChkLog;     //!< ログ管理クラス

	char	*mFN_CityList;       //!< 市区町村リストファイル名（-clオプション指定時）
	char	*mCityCode;          //!< 市区町村コード（-csオプション指定時）
	char    *mAddr8List;         //!< 住所8桁コードリスト名（-adr8オプション指定時）

	long	mRangeMin;           //!< 市区町村コード最小値（-crオプション指定時）
	long	mRangeMax;           //!< 市区町村コード最大値（-crオプション指定時）

	double	mDistErr;            //!< グループ距離（-dnオプション値）
	double	mDistNbr;            //!< エラーとするグループ間距離（-deオプション値）

	int		m_iExeMode;			 //!< 実行モード（5桁、11桁、8桁＋地番）

	CityList	mCityList;       //!< 処理対象の住所コードリスト

	ECityListType	mCLType;     //!< 市区町村リスト作成タイプ

	AddrTmpSet	mAddrTmpSet;     //!< 住所ポイント格納コンテナ
	AddrList	mAddrList;       //!< 同一住所コードでグループ化した住所ポイントのリスト

	ACSet		mACSet;          //!< 住所8桁-11桁の対応リスト

	crd_cnv		mCConv;          //!< 座標値変換ライブラリ

	JDBAccess	mJDBAccess;      //!< 住所DBアクセスクラス

	ISpatialCacheManagerPtr	ipSpCacheMgr;       //!< ISpatialCacheManager 

	std::set<int> m_exeModeList; //!< 実行モードリスト

	/**
	 * @brief	環境変数を取得する
	 * @retval true  取得成功
	 * @retval false 取得失敗
	 */
	bool	fnGetEnvVar();

	/**
	 * @brief	オプション解析＋検査
	 * @param[in] argc  コマンドラインで指定した引数の数
	 * @param[in] argv  コマンドラインで指定した引数の文字列
	 * @retval true  検査OK
	 * @retval false 検査NG
	 */
	bool	fnAnalyzeArg( int argc, char** argv );

	/**
	 * @brief	エラー住所ポイントPGDBを生成する
	 * @retval true  生成成功
	 * @retval false 生成失敗
	 */
	bool	fnCreateTmpFC();

	/**
	 * @brief	処理対象の住所コードリストを生成する
	 * @retval true  生成成功
	 * @retval false 生成失敗
	 */
	bool	fnMakeCityList();

	/**
	 * @brief	行政界リスト生成
	 * @param	cCityCode	[in]	都市コード
	 * @retval true  生成成功
	 * @retval false 生成失敗
	 */
	bool	fnMakeACList(const char *cCityCode);

	/**
	 * @brief	エラーログ・ファイルのオープン＋ヘッダ情報の書込み
	 * @retval true  成功
	 * @retval false 失敗
	 */
	bool	fnOpenChkLog();

	/**
	 * @brief	指定行政界の号ポイント20桁化
	 * @param	ipFeature	[in]	行政界
	 */
	bool fnAdminProc(const IFeaturePtr& ipFeature );

	/**
	 * @brief	指定行政界内の号ポイント20桁化
	 * @param	ipFeature	[in]	号ポイント
	 * @param	cAdmAttr	[in]	行政界
	 * @param	isAddAdr2Point	[in]	ADDRCODE2のコード値を持つ号ポイントの追加有無（true：追加する、false：追加しない）
	 */
	void fnGetGPInfo(const IFeaturePtr& ipFeature, const AdminAttr &cAdmAttr, bool isAddAdr2Point);

	/**
	 * @brief	[Bug6516]20桁生成関数
	 * @param	cAdmAttr	[in]	行政界
	 * @param	strGouNo	[in]	号番号
	 * @param	bExGouF		[in]	拡張号番号フラグ
	 * @param	rRec		[in]	住所コード・地番名称(文字地番時)格納用コンテナ
	 */
	void CreateAddrCode( const AdminAttr& cAdmAttr, const string& strGouNo, bool bExGouF, AddrTmpRec& rRec );

	/**
	 * @brief 指定した家形リンクポイントから情報を取得する
	 * @param[in] ipF_KlinkPoint  家形リンクポイントのフィーチャ
	 * @param[in] cAdmAttr  都市地図行政界ポリゴン情報
	 * @retval true  取得成功
	 * @retval false 取得失敗
	 */
	bool fnGetKPInfo(const IFeaturePtr& ipF_KlinkPoint, const AdminAttr& cAdmAttr);

	/**
	 * @brief	CSポイント情報取得
	 * @param	ipFeature	[in]	CSポイント
	 */
	void fnCSAddrProc(const IFeaturePtr& ipFeature );

	/**
	 * @brief 住所ポイント群をグループ化する（住所16桁コード単位、実行モード：-e,-f用）
	 */
	void fnConvDataSet16();

	/**
	 * @brief 住所ポイント群をグループ化する（住所8桁コード＋地番単位、実行モード：-adr8用）
	 */
	void fnConvDataSet8Chiban();

	/**
	 * @brief グループ化した住所ポイント群ごとに距離チェックを行なう
	 */
	void fnCheckDataSet16();

	/**
	 * @brief	8桁コードエリアでキャッシュ作成
	 * @param	cAddrCode	[in]	8桁住所コード
	 */
	bool fnCreateCache8(const std::string &cAddrCode);

	/**
	 * @brief 指定したCITY_ADMINのフィーチャの情報を取得する
	 * @param[in] ipCityAdmin  取得対象のCITY_ADMINのフィーチャ
	 * @param[out] adminAttr  CITY_ADMINの情報（コンテナ）
	 * @param[out] ipCityAdminGeo  CITY_ADMINのジオメトリ
	 */
	void getCityAdmin(const IFeaturePtr& ipCityAdmin, AdminAttr& adminAttr, IGeometryPtr& ipCityAdminGeo);

	/**
	 * @brief 特定条件を満たす住所ポイントを除外
	 * @param[in] isExcludeKakeiPoint  家形リンクポイントを除外対象とするか（true：対象とする、false：対象としない）
	 */
	void excludingPoints(bool isExcludeKakeiPoint);

	/**
	 * @brief 住所16桁コード単位に住所ポイント群をグループ化する
	 */
	void makeAddrcodeGroupsByAdr16();

	/**
	 * @brief 住所8桁コード＋地番コード（文字付き住所の場合は住所8桁コード＋地番名称）単位に住所ポイント群をグループ化する
	 */
	void makeAddrcodeGroupsByAdr8Chiban();

	/**
	 * @brief 指定した都市地図行政界ポリゴン内に存在する号ポイントを取得する
	 * @param[in] ipFeature  都市地図行政界ポリゴンのフィーチャ
	 * @retval true  取得成功
	 * @retval false 取得失敗
	 */
	bool getGouPoints(const IFeaturePtr& ipFeature );

	/**
	 * @brief 指定した住所コードに紐付くCSポイントを取得する
	 * @param[in] addrcode  住所コード
	 * @retval true  取得成功
	 * @retval false 取得失敗
	 */
	bool getCSAddrPoints(const CString& addrcode);

};

#endif // !defined(AFX_CHECKJUSYODIST_H__BF9CACAC_4A4E_4A68_9937_C1F4A4D98FEB__INCLUDED_)
