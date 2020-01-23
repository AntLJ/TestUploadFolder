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

namespace sindy {
namespace schema {
namespace ssmaster {

	// DTMDSP(データ仕様管理)
	namespace dtmdsp {
		extern const _TCHAR kTableName[];

		extern const _TCHAR kGrpCode[];       //!< データ分類コード
		extern const _TCHAR kChainCode[];     //!< 店舗コード
		extern const _TCHAR kIdnCode[];       //!< データ識別コード
		extern const _TCHAR kFeatureItem1[];  //!< 紹介文項目1
		extern const _TCHAR kFeatureItem2[];  //!< 紹介文項目2
		extern const _TCHAR kFeatureItem3[];  //!< 紹介文項目3
		extern const _TCHAR kFeatureItem4[];  //!< 紹介文項目4
		extern const _TCHAR kFeatureItem5[];  //!< 紹介文項目5
		extern const _TCHAR kFeatureItem6[];  //!< 紹介文項目6
		extern const _TCHAR kFeatureItem7[];  //!< 紹介文項目7
		extern const _TCHAR kFeatureItem8[];  //!< 紹介文項目8
		extern const _TCHAR kFeatureItem9[];  //!< 紹介文項目9
		extern const _TCHAR kFeatureItem10[]; //!< 紹介文項目10
		extern const _TCHAR kInfoFilePath[];  //!< 情報ファイル格納先
		extern const _TCHAR kInfoFileseq[];   //!< 情報ファイルシーケンス
	}

	// DTMIDN(データ識別マスタ)
	namespace dtmidn {
		extern const _TCHAR kTableName[];

		extern const _TCHAR kIdnCode[];        //!< データ識別コード
		extern const _TCHAR kName[];           //!< 名称
		extern const _TCHAR kTableNameField[]; //!< テーブル名称
		extern const _TCHAR kTableCode[];      //!< 格納テーブルコード
	}

	// DTMGLC(データ分類店舗マスタ)
	namespace dtmglc {
		extern const _TCHAR kTableName[];

		extern const _TCHAR kGrpCode[];      //!< データ分類コード
		extern const _TCHAR kChainCode[];    //!< 店舗コード
		extern const _TCHAR kName[];         //!< 名称
		extern const _TCHAR kMntGroupCode[]; //!< 同時メンテグループコード
		extern const _TCHAR kCatGroupCode[]; //!< 大分類グループコード
		extern const _TCHAR kContentsType[]; //!< コンテンツタイプ
	}

	// MSTGRP(データ分類マスタ)
	namespace mstgrp {
		extern const _TCHAR kTableName[];

		extern const _TCHAR kGrpCode[];    //!< データ分類コード
		extern const _TCHAR kName[];       //!< 名称
		extern const _TCHAR kMjrGrpCode[]; //!< データ大分類コード
	}

	// MSTIRP(情報提供元マスタ)
	namespace mstipr {
		extern const _TCHAR kTableName[];

		extern const _TCHAR kIPCode[];      //!< 情報提供元コード
		extern const _TCHAR kName[];        //!< 名称
		extern const _TCHAR kMntPriority[]; //!< 整備優先順位
	}

	// MSTACC(精度マスタ)
	namespace mstacc {
		extern const _TCHAR kTableName[];
				
		extern const _TCHAR kAccCode[];     //!< 精度コード
		extern const _TCHAR kName[];        //!< 名称
		extern const _TCHAR kMntPriority[]; //!< 整備優先順位
	}

	// MASTCLS(閉鎖理由マスタ)
	namespace mstcls {
		extern const _TCHAR kTableName[];

		extern const _TCHAR kCloseCode[]; //!< 閉鎖理由コード
		extern const _TCHAR kName[];      //!< 名称
	}

	// MSTFTM(紹介文見出しマスタ)
	namespace mstftm {
		extern const _TCHAR kTableName[];

		extern const _TCHAR kFeatureMark[]; //!< 紹介文見出しコード
		extern const _TCHAR kName[];        //!< 名称
	}

	// MSTDAY(曜日マスタ)
	namespace mstday {
		extern const _TCHAR kTableName[];

		extern const _TCHAR kDayCode[]; //!< 曜日コード
		extern const _TCHAR kName[];    //!< 名称
	}

	// MSTPSA(座標値種別マスタ)
	namespace mstpsa {
		extern const _TCHAR kTableName[];

		extern const _TCHAR kPstAttCode[];  //!< 座標値種別コード
		extern const _TCHAR kName[];        //!< 名称
		extern const _TCHAR kPsaPriority[]; //!< 座標値種別間優先順位
		extern const _TCHAR kMntPriority[]; //!< 整備優先順位
	}

	// MSTPRA(座標値種別内優先順位マスタ)
	namespace mstpra {
		extern const _TCHAR kTableName[];

		extern const _TCHAR kPriorityAttCode[]; //!< 座標値種別内優先順位種別コード
		extern const _TCHAR kName[];            //!< 名称
	}

	// MSTSLS(販社マスタ)
	namespace mstsls {
		extern const _TCHAR kTableName[];

		extern const _TCHAR kSlsCode[];   //!< 販社コード
		extern const _TCHAR kName[];      //!< 名称
		extern const _TCHAR kYomi[];      //!< 読み
		extern const _TCHAR kGrpCode[];   //!< データ分類コード
		extern const _TCHAR kChainCode[]; //!< 店舗コード
	}

	// MSTTST(味覚マスタ)
	namespace msttst {
		extern const _TCHAR kTableName[];

		extern const _TCHAR kNo[];    //!< 味覚コード
		extern const _TCHAR kTaste[]; //!< 味覚
	}

	// MSTPCA(POI識別子紐付種別マスタ)
	namespace mstpca {
		extern const _TCHAR kTableName[];

		extern const _TCHAR kPoiCncAttCode[];    //!< POI識別子紐付種別コード
		extern const _TCHAR kName[];             //!< 名称
		extern const _TCHAR kBankContentsCode[]; //!< コンテンツコード(銀行)
		extern const _TCHAR kAtmContentsCode[];  //!< コンテンツコード(ATM)
	}

	// DTMDTS(詳細情報仕様管理)
	namespace dtmdts {
		extern const _TCHAR kTableName[];

		extern const _TCHAR kGrpCode[];     //!< 大分類コード
		extern const _TCHAR kChainCode[];   //!< 店舗コード
		extern const _TCHAR kItemCode[];    //!< 項目コード
		extern const _TCHAR kItemMngFlag[]; //!< 項目取扱い管理フラグ
		extern const _TCHAR kSortOrder[];   //!< 並び順
	}

	// MSTDTI(詳細情報項目マスタ)
	namespace mstdti {
		extern const _TCHAR kTableName[];

		extern const _TCHAR kItemCode[]; //!< 項目コード
		extern const _TCHAR kName[];     //!< 名称
	}

	// MSTATT(種別マスタ)
	namespace mstatt {
		extern const _TCHAR kTableName[];

		extern const _TCHAR kAttCode[]; //!< 種別コード
		extern const _TCHAR kName[];    //!< 名称
	}

	// MSTCST(地番状態マスタ)
	namespace mstcst {
		extern const _TCHAR kTableName[];

		extern const _TCHAR kChibanStatus[]; //!< 地番状態コード
		extern const _TCHAR kName[];         //!< 名称
		extern const _TCHAR kPnpSetFlag[];   //!< ピンポイント設定対象フラグ
	}

	// MSTDIR(方面マスタ)
	namespace mstdir {
		extern const _TCHAR kTableName[];

		extern const _TCHAR kDirectionCode[]; //!< 方面コード
		extern const _TCHAR kName[];          //!< 名称
		extern const _TCHAR kYomi[];          //!< 読み
	}

	// MSTFCA(道路施設種別マスタ)
	namespace mstfca {
		extern const _TCHAR kTableName[];

		extern const _TCHAR kFclAttCode[]; //!< 道路施設種別コード
		extern const _TCHAR kName[];       //!< 名称
		extern const _TCHAR kYomi[];       //!< 読み
	}

	// MSTKBN(データ取得区分マスタ)
	namespace mstkbn {
		extern const _TCHAR kTableName[];

		extern const _TCHAR kDataKubun[]; //!< データ区分取得コード
		extern const _TCHAR kName[];      //!< 名称
	}

	// MSTMSH(2次メッシュコードマスタ)
	namespace mstmsh {
		extern const _TCHAR kTableName[];

		extern const _TCHAR kMeshCode[]; //!< 2次メッシュコード
	}

	// MSTTEL(電番略符号マスタ)
	namespace msttel {
		extern const _TCHAR kTableName[];

		extern const _TCHAR kTelCode[];    //!< 電番略符号
		extern const _TCHAR kName[];       //!< 名称
		extern const _TCHAR kTelAttCode[]; //!< 電話番号種別コード
		extern const _TCHAR kNacSetFlag[]; //!< 使用禁止設定対象フラグ
	}

	// MSTTKB(電話区分マスタ)
	namespace msttkb {
		extern const _TCHAR kTableName[];

		extern const _TCHAR kTelKubun[];   //!< 電話区分コード
		extern const _TCHAR kName[];       //!< 名称
		extern const _TCHAR kTelAttCode[]; //!< 電話番号種別コード
	}

	// CNGCID(コンテンツ識別子変換)
	namespace cngcid {
		extern const _TCHAR kTableName[];

		extern const _TCHAR kGrpCode[];     //!< データ分類コード
		extern const _TCHAR kChainCode[];   //!< 店舗コード
		extern const _TCHAR kIPCode[];      //!< 情報提供元コード
		extern const _TCHAR kDataKubun[];   //!< データ取得区分コード
		extern const _TCHAR kAttCode[];     //!< 種別コード
		extern const _TCHAR kChainCode8d[]; //!< 店舗コード(8桁)
	}

	// CNGDAY(曜日変換)
	namespace cngday {
		extern const _TCHAR kTableName[];

		extern const _TCHAR kDayCode[];     //!< 曜日コード
		extern const _TCHAR kDayCodeBit[];  //!< 曜日コード(ビット)
		extern const _TCHAR kTermAttCode[]; //!< 期間種別コード
	}

	// CNGDVL(削除データ変換)
	namespace cngdvl {
		extern const _TCHAR kTableName[];

		extern const _TCHAR kTableNameField[]; //!< テーブル名称
		extern const _TCHAR kColumnName[];     //!< カラム名称
		extern const _TCHAR kFieldsNum[];      //!< ファイル項目位置
		extern const _TCHAR kDeleteValue[];    //!< 削除文字列
		extern const _TCHAR kMatchMtdCode[];   //!< 一致方法コード
	}

	// CNGPST(座標値種別コード変換)
	namespace cngpst {
		extern const _TCHAR kTableName[];

		extern const _TCHAR kPstAttCode[];    //!< 座標値種別コード
		extern const _TCHAR kOrgPstAttCode[]; //!< 座標値種別コード(派生元)
		extern const _TCHAR kTollRoadFlag[];  //!< 高速道路フラグ
	}

	// CNGTRC(鉄道分類変換)
	namespace cngtrc {
		extern const _TCHAR kTableName[];

		extern const _TCHAR kGrpCode[];          //!< データ分類コード
		extern const _TCHAR kRecordSeq[];        //!< コンテンツ内シーケンス
		extern const _TCHAR kTrainTypeCode[];    //!< 鉄道分類コード
		extern const _TCHAR kTrainCorpCode[];    //!< 鉄道会社コード
		extern const _TCHAR kTrainTypeCodeOld[]; //!< 鉄道会社コード(旧)
		extern const _TCHAR kTrcDplFlag[];       //!< 鉄道分類重複フラグ
	}

	// CNGVAL(格納文字列変換)
	namespace cngval {
		extern const _TCHAR kTableName[];

		extern const _TCHAR kTableNameField[]; //!< テーブル名称
		extern const _TCHAR kColumnName[];     //!< カラム名称
		extern const _TCHAR kValueOld[];       //!< 変更前文字列
		extern const _TCHAR kValueNew[];       //!< 変更後文字列
	}

	// MSTA1A(大字・字種別マスタ)
	namespace msta1a {
		extern const _TCHAR kTableName[];

		extern const _TCHAR kAdr1AttCode[]; //!< 大字・字種別コード
		extern const _TCHAR kName[];        //!< 名称
	}

	// MSTA2A(字・小字種別マスタ)
	namespace msta2a {
		extern const _TCHAR kTableName[];

		extern const _TCHAR kAdr2AttCode[]; //!< 字・小字種別コード
		extern const _TCHAR kName[];        //!< 名称
	}

	// MSTAFA(付加機能種別マスタ)
	namespace mstafa {
		extern const _TCHAR kTableName[];

		extern const _TCHAR kAddFuncAttCode[]; //!< 付加機能種別コード
		extern const _TCHAR kName[];           //!< 名称
	}

	// MSTAFS(付加機能状態マスタ)
	namespace mstafs {
		extern const _TCHAR kTableName[];

		extern const _TCHAR kAddFuncStatus[]; //!< 付加機能状態コード
		extern const _TCHAR kName[];          //!< 名称
	}

	// MSTCHG(店舗分類マスタ)
	namespace mstchg {
		extern const _TCHAR kTableName[];

		extern const _TCHAR kChnGrpCode[]; //!< 店舗分類コード
		extern const _TCHAR kName[];       //!< 名称
	}

	// MSTCPC(法人格マスタ)
	namespace mstcpc {
		extern const _TCHAR kTableName[];

		extern const _TCHAR kCorporateClass[]; //!< 法人格コード
		extern const _TCHAR kName[];          //!< 名称
	}

	// MSTCPJ(法人格接続位置マスタ)
	namespace mstcpj {
		extern const _TCHAR kTableName[];

		extern const _TCHAR kCorporateJoint[]; //!< 法人格接続位置コード
		extern const _TCHAR kName[];           //!< 名称
	}

	// MSTFRA(道路施設代表点種別マスタ)
	namespace mstfra {
		extern const _TCHAR kTableName[];

		extern const _TCHAR kFclRepAttCode[]; //!< 道路施設代表点種別コード
		extern const _TCHAR kName[];         //!< 名称
	}

	// MSTGND(性別マスタ)
	namespace mstgnd {
		extern const _TCHAR kTableName[];

		extern const _TCHAR kGenderCode[]; //!< 性別コード
		extern const _TCHAR kName[];       //!< 名称
	}

	// MSTLNG(言語マスタ)
	namespace mstlng {
		extern const _TCHAR kTableName[];

		extern const _TCHAR kLangCode[]; //!< 言語コード
		extern const _TCHAR kName[];     //!< 名称
	}

	// MSTMGR(データ大分類マスタ)
	namespace mstmgr {
		extern const _TCHAR kTableName[];

		extern const _TCHAR kMjrGrpCode[]; //!< データ大分類コード
		extern const _TCHAR kName[];       //!< 名称
	}

	// MSTMMD(一致方法マスタ)
	namespace mstmmd {
		extern const _TCHAR kTableName[];

		extern const _TCHAR kMatchMtdCode[]; //!< 一致方法コード
		extern const _TCHAR kName[];         //!< 名称
	}

	// MSTNAV(使用禁止理由マスタ)
	namespace mstnav {
		extern const _TCHAR kTableName[];

		extern const _TCHAR kNaCode[]; //!< 使用禁止理由コード
		extern const _TCHAR kName[];   //!< 名称
	}

	// MSTSTF(店舗形態マスタ)
	namespace mststf {
		extern const _TCHAR kTableName[];

		extern const _TCHAR kStoreForm[]; //!< 店舗形態コード
		extern const _TCHAR kName[];      //!< 名称
	}

	// MSTTLA(電話番号種別マスタ)
	namespace msttla {
		extern const _TCHAR kTableName[];

		extern const _TCHAR kTelAttCode[]; //!< 電話番号種別コード
		extern const _TCHAR kName[];       //!< 名称
	}

	// MSTTMA(期間種別マスタ)
	namespace msttma {
		extern const _TCHAR kTableName[];

		extern const _TCHAR kTermAttCode[]; //!< 期間種別コード
		extern const _TCHAR kName[];        //!< 名称
	}

	// MSTUDA(上下線種別マスタ)
	namespace mstuda {
		extern const _TCHAR kTableName[];

		extern const _TCHAR kUpDownAttCode[]; //!< 上下線種別コード
		extern const _TCHAR kName[];          //!< 名称
	}

	// MSTZPA(郵便番号種別マスタ)
	namespace mstzpa {
		extern const _TCHAR kTableName[];

		extern const _TCHAR kZipAttCode[]; //!< 郵便番号種別コード
		extern const _TCHAR kName[];       //!< 名称
	}

	// CNGACC(精度変換TB)
	namespace cngacc {
		extern const _TCHAR kTableName[];

		extern const _TCHAR kAccCodeDB[];   //!< 精度コード(DB格納値)
		extern const _TCHAR kAccCodeAddr[]; //!< 精度コード(住所)
	}

	// MSTFCD(機能詳細)
	namespace mstfcd {
		extern const _TCHAR kTableName[];
				
		extern const _TCHAR kIdnCode[];          //!< データ識別コード
		extern const _TCHAR kSortOrder[];        //!< 並び順
		extern const _TCHAR kColumnName[];       //!< 列名
		extern const _TCHAR kSiriusColumnName[]; //!< SS-Format列定義
	}

	// MSTREF(参照バージョンマスタ)
	namespace mstref {
		extern const _TCHAR kTableName[];

		extern const _TCHAR kMasterCode[];    //!< マスタコード
		extern const _TCHAR kDataLot[];       //!< データロット
		extern const _TCHAR kWorkName[];      //!< 作業名
		extern const _TCHAR kAddrVersion[];   //!< 住所リリースユーザ
		extern const _TCHAR kDBConnectInfo[]; //!< SiNDYDB接続情報
		extern const _TCHAR kVersionCode[];   //!< マスタバージョンコード
	}

	// MSTMNT(同時メンテグループマスタ)
	namespace mstmnt {
		extern const _TCHAR kTableName[];

		extern const _TCHAR kMntCode[]; //!< 同時メンテグループコード
		extern const _TCHAR kName[];    //!< 名称
	}

	// MSTCAT(大分類グループマスタ)
	namespace mstcat {
		extern const _TCHAR kTableName[];

		extern const _TCHAR kCatCode[]; //!< 大分類コード
		extern const _TCHAR kName[];    //!< 名称
	}

	// MSTTBL(機能格納先テーブルマスタ)
	namespace msttbl {
		extern const _TCHAR kTableName[];

		extern const _TCHAR kTblCode[]; //!< 機能格納先テーブルコード
		extern const _TCHAR kName[];    //!< 名称
	}

	// CNGTLA(特定市外局番マスタ)
	namespace cngtla {
		extern const _TCHAR kTableName[];

		extern const _TCHAR kTel[];        //!< 市外局番
		extern const _TCHAR kTelAttCode[]; //!< 特定市外局番コード
	}

	// CNGADR(住所コード変換)
	namespace cngadr {
		extern const _TCHAR kTableName[];

		extern const _TCHAR kPrefCodeOld[];  //!< 旧:都道府県コード
		extern const _TCHAR kCityCodeOld[];  //!< 旧:市区町村コード
		extern const _TCHAR kPrefNameOld[];  //!< 旧:都道府県名称
		extern const _TCHAR kCityNameOld[];  //!< 旧:市区町村名称
		extern const _TCHAR kPrefCodeNew[];  //!< 新:都道府県コード
		extern const _TCHAR kCityCodeNew[];  //!< 新:市区町村コード
		extern const _TCHAR kPrefNameNew[];  //!< 新:都道府県名称
		extern const _TCHAR kCityNameNew[];  //!< 新:市区町村名称
		extern const _TCHAR kSheetSeq[];     //!< EXCELシートシーケンス
		extern const _TCHAR kEnforcedDate[]; //!< 実施日付
	}

	// MSTCHN(店舗マスタ)
	namespace mstchn {
		extern const _TCHAR kTableName[];

		extern const _TCHAR kChainCode[];   //!< 店舗コード
		extern const _TCHAR kChaincode8d[]; //!< 店舗コード(8桁)
		extern const _TCHAR kChnGrpCode[];  //!< 店舗分類コード
		extern const _TCHAR kName[];        //!< 名称
		extern const _TCHAR kYomi[];        //!< 読み
		extern const _TCHAR kTwnSetFlag[];  //!< タウンページ設定対象フラグ
		extern const _TCHAR kGnrSetFlag[];  //!< 一般POI設定対象フラグ
	}

	// MSTNSS
	namespace mstnss {
		extern const _TCHAR kTableName[];

		extern const _TCHAR kNssCode[]; //!< NSS業種コード
		extern const _TCHAR kName[];    //!< 名称
	}

	// MSTNTT
	namespace mstntt {
		extern const _TCHAR kTableName[];

		extern const _TCHAR kNttCode[];    //!< NTT業種コード
		extern const _TCHAR kName[];       //!< NTT業種名称
		extern const _TCHAR kNttGrpCode[]; //!< NTT基本分類コード
		extern const _TCHAR kGrpCode[];    //!< データ分類コード
	}

	// CNGTEL(局番変換)
	namespace cngtel {
		extern const _TCHAR kTableName[];

		extern const _TCHAR kTelInfOld[];    //!< 番号領域
		extern const _TCHAR kTelCOld[];      //!< 変更前市外局番
		extern const _TCHAR kTelCNew[];      //!< 変更後市外局番
		extern const _TCHAR kTelAddNew[];    //!< 変更後付加情報
		extern const _TCHAR kDigitOld[];     //!< 変更前番号桁数
		extern const _TCHAR kDigitNew[];     //!< 変更後番号桁数
		extern const _TCHAR kTgtArea[];      //!< 対象地域
		extern const _TCHAR kCngTelFlag[];   //!< 局番変換対象フラグ
		extern const _TCHAR kEnforcedDate[]; //!< 実施日付
	}

	// GNM001
	namespace gnm001 {
		extern const _TCHAR kTableName[];

		extern const _TCHAR kIPCode[];        //!< 情報提供元コード
		extern const _TCHAR kDataKubun[];     //!< データ取得区分コード
		extern const _TCHAR kAttCode[];       //!< 種別コード
		extern const _TCHAR kFeatureItem1[];  //!< 紹介文項目1
		extern const _TCHAR kFeatureItem2[];  //!< 紹介文項目2
		extern const _TCHAR kFeatureItem3[];  //!< 紹介文項目3
		extern const _TCHAR kFeatureItem4[];  //!< 紹介文項目4
		extern const _TCHAR kFeatureItem5[];  //!< 紹介文項目5
		extern const _TCHAR kFeatureItem6[];  //!< 紹介文項目6
		extern const _TCHAR kFeatureItem7[];  //!< 紹介文項目7
		extern const _TCHAR kFeatureItem8[];  //!< 紹介文項目8
		extern const _TCHAR kFeatureItem9[];  //!< 紹介文項目9
		extern const _TCHAR kFeatureItem10[]; //!< 紹介文項目10
		extern const _TCHAR kFeatureItem11[]; //!< 紹介文項目11
		extern const _TCHAR kFeatureItem12[]; //!< 紹介文項目12
		extern const _TCHAR kFeatureItem13[]; //!< 紹介文項目13
		extern const _TCHAR kFeatureItem14[]; //!< 紹介文項目14
		extern const _TCHAR kFeatureItem15[]; //!< 紹介文項目15
		extern const _TCHAR kFeatureItem16[]; //!< 紹介文項目16
		extern const _TCHAR kFeatureItem17[]; //!< 紹介文項目17
		extern const _TCHAR kFeatureItem18[]; //!< 紹介文項目18
		extern const _TCHAR kFeatureItem19[]; //!< 紹介文項目19
		extern const _TCHAR kFeatureItem20[]; //!< 紹介文項目20
	}

	// MSTEMS(緊急避難所コードマスタ)
	namespace mstems {
		extern const _TCHAR kTableName[];

		extern const _TCHAR kCode[];  //!< 緊急避難所コード
		extern const _TCHAR kName[];  //!< 名称
	}
} // ssmaster
} // schema
} //sindy
