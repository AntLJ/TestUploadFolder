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
#include "CheckBuildNamePoint.h"

#include <sindy/workspace.h>
#include <ArcHelperEx/GlobalFunctions.h>
#include <TDC/useful_headers/str_util.h>
#include <TDC/common_check/NameYomiChk.h>
#include "lonlat.h"
#include "NameChk.h"

using namespace std;
using namespace uh;
using namespace sindy;
using namespace sindy::schema::sj::buildingname_point;

namespace
{
	crd_cnv g_crd;
	using namespace sindy::schema::sj;

	// �����Œ�`�����t�B�[���h�����`�F�b�N�ł��Ȃ��iSUB FIELD�ōi����j�̂Œ���

	const vector<CString> blNameFields = boost::assign::list_of
		( buildingname_point::kObjectID )
		( buildingname_point::kShape )
		( buildingname_point::kBuildingOID )
		( buildingname_point::kName )
		( buildingname_point::kPriority )
		( buildingname_point::kMatchPer )
		( buildingname_point::kYomiSeparated )
		( buildingname_point::kNameSeparated )
		( buildingname_point::kBldgClass1 )
		( buildingname_point::kBldgClass2 )
		( buildingname_point::kBldgClass3 )
		( buildingname_point::kFloors )
		( buildingname_point::kFixClass );

	using namespace sindy::schema;
	const vector<CString> buildingFields = boost::assign::list_of
		(building::kObjectID)
		(ipc_feature::kShape)
		(building::kBldClass);

	const vector<CString> cAnnoFields = boost::assign::list_of
		(city_annotation::kObjectID)
		(ipc_feature::kShape)
		(city_annotation::kNameString1);


	const CString nStr = _T("");

	const int nameMaxLength = 80;
}

// ������
bool CCheckBuildNamePoint::init()
{
	// �e�탌�C���I�[�v��
	if( ! OpenLayers( m_strBlNameDB, m_strBuildingDB, m_strAnnoDB, m_strMeshDB ) )
		return false;

	// ���b�V�����X�g�I�[�v��
	if( ! util::loadLongSet( m_strMeshList, m_meshList ) ){
		m_log.OutputStdErr( err_def::OpenMeshFail, m_strMeshList );
		return false;
	}

	// NG������ʃ��X�g�I�[�v��
	if( ! util::loadLongSet( m_strNgBuild, m_ngBuildClass ) ){
		m_log.OutputStdErr( err_def::OpenNgBlCFail, m_strNgBuild );
		return false;
	}

	// NG�����񃊃X�g�I�[�v��
	if( ! util::loadStringList( m_strErrWord, m_errWordVec ) ){
		m_log.OutputStdErr( err_def::OpenNgWordFail, m_strErrWord );
		return false;
	}
	if( ! util::loadStringList( m_strWarnWord, m_warnWordVec ) ){
		m_log.OutputStdErr( err_def::OpenNgWordFail, m_strWarnWord );
		return false;
	}

	// �������r�p�u�����X�g�I�[�v��
	if( ! util::loadPairVec( m_strReplaceWord, m_replaceWordVec ) ){
		m_log.OutputStdErr( err_def::OpenReplaceFail, m_strReplaceWord );
		return false;
	}

	// ���L��r�p���������񃊃X�g�I�[�v��
	if( ! util::loadVecMap( m_strJoinAnno, m_joinAnnoMap ) ){
		m_log.OutputStdErr( err_def::OpenJoinAnnoFail, m_strJoinAnno );
		return false;
	}

	// ������ʕ�OK���[�h���X�g�I�[�v��
	if( ! util::loadStrKeyMap( m_strBldcOKWord, m_bldcOKWordMap ) ){
		m_log.OutputStdErr( err_def::OpenBldcOKWordFail, m_strBldcOKWord );
		return false;
	}

	// ������ʕ�NG���[�h���X�g�I�[�v��
	if( ! util::loadVecMap( m_strBldcNGWord, m_bldcNGWordMap ) ){
		m_log.OutputStdErr( err_def::OpenBldcNGWordFail, m_strBldcNGWord );
		return false;
	}
	// �������`�F�b�N�����O�L�����X�g�I�[�v��
	if (!m_strExclude_mark.IsEmpty() && !util::loadStringSet(m_strExclude_mark, m_ExcludeMarkVec)) {
		m_log.OutputStdErr(err_def::OpenExcludMarkFail, m_strExclude_mark);
		return false;
	}

	// ���O�t�@�C���I�[�v��
	if( ! m_log.Open( m_strOutput ) ){
		m_log.OutputStdErr( err_def::OpenLogFail, m_strOutput );
		return false;
	}
	return true;
}

// �^�������b�V���א�9���b�V�����X�g���쐬����
void CCheckBuildNamePoint::Get9Mesh( long mesh, std::set<long>& meshlist )
{
	meshlist.insert( mesh );                              // ���S
	meshlist.insert( g_crd.GetMeshCode( mesh, -1,  1 ) ); // ����
	meshlist.insert( g_crd.GetMeshCode( mesh,  0,  1 ) ); // ��
	meshlist.insert( g_crd.GetMeshCode( mesh,  1,  1 ) ); // �E��
	meshlist.insert( g_crd.GetMeshCode( mesh, -1,  0 ) ); // ��
	meshlist.insert( g_crd.GetMeshCode( mesh,  1,  0 ) ); // �E
	meshlist.insert( g_crd.GetMeshCode( mesh, -1, -1 ) ); // ����
	meshlist.insert( g_crd.GetMeshCode( mesh,  0, -1 ) ); // ��
	meshlist.insert( g_crd.GetMeshCode( mesh,  1, -1 ) ); // �E��
};

// ���b�V�����X�g����A�����������b�V���|���S�����擾
bool CCheckBuildNamePoint::GetMeshPolygons( long mesh, const set<long>& meshlist, IGeometryPtr& ipMeshGeo, IGeometryPtr& ipUnionMeshGeo )
{
	ISpatialReferencePtr ipSpRef;
	CString strWhere;
	strWhere.Format( _T("%s in ( "), basemesh::kMeshCode );
	int i = 0;
	for( auto& mesh : meshlist ){
		if( i != 0 )
			strWhere.AppendFormat( _T(", ") );
		strWhere.AppendFormat( _T("%ld"), mesh );
		++i;
	}
	strWhere.AppendFormat( _T(" )") );

	IFeatureCursorPtr ipMeshCursor = m_ipCityMeshFC->_Search( AheInitQueryFilter( NULL, NULL, strWhere ), VARIANT_FALSE );
	if( !ipMeshCursor )
		return false;

	i = 0;
	IFeaturePtr ipMeshFeature;
	IGeometryCollectionPtr ipGeoCol( CLSID_GeometryBag );
	while( S_OK == ipMeshCursor->NextFeature( &ipMeshFeature ) && ipMeshFeature ){

		IGeometryPtr ipTempMeshGeo = ipMeshFeature->GetShapeCopy();
		if( !ipTempMeshGeo )
			return false;

		// ��ԎQ�Ƃ��擾���Ă���
		if( i == 0 ){
			ipTempMeshGeo->get_SpatialReference( &ipSpRef );
			((IGeometryPtr)ipGeoCol)->putref_SpatialReference( ipSpRef );
		}

		// ���g�̃��b�V�����擾���Ă���
		if( mesh == (ipMeshFeature->GetValue( m_cityMeshFM.GetFieldIndex( basemesh::kMeshCode ) )).lVal ){
			ipMeshGeo = ipTempMeshGeo;

			// ���b�V���̒��S���W���瓊�e�����擾����
			WKSPoint meshCenter = {};
			g_crd.MeshtoLL( mesh, 500000, 500000, &(meshCenter.X), &(meshCenter.Y));
			int sysNum = g_crd.Sys_Number(meshCenter.X, meshCenter.Y);

			// ���e���W�n�̏���ݒ肷��
			// sysNum��1�`19��19���W�n�̔ԍ����Ӗ�����B
			// ���e���̔ԍ���esriSRProjCS_TokyoJapan1�`19�܂ł���̂ŁA
			// ��1�n�̒l��sysNum-1�𑫂��Γ��e���̃R�[�h���擾�ł���
			// 19���W�n�̃R�[�h���A�����Ă���O��Ɋ�Â����A���̒l��EPSG�R�[�h�B
			// �����ESRI����`�����l�ł͂Ȃ����A���������ς����̂ł��Ȃ��̂Ŗ��Ȃ����낤(���蓖�Ă��̂�ESRI�����ǁc)
			ISpatialReferenceFactory2Ptr ipSpRefFct(CLSID_SpatialReferenceEnvironment);
			ipSpRefFct->CreateProjectedCoordinateSystem(esriSRProjCS_TokyoJapan1 + sysNum -1, &m_ipPrjCoordinateSystem);
		}

		// ��ňꊇ��ConstructUnion���邽�߂ɁA�W�I���g�����i�[����
		ipGeoCol->AddGeometry( ipTempMeshGeo );

		++i;
	}

	if( ! ipMeshGeo )
		return false;

	// �l�߂����̂�ConstructUnion
	IGeometryPtr ipTempUnionMeshGeo( CLSID_Polygon );
	IEnumGeometryPtr ipEnumGeo( ipGeoCol );
	if( FAILED(((ITopologicalOperatorPtr)ipTempUnionMeshGeo)->ConstructUnion( ipEnumGeo ) ))
		return false;

	ipTempUnionMeshGeo->putref_SpatialReference( ipSpRef );
	((ITopologicalOperator2Ptr)ipTempUnionMeshGeo)->put_IsKnownSimple( VARIANT_FALSE );
	((ITopologicalOperator2Ptr)ipTempUnionMeshGeo)->Simplify();

	ipUnionMeshGeo = ipTempUnionMeshGeo;

	return true;
}

// �^����ꂽ���b�V���`����̃I�u�W�F�N�g���擾
bool CCheckBuildNamePoint::GetObjInMesh( const IGeometryPtr& ipMeshGeo, const vector<CString>& fields, const IFeatureClassPtr& ipFeatureClass, const CFieldMap& fieldMap, map<long, GeoClass>& geoMap )
{
	// �T�u�t�B�[���h�w��
	CString subField = str_util::join( fields, _T(",") );

	// �W�I���g����ێ����邽�߁AVARIANT_FALSE
	IFeatureCursorPtr ipCursor = ipFeatureClass->_Search( AheInitQueryFilter( AheInitSpatialFilter( NULL, ipMeshGeo ), subField, NULL ), VARIANT_FALSE );
	if( !ipCursor )
		return false;

	IFeaturePtr ipFeature;

	// �擾�����I�u�W�F�N�g�̃��[�v
	while( S_OK == ipCursor->NextFeature( &ipFeature ) && ipFeature ){

		// ObjectID�ƌ`��擾
		GeoClass obj;

		long oID     = ipFeature->GetOID();
		if( ! GetFieldValues( ipFeature, fields, fieldMap, obj ) )
			return false;

		// ObjectID���L�[�ɁAmap�Ɋi�[
		geoMap[oID]  = obj;
	}

	return true;
}

// �t�B�[���h�����̒l���擾���A�I�u�W�F�N�g�R���e�i�ɋl�߂�
bool CCheckBuildNamePoint::GetFieldValues( const IFeaturePtr& ipFeature, const vector<CString>& fields, const CFieldMap& fieldMap, GeoClass& obj )
{
	for( const auto& field : fields ){
		if( 0 == field.CompareNoCase( sindy::schema::ipc_feature::kShape ) )
			obj.SetGeometry( ipFeature->GetShape() );
		else
			obj.SetFieldValue( field, str_util::ToString( ipFeature->GetValue( fieldMap.GetFieldIndex( field ) )));
	}
	return true;
}

// R-Tree�𐶐�����
void CCheckBuildNamePoint::createRTree( const map<long, GeoClass>& geoMap, BGRTree& rTree )
{
	for(const auto& geo : geoMap){
		long  oID = geo.first;
		BGBox box = geo.second.GetBox();
		rTree.insert( make_pair( box, static_cast<unsigned int>(oID) ) );
	}
}

// R-Tree����A�^�������͈̔͂Ɉ���������ID�Q���擾
set<long> CCheckBuildNamePoint::getIntersectsIDs( const BGBox& box, const BGRTree& rTree )
{
	set<long> retIDs;
	vector<pair<BGBox, unsigned int>> results;
	bgi::query( rTree, bgi::intersects( box ), std::back_inserter(results) );

	for( const auto& result : results )
		retIDs.insert( static_cast<long>( result.second ) );

	return retIDs;
}

// ���O�w�b�_�o��
void CCheckBuildNamePoint::OutputLogHeader()
{
	m_log.OutputHeader();
	m_log.OutputLog( err_def::StrBlNameDB,    m_strBlNameDB );
	m_log.OutputLog( err_def::StrBuildingDB,  m_strBuildingDB );
	m_log.OutputLog( err_def::StrAnnoDB,      m_strAnnoDB );
	m_log.OutputLog( err_def::StrMeshDB,      m_strMeshDB );
	m_log.OutputLog( err_def::StrLogFile,     m_strOutput );
	m_log.OutputLog( err_def::StrMeshList,    m_strMeshList );
	m_log.OutputLog( err_def::StrNgBuild,     m_strNgBuild );
	m_log.OutputLog( err_def::StrReplace,     m_strReplaceWord );
	m_log.OutputLog( err_def::StrErrWord,     m_strErrWord );
	m_log.OutputLog( err_def::StrWarnWord,    m_strWarnWord );
	m_log.OutputLog( err_def::StrBldcOKWord,  m_strBldcOKWord );
	m_log.OutputLog( err_def::StrBldcNGWord,  m_strBldcNGWord );
	m_log.OutputLog( err_def::StrChkFixclass, m_chkFixclass ? _T("true") : _T("false") );
	m_log.OutputLog( err_def::StrJoinAnno,    m_strJoinAnno );
	m_log.OutputLog(err_def::StrExcludeMark, m_strExclude_mark.IsEmpty()? _T("�w��Ȃ�"): m_strExclude_mark );
	m_log.OutputLog( err_def::StrBufferRange, str_util::ToString( m_bufferRange ) );
};

// ���s
bool CCheckBuildNamePoint::execute()
{
	// ������
	if( !init() )
		return false;

	// �w�b�_�o��
	OutputLogHeader();
	m_log.OutputLog( err_def::StrStartTime, util::GetCurTime() );

	// ���b�V�����Ƃ̏���
	for( auto& mesh : m_meshList ){

		CString strMesh;
		strMesh.Format( _T("%ld"), mesh );

		m_log.OutputStdErr( strMesh );
		m_log.OutputLog( strMesh );

		// �א�9���b�V���R�[�h�擾
		set<long> rin9Mesh;
		Get9Mesh( mesh, rin9Mesh );

		// 9���b�V�����̃|���S���擾
		IGeometryPtr ipMeshGeo, ip9MeshGeo;
		if( ! GetMeshPolygons( mesh, rin9Mesh, ipMeshGeo, ip9MeshGeo ) ){
			m_log.OutputStdErr( err_def::GetMeshFail, strMesh );
			m_log.OutputLog( err_def::GetMeshFail, strMesh );
			continue;
		}

		// �Y���͈͓��́A�r�����́A�����A���L��S�Ċo����
		map<long, GeoClass> blNameMap, buildingMap, cAnnoMap;
		if( ! GetObjInMesh( ip9MeshGeo, blNameFields,   m_ipBlNameFC,   m_blNameFM,   blNameMap ) ||
			! GetObjInMesh( ip9MeshGeo, buildingFields, m_ipBuildingFC, m_buildingFM, buildingMap ) ||
			! GetObjInMesh( ip9MeshGeo, cAnnoFields,    m_ipCityAnnoFC, m_cityAnnoFM, cAnnoMap ) ){

			m_log.OutputStdErr( err_def::GetObjFail, strMesh );
			m_log.OutputLog( err_def::GetObjFail, strMesh );
			continue;
		}

		// RTree�쐬
		BGRTree blNameRTree, buildingRTree, cAnnoRTree;
		createRTree( blNameMap,   blNameRTree );
		createRTree( buildingMap, buildingRTree );
		createRTree( cAnnoMap,    cAnnoRTree );

		// �`�F�b�N
		CheckMain( ipMeshGeo, blNameMap, buildingMap, cAnnoMap, blNameRTree, buildingRTree, cAnnoRTree );
	}

	m_log.OutputLog( err_def::StrEndTime, util::GetCurTime() );

	return true;
}

// �`�F�b�N�̃��C���֐�
void CCheckBuildNamePoint::CheckMain( const IGeometryPtr& ipMeshGeo, const map<long, GeoClass>& blNameMap, const map<long, GeoClass>& buildingMap, const map<long, GeoClass>& cAnnoMap, const BGRTree& blNameRTree, const BGRTree& buildingRTree, const BGRTree& cAnnoRTree )
{
	using namespace sindy::schema::sj::buildingname_point;

	// �r�����̃|�C���g���Ƃ̃`�F�b�N
	for( const auto& blName : blNameMap ){

		long  blNameOid = blName.first;
		auto& blNameObj = blName.second;

		// �Ώۃ��b�V���Ɗ֌W�Ȃ����̂̓X�L�b�v
		if( blNameObj.IsDisjoint( ipMeshGeo ) )
			continue;

		// �r���f�B���OID
		long  buildID = _ttol( blNameObj.GetFieldValue( kBuildingOID ) );

		// �ƌ`ID�ƌ����̐������������m�F
		bool bBuildOK = CheckBuildIdPos( blNameOid, blNameObj, buildID, buildingMap, buildingRTree );

		// ���̊֘A�̃`�F�b�N
		CheckBlNameStr( blNameOid, blNameObj );

		// �t�����l���`�F�b�N
		CheckBlNameAddInfo( blNameOid, blNameObj );

		// �ȉ��͏����ƌ`�����������Ƃ�O��ɐi�߂�
		if( ! bBuildOK )
			continue;

		// ���������擾�i��v���Ă��邱�Ƃ͏�Ŋm�F�ς݁j
		auto  buildItr = buildingMap.find( buildID );
		if( buildItr == buildingMap.end() )
			continue; // �Ȃ��͂�
		auto& buildObj = buildItr->second;

		// NG��������Ȃ����`�F�b�N
		CheckBuildClass( blNameOid, blNameObj, buildID, buildObj );

		// ���ꌚ���ɁA�����|�C���g���������Ă��Ȃ����`�F�b�N
		CheckDupBlName( blNameOid, blNameObj, buildObj, blNameMap, blNameRTree );

		// �r�����̂���A�o�b�t�@���L�����W�I���g��
		IGeometryPtr ipBufferGeom = getBufferGeometry( blNameObj.GetGeometry() );

		// �o�b�t�@���L���āA�ߗׂ̃r�����ƈ�v���Ȃ����`�F�b�N
		CheckBufferRangeBlName( blNameOid, blNameObj, ipBufferGeom, blNameMap, buildingMap, blNameRTree, buildingRTree );

		// ���L�Ƃ̐����`�F�b�N
		CheckBufferRangeAnno( blNameOid, buildID, blNameObj, buildObj, ipBufferGeom, cAnnoMap, buildingMap, cAnnoRTree, buildingRTree );
	
	}
}

// �r�����̃|�C���g�̌���ID�ƈʒu�̐����`�F�b�N
bool CCheckBuildNamePoint::CheckBuildIdPos( long blNameOid, const GeoClass& blNameObj, long buildID, const map<long, GeoClass>& buildingMap, const BGRTree& buildingRTree )
{
	// RTree�Ŋ֌W����ID�擾
	bool bFind = false, bOK = false, bHole = false;
	long findID = -1;
	auto& findBuildIDs = getIntersectsIDs( blNameObj.GetBox(0.1), buildingRTree );
	for( const auto& findBuildID : findBuildIDs ){
		auto itr = buildingMap.find( findBuildID );
		if( itr == buildingMap.end() )
			continue;
		auto& findBuildObj = itr->second;

		// �_�ƃ|���S���̂��߁ADisjoint�ŗǂ��i�����OK�j
		// �������𖄂߂��`��Ő�Ɍ���
		if( blNameObj.IsDisjoint( getExterior( findBuildObj.GetGeometry() ) ) )
			continue;

		// ���̏�ԂŊm�F���A�������ɗ����Ă邩�ǂ���
		if( blNameObj.IsDisjoint( findBuildObj.GetGeometry() ) )
			bHole = true;

		bFind = true;
		findID = findBuildID;
		if( buildID != findBuildID )
			continue;

		// �����܂ł����OK�i�������A��Ō��������ǂ���������j
		bOK = true;
		break;
	}

	// �G���[�o��
	if( ! bFind ){ // �ƌ`�O�ɑ��݂���( ! bFind )
		m_log.OutputLog( kTableName, blNameOid, blNameObj, err_def::eDisjointBuild );
		return false;
	}

	if( ! bOK ){
		if( bHole ) // �ƌ`ID����v���Ȃ��ƌ`�̒����������ɑ��݂���( bFind && ! bOK && bHole )
			m_log.OutputLog( kTableName, blNameOid, blNameObj, err_def::eHoleUnMatchID, str_util::ToString( findID ), nStr );
		else // �ƌ`ID����v���Ȃ�( bFind && ! bOK && ! bHole )
			m_log.OutputLog( kTableName, blNameOid, blNameObj, err_def::eUnMatchBuildID, str_util::ToString( findID ), nStr );
			
		return false;
	}

	if( bHole ){ // �ƌ`ID����v����ƌ`�̒����������ɑ��݂���( bFind && bOK && bHole )
		m_log.OutputLog( kTableName, blNameOid, blNameObj, err_def::wHoleMatchID, str_util::ToString( findID ), nStr );
		return false;
	}

	// OK( bFind && bOK && ! bHole )
	return true;
}

// �r�����̃|�C���g�̖��̃`�F�b�N
void CCheckBuildNamePoint::CheckBlNameStr( long blNameOid, const GeoClass& blNameObj )
{
	using namespace sindy::schema::sj::buildingname_point;
	auto& nameStr = blNameObj.GetFieldValue( kName );

	std::string str = nameStr;
	err_def::ECode errDef = err_def::Ok;

	if( m_cNameChk.IsNullString( str ) ){
		// ���̂���G���[
		m_log.OutputLog( kTableName, blNameOid, blNameObj, err_def::eNoName );
		return;
	}

	// ��������Ɏd�l�͈́i���E��񊿎��A���ȃJ�i�A�S�p�����A�M���V�������A�L���j�O�̕������������m�F
	if( ! m_cNameChk.IsOkString( str ) || ! m_cNameChk.IsEvenSize( str ) ){
		// �d�l�͈͊O�G���[
		m_log.OutputLog( kTableName, blNameOid, blNameObj, err_def::eOutRangeWord );
		return; 
	}

	// �����񒷃`�F�b�N(bug12343�Œǉ�)
	if (nameStr.GetLength() > nameMaxLength) {
		m_log.OutputLog(kTableName, blNameOid, blNameObj, err_def::eLengthOver);
	}
	// �b�����݂�����G���[
	if( m_cNameChk.IsFindPipe( str ) )
		m_log.OutputLog( kTableName, blNameOid, blNameObj, err_def::ePipeExist );

	// �S�p�X�y�[�X�̊m�F
	if( ! m_cNameChk.IsOkSpace( str ) )
		m_log.OutputLog( kTableName, blNameOid, blNameObj, err_def::eNgSpace );

	// �n�C�t���֘A������Ɋi�[����Ă��邩
	errDef = m_cNameChk.IsOkHiphen( str );
	if( errDef != err_def::Ok )
		m_log.OutputLog( kTableName, blNameOid, blNameObj, errDef );

	// �������֘A������Ɋi�[����Ă��邩
	errDef = m_cNameChk.IsOkLowStr( str );
	if( errDef != err_def::Ok )
		m_log.OutputLog( kTableName, blNameOid, blNameObj, errDef );

	// �J�b�R�̐����m�F
	errDef = m_cNameChk.IsOkKakko( str );
	if( errDef != err_def::Ok )
		m_log.OutputLog( kTableName, blNameOid, blNameObj, errDef );

	// NG���[�h�̊m�F
	for( const auto& errStr : m_errWordVec ){
		if( string::npos != m_cNameChk.string_find( str, errStr ) )
			m_log.OutputLog( kTableName, blNameOid, blNameObj, err_def::eNgWord, errStr.c_str() );
	}
	for( const auto& warnStr : m_warnWordVec ){
		if( string::npos != m_cNameChk.string_find( str, warnStr ) )
			m_log.OutputLog( kTableName, blNameOid, blNameObj, err_def::wNgWord, warnStr.c_str() );
	}

	// ��ʊm��R�[�h�擾
	long fixclass = _ttol( blNameObj.GetFieldValue( kFixClass ) );

	// "chk_fixclass"�I�v�V������true�̏ꍇ��
	// ��ʊm��R�[�h���u���m�F�i0�j�v�łȂ��I�u�W�F�N�g�ɑ΂��Ă͌�����ʃR�[�h��OK/NG���[�h�`�F�b�N���s��Ȃ�
	if( m_chkFixclass && fixclass != 0 )
		return;

	// ������ʃR�[�h��OK/NG���[�h�`�F�b�N
	CString errStr;
	errDef = CheckBlNameByBldcWord( blNameObj, str, errStr );
	if( errDef != err_def::Ok )
		m_log.OutputLog( kTableName, blNameOid, blNameObj, errDef, errStr );
}

// ������ʃR�[�h��OK/NG���[�h�ɂ�閼�̃`�F�b�N
err_def::ECode CCheckBuildNamePoint::CheckBlNameByBldcWord( const GeoClass& blNameObj, const std::string& blName, CString& judgeStr )
{
	using namespace sindy::schema::sj::buildingname_point;

	// ������ʃR�[�h1~3�擾
	set<long> bldClassSet;
	CString bldClass1 = blNameObj.GetFieldValue( kBldgClass1 );
	CString bldClass2 = blNameObj.GetFieldValue( kBldgClass2 );
	CString bldClass3 = blNameObj.GetFieldValue( kBldgClass3 );

	if( !bldClass1.IsEmpty() )
		bldClassSet.insert( _ttol( bldClass1 ) );
	if( !bldClass2.IsEmpty() )
		bldClassSet.insert( _ttol( bldClass2 ) );
	if( !bldClass3.IsEmpty() )
		bldClassSet.insert( _ttol( bldClass3 ) );

	// ����ɗp����ꂽ������i�[�p
	CString okWord, ngWord;

	// ������ʃR�[�h��NG���[�h�������Ă��邩�`�F�b�N
	bool hasNGWord = HasBldcNGWord( blName, bldClassSet, ngWord );

	// ������ʃR�[�h��OK���[�h�̃`�F�b�N
	err_def::ECode errDef = err_def::Ok;
	errDef = CheckBldNameByBldcOKWord( blName, bldClassSet, okWord );

	// OK���[�h�`�F�b�N���ʂ�OK�ŁA������ʃR�[�h��NG���[�h�������Ă���
	if( ( errDef == err_def::oOKWordByBldcOKWord || errDef == err_def::oOkWordByMultiBldcOKWord ) && hasNGWord ){
		judgeStr = ngWord;
		return err_def::wConflictBldcWordChk;
	}

	// ������ʃR�[�h��NG���[�h�������Ă���
	if( hasNGWord ){
		judgeStr = ngWord;
		return err_def::eNGNameByBldcNGWord;
	}

	judgeStr = okWord;
	return errDef;
}

// ������ʃR�[�h��NG���[�h�𖼏̂Ɏ����ǂ���
bool CCheckBuildNamePoint::HasBldcNGWord( const std::string& blName, const std::set<long>& bldClassSet, CString& judgeStr )
{
	for( const auto& bldClass : bldClassSet )
	{
		if( !bldClass )
			continue;

		// ������ʃR�[�h��NG���[�h���X�g�ɒ�`�����錚����ʂ��ǂ���
		auto& itr = m_bldcNGWordMap.find( bldClass );
		if( itr == m_bldcNGWordMap.end() )
			continue;

		// ������ʃR�[�h��NG���[�h�������Ă��邩
		for( const auto& ngWord : itr->second )
		{
			if( string::npos != m_cNameChk.string_find( blName, ngWord.GetString() ) ){
				judgeStr = ngWord.GetString();
				return true;
			}
		}
	}
	return false;
}

// ������ʃR�[�h��OK���[�h�ɂ��`�F�b�N
err_def::ECode CCheckBuildNamePoint::CheckBldNameByBldcOKWord( const std::string& blName, const std::set<long>& bldClassSet, CString& judgeStr )
{
	// �G���[��ʊi�[�p
	err_def::ECode errDef = err_def::Ok;

	// ������ʃR�[�h��OK���[�h���X�g�ɒ�`������OK���[�h�̃��[�v
	for( const auto& okWord : m_bldcOKWordMap )
	{
		// ���̂�OK���[�h���܂ނ�
		if( string::npos == m_cNameChk.string_find( blName, okWord.first.GetString() ) )
			continue;

		switch( errDef ){
		// ����OK���[�h���X�g�̒�`�ɍ��v���Ă���ꍇ�́A
		// ���̂�OK���[�h���X�g�̒�`�ɔ����镶������܂ނ��ǂ����`�F�b�N����
		case err_def::oOKWordByBldcOKWord:
		case err_def::oOkWordByMultiBldcOKWord:
			if( err_def::wNGNameByBldcOKWord == CheckOKBldc( bldClassSet, okWord.second ) ){
				judgeStr = okWord.first.GetString();
				return err_def::wConflictOKWordChk;
			}
			break;
		// ���ɖ��̂�OK���[�h���X�g�̒�`�ɔ����Ă���ꍇ�́A
		// ���̂�OK���[�h���X�g�̒�`�ɍ��v���镶������܂ނ��ǂ����`�F�b�N����
		case err_def::wNGNameByBldcOKWord:
			errDef = CheckOKBldc( bldClassSet, okWord.second );
			if( errDef == err_def::oOKWordByBldcOKWord || errDef == err_def::oOkWordByMultiBldcOKWord ){
				return err_def::wConflictOKWordChk;
			}
			break;
		case err_def::Ok:
		default:
			judgeStr = okWord.first.GetString();
			errDef = CheckOKBldc( bldClassSet, okWord.second );
			break;
		}

	}
	return errDef;
}

// ������ʃR�[�h�`�F�b�N
err_def::ECode CCheckBuildNamePoint::CheckOKBldc( const std::set<long>& chkBldcSet, const std::vector<long>& okBldcVec )
{
	// chkBldcSet�ɁAokBldcVec�Ɋ܂܂�錚����ʃR�[�h�����邩�ǂ���
	for( const auto& bldClass : chkBldcSet )
	{
		if( !bldClass )
			continue;

		if( okBldcVec.end() != find( okBldcVec.begin(), okBldcVec.end(), bldClass ) ){
			// ������ʂŒ�`����Ă���OK���[�h���ǂ���
			return okBldcVec.size() > 1 ? err_def::oOkWordByMultiBldcOKWord : err_def::oOKWordByBldcOKWord;
		}
	}
	return err_def::wNGNameByBldcOKWord;
}

namespace check_addinfo_func{
	// �b�i�p�C�v�j�Ɋւ���`�F�b�N
	err_def::ECode checkPipe( const GeoClass& blNameObj, const CString& field )
	{
		// ���̂ƃ��~�ŃG���[�𕪂��郉���_��
		auto getErr = [&field]{
			if( field.CompareNoCase( kNameSeparated ) == 0 )
				return err_def::eNamePipeInvalid;
			
			if( field.CompareNoCase( kYomiSeparated ) == 0 )
				return err_def::eYomiPipeInvalid;

			assert( false );
			return err_def::Ok;
		};

		// UNICODE�łȂ��Ɛ擪�A�����̎擾�����܂������Ȃ�
		const CStringW checkStr = blNameObj.GetFieldValue( field );

		// �擪�܂��͖���
		if( ( checkStr.Left(1).Compare( L"�b" ) == 0 ) ||
			( checkStr.Right(1).Compare( L"�b" ) == 0 ) )
			return getErr();

		// �A��
		if( -1 < checkStr.Find( L"�b�b" ) )
			return getErr();

		return err_def::Ok;

	}

	err_def::ECode checkMustInfo(const GeoClass& blNameObj)
	{
		// �t�����K�{����
		static const std::vector<CString> mustFields = boost::assign::list_of
			(kNameSeparated)
			(kYomiSeparated)
			(kBldgClass1);

		// �l�������Ă��Ȃ����̂��J�E���g
		long nulls = std::count_if(mustFields.begin(), mustFields.end(),
			[&blNameObj](const CString& field) {
				return blNameObj.GetFieldValue(field).IsEmpty();
		});

		// �S�������Ă����OK
		if( nulls == 0 )
			return err_def::Ok;

		// �S�������Ă��Ȃ��ꍇ�́A���ɗ]�v�Ȃ��̂��Ȃ����OK
		if (nulls == mustFields.size() )
		{
			static const std::vector<CString> notMust = boost::assign::list_of
				(kBldgClass2)
				(kBldgClass3)
				(kFloors);

			for (const auto& fld : notMust)
			{
				if (!blNameObj.GetFieldValue(fld).IsEmpty())
					return err_def::eInsufficientInfo;
			}

			// ������OK
			return err_def::Ok;
		}

		return err_def::eRequiredInfoInvalid;
	}

	err_def::ECode checkBldClass(const GeoClass& blNameObj)
	{
		const auto bldClass1 = blNameObj.GetFieldValue(kBldgClass1);
		// ��ʂP���󂾂�����Q�C�R����Ȃ͕̂ۏ؂����i�K�{���ڃ`�F�b�N�j
		if (bldClass1.IsEmpty())
			return err_def::Ok;


		const auto bldClass2 = blNameObj.GetFieldValue(kBldgClass2);
		const auto bldClass3 = blNameObj.GetFieldValue(kBldgClass3);

		// ���2������
		if (!bldClass1.IsEmpty() &&
			bldClass2.IsEmpty() &&
			!bldClass3.IsEmpty())
			return err_def::eNoBldClass2;

		// ������
		if (bldClass1.Compare(bldClass2) == 0 ||
			bldClass1.Compare(bldClass3) == 0 )
			return err_def::eSameBldClass;

		// 2��3�͋�łȂ��ꍇ�̂ݔ�r
		if( !bldClass2.IsEmpty() && 
			(bldClass2.Compare(bldClass3) == 0) )
			return err_def::eSameBldClass;

		return err_def::Ok;
	}
} // check_addinfo_func

// �r�����̃|�C���g�t�����l���֘A�`�F�b�N
void CCheckBuildNamePoint::CheckBlNameAddInfo( long blNameOid, const GeoClass& blNameObj )
{
	using namespace check_addinfo_func;

	// �p�C�v�`�F�b�N
	static const std::vector<CString> fields = boost::assign::list_of
		(kNameSeparated)
		(kYomiSeparated);

	for( const auto& field : fields )
	{
		auto errDef = checkPipe( blNameObj, field );
		if( errDef != err_def::Ok )
			m_log.OutputLog( kTableName, blNameOid, blNameObj, errDef );
	}

	// �K�{���ڃ`�F�b�N
	auto errDef = checkMustInfo(blNameObj);
	if (errDef != err_def::Ok)
		m_log.OutputLog(kTableName, blNameOid, blNameObj, errDef);

	// ��ʃ`�F�b�N
	errDef = checkBldClass(blNameObj);
	if (errDef != err_def::Ok)
		m_log.OutputLog(kTableName, blNameOid, blNameObj, errDef);

	// ���̂Ɩ��́i�����j�̐����`�F�b�N
	auto nameSeparated = blNameObj.GetFieldValue(kNameSeparated);
	vector<CString> nameStrings;
	// �������������������Ă��Ȃ����̂͑ΏۊO
	if (!nameSeparated.IsEmpty())
	{
		auto nameForLog = nameSeparated;	// ���s�R�[�h�������Ă���ƁA���O������邽��
		nameForLog.Replace(_T("\r\n"), _T("<���s>"));
		// Compare()�Ŕ�r�i�Ɠ����j
		if (blNameObj.GetFieldValue(kName) !=
			uh::str_util::replace(nameSeparated, _T("�b")))
		{
			m_log.OutputLog(kTableName, blNameOid, blNameObj, err_def::eDifferentName, nameForLog);
		}
		else {
			// �������ꂽ���ꂼ��̋�ɑ΂��āA������̑Ó������`�F�b�N�ibug12336�Œǉ��j
			nameStrings = ExcludeListedMark(uh::str_util::split(nameSeparated, _T("�b"), true));
			for (const auto& str : nameStrings) {
				// �n�C�t���֘A������Ɋi�[����Ă��邩
				errDef = m_cNameChk.IsOkHiphen( str.GetString() );
				if (errDef != err_def::Ok)
					m_log.OutputLog(kTableName, blNameOid, blNameObj, errDef, nameForLog);

				// �������֘A������Ɋi�[����Ă��邩�ibug12336�Œǉ��j
				errDef = m_cNameChk.IsOkLowStr( str.GetString() );
				if (errDef != err_def::Ok)
					m_log.OutputLog(kTableName, blNameOid, blNameObj, errDef, nameForLog);
			}
		}
	}

	// ���~�i�����j�̃`�F�b�N
	auto yomiSeparated = blNameObj.GetFieldValue(kYomiSeparated);
	if ( !yomiSeparated.IsEmpty() ) {
		auto yomiForLog = yomiSeparated;	// ���s�R�[�h�������Ă���ƁA���O������邽��
		yomiForLog.Replace(_T("\r\n"), _T("<���s>"));

		// �J�i�`�F�b�N(�u�b�v�����������̂Ń`�F�b�N)
		if (!CheckYomiCharacter(uh::str_util::replace(yomiSeparated, _T("�b")).GetString()))
		{
			m_log.OutputLog(kTableName, blNameOid, blNameObj, err_def::eYomiKanaInvalid, yomiForLog);
		}
		else {
			// ���́i�����j�ƃ��~�i�����j�̕������`�F�b�N�ibug12337�Œǉ��j
			// ���́i�����j�́A���O�L�����X�g���l�����Ă̔�r
			auto yomiStrings = uh::str_util::split(yomiSeparated, _T("�b"), true);
			const auto nameSize = nameStrings.size();
			if ( nameSize > 0 && yomiStrings.size() != nameSize)
			{
				m_log.OutputLog(kTableName, blNameOid, blNameObj, err_def::eDifferentCount, yomiForLog);
			}
			// �������ꂽ���ꂼ��̋�ɑ΂��āA������̑Ó������`�F�b�N�ibug12336�Œǉ��j
			for (const auto& str : yomiStrings) {
				// �n�C�t���֘A������Ɋi�[����Ă��邩
				errDef = m_cNameChk.IsOkHiphen(str.GetString());
				if (errDef != err_def::Ok)
					m_log.OutputLog(kTableName, blNameOid, blNameObj, errDef, yomiForLog);

				// �������֘A������Ɋi�[����Ă��邩�ibug12336�Œǉ��j
				errDef = m_cNameChk.IsOkLowStr(str.GetString());
				if (errDef != err_def::Ok)
					m_log.OutputLog(kTableName, blNameOid, blNameObj, errDef, yomiForLog);
			}
		}

	}

	//�K���`�F�b�N�uNULL�v��OK�u0�v���c���Ă�����NG�ibug12344�Œǉ��j
	auto kaisu = blNameObj.GetFieldValue(kFloors);
	if (!kaisu.IsEmpty() && _ttol(kaisu) == 0)
	{
		m_log.OutputLog(kTableName, blNameOid, blNameObj, err_def::eKaisuZero);
	}

}

// �r�����̃|�C���g�̏����ƌ`�̑����`�F�b�N
void CCheckBuildNamePoint::CheckBuildClass( long blNameOid, const GeoClass& blNameObj, long buildID, const GeoClass& buildObj )
{
	using namespace sindy::schema;
	long buildClass = _ttol( buildObj.GetFieldValue( building::kBldClass ) );
	auto ngItr = m_ngBuildClass.find( buildClass );
	if( ngItr == m_ngBuildClass.end() ) // ���O�����̂݊i�[����Ă��邽�߁A������Ȃ����OK
		return;

	// NG�����G���[
	m_log.OutputLog( kTableName, blNameOid, blNameObj, err_def::eNgBlClass, str_util::ToString( buildID ), str_util::ToString( buildClass ) );
}

// ���ꌚ�����̃r�����̃|�C���g���`�F�b�N
void CCheckBuildNamePoint::CheckDupBlName( long blNameOid, const GeoClass& blNameObj, const GeoClass& buildObj, const std::map<long, GeoClass>& blNameMap, const BGRTree& blNameRTree )
{
	// RTree�Ŋ֌W�r������ID�擾
	auto& findBlNameIDs = getIntersectsIDs( buildObj.GetBox(), blNameRTree );
	for( const auto& findBlNameID : findBlNameIDs ){

		// ���g�̓X�L�b�v
		if( findBlNameID == blNameOid )
			continue;

		auto itr = blNameMap.find( findBlNameID );
		if( itr == blNameMap.end() )
			continue;
		auto& findBlNameObj = itr->second;

		// �_�ƃ|���S���̂��߁ADisjoint�ŗǂ��i�����OK�j
		// �������𖄂߂��`��Ō����ibug 12342�j
		if (findBlNameObj.IsDisjoint(getExterior(buildObj.GetGeometry())))
			continue;

		// ����ƌ`�����r�����̃G���[
		auto& findBlName = findBlNameObj.GetFieldValue( kName );
		cLonLat findLonLat( findBlNameObj.GetGeometry() );
		m_log.OutputLog( kTableName, blNameOid, blNameObj, err_def::eDupPointInOne, findBlName, str_util::ToString( findBlNameID ), nStr, findLonLat );
	}
}

// �o�b�t�@�͈͂̃r�����̃|�C���g���̃`�F�b�N
void CCheckBuildNamePoint::CheckBufferRangeBlName( long blNameOid, const GeoClass& blNameObj, const IGeometryPtr& ipBufferGeom, const map<long, GeoClass>& blNameMap, const map<long, GeoClass>& buildingMap, const BGRTree& blNameRTree, const BGRTree& buildingRTree )
{
	using namespace sindy::schema::sj::buildingname_point;

	// �T�����̃r������
	auto& blName = blNameObj.GetFieldValue( kName );
	// �o�b�t�@�ɂ����錚���̃��[�v
	GeoClass bufferObj;
	bufferObj.SetGeometry( ipBufferGeom );
	auto& findBuildingIDs = getIntersectsIDs( bufferObj.GetBox(), buildingRTree );
	for( const auto& findBuildingID : findBuildingIDs ){

		// ���ۂɐڐG���Ă��邩�m�F
		auto buildItr = buildingMap.find( findBuildingID );
		if( buildItr == buildingMap.end() )
			continue;
		auto& findBuildObj = buildItr->second;

		if( findBuildObj.IsDisjoint( ipBufferGeom ) )
			continue;

		// �t�߂̉ƌ`�ɏ�������r�����̂Ƃ̖��̈�v�`�F�b�N
		CheckBufferRangeName( blNameOid, blName, blNameObj, findBuildObj, blNameMap, blNameRTree );
	}
}

// ���̔�r�`�F�b�N
void CCheckBuildNamePoint::CheckBufferRangeName( long blNameOid, const CString& blName, const GeoClass& blNameObj, const GeoClass& buildObj, const map<long, GeoClass>& blNameMap, const BGRTree& blNameRTree )
{
	using namespace sindy::schema::sj::buildingname_point;

	// �o�b�t�@�ɂ����錚���ɏ�������r�����̃|�C���g�̃��[�v
	auto& findBlNameIDs = getIntersectsIDs( buildObj.GetBox(), blNameRTree );
	for( const auto& findBlNameID : findBlNameIDs ){

		// ���g�̓X�L�b�v
		if( findBlNameID == blNameOid )
			continue;

		// ���ۂɐڐG���Ă��邩�m�F
		auto blItr = blNameMap.find( findBlNameID );
		if( blItr == blNameMap.end() )
			continue;
		auto& findBlNameObj = blItr->second;

		if( buildObj.IsDisjoint( findBlNameObj.GetGeometry() ) )
			continue;

		// ���̂���v�A�������͂ǂ��炩��܂̏ꍇ�G���[
		bool bSame = false;
		auto& findBlName = findBlNameObj.GetFieldValue( kName );
		if( ! IsLikeName( blName, findBlName, bSame ) )
			continue;

		// ��v�G���[
		cLonLat findLonLat( findBlNameObj.GetGeometry() );
		err_def::ECode errDef = bSame ? err_def::wSameNameInBuf : err_def::wLikeNameInBuf;
		m_log.OutputLog( kTableName, blNameOid, blNameObj, errDef, findBlName, str_util::ToString( findBlNameID ), str_util::ToString( m_bufferRange ), findLonLat );
	}
}

// �����r�����ƒ��L���̂���v���邩�i�ǂ��炩��܂��܂ށj
bool CCheckBuildNamePoint::CompAnnoStr( const CString& blName, const GeoClass& annoObj, bool& bSame )
{
	// ���̂���v�A�������͂ǂ��炩��܂̏ꍇ��OK
	auto& findAnnoName = annoObj.GetFieldValue( city_annotation::kNameString1 );
	if( IsLikeName( blName, findAnnoName, bSame) )
		return true; // ��v

	// ���L��ʂɂ���ĕ�������Ȃ��Ċm�F����
	long findAnnoClass = _ttol( annoObj.GetFieldValue( city_annotation::kAnnoCode ) );
	auto itr = m_joinAnnoMap.find( findAnnoClass );
	if( itr == m_joinAnnoMap.end() )
		return false; // �s��v
	for( const auto& joinAnnoStr : itr->second ){
		CString joinAnnoName1 = findAnnoName + joinAnnoStr;
		CString joinAnnoName2 = joinAnnoStr + findAnnoName;
		if( IsLikeName( blName, joinAnnoName1, bSame) || IsLikeName( blName, joinAnnoName2, bSame) )
			return true; // ��v
	}
	return false;
}

// ���̔�r�`�F�b�N
void CCheckBuildNamePoint::CheckBufferRangeAnno( long blNameOid, long buildID, const GeoClass& blNameObj, const GeoClass& buildObj, const IGeometryPtr& ipBufferGeom, const map<long, GeoClass>& cAnnoMap, const map<long, GeoClass>& buildingMap, const BGRTree& cAnnoRTree, const BGRTree& buildingRTree )
{
	// ���g�̉ƌ`���Ɉ�v���̂�����΁A�t�߂͌���K�v������
	// ���g�̉ƌ`���ɒ��L�������A�������͈�v���̂������ꍇ�A�t�߂����āA�G���[�o���킯
	using namespace sindy::schema;
	using namespace sj;

	// �T�����̃r������
	auto& blName = blNameObj.GetFieldValue( buildingname_point::kName );

	// ���g�̌������ɒ��L�����݂��邩�m�F
	long sameBuildAnnoCnt = 0; // ���g�̌������ɑ��݂��钍�L��
	auto& findSameBuildAnnoIDs = getIntersectsIDs( buildObj.GetBox(), cAnnoRTree );
	for( const auto& findSameBuildAnnoID : findSameBuildAnnoIDs ){
		
		// ���ۂɐڐG���Ă��邩�m�F
		auto annoItr = cAnnoMap.find( findSameBuildAnnoID );
		if( annoItr == cAnnoMap.end() )
			continue;
		auto& findAnnoObj = annoItr->second;

		if( buildObj.IsDisjoint( findAnnoObj.GetGeometry() ) )
			continue;

		++sameBuildAnnoCnt;
		bool bSame = false;
		if( CompAnnoStr( blName, findAnnoObj, bSame ) )
			return; // ���g�̉ƌ`���ň�v������OK
	}

	// �o�b�t�@�ɂ����錚���̃��[�v
	IGeometryPtr ipUnionGeom, ipTempGeom = buildObj.GetGeometry(); // ��Ō������O����p
	ISpatialReferencePtr ipSpRef = buildObj.GetGeometry()->GetSpatialReference();
	bool bBuildUnionFail = false;
	bool bOtherSameName  = false;
	GeoClass bufferObj;
	bufferObj.SetGeometry( ipBufferGeom );
	auto& findBuildingIDs = getIntersectsIDs( bufferObj.GetBox(), buildingRTree );
	for( const auto& findBuildingID : findBuildingIDs ){

		// ���g�̏��������͏�Ŋm�F���Ă��邽�߁A�X�L�b�v
		if( findBuildingID == buildID )
			continue;

		// �`��擾
		auto buildItr = buildingMap.find( findBuildingID );
		if( buildItr == buildingMap.end() )
			continue;
		auto& findBuildObj = buildItr->second;

		// ��ŉƌ`�O�̒��L���ʗp��Union
		if( FAILED( ((ITopologicalOperatorPtr)ipTempGeom)->Union( findBuildObj.GetGeometry(), &ipUnionGeom )) )
			bBuildUnionFail = true;
		ipUnionGeom->PutRefSpatialReference( ipSpRef );
		ipTempGeom = ipUnionGeom;
		
		// ���ۂɐڐG���Ă��邩�m�F
		if( findBuildObj.IsDisjoint( ipBufferGeom ) )
			continue;

		// �Y���������ɑ��݂��钍�L�̃��[�v
		auto& findOtherBuildAnnoIDs = getIntersectsIDs( findBuildObj.GetBox(), cAnnoRTree );
		for( const auto& findOtherBuildAnnoID : findOtherBuildAnnoIDs ){

			// ���ۂɐڐG���Ă��邩�m�F
			auto annoItr = cAnnoMap.find( findOtherBuildAnnoID );
			if( annoItr == cAnnoMap.end() )
				continue;
			auto& findAnnoObj = annoItr->second;

			if( findBuildObj.IsDisjoint( findAnnoObj.GetGeometry() ) )
				continue;

			// ���̂���v�A�������͂ǂ��炩��܂̏ꍇ�̓G���[
			bool bSame = false;
			if( CompAnnoStr( blName, findAnnoObj, bSame ) ){
				
				// �G���[�o��
				err_def::ECode errDef = GetExistAnnoErrDef( sameBuildAnnoCnt, bSame );
				auto& findAnnoName = findAnnoObj.GetFieldValue( city_annotation::kNameString1 );
				cLonLat findLonLat( findAnnoObj.GetGeometry() );
				m_log.OutputLog( kTableName, blNameOid, blNameObj, errDef, findAnnoName, str_util::ToString( findOtherBuildAnnoID ), str_util::ToString( m_bufferRange ), findLonLat );
				bOtherSameName = true;
			}
		}
	}

	// �o�b�t�@�ɂ����钍�L�̃��[�v
	auto& findBufferAnnoIDs = getIntersectsIDs( bufferObj.GetBox(), cAnnoRTree );
	for( const auto& findBufferAnnoID : findBufferAnnoIDs ){
		
		auto itr = cAnnoMap.find( findBufferAnnoID );
		if( itr == cAnnoMap.end() )
			continue;
		auto& findBufferAnnoObj = itr->second;

		// �����ɐڐG���Ă�����̂͊m�F�ς݂̂��߃X�L�b�v
		if( ! bBuildUnionFail ){
			if( ! findBufferAnnoObj.IsDisjoint( ipUnionGeom ) )
				continue;
		}
		else{ // �Œ�������̉ƌ`�����O���Ă���΁A��������Ȃ�(�]�v�ɓ���G���[���o��)
			if( ! findBufferAnnoObj.IsDisjoint( buildObj.GetGeometry() ) )
				continue;
		}

		// ���̂���v�A�������͂ǂ��炩��܂̏ꍇ�̓G���[
		bool bSame = false;
		if( CompAnnoStr( blName, findBufferAnnoObj, bSame ) ){

			// �G���[�o��
			err_def::ECode errDef = GetExistAnnoErrDef( sameBuildAnnoCnt, bSame );
			auto& findAnnoName = findBufferAnnoObj.GetFieldValue( city_annotation::kNameString1 );
			cLonLat findLonLat( findBufferAnnoObj.GetGeometry() );
			m_log.OutputLog( kTableName, blNameOid, blNameObj, errDef, findAnnoName, str_util::ToString( findBufferAnnoID ), str_util::ToString( m_bufferRange ), findLonLat );
			bOtherSameName = true;
		}
	}	

	// ���ӂɑ��݂���ꍇ�͂����ŏI��
	if( bOtherSameName )
		return;

	// ���ӂɑ��݂��Ȃ������ꍇ�̃G���[�o��
	err_def::ECode errDef = GetNoAnnoErrDef( sameBuildAnnoCnt );
	if( errDef == err_def::Ok )
		return;
	
	// �G���[�o��
	cLonLat dummyLonLat;
	m_log.OutputLog( kTableName, blNameOid, blNameObj, errDef, nStr, nStr, str_util::ToString( m_bufferRange ), dummyLonLat );
}

// �w��͈͕��̃o�b�t�@���擾����
IGeometryPtr CCheckBuildNamePoint::getBufferGeometry(const IGeometryPtr& ipGeometry)
{
	// ���̋�ԎQ��
	ISpatialReferencePtr ipSpRef = ipGeometry->GetSpatialReference();

	IGeometryPtr ipBuffer;

	//�W�I���g���̏���ύX����̂ŁA�N���[�����쐬����
	IClonePtr ipClone;
	IClonePtr(ipGeometry)->Clone(&ipClone);

	IGeometryPtr ipNewGeometry = ipClone;

	//���n�n�̕ύX(���b�V���Z���^�[�Ŏ擾���Ă���19���W�n)	
	ipNewGeometry->Project(m_ipPrjCoordinateSystem);

	ITopologicalOperatorPtr ipTopo( ipNewGeometry );

	//�P�ʂ̓��[�g��(m)
	ipTopo->Buffer( m_bufferRange, &ipBuffer );

	// ���n�n��߂�
	ipBuffer->Project( ipSpRef );

	return ipBuffer;
}

// �������|���S���𖄂߂�(Exterior�̂ݎ擾)
IGeometryPtr CCheckBuildNamePoint::getExterior( const IGeometryPtr& ipGeom )
{
	// �W�I���g���J�E���g�擾
	IGeometryPtr ipRetGeom = ipGeom;
	IGeometryCollectionPtr ipGeoCol = ipGeom;
	long geoCnt = ipGeoCol->GetGeometryCount();
	if( geoCnt < 2 )
		return ipRetGeom;

	// ��ȏ�̏ꍇ�́AExterior�݂̂ōč\�z����
	ISpatialReferencePtr ipSpRef = ipGeom->GetSpatialReference();
	IGeometryPtr ipExteriorGeo = ipGeoCol->GetGeometry(0);
	IPointCollectionPtr ipPointCol = ipExteriorGeo;
	long exteriorPointCnt = ipPointCol->GetPointCount();
	IPointCollectionPtr ipPolygon( CLSID_Polygon );
	((IGeometryPtr)ipPolygon)->PutRefSpatialReference( ipSpRef );
	for( int i = 0 ; i < exteriorPointCnt ; ++i )
		ipPolygon->AddPoint( ipPointCol->GetPoint( i ) );
	
	ipRetGeom = (IGeometryPtr)ipPolygon;

	return ipRetGeom;
};

// ��̕����񂪈�v�A�������͂ǂ��炩��܂��H
bool CCheckBuildNamePoint::IsLikeName( const CString& cStr1, const CString& cStr2, bool& bSame )
{
	bSame = false;
	if( cStr1.IsEmpty() || cStr2.IsEmpty() )
		return false;
	string str1 = ReplaceStr(cStr1), str2 = ReplaceStr(cStr2);
	if( str1.empty() || str2.empty() )
		return false;

	if( 0 == str1.compare( str2 ) ){
		bSame = true;
		return true; // ��v
	}

	if( string::npos != m_cNameChk.string_find( str1, str2 ) )
		return true; // str2��str1�ɕ�܂����

	if( string::npos != m_cNameChk.string_find( str2, str1 ) )
		return true; // str1��str2�ɕ�܂����

	return false;
}

string CCheckBuildNamePoint::ReplaceStr( const CString& cStr )
{
	string str = cStr;
	for( const auto& replaceWord : m_replaceWordVec ){
		long size = str.size();
		for( int i = 0 ; i < size ; ){
			string::size_type pos = m_cNameChk.string_find( str, replaceWord.first, i );
			if( pos == string::npos )
				break;

			if( 0 == replaceWord.second.compare( _T("�i�폜�j") ) )
				str = str.substr( 0, pos ) + str.substr( pos + replaceWord.first.length() );
			else
				str = str.substr( 0, pos ) + replaceWord.second + str.substr( pos + replaceWord.first.length() );
			i = pos;
		}
	}
	return str;
}

// ���O���X�g��̋L������������Ԃ̕�������������߂�
std::vector<CString> CCheckBuildNamePoint::ExcludeListedMark( const std::vector<CString>& cStr)
{
	std::vector<CString> resultStrings;
	for (const auto& str : cStr) {
		if (m_ExcludeMarkVec.find( str.GetString() ) == m_ExcludeMarkVec.end() ){
			resultStrings.push_back( str );
		}
	}
	return resultStrings;
}
