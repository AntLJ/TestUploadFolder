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

#ifndef SINDY_MESH_SHAPE_LOADER_H_
#define SINDY_MESH_SHAPE_LOADER_H_

#include <rns/string.h>
#include <rns/sstream.h>
#include <sindy/schema.h>
#include <arclib/sindy/geodatabase.h>

using namespace sindy::schema;
namespace sindy {

/////////////////////////////////////////////////////////////////////////////
// ��`
/////////////////////////////////////////////////////////////////////////////

/**
 * @brief ���b�V���`���Ԃ��֐��N���X�B
 */
class mesh_shape_loader_base : public std::unary_function<long, IGeometryPtr>
{
public:
	mesh_shape_loader_base(IFeatureClass* ipFeatureClass, LPCTSTR lpszMeshCodeFieldName);
	
	IGeometryPtr operator()(long nMeshCode) const;

private:
	IFeatureClassPtr m_ipFeatureClass;
	rns::tstring m_strMeshCodeFieldName;
};

/**
 * @brief �Q�����b�V���`���Ԃ��֐��N���X�B
 */
class base_mesh_shape_loader : public mesh_shape_loader_base
{
public:
	base_mesh_shape_loader(IFeatureWorkspace* ipFeatureWorkspace);
	base_mesh_shape_loader(IFeatureClass* ipBaseMeshClass);
};

/**
 * @brief �s�s�n�}���b�V���`���Ԃ��֐��N���X�B
 */
class city_mesh_shape_loader : public mesh_shape_loader_base
{
public:
	city_mesh_shape_loader(IFeatureWorkspace* ipFeatureWorkspace);
	city_mesh_shape_loader(IFeatureClass* ipCityMeshClass);
};

/**
 * @brief �����ɂ��킹�ĂQ���^�s�s�n�}���b�V���`���Ԃ��֐��N���X�B
 */
class multi_mesh_shape_loader : public std::unary_function<long, IGeometryPtr>
{
public:
	multi_mesh_shape_loader(IFeatureWorkspace* ipFeatureWorkspace);
	multi_mesh_shape_loader(IFeatureClass* ipBaseMeshClass, IFeatureClass* ipCityMeshClass);

	IGeometryPtr operator()(long nMeshCode) const;

private:
	base_mesh_shape_loader m_BaseLoader;
	city_mesh_shape_loader m_CityLoader;
};

/////////////////////////////////////////////////////////////////////////////
// mesh_shape_loader_base
/////////////////////////////////////////////////////////////////////////////

/**
 * @param ipFeatureClass [in] �t�B�[�`���N���X�B
 * @param lpszMeshCodeFieldName [in] ���b�V���R�[�h�����t�B�[���h���Bnull�s���B
 *
 * @attention
 * @p ipFeatureClass ��null�ɂ���� operator()() ����O���N�����B
 */
inline mesh_shape_loader_base::mesh_shape_loader_base(IFeatureClass* ipFeatureClass, LPCTSTR lpszMeshCodeFieldName) :
m_ipFeatureClass(ipFeatureClass),
m_strMeshCodeFieldName(lpszMeshCodeFieldName)
{
}

/**
 * @brief ���b�V���`������߂�B
 *
 * @param nMeshCode [in] ���b�V���R�[�h�B
 * @return ���b�V���`��B�Y�����郁�b�V�������݂��Ȃ��ꍇnull�B
 */
inline IGeometryPtr mesh_shape_loader_base::operator()(long nMeshCode) const
{
	CComBSTR bstrShapeFieldName;
	m_ipFeatureClass->get_ShapeFieldName(&bstrShapeFieldName);

	IQueryFilterPtr ipQueryFilter(CLSID_QueryFilter);
	ipQueryFilter->put_SubFields(bstrShapeFieldName);

	// "MESHCODE=�q���b�V���R�[�h�r"
	rns::tostringstream oss;
	std::locale loc(std::locale("japanese"), "C", std::locale::numeric);
	oss.imbue(loc);
	oss << m_strMeshCodeFieldName << '=' << nMeshCode;
	atl2::valid(ipQueryFilter->put_WhereClause(CComBSTR(oss.str().c_str())));

	// �J�[�\������B�P�̌`������o���B
	IFeatureCursorPtr ipFeatureCursor;
	atl2::valid(m_ipFeatureClass->Search(ipQueryFilter, VARIANT_TRUE, &ipFeatureCursor));

	IFeaturePtr ipFeature;
	if(ipFeatureCursor->NextFeature(&ipFeature) != S_OK)
		return 0;

	IGeometryPtr ipMeshShape;
	atl2::valid(ipFeature->get_ShapeCopy(&ipMeshShape));

	return ipFeatureCursor->NextFeature(&ipFeature) != S_OK ? ipMeshShape : 0;
}

/////////////////////////////////////////////////////////////////////////////
// base_mesh_shape_loader
/////////////////////////////////////////////////////////////////////////////

/**
 * @param ipFeatureWorkspace [in] <b>Base_Mesh</b>�t�B�[�`���N���X���܂ނ�������Ȃ��t�B�[�`�����[�N�X�y�[�X�Bnull�s���B
 */
inline base_mesh_shape_loader::base_mesh_shape_loader(IFeatureWorkspace* ipFeatureWorkspace) :
mesh_shape_loader_base(open_ref_featureclass(ipFeatureWorkspace, basemesh::kTableName), basemesh::kMeshCode)
{
}

/**
 * @param ipBaseMeshClass [in] <b>Base_Mesh</b>�t�B�[�`���N���X�B
 */
inline base_mesh_shape_loader::base_mesh_shape_loader(IFeatureClass* ipBaseMeshClass) :
mesh_shape_loader_base(ipBaseMeshClass, basemesh::kMeshCode)
{
}

/////////////////////////////////////////////////////////////////////////////
// city_mesh_shape_loader
/////////////////////////////////////////////////////////////////////////////

/**
 * @param ipFeatureWorkspace [in] <b>CityMesh</b>�t�B�[�`���N���X���܂ނ�������Ȃ��t�B�[�`�����[�N�X�y�[�X�Bnull�s���B
 */
inline city_mesh_shape_loader::city_mesh_shape_loader(IFeatureWorkspace* ipFeatureWorkspace) :
mesh_shape_loader_base(open_ref_featureclass(ipFeatureWorkspace, citymesh::kTableName), citymesh::kCityMeshCode)
{
}

/**
 * @param ipCityMeshClass [in] <b>CityMesh</b>�t�B�[�`���N���X�B
 */
inline city_mesh_shape_loader::city_mesh_shape_loader(IFeatureClass* ipCityMeshClass) :
mesh_shape_loader_base(ipCityMeshClass, citymesh::kCityMeshCode)
{
}

/////////////////////////////////////////////////////////////////////////////
// multi_mesh_shape_loader
/////////////////////////////////////////////////////////////////////////////

/**
 * @param ipFeatureWorkspace [in] �t�B�[�`�����[�N�X�y�[�X�Bnull�s���B
 */
inline multi_mesh_shape_loader::multi_mesh_shape_loader(IFeatureWorkspace* ipFeatureWorkspace) :
m_BaseLoader(ipFeatureWorkspace),
m_CityLoader(ipFeatureWorkspace)
{
}

/**
 * @param ipBaseMeshClass [in] <b>Base_Mesh</b>�t�B�[�`���N���X�B
 * @param ipCityMeshClass [in] <b>CityMesh</b>�t�B�[�`���N���X�B
 */
inline multi_mesh_shape_loader::multi_mesh_shape_loader(IFeatureClass* ipBaseMeshClass, IFeatureClass* ipCityMeshClass) :
m_BaseLoader(ipBaseMeshClass),
m_CityLoader(ipCityMeshClass)
{
}

/**
 * @brief ���b�V���`������߂�B
 *
 * ���b�V���R�[�h��1�`6���̐��Ȃ�Q�����b�V���A����ȊO�Ȃ�s�s�n�}���b�V���Ɣ���B
 *
 * @param nMeshCode [in] ���b�V���R�[�h�B
 * @return ���b�V���`��B�Y�����郁�b�V�������݂��Ȃ��ꍇnull�B
 * @exception _com_error ���b�V���R�[�h�ɑΉ����郁�b�V���t�B�[�`���N���X���J���Ȃ��������߁A�����ł��Ȃ������̂�������Ȃ��B
 */
inline IGeometryPtr multi_mesh_shape_loader::operator()(long nMeshCode) const
{
	return nMeshCode <= 999999 ? m_BaseLoader(nMeshCode) : m_CityLoader(nMeshCode);
}

} // namespace sindy

#endif // SINDY_MESH_SHAPE_LOADER_H_
