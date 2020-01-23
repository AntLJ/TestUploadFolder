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
#include <sindy/schema/ssmaster.h>

namespace sindy {
namespace schema {
namespace ssmaster {

	// DTMDSP(データ仕様管理)
	namespace dtmdsp {
		const _TCHAR kTableName[] = _T("DTMDSP");

		const _TCHAR kGrpCode[]       = _T("GRPCODE");        //!< データ分類コード
		const _TCHAR kChainCode[]     = _T("CHAINCODE");      //!< 店舗コード
		const _TCHAR kIdnCode[]       = _T("IDNCODE");        //!< データ識別コード
		const _TCHAR kFeatureItem1[]  = _T("FEATURE_ITEM1");  //!< 紹介文項目1
		const _TCHAR kFeatureItem2[]  = _T("FEATURE_ITEM2");  //!< 紹介文項目2
		const _TCHAR kFeatureItem3[]  = _T("FEATURE_ITEM3");  //!< 紹介文項目3
		const _TCHAR kFeatureItem4[]  = _T("FEATURE_ITEM4");  //!< 紹介文項目4
		const _TCHAR kFeatureItem5[]  = _T("FEATURE_ITEM5");  //!< 紹介文項目5
		const _TCHAR kFeatureItem6[]  = _T("FEATURE_ITEM6");  //!< 紹介文項目6
		const _TCHAR kFeatureItem7[]  = _T("FEATURE_ITEM7");  //!< 紹介文項目7
		const _TCHAR kFeatureItem8[]  = _T("FEATURE_ITEM8");  //!< 紹介文項目8
		const _TCHAR kFeatureItem9[]  = _T("FEATURE_ITEM9");  //!< 紹介文項目9
		const _TCHAR kFeatureItem10[] = _T("FEATURE_ITEM10"); //!< 紹介文項目10
		const _TCHAR kInfoFilePath[]  = _T("INFOFILE_PATH");  //!< 情報ファイル格納先
		const _TCHAR kInfoFileseq[]   = _T("INFOFILE_SEQ");   //!< 情報ファイルシーケンス
	}

	// DTMIDN(データ識別マスタ)
	namespace dtmidn {
		const _TCHAR kTableName[] = _T("DTMIDN");

		const _TCHAR kIdnCode[]        = _T("IDNCODE");    //!< データ識別コード
		const _TCHAR kName[]           = _T("NAME");       //!< 名称
		const _TCHAR kTableNameField[] = _T("TABLE_NAME"); //!< テーブル名称
		const _TCHAR kTableCode[]      = _T("TABLE_CODE"); //!< 格納テーブルコード
	}

	// DTMGLC(データ分類店舗マスタ)
	namespace dtmglc {
		const _TCHAR kTableName[] = _T("DTMGLC");

		const _TCHAR kGrpCode[]      = _T("GRPCODE");        //!< データ分類コード
		const _TCHAR kChainCode[]    = _T("CHAINCODE");      //!< 店舗コード
		const _TCHAR kName[]         = _T("NAME");           //!< 名称
		const _TCHAR kMntGroupCode[] = _T("MNT_GROUP_CODE"); //!< 同時メンテグループコード
		const _TCHAR kCatGroupCode[] = _T("CAT_GROUP_CODE"); //!< 大分類グループコード
		const _TCHAR kContentsType[] = _T("CONTENTS_TYPE");  //!< コンテンツタイプ
	}

	// MSTGRP(データ分類マスタ)
	namespace mstgrp {
		const _TCHAR kTableName[] = _T("MSTGRP");

		const _TCHAR kGrpCode[]    = _T("GRPCODE");    //!< データ分類コード
		const _TCHAR kName[]       = _T("NAME");       //!< 名称
		const _TCHAR kMjrGrpCode[] = _T("MJRGRPCODE"); //!< データ大分類コード
	}

	// MSTIRP(情報提供元マスタ)
	namespace mstipr {
		const _TCHAR kTableName[] = _T("MSTIPR");

		const _TCHAR kIPCode[]      = _T("IPCODE");       //!< 情報提供元コード
		const _TCHAR kName[]        = _T("NAME");         //!< 名称
		const _TCHAR kMntPriority[] = _T("MNT_PRIORITY"); //!< 整備優先順位
	}

	// MSTACC(精度マスタ)
	namespace mstacc {
		const _TCHAR kTableName[] = _T("MSTACC");
				
		const _TCHAR kAccCode[]     = _T("ACCCODE");      //!< 精度コード
		const _TCHAR kName[]        = _T("NAME");         //!< 名称
		const _TCHAR kMntPriority[] = _T("MNT_PRIORITY"); //!< 整備優先順位
	}

	// MASTCLS(閉鎖理由マスタ)
	namespace mstcls {
		const _TCHAR kTableName[] = _T("MSTCLS");

		const _TCHAR kCloseCode[] = _T("CLOSECODE"); //!< 閉鎖理由コード
		const _TCHAR kName[]      = _T("NAME");      //!< 名称
	}

	// MSTFTM(紹介文見出しマスタ)
	namespace mstftm {
		const _TCHAR kTableName[] = _T("MSTFTM");

		const _TCHAR kFeatureMark[] = _T("FEATURE_MARK"); //!< 紹介文見出しコード
		const _TCHAR kName[]        = _T("NAME");         //!< 名称
	}

	// MSTDAY(曜日マスタ)
	namespace mstday {
		const _TCHAR kTableName[] = _T("MSTDAY");

		const _TCHAR kDayCode[] = _T("DAYCODE"); //!< 曜日コード
		const _TCHAR kName[]    = _T("NAME");    //!< 名称
	}

	// MSTPSA(座標値種別マスタ)
	namespace mstpsa {
		const _TCHAR kTableName[] = _T("MSTPSA");

		const _TCHAR kPstAttCode[]  = _T("PST_ATTCODE");  //!< 座標値種別コード
		const _TCHAR kName[]        = _T("NAME");         //!< 名称
		const _TCHAR kPsaPriority[] = _T("PSA_PRIORITY"); //!< 座標値種別間優先順位
		const _TCHAR kMntPriority[] = _T("MNT_PRIORITY"); //!< 整備優先順位
	}

	// MSTPRA(座標値種別内優先順位マスタ)
	namespace mstpra {
		const _TCHAR kTableName[] = _T("MSTPRA");

		const _TCHAR kPriorityAttCode[] = _T("PRIORITY_ATTCODE"); //!< 座標値種別内優先順位種別コード
		const _TCHAR kName[]            = _T("NAME");             //!< 名称
	}

	// MSTSLS(販社マスタ)
	namespace mstsls {
		const _TCHAR kTableName[] = _T("MSTSLS");

		const _TCHAR kSlsCode[]   = _T("SLSCODE");   //!< 販社コード
		const _TCHAR kName[]      = _T("NAME");      //!< 名称
		const _TCHAR kYomi[]      = _T("YOMI");      //!< 読み
		const _TCHAR kGrpCode[]   = _T("GRPCODE");   //!< データ分類コード
		const _TCHAR kChainCode[] = _T("CHAINCODE"); //!< 店舗コード
	}

	// MSTTST(味覚マスタ)
	namespace msttst {
		const _TCHAR kTableName[] = _T("MSTTST");

		const _TCHAR kNo[]    = _T("NO");    //!< 味覚コード
		const _TCHAR kTaste[] = _T("TASTE"); //!< 味覚
	}

	// MSTPCA(POI識別子紐付種別マスタ)
	namespace mstpca {
		const _TCHAR kTableName[] = _T("MSTPCA");

		const _TCHAR kPoiCncAttCode[]    = _T("POI_CNC_ATTCODE");    //!< POI識別子紐付種別コード
		const _TCHAR kName[]             = _T("NAME");               //!< 名称
		const _TCHAR kBankContentsCode[] = _T("BANK_CONTENTS_CODE"); //!< コンテンツコード(銀行)
		const _TCHAR kAtmContentsCode[]  = _T("ATM_CONTENTS_CODE");  //!< コンテンツコード(ATM)
	}

	// DTMDTS(詳細情報仕様管理)
	namespace dtmdts {
		const _TCHAR kTableName[] = _T("DTMDTS");

		const _TCHAR kGrpCode[]     = _T("GRPCODE");       //!< 大分類コード
		const _TCHAR kChainCode[]   = _T("CHAINCODE");     //!< 店舗コード
		const _TCHAR kItemCode[]    = _T("ITEMCODE");      //!< 項目コード
		const _TCHAR kItemMngFlag[] = _T("ITEM_MNG_FLAG"); //!< 項目取扱い管理フラグ
		const _TCHAR kSortOrder[]   = _T("SORT_ORDER");    //!< 並び順
	}

	// MSTDTI(詳細情報項目マスタ)
	namespace mstdti {
		const _TCHAR kTableName[] = _T("MSTDTI");

		const _TCHAR kItemCode[] = _T("ITEMCODE"); //!< 項目コード
		const _TCHAR kName[]     = _T("NAME");     //!< 名称
	}

	// MSTATT(種別マスタ)
	namespace mstatt {
		const _TCHAR kTableName[] = _T("MSTATT");

		const _TCHAR kAttCode[] = _T("ATTCODE"); //!< 種別コード
		const _TCHAR kName[]    = _T("NAME");    //!< 名称
	}

	// MSTCST(地番状態マスタ)
	namespace mstcst {
		const _TCHAR kTableName[] = _T("MSTCST");

		const _TCHAR kChibanStatus[] = _T("CHIBAN_STATUS"); //!< 地番状態コード
		const _TCHAR kName[]         = _T("NAME");          //!< 名称
		const _TCHAR kPnpSetFlag[]   = _T("PNP_SET_FLAG");  //!< ピンポイント設定対象フラグ
	}

	// MSTDIR(方面マスタ)
	namespace mstdir {
		const _TCHAR kTableName[] = _T("MSTDIR");

		const _TCHAR kDirectionCode[] = _T("DIRECTIONCODE"); //!< 方面コード
		const _TCHAR kName[]          = _T("NAME");          //!< 名称
		const _TCHAR kYomi[]          = _T("YOMI");          //!< 読み
	}

	// MSTFCA(道路施設種別マスタ)
	namespace mstfca {
		const _TCHAR kTableName[] = _T("MSTFCA");

		const _TCHAR kFclAttCode[] = _T("FCL_ATTCODE"); //!< 道路施設種別コード
		const _TCHAR kName[]       = _T("NAME");        //!< 名称
		const _TCHAR kYomi[]       = _T("YOMI");        //!< 読み
	}

	// MSTKBN(データ取得区分マスタ)
	namespace mstkbn {
		const _TCHAR kTableName[] = _T("MSTKBN");

		const _TCHAR kDataKubun[] = _T("DATA_KUBUN"); //!< データ区分取得コード
		const _TCHAR kName[]      = _T("NAME");       //!< 名称
	}

	// MSTMSH(2次メッシュコードマスタ)
	namespace mstmsh {
		const _TCHAR kTableName[] = _T("MSTMSH");

		const _TCHAR kMeshCode[] = _T("MESHCODE"); //!< 2次メッシュコード
	}

	// MSTTEL(電番略符号マスタ)
	namespace msttel {
		const _TCHAR kTableName[] = _T("MSTTEL");

		const _TCHAR kTelCode[]    = _T("TELCODE");      //!< 電番略符号
		const _TCHAR kName[]       = _T("NAME");         //!< 名称
		const _TCHAR kTelAttCode[] = _T("TEL_ATTCODE");  //!< 電話番号種別コード
		const _TCHAR kNacSetFlag[] = _T("NAC_SET_FLAG"); //!< 使用禁止設定対象フラグ
	}

	// MSTTKB(電話区分マスタ)
	namespace msttkb {
		const _TCHAR kTableName[] = _T("MSTTKB");

		const _TCHAR kTelKubun[]   = _T("TEL_KUBUN");   //!< 電話区分コード
		const _TCHAR kName[]       = _T("NAME");        //!< 名称
		const _TCHAR kTelAttCode[] = _T("TEL_ATTCODE"); //!< 電話番号種別コード
	}

	// CNGCID(コンテンツ識別子変換)
	namespace cngcid {
		const _TCHAR kTableName[] = _T("CNGCID");

		const _TCHAR kGrpCode[]     = _T("GRPCODE");      //!< データ分類コード
		const _TCHAR kChainCode[]   = _T("CHAINCODE");    //!< 店舗コード
		const _TCHAR kIPCode[]      = _T("IPCODE");       //!< 情報提供元コード
		const _TCHAR kDataKubun[]   = _T("DATA_KUBUN");   //!< データ取得区分コード
		const _TCHAR kAttCode[]     = _T("ATTCODE");      //!< 種別コード
		const _TCHAR kChainCode8d[] = _T("CHAINCODE_8D"); //!< 店舗コード(8桁)
	}

	// CNGDAY(曜日変換)
	namespace cngday {
		const _TCHAR kTableName[] = _T("CNGDAY");

		const _TCHAR kDayCode[]     = _T("DAYCODE");      //!< 曜日コード
		const _TCHAR kDayCodeBit[]  = _T("DAYCODE_BIT");  //!< 曜日コード(ビット)
		const _TCHAR kTermAttCode[] = _T("TERM_ATTCODE"); //!< 期間種別コード
	}

	// CNGDVL(削除データ変換)
	namespace cngdvl {
		const _TCHAR kTableName[] = _T("CNGDVL");

		const _TCHAR kTableNameField[] = _T("TABLE_NAME");    //!< テーブル名称
		const _TCHAR kColumnName[]     = _T("COLUMN_NAME");   //!< カラム名称
		const _TCHAR kFieldsNum[]      = _T("FIELDS_NUM");    //!< ファイル項目位置
		const _TCHAR kDeleteValue[]    = _T("DELETE_VALUE");  //!< 削除文字列
		const _TCHAR kMatchMtdCode[]   = _T("MATCH_MTDCODE"); //!< 一致方法コード
	}

	// CNGPST(座標値種別コード変換)
	namespace cngpst {
		const _TCHAR kTableName[] = _T("CNGPST");

		const _TCHAR kPstAttCode[]    = _T("PST_ATTCODE");     //!< 座標値種別コード
		const _TCHAR kOrgPstAttCode[] = _T("ORG_PST_ATTCODE"); //!< 座標値種別コード(派生元)
		const _TCHAR kTollRoadFlag[]  = _T("TOLLROAD_FLAG");   //!< 高速道路フラグ
	}

	// CNGTRC(鉄道分類変換)
	namespace cngtrc {
		const _TCHAR kTableName[] = _T("CNGTRC");

		const _TCHAR kGrpCode[]          = _T("GRPCODE");           //!< データ分類コード
		const _TCHAR kRecordSeq[]        = _T("RECORD_SEQ");        //!< コンテンツ内シーケンス
		const _TCHAR kTrainTypeCode[]    = _T("TRAINTYPECODE");     //!< 鉄道分類コード
		const _TCHAR kTrainCorpCode[]    = _T("TRAINCORPCODE");     //!< 鉄道会社コード
		const _TCHAR kTrainTypeCodeOld[] = _T("TRAINTYPECODE_OLD"); //!< 鉄道会社コード(旧)
		const _TCHAR kTrcDplFlag[]       = _T("TRC_DPL_FLAG");      //!< 鉄道分類重複フラグ
	}

	// CNGVAL(格納文字列変換)
	namespace cngval {
		const _TCHAR kTableName[] = _T("CNGVAL");

		const _TCHAR kTableNameField[] = _T("TABLE_NAME");  //!< テーブル名称
		const _TCHAR kColumnName[]     = _T("COLUMN_NAME"); //!< カラム名称
		const _TCHAR kValueOld[]       = _T("VALUE_OLD");   //!< 変更前文字列
		const _TCHAR kValueNew[]       = _T("VALUE_NEW");   //!< 変更後文字列
	}

	// MSTA1A(大字・字種別マスタ)
	namespace msta1a {
		const _TCHAR kTableName[] = _T("MSTA1A");

		const _TCHAR kAdr1AttCode[] = _T("ADR1_ATTCODE"); //!< 大字・字種別コード
		const _TCHAR kName[]        = _T("NAME");         //!< 名称
	}

	// MSTA2A(字・小字種別マスタ)
	namespace msta2a {
		const _TCHAR kTableName[] = _T("MSTA2A");

		const _TCHAR kAdr2AttCode[] = _T("ADR2_ATTCODE"); //!< 字・小字種別コード
		const _TCHAR kName[]        = _T("NAME");         //!< 名称
	}

	// MSTAFA(付加機能種別マスタ)
	namespace mstafa {
		const _TCHAR kTableName[] = _T("MSTAFA");

		const _TCHAR kAddFuncAttCode[] = _T("ADD_FUNC_ATTCODE"); //!< 付加機能種別コード
		const _TCHAR kName[]           = _T("NAME");             //!< 名称
	}

	// MSTAFS(付加機能状態マスタ)
	namespace mstafs {
		const _TCHAR kTableName[] = _T("MSTAFS");

		const _TCHAR kAddFuncStatus[] = _T("ADD_FUNC_STATUS"); //!< 付加機能状態コード
		const _TCHAR kName[]          = _T("NAME");            //!< 名称
	}

	// MSTCHG(店舗分類マスタ)
	namespace mstchg {
		const _TCHAR kTableName[] = _T("MSTCHG");

		const _TCHAR kChnGrpCode[] = _T("CHN_GRPCODE"); //!< 店舗分類コード
		const _TCHAR kName[]       = _T("NAME");       //!< 名称
	}

	// MSTCPC(法人格マスタ)
	namespace mstcpc {
		const _TCHAR kTableName[] = _T("MSTCPC");

		const _TCHAR kCorporateClass[] = _T("CORPORATE_CLASS"); //!< 法人格コード
		const _TCHAR kName[]          = _T("NAME");             //!< 名称
	}

	// MSTCPJ(法人格接続位置マスタ)
	namespace mstcpj {
		const _TCHAR kTableName[] = _T("MSTCPJ");

		const _TCHAR kCorporateJoint[] = _T("CORPORATE_JOINT"); //!< 法人格接続位置コード
		const _TCHAR kName[]           = _T("NAME");            //!< 名称
	}

	// MSTFRA(道路施設代表点種別マスタ)
	namespace mstfra {
		const _TCHAR kTableName[] = _T("MSTFRA");

		const _TCHAR kFclRepAttCode[] = _T("FCL_REP_ATTCODE"); //!< 道路施設代表点種別コード
		const _TCHAR kName[]          = _T("NAME");            //!< 名称
	}

	// MSTGND(性別マスタ)
	namespace mstgnd {
		const _TCHAR kTableName[] = _T("MSTGND");

		const _TCHAR kGenderCode[] = _T("GENDERCODE"); //!< 性別コード
		const _TCHAR kName[]       = _T("NAME");       //!< 名称
	}

	// MSTLNG(言語マスタ)
	namespace mstlng {
		const _TCHAR kTableName[] = _T("MSTLNG");

		const _TCHAR kLangCode[] = _T("LANGCODE"); //!< 言語コード
		const _TCHAR kName[]     = _T("NAME");     //!< 名称
	}

	// MSTMGR(データ大分類マスタ)
	namespace mstmgr {
		const _TCHAR kTableName[] = _T("MSTMGR");

		const _TCHAR kMjrGrpCode[] = _T("MJRGRPCODE"); //!< データ大分類コード
		const _TCHAR kName[]       = _T("NAME");       //!< 名称
	}

	// MSTMMD(一致方法マスタ)
	namespace mstmmd {
		const _TCHAR kTableName[] = _T("MSTMMD");

		const _TCHAR kMatchMtdCode[] = _T("MATCH_MTDCODE"); //!< 一致方法コード
		const _TCHAR kName[]         = _T("NAME");          //!< 名称
	}

	// MSTNAV(使用禁止理由マスタ)
	namespace mstnav {
		const _TCHAR kTableName[] = _T("MSTNAV");

		const _TCHAR kNaCode[] = _T("NACODE"); //!< 使用禁止理由コード
		const _TCHAR kName[]   = _T("NAME");   //!< 名称
	}

	// MSTSTF(店舗形態マスタ)
	namespace mststf {
		const _TCHAR kTableName[] = _T("MSTSTF");

		const _TCHAR kStoreForm[] = _T("STORE_FORM"); //!< 店舗形態コード
		const _TCHAR kName[]      = _T("NAME");       //!< 名称
	}

	// MSTTLA(電話番号種別マスタ)
	namespace msttla {
		const _TCHAR kTableName[] = _T("MSTTLA");

		const _TCHAR kTelAttCode[] = _T("TEL_ATTCODE"); //!< 電話番号種別コード
		const _TCHAR kName[]       = _T("NAME");        //!< 名称
	}

	// MSTTMA(期間種別マスタ)
	namespace msttma {
		const _TCHAR kTableName[] = _T("MSTTMA");

		const _TCHAR kTermAttCode[] = _T("TERM_ATTCODE"); //!< 期間種別コード
		const _TCHAR kName[]        = _T("NAME");         //!< 名称
	}

	// MSTUDA(上下線種別マスタ)
	namespace mstuda {
		const _TCHAR kTableName[] = _T("MSTUDA");

		const _TCHAR kUpDownAttCode[] = _T("UPDOWN_ATTCODE"); //!< 上下線種別コード
		const _TCHAR kName[]          = _T("NAME");           //!< 名称
	}

	// MSTZPA(郵便番号種別マスタ)
	namespace mstzpa {
		const _TCHAR kTableName[] = _T("MSTZPA");

		const _TCHAR kZipAttCode[] = _T("ZIP_ATTCODE"); //!< 郵便番号種別コード
		const _TCHAR kName[]       = _T("NAME");        //!< 名称
	}

	// CNGACC(精度変換TB)
	namespace cngacc {
		const _TCHAR kTableName[] = _T("CNGACC");

		const _TCHAR kAccCodeDB[]   = _T("ACCCODE_DB");   //!< 精度コード(DB格納値)
		const _TCHAR kAccCodeAddr[] = _T("ACCCODE_ADDR"); //!< 精度コード(住所)
	}

	// MSTFCD(機能詳細)
	namespace mstfcd {
		const _TCHAR kTableName[] = _T("MSTFCD");
				
		const _TCHAR kIdnCode[]          = _T("IDNCODE");            //!< データ識別コード
		const _TCHAR kSortOrder[]        = _T("SORT_ORDER");         //!< 並び順
		const _TCHAR kColumnName[]       = _T("COLUMN_NAME");        //!< 列名
		const _TCHAR kSiriusColumnName[] = _T("SIRIUS_COLUMN_NAME"); //!< SS-Format列定義
	}

	// MSTREF(参照バージョンマスタ)
	namespace mstref {
		const _TCHAR kTableName[] = _T("MSTREF");

		const _TCHAR kMasterCode[]    = _T("MASTER_CODE");     //!< マスタコード
		const _TCHAR kDataLot[]       = _T("DATALOT");         //!< データロット
		const _TCHAR kWorkName[]      = _T("WORK_NAME");       //!< 作業名
		const _TCHAR kAddrVersion[]   = _T("ADDR_VERSION");    //!< 住所リリースユーザ
		const _TCHAR kDBConnectInfo[] = _T("DB_CONNECT_INFO"); //!< SiNDYDB接続情報
		const _TCHAR kVersionCode[]   = _T("VERSION_CODE");    //!< マスタバージョンコード
	}

	// MSTMNT(同時メンテグループマスタ)
	namespace mstmnt {
		const _TCHAR kTableName[] = _T("MSTMNT");

		const _TCHAR kMntCode[] = _T("MNT_CODE"); //!< 同時メンテグループコード
		const _TCHAR kName[]    = _T("NAME");     //!< 名称
	}

	// MSTCAT(大分類グループマスタ)
	namespace mstcat {
		const _TCHAR kTableName[] = _T("MSTCAT");

		const _TCHAR kCatCode[] = _T("CAT_CODE"); //!< 大分類コード
		const _TCHAR kName[]    = _T("NAME");     //!< 名称
	}

	// MSTTBL(機能格納先テーブルマスタ)
	namespace msttbl {
		const _TCHAR kTableName[] = _T("MSTTBL");

		const _TCHAR kTblCode[] = _T("TBL_CODE"); //!< 機能格納先テーブルコード
		const _TCHAR kName[]    = _T("NAME");     //!< 名称
	}

	// CNGTLA(特定市外局番マスタ)
	namespace cngtla {
		const _TCHAR kTableName[] = _T("CNGTLA");

		const _TCHAR kTel[]        = _T("TEL_C");       //!< 市外局番
		const _TCHAR kTelAttCode[] = _T("TEL_ATTCODE"); //!< 特定市外局番コード
	}

	// CNGADR(住所コード変換)
	namespace cngadr {
		const _TCHAR kTableName[] = _T("CNGADR");

		const _TCHAR kPrefCodeOld[]  = _T("PREFCODE_OLD");  //!< 旧:都道府県コード
		const _TCHAR kCityCodeOld[]  = _T("CITYCODE_OLD");  //!< 旧:市区町村コード
		const _TCHAR kPrefNameOld[]  = _T("PREFNAME_OLD");  //!< 旧:都道府県名称
		const _TCHAR kCityNameOld[]  = _T("CITYNAME_OLD");  //!< 旧:市区町村名称
		const _TCHAR kPrefCodeNew[]  = _T("PREFCODE_NEW");  //!< 新:都道府県コード
		const _TCHAR kCityCodeNew[]  = _T("CITYCODE_NEW");  //!< 新:市区町村コード
		const _TCHAR kPrefNameNew[]  = _T("PREFNAME_NEW");  //!< 新:都道府県名称
		const _TCHAR kCityNameNew[]  = _T("CITYNAME_NEW");  //!< 新:市区町村名称
		const _TCHAR kSheetSeq[]     = _T("SHEET_SEQ");     //!< EXCELシートシーケンス
		const _TCHAR kEnforcedDate[] = _T("ENFORCED_DATE"); //!< 実施日付
	}

	// MSTCHN(店舗マスタ)
	namespace mstchn {
		const _TCHAR kTableName[] = _T("MSTCHN");

		const _TCHAR kChainCode[]   = _T("CHAINCODE");    //!< 店舗コード
		const _TCHAR kChaincode8d[] = _T("CHAINCODE_8D"); //!< 店舗コード(8桁)
		const _TCHAR kChnGrpCode[]  = _T("CHN_GRPCODE");  //!< 店舗分類コード
		const _TCHAR kName[]        = _T("NAME");         //!< 名称
		const _TCHAR kYomi[]        = _T("YOMI");         //!< 読み
		const _TCHAR kTwnSetFlag[]  = _T("TWN_SET_FLAG"); //!< タウンページ設定対象フラグ
		const _TCHAR kGnrSetFlag[]  = _T("GNR_SET_FLAG"); //!< 一般POI設定対象フラグ
	}

	// MSTNSS
	namespace mstnss {
		const _TCHAR kTableName[] = _T("MSTNSS");

		const _TCHAR kNssCode[] = _T("NSSCODE"); //!< NSS業種コード
		const _TCHAR kName[]    = _T("NAME");    //!< 名称
	}

	// MSTNTT
	namespace mstntt {
		const _TCHAR kTableName[] = _T("MSTNTT");

		const _TCHAR kNttCode[]    = _T("NTTCODE");    //!< NTT業種コード
		const _TCHAR kName[]       = _T("NAME");       //!< NTT業種名称
		const _TCHAR kNttGrpCode[] = _T("NTTGRPCODE"); //!< NTT基本分類コード
		const _TCHAR kGrpCode[]    = _T("GRPCODE");    //!< データ分類コード
	}

	// CNGTEL(局番変換)
	namespace cngtel {
		const _TCHAR kTableName[] = _T("CNGTEL");

		const _TCHAR kTelInfOld[]    = _T("TEL_INF_OLD");   //!< 番号領域
		const _TCHAR kTelCOld[]      = _T("TEL_C_OLD");     //!< 変更前市外局番
		const _TCHAR kTelCNew[]      = _T("TEL_C_NEW");     //!< 変更後市外局番
		const _TCHAR kTelAddNew[]    = _T("TEL_ADD_NEW");   //!< 変更後付加情報
		const _TCHAR kDigitOld[]     = _T("DIGIT_OLD");     //!< 変更前番号桁数
		const _TCHAR kDigitNew[]     = _T("DIGIT_NEW");     //!< 変更後番号桁数
		const _TCHAR kTgtArea[]      = _T("TGT_AREA");      //!< 対象地域
		const _TCHAR kCngTelFlag[]   = _T("CNG_TEL_FLAG");  //!< 局番変換対象フラグ
		const _TCHAR kEnforcedDate[] = _T("ENFORCED_DATE"); //!< 実施日付
	}

	// GNM001
	namespace gnm001 {
		const _TCHAR kTableName[] = _T("GNM001");

		const _TCHAR kIPCode[]        = _T("IPCODE");         //!< 情報提供元コード
		const _TCHAR kDataKubun[]     = _T("DATA_KUBUN");     //!< データ取得区分コード
		const _TCHAR kAttCode[]       = _T("ATTCODE");        //!< 種別コード
		const _TCHAR kFeatureItem1[]  = _T("FEATURE_ITEM1");  //!< 紹介文項目1
		const _TCHAR kFeatureItem2[]  = _T("FEATURE_ITEM2");  //!< 紹介文項目2
		const _TCHAR kFeatureItem3[]  = _T("FEATURE_ITEM3");  //!< 紹介文項目3
		const _TCHAR kFeatureItem4[]  = _T("FEATURE_ITEM4");  //!< 紹介文項目4
		const _TCHAR kFeatureItem5[]  = _T("FEATURE_ITEM5");  //!< 紹介文項目5
		const _TCHAR kFeatureItem6[]  = _T("FEATURE_ITEM6");  //!< 紹介文項目6
		const _TCHAR kFeatureItem7[]  = _T("FEATURE_ITEM7");  //!< 紹介文項目7
		const _TCHAR kFeatureItem8[]  = _T("FEATURE_ITEM8");  //!< 紹介文項目8
		const _TCHAR kFeatureItem9[]  = _T("FEATURE_ITEM9");  //!< 紹介文項目9
		const _TCHAR kFeatureItem10[] = _T("FEATURE_ITEM10"); //!< 紹介文項目10
		const _TCHAR kFeatureItem11[] = _T("FEATURE_ITEM11"); //!< 紹介文項目11
		const _TCHAR kFeatureItem12[] = _T("FEATURE_ITEM12"); //!< 紹介文項目12
		const _TCHAR kFeatureItem13[] = _T("FEATURE_ITEM13"); //!< 紹介文項目13
		const _TCHAR kFeatureItem14[] = _T("FEATURE_ITEM14"); //!< 紹介文項目14
		const _TCHAR kFeatureItem15[] = _T("FEATURE_ITEM15"); //!< 紹介文項目15
		const _TCHAR kFeatureItem16[] = _T("FEATURE_ITEM16"); //!< 紹介文項目16
		const _TCHAR kFeatureItem17[] = _T("FEATURE_ITEM17"); //!< 紹介文項目17
		const _TCHAR kFeatureItem18[] = _T("FEATURE_ITEM18"); //!< 紹介文項目18
		const _TCHAR kFeatureItem19[] = _T("FEATURE_ITEM19"); //!< 紹介文項目19
		const _TCHAR kFeatureItem20[] = _T("FEATURE_ITEM20"); //!< 紹介文項目20
	}

	// MSTEMS(緊急避難所コードマスタ)
	namespace mstems {
		const _TCHAR kTableName[] = _T("MSTEMS");

		const _TCHAR kCode[] = _T("EMS_CODE"); //!< 緊急避難所コード
		const _TCHAR kName[] = _T("NAME"); //!< 名称
	}
} // ssmaster
} // schema
} //sindy
