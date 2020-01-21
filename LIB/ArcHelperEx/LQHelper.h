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

// LQHelper.h: CLQHelper �N���X�̃C���^�[�t�F�C�X
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

	// �����������F�p�t�H�[�}���X�J�E���^�̒P�ʂ��l��
	void Init();
	// ���݂̃p�t�H�[�}���X�J�E���^�l��ۑ�
	void Start();
	// �O��ۑ������l�Ƃ̍�����Ԃ�
	DOUBLE Stop();
private:
	LONGLONG	perf_cnt;
	LONGLONG	start_time;
	DOUBLE		time_scale;
};
#endif

// LQ�A�C�e���N���X
class LQITEM {
	public:
		LQITEM() : OID(-1), InfoID(-1), LinkID(-1), MeshCode(-1), Sequence(-1), Direction(-1), LastF(-1) {};
		virtual ~LQITEM(){};

		bool operator==( const LQITEM& LQItem ) const
		{
			return OID == LQItem.OID ? TRUE : FALSE;
		};

	public:
		_IRowPtr		Object;		// IRow�p
		IGeometryPtr	Shape;
		CTargetItem		RoadItem;
		LONG			OID;		// IRow�AIFeature �p
		LONG			InfoID;
		LONG			LinkID;		// 
		LONG			MeshCode;
		LONG			Sequence;	// SEQUENCE �i�[�p�i���H�����N���i�[����ꍇ�̂݁j
		LONG			Direction;	// LINKDIR_C �i�[�p�i���H�����N���i�[����ꍇ�̂݁j
		LONG			LastF;
};

// �A�C�e���N���X
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
	 * @brief �K���`�F�b�N
	 * @param cRoadItems					���H���X�g
	 * @param ipBaseFromPoint			�I���������H���܂ތ`���From�|�C���g
	 * @param lRetErrorDrawMode			�����_�����[�h
	 * @param bIsCheckAll				���H���X�g�S�����`�F�b�N���邩
	 * @param bIsCheckFront				From�����`�F�b�N���邩
	 */
	BOOL LQCheck( const CTargetItems& cRoadItems, IPointPtr ipBaseFromPoint, LONG& lRetErrorDrawMode, BOOL bIsCheckFrom, BOOL bEditEnd, BOOL bIsCheckAll=TRUE, LONG lOwnerOID=-1, std::vector<LONG>* vecVICSID=NULL, bool bInit=true );
	//!< �K���`�F�b�N
	BOOL LQCheck( const CTargetItem& cRoadItem, const CComVariant& vaNewValue_Oneway=-1, const CComVariant& vaNewValue_NoPassage=-1, const CComVariant& vaNewValue_OutCourse=-1 );
	/**
	 * @brief �K���`�F�b�N
	 * @param ipFeature ���H�����N
	 * @param vaNewValue_Oneway ����ʍs��ʃR�[�h -1�Ȃ����ʍs�`�F�b�N���Ȃ��i�f�t�H���g�j 
	 * @param vaNewValue_NoPassage �ʍs�֎~��ʃR�[�h -1�Ȃ�ʍs�֎~�`�F�b�N���Ȃ��i�f�t�H���g�j 
	 * @param vaNewValue_OutCourse �|�P�Ȃ�o�H�O�`�F�b�N���Ȃ��i�f�t�H���g�j
	 * @param excludeUTurn U�^�[�����`�F�b�N�Ώۂ��珜���ꍇ��true �i�f�t�H���g�Ffalse�j
	 * @return �G���[���������TRUE
	 */
	// TODO: excludeUTurn �͂�����Ή��ibug 10617�j
	//       �����A���O�Ώۂ�������Ȃ�΁AsindyeRoadQueueType �̃��X�g��n���悤�ɂ���i���H�j
	//       �܂��A����ł���������͔ۂ߂Ȃ��B�B
	BOOL LQCheck( IFeaturePtr ipFeature, const CComVariant& vaNewValue_Oneway=-1, const CComVariant& vaNewValue_NoPassage=-1, const CComVariant& vaNewValue_OutCourse=-1, bool excludeUTurn = false );
//protected:
	void Initialization();
	IGeometryPtr GetGeometry(const CTargetItems& cRoadItems, IPointPtr ipBaseFromPoint);
	IGeometryPtr GetGeometry(LONG lInfoID, const std::list<LQITEM>& listLQ, std::map<LONG, IGeometryPtr> mapShape);
	std::map< LONG, LONG > GetInfoID(LONG lDrawMode, LONG LinkID, LPCTSTR lpszFieldName );
	/// LQ��\�[�g
	void LQItemsSort(std::list<LQITEM>& listLQ);
	void GetInfoLQ(LONG lDrawMode, LONG LinkID/*, map< LONG, list<LQITEM> >& mapInfo, map< LONG, IGeometryPtr >& mapShape*/);

	//////////////////////////////////////////////////////////////////////
	// �K���`�F�b�N�֐�
	//////////////////////////////////////////////////////////////////////
	/**
	 * @brief �K���`�F�b�N
	 * @param cRoadItem					�I���������H
	 * @param ipDrawGeometry				�I���������H�̑O�܂ł̌`��
	 * @param lDrawMode					�����_�����[�h
	 * @param SindyeRoadQueueCheckType	�`�F�b�N�`��̕���
	 */
	BOOL LQCheckByDrawMode( const CTargetItems& cRoadItems, const CTargetItem& cRoadItem, IGeometryPtr ipLQGeometry, IPointPtr ipBaseFromPoint, LONG lDrawMode, BOOL bIsCheckFrom, LONG lCount, BOOL bEnd=FALSE, BOOL bEditEnd=FALSE, LONG lOwnerOID=-1, std::vector<LONG>* vecVICSID=NULL );
	/**
	 * @brief �K���`�F�b�N(����ʍs)
	 * @param cRoadItem					�I���������H
	 * @param ipDrawGeometry				�I���������H�̑O�܂ł̌`��
	 * @param lDrawMode					�����_�����[�h
	 * @param SindyeRoadQueueCheckType	�`�F�b�N�`��̕���
	 */
	BOOL LQCheckByOneWay( const CTargetItem& cRoadItem, IGeometryPtr ipLQGeometry, IPointPtr ipBaseFromPoint, LONG lDrawMode, LONG lCount, BOOL bEnd, LONG lOwnerOID=-1 );
	/**
	 * @brief �K���`�F�b�N(�ʍs�֎~)
	 * @param CTargetItem	cRoadItem			�I���������H
	 * @param LONG			lDrawMode			�����_�����[�h
	 */
	BOOL LQCheckByNoPassage( const CTargetItem& cRoadItem, LONG lDrawMode, LONG lCount, BOOL bEnd, LONG lOwnerOID=-1 );

	BOOL LQCheckByLQ( const CTargetItem& cRoadItem, IGeometryPtr ipLQGeometry, IPointPtr ipBaseFromPoint, LONG lDrawMode, BOOL bIsCheckFrom, LONG lCount, BOOL bEnd=FALSE, LONG lOwnerOID=-1);
	/**
	 * @brief �K���`�F�b�N(�E���܋֎~)
	 * @param cRoadItem					�I���������H
	 * @param ipDrawGeometry				�I���������H�̑O�܂ł̌`��
	 * @param lDrawMode					�����_�����[�h
	 * @param SindyeRoadQueueCheckType	�`�F�b�N�`��̕���
	 */
	BOOL LQCheckByNoLRTurn( const CTargetItem& cRoadItem, IGeometryPtr ipLQGeometry, IPointPtr ipBaseFromPoint, LONG lDrawMode, BOOL bIsCheckFrom, LONG lCount, BOOL bEnd=FALSE, LONG lOwnerOID=-1 );
	/**
	 * @brief �d���`�F�b�N
	 * @param cRoadItem					�I���������H
	 * @param ipDrawGeometry				�I���������H�̑O�܂ł̌`��
	 * @param lDrawMode					�����_�����[�h
	 * @param SindyeRoadQueueCheckType	�`�F�b�N�`��̕���
	 */
	BOOL LQCheckBySameShapeDirection( const CTargetItem& cRoadItem, IGeometryPtr ipLQGeometry, IPointPtr ipBaseFromPoint, LONG lDrawMode, LONG lOwnerOID, BOOL bIsCheckFrom=FALSE );
	/// �o�H�O�`�F�b�N
	BOOL LQCheckByOutCourse( const CTargetItem& cRoadItem, LONG lDrawMode );
	/**
	 * @brief �K���`�F�b�N(����ʍs)
	 * @param cRoadItem					�I���������H
	 * @param lDrawMode					�����_�����[�h
	 */
	BOOL LQCheckByOneWay( IFeaturePtr ipFeature, const CComVariant& vaNewValue );
	/**
	 * @brief �K���`�F�b�N(�ʍs�֎~)
	 * @param ipFeature �I���������H
	 * @param vaNewValue �ʍs�֎~��ʃR�[�h
	 * @param excludeUTurn U�^�[�����`�F�b�N�Ώۂ��珜���ꍇ��true �i�f�t�H���g�Ffalse�j
	 * @return �G���[���������TRUE
	 */
	BOOL LQCheckByNoPassage( IFeaturePtr ipFeature, const CComVariant& vaNewValue, bool excludeUTurn = false );
	/// �o�H�O�`�F�b�N
	BOOL LQCheckByOutCourse( IFeaturePtr ipFeature, const CComVariant& vaNewValue );
	BOOL IsAdjoinDisconnect(const CTargetItems& cRoadItems, IPointPtr ipBaseFromPoint, LONG lDrawMode);
	BOOL LQCheckByVICSNoChange( const CTargetItems& cRoadItems, const CTargetItem& cRoadItem, IGeometryPtr ipLQGeometry, IPointPtr ipBaseFromPoint, LONG lDrawMode, std::vector<LONG>* vecVICSID=NULL );
	//
	BOOL LQCheckByLQ(ITablePtr ipInfoTableOwner, ITablePtr ipInfoTable, LONG CheckDrawMode, LONG lInfoID, const CTargetItem& cRoadItem, IGeometryPtr ipBaseGeometry, IGeometryPtr ipUninoGeom, LONG lDrawMode, BOOL bIsCheckFrom, LONG lCount, BOOL bEnd=FALSE, LONG lOwnerOID=-1 );
	BOOL IsVICSNoChange( const CTargetItems& cRoadItems, const CTargetItem& cRoadItem, IPointPtr ipBaseFromPoint, IPointPtr ipBaseToPoint, IGeometryPtr ipLQGeometry, LONG lDrawMode, std::vector<LONG>* vecVICSID=NULL);
	BOOL CheckVicsHitDir( const CTargetItem& cRoadItem, CTargetItems* cRoads, IPointPtr ipBaseFromPoint, IPointPtr ipBaseToPoint, IGeometryPtr ipLQGeometry, std::list<LQITEM>* lstLQ);
	IGeometryPtr GetOffset(IGeometryPtr ipGeometry, IPointPtr ipBasePonit, bool bChk=false);
	//////////////////////////////////////////////////////////////////////
	// �G���[
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
