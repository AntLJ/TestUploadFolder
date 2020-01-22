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
#include <fstream>

struct ArgPoint{
	long k;			
	long l;
};

struct ArgPoints{
	ArgPoint e_StartNum;
	ArgPoint e_EndNum;
};

class CErrorCheck
{
public:
	CErrorCheck(void);
	~CErrorCheck(void);
	bool MakeTKYList(FiveMeshInfo<TKYMeshInfo> &rTKY5MeshInfo);
	bool CheckJGDData(FiveMeshInfo<JGDMeshInfo> &rJGD5MeshInfo,std::ofstream *pErrorFile,std::string aLayerName,double aBorder);

private:
	bool MatchingCheck(std::pair<coord,long> &rCoordInfo,  TKYObjectData &rMTKYObjectData,TKYMeshInfo &rNTKYMeshInfo,long MMeshCode,const ObjectDataKey &aObjectDataKey,long aFID);
	bool GetPolyLineArg(CheckElement *pArg,long aMPointNum,long aNPointNum,std::map<long,IPointPtr> &rMPointSet,std::map<long,IPointPtr> &rNPointSet);
	bool GetPolygonArg(CheckElement *pArg,long aMPointNum ,long aNPointNum ,std::map<long,IPointPtr> &rMPointSet,std::map<long,IPointPtr> &rNPointSet,ArgPoints &rArgPoints);
	bool InsertRow(long aMMesh,long aMOID, long aMDIVID, long aMPointNum, long aNMesh, long aNOID, long aNDIVID, long aNPointNum, CheckElement aArg,FIDPair aFIDPair);
	bool GetObjectData(JGDObjectData *pObjectData, std::pair<long,coord> *pPointInfo, JGDMeshInfo &rJGDMeshInfo,std::map<CheckKey,CheckElement>::iterator &Litr,long judge);
	CheckList m_CheckList;

	template<class type>
	bool GetArgPoint( ArgPoint *pArgPoint, type &rObjectData, long aPointNum)
	{
		if(rObjectData.e_multi){
			//該当補間点の座標を検索
			std::multimap<long,coord>::iterator itr = rObjectData.e_PointInfo.find(aPointNum);
			if( itr == rObjectData.e_PointInfo.end()){
				std::cerr << "programに異常があります。開発者に問合せてください(角度検出部)" << std::endl;
				return false;
			}
			//該当補間点と座標が一致する補間点の数を検索
			long aMCoordNum = rObjectData.e_CoordInfo.count( itr->second );
			if( aMCoordNum == 1 ){
				pArgPoint->k = aPointNum-1;
				pArgPoint->l = aPointNum+1;
			}
			else if( aMCoordNum == 2 ){
				//該当補間点と座標が一致する補間点の検索
				std::multimap<coord,long>::iterator itr2 = rObjectData.e_CoordInfo.find( itr->second );
				//一致する補間点の補間点番号
				long aPointNum2;
				for( int i=0; i<aMCoordNum; i++)
				{
					if( itr2->second != aPointNum ) aPointNum2 = itr2->second;
					++itr2;
				}
				//aPointNumとaPointNum2どちらが始点(終点)かによって処理を変える
				if( aPointNum > aPointNum2)
				{
					pArgPoint->k = aPointNum-1;
					pArgPoint->l = aPointNum2+1;
				}else
				{
					pArgPoint->k = aPointNum2-1;
					pArgPoint->l = aPointNum+1;
				}
			}
		}else
		{
			size_t aSize = rObjectData.e_PointSet.size();
			if(aPointNum == 0){
				pArgPoint->k = aSize-2;
			}
			else{
				pArgPoint->k = aPointNum-1;
			}
			if(aPointNum == (aSize-1)){
				pArgPoint->l = 1;
			}
			else{
				pArgPoint->l = aPointNum+1;
			}
		}
		return true;
	}

};

