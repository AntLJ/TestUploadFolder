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

// コマンドライン引数
const LPCSTR REPDB = "rep_db";
const LPCSTR REPSCHEMA = "rep_schema";
const LPCSTR ILLSET = "illust_settings";
const LPCSTR INPUTLIST = "input_list";
const LPCSTR EGGDB = "egg_db";
const LPCSTR EGGSCHEMA = "egg_schema";
const LPCSTR RUNLOG = "runlog";
const LPCSTR ERRLOG = "errlog";


// イラスト処理情報設定ファイルフィールド数
const long ILLSETFNUM = 7;

// イラスト処理情報設定ファイル用構造体
struct IllSetRecord
{
	CString e_illCls;			// イラスト種別コード
	CString e_dstCode;			// 仕向け先コード
	CString e_prefix;			// イラストファイルのプレフィックス
	CString e_suffix;			// イラストファイルの拡張子
	CString e_nameRule;			// イラストファイル命名規則
	CString e_illDir;			// イラストファイル格納ディレクトリ
	CString e_excCond;			// 出力除外条件
};

// イラストファイル名フィールド数
const long ILLFILEFNUM = 5;

// イラストファイル名対応表用構造体
struct ClassCode
{
	CString e_illCls;			// イラスト種別コード
	CString e_dstCode;			// 仕向け先コード

	ClassCode(CString illCls, CString dstCode)
		:e_illCls(illCls), e_dstCode(dstCode)
	{}

	bool operator < (const ClassCode& classCode) const {
		if (e_illCls != classCode.e_illCls) 
			return e_illCls < classCode.e_illCls;
		else
			return e_dstCode < classCode.e_dstCode;
	}
};

typedef long OID;

struct IllFileNames
{
	CString e_orgIllName;
	CString e_tgtIllName;
};

// イラストリンク情報取得用構造体
struct IllLinkInfo
{
	OID e_objectId;
	long e_illustClass;
	std::vector<OID> e_tgtNodeIds;
	

	IllLinkInfo(OID objectId, long illustClass, std::vector<OID> tgtNodeIds)
		:e_objectId(objectId), e_illustClass(illustClass), e_tgtNodeIds(tgtNodeIds)
	{}
};

// 道路リンク情報取得用
const long ForwardDir = sindy::schema::category::directional_link_relational_table::link_dir::kFore;
const long ReverseDir = sindy::schema::category::directional_link_relational_table::link_dir::kReverse;

struct RoadLinkKey
{
	long e_fromNodeId;
	long e_toNodeId;

	RoadLinkKey(long fromNodeId, long toNodeId)
		:e_fromNodeId(fromNodeId), e_toNodeId(toNodeId)
	{}
	bool operator < (const RoadLinkKey& roadLinkKey) const {
		if (e_fromNodeId != roadLinkKey.e_fromNodeId)
			return e_fromNodeId < roadLinkKey.e_fromNodeId;
		else
			return e_toNodeId < roadLinkKey.e_toNodeId;
	}
};
struct RoadLinkInfo
{
	OID	 e_objectId;
	long e_fromNodeId;
	long e_toNodeId;
};

// リンク列情報格納構造体
struct LinkQueueInfo 
{
	long e_linkid;
	long e_linkDir;
};



// イラスト種別(inf_illust)用

struct InfIllInfo
{
	OID	 e_objectID;
	long e_illustCode;
	CString e_fileName;

	InfIllInfo(OID objectId, long illustCode, CString fileName)
		:e_objectID(objectId), e_illustCode(illustCode), e_fileName(fileName)
	{}
};
// イラストリンク列(lq_illust)用
struct LqIllKey
{
	long e_infId;			// イラスト情報ID
	long e_Seq;				// シーケンス

	LqIllKey(long infId, long seq) 
		:e_infId(infId), e_Seq(seq)
	{}

	bool operator < (const LqIllKey lqIllKey) const {
		if (e_infId != lqIllKey.e_infId)
			return e_infId < lqIllKey.e_infId;
		else
			return e_Seq < lqIllKey.e_Seq;
	}
};
struct LqIllInfo
{
	long e_infId;
	long e_linkId;			// リンクID
	long e_linkDir;			// リンク方向
	long e_seq;				// シーケンス

	LqIllInfo(long infId, long linkId, long linkDir, long seq)
		:e_infId(infId), e_linkId(linkId), e_linkDir(linkDir), e_seq(seq)
	{}
};
