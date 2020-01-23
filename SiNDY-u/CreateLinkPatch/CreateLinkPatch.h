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
 *	@file CreateLinkPatch.h
 *	@brief アプリケーションクラス 定義部
 *	@author	F.Adachi
 *	@date	2005/08/02		新規作成
 *	$ID$
 */

#ifndef ___CREATELINKPATCH___
#define ___CREATELINKPATCH___

#include "DBHandleBase.h"
#include "CmdLineArg.h"
#include "AppropriateAttribute.h"
#include "ConfirmedLink.h"
#include "RoadLinkCollection.h"
#include "HighwayNodeCollection.h"
#include "LinkPatch.h"
#include "EtcLinkList.h"


/// <b>アプリケーションクラス</b>
class CreateLinkPatch : public DBHandleBase
{
public:
	CreateLinkPatch(void);
	~CreateLinkPatch();

	void	init( int cArgc, char** cArgv );	///< 初期化
	void	process(void);						///< 変換処理

protected:
	void	read(void);
	void	readRoadLink(void);
	void	write(void) const;

	void	checkMainLink(void) const;
	void	checkSubLink(void) const;
	void	searchSubLink( long cObjectID, long cRoadCode ) const;
	void	trackSubLink( long cObjectID, IPointPtr cpPrevPoint, long cRoadCode ) const;
	void	getNodePair( long cObjectID, IPoint** cpFromNodePoint, IPoint** cpToNodePoint ) const;
	bool	isConnected( IPoint* cpPoint, long cObjectID ) const;
	void	dispLogHeader( std::ostream& crStream ) const;
	void	dispRoadLink( std::ostream& crStream, long cObjectID, long cRoadCode, bool cFlag, const char* cpComment ) const;
	std::string	getINRoadClass( long cSindyRoadClass ) const;

private:
	std::auto_ptr<CmdLineArg>				m_pCmdLineArg;				//< コマンドライン引数クラスへの関連
	std::auto_ptr<AppropriateAttribute>		m_pAppropriateAttribute;	//< 路線コード指定ファイルクラスへの関連
	std::auto_ptr<ConfirmedLink>			m_pConfirmedLink;			//< 確認済みリンク集合クラスへの関連
	std::auto_ptr<RoadLinkCollection>		m_pRoadLinkCollection;		//< 道路リンク集合クラスへの関連
	std::auto_ptr<HighwayNodeCollection>	m_pHighwayNodeCollection;	//< ハイウェイノード集合クラスへの関連
	std::auto_ptr<LinkPatch>				m_pLinkPatch;				//< リンクパッチクラスへの関連	
};

#endif
