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

/**
 *	@file CreateLinkPatch.cpp
 *	@brief アプリケーションクラス 実装部
 *	@author	F.Adachi
 *	@date	2005/10/18		新規作成
 *	$ID$
 */

#include "StdAfx.h"
#include "CreateLinkPatch.h"
#include "LogFile.h"
#include "GlobalFunction.h"
#include "SINDY2INCODE.h"
#include <sstream>
#include <set>
#include <cassert>
#include <iomanip>


const CString DBHandleBase::m_ProgramName = "CreateLinkPatch";

CreateLinkPatch::CreateLinkPatch(void)
{
}

CreateLinkPatch::~CreateLinkPatch()
{
}


/**
 *	@brief	初期化
 *	@param	cArgc	[in]	コマンドライン引数の数
 *	@param	cArgv	[in]	コマンドライン引数の列
 **/
void CreateLinkPatch::init( int cArgc, char** cArgv )
{
	std::string aFunc = "CreateLinkPatch::init()";

	// 所有インスタンスの生成
	m_pCmdLineArg = std::auto_ptr<CmdLineArg>(new CmdLineArg);
	m_pAppropriateAttribute = std::auto_ptr<AppropriateAttribute>(new AppropriateAttribute);
	m_pConfirmedLink = std::auto_ptr<ConfirmedLink>(new ConfirmedLink);
	m_pRoadLinkCollection = std::auto_ptr<RoadLinkCollection>(new RoadLinkCollection);
	m_pHighwayNodeCollection = std::auto_ptr<HighwayNodeCollection>(new HighwayNodeCollection);
	m_pLinkPatch = std::auto_ptr<LinkPatch>(new LinkPatch);

	std::auto_ptr<DBInfo> pDBInfo(new DBInfo);
	m_pCmdLineArg->setDBInfo( pDBInfo.get() );
	m_pCmdLineArg->analyze( cArgc, cArgv );
	this->setDBInfo( std::auto_ptr<const DBInfo>(pDBInfo) );

	// 出力ファイルが書き込み可能かチェック ///////
	std::ofstream aFout( m_pCmdLineArg->getLinkPatchFilePath() );
	if (!aFout) { throw std::runtime_error( "R1F:" + aFunc + " : リンクパッチファイルオープンエラー" ); }
	aFout.close();
}


/**
 *	@brief	順次処理
 **/
void CreateLinkPatch::process(void)
{
	read();
	checkMainLink();
	checkSubLink();
	write();
}


/**
 *	@brief	読み込み
 **/
void CreateLinkPatch::read(void)
{
	// Road_Linkの読み込み
	readRoadLink();

	// Highway_Nodeの読み込み
	m_pHighwayNodeCollection->read( getWorkspace() );
}


/**
 *	@brief	Road_Linkデータの読み込み
 **/
void CreateLinkPatch::readRoadLink(void)
{
	// 路線コード指定ファイルの読み込み
	m_pAppropriateAttribute->read( m_pCmdLineArg->getApprAttrFilePath() );
	const std::map<long, long>* pTable = m_pAppropriateAttribute->getTable();

	std::string aSubFieldStr( "OBJECTID, ROAD_CODE, ROADCLASS_C, NAVICLASS_C, UPDOWNCLASS_C, MAIN_LINKCLASS_C, PROGMODIFYDATE, MODIFYPROGNAME, FROM_NODE_ID, TO_NODE_ID, SHAPE" );
	

	// 指定された路線コードを持つリンクの読み込み
	for (std::map<long, long>::const_iterator aIter = pTable->begin(); aIter != pTable->end(); ++aIter ) {
		long aRoadCode = aIter->first;

		std::ostringstream aSout;
		aSout << "ROAD_CODE = " << aRoadCode;
		std::string aWhereStr = aSout.str();

		m_pRoadLinkCollection->read( getWorkspace(), aWhereStr, aSubFieldStr );
	}

	// 本線以外のリンクの読み込み
	{
		// ランプリンクを含みそうな条件でフィルタリング
		std::string aWhereStr = "(ROAD_CODE = 0 AND UPDOWNCLASS_C <> 0 AND MAIN_LINKCLASS_C IN( 3, 5, 6, 7, 9)) OR UPDOWNCLASS_C IN (5, 6)";
		m_pRoadLinkCollection->read( getWorkspace(), aWhereStr, aSubFieldStr );
	}
}


/**
 *	@brief	本線リンクのチェック
 **/
void CreateLinkPatch::checkMainLink(void) const
{
	LogFile* pLog = LogFile::Instance();

	const std::map<long, long>* pTable = m_pAppropriateAttribute->getTable();

	// Road_Link集合の中から指定された路線コードを持つリンクのみチェック
	for (std::map<long, long>::const_iterator aTableIter = pTable->begin(); aTableIter != pTable->end(); ++aTableIter) {
		long aRoadCode = aTableIter->first;
		long aRoadClass = aTableIter->second;

		const std::map<long, RoadLink>* pRoadLinkCollection = m_pRoadLinkCollection->getRoadLinkCollection();
		// 本線のみのチェック
		for (RoadLinkCollection::RoadLinkIter aRoadLinkIter = pRoadLinkCollection->begin(); aRoadLinkIter != pRoadLinkCollection->end(); ++aRoadLinkIter ) {
			if (aRoadLinkIter->second.m_RoadCode != aRoadCode) { continue; }
			
			// チェック済みリンク集合に追加
			m_pConfirmedLink->insert( aRoadLinkIter->first );
			// 道路経路種別のチェック（不要？）
///////		if (aRoadLinkIter->m_NaviClass == 0) { continue; }

			// 道路標示種別のチェック
			if (aRoadLinkIter->second.m_RoadClass != aRoadClass) {
				// 道路標示種別が指定されたものと異なればFLAG==1でログへ出力
				dispRoadLink( (std::ostream&)*pLog, aRoadLinkIter->first, aRoadCode, true, "MAIN" );
			}
			else {
				dispRoadLink( (std::ostream&)*pLog, aRoadLinkIter->first, aRoadCode, false, "MAIN" );
			}
		}
	}
}


/**
 *	@brief	本線以外のリンクのチェック
 **/
void CreateLinkPatch::checkSubLink(void) const
{
	LogFile* pLog = LogFile::Instance();

	const std::map<long, long>* pTable = m_pAppropriateAttribute->getTable();

	// Road_Link集合の中から指定された路線コードを持つリンクのみチェック
	for (std::map<long, long>::const_iterator aTableIter = pTable->begin(); aTableIter != pTable->end(); ++aTableIter) {
		long aRoadCode = aTableIter->first;
		long aRoadClass = aTableIter->second;

		const std::map<long, RoadLink>* pRoadLinkCollection = m_pRoadLinkCollection->getRoadLinkCollection();
		for (RoadLinkCollection::RoadLinkIter aRoadLinkIter = pRoadLinkCollection->begin(); aRoadLinkIter != pRoadLinkCollection->end(); ++aRoadLinkIter ) {
			if (aRoadLinkIter->second.m_RoadCode != aRoadCode) { continue; }
									
			// サブリンクの探索開始
			searchSubLink( aRoadLinkIter->first, aRoadCode );
		}
	}
}


/**
 * @brief	サブリンク探索
 * @note	与えられた高速道路本線のRoadLinkオブジェクトに接続されている本線以外のリンク（ランプリンク等）を探し、追跡を開始する
 * @param	cObjectID	[in]	対象RoadLinkオブジェクトのオブジェクトID
 * @param	cRoadCode	[in]	探索開始位置の路線コード
 **/
void CreateLinkPatch::searchSubLink( long cObjectID, long cRoadCode ) const
{
	RoadLink aRoadLink = m_pRoadLinkCollection->getRoadLink( cObjectID );

	IPointPtr ipFromNodePoint, ipToNodePoint;
	getNodePair( cObjectID, &ipFromNodePoint, &ipToNodePoint );

	int aCount = 0;

	// Road_Link集合の中から同じIPointを持つリンクを探す
	const std::map<long, RoadLink>* pRoadLinkCol = m_pRoadLinkCollection->getRoadLinkCollection();
	for (RoadLinkCollection::RoadLinkIter aIter = pRoadLinkCol->begin(); aIter != pRoadLinkCol->end(); ++aIter ) {

		if ( isConnected( ipFromNodePoint, aIter->first ) ) {
			trackSubLink( aIter->first, ipFromNodePoint, cRoadCode );
		}
		if ( isConnected( ipToNodePoint, aIter->first ) ) {
			trackSubLink( aIter->first, ipToNodePoint, cRoadCode );
		}
		aCount++;
	}
}


/**
 *	@brief	From/Toノードの取得
 *	@note	与えられたRoadLinkオブジェクトからFrom/ToノードのIPointを取得する
 *	@note	（RoadLinkに実装した方がいいかも…）
 *	@param	cObjectID	[in]	対象RoadLinkオブジェクトのオブジェクトID
 *	@param	cpFromNodePoint	[out]	Fromノード用バッファ
 *	@param	cpToNodePoint	[out]	Toノード用バッファ
 **/
void CreateLinkPatch::getNodePair( long cObjectID, IPoint** cpFromNodePoint, IPoint** cpToNodePoint ) const
{
	std::string aFunc = "CreateLinkPatch::getNodePair()";

	RoadLink aRoadLink = m_pRoadLinkCollection->getRoadLink( cObjectID );

	IPointCollectionPtr ipPointCol( aRoadLink.m_pShape );	// dynamic_castと同じ。失敗した場合の例外をcatchすべき
	long aPointCount = 0;
	if (ipPointCol->get_PointCount( &aPointCount ) != S_OK ) { throw std::runtime_error( "R1F:" + aFunc + " : get_PointCount()失敗"); }

	if (ipPointCol->get_Point( 0, cpFromNodePoint ) != S_OK ) { throw std::runtime_error( "R1F:" + aFunc + " : get_Point()失敗"); }
	if (ipPointCol->get_Point( aPointCount-1, cpToNodePoint ) != S_OK ) { throw std::runtime_error( "R1F:" + aFunc + " : get_Point()失敗"); }
}


/**
 *	@brief	IPointとRoadLinkの接触判定
 *	@note	与えられたIPointが、与えられたオブジェクトIDを持つRoadLinkのFrom/Toノードになっているか調べる
 *	@param	cpPoint	[in]	対象IPointへのポインタ
 *	@param	cObjectID	[in]	対象RoadLinkオブジェクトのオブジェクトID
 *	@return	接触しているか否か
 **/
bool CreateLinkPatch::isConnected( IPoint* cpPoint, long cObjectID ) const
{
	std::string aFunc = "CreateLinkPatch::isConnected()";

	RoadLink aRoadLink = m_pRoadLinkCollection->getRoadLink( cObjectID );

	if (cpPoint == NULL) { throw std::runtime_error( "R1F:" + aFunc + " : NULL" ); }

	IPointPtr ipFromPoint, ipToPoint;
	getNodePair( cObjectID, &ipFromPoint, &ipToPoint );
	
	int aIsSamePoint;
	cpPoint->Compare( ipFromPoint, &aIsSamePoint );
	if ( !aIsSamePoint ) { return true;	}
	else {
		cpPoint->Compare( ipToPoint, &aIsSamePoint );
		if ( !aIsSamePoint ) { return true;	}
	}

	return false;
}


/**
 * @brief	サブリンク追跡
 * @note	本線（上下分離）以外のリンクを辿り、終了条件を満たすまで再帰的にチェックを行なう
 * @param	cObjectID	[in]	対象RoadLinkオブジェクトのオブジェクトID
 * @param	cpPrevPoint	[in]	１つ前のリンクとの共有ノードの座標
 * @param	cRoadCode	[in]	探索開始位置の路線コード
 */
void CreateLinkPatch::trackSubLink( long cObjectID, IPointPtr cpPrevPoint, long cRoadCode ) const
{
	std::string aFunc = "CreateLinkPatch::trackSubLink()";

	RoadLink aRoadLink = m_pRoadLinkCollection->getRoadLink( cObjectID );

	LogFile* pLog = LogFile::Instance();
	static long aCount = 0;

	// 着目リンクがチェック済みなら終了
	if ( m_pConfirmedLink->exist( cObjectID ) ) { return; }
	++aCount;
#ifdef	_DEBUG
	std::cout << "追跡済みリンク数 : " << aCount << "\r";
#endif

	{
		// チェック済みリンク集合に追加
		m_pConfirmedLink->insert( cObjectID );
		// 道路経路種別のチェック（不要？）
//		if (aRoadLink.m_NaviClass == 0) { }

		// 道路標示種別のチェック
		long aRoadClass = m_pAppropriateAttribute->getRoadClass( cRoadCode );
		if (aRoadLink.m_RoadClass != aRoadClass) {
			dispRoadLink( (std::ostream&)*pLog, cObjectID, cRoadCode, true, "SUB" );
		}
		else {
			dispRoadLink( (std::ostream&)*pLog, cObjectID, cRoadCode, false, "SUB" );
		}
	}

	// From/ToノードのIPoint取得
	IPointPtr ipFromNodePoint, ipToNodePoint;
	getNodePair( cObjectID, &ipFromNodePoint, &ipToNodePoint );

	// 進行方向を調べる
	int aResult;
	IPointPtr ipNextPoint;
	ipFromNodePoint->Compare( cpPrevPoint, &aResult );
	if (!aResult) { ipNextPoint = ipToNodePoint; }
	else {
		ipToNodePoint->Compare( cpPrevPoint, &aResult );
		if (!aResult) { ipNextPoint = ipFromNodePoint; }
		else { assert(0); }		// どっちにもヒットしない場合は異常
	}
	
	// 進行方向のHighwayNodeをチェック
	std::set<const HighwayNode*> aMultiplyHighwayNode;	// 051216 同一座標に複数のハイウェイノードがある場合を考慮して変更
	m_pHighwayNodeCollection->getHighwayNode( ipNextPoint, &aMultiplyHighwayNode );
	for (std::set<const HighwayNode*>::const_iterator aIter = aMultiplyHighwayNode.begin(); aIter != aMultiplyHighwayNode.end(); ++aIter ) {
		const HighwayNode* pHighwayNode = *aIter;
		if (pHighwayNode != NULL) { 
			if (pHighwayNode->m_TollClass == 0 || pHighwayNode->m_TollClass == 4) { // 0;分岐合流ノード 4:出入口ノード
				return;
			}
		}
	}

	// 隣接リンクがあれば再帰的に追跡
	const std::map<long, RoadLink>* pRoadLinkCol = m_pRoadLinkCollection->getRoadLinkCollection();
	for (RoadLinkCollection::RoadLinkIter aIter = pRoadLinkCol->begin(); aIter != pRoadLinkCol->end(); ++aIter ) {
		if ( isConnected( ipNextPoint, aIter->first ) ) {
			trackSubLink( aIter->first, ipNextPoint, cRoadCode );
		}
	}
}


/**
 * @brief	ログの見出しを出力
 * @note	１回目の呼び出し時のみログの見出しを表示する
 * @param	crStream	[in]	出力ストリーム
 */
void CreateLinkPatch::dispLogHeader( std::ostream& crStream ) const
{
	static unsigned aRowCount = 0;
	if (aRowCount == 0) {
		crStream << "# FREESTYLELOG" << std::endl;
		crStream << "FLAG	LAYER	OBJECTID	ROAD_CODE	ROADCLASS_C	NAVICLASS_C	UPDOWNCLASS_C	MAIN_LINKCLASS_C	FROM_NODE_ID	TO_NODE_ID	COMMENT	LINK_PATCH" << std::endl;
	}
	++aRowCount;
}


/**
 *	@brief	指定したRoadLinkオブジェクトの値を出力
 *	@param	crStream	[out]	出力ストリームへのリファレンス
 *	@param	cObjectID	[in]	対象RoadLinkオブジェクトのオブジェクトID
 *	@param	cRoadCode	[in]	探索開始位置の路線コード
 *	@param	cFlag	[in]	ログの第１フィールド（調査済みフラグ）を'1'にするか否か
 *	@param	cpComment	[in]	ログのCOMMENT列に出力する文字列
 **/
void CreateLinkPatch::dispRoadLink( std::ostream& crStream, long cObjectID, long cRoadCode, bool cFlag, const char* cpComment ) const
{
	RoadLink aRoadLink = m_pRoadLinkCollection->getRoadLink( cObjectID );
	int aMeshCode = m_pRoadLinkCollection->getMeshCode( cObjectID );

	long aRoadClass = m_pAppropriateAttribute->getRoadClass( cRoadCode );
	std::string aINRoadClass = getINRoadClass( aRoadClass );

	std::stringstream aSout;
	aSout << "0x" << std::setw(8) << std::setfill('0') << std::right << aINRoadClass;

	dispLogHeader( crStream );
	crStream << ( cFlag ? 1 : 0 ) << "\t";
	crStream << "ROAD_LINK" << "\t";
	crStream << cObjectID << "\t";
	crStream << aRoadLink.m_RoadCode << "\t";
	crStream << aRoadLink.m_RoadClass << "\t";
	crStream << aRoadLink.m_NaviClass << "\t";
	crStream << aRoadLink.m_UpDownClass << "\t";
	crStream << aRoadLink.m_MainLinkClass << "\t";
	crStream << aRoadLink.m_FromNodeID << "\t";
	crStream << aRoadLink.m_ToNodeID << "\t";
	crStream << cpComment << "\t";
	crStream << "( \"DISP\", ";
	crStream << aSout.str();
	crStream << ", " << aMeshCode << ", " << aRoadLink.m_FromNodeID << ", " << aRoadLink.m_ToNodeID << " ),";	// 0x05001100
	crStream << std::endl;

	// LinkPatchにレコードを格納
	if ( cFlag ) {
		std::string aLinkPatchFilePath = m_pCmdLineArg->getLinkPatchFilePath();
		if ( aLinkPatchFilePath.size() ) {
			LinkPatchRecord aRecord;
			aRecord.m_Attribute = "\"DISP\"";
			aRecord.m_RevisedValue = aSout.str();
			aRecord.m_MeshCode = aMeshCode;
			aRecord.m_NodeIDCollection.push_back( aRoadLink.m_FromNodeID );
			aRecord.m_NodeIDCollection.push_back( aRoadLink.m_ToNodeID );
			m_pLinkPatch->assign( aRecord );
		}
	}

}


/**
 *	@brief	書き込み
 **/
void CreateLinkPatch::write(void) const
{
	m_pLinkPatch->write( m_pCmdLineArg->getLinkPatchFilePath(), m_pCmdLineArg->useEUC() );
}


/**
 *	@brief	与えられたSiNDYの道路標示種別に対応するI/Nの道路標示種別を返す
 *	@param	cSindyRoadClass	SiNDYの道路標示種別
 *	@return	I/Nの道路標示種別
 **/
std::string	CreateLinkPatch::getINRoadClass( long cSindyRoadClass ) const
{
	std::string aFunc = "CreateLinkPatch::getINRoadClass()";

	long aINCode;
	if (SINDY2INCODE::Convert( SINDY2INCODE::road_class, cSindyRoadClass, &aINCode ) != S_OK) {
		throw std::runtime_error( "R1F:" + aFunc + " : SINDY2INCODE::Convert()失敗" );
	}
	std::stringstream aSout;
	aSout << std::hex << aINCode;

	return aSout.str();
}
