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
 *	GDBSpatialSearch.h
 *	@author	Fusatoshi Abe
 *	@date	2003/09/26		�V�K�쐬
 */

#ifndef	___GDB_SPATIAL_SEARCH_H___
#define	___GDB_SPATIAL_SEARCH_H___

#include <string>

extern HRESULT GDBCreateSpatialFilter(IFeatureClassPtr ipFeatureClass, IGeometryPtr ipGeometry, esriSpatialRelEnum cSpatRel, ISpatialFilter **ipSpatialFilter);

/**
 * @brief �Ώۃt�B�[�`���N���X��WHERE��ŁA�����������A�J�[�\�����擾����
 * @param ipFeatureClass [in] �Ώۃt�B�[�`���N���X
 * @param ipGeometry     [in] ��Ԍ����Ŏg�p����W�I���g��
 * @param cSpatRel       [in] SpatialReference
 * @param ipFeatureCursor [out] �擾�J�[�\��
 * @param strSubFields   [in] SubFields
 * @retval S_OK �擾����
 */
extern HRESULT GDBSearchByGeometry(IFeatureClassPtr ipFeatureClass, IGeometryPtr ipGeometry, esriSpatialRelEnum cSpatRel, IFeatureCursor **ipFeatureCursor, const std::string& strSubFields = "");
//extern HRESULT GDBSelectByGeometry(IFeatureClassPtr ipFeatureClass, IGeometryPtr ipGeometry, esriSpatialRelEnum cSpatRel, ISelectionSet **ipSelectionSet);
extern HRESULT GDBCountByGeometry(IFeatureClassPtr ipFeatureClass, IGeometryPtr ipGeometry, esriSpatialRelEnum cSpatRel, long *cCount);

extern HRESULT GDBSearchByGeometryAndWhereClause(IFeatureClassPtr ipFeatureClass, IGeometryPtr ipGeometry, esriSpatialRelEnum cSpatRel, const std::string &cWhereClause, esriSearchOrder cSearchOrder, IFeatureCursor **ipFeatureCursor);
//extern HRESULT GDBSelectByGeometryAndWhereClause(IFeatureClassPtr ipFeatureClass, IGeometryPtr ipGeometry, esriSpatialRelEnum cSpatRel, const std::string &cWhereClause, esriSearchOrder cSearchOrder, ISelectionSet **ipSelectionSet);
extern HRESULT GDBCountByGeometryAndWhereClause(IFeatureClassPtr ipFeatureClass, IGeometryPtr ipGeometry, esriSpatialRelEnum cSpatRel, const std::string &cWhereClause, esriSearchOrder cSearchOrder, long *cCount);

#endif	//___GDB_SPATIAL_SEARCH_H___
