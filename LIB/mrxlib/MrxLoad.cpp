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

// MrxLoad.cpp: CMrxLoad クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MrxLoad.h"


/**
 * マップルＭＲＸデータクラスの構築
 * @note マップルＭＲＸデータクラスを構築します。
 * @param	void
 * @retval	void
 */
CMrxLoad::CMrxLoad(){}
CMdxLoad::CMdxLoad(){}
/**
 * マップルＭＲＸデータクラスの消滅
 * @note マップルＭＲＸデータクラスを消滅します。
 * @param	void
 * @retval	void
 */
CMrxLoad::~CMrxLoad()
{
	close();
}
CMdxLoad::~CMdxLoad()
{
	CMdxLoad::close();
}
/**
 * マップルＭＲＸデータ開放
 * @note マップルＭＲＸデータ開放します
 * @param	void
 * @retval	void
 */
void CMrxLoad::close(void)
{
	for(vector<MRXNode>::iterator it1=m_MrxInfo.m_Nodes.begin(); it1!=m_MrxInfo.m_Nodes.end(); it1++ ){
		(*it1).vLinkID.clear();
	}
	m_MrxInfo.m_Nodes.clear();

	for(vector<MRXLink>::iterator it2=m_MrxInfo.m_Links.begin(); it2!=m_MrxInfo.m_Links.end(); it2++ ){
		(*it2).vLinkPos.clear();
	}
	m_MrxInfo.m_Links.clear();

	for(vector<MRXGroup>::iterator it3=m_MrxInfo.m_Group.begin(); it3!=m_MrxInfo.m_Group.end(); it3++ ){
		(*it3).vGroupID.clear();
	}
	m_MrxInfo.m_Group.clear();

	for(vector<MRXLQLink>::iterator it4=m_MrxInfo.m_LQ.begin(); it4!=m_MrxInfo.m_LQ.end(); it4++ ){
		(*it4).vLinkPos.clear();
	}
	m_MrxInfo.m_LQ.clear();
}

void CMdxLoad::close()
{
	for(vector<MDXPolygon>::iterator it=m_MdxInfo.m_Poly.begin(); it!=m_MdxInfo.m_Poly.end(); ++it ){
		(*it).vPolygonPos.clear();
	}
	m_MdxInfo.m_Poly.clear();
}
/**
 * マップルＭＲＸデータディバグ情報を出力
 * @note マップルＭＲＸデータディバグ情報を出力
 * @param	void
 * @retval	void
 */
void CMrxLoad::print(void)
{
	for(vector<MRXNode>::iterator it1=m_MrxInfo.m_Nodes.begin(); it1!=m_MrxInfo.m_Nodes.end(); it1++ ){
		for(vector<U_LONG>::iterator it=(*it1).vLinkID.begin(); it!=(*it1).vLinkID.begin(); it++ ){
			cout << "NODE "
				 << (*it) << " : "						///< 接続リンク番号
				 << (*it1).eNodes.eLon		<< "-"		///< 経度[1/1000秒]
				 << (*it1).eNodes.eLat		<< endl;	///< 緯度[1/1000秒]
		}
	}

	for(vector<MRXLink>::iterator it2=m_MrxInfo.m_Links.begin(); it2!=m_MrxInfo.m_Links.end(); it2++ ){
		for(vector<MRX_Pos>:: iterator it=(*it2).vLinkPos.begin(); it!=(*it2).vLinkPos.end(); it++ ){
			cout << "LINK "
				 << (*it2).eLinks.eSnodeID  << "-"		///< 始点ノード番号
				 << (*it2).eLinks.eEnodeID  << " "		///< 終点ノード番号
				 << (*it).ePos[0] << "-" << (*it).ePos[1] << endl;
		}
	}

	for(vector<MRXGroup>::iterator it3=m_MrxInfo.m_Group.begin(); it3!=m_MrxInfo.m_Group.end(); it3++ ){
		for( vector<MRX_GroupId>::iterator it=(*it3).vGroupID.begin(); it!=(*it3).vGroupID.end(); it++ ){
			cout << "GRP "
				 << (*it).eID << " : "					///< 構成線の識別番号
				 << (*it3).eGroup.eKindCode << "-"		///< 図式分類コード
				 << (*it3).eGroup.eAttrCode << endl;	///< 属性分類コード
		}
	}
}
/**
 * 指定昭文社ＭＲＸ（Mapple Routing eXchange）ノードデータの取得
 * @note	指定昭文社ＭＲＸノードデータを取得します
 * @param	cFileHDL	(I)	MRXデータファイルハンドル
 * @return	true：取得成功、false：取得失敗
 */
bool CMrxLoad::loadNodes(std::ifstream& ifp)
{
	bool	bDBGMode=false;
#ifdef	_DEBUG___
	bDBGMode=true;
#endif
	for( U_LONG i=0; i<m_MrxInfo.m_Header.eNodeNs; i++ ){
		MRXNode		aNode;
		ifp.read((char*)&aNode.eNodes,sizeof(MRX_Node));
		if(!ifp){
			cerr << "ERROR CMrxLoad::read() m_MrxInfo.m_Nodes (" << i << ")" << endl;
			continue;
		}
		//接続リンク番号を取得
		for( short j=0; j<aNode.eNodes.eLinkCount ; j++ ){
			unsigned long aLinkNo;
			ifp.read((char*)&aLinkNo,sizeof(unsigned long));
			if(!ifp){
				cerr << "ERROR CMrxLoad::read() m_MrxInfo.eNode (" << i << "/" << aNode.eNodes.eLinkCount << ")  " << endl;
				continue;
			}
			aNode.vLinkID.push_back(aLinkNo);
			if( bDBGMode ) cerr << "\r read NODE.. " <<  i+1 << "/" << m_MrxInfo.m_Header.eNodeNs << " " << aLinkNo;
		}		
		short	aRegCodeNs;
		//交通規制情報個数を取得
		ifp.read((char*)&aRegCodeNs,sizeof(short));
		if(!ifp){
			cerr << "ERROR CMrxLoad::read() m_MrxInfo.m_Nodes.eNodeNs (" << i << ") " << endl;
			continue;
		}
		m_MrxInfo.m_Nodes.push_back(aNode);
	}
	if( bDBGMode ) cerr << endl;
	return true;
}
/**
 * 指定昭文社ＭＲＸ（Mapple Routing eXchange）リンクデータの取得
 * @note	指定昭文社ＭＲＸリンクデータを取得します
 * @param	cFileHDL	(I)	MRXデータファイルハンドル
 * @return	true：取得成功、false：取得失敗
 */
bool CMrxLoad::loadLinks(std::ifstream& ifp)
{
	bool	bDBGMode=false;
#ifdef	_DEBUG___
	bDBGMode=true;
#endif
	//リンク情報オフセット検査
	ifp.seekg(m_MrxInfo.m_Header.eLinkOffset, ios::beg);

	// リンク要素部の取得
	for( U_LONG i=0; i<m_MrxInfo.m_Header.eLinkNs; i++ ){
		MRXLink		aLink;
		ifp.read((char*)&aLink.eLinks,sizeof(MRX_Link));
		if(!ifp){
			cerr << "ERROR CMrxLoad::read() m_MrxInfo.eLinkNs (" << i << ")" << endl;
			continue;
		}
		//形状点を取得
		for( short j=0; j<aLink.eLinks.eCount; j++ ){
			MRX_Pos	aPos;
			ifp.read((char*)&aPos,sizeof(MRX_Pos));
			if(!ifp){
				cerr << "ERROR CMrxLoad::read() m_MrxInfo.eCount (" << i << "/" << m_MrxInfo.m_Header.eLinkNs << ")" << endl;
				continue;
			}
			aLink.vLinkPos.push_back(aPos);
			if( bDBGMode ){
				cerr << "\r read LINK.. " << i+1 << "/" << m_MrxInfo.m_Header.eLinkNs << " "
												 << aLink.eLinks.eSnodeID << "→"
												 << aLink.eLinks.eEnodeID << " : ("
												 << aPos.ePos[0] << " - " << aPos.ePos[1] << ")";
			}
		}
		m_MrxInfo.m_Links.push_back(aLink);
	}
	if( bDBGMode )	cerr << endl;
	return true;
}
/**
 * 指定昭文社ＭＲＸ（Mapple Routing eXchange）グループデータの取得
 * @note	指定昭文社ＭＲＸグループデータを取得します
 * @param	cFileHDL	(I)	MRXデータファイルハンドル
 * @return	true：取得成功、false：取得失敗
 */
bool CMrxLoad::loadGroups(std::ifstream& ifp)
{
	bool	bDBGMode=false;
#ifdef	_DEBUG___
	bDBGMode=true;
#endif
	//グループ情報オフセット検査
	ifp.seekg(m_MrxInfo.m_Header.eGroupLineOffset, ios::beg);

	// グループ情報要素部の取得
	for( U_LONG i=0; i<m_MrxInfo.m_Header.eGroupLineNs; i++ ){
		MRXGroup	aGroup;

		ifp.read((char*)&aGroup.eGroup,sizeof(MRX_Group));


		if( !ifp ){
			cerr << "ERROR CMrxLoad::read() m_MrxInfo.eGroupLineNs (" << i << ")" << endl;
			continue;
		}
		//構成リンク列を取得
		for( short j=0; j<aGroup.eGroup.eCount; j++ ){
			MRX_GroupId	aGID;
			ifp.read((char*)&aGID,sizeof(MRX_GroupId));

			if(!ifp){
				cerr << "ERROR CMrxLoad::read() m_MrxInfo.eCount ("
					  << i << "/" << m_MrxInfo.m_Header.eGroupLineNs << ")" << endl;
				continue;
			}
			aGroup.vGroupID.push_back(aGID);
			if( bDBGMode ){
				cerr << "\r road GRP..  "
					 << i+1 << "/" << m_MrxInfo.m_Header.eGroupLineNs << " "
					 << aGroup.eGroup.eGroupID << " : ("
					 << aGID.eID << "＝" << aGID.eDir << ")";
			}
		}
		m_MrxInfo.m_Group.push_back(aGroup);
	}
	if( bDBGMode )	cerr << endl;

	return true;
}
/**
 * 指定昭文社ＭＲＸ（Mapple Routing eXchange）データより道路を結合
 * @note	指定昭文社ＭＲＸグループデータより道路を結合します
 * @param	void
 * @return	true：結合成功、false：結合失敗
 */
bool CMrxLoad::bindGroups(void)
{
	//グループ情報とリンク情報より、構成点列のコンテナを作成する
	long aOID=1;
	int aMeshcode = atoi(m_MrxInfo.m_Header.eMeshcode);			///< 昭文社メッシュコード
	int aType = 0 ;
	for( aType=1; aType<=3; aType++ )						///< (1:ぬけみち、2:うらみち、3:渋滞)
	{
		for(vector<MRXGroup>::iterator it1=m_MrxInfo.m_Group.begin(); it1!=m_MrxInfo.m_Group.end(); it1++ )
		{
			int	aKind=0;
				 if( !strcmp((*it1).eGroup.eKindCode,"mga@@@@")){ aKind=1; }//ぬけみち
			else if( !strcmp((*it1).eGroup.eKindCode,"mgb@@@@")){ aKind=2; }//うらみち
			else if( !strcmp((*it1).eGroup.eKindCode,"mgc@@@@")){ aKind=3; }//渋滞道路
			else { ; }
			if( aType!=aKind ){	continue; }							///< レイヤ別にコンテナ登録

			MRXLQLink	aLQ;										///< リンク列管理コンテナ
						aLQ.eType	  = aType;						///< リンク列種別コード
						aLQ.eMeshcode = aMeshcode;					///< メッシュコード
						aLQ.ePrimId	  = aOID++;						///< リンク列OBJECTID
						aLQ.eCount	  = 0;							///< 構成要素数(init)
						aLQ.eGroupID  = (*it1).eGroup.eGroupID;		///< 識別番号
			for( vector<MRX_GroupId>::iterator it2=(*it1).vGroupID.begin(); it2!=(*it1).vGroupID.end(); it2++ )
			{				
				MRX_Pos	aPos;
				MRXLink	*pMrxLink;									///< 道路リンク取得
				pMrxLink = m_MrxInfo.FindLink((*it2).eID);
				if( (*it2).eDir==1 ){								///< 正方向
					for(long i=0; i<pMrxLink->eLinks.eCount; i++ ){
						aPos.ePos[0] = pMrxLink->vLinkPos[i].ePos[0];
						aPos.ePos[1] = pMrxLink->vLinkPos[i].ePos[1];
						aLQ.vLinkPos.push_back(aPos);
						aLQ.eCount ++;
					}
				}
				else{												///< 逆方向
					for(long i=pMrxLink->eLinks.eCount-1; i>=0; i-- ){
						aPos.ePos[0] = pMrxLink->vLinkPos[i].ePos[0];
						aPos.ePos[1] = pMrxLink->vLinkPos[i].ePos[1];
						aLQ.vLinkPos.push_back(aPos);
						aLQ.eCount ++;
					}
				}
			}
			m_MrxInfo.m_LQ.push_back(aLQ);
		}
	}
#ifdef	ADDNW
//		aOID=1;
		aType=4;												///< (4:道路リンク)
		for(vector<MRXLink>::iterator it3=m_MrxInfo.m_Links.begin(); it3!=m_MrxInfo.m_Links.end(); it3++ ){
			MRXLQLink	aLQ;									///< リンク列管理コンテナ(必ずリンク１つで構成)
						aLQ.eType	  = aType;					///< リンク列種別コード
						aLQ.eMeshcode = aMeshcode;				///< メッシュコード
						aLQ.ePrimId	  = aOID++;					///< リンク列OBJECTID
						aLQ.eCount	  = 0;						///< 構成要素数(init)
						aLQ.eGroupID  = it3->eLinks.eLinkNo;	///< 道路種別コード
			for(long i=0; i<it3->eLinks.eCount; i++ ){
				MRX_Pos	aPos;
						aPos.ePos[0] = it3->vLinkPos[i].ePos[0];
						aPos.ePos[1] = it3->vLinkPos[i].ePos[1];
				aLQ.vLinkPos.push_back(aPos);
				aLQ.eCount ++;
			}
			m_MrxInfo.m_LQ.push_back(aLQ);
		}
//		aOID=1;
		aType=5;												///< (5:道路ノード)
		for(vector<MRXNode>::iterator it4=m_MrxInfo.m_Nodes.begin(); it4!=m_MrxInfo.m_Nodes.end(); it4++ ){
			MRX_Pos		aPos;
						aPos.ePos[0] = it4->eNodes.eLon;
						aPos.ePos[1] = it4->eNodes.eLat;
			MRXLQLink	aLQ;									///< リンク列管理コンテナ(必ずノード１つで構成)
						aLQ.eType	  = aType;					///< リンク列種別コード
						aLQ.eMeshcode = aMeshcode;				///< メッシュコード
						aLQ.ePrimId	  = aOID++;					///< リンク列OBJECTID
						aLQ.eCount	  = 1;						///< 構成要素数
						aLQ.eGroupID  = it4->eNodes.eNodeNo;	///< ノード番号
						aLQ.vLinkPos.push_back(aPos);
			m_MrxInfo.m_LQ.push_back(aLQ);
		}
#endif
	
	return true;
}
/**
 * 指定昭文社ＭＲＸ（Mapple Routing eXchange）データの取得
 * @note	指定昭文社ＭＲＸデータを取得します
 * @param	cFileStr	(I)	MRXデータへのパス
 * @return	true：取得成功、false：取得失敗
 */
bool CMrxLoad::load(char *cFileStr)
{
	bool	bDBGMode=false;

#ifdef	_DEBUG
	cerr << "Open [" << cFileStr << "]" << endl;	bDBGMode = true;
#endif
	//指定ＭＲＸファイルをオープン
	ifstream aFileStream(CA2CT(cFileStr), ios::in | ios::binary);

	if(!aFileStream){
		cerr << "ERROR CMrxLoad::OpenFile() [" << cFileStr << "]" << endl;
		return false;
	}

	//ヘッダー部の取得
	aFileStream.read((char*)&m_MrxInfo.m_Header,sizeof(MRXHead));
	if(!aFileStream){
		cerr << "ERROR CMrxLoad::read() m_MrxInfo.m_Header " << endl;
		return false;
	}
	// ノード要素部の取得
	if( !loadNodes(aFileStream)){
		return false;
	}
	// リンク要素部の取得
	if( !loadLinks(aFileStream)){
		return false;
	}
	// グループ情報要素部の取得
	if( !loadGroups(aFileStream)){
		return false;
	}
	if( !bindGroups()){	///< グループ情報毎に道路を接続
		return false;
	}
	//指定ＭＲＸファイルをクローズ
	aFileStream.close();

	return true;
}

/**
 * 指定昭文社ＭＤＸ（Mapple Drawing eXchange）データの取得
 * @note	指定昭文社ＭＤＸデータを取得します
 * @param	cFileStr	(I)	MDXデータへのパス
 * @return	true：取得成功、false：取得失敗
 */
bool CMdxLoad::load(char *cFileStr)
{
	bool	bDBGMode=false;

#ifdef	_DEBUG
	cerr << "Open [" << cFileStr << "]" << endl;	bDBGMode = true;
#endif

	ifstream aFileStream(CA2CT(cFileStr), ios::in | ios::binary);

	//指定ＭＤＸファイルをオープン
	if(!aFileStream)
	{
		cerr << "ERROR CMrxLoad::OpenFile() [" << cFileStr << "]" << endl;
		return false;
	}
	//ヘッダー部の取得
	aFileStream.read((char*)&m_MdxInfo.m_Header,sizeof(MDXHead));
	if( !aFileStream )
	{
		cerr << "ERROR CMdxLoad::read() m_MdxInfo.m_Header " << endl;
		return false;
	}

	// リンク情報要素部の取得
	if( !loadLinks(aFileStream)){
		return false;
	}

	// ポリライン情報要素部の取得
	if( !loadGroups(aFileStream)){
		return false;
	}

	// ポリゴン情報要素部の取得
	if( !loadPoly(aFileStream)){
		return false;
	}
	//指定ＭＤＸファイルをクローズ
	aFileStream.close();

	return true;
}

/**
 * 指定昭文社ＭＤＸポリゴンデータの取得
 * @note	指定昭文社ＭＤＸポリゴンデータを取得します
 * @param	cFileHDL	(I)	MDXデータファイルハンドル
 * @return	true：取得成功、false：取得失敗
 */
bool CMdxLoad::loadPoly(ifstream& ifp)
{
	bool	bDBGMode=false;
#ifdef	_DEBUG___
	bDBGMode=true;
#endif

	ifp.seekg(m_MdxInfo.m_Header.ePolyOffset, ios::beg);
	//リンク情報オフセット検査

	// ポリゴン要素部の取得
	for( U_LONG i=0; i<m_MdxInfo.m_Header.ePolyNs; ++i )
	{
		MDXPolygon		aPoly;
		ifp.read((char*)&aPoly.eMdxPoly,sizeof(MDX_Polygon));

		if(!ifp )
		{
			cerr << "ERROR CMdxLoad::read() m_MdxInfo.ePolyNs (" << i << ")" << endl;
			continue;
		}
		//形状点を取得
		for( short j=0; j<aPoly.eMdxPoly.eCount; j++ )
		{
			MRX_Pos	aPos;
			ifp.read((char*)&aPos,sizeof(MRX_Pos));

			 if(!ifp )
			{
				cerr << "ERROR CMdxLoad::read() m_MdxInfo.eCount (" << i << "/" << m_MdxInfo.m_Header.ePolyNs << ")" << endl;
				continue;
			}
			aPoly.vPolygonPos.push_back(aPos);
			if( bDBGMode )
			{
				cerr << "\r read Polygon.. " << i+1 << "/" << m_MdxInfo.m_Header.ePolyNs << " " 
					<< aPoly.eMdxPoly.eLavel << " : (" << aPos.ePos[0] << " - " << aPos.ePos[1] << ")";
			}
		}
		m_MdxInfo.m_Poly.push_back(aPoly);
	}
	if( bDBGMode )	cerr << endl;
	return true;
}

/**
 * 指定昭文社ＭＤＸ（Mapple Drawing eXchange）グループ線データの取得
 * @note	指定昭文社ＭＤＸグループ線データを取得します
 * @param	cFileHDL	(I)	MDXデータファイルハンドル
 * @return	true：取得成功、false：取得失敗
 */
bool CMdxLoad::loadGroups(std::ifstream& ifp)
{
	bool	bDBGMode=false;
#ifdef	_DEBUG___
	bDBGMode=true;
#endif
	//グループ情報オフセット検査
	ifp.seekg(m_MdxInfo.m_Header.eGroupLineOffset, ios::beg);

	// グループ情報要素部の取得
	for( U_LONG i=0; i<m_MdxInfo.m_Header.eGroupLineNs; i++ ){
		MRXGroup	aGroup;

		ifp.read((char*)&aGroup.eGroup,sizeof(MRX_Group));


		if( !ifp ){
			cerr << "ERROR CMrxLoad::read() m_MrxInfo.eGroupLineNs (" << i << ")" << endl;
			continue;
		}
		//構成リンク列を取得
		for( short j=0; j<aGroup.eGroup.eCount; j++ ){
			MRX_GroupId	aGID;
			ifp.read((char*)&aGID,sizeof(MRX_GroupId));

			if(!ifp){
				cerr << "ERROR CMrxLoad::read() m_MrxInfo.eCount ("
					  << i << "/" << m_MdxInfo.m_Header.eGroupLineNs << ")" << endl;
				continue;
			}
			aGroup.vGroupID.push_back(aGID);
			if( bDBGMode ){
				cerr << "\r road GRP..  "
					 << i+1 << "/" << m_MdxInfo.m_Header.eGroupLineNs << " "
					 << aGroup.eGroup.eGroupID << " : ("
					 << aGID.eID << "＝" << aGID.eDir << ")";
			}
		}
		m_MdxInfo.m_Group.push_back(aGroup);
	}
	if( bDBGMode )	cerr << endl;

	return true;
}

/**
 * 指定昭文社ＭＤＸ（Mapple Drawing eXchange）線データの取得
 * @note	指定昭文社ＭＤＸ線データを取得します
 * @param	cFileHDL	(I)	MDXデータファイルハンドル
 * @return	true：取得成功、false：取得失敗
 */
bool CMdxLoad::loadLinks(std::ifstream& ifp)
{
	bool	bDBGMode=false;
#ifdef	_DEBUG___
	bDBGMode=true;
#endif
	//リンク情報オフセット検査
	ifp.seekg(m_MdxInfo.m_Header.eLineOffset, ios::beg);

	// リンク要素部の取得
	for( U_LONG i=0; i<m_MdxInfo.m_Header.eLineNs; i++ ){
		MDXLink		aLink;
		ifp.read((char*)&aLink.eMdxLink,sizeof(MDX_Link));
		if(!ifp){
			cerr << "ERROR CMrxLoad::read() m_MrxInfo.eLinkNs (" << i << ")" << endl;
			continue;
		}
		//形状点を取得
		for( short j=0; j<aLink.eMdxLink.eCount; j++ ){
			MRX_Pos	aPos;
			ifp.read((char*)&aPos,sizeof(MRX_Pos));
			if(!ifp){
				cerr << "ERROR CMrxLoad::read() m_MrxInfo.eCount (" << i << "/" << m_MdxInfo.m_Header.eLineNs << ")" << endl;
				continue;
			}
			aLink.vLinkPos.push_back(aPos);
			if( bDBGMode ){
				cerr << "\r read LINK.. " << i+1 << "/" << m_MdxInfo.m_Header.eLineNs << " "
												 << aPos.ePos[0] << " - " << aPos.ePos[1] << ")";
			}
		}
		m_MdxInfo.m_Links.push_back(aLink);
	}
	if( bDBGMode )	cerr << endl;
	return true;
}
