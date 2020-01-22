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

#include "stdafx.h"

/**
 * @brief   �ܓx�o�x�N���X
 */
class cLonLat
{
public:

	/**
	 * @brief   �R���X�g���N�^
	 */
	cLonLat() : m_lon(0.0), m_lat(0.0) {};

	/**
	 * @brief   �R���X�g���N�^
	 * @param   ipPoint [in]  �|�C���g
	 */
	cLonLat( const IPointPtr& ipPoint ){
		init( ipPoint );
	};

	/**
	 * @brief   �R���X�g���N�^
	 * @param   ipGeom  [in]  �W�I���g��
	 */
	cLonLat( const IGeometryPtr& ipGeom ){
		SetLonLat( ipGeom );
	};

	/**
	 * @brief   �f�X�g���N�^
	 */
	~cLonLat(){};

	/**
	 * @brief   �W�I���g���Z�b�g
	 * @note    ���C����|���S���̏ꍇ�́A�����ɋ��߂Ă��Ȃ�
	 */
	void SetLonLat( const IGeometryPtr& ipGeom ){
		esriGeometryType eType =  ipGeom->GetGeometryType();
		switch( eType ){
		case esriGeometryPoint:
			init( (IPointPtr)ipGeom );
			break;
		case esriGeometryPolyline:
			init( ((IPointCollectionPtr)ipGeom)->GetPoint( ((IPointCollectionPtr)ipGeom)->GetPointCount() / 2 ) );
			break;
		case esriGeometryPolygon:
			init( (IPointPtr)(IAreaPtr(IGeometryPtr(ipGeom->GetEnvelope()))->GetCentroid()) );
			break;
		default:
			init();
			break;
		}
	};

	/**
	 * @brief   �ܓx�擾
	 * @retval  double   �ܓx
	 */
	double GetLat() const { return m_lat; };

	/**
	 * @brief   �o�x�擾
	 * @retval  double   �o�x
	 */
	double GetLon() const { return m_lon; };

private:

	/**
	 * @brief   ������
	 */
	void init(){ m_lon = 0.0; m_lat = 0.0; };

	/**
	 * @brief   �|�C���g������W�l�Z�b�g
	 * @param   ipPoint [in]  �|�C���g
	 */
	void init( const IPointPtr& ipPoint ){
		init();
		if( ipPoint )
			ipPoint->QueryCoords( &m_lon, &m_lat );
	};

private:

	double m_lon; //!< �o�x
	double m_lat; //!< �ܓx
};

