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
// 定義
/////////////////////////////////////////////////////////////////////////////

/**
 * @brief メッシュ形状を返す関数クラス。
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
 * @brief ２次メッシュ形状を返す関数クラス。
 */
class base_mesh_shape_loader : public mesh_shape_loader_base
{
public:
	base_mesh_shape_loader(IFeatureWorkspace* ipFeatureWorkspace);
	base_mesh_shape_loader(IFeatureClass* ipBaseMeshClass);
};

/**
 * @brief 都市地図メッシュ形状を返す関数クラス。
 */
class city_mesh_shape_loader : public mesh_shape_loader_base
{
public:
	city_mesh_shape_loader(IFeatureWorkspace* ipFeatureWorkspace);
	city_mesh_shape_loader(IFeatureClass* ipCityMeshClass);
};

/**
 * @brief 引数にあわせて２次／都市地図メッシュ形状を返す関数クラス。
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
 * @param ipFeatureClass [in] フィーチャクラス。
 * @param lpszMeshCodeFieldName [in] メッシュコード属性フィールド名。null不許可。
 *
 * @attention
 * @p ipFeatureClass をnullにすると operator()() が例外を起こす。
 */
inline mesh_shape_loader_base::mesh_shape_loader_base(IFeatureClass* ipFeatureClass, LPCTSTR lpszMeshCodeFieldName) :
m_ipFeatureClass(ipFeatureClass),
m_strMeshCodeFieldName(lpszMeshCodeFieldName)
{
}

/**
 * @brief メッシュ形状を求める。
 *
 * @param nMeshCode [in] メッシュコード。
 * @return メッシュ形状。該当するメッシュが存在しない場合null。
 */
inline IGeometryPtr mesh_shape_loader_base::operator()(long nMeshCode) const
{
	CComBSTR bstrShapeFieldName;
	m_ipFeatureClass->get_ShapeFieldName(&bstrShapeFieldName);

	IQueryFilterPtr ipQueryFilter(CLSID_QueryFilter);
	ipQueryFilter->put_SubFields(bstrShapeFieldName);

	// "MESHCODE=〈メッシュコード〉"
	rns::tostringstream oss;
	std::locale loc(std::locale("japanese"), "C", std::locale::numeric);
	oss.imbue(loc);
	oss << m_strMeshCodeFieldName << '=' << nMeshCode;
	atl2::valid(ipQueryFilter->put_WhereClause(CComBSTR(oss.str().c_str())));

	// カーソルから唯１つの形状を取り出す。
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
 * @param ipFeatureWorkspace [in] <b>Base_Mesh</b>フィーチャクラスを含むかもしれないフィーチャワークスペース。null不許可。
 */
inline base_mesh_shape_loader::base_mesh_shape_loader(IFeatureWorkspace* ipFeatureWorkspace) :
mesh_shape_loader_base(open_ref_featureclass(ipFeatureWorkspace, basemesh::kTableName), basemesh::kMeshCode)
{
}

/**
 * @param ipBaseMeshClass [in] <b>Base_Mesh</b>フィーチャクラス。
 */
inline base_mesh_shape_loader::base_mesh_shape_loader(IFeatureClass* ipBaseMeshClass) :
mesh_shape_loader_base(ipBaseMeshClass, basemesh::kMeshCode)
{
}

/////////////////////////////////////////////////////////////////////////////
// city_mesh_shape_loader
/////////////////////////////////////////////////////////////////////////////

/**
 * @param ipFeatureWorkspace [in] <b>CityMesh</b>フィーチャクラスを含むかもしれないフィーチャワークスペース。null不許可。
 */
inline city_mesh_shape_loader::city_mesh_shape_loader(IFeatureWorkspace* ipFeatureWorkspace) :
mesh_shape_loader_base(open_ref_featureclass(ipFeatureWorkspace, citymesh::kTableName), citymesh::kCityMeshCode)
{
}

/**
 * @param ipCityMeshClass [in] <b>CityMesh</b>フィーチャクラス。
 */
inline city_mesh_shape_loader::city_mesh_shape_loader(IFeatureClass* ipCityMeshClass) :
mesh_shape_loader_base(ipCityMeshClass, citymesh::kCityMeshCode)
{
}

/////////////////////////////////////////////////////////////////////////////
// multi_mesh_shape_loader
/////////////////////////////////////////////////////////////////////////////

/**
 * @param ipFeatureWorkspace [in] フィーチャワークスペース。null不許可。
 */
inline multi_mesh_shape_loader::multi_mesh_shape_loader(IFeatureWorkspace* ipFeatureWorkspace) :
m_BaseLoader(ipFeatureWorkspace),
m_CityLoader(ipFeatureWorkspace)
{
}

/**
 * @param ipBaseMeshClass [in] <b>Base_Mesh</b>フィーチャクラス。
 * @param ipCityMeshClass [in] <b>CityMesh</b>フィーチャクラス。
 */
inline multi_mesh_shape_loader::multi_mesh_shape_loader(IFeatureClass* ipBaseMeshClass, IFeatureClass* ipCityMeshClass) :
m_BaseLoader(ipBaseMeshClass),
m_CityLoader(ipCityMeshClass)
{
}

/**
 * @brief メッシュ形状を求める。
 *
 * メッシュコードが1～6桁の数なら２次メッシュ、それ以外なら都市地図メッシュと判定。
 *
 * @param nMeshCode [in] メッシュコード。
 * @return メッシュ形状。該当するメッシュが存在しない場合null。
 * @exception _com_error メッシュコードに対応するメッシュフィーチャクラスを開けなかったため、検索できなかったのかもしれない。
 */
inline IGeometryPtr multi_mesh_shape_loader::operator()(long nMeshCode) const
{
	return nMeshCode <= 999999 ? m_BaseLoader(nMeshCode) : m_CityLoader(nMeshCode);
}

} // namespace sindy

#endif // SINDY_MESH_SHAPE_LOADER_H_
