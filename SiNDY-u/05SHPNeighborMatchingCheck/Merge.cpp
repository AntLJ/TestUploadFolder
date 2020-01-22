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

#include "StdAfx.h"
#include "Merge.h"
#include "Math.h"

CMerge::CMerge(void)
{
}

CMerge::~CMerge(void)
{
}

bool CMerge::MakeList(std::multimap<Key,Info> &rMainMap, std::multimap<Key,Info> &rNeighborMap, std::set<CheckKey> & rMergeSet,long aShapeType)
{
	//rMainMap用のイテレータ
	std::multimap<Key,Info>::iterator Mitr; 
	//rNeghbor用のイテレータ
	std::multimap<Key,Info>::iterator Nitr;

	bool aCheck = false;



	Mitr = rMainMap.begin();
	
	if(aShapeType == 4)	
	{
		while(Mitr != rMainMap.end())
		{
		
			Nitr = rNeighborMap.find(Mitr->first);

			if( Nitr != rNeighborMap.end()){
				CheckKey aKey;
	
				//情報をマージして、rMergeMapへ
				aKey.e_MMeshCode = Mitr->second.e_MeshCode;
				aKey.e_MOID = Mitr->first.e_OID;
				aKey.e_MainPointNumber = Mitr->second.e_PointNumber;
				aKey.e_NMeshCode = Nitr->second.e_MeshCode;
				aKey.e_NOID = Nitr->first.e_OID;
				aKey.e_NeighborPointNumber = Nitr->second.e_PointNumber;
			
				rMergeSet.insert(aKey);

				aCheck = true;

				Nitr++;
				if(Nitr != rNeighborMap.end()){					
				//始点と終点の重なりがないかチェック	
					if(Mitr->first == Nitr->first){
						aKey.e_NMeshCode = Nitr->second.e_MeshCode;
						aKey.e_NOID = Nitr->first.e_OID;
						aKey.e_NeighborPointNumber = Nitr->second.e_PointNumber;
						rMergeSet.insert(aKey);
						Nitr++;
					}
				}
					
				
			}
			Mitr++;
		}
	}
	else		
	{
		while(Mitr != rMainMap.end())
		{
			Nitr = rNeighborMap.find(Mitr->first);
			if( Nitr != rNeighborMap.end()){
				CheckKey aKey;

				//情報をマージして、rMergeMapへ
				aKey.e_MMeshCode = Mitr->second.e_MeshCode;
				aKey.e_MOID = Mitr->second.e_OID;
				aKey.e_MainPointNumber = Mitr->second.e_PointNumber;
				aKey.e_NMeshCode = Nitr->second.e_MeshCode;
				aKey.e_NOID = Nitr->second.e_OID;
				aKey.e_NeighborPointNumber = Nitr->second.e_PointNumber;
			
				rMergeSet.insert(aKey);

				aCheck = true;

				Nitr++;

				//始点と終点の重なりがないかチェック
				/*if((Nitr != rNeighborMap.end())&&(Mitr->first == Nitr->first)){
					aKey.e_NMeshCode = Nitr->second.e_MeshCode;
					aKey.e_NOID = Nitr->second.e_OID;
					aKey.e_NeighborPointNumber = Nitr->second.e_PointNumber;

					rMergeSet.insert(aKey);
					
				}*/
			}
			Mitr++;
		}

	}


	return aCheck;

}


bool CMerge::MakeListAllowError(std::multimap<Key,Info> &rMainMap, std::multimap<Key,Info> &rNeighborMap, std::set<CheckKey> &rMeregeSet, long aShapeType)
{
	//rMainMap用のイテレータ
	std::multimap<Key,Info>::iterator Mitr; 
	//rNeghbor用のイテレータ
	std::multimap<Key,Info>::iterator Nitr;

	bool aCheck = false;
	


	Mitr = rMainMap.begin();
	
	if(aShapeType == 4)	
	{
		while(Mitr != rMainMap.end())
		{
		
			Nitr = rNeighborMap.begin();

			for(;Nitr != rNeighborMap.end() ;Nitr++){
				if( (fabs( Nitr->first.e_X - Mitr->first.e_X ) < 0.000000001 )
					&& (fabs( Nitr->first.e_Y - Mitr->first.e_Y ) < 0.000000001 )
					&& (Nitr->first.e_OID == Mitr->first.e_OID))
				{

					CheckKey aKey;
	
					//情報をマージして、rMergeMapへ
					aKey.e_MMeshCode = Mitr->second.e_MeshCode;
					aKey.e_MOID = Mitr->first.e_OID;
					aKey.e_MainPointNumber = Mitr->second.e_PointNumber;
					aKey.e_NMeshCode = Nitr->second.e_MeshCode;
					aKey.e_NOID = Nitr->first.e_OID;
					aKey.e_NeighborPointNumber = Nitr->second.e_PointNumber;
			
					rMeregeSet.insert(aKey);

					aCheck = true;

					/*Nitr++;
					if(Nitr != rNeighborMap.end()){					
					//始点と終点の重なりがないかチェック	
						if((fabs( Nitr->first.e_X - Mitr->first.e_X ) < 0.000000001 )
							&& (fabs( Nitr->first.e_Y - Mitr->first.e_Y ) < 0.000000001 )
							&& (Nitr->first.e_OID == Mitr->first.e_OID))
						{
							aKey.e_NMeshCode = Nitr->second.e_MeshCode;
							aKey.e_NOID = Nitr->first.e_OID;
							aKey.e_NeighborPointNumber = Nitr->second.e_PointNumber;
							rMeregeSet.insert(aKey);
						}
					}*/
				}
				
			}
			Mitr++;
		}
	}
	else		
	{
		while(Mitr != rMainMap.end())
		{
			Nitr = rNeighborMap.begin();
			for(;Nitr != rNeighborMap.end() ;Nitr++){
				double aDeltaX = fabs(Nitr->first.e_X - Mitr->first.e_X);
				double aDeltaY = fabs(Nitr->first.e_Y - Mitr->first.e_Y);
				if( (aDeltaX < 0.000000001 )
					&& (aDeltaY < 0.000000001 ))
				{
					CheckKey aKey;

					//情報をマージして、rMergeMapへ
					aKey.e_MMeshCode = Mitr->second.e_MeshCode;
					aKey.e_MOID = Mitr->second.e_OID;
					aKey.e_MainPointNumber = Mitr->second.e_PointNumber;
					aKey.e_NMeshCode = Nitr->second.e_MeshCode;
					aKey.e_NOID = Nitr->second.e_OID;
					aKey.e_NeighborPointNumber = Nitr->second.e_PointNumber;
				
					rMeregeSet.insert(aKey);	

					aCheck = true;
				}
		
			}
			Mitr++;
		}

	}


	return aCheck;
}
	

			
				





