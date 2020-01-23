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
 * @file FeatureClass.h
 * @brief CFeatureClass�N���X��`
 * $Id$
 */
#ifndef _FEATURECLASS_H_
#define _FEATURECLASS_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Table.h"

namespace sindy {
/**
 * @class CFeatureClass
 * @brief IFeatureClass���b�p�[�N���X
 * @author �n�}�c�a���암�J���O���[�v �Ð�M�G
 */
class SINDYLIB_API CFeatureClass : public CTable
{
public:
	explicit CFeatureClass(){}
	virtual ~CFeatureClass(){}

	CFeatureClass( sindyTableType::ECode emTableType, ITable* lp ){ SetObject( emTableType, lp ); }
	CFeatureClass( sindyTableType::ECode emTableType, IFeatureClass* lp ){ SetObject( emTableType, lp ); }
	CFeatureClass( ITable* lp ){ SetObject( lp ); }
	CFeatureClass( IFeatureClass* lp ){	SetObject( lp ); }

	//@{ @name IObjectClass�C���^�[�t�F�[�X
	LPCTSTR GetAliasName( LPTSTR lpszAliasName, int nLen ) const;
	long GetObjectClassID() const;
	//@}
	//@{ @name IFeatureClass�C���^�[�t�F�[�X
	IFeaturePtr CreateFeature();
	long FeatureCount( IQueryFilter* Filter ) const;
	IFieldPtr GetAreaField() const;
	long GetFeatureClassID() const;
	IFeatureDatasetPtr GetFeatureDataset() const;
	esriFeatureType GetFeatureType() const;
	IFieldPtr GetLengthField() const;
	LPCTSTR GetShapeFieldName( LPTSTR lpszShapeFieldName, int nLen ) const;
	esriGeometryType GetShapeType() const;
	IFeaturePtr GetFeature( long OID ) const;
	IFeatureCursorPtr GetFeatures( const VARIANT& fids, bool Recycling ) const;
	//@}
	//@{ @name �w���p�[�֐�
	/**
	 * @brief �t�B�[�`���N���X�Ŏg�p����Ă����ԎQ�Ƃ��擾����
	 * return ISpatialReferencePtr
	 */
	ISpatialReferencePtr GetSpatialReference() const;

	/**
	 * @brief <b>OID���X�g�Ō�������</b>\n
	 * @param fids		[in]	OID���X�g
	 * @param Recycling	[in]	NextFeature�ł�IFeature�����T�C�N������ꍇ��true
	 * @return IFeatureCursorPtr
	 */
	IFeatureCursorPtr GetFeatures( const std::list<long>& fids, bool Recycling ) const;
	//@}
};

} // sindy

#endif // _FEATURECLASS_H_
