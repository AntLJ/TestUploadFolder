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

// LQHelper.h: CLQHelper クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LQHELPER_H__34DEC58A_2201_ADD0_547D_321DEACC47DF__INCLUDED_)
#define AFX_LQHELPER_H__34DEC58A_2201_ADD0_547D_321DEACC47DF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LoadQueueInfo.h"

#ifdef _DEBUG
class CTimerPerformanceCounter
{
public:
	CTimerPerformanceCounter();
	~CTimerPerformanceCounter(){}

	// 初期化処理：パフォーマンスカウンタの単位を獲得
	void Init();
	// 現在のパフォーマンスカウンタ値を保存
	void Start();
	// 前回保存した値との差分を返す
	DOUBLE Stop();
private:
	LONGLONG	perf_cnt;
	LONGLONG	start_time;
	DOUBLE		time_scale;
};
#endif

// LQアイテムクラス
class LQITEM {
	public:
		LQITEM() : OID(-1), InfoID(-1), LinkID(-1), MeshCode(-1), Sequence(-1), Direction(-1), LastF(-1) {};
		virtual ~LQITEM(){};

		bool operator==( const LQITEM& LQItem ) const
		{
			return OID == LQItem.OID ? TRUE : FALSE;
		};

	public:
		_IRowPtr		Object;		// IRow用
		IGeometryPtr	Shape;
		CTargetItem		RoadItem;
		LONG			OID;		// IRow、IFeature 用
		LONG			InfoID;
		LONG			LinkID;		// 
		LONG			MeshCode;
		LONG			Sequence;	// SEQUENCE 格納用（道路リンクを格納する場合のみ）
		LONG			Direction;	// LINKDIR_C 格納用（道路リンクを格納する場合のみ）
		LONG			LastF;
};

// アイテムクラス
class CHECKITEM {
	public:
		CHECKITEM() : Mode(-1), Name(_T("")), bEditEnd(FALSE) {};
		virtual ~CHECKITEM(){};

		bool operator==( const CHECKITEM& CheckItem ) const
		{
			return Item.GetOID() == CheckItem.Item.GetOID() ? TRUE : FALSE;
		};

	public:
		CTargetItem		Item;
		LONG			Mode;
		BOOL			bEditEnd;
		CString			Name;
};

struct MAPITEMS {
	std::map< LONG, std::list<LQITEM> >	mapInfo;
	std::map< LONG, IGeometryPtr >	mapShape;
};

struct CreateGeomInfo {
	BOOL			bFirst;
	IPolylinePtr	ipPolyline;
	IPointPtr 		ipStartPoint;
	IPointPtr		ipEndToPoint;
};

class CLQHelper : 
	public CLoadQueueInfo
{
public:
	CLQHelper(){ Initialization(); }
	virtual ~CLQHelper(){}

	// LQEditTool/LQSkecthTool
	CLQHelper( IApplicationPtr ipApp, IWorkspacePtr ipWorkspace );
	// AttributeDlg
	CLQHelper( IApplicationPtr ipApp, IWorkspacePtr ipWorkspace, HWND hWnd=NULL, HWND hParent=NULL );
	// LQEditTool/LQSkecthTool
	void Init( IApplicationPtr ipApp, IWorkspacePtr ipWorkspace );
	// AttributeDlg
	void Init2( IApplicationPtr ipApp, IWorkspacePtr ipWorkspace, HWND hWnd=NULL, HWND hParent=NULL );
	/**
	 * @brief 規制チェック
	 * @param cRoadItems					道路リスト
	 * @param ipBaseFromPoint			選択した道路を含む形状のFromポイント
	 * @param lRetErrorDrawMode			レンダラモード
	 * @param bIsCheckAll				道路リスト全部をチェックするか
	 * @param bIsCheckFront				From側をチェックするか
	 */
	BOOL LQCheck( const CTargetItems& cRoadItems, IPointPtr ipBaseFromPoint, LONG& lRetErrorDrawMode, BOOL bIsCheckFrom, BOOL bEditEnd, BOOL bIsCheckAll=TRUE, LONG lOwnerOID=-1, std::vector<LONG>* vecVICSID=NULL, bool bInit=true );
	//!< 規制チェック
	BOOL LQCheck( const CTargetItem& cRoadItem, const CComVariant& vaNewValue_Oneway=-1, const CComVariant& vaNewValue_NoPassage=-1, const CComVariant& vaNewValue_OutCourse=-1 );
	/**
	 * @brief 規制チェック
	 * @param ipFeature 道路リンク
	 * @param vaNewValue_Oneway 一方通行種別コード -1なら一方通行チェックしない（デフォルト） 
	 * @param vaNewValue_NoPassage 通行禁止種別コード -1なら通行禁止チェックしない（デフォルト） 
	 * @param vaNewValue_OutCourse －１なら経路外チェックしない（デフォルト）
	 * @param excludeUTurn Uターンをチェック対象から除く場合はtrue （デフォルト：false）
	 * @return エラーが無ければTRUE
	 */
	// TODO: excludeUTurn はやっつけ対応（bug 10617）
	//       もし、除外対象が増えるならば、sindyeRoadQueueType のリストを渡すようにする（か？）
	//       まあ、それでもやっつけ感は否めない。。
	BOOL LQCheck( IFeaturePtr ipFeature, const CComVariant& vaNewValue_Oneway=-1, const CComVariant& vaNewValue_NoPassage=-1, const CComVariant& vaNewValue_OutCourse=-1, bool excludeUTurn = false );
//protected:
	void Initialization();
	IGeometryPtr GetGeometry(const CTargetItems& cRoadItems, IPointPtr ipBaseFromPoint);
	IGeometryPtr GetGeometry(LONG lInfoID, const std::list<LQITEM>& listLQ, std::map<LONG, IGeometryPtr> mapShape);
	std::map< LONG, LONG > GetInfoID(LONG lDrawMode, LONG LinkID, LPCTSTR lpszFieldName );
	/// LQ列ソート
	void LQItemsSort(std::list<LQITEM>& listLQ);
	void GetInfoLQ(LONG lDrawMode, LONG LinkID/*, map< LONG, list<LQITEM> >& mapInfo, map< LONG, IGeometryPtr >& mapShape*/);

	//////////////////////////////////////////////////////////////////////
	// 規制チェック関数
	//////////////////////////////////////////////////////////////////////
	/**
	 * @brief 規制チェック
	 * @param cRoadItem					選択した道路
	 * @param ipDrawGeometry				選択した道路の前までの形状
	 * @param lDrawMode					レンダラモード
	 * @param SindyeRoadQueueCheckType	チェック形状の方向
	 */
	BOOL LQCheckByDrawMode( const CTargetItems& cRoadItems, const CTargetItem& cRoadItem, IGeometryPtr ipLQGeometry, IPointPtr ipBaseFromPoint, LONG lDrawMode, BOOL bIsCheckFrom, LONG lCount, BOOL bEnd=FALSE, BOOL bEditEnd=FALSE, LONG lOwnerOID=-1, std::vector<LONG>* vecVICSID=NULL );
	/**
	 * @brief 規制チェック(一方通行)
	 * @param cRoadItem					選択した道路
	 * @param ipDrawGeometry				選択した道路の前までの形状
	 * @param lDrawMode					レンダラモード
	 * @param SindyeRoadQueueCheckType	チェック形状の方向
	 */
	BOOL LQCheckByOneWay( const CTargetItem& cRoadItem, IGeometryPtr ipLQGeometry, IPointPtr ipBaseFromPoint, LONG lDrawMode, LONG lCount, BOOL bEnd, LONG lOwnerOID=-1 );
	/**
	 * @brief 規制チェック(通行禁止)
	 * @param CTargetItem	cRoadItem			選択した道路
	 * @param LONG			lDrawMode			レンダラモード
	 */
	BOOL LQCheckByNoPassage( const CTargetItem& cRoadItem, LONG lDrawMode, LONG lCount, BOOL bEnd, LONG lOwnerOID=-1 );

	BOOL LQCheckByLQ( const CTargetItem& cRoadItem, IGeometryPtr ipLQGeometry, IPointPtr ipBaseFromPoint, LONG lDrawMode, BOOL bIsCheckFrom, LONG lCount, BOOL bEnd=FALSE, LONG lOwnerOID=-1);
	/**
	 * @brief 規制チェック(右左折禁止)
	 * @param cRoadItem					選択した道路
	 * @param ipDrawGeometry				選択した道路の前までの形状
	 * @param lDrawMode					レンダラモード
	 * @param SindyeRoadQueueCheckType	チェック形状の方向
	 */
	BOOL LQCheckByNoLRTurn( const CTargetItem& cRoadItem, IGeometryPtr ipLQGeometry, IPointPtr ipBaseFromPoint, LONG lDrawMode, BOOL bIsCheckFrom, LONG lCount, BOOL bEnd=FALSE, LONG lOwnerOID=-1 );
	/**
	 * @brief 重複チェック
	 * @param cRoadItem					選択した道路
	 * @param ipDrawGeometry				選択した道路の前までの形状
	 * @param lDrawMode					レンダラモード
	 * @param SindyeRoadQueueCheckType	チェック形状の方向
	 */
	BOOL LQCheckBySameShapeDirection( const CTargetItem& cRoadItem, IGeometryPtr ipLQGeometry, IPointPtr ipBaseFromPoint, LONG lDrawMode, LONG lOwnerOID, BOOL bIsCheckFrom=FALSE );
	/// 経路外チェック
	BOOL LQCheckByOutCourse( const CTargetItem& cRoadItem, LONG lDrawMode );
	/**
	 * @brief 規制チェック(一方通行)
	 * @param cRoadItem					選択した道路
	 * @param lDrawMode					レンダラモード
	 */
	BOOL LQCheckByOneWay( IFeaturePtr ipFeature, const CComVariant& vaNewValue );
	/**
	 * @brief 規制チェック(通行禁止)
	 * @param ipFeature 選択した道路
	 * @param vaNewValue 通行禁止種別コード
	 * @param excludeUTurn Uターンをチェック対象から除く場合はtrue （デフォルト：false）
	 * @return エラーが無ければTRUE
	 */
	BOOL LQCheckByNoPassage( IFeaturePtr ipFeature, const CComVariant& vaNewValue, bool excludeUTurn = false );
	/// 経路外チェック
	BOOL LQCheckByOutCourse( IFeaturePtr ipFeature, const CComVariant& vaNewValue );
	BOOL IsAdjoinDisconnect(const CTargetItems& cRoadItems, IPointPtr ipBaseFromPoint, LONG lDrawMode);
	BOOL LQCheckByVICSNoChange( const CTargetItems& cRoadItems, const CTargetItem& cRoadItem, IGeometryPtr ipLQGeometry, IPointPtr ipBaseFromPoint, LONG lDrawMode, std::vector<LONG>* vecVICSID=NULL );
	//
	BOOL LQCheckByLQ(ITablePtr ipInfoTableOwner, ITablePtr ipInfoTable, LONG CheckDrawMode, LONG lInfoID, const CTargetItem& cRoadItem, IGeometryPtr ipBaseGeometry, IGeometryPtr ipUninoGeom, LONG lDrawMode, BOOL bIsCheckFrom, LONG lCount, BOOL bEnd=FALSE, LONG lOwnerOID=-1 );
	BOOL IsVICSNoChange( const CTargetItems& cRoadItems, const CTargetItem& cRoadItem, IPointPtr ipBaseFromPoint, IPointPtr ipBaseToPoint, IGeometryPtr ipLQGeometry, LONG lDrawMode, std::vector<LONG>* vecVICSID=NULL);
	BOOL CheckVicsHitDir( const CTargetItem& cRoadItem, CTargetItems* cRoads, IPointPtr ipBaseFromPoint, IPointPtr ipBaseToPoint, IGeometryPtr ipLQGeometry, std::list<LQITEM>* lstLQ);
	IGeometryPtr GetOffset(IGeometryPtr ipGeometry, IPointPtr ipBasePonit, bool bChk=false);
	//////////////////////////////////////////////////////////////////////
	// エラー
	//////////////////////////////////////////////////////////////////////
	BOOL DispErrorMessage(sindyeCheckErrorCode ErrorCode, LONG lDrawMode, BOOL bIsType=FALSE, LPCTSTR lpcszConMsg=NULL);
	BOOL DispErrorMessage(sindyeCheckErrorCode ErrorCode, LONG lDrawMode, LONG CheckDrawMode, LONG OID, LONG InfoID, BOOL bIsType=FALSE, LPCTSTR lpcszConMsg=NULL);
	BOOL DispErrorMessage(sindyeCheckErrorCode ErrorCode, LONG lDrawMode, LONG OID, BOOL bIsType=FALSE, LPCTSTR lpcszConMsg=NULL);
	BOOL IsEqualByPoints( IGeometryPtr ipSourceGeom, IGeometryPtr ipDestGeom, ISpatialReferencePtr ipSpRef=NULL, BOOL bDir=FALSE );
	BOOL IsEqualDirectionByPoints( IGeometryPtr ipSourceGeom, IGeometryPtr ipDestGeom, BOOL bIsCheckFrom=FALSE, ISpatialReferencePtr ipSpRef=NULL );
	long IsIncluedByPoints( IGeometryPtr ipSourceGeom, IGeometryPtr ipDestGeom, ISpatialReferencePtr ipSpRef=NULL );
//	void SetMouseCursor();
	std::list<LONG> GetContiguityMeshCodes();
	CString GetContiguityMeshCode();

	void GetRoadShapes(LONG lDrawMode, LPCTSTR lpcszIDs);
	IPolylinePtr MeargePolyline(IPolylinePtr ipSourceGeom, IPolylinePtr ipDestGeom, IPointPtr ipStartPoint);
	void CreateGeometry( CreateGeomInfo& geominfo, IGeometryPtr ipGeom, IPointPtr ipBaseFromPoint);
	void GetRoadLayer();
	BOOL GetConditionType( ITablePtr ipInfoTable, long InfID, long DrawMode );
	IPolylinePtr Segment2Polyline(_ISegmentPtr ipSegment, ISpatialReferencePtr ipSpRef);
	void GetCheckOperationAction(sindyeCheckType checkcode, long lCheckType, long lDrawMode, ITablePtr ipInfoTable, long lOwnerOID, sindyeCheckOperation* pCheckOPType, sindyeCheckAction* pCheckACTType );
	ITablePtr GetTable(LPCTSTR lpcszName);

public:
	IWorkspacePtr					m_ipWorkspace;
protected:
//	CMouseCursor					m_cMoueCursor;
	IApplicationPtr					m_ipApp;
	IQueryFilterPtr 				m_ipQueryFilter;
	ILayerPtr						m_ipRoadLayer;
	ITablePtr						m_ipRoadTable;

	CString							m_strErrorMessage;

	sindyeCheckErrorType			m_Error;
	CHECKITEM						m_CheckItem;

	BOOL							m_bIsNoMessage;
	BOOL							m_bIsMessageBox;

	HWND							m_hWnd;
	HWND							m_hParent;

	std::map< LONG, MAPITEMS >		m_mapInfos;
	std::map< LONG, IGeometryPtr >	m_mapLQShape;
	std::map< LONG, LONG >			m_mapINF;

	std::map< LONG, CString>		m_mapMsg;
	bool							m_bInit;
	std::map<CString, CAdapt<ITablePtr>> m_mapTables;
};

#endif // !defined(AFX_LQHELPER_H__34DEC58A_2201_ADD0_547D_321DEACC47DF__INCLUDED_)
