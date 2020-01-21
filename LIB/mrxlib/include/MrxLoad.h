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

// MrxLoad.h: CMrxLoad クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MRXLOAD_H__F1D0113A_62FF_44D3_865C_8A9DBDF0715B__INCLUDED_)
#define AFX_MRXLOAD_H__F1D0113A_62FF_44D3_865C_8A9DBDF0715B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "StdAfx.h"
#include <io.h>
#include <fcntl.h>	//typedef O_RDONLY
#include <vector>	//typedef STL

typedef unsigned long	U_LONG;
typedef unsigned short	U_SHORT;

#pragma pack(push, 2)
/**
 * @brief マップルＭＲＸヘッダデータテーブル（２５６byte）
 */
struct MRXHead{
	char	eVersion[8];			///< ファイルフォーマットバージョン
	short	eSokuchi;				///< 側地系コード
	short	eReserve1;				///< (予備)
	long	eScale;					///< 基準データ縮尺
	char	eMeshcode[16];			///< メッシュ番号
	long	eMinX;					///< 図郭情報－南西端経度
	long	eMaxX;					///< 図郭情報－北東端経度
	long	eMinY;					///< 図郭情報－南西端緯度
	long	eMaxY;					///< 図郭情報－北東端緯度
	char	eLastUpdate[16];		///< 最終更新日
	char	eReleaseID[16];			///< リリース識別情報
	char	eReserve2[48];			///< (予備)
	U_LONG	eNodeOffset;			///< ノードレコード開始位置
	U_LONG	eNodeNs;				///< ノードレコード数
	U_LONG	eLinkOffset;			///< リンクレコード開始位置
	U_LONG	eLinkNs;				///< リンクレコード数
	U_LONG	eGroupLineOffset;		///< グループ線開始位置
	U_LONG	eGroupLineNs;			///< グループ線数
	char	eReserve3[104];			///< (予備)
};

/**
 * @brief マップルＭＤＸヘッダデータテーブル（２５６byte）
 */
struct MDXHead{
	char	eVersion[8];			///< ファイルフォーマットバージョン
	short	eSokuchi;				///< 側地系コード
	short	eReserve1;				///< (予備)
	long	eScale;					///< 基準データ縮尺
	char	eMeshcode[16];			///< メッシュ番号
	long	eMinX;					///< 図郭情報－南西端経度
	long	eMaxX;					///< 図郭情報－北東端経度
	long	eMinY;					///< 図郭情報－南西端緯度
	long	eMaxY;					///< 図郭情報－北東端緯度
	char	eLastUpdate[16];		///< 最終更新日
	char	eReleaseID[16];			///< リリース識別情報
	char	eReserve2[48];			///< (予備)
	U_LONG	eLineOffset;			///< 線レコード開始位置
	U_LONG	eLineNs;				///< 線レコード数
	U_LONG	ePolyOffset;			///< ポリゴンレコード開始位置
	U_LONG	ePolyNs;				///< ポリゴンレコード数
	char	eReserve3[16];			///< (予備)
	U_LONG	eGroupLineOffset;		///< グループ線開始位置
	U_LONG	eGroupLineNs;			///< グループ線数
	char	eReserve4[88];			///< (予備)
};

/**
 * @brief マップルＭＲＸノードデータテーブル
 */
struct MRX_Node{
	short	eObjectCode;			///< オブジェクトコード（11 道路ノード）
	U_LONG	eNodeNo;				///< ノード番号
	char	eNodeType[8];			///< ノード種別コード
	char	eNodeAttr[8];			///< 属性分類コード
	char	eLavel[16];				///< エレメントラベル
	long	eLon;					///< 経度[1/1000秒]
	long	eLat;					///< 緯度[1/1000秒]
	long	eHeight;				///< 標高値[1/10m]
	char	eNextMesh[8];			///< 隣接メッシュ番号（図郭上ノードの場合のみ）
	U_LONG	eNextMeshNodeNo;		///< 隣接メッシュ接続ノード番号
	short	eLinkCount;				///< 接続リンク本数
};

/**
 * @brief マップルＭＲＸノードデータコンテナ
 */
struct MRXNode{
	MRX_Node		eNodes;			///< ノード要素部
	std::vector<U_LONG>	vLinkID;		///< ノード接続リンク番号コンテナ
};

/**
 * @brief マップルＭＲＸリンク形状点座標管理テーブル
 */
struct	MRX_Pos{
	long			ePos[3];		///< 形状点座標
};

/**
 * @brief マップルＭＲＸリンクデータテーブル
 */
struct MRX_Link{
	short	eObjectCode;			///< オブジェクトコード（21 道路リンク）
	U_LONG	eLinkNo;				///< リンク番号
	char	eLinkType[8];			///< リンク種別コード
	char	eLinkAttr[8];			///< 属性分類コード
	char	eLavel[16];				///< エレメントラベル
	long	eWidth;					///< 幅員値
	U_LONG	eSnodeID;				///< 始点ノード番号
	U_LONG	eEnodeID;				///< 終点ノード番号
	short	eRegCode;				///< 交通規種別コード
	short	eLength;				///< リンク長
	long	eCount;					///< 形状点総数	
};

/**
 * @brief マップルＭＲＸリンク構成点コンテナ
 */
struct MRXLink{
	long			eType;					///< リンク列種別コード(1:ぬけみち、2:うらみち、3:渋滞道路)
	U_LONG			eID;					///< 構成線の識別番号
	int				eMeshcode;				///< メッシュコード
	long			ePrimId;				///< リンクOBJECTID
	short			eLinkDir;				///< リンク構成点の向き
	long			eSeq;					///< 構成要素番号
	long			eCount;					///< 構成要素数
	long			eLastF;					///< リンク列終端フラグ
	MRX_Link		eLinks;					///< リンク要素部
	std::vector<MRX_Pos>	vLinkPos;		///< リンク形状点座標コンテナ
};

/**
 * @brief マップルＭＲＸグループデータテーブル（渋滞・ぬけみち）
 */
struct MRX_GroupId{
	U_LONG	eID;					///< 構成線の識別番号
	short	eDir;					///< 構成線の向き（1:始点→終点、2:終点→始点）
};

/**
 * @brief マップルＭＲＸグループデータテーブル（渋滞・ぬけみち）
 */
struct MRX_Group{
	short	eObjectCode;			///< オブジェクトコード（91 道路グループ）
	U_LONG	eGroupID;				///< 識別番号
	char	eKindCode[8];			///< 図式分類コード
	char	eAttrCode[8];			///< 属性分類コード
	char	eLavel[16];				///< エレメントラベル
	short	eDrawSeq;				///< 描画順
	short	eStatCode;				///< 状態種別コード
	long	eHVal;					///< Ｈ値
	long	eCount;					///< 構成要素数
};

/**
 * @brief マップルＭＲＸグループデータコンテナ
 */
struct MRXGroup{
	MRX_Group			eGroup;		///< グループ要素部
	std::vector<MRX_GroupId>	vGroupID;	///< グループ要素構成リンク列識別コンテナ
};

/**
 * @brief マップルＭＲＸリンク列管理コンテナ
 */
struct MRXLQLink{
	long			eType;			///< リンク列種別コード(1:ぬけみち、2:うらみち、3:渋滞道路、4:道路リンク)
	int				eMeshcode;		///< メッシュコード
	U_LONG			eGroupID;		///< 識別番号
	long			ePrimId;		///< リンク列OBJECTID
	long			eCount;			///< 構成要素数
	std::vector<MRX_Pos>	vLinkPos;		///< リンク形状点座標コンテナ
};

/**
 * @brief マップルＭＤＸ駐禁取締ポリゴンデータテーブル
 */
struct MDX_Polygon
{
	short	eObjectCode;			///< オブジェクトコード（50 その他ポリゴン）
	U_LONG	eGroupID;				///< 識別番号
	char	eKindCode[8];			///< 図式分類コード
	char	eLinkAttr[8];			///< 属性分類コード
	char	eLavel[16];				///< エレメントラベル
	short	eDrawSeq;				///< 描画順
	short	eStatCode;				///< 状態種別コード
	long	eHVal;					///< Ｈ値(未使用)
	long	eCount;					///< 構成要素数
};

/**
 * @brief マップルＭＤＸ駐禁取締ポリゴンデータコンテナ
 */
struct MDXPolygon
{
	MDX_Polygon	eMdxPoly;				///< ポリゴン要素部
	std::vector<MRX_Pos> vPolygonPos;	///< ポリゴン形状点座標コンテナ
};

/**
 * @brief マップルＭＤＸリンク構成点テーブル
 */
struct MDX_Link{
	short	eObjectCode;			///< オブジェクトコード（40:その他の線、41:道路、42:鉄道、43:行政界、44:等高線）
	U_LONG	eGroupID;				///< 識別番号
	char	eKindCode[8];			///< 図式分類コード
	char	eLinkAttr[8];			///< 属性分類コード
	char	eLavel[16];				///< エレメントラベル
	short	eDrawSeq;				///< 描画順
	short	eStatCode;				///< 状態種別コード
	long	eHVal;					///< Ｈ値（[41:道路]道路幅員、[44:等高線]標高値）
	long	eCount;					///< 構成要素数
};

/**
 * @brief マップルＭＤＸリンク構成点コンテナ
 */
struct MDXLink
{
	MDX_Link	eMdxLink;
	std::vector<MRX_Pos> vLinkPos;
};
#pragma pack(pop)

/**
 * @brief マップルＭＲＸデータコンテナ
 */
class MRX_Alldata
{
public:
// 構築/消滅
	MRX_Alldata(){}
	~MRX_Alldata(){}

	inline MRXLink *FindLink(U_LONG aKeyID /*リンク番号*/){
		for(std::vector<MRXLink>::iterator it=m_Links.begin(); it!=m_Links.end(); it++ ){
			if( aKeyID==(*it).eLinks.eLinkNo ){
				return &(*it);
			}
		}
		return NULL;
	}

public:
	MRXHead					m_Header;	///< ヘッダー部
	std::vector<MRXNode>	m_Nodes;	///< ノード要素部コンテナ
	std::vector<MRXLink>	m_Links;	///< リンク要素部コンテナ
	std::vector<MRXGroup>	m_Group;	///< グループ要素部コンテナ
	std::vector<MRXLQLink>	m_LQ;		///< リンク列管理コンテナ
	std::vector<MDXPolygon>	m_Poly;		///< ポリゴン要素部コンテナ
};

/**
 * @brief マップルＭＤＸデータコンテナ
 */
class MDX_Alldata
{
public:
// 構築/消滅
	MDX_Alldata(){}
	~MDX_Alldata(){}
public:
	MDXHead					m_Header;		///< ヘッダー部
	std::vector<MDXPolygon>	m_Poly;			///< ポリゴン要素部コンテナ
	std::vector<MDXLink>	m_Links;		///< リンク要素部
	std::vector<MRXGroup>   m_Group;		///< ポリライン要素部コンテナ
};


/**
* @class CMrxLoad
*
* @brief マップルＭＲＸデータを包括管理するクラス
*/
class CMrxLoad
{
public:
// 構築/消滅
	CMrxLoad();
	~CMrxLoad();

// 取得
	char*	getExtension(void) { return "mrx"; }

// 登録
	bool	load(char *cFileStr);	///< マップルＭＲＸデータ読み込み	
	void	close(void);			///< マップルＭＲＸデータ開放
	const MRX_Alldata&	getMRXdata(void) const { return(m_MrxInfo);	 }///< MRXデータクラスを取得
	MRX_Alldata	*getMRXinfo(char *cFileStr){ 
		if( load(cFileStr)){ return(&m_MrxInfo); }	///< MRXデータの取得成功
		else{                return(NULL);       }	///< MRXデータの取得失敗
	}

//ディバグ情報を出力
	void	print();

// 取得
protected:
	bool	loadNodes(std::ifstream& ifp);	///< ノード要素部の取得	
	bool	loadLinks(std::ifstream& ifp);	///< リンク要素部の取得	
	bool	loadGroups(std::ifstream& ifp);	///< グループ情報要素部の取得
	bool	bindGroups(void);		///< グループ情報毎に道路を結合

private:
// プロパティ
	MRX_Alldata		m_MrxInfo;		///< マップルＭＲＸデータ管理データ
};

/**
* @class CMdxLoad
*
* @brief マップルＭＤＸデータを包括管理するクラス
*/
class CMdxLoad
{
public:
// 構築/消滅
	CMdxLoad();
	~CMdxLoad();

// 取得
	char*	getExtension(void) { return "mdx"; }
// 登録
	bool	load(char *cFileStr);	///< マップルＭＤＸデータ読み込み	
	void	close(void);			///< マップルＭＤＸデータ開放
	const MDX_Alldata& getMDXdata(void) const { return(m_MdxInfo);	 }///< MDXデータクラスを取得
	MDX_Alldata	*getMDXinfo(char *cFileStr){ 
		if( load(cFileStr)){ return(&m_MdxInfo); }	///< MDXデータの取得成功
		else{                return(NULL);       }	///< MDXデータの取得失敗
	}
// 取得
protected:
	bool	loadPoly(std::ifstream& ifp);	///< ポリゴン要素部の取得
	bool	loadGroups(std::ifstream& ifp);	///< グループ線要素部の取得
	bool	loadLinks(std::ifstream& ifp);	///< 線要素部の取得

private:
// プロパティ
	MDX_Alldata		m_MdxInfo;		///< マップルＭＤＸデータ管理データ
};

#endif // !defined(AFX_MRXLOAD_H__F1D0113A_62FF_44D3_865C_8A9DBDF0715B__INCLUDED_)
