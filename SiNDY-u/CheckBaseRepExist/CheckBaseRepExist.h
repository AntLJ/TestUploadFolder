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

// CheckBaseRepExist.h: CheckBaseRepExist クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHECKBASEREPEXIST_H__B1E7A4F7_5191_438C_857C_AFCCA442E3A1__INCLUDED_)
#define AFX_CHECKBASEREPEXIST_H__B1E7A4F7_5191_438C_857C_AFCCA442E3A1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <iostream>
#include <fstream>
#include <string>
#include <oracl.h>
#include <libjdbaccess.h>
#include <libsdeaccess.h>
#include <liboutputerrlog.h>
#include <JDBAccess.h>
#include <SDEAccess.h>
#include <OutputErrLog.h>
#include <sindy/libschema.h>

namespace {
	enum ErrCode {
		ERR_NO_BASE_REP = 1,
		ERR_NO_OAZA_REP,
		ERR_ONLY_PAREA,
		ERR_INVALID_CODE,
		ERR_FROM_GOU_POINT,
		ERR_NO_BASE_REP_ADDR2,
		ERR_NO_OAZA_REP_ADDR2,
		ERR_INVALID_CODE_ADDR2,
		ERR_FROM_GOU_POINT_ADDR2,
		ERR_NOT_MATCH_AREACODEMASTER,
	};

	// OutPutErrorLog.libを使うための定義
	const char* PROGRAM_ID = "H04xx";
	const int ECT_MAX_RECORD = 10;
	static ErrCodeRec gECTable[] = {
		{ ERR_NO_BASE_REP,			kError,		"Base_Rep_Point がない" },
		{ ERR_NO_OAZA_REP,			kError,		"大字代表点がない" },
		{ ERR_ONLY_PAREA,			kError,		"PAREA のみ" },
		{ ERR_INVALID_CODE,			kError,		"廃止 or 無効な11桁" },
		{ ERR_FROM_GOU_POINT,		kWarning,	"号ポイントより生成" },
		{ ERR_NO_BASE_REP_ADDR2,	kError,		"Base_Rep_Point がない (ADDRCODE2)" },
		{ ERR_NO_OAZA_REP_ADDR2,	kError,		"大字代表点がない (ADDRCODE2)" },
		{ ERR_INVALID_CODE_ADDR2,	kError,		"廃止 or 無効な11桁 (ADDRCODE2)" },
		{ ERR_FROM_GOU_POINT_ADDR2,	kWarning,	"号ポイントより生成 (ADDRCODE2)" },
		{ ERR_NOT_MATCH_AREACODEMASTER,	kWarning,	"住所マスタにあってBase_Rep_Pointにない" },
	};

	/// チェックモード
	enum	ECheckMode {
		kNone		= 0,
		kCityAll	= 1,
		kCityList	= 2,
		kCitySingle	= 3,
	};
} // namespace

/**
 * @brief 行政界情報格納用
 * @note 行政界の11桁を住所コード１、２の判別がつくように
 */
struct adminfo
{
	enum infotype {
		ADDR1 = 0,	///< ADDRCODE1から
		ADDR2 = 1	///< ADDRCODE2から
	};
	
	std::string	mCode6;		///< 6桁コード
	long	mInfoType;	///< 住所コード１からなのか２からなのか(0=１から、1=２から)

	///< コンストラクタ
	adminfo () {
		mInfoType = ADDR1;
	}
	///< デストラクタ
	~adminfo () {}

	bool operator<(const adminfo& cDat ) const {
		if( mCode6 != cDat.mCode6 ) {
			if( mCode6 < cDat.mCode6 ) {
				return true;
			}
		}else {
			if( mInfoType != cDat.mInfoType ) {
				if( mInfoType < cDat.mInfoType ) {
					return true;
				}
			}
		}
		return false;
	}
};

typedef std::set<std::string>	AddrList;
typedef std::set<adminfo> AdminList;	// [Bug9463]で追加

struct AORec
{

	std::string	eAddrCode;
	long	eOID;

	AORec() {
		eOID	= 0;
	}

	AORec(const AORec &cAORec) {
		eAddrCode	= cAORec.eAddrCode;
		eOID		= cAORec.eOID;
	}

	AORec(const std::string &cAddrCode) {
		eAddrCode	= cAddrCode;
		eOID		= 0;
	}

	AORec& operator = (const AORec &cAORec) {
		eAddrCode	= cAORec.eAddrCode;
		eOID		= cAORec.eOID;
		return *this;
	}

	bool operator < (const AORec &cAORec) const {
		return eAddrCode < cAORec.eAddrCode;
	}

};

typedef std::set<AORec, std::less<AORec> > AOList;

class CheckBaseRepExist : public SDEAccess  
{

private:

	int		m_Argc; //!< 実行時オプション数
	char	**m_Argv; //!< 実行時オプション

	std::string		m_fnLogFile; //!< ログファイル名
	std::string		m_fnStatList; //!< ステータスファイル名
	OutputErrLog	m_errLog; //!< ログ管理クラス

	FILE		*m_fpStatList; //!< ステータスファイル用ファイル構造体

	ECheckMode	m_checkMode; //!< チェックモード(ca、cs、clオプション)
	std::string		m_cityCode; //!< 5桁コード(csオプション指定時に使用)
	std::string		m_fnCityList; //!< 5桁コードリストファイル名(clオプション指定時に使用)
	AddrList	m_cityList; //!< 処理対象5桁コードリスト

	std::string		m_dbAddress; //!< 住所系SDEサーバ接続先

	std::string		m_fcGouPoint; //!< GOU_POINTフィーチャクラス名
	std::string		m_fcCityAdmin; //!< CITY_ADMINフィーチャクラス名
	std::string		m_fcBaseRep; //!< BASE_REP_POINTフィーチャクラス名
	std::string		m_fcCSAddrPoint; //!< CS_ADDR_POINTフィーチャクラス名

	IFeatureClassPtr	m_ipFcGouPoint; //!< GOU_POINTフィーチャクラス
	IFeatureClassPtr	m_ipFcCityAdmin; //!< CITY_ADMINフィーチャクラス
	IFeatureClassPtr	m_ipFcBaseRep; //!< BASE_REP_POINTフィーチャクラス
	IFeatureClassPtr	m_ipFcCSAddrPoint; //!< CS_ADDR_POINTフィーチャクラス

	long	m_fIdGPGouNumber; //!< GOU_POINTのGOU_NOフィールドインデックス
	long	m_fIdCACityCode; //!< CITY_ADMINのCITYCODEフィールドインデックス
	long	m_fIdCAAddrCode; //!< CITY_ADMINのADDRCODEフィールドインデックス
	long	m_fIdCSAddrCode; //!< CS_ADDR_POINTのADDRCODEフィールドインデックス
	long	m_fIdBRAddrCode; //!< BASE_REP_POINTのADDRCODEフィールドインデックス

	JDBAccess	m_jdbAccess; //!< JDBAccessクラス

	/*
	 * @brief 環境変数の解析・取得
	 * @retval true 成功
	 * @retval false 失敗
	 */
	bool	fnGetEnvVar();

	/*
	 * @brief 実行時オプションの解析・取得
	 * @retval true 成功
	 * @retval false 失敗
	 */
	bool	fnAnalyzeArg();

	/*
	 * @brief 5桁コードリスト取得
	 * @note -ca 住所マスタから全国分の5桁コードを取得し、リストに格納
	 *       -cs オプションで指定された5桁コードをリストに格納
	 *       -cl オプションで指定されたファイルから5桁コードを取得し、リストに格納
	 * @retval true 成功
	 * @retval false 失敗
	 */
	bool	fnCreateCityList();

public:

	CheckBaseRepExist();

	virtual ~CheckBaseRepExist();

	/**
	 * @brief 実行の初期化
	 * @retval true 初期化成功
	 * @retval false 初期化失敗
	 */
	bool	Init( void );

	/**
	 * @brief 大字／字コードリストの作成
	 * @param crCityCode [in]		市区町村コード	 
	 * @param crAddrList [out]		大字／字コードリスト
	 */
	void	MakeOazaAzaList( const std::string& crCityCode, AdminList& crAddrList );

	/**
	 * @brief Base_Rep_Point のリスト作成
	 * @param crCityCode [in]		市区町村コード	 
	 * @param crAOListBR [out]		Base_Rep_Pointのリスト	 
	 */
	void	MakeBaseRepPointList( const std::string& crCityCode, AOList& crAOListBR );

	/**
	 * @brief 11桁行政界の内部に含まれる号のカウントを取得
	 * @param crCityCode [in]		市区町村コード	 
	 * @param crAdmInfo [in]		行政界の情報
	 * @param crOazaRepFlag [out]	大字代表点フラグ
	 * @param crOID_CityAdmin [out]	行政界のOID
	 * @param crGPTotal [out]		号のカウント
	 */
	void 	GetContainedGouCount( const std::string& crCityCode, 
									const adminfo& crAdmInfo,
									bool& crOazaRepFlag,
									long& crOID_CityAdmin,
									long& crGPTotal );

	/**
	 * @brief 号ポイントのチェック
	 * @param crCityCode [in]		市区町村コード
	 * @param crAdmInfo [in]		行政界の情報
	 * @param crOazaRepFlag [in]	大字代表点フラグ
	 * @param crOID_CityAdmin [in]	行政界のOID
	 * @param crGPTotal [in]		号のカウント
	 */
	void 	CheckGoupoint( const std::string& crCityCode, 
							const adminfo& crAdmInfo,
							bool crOazaRepFlag,
							long crOID_CityAdmin,
							long crGPTotal );

	/**
	 * @brief Base_Rep_Point に関するチェック
	 * @param crAOListBR [in]		Base_Rep_Point のリスト
	 */
	void	CheckBaseRepPoint( const AOList& crAOListBR );

	/**
	 * @brief CS_Addr_Point に関するチェック
	 * @param crAOListBR [in]		Base_Rep_Point のリスト
	 * @param crCityCode [in]		市区町村コード
	 */
	void 	CheckCSAddrPoint( const AOList& crAOListBR, const std::string& crCityCode );

	/**
	 * @brief AreacodeMaster(住所マスタ)に関するチェック
	 * @param cityCode [in] 市区町村コード
	 * @param baseRepAddrCodes [in] Base_Rep_Pointの11桁コードリスト
	 */
	void CheckAreacodeMaster(const std::string& cityCode, const AOList& baseRepAddrCodes);

	/**
	 * @brief メイン処理関数
	 * @param cArgc [in] 実行時オプション数
	 * @param cArgv [in] 実行時オプション文字列
	 * @retval true 処理成功
	 * @retval false 処理失敗
	 */
	bool	Execute(int cArgc, char **cArgv);

};

#endif // !defined(AFX_CHECKBASEREPEXIST_H__B1E7A4F7_5191_438C_857C_AFCCA442E3A1__INCLUDED_)
