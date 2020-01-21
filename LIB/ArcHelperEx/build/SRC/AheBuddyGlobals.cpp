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


#include "stdafx.h"
#include "AheBuddyGlobals.h"

// STL
#include <algorithm>

// ArcHelperEx
#include <GlobalFunctions.h>
#include <AheGlobalsCoord.h>
#include <AheGeometryOp.h>
#include <AheGeometryConvert.h>
#include <AheGlobalsMisc.h>

// crd_cnv
#include <crd_cnv/crd_cnv.h>


#ifdef _DEBUG
// DEBUG
#include "../../LIB/sindy/workspace.h"
#endif


extern crd_cnv g_cnv;

namespace sindy{

#ifdef _DEBUG
/**
 * @brief  [DEBUG�p] �t�B�[�`�����o�͂���
 * @param [in]  table    �o�͐�̃t�B�[�`���N���X
 * @param [in]  �o�͂���W�I���g��
 * @return �o�͂��ꂽ�t�B�[�`��
 */
IFeaturePtr insertFeature( ITablePtr ipTable, IGeometryPtr ipGeom )
{
	_IRowPtr row;
	ipTable->CreateRow(&row);

	((IFeaturePtr)row)->putref_Shape(ipGeom);
	if( SUCCEEDED(row->Store()) )
		return row;
	return nullptr;
}

/**
 * @brief  [DEBUG�p] �������C���̋O�Ղ��t�B�[�`���Ƃ��ďo�͂���
 * @detail �o�͐�̃t�B�[�`�����́uTRACE_LINE�v�ŌŒ�
 * @param [in]  ipPolyline  �o�͂��郉�C��
 * @retval true  �o�͐���
 * @retval false �o�͎��s
 */
bool outputSearchLine( IPolylinePtr ipPolyline )
{
	IFeatureWorkspacePtr ipWorkSpace = sindy::create_workspace(_T("..\\unittest\\TestData\\WalkNw.mdb"));
	IFeatureClassPtr ipFC;
	if( FAILED( ipWorkSpace->OpenFeatureClass(CComBSTR( _T("TRACE_LINE")), &ipFC ) ))
		return false;

	insertFeature( ipFC, ipPolyline );

	return true;
}

/**
 * @brief  [DEBUG�p] �������C���̋O�Ղ��t�B�[�`���Ƃ��ďo�͂���
 * @detail �o�͐�̃t�B�[�`�����́uBUFFER_LINK�v�ŌŒ�
 * @param [in]  ipBufGeo  �o�͂���Geometry
 * @retval true  �o�͐���
 * @retval false �o�͎��s
 */
bool outputBufferLine( IGeometryPtr ipBufGeo )
{
	IFeatureWorkspacePtr ipWorkSpace = sindy::create_workspace(_T("..\\unittest\\TestData\\WalkNw.mdb"));
	IFeatureClassPtr ipFC;
	if( FAILED( ipWorkSpace->OpenFeatureClass(CComBSTR( _T("BUFFER_LINK")), &ipFC ) ))
		return false;

	insertFeature( ipFC, ipBufGeo );

	return true;


}
#endif	// DEBUG


/**
 * @brief  ���݂̒n�����W�ł̋������烁�[�g�����v�Z����
 * @param  [in]  coordinate  ���݂̒n�����W�n�ł̋���
 * @param  [in]  geom        �n�����W�n�̊�Ƃ���W�I���g��
 * @return ���[�g���ł̋���
 * @attention   X���W�݂̂Ōv�Z���Z�o
 */
double coordinate2Meter( double coordinate, const IGeometryPtr& geom )
{
	// ���t�@�����X�|�C���g�쐬
	IPointPtr point;
	esriGeometryType geomType = esriGeometryPoint;
	geom->get_GeometryType( &geomType );
	switch( geomType )
	{
	case esriGeometryPoint:
		point = AheCreateClone(geom);
		break;
	case esriGeometryEnvelope:
		((IAreaPtr)geom)->get_Centroid(&point);
		break;
	default:
		((IPointCollectionPtr)geom)->get_Point(0,&point);
		break;
	}
 
	double dX(0.0f), dY(0.0f);
	point->QueryCoords( &dX, &dY );

	double dX2 = dX + coordinate , dY2 = dY;
	double dXDist(0.0f), dYDist(0.0f);
	return g_cnv.GetDistXY( dX, dY, dX2, dY2, &dXDist, &dYDist );
}


/**
 * @brief  ���[�g�����猻�݂̒n�����W�ł̋������v�Z����
 * @note   UpdateGenerationDiff �̃\�[�X�R�[�h����q��
 * @param  [in]  meter ���[�g��
 * @param  [in]  geom  �n�����W�n�̊�Ƃ���W�I���g��
 * @return �n�����W�n�ł̋���
 * @attention   X���W�݂̂Ōv�Z���Z�o
 */
double meter2Coordinate( double meter, const IGeometryPtr& geom )
{
		// ���t�@�����X�|�C���g�쐬
	IPointPtr point;
	esriGeometryType geomType = esriGeometryPoint;
	geom->get_GeometryType( &geomType );
	switch( geomType )
	{
	case esriGeometryPoint:
		point = AheCreateClone(geom);
		break;
	case esriGeometryEnvelope:
		((IAreaPtr)geom)->get_Centroid(&point);
		break;
	default:
		((IPointCollectionPtr)geom)->get_Point(0,&point);
		break;
	}

	double dX(0.0f), dY(0.0f);
	point->QueryCoords( &dX, &dY );

	double dLon(0.0f), dLat(0.0f);
	g_cnv.GetLLPt( dX, dY, meter, 0, &dLon, &dLat ); 

	return abs( dLon - dX );
}


/**
 * @brief  �|�����C�����Z�O�����g�P�ʂŕ��������|�����C���Q�ŕԂ�
 * @param  [in]  ipPolyline ��������|�����C��
 * @return ������̃|�����C���Q
 */
std::vector<CAdapt<IPolylinePtr>> splitPolyline( const IPolylinePtr& ipPolyline )
{
	ISpatialReferencePtr ipSpRef;
	ipPolyline->get_SpatialReference( &ipSpRef );

	// ������̃|�����C���Q�i�[�p
	std::vector<CAdapt<IPolylinePtr>> retPolylines;

	ISegmentCollectionPtr ipSegCol( ipPolyline );
	long lSegCount(-1L);
	ipSegCol->get_SegmentCount( &lSegCount );
	for( long i = 0; i < lSegCount; ++i )
	{
		_ISegmentPtr ipPartSeg;
		ipSegCol->get_Segment( i, &ipPartSeg );

		ISegmentCollectionPtr ipTmpSegCol(CLSID__Path);
		((IGeometryPtr)ipTmpSegCol)->putref_SpatialReference(ipSpRef);
		ipTmpSegCol->AddSegment(ipPartSeg);

		IGeometryCollectionPtr ipTmpGeoCol(CLSID_Polyline);
		((IGeometryPtr)ipTmpGeoCol)->putref_SpatialReference(ipSpRef);
		ipTmpGeoCol->AddGeometry(IGeometryPtr(ipTmpSegCol));

		IPolylinePtr ipTmpPolyline(ipTmpGeoCol);
		ipTmpPolyline->putref_SpatialReference( ipSpRef );

		retPolylines.push_back(ipTmpPolyline);
	}

	return retPolylines;
}


/**
 * @brief  �|�����C������݂ă|�C���g�����E�ǂ���̈ʒu�ɂ��邩�𔻒肷��
 * @param  [in]  ipBasePolyline ��ƂȂ�|�����C��
 * @param  [in]  ipPoint        ���肷��|�C���g
 * @return �E or ��
 */
LinkRelationPosition getPointPosition( const IPolylinePtr& ipBasePolyline, const IPointPtr& ipPoint )
{
	IPointPtr ipFromPoint, ipToPoint;
	ipBasePolyline->get_FromPoint(&ipFromPoint);
	ipBasePolyline->get_ToPoint(&ipToPoint);

	double dX1(0.0f), dY1(0.0f), dX2(0.0f), dY2(0.0f), dX3(0.0f), dY3(0.0f);
	ipFromPoint->QueryCoords( &dX1, &dY1 );
	ipToPoint->QueryCoords( &dX2, &dY2 );
	ipPoint->QueryCoords( &dX3, &dY3 );

	// �O�ςŔ���
	double dOuterProduct = (dX2 - dX1)*(dY3 - dY1) - (dY2 - dY1)*(dX3 - dX1);

	return ( dOuterProduct > 0 ) ? kLeft : kRight;
}

/**
 * @brief  �������C���쐬�֐�
 * @param  [in]  ipBaseLine       �������ƂȂ�|�����C��
 * @param  [in]  ipOriginPt		�@�����J�n�ʒu
 * @param  [in]  dSearchDistance  ��������
 * @param  [in]  nDegree          �������̃|�����C������Ƃ��������p�x
 * @param  [out] ipSearchLine     �������C��
 * @retval true  ����
 * @retval false ���s
 */
bool makeSearchLine( const IPolylinePtr& ipBaseLine, const IPointPtr& ipOriginPt,
				    double dSearchDistance, int nDegree, IPolylinePtr& ipSearchLine)
{
	// �������C�����쐬(�x�[�X�̃��C���� �ړ�->��]->�����ύX ���č��)
	ipSearchLine = AheCreateClone(ipBaseLine);

	//   �n�_�ʒu���ړ�
	double dOrigX(0.0f), dOrigY(0.0f), dFromX(0.0f), dFromY(0.0f);
	ipOriginPt->QueryCoords( &dOrigX, &dOrigY );

	IPointPtr ipBaseFromPt;
	ipBaseLine->get_FromPoint(&ipBaseFromPt);
	ipBaseFromPt->QueryCoords( &dFromX, &dFromY );

	double dDistX = dOrigX - dFromX;
	double dDistY = dOrigY - dFromY;
	if( FAILED(((ITransform2DPtr)ipSearchLine)->Move( dDistX, dDistY ))) {
		_ASSERTE(FALSE);
		return false;
	}

	//   �������C�����w��p�x��]   
	//     �x�� -> ���W�A�� �ϊ�
	double dRadian = ((double)nDegree / 360.0) * 2.0 * M_PI;
	//     �n�_�ʒu�����]
	if( FAILED(((ITransform2DPtr)ipSearchLine)->Rotate( ipOriginPt, dRadian ))){
		_ASSERTE(FALSE);
		return false;
	}

	//   �������C���̒������w�肵�������ɕϊ�
	double dLength(0.0);
	ipSearchLine->get_Length( &dLength );
	double dRatio = dSearchDistance / dLength;
	if( FAILED(((ITransform2DPtr)ipSearchLine)->Scale( ipOriginPt, dRatio, dRatio ))) {
		_ASSERTE(FALSE);
		return false;
	}
	return true;
}

/**
 * @brief  �������̃|�����C��(�x�[�X��)����A�����J�n�ʒu�E�����E�p�x���w�肵���͈͓��ɁA<br>
 *         ������̃|�����C�������݂��邩��Ԃ�
 * @detail ���������փ��C�����쐬���A�Ώۂƌ������邩�ǂ����Ŕ��肷��<br>
 *         ������Ȃ������ꍇ�� ePosition �� kNotFound ��Ԃ�(�Ԃ�l�� true)
 * @param  [in]  ipBaseLine       �������ƂȂ�|�����C��
 * @param  [in]  ipTargetLine     ������ƂȂ�|�����C��
 * @param  [in]  ipOriginPt       �����J�n�ʒu(��������Ɏw��p�x�A�����֌�������)
 * @param  [in]  dSearchDistance  ��������
 * @param  [in]  nDegree          �������̃|�����C������Ƃ��������p�x
 * @param  [out] ePosition        �^�[�Q�b�g������݂ăx�[�X�������E�ǂ���ɑ��݂��邩
 * @param  [out] dFindDistance    �x�[�X����^�[�Q�b�g�ւ̋���
 * @retval true  ����Ɍ�������
 * @retval false �������s���I��
 */
bool findLine( const IPolylinePtr& ipBaseLine, const IPolylinePtr& ipTargetLine, const IPointPtr ipOriginPt, 
			   double dSearchDistance, int nDegree, LinkRelationPosition& ePosition, double& dFindDistance )
{
	dFindDistance = DBL_MAX;
	ePosition = kNotFound;

	// �������C���쐬
	IPolylinePtr ipSearchLine;

	if( ! makeSearchLine( ipBaseLine, ipOriginPt, dSearchDistance, nDegree, ipSearchLine ) )
		return false;
		

// [DEBUG] �������C�����o��
//#ifdef _DEBUG
//	outputSearchLine( ipSearchLine );
//#endif


	// �|�����C�����m���������Ă��Ȃ���΁A������Ȃ������Ɣ���
	if( ( ! AheIsCross( ipSearchLine, ipTargetLine ) ))
		return true;


	// �|�����C�������������Ȃ�ȉ����擾����
	//   - ���������|�����C���̓x�[�X���̃|�����C�����猩�č��E�ǂ���Ɉʒu���邩 (-> ePosition)
	//   - ���������|�����C���܂ł̋��� (-> dFindDistance)

	// �^�[�Q�b�g�̃|�����C�����\�����郉�C������A�������C���ƌ������郉�C���Q���擾
	// (������������ӏ������������ꍇ���l��)
	std::vector<CAdapt<IPolylinePtr>> lines = splitPolyline( ipTargetLine );
	std::vector<CAdapt<IPolylinePtr>> find_polylines;
	for( std::vector<CAdapt<IPolylinePtr>>::const_iterator line_iter = lines.begin(); line_iter != lines.end(); ++line_iter )
	{
		if( AheIsCross( ipSearchLine, line_iter->m_T ) )
			find_polylines.push_back( *line_iter );
	}

	// �����������C���̂����ŒZ�����ɂ��郉�C���ɑ΂��āA
	// ���E�ʒu���Ƌ������擾����
	IPointPtr ipSearchFromPt;
	ipSearchLine->get_FromPoint(&ipSearchFromPt);
	dFindDistance = DBL_MAX;
	for( std::vector<CAdapt<IPolylinePtr>>::const_iterator line_iter = find_polylines.begin(); line_iter != find_polylines.end(); ++line_iter )
	{
		// AheIsCross�ŃN���X���肪�ł��Ƃ��Ă�AheGetCrossOrTouchPoints�ł́A���x�̈Ⴂ�ɂ��Touch�Ɣ��肳���P�[�X�����邽�߁A
		// Cross��Touch�����̃P�[�X���擾����B
		IGeometryPtr ipGeo = AheGetCrossOrTouchPoints( line_iter->m_T, ipSearchLine );
		if( ! ipGeo ) {
			_ASSERTE(FALSE);
			return false;
		}

		double dist(DBL_MAX);
		if( FAILED((( IProximityOperatorPtr)ipGeo)->ReturnDistance( ipSearchFromPt, &dist))) {
			_ASSERTE(FALSE);
			return false;
		}

		if( dist < dFindDistance ) {
			dFindDistance = dist;
			ePosition = getPointPosition( *line_iter, ipSearchFromPt );
		}

	}

	return true;
}

/**
 * @brief  �|�����C���̎n�_�ƏI�_�̒��_���擾����
 * @param  [in]  ipPolyline ���_�����߂�|�����C��
 * @return �|�����C���̒��_
 */
IPointPtr getMiddlePoint( const IPolylinePtr& ipPolyline )
{
	ISpatialReferencePtr ipRef;
	ipPolyline->get_SpatialReference( &ipRef );

	IPointPtr ipFromPt, ipToPt;
	ipPolyline->get_FromPoint(&ipFromPt);
	ipPolyline->get_ToPoint(&ipToPt);

	double dFromX(0.0f), dFromY(0.0f), dToX(0.0f), dToY(0.0f);	
	ipFromPt->QueryCoords( &dFromX, &dFromY );
	ipToPt->QueryCoords( &dToX,&dToY );

	double dDistX = dToX - dFromX;
	double dDistY = dToY - dFromY;

	IPointPtr ipMiddlePt(CLSID_Point);
	ipMiddlePt->putref_SpatialReference( ipRef );

	ipMiddlePt->put_X( dFromX + dDistX / 2.0 );
	ipMiddlePt->put_Y( dFromY + dDistY / 2.0 );

	return ipMiddlePt;
}


/**
 *	@brief ���E������i�[�p�̍\����
 */
struct RightLeftRelInfo
{
public:
	RightLeftRelInfo() : mRightCnt(0), mLeftCnt(0), mMinRightDist(DBL_MAX), mMinLeftDist(DBL_MAX){}
	~RightLeftRelInfo(){}

	// ���̃N���A
	void clear(){  mRightCnt = 0; mLeftCnt = 0; mMinRightDist = DBL_MAX; mMinLeftDist = DBL_MAX; }
	// ���E��񂪊i�[����Ă��邩�ǂ���
	bool isEmpty(){ return mRightCnt == 0 && mLeftCnt == 0; }

	// ���E�����X�V
	void addRelInfo( LinkRelationPosition position, double distance )
	{
		if( position == kRight ) {
			++mRightCnt;
			if( mMinRightDist > distance )
				mMinRightDist = distance;
		}
		else if( position == kLeft ) {
			++mLeftCnt;
			if( mMinLeftDist > distance )
				mMinLeftDist = distance;
		}	
	}

	// ���E�̌����������Ƌ�������ǂ��瑤�Ɉʒu���郊���N����Ԃ�
	LinkRelationPosition getRightOrLeft()
	{
		using namespace sindy;

		if( isEmpty() )
			return kNotFound;

		if( mRightCnt > mLeftCnt )
			return kRight;
		else if( mRightCnt < mLeftCnt )
			return kLeft;

		if( mMinRightDist < mMinLeftDist )
			return kRight;
		else if( mMinRightDist > mMinLeftDist )
			return kLeft;
			
		// �����܂ňꏏ�Ȃ�����E��Ԃ��Ă��܂�
		return kRight;
	}

	// �w�肳�ꂽ�����̍ŒZ������Ԃ�
	double getMinDist( LinkRelationPosition ePostion )
	{
		switch ( ePostion )
		{
			case kLeft:
				return mMinLeftDist;
			case kRight:
				return mMinRightDist;
			default:
				return 0;
		}
	}

private:
	long   mRightCnt;		//!< �E���Ɍ����������C����
	long   mLeftCnt;		//!< �����Ɍ����������C����
	double mMinRightDist;	//!< �E���Ɍ����������C���ւ̍ŒZ����
	double mMinLeftDist;	//!< �����Ɍ����������C���ւ̍ŒZ����
};


/**
 * @brief  �x�[�X�̃|�����C������^�[�Q�b�g�̃|�����C���֌������Ĉʒu�֌W���擾����
 * @param  [in]  ipBasePolyline    �x�[�X���̃|�����C��
 * @param  [in]  ipTargetPolyline  �^�[�Q�b�g���̃|�����C��
 * @param  [in]  dSearchDistance   ��������
 * @param  [out] RelInfo           �擾���ꂽ�ʒu�֌W
 */
bool searchBaseToTarget(const IPolylinePtr& ipBasePolyline, const IPolylinePtr& ipTargetPolyline, double dSearchDistance, RightLeftRelInfo& RelInfo )
{
	RelInfo.clear();

	std::vector<CAdapt<IPolylinePtr>> BasePolylines = splitPolyline( ipBasePolyline );

	// �����擾�p
	double dDistance(DBL_MAX);
	LinkRelationPosition position = kNotFound;

	// �n�_���獶�E�ɐ����������Č���
	IPointPtr ipFromPt;
	ipBasePolyline->get_FromPoint(&ipFromPt);
	//   ���Ɍ���
	if( ! findLine( BasePolylines.front(), ipTargetPolyline, ipFromPt, dSearchDistance, 90, position, dDistance))
		return false;
	if( position != kNotFound )
		RelInfo.addRelInfo( kLeft, dDistance );
	//  �E�Ɍ���
	if( ! findLine( BasePolylines.front(), ipTargetPolyline, ipFromPt, dSearchDistance, -90, position, dDistance ))
		return false;
	if( position != kNotFound )
		RelInfo.addRelInfo( kRight, dDistance );

	// �I�_���獶�E�ɐ����������Č���
	IPointPtr ipToPt;
	ipBasePolyline->get_ToPoint(&ipToPt);
	//   ���Ɍ���
	if( ! findLine( BasePolylines.back(), ipTargetPolyline, ipToPt, dSearchDistance, 90, position, dDistance))
		return false;
	if( position != kNotFound )
		RelInfo.addRelInfo( kLeft, dDistance );
	//   �E�Ɍ���
	if( ! findLine( BasePolylines.back(), ipTargetPolyline, ipToPt, dSearchDistance, -90, position, dDistance ))
		return false;
	if( position != kNotFound )
		RelInfo.addRelInfo( kRight, dDistance );


	// �|�����C�����\������e�Z�O�����g�̒��_���猟��
	for( std::vector<CAdapt<IPolylinePtr>>::const_iterator line_iter = BasePolylines.begin(); line_iter != BasePolylines.end(); ++line_iter )
	{
		// �Z�O�����g���_���獶�E�֐����������Č���
		IPointPtr ipSearchOrgPt = getMiddlePoint( *line_iter );
		//   ���Ɍ���
		if( ! findLine( *line_iter, ipTargetPolyline, ipSearchOrgPt, dSearchDistance, 90, position, dDistance))
			return false;
		if( position != kNotFound )
			RelInfo.addRelInfo( kLeft, dDistance );
		//   �E�Ɍ���
		if( ! findLine( *line_iter, ipTargetPolyline, ipSearchOrgPt, dSearchDistance, -90, position, dDistance ))
			return false;
		if( position != kNotFound )
			RelInfo.addRelInfo( kRight, dDistance );
	}
	return true;
}


/**
 * @brief  �^�[�Q�b�g�̃|�����C������x�[�X�̃|�����C���֌������Ĉʒu�֌W���擾����
 * @param  [in]  ipBasePolyline    �x�[�X���̃|�����C��
 * @param  [in]  ipTargetPolyline  �^�[�Q�b�g���̃|�����C��
 * @param  [in]  dSearchDistance   ��������
 * @param  [out] RelInfo           �擾���ꂽ�ʒu�֌W
 * @retval true
 */
bool searchTargetToBsse(const IPolylinePtr& ipBasePolyline, const IPolylinePtr& ipTargetPolyline, double dSearchDistance, RightLeftRelInfo& RelInfo )
{
	RelInfo.clear();

	std::vector<CAdapt<IPolylinePtr>> TargetPolylines = splitPolyline( ipTargetPolyline );

	// �����擾�p
	double dDistance(DBL_MAX);
	LinkRelationPosition position = kNotFound;

	// �n�_���獶�E�ɐ����������Č���
	IPointPtr ipFromPt;
	ipTargetPolyline->get_FromPoint(&ipFromPt);
	//   ���Ɍ���
	if( ! findLine( TargetPolylines.front(), ipBasePolyline, ipFromPt, dSearchDistance, 90, position, dDistance ))
		return false;
	RelInfo.addRelInfo( position, dDistance );
	//   �E�Ɍ���
	if( ! findLine( TargetPolylines.front(), ipBasePolyline, ipFromPt, dSearchDistance, -90, position, dDistance ))
		return false;
	RelInfo.addRelInfo( position, dDistance );

	// �I�_���獶�E�ɐ����������Č���
	IPointPtr ipToPt;
	ipTargetPolyline->get_ToPoint(&ipToPt);
	//   ���Ɍ����@ 
	if( ! findLine( TargetPolylines.back(), ipBasePolyline, ipToPt, dSearchDistance, 90, position, dDistance))
		return false;
	RelInfo.addRelInfo( position, dDistance );
	//    �E�Ɍ���
	if( ! findLine( TargetPolylines.back(), ipBasePolyline, ipToPt, dSearchDistance, -90, position, dDistance ))
		return false;
	RelInfo.addRelInfo( position, dDistance );

	// �|�����C�����\������e�Z�O�����g�̒��_���猟��
	for( std::vector<CAdapt<IPolylinePtr>>::const_iterator line_iter = TargetPolylines.begin(); line_iter != TargetPolylines.end(); ++line_iter )
	{
		// �Z�O�����g�̒��_���擾
		IPointPtr ipSearchOrgPt = getMiddlePoint( *line_iter );

		// ���_���獶�E�֐����������Č���
		//   ���Ɍ���
		if( ! findLine( *line_iter, ipBasePolyline, ipSearchOrgPt, dSearchDistance, 90, position, dDistance))
			return false;
		RelInfo.addRelInfo( position, dDistance );
		//   �E�Ɍ���
		if( ! findLine( *line_iter, ipBasePolyline, ipSearchOrgPt, dSearchDistance, -90, position, dDistance ))
			return false;
		RelInfo.addRelInfo( position, dDistance );
	}
	return true;
}


// ��̃|�����C�����猩�āA�^�[�Q�b�g�̃|�����C�������E�ǂ���Ɉʒu���邩�𔻒肷��
bool AheJudgeLinkRelation( const IPolylinePtr& ipBasePolyline, const IPolylinePtr& ipTargetPolyline,  double dSearchMeter, LinkRelationPosition& eRelationInfo, double& dMinMeter )
{
	if( ! ipBasePolyline || ! ipTargetPolyline || dSearchMeter <= 0 )
		return false;

	eRelationInfo = kNotFound;
	dMinMeter = -1;

	// ���E������i�[�p�̍\����
	RightLeftRelInfo rel_info;

	// �x�[�X�ƃ^�[�Q�b�g�̒������擾
	double dBaseLength(DBL_MAX), dTargetLength(DBL_MAX);
	ipBasePolyline->get_Length( &dBaseLength );
	ipTargetPolyline->get_Length( &dTargetLength );

	if( dBaseLength == 0.0 || dTargetLength == 0.0 )
		return false;

	// ���[�g��->�n�����W�ϊ�
	double dSearchDistance = meter2Coordinate( dSearchMeter, ipBasePolyline );

	// �x�[�X���̃|�����C���̕����Z����΁A�x�[�X�������Ɍ������A
	// ������Ȃ���΃^�[�Q�b�g�������Ɍ���
	if( dBaseLength < dTargetLength )
	{
		if( ! searchBaseToTarget( ipBasePolyline, ipTargetPolyline, dSearchDistance, rel_info ))
			return false;
		
		if( rel_info.isEmpty() && ! searchTargetToBsse( ipBasePolyline, ipTargetPolyline, dSearchDistance, rel_info ))
			return false;
	}
	else
	{
		// �^�[�Q�b�g���̃|�����C���̕����Z����΁A�^�[�Q�b�g�������Ɍ������A
		// ������Ȃ���΃x�[�X�������Ɍ���
		if( ! searchTargetToBsse( ipBasePolyline, ipTargetPolyline, dSearchDistance, rel_info ))
			return false;

		if( rel_info.isEmpty() && ! searchBaseToTarget( ipBasePolyline, ipTargetPolyline, dSearchDistance, rel_info ))
			return false;
	}

	if( ! rel_info.isEmpty() ) {
		eRelationInfo = rel_info.getRightOrLeft();
		dMinMeter = coordinate2Meter(rel_info.getMinDist( eRelationInfo ), ipBasePolyline);
		return true;
	}

	return true;
}

bool AheJudgeLinkRelation( const IPolylinePtr& ipBasePolyline, const IPolylinePtr& ipTargetPolyline, double dSearchMeter, LinkRelationPosition& eRelationInfo )
{
	// �ꎞ�i�[�p�ϐ�
	double minMeter = 0.0;
	return AheJudgeLinkRelation(ipBasePolyline, ipTargetPolyline, dSearchMeter, eRelationInfo, minMeter);
}

/**
 * @brief  �`�F�b�N����Ώۃ����N�擾�֐�
 * @param  [in] ipGeomCol ���f�����N�W��
 * @param  [in] ipBasePolyline �`�F�b�N�̍ۂɊ�ƂȂ郊���N
 * @param  [in] eRelationInfo ���f�O�����N�̊�����N����̑��Έʒu
 * @param  [in] dSerachMeter  ���Έʒu��������(m)
 * @param  [out] ipJudgeLine �`�F�b�N����Ώۃ����N
 * @retval true ����
 * @retval false ���s
 */
bool selectJudgeLine( const IGeometryCollectionPtr& ipGeomCol,
					  const IPolylinePtr& ipBasePolyline,
					  LinkRelationPosition eRelationInfo,
					  double dSearchMeter,
					  IPolylinePtr& ipJudgeLine )
{
	// ���ʕ��������f����Ă���ꍇ�́A�e���f�����N�ɑ΂��đ��Έʒu���擾���A�����O�`��Ƃ̑��Έʒu����v���A
	// ����������N�Ɉ�ԋ߂������N�ɑ΂��āA���菈�����s���B(��ԋ߂������N����������ꍇ�̓����N�����A
	// ��Ԓ��������N�Ŕ���)

	// ������L�΂������̋����ŏ��l�i�[�p�ϐ��������l�́AdSerachMeter+1.0��ݒ�(dSearchMeter��蒷���Ȃ邱�Ƃ͖�������(���m�ł͂Ȃ��̂ňꉞ�{1.0����))
	double dMinJudgeMeter = dSearchMeter+1.0;

	// ����ΏۂɂȂ肤�镪�f�����N�̋����ő�l�i�[�p�ϐ�
	long count = 0;
	if ( FAILED(ipGeomCol->get_GeometryCount(&count)) ) 
		return false;
	double dMaxPartLinkLength = 0.0;
	for (long i = 0; i < count; ++i) {
		IGeometryPtr ipPartGeo;
		if ( FAILED(ipGeomCol->get_Geometry(i, &ipPartGeo)) )
			return false;
		IPolylinePtr ipPartLine(ToPolyline(ipPartGeo));
		if ( !ipPartLine )
			return false;
		// ���f�����N�ɑ΂��čēx�A���E��������{
		LinkRelationPosition ePartRelation = kNotFound;
		double dPartJudgeMeter = 0.0;
		if ( !AheJudgeLinkRelation(ipBasePolyline, ipPartLine, dSearchMeter, ePartRelation, dPartJudgeMeter) )
			return false;
		// ���f�O�����N�Ƒ��Έʒu����v���Ȃ��ꍇ�͔�΂�
		if ( eRelationInfo != ePartRelation ) 
			continue;
		// dMinJudgeDistacnce���߂��ꍇ�́A�X�V
		// dMinJudgeDistance�Ɠ��������̏ꍇ�́A���f�����N�̒�����dMaxPartLinkLength��蒷���ꍇ�A�X�V
		double dPartLength = 0.0;
		if ( FAILED(((IPolylinePtr)ipPartLine)->get_Length(&dPartLength)) )
			return false;
		if ( dMinJudgeMeter > dPartJudgeMeter ) {
			ipJudgeLine = ipPartLine;
			dMinJudgeMeter = dPartJudgeMeter;
			dMaxPartLinkLength = dPartLength;
		} else if ( dMinJudgeMeter == dPartJudgeMeter ) {
			if ( dMaxPartLinkLength < dPartLength ) {
				ipJudgeLine = ipPartLine;
				dMinJudgeMeter = dPartJudgeMeter;
				dMaxPartLinkLength = dPartLength;
			}
		}
	}
	return true;
}

/**
 * @brief  �Z�O�����g�`��o�b�g��[�̃o�b�t�@���쐬
 * @param  [in]  ipSegmentLine	�ϊ����|�����C��
 * @param  [in]	 ipPreSegBuf	��O�̃Z�O�����g�̃o�b�t�@�`��
 * @param  [in]	 ipPreToLeftPt	��O�̃Z�O�����g�̃o�b�t�@�`��̃Z�O�����g�̏I�_���̍����̃|�C���g
 * @param  [in]	 ipPreToRightPt ��O�̃Z�O�����g�̃o�b�t�@�`��̃Z�O�����g�̏I�_���̉E���̃|�C���g
 * @param  [in]  dBufDsitance	�o�b�t�@����(�x)
 * @param  [out] ipSegmentBuf	�ϊ����|�����C���̃o�b�t�@�`��
 * @param  [out] ipConnectBuf	��O�̃Z�O�����g�̃o�b�t�@�`��ƍ���̃o�b�t�@�`��̌��Ԃ𖄂߂�`��
 * @param  [out] ipOutToLeftPt	�ϊ����|�����C���̃o�b�t�@�`��̃Z�O�����g�̏I�_���̍����̃|�C���g
 * @param  [out] ipOutToRightPt	�ϊ����|�����C���̃o�b�t�@�`��̃Z�O�����g�̏I�_���̉E���̃|�C���g
 * @retval true  ����
 * @retval false ���s
 */
bool makeSegmentBatBuf( const IPolylinePtr& ipSegmentLine,
					    const IGeometryPtr& ipPreSegBuf,
					    const IPointPtr& ipPreToLeftPt,
						const IPointPtr& ipPreToRightPt,
						double dBufDistance,
						IGeometryPtr& ipSegmentBuf,
						IGeometryPtr& ipConnectBuf,
						IPointPtr& ipOutToLeftPt,
						IPointPtr& ipOutToRightPt)
{
	IPointPtr ipSegFromPt, ipSegToPt;
	if ( FAILED(ipSegmentLine->get_FromPoint(&ipSegFromPt)) )
		return false;
	if ( FAILED(ipSegmentLine->get_ToPoint(&ipSegToPt)) )
		return false;

	IPolylinePtr ipFromLeftLine, ipFromRightLine, ipToLeftLine, ipToRightLine;
	// �n�_���獶�E��dBufDistance���̃��C���쐬
	if ( ! makeSearchLine(ipSegmentLine, ipSegFromPt, dBufDistance, 90, ipFromLeftLine) )
		return false;
	if ( ! makeSearchLine(ipSegmentLine, ipSegFromPt, dBufDistance, -90, ipFromRightLine) )
		return false;
	// �I�_���獶�E��dBufDistance���̃��C���쐬
	if ( ! makeSearchLine(ipSegmentLine, ipSegToPt, dBufDistance, 90, ipToLeftLine) )
		return false;
	if ( ! makeSearchLine(ipSegmentLine, ipSegToPt, dBufDistance, -90, ipToRightLine) )
		return false;
	// �e���C���̏I�_�擾
	IPointPtr ipFromLeftPt, ipFromRightPt, ipToLeftPt, ipToRightPt;
	if ( FAILED(ipFromLeftLine->get_ToPoint(&ipFromLeftPt)) ||
		 FAILED(ipFromRightLine->get_ToPoint(&ipFromRightPt)) ||
		 FAILED(ipToLeftLine->get_ToPoint(&ipToLeftPt)) ||
		 FAILED(ipToRightLine->get_ToPoint(&ipToRightPt)) )
		return false;

	ipOutToLeftPt = ipToLeftPt;
	ipOutToRightPt = ipToRightPt;

	// ��ԎQ�Ǝ擾
	ISpatialReferencePtr ipSpRef;
	if ( FAILED(ipSegmentLine->get_SpatialReference(&ipSpRef)) )
		return false;

	// ���`��̎n�_�E�I�_�Ǝ擾�����I�_�Ń|���S���쐬
	IGeometryCollectionPtr ipPolygon(CLSID_Polygon);
	if ( FAILED(IGeometryPtr(ipPolygon)->putref_SpatialReference(ipSpRef)) )
		return false;
	IPointCollectionPtr ipPtCol(CLSID_Ring);
	if ( FAILED(IGeometryPtr(ipPtCol)->putref_SpatialReference(ipSpRef)) )
		return false;
	if ( FAILED(ipPtCol->AddPoint(ipSegFromPt)) ||
		 FAILED(ipPtCol->AddPoint(ipFromLeftPt)) ||
		 FAILED(ipPtCol->AddPoint(ipToLeftPt)) ||
		 FAILED(ipPtCol->AddPoint(ipSegToPt)) ||
		 FAILED(ipPtCol->AddPoint(ipToRightPt)) ||
		 FAILED(ipPtCol->AddPoint(ipFromRightPt)) )
		 return false;
	IRingPtr ipRing(ipPtCol);
	if ( FAILED(ipRing->Close()) )
		return false;
	if ( FAILED(ipPolygon->AddGeometry(ipRing)) )
		return false;
	ipSegmentBuf = IGeometryPtr(ipPolygon);
	// �Z�O�����g�Ԃ̐ڑ����̃|���S���쐬
	// ipPreSegBuf(��O�̃Z�O�����g�̃o�b�t�@)�����݂��Ȃ��ꍇ�́A�������s��Ȃ��B
	if ( !ipPreSegBuf ) 
		return true;
	IRelationalOperatorPtr ipRel(ipPreSegBuf);

	// �n�_���獶�ɐL�΂������C����ipPreSegBuf�Ɋ܂܂��ꍇ�́A
	// ��O�̃Z�O�����g�̏I�_����E���̓_(ipOutToRightPt)
	// �����ΏۃZ�O�����g�̎n�_
	// �����ΏۃZ�O�����g�̎n�_����E���̓_(ipFromRightPt) �Őڑ��p�̃|���S�����쐬����B

	// �n�_����E�ɐL�΂������C����ipPreSegBuf�ƌ����ꍇ
	// �����ΏۃZ�O�����g�̎n�_���獶���̓_(ipFromLeftPt)
	// �����ΏۃZ�O�����g�̎n�_
	// ��O�̃Z�O�����g�̏I�_���獶���̓_(ipOutToLeftPt)
		
	bool tgtLeft = false, tgtRight = false;
	VARIANT_BOOL vaLeftPtTouch = VARIANT_FALSE, vaLeftPtContain = VARIANT_FALSE, vaLeftLineCross = VARIANT_FALSE;
	if ( FAILED(ipRel->Touches(ipFromLeftPt, &vaLeftPtTouch)) ||
		 FAILED(ipRel->Contains(ipFromLeftPt, &vaLeftPtContain)) ||
		 FAILED(ipRel->Crosses(ipFromLeftLine, &vaLeftLineCross)) ) {
			_ASSERTE(FALSE);
			return false;
	}
	if  (vaLeftPtTouch || vaLeftPtContain || vaLeftLineCross)
		tgtRight = true;
	VARIANT_BOOL vaRightPtTouch = VARIANT_FALSE, vaRightPtContain = VARIANT_FALSE, vaRightLineCross = VARIANT_FALSE;
	if ( FAILED(ipRel->Touches(ipFromRightPt, &vaRightPtTouch)) ||
			FAILED(ipRel->Contains(ipFromRightPt, &vaRightPtContain)) ||
			FAILED(ipRel->Crosses(ipFromRightLine, &vaRightLineCross)) ) {
			_ASSERTE(FALSE);
			return false;
	}
	if (vaRightPtTouch || vaRightPtContain || vaRightLineCross)
		tgtLeft = true;
	IPointPtr ipPt1, ipPt2, ipPt3;
	// �����Ƃ�TRUE�̏ꍇ�̓^�b�`���Ă���̂ŁA�������Ȃ�
	if ( tgtLeft && tgtRight ) 
		return true;
	// ���Ԃ̃|���S���쐬
	IGeometryCollectionPtr ipPolygon2(CLSID_Polygon);
	if ( FAILED(IGeometryPtr(ipPolygon2)->putref_SpatialReference(ipSpRef)) )
		return false;
	IPointCollectionPtr ipPtCol2(CLSID_Ring);
	if ( FAILED(IGeometryPtr(ipPtCol2)->putref_SpatialReference(ipSpRef)) )
		return false;
	if (!tgtLeft && !tgtRight) {
		// �����Ƃ�FALSE�̏ꍇ�́A���E�Ɍ��Ԃ��󂢂Ă���̂ŏ��X�㒷����
		// �����ΏۃZ�O�����g�̉E���_�E��O�̃Z�O�����g�̉E���̓_
		// ��O�̃Z�O�����g�̍����̓_�E�����ΏۃZ�O�����g�̍����̓_�ŋ�`���쐬����B
		if ( FAILED(ipPtCol2->AddPoint(ipFromRightPt)) ||
			 FAILED(ipPtCol2->AddPoint(ipPreToRightPt)) ||
			 FAILED(ipPtCol2->AddPoint(ipPreToLeftPt)) ||
			 FAILED(ipPtCol2->AddPoint(ipFromLeftPt)) )
			return false;
	} else {
		if ( tgtRight ) {
			ipPt1 = ipPreToRightPt;
			ipPt2 = ipSegFromPt;
			ipPt3 = ipFromRightPt;
		} else {
			ipPt1 = ipFromLeftPt;
			ipPt2 = ipSegFromPt;
			ipPt3 = ipPreToLeftPt;
		}
		// �ڑ����̃|���S���쐬
		if ( FAILED(ipPtCol2->AddPoint(ipPt1)) ||
			 FAILED(ipPtCol2->AddPoint(ipPt2)) ||
			 FAILED(ipPtCol2->AddPoint(ipPt3)) )
				return false;
	}
	IRingPtr ipRing2(ipPtCol2);
	if ( FAILED(ipRing2->Close()) )
		return false;
	if ( FAILED(ipPolygon2->AddGeometry(ipRing2)) )
		return false;
	ipConnectBuf = IGeometryPtr(ipPolygon2);
	return true;
}


/**
 * @brief  �|�����C���`�󂩂�o�b�g��[�E�׃x�������̃o�b�t�@�`����쐬
 * @param  [in]  ipTargetPolyline	�ϊ����|�����C��
 * @param  [in]  dBufDsitance		�o�b�t�@����(�x)
 * @param  [out] ipBuf				�ϊ���`��
 * @retval true  ����
 * @retval false ���s
 */
bool makeBatBevelBuffer( const IPolylinePtr& ipTargetPolyline, double dBufDistance, IGeometryPtr& ipBuf)
{
	ipBuf = NULL;
	// �|�����C�����Z�O�����g�P�ʂ̃|�����C���ɕ�������B
	std::vector<CAdapt<IPolylinePtr>> TargetPolylines = splitPolyline( ipTargetPolyline );
	// ��ԎQ�Ǝ擾
	ISpatialReferencePtr ipSpRef;
	if ( FAILED(ipTargetPolyline->get_SpatialReference(&ipSpRef)) )
		return false;
	// �Z�O�����g���Ƀo�b�g��[�̃o�b�t�@�`��ƁA�n�I�_�ȊO�̃|�C���g�o�b�t�@�`����쐬
	IGeometryCollectionPtr ipGeoColl(CLSID_GeometryBag);
	if ( FAILED(IGeometryPtr(ipGeoColl)->putref_SpatialReference(ipSpRef)) )
		return false;
	long count = TargetPolylines.size();
	std::vector<CAdapt<IGeometryPtr>> bufferLines;
	IGeometryPtr ipPreSegBuf;
	IPointPtr ipPreToLeftPt, ipPreToRightPt;
	for ( long i = 0; i < count; ++i) {
		// �Z�O�����g����o�b�g��[�̃o�b�t�@�쐬
		IGeometryPtr ipSegBuf, ipConnectBuf;
		IPointPtr ipToLeftPt, ipToRightPt;
		if ( ! makeSegmentBatBuf(TargetPolylines[i].m_T, 
			                     ipPreSegBuf, 
								 ipPreToLeftPt, 
								 ipPreToRightPt,
								 dBufDistance,
								 ipSegBuf,
								 ipConnectBuf,
								 ipToLeftPt,
								 ipToRightPt) )
			return false;
		ipPreSegBuf = ipSegBuf;
		ipPreToLeftPt = ipToLeftPt;
		ipPreToRightPt = ipToRightPt;
		bufferLines.push_back(ipSegBuf);
		if ( FAILED(ipGeoColl->AddGeometry(ipSegBuf)) )
			return false;
		if ( ipConnectBuf ) {
			bufferLines.push_back(ipConnectBuf);
			if ( FAILED(ipGeoColl->AddGeometry(ipConnectBuf)) )
				return false;
		}
	}
	switch (count) {
		case 0:
			return false;
		case 1:
			ipBuf = bufferLines[0].m_T;
			break;
		default:
			IGeometryPtr ipPolygon(CLSID_Polygon);
			if ( FAILED(ipPolygon->putref_SpatialReference(ipSpRef)) )
				return false;

			if ( FAILED(((ITopologicalOperatorPtr)ipPolygon)->ConstructUnion(IEnumGeometryPtr(ipGeoColl))) ) {
				_ASSERTE(FALSE);
				return false;
			}
			ipBuf = ipPolygon;
			break;
	}
/* _DEBUG��PGDB�Ńo�b�t�@�`����o�͂������ꍇ�̓R�����g���O��
#ifdef _DEBUG
			for ( const auto buffer : bufferLines ) {
				outputBufferLine(buffer.m_T);
			}
			outputBufferLine(ipBuf);
#endif
*/

	return true;
}

bool AheJudgeLinkRelationBuf( const IPolylinePtr& ipBasePolyline, const IPolylinePtr& ipTargetPolyline, double dSearchMeter, double dRoundOffMeter, double dNoCheckMeter, LinkRelationPosition& eRelationInfo )
{
	// dRoundOffMeter, dNoCheckMeter�̃`�F�b�N(���̑���AheJudgeLinkRelation�Ń`�F�b�N���Ă���̂ł��Ȃ�)
	if( dRoundOffMeter < 0 || dNoCheckMeter < 0)
		return false;

	// ���E����
	double dJudgeMeter = 0.0;
	if( ! AheJudgeLinkRelation(ipBasePolyline, ipTargetPolyline, dSearchMeter, eRelationInfo, dJudgeMeter))
		return false;

	// dRoundOffMeter��0�̏ꍇ
	// ���E���肪����ł��Ȃ��ꍇ�͂��̂܂ܕԂ�
	if ( eRelationInfo == kNotFound || dRoundOffMeter == 0 )
		return true;

	// �|�����C���̋����𒲂ׂāAdNoCheckMeter�����̏ꍇ�͂��̂܂܌��ʂ�Ԃ�
	double dNoCheckDistance = meter2Coordinate(dNoCheckMeter, ipBasePolyline);
	double dBaseDistance = 0.0, dTargetDistance = 0.0;
	if ( FAILED(ipBasePolyline->get_Length(&dBaseDistance)) || FAILED(ipTargetPolyline->get_Length(&dTargetDistance) ))
		return false;
	if ( dBaseDistance < dNoCheckDistance || dTargetDistance < dNoCheckDistance )
		return true;

	// ��ԎQ�Ǝ擾
	ISpatialReferencePtr ipSpRef;
	if ( FAILED(ipBasePolyline->get_SpatialReference(&ipSpRef)) )
		return false;

	// ������N��dSearchMeter�Ńo�b�t�@�����A�Ώۃ����N�Ƃ̋��ʕ������擾
	double dSearchDistance = meter2Coordinate(dSearchMeter, ipBasePolyline);
	IGeometryPtr ipBaseBuf(CLSID_Polygon);
	if ( FAILED(ipBaseBuf->putref_SpatialReference(ipSpRef)) )
		return false;
	if ( ! makeBatBevelBuffer(ipBasePolyline, dSearchDistance, ipBaseBuf) )
		return false;
	IGeometryPtr ipOverlapGeo = AheIntersect( ipBaseBuf, ipTargetPolyline, esriGeometry1Dimension );
	if ( !ipOverlapGeo )
		return false;
	IGeometryCollectionPtr ipGeomCol(ipOverlapGeo);
	long count = 0;
	if ( FAILED(ipGeomCol->get_GeometryCount(&count)) ) 
		return false;
	// ����Ώۂ̃����N�̑I��
	IPolylinePtr ipJudgeLine;
	switch (count) {
	case 0:
		eRelationInfo = kNotFound;
		return true;
	case 1:
		ipJudgeLine = ipOverlapGeo;
		break;
	default:
		if ( !selectJudgeLine(ipGeomCol, ipBasePolyline, eRelationInfo, dSearchMeter, ipJudgeLine) )
			return false;
		break;
	}
	// ����Ώۃ����N�̒�����dRoundOffMeter���Z���ꍇ�́A���Έʒu���u������Ȃ��v�ɕς���B
	double dLapLength = 0.0;
	if ( FAILED(ipJudgeLine->get_Length(&dLapLength)) ) 
		return false;
	double dRoundOffDistance = meter2Coordinate(dRoundOffMeter, ipBasePolyline);
	if ( dLapLength < dRoundOffDistance )
		eRelationInfo = kNotFound;

	return true;
}

/**
 * @brief  �|�����C���̍\���_�Q���擾����
 * @param  [in]  ipPolyline  �|�����C��
 * @return �|�����C���̍\���_�Q
 */
std::vector<CAdapt<IPointPtr>> getPointList( const IPolylinePtr& ipPolyline )
{
	std::vector<CAdapt<IPointPtr>> retPoints;

	IPointCollectionPtr ipPoints(ToMultipoint(ipPolyline));

	long count(0L);
	ipPoints->get_PointCount(&count);
	for( long i = 0; i < count; ++i )
	{
		IPointPtr ipPoint;
		ipPoints->get_Point( i, &ipPoint );

		retPoints.push_back( ipPoint );
	}

	return retPoints;
}


/**
 * @brief  �C�ӂ̃|�C���g����ɁA���̃|�����C����̈�ԋ߂��|�C���g�A�y�у|�C���g�܂ł̋������擾����
 * @param  [in]  ipBasePoint       ����̃|�C���g
 * @param  [in]  ipTargetPolyline  ��ԋ߂��|�C���g����������鑤�̃|�����C��
 * @param  [out] ipNearestPoint    ��ԋ߂��|�C���g
 * @param  [out] distance          ��ԋ߂��|�C���g�܂ł̋���
 * @return true   �擾����
 * @return false  �擾���s 
 */
bool getNearestPointAndDistance(  const IPointPtr& ipBasePoint, const IPolylinePtr& ipTargetPolyline, IPointPtr& ipNearestPoint, double& distance )
{
	// �ŋߖT�_���擾
	if( FAILED((( IProximityOperatorPtr)ipTargetPolyline)->ReturnNearestPoint( ipBasePoint, esriSegmentExtension::esriNoExtension, &ipNearestPoint )))
	{
		_ASSERTE(FALSE);
		return false;
	}

	// �ŋߖT�_�ւ̋������擾
	if( FAILED((( IProximityOperatorPtr)ipTargetPolyline)->ReturnDistance( ipBasePoint, &distance)))
	{
		_ASSERTE(FALSE);
		return false;
	}
	return true;
}


/**
 * @brief  �C�ӂ̃|�����C������ɁA���̃|�����C����̈�ԋ߂��|�C���g���擾����
 * @param  [in]  ipBasePolyline    ����̃|�����C��
 * @param  [in]  ipTargetPolyline  ��ԋ߂��|�C���g����������鑤�̃|�����C��
 * @return NULL�ȊO  ��ԋ߂��|�C���g
 * @return NULL      �擾���s
 */
IPointPtr getNearestPoint( const IPolylinePtr& ipBasePolyline, const IPolylinePtr& ipTargetPolyline )
{
	// �x�[�X���A�^�[�Q�b�g���̃|�����C���̍\���_�Q���擾
	std::vector<CAdapt<IPointPtr>> basePoints   = getPointList( ipBasePolyline );
	std::vector<CAdapt<IPointPtr>> targetPoints = getPointList( ipTargetPolyline );

	double nearest_dist(DBL_MAX);
	IPointPtr ipNearestPoint;

	for( std::vector<CAdapt<IPointPtr>>::const_iterator pt_iter = basePoints.begin();
		    pt_iter != basePoints.end();
		    ++pt_iter )
	{
		IPointPtr ipPoint = *pt_iter;

		// �ŋߖT�_�Ƃ����܂ł̋������擾
		IPointPtr ipTmpNearestPoint;
		double dist(DBL_MAX);
		if( ! getNearestPointAndDistance( ipPoint, ipTargetPolyline, ipTmpNearestPoint, dist))
			return NULL;

		// ���߂��ŋߖT�_���擾
		if( dist < nearest_dist ){
			nearest_dist = dist;
			ipNearestPoint = ipTmpNearestPoint;
		}
	}

	for( std::vector<CAdapt<IPointPtr>>::const_iterator pt_iter = targetPoints.begin();
		    pt_iter != targetPoints.end();
		    ++pt_iter )
	{
		IPointPtr ipPoint = *pt_iter;
		
		// �ŋߖT�_�Ƃ����܂ł̋������擾
		IPointPtr ipTmpNearestPoint;
		double dist(DBL_MAX);
		if( ! getNearestPointAndDistance( ipPoint, ipBasePolyline, ipTmpNearestPoint, dist))
			return NULL;

		// ���߂��^�[�Q�b�g���̍\���_���擾
		if( dist < nearest_dist ){
			nearest_dist = dist;
			ipNearestPoint = ipPoint;
		}
	}

	return ipNearestPoint;
}


/**
 * @brief  �|�����C������Ƃ��A���̃|�����C�����\������Z�O�����g�̓��A��ԋ߂����̂��擾
 * @param  [in]  ipBasePolyline    ����̃|�����C��
 * @param  [in]  ipTargetPolyline  ��ԋ߂��Z�O�����g����������鑤�̃|�����C��
 * @return NULL�ȊO  ��ԋ߂��Z�O�����g(Polyline�^)
 * @return NULL      �擾���s
 */
IPolylinePtr getNearestSegment( const IPolylinePtr& ipBasePolyline, const IPolylinePtr& ipTargetPolyline )
{
	// �^�[�Q�b�g���̃|�����C���̍\���_�Q���擾
	std::vector<CAdapt<IPolylinePtr>> targetPolylines = splitPolyline( ipTargetPolyline );

	// �^�[�Q�b�g���̃|�����C�������ԋ߂��x�[�X���̃|�C���g���擾
	IPointPtr ipNearestBasePoint = getNearestPoint(ipTargetPolyline, ipBasePolyline);

	// �x�[�X���̈�ԋ߂��|�C���g(ipNearestBasePoint)����ɁA
	// �^�[�Q�b�g���̊e�Z�O�����g�ւ̋����𒲂׈�ԋ߂��Z�O�����g���擾����
	double nearest_dist(DBL_MAX);
	IPolylinePtr ipNearestPolyline;
	for( std::vector<CAdapt<IPolylinePtr>>::const_iterator line_iter = targetPolylines.begin();
		    line_iter != targetPolylines.end();
		    ++line_iter )
	{
		IPolylinePtr ipPolyline = *line_iter;

		// �������擾
		double dist(DBL_MAX);
		if( FAILED((( IProximityOperatorPtr)ipPolyline)->ReturnDistance( ipNearestBasePoint, &dist)))
		{
			_ASSERTE(FALSE);
			return NULL;
		}
		// ���߂��Z�O�����g���擾
		if( dist < nearest_dist ){
			nearest_dist = dist;
			ipNearestPolyline = ipPolyline;
		}
	}

	return ipNearestPolyline;
}


 // �^�[�Q�b�g�̃|�����C������x�[�X�̃|�����C���֌������Ĉʒu�֌W���擾����
bool AheSimpleJudgeLinkRelation( const IPolylinePtr& ipBasePolyline, const IPolylinePtr& ipTargetPolyline, unsigned int& nRelationInfo )
{
	if( ! ipBasePolyline || ! ipTargetPolyline )
		return false;

	std::vector<CAdapt<IPolylinePtr>> basePolylines  = splitPolyline( ipBasePolyline );

	// �x�[�X���̃|�����C�����\������e�Z�O�����g���猩�āA
	// �ł��߂��^�[�Q�b�g���̍\���_�����E�ǂ���ɂ��邩��1�����肷��
	long lLeftCnt(0L), lRightCnt(0L);
	for( std::vector<CAdapt<IPolylinePtr>>::const_iterator line_iter = basePolylines.begin();
		 line_iter != basePolylines.end();
		 ++line_iter )
	{
		// �ł��߂��^�[�Q�b�g���̃|�C���g���擾
		IPointPtr ipNearestPoint = getNearestPoint( *line_iter, ipTargetPolyline );
		if( ! ipNearestPoint )
			return false;

		// �Z�O�����g����ł��߂��^�[�Q�b�g���̍\���_�ɑ΂��č��E���肷��
		if( getPointPosition( *line_iter, ipNearestPoint ) == kLeft )
			++lLeftCnt;
		else
			++lRightCnt;
	}
	
	// �x�[�X���̊e�Z�O�����g����̍��E������W�v���čŏI���肷��
	if( lLeftCnt != lRightCnt ) {
		nRelationInfo = ( lLeftCnt > lRightCnt ? kLeft : kRight );
		return true;
	}


	// ���E����̌��_���o�Ȃ��ꍇ(���茋�ʂ̍��E�̐��������̏ꍇ)
	// �^�[�Q�b�g���ɍł��߂��x�[�X���̃Z�O�����g���瓯�l�̍��E������s��
	IPolylinePtr ipNearestPolyline = getNearestSegment(ipTargetPolyline, ipBasePolyline);
	if( ! ipNearestPolyline )
		return false;

	IPointPtr ipNearestPoint = getNearestPoint( ipNearestPolyline, ipTargetPolyline );
	if( ! ipNearestPoint )
		return false;

	nRelationInfo = getPointPosition( ipNearestPolyline, ipNearestPoint );

	return true;
}

}	// sindy



