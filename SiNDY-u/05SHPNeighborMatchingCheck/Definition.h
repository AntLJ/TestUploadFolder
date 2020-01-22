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
#include<vector>
#include<map>
#include<set>

//設定ファイルから読み込むパラメータ
struct Parameters
{
	std::string e_MeshList;			//チェック対象メッシュリスト
	std::string e_LayerList;		//チェック対象レイヤリスト
	std::string e_TKYDataPath;		//元データ(東京測地系)のデータが格納されているディレクトリ
	std::string e_JGDDataPath;		//変換後データ(世界測地系)のデータが格納されているディレクトリ
	std::string e_Area;				//データ全てのメッシュをチェックする場合:all 一部分チェックを行う場合:part
	std::string e_DataSet;			//チェック対象データセット(top,middle,base,city,road)
	std::string e_ErrorDir;			//エラーファイルの出力フォルダ
	double		e_Border;			//角度の許容誤差設定
	bool		e_WalkFlag;			//道路・歩行者データかどうか(オブジェクトキー:OBJECTIDとDIVID)
};
//座標
struct coord{
	double	e_X;
	double	e_Y;
	bool operator < (const coord &rCoord) const{
		if( e_X != rCoord.e_X){
			return e_X < rCoord.e_X;
		}else
		{
			return e_Y < rCoord.e_Y;
		}
	}
	bool operator == (const coord &rCoord) const{
		if( e_X == rCoord.e_X){
			return e_Y == rCoord.e_Y;
		}
		else{
			return false;
		}
	}
};

//TKYオブジェクト情報を格納するための構造体
struct TKYObjectData{
	long e_FID;
	esriGeometryType e_ShapeType;
	std::map<long,IPointPtr> e_PointSet;
	std::multimap<long,coord> e_PointInfo;					//補間点番号からの座標検索用
	std::multimap<coord,long> e_CoordInfo;					//座標からの補間点番号検索用
	bool e_multi;												//ポリゴンが複数のリングで作成されているか判定用
};

//JGDオブジェクト情報を格納するための構造体
struct JGDObjectData{
	long e_FID;
	esriGeometryType e_ShapeType;
	std::map<long,IPointPtr> e_PointSet;
	std::multimap<long,coord> e_PointInfo;					//補間点番号から座標検索用
	std::multimap<coord,long> e_CoordInfo;					//座標から補間点番号検索用
	bool e_multi;												//ポリゴンが複数のリングで作成されているか判定用
};

//オブジェクトのデフォルト値
const long DEFAULT_VALUE = -1;

//TKY,JGDのメッシュ情報のキーの値を格納するための構造体
struct ObjectDataKey{
	long e_OID;			//OBJECTIDもしくはそれ相当のID
	long e_DIVID;		//DIVID(道路・歩行者データの場合のみ使用)

	ObjectDataKey(void):e_OID(DEFAULT_VALUE),e_DIVID(DEFAULT_VALUE){};

	//比較関数
	//std::mapで独自の型(構造体など)をキーにするには構造体内の比較関数が必要。insert()やfind()で使用される
	bool operator< (const ObjectDataKey& aObjectDataKey) const
	{
		//e_OID比較
		if(e_OID < aObjectDataKey.e_OID)
			return true;
		if(e_OID > aObjectDataKey.e_OID)
			return false;
		//e_DIVID比較
		if(e_DIVID < aObjectDataKey.e_DIVID)
			return true;
		if(e_DIVID > aObjectDataKey.e_DIVID)
			return false;
		return false;
	};
};

//TKY,JGDのメッシュ情報を格納するための構造体
//multimapにしなきゃだめ！！！！！
typedef std::map<ObjectDataKey,TKYObjectData> TKYMeshData;
typedef std::map<ObjectDataKey,JGDObjectData> JGDMeshData;


struct TKYMeshInfo{
	long e_MeshCode;
	TKYMeshData e_TKYMeshData;
};

struct JGDMeshInfo{
	long e_MeshCode;
	JGDMeshData e_JGDMeshData;
};


template <class Type>
struct FiveMeshInfo
{
	Type e_MainMesh;
	Type e_NorthMesh;
	Type e_SouthMesh;
	Type e_EastMesh;
	Type e_WestMesh;
};


//比較リスト作成の為の構造体
struct CheckKey{
	long e_MMeshCode;			//着目メッシュのメッシュコード
	long e_MOID;				//着目メッシュ側のOID
	long e_MDIVID;				//着目メッシュ側のDIVID(道路・歩行者データの場合のみ使用)
	long e_MPointNum;			//着目メッシュ側の補間点番号
	long e_NMeshCode;			//隣接メッシュのメッシュコード
	long e_NOID;				//隣接メッシュ側のOID
	long e_NDIVID;				//隣接メッシュ側のDIVID(道路・歩行者データの場合のみ使用)
	long e_NPointNum;			//隣接メッシュ側の補間点番号
	long e_MFID;				//着目メッシュ側のFID
	long e_NFID;				//隣接メッシュ側のFID

	bool operator < (const CheckKey & rCheckKey) const {
		if (e_MMeshCode != rCheckKey.e_MMeshCode){
			return (e_MMeshCode < rCheckKey.e_MMeshCode);
		}
		else if (e_NMeshCode != rCheckKey.e_NMeshCode)
		{
			return (e_NMeshCode < rCheckKey.e_NMeshCode );

		}
		else if (e_MOID != rCheckKey.e_MOID)
		{
			return (e_MOID < rCheckKey.e_MOID);
		}
		else if (e_MPointNum != rCheckKey.e_MPointNum)
		{
			return (e_MPointNum < rCheckKey.e_MPointNum);
		}
		else if (e_NOID != rCheckKey.e_NOID)
		{
			return (e_NOID < rCheckKey.e_NOID);
		}
		else if (e_NPointNum != rCheckKey.e_NPointNum)
		{
			return (e_NPointNum < rCheckKey.e_NPointNum);
		}
		else if (e_MFID != rCheckKey.e_MFID)
		{
			return (e_MFID < rCheckKey.e_MFID);
		}
		else
		{
			return (e_NFID < rCheckKey.e_NFID);
		}

	}
};

struct FIDPair{
	long e_MFID;
	long e_NFID;
};

struct CheckElement{

	double e_arg1;
	double e_arg2;
	double e_arg3;
	double e_arg4;

	bool operator != (const CheckElement & rCheckElement) const{
		if( e_arg1 != rCheckElement.e_arg1 || e_arg2 != rCheckElement.e_arg2 
			|| e_arg3 != rCheckElement.e_arg3 || e_arg4 != rCheckElement.e_arg4)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

};

typedef std::map<CheckKey,CheckElement> CheckList;

typedef std::set<long> FID_Set;

