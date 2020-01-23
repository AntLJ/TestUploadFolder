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

#include "CreateZipCodePolygon.h"
#include "ShapeFileManager.h"
#include "EditManager.h"

#include <AddrLib/AreacodeHelper.h>
#include <AddrLib/JusyoHelper.h>
#include <WinLib/VersionInfo.h>
#include <TDC/useful_headers/directory_util.h>
#include <TDC/useful_headers/enumerate_elements.h>

using namespace std;
using namespace sindy;
using namespace sindy::schema;

namespace
{
	/**
	 * @brief �t�B�[���h���쐬����
	 * @param name [in] �t�B�[���h��
	 * @param type [in] �t�B�[���h�^�C�v
	 * @param length [in] �t�B�[���h�T�C�Y
	 * @retval IFieldEditPtr �����ɐ���
	 * @retval nullptr �����Ɏ��s
	 */
	IFieldEditPtr CreateField(const CString& name, esriFieldType type, int length)
	{
		IFieldEditPtr field(CLSID_Field);

		if( FAILED(field->put_Name(CComBSTR(name))) )
			return nullptr;

		if( FAILED(field->put_Type(type)) )
			return nullptr;

		// �p�r������I�Ȃ̂�precision��length�������Ă���
		// �t�B�[���h��`�ɕύX����������A�����̏��������������ق����ǂ�
		switch(type)
		{
		case esriFieldTypeInteger:
			if( FAILED(field->put_Precision(length)) ) return nullptr;
			break;
		case esriFieldTypeString:
			if( FAILED(field->put_Length(length)) ) return nullptr;
			break;
		default:
			return nullptr;
			break;
		}
		return field;
	}

	/**
	 * @brief �t�B�[���h�R���N�V�������쐬����
	 * @param createdFields [in] �t�B�[���h���
	 * @param fields [in,out] �t�B�[���h�R���N�V����
	 * @retval true �����ɐ���
	 * @retval false �����Ɏ��s
	 */
	bool CreateFields(
		const vector<CAdapt<IFieldEditPtr>>& createdFields,
		IFieldsEditPtr& fields)
	{
		for( auto& field : createdFields )
		{
			if( !field.m_T )
			{
				cerr << "�t�B�[���h�쐬�Ɏ��s" << endl;
				return false;
			}

			if( FAILED(fields->AddField(field.m_T)) )
			{
				cerr << "AddField�Ɏ��s" << endl;
				return false;
			}
		}
		return true;
	}

	/*
	 * @brief �X�֔ԍ��|���S���o�͐�t�B�[���h�̐���
	 * @param fields [in,out] �t�B�[���h�R���N�V����
	 * @retval true ����
	 * @retval false ���s
	 */
	bool CreateZipCodePolygonField(IFieldsEditPtr& fields)
	{
		vector<CAdapt<IFieldEditPtr>> createdFields;
		createdFields.push_back(CreateField(shape_field::kObjectId, esriFieldTypeInteger, 9));
		createdFields.push_back(CreateField(shape_field::kZipCode, esriFieldTypeString, 7));
		// Shape�t�B�[���h�͋�ԎQ�Ƃ��g���֌W�ŕʊ֐��Œǉ�����
		return CreateFields(createdFields, fields);
	}

	/*
	 * @brief �X�֔ԍ��e�[�u���o�͐�t�B�[���h�̐���
	 * @param fields [in,out] �t�B�[���h�R���N�V����
	 * @retval true ����
	 * @retval false ���s
	 */
	bool CreateZipCodeTableField(IFieldsEditPtr& fields)
	{
		vector<CAdapt<IFieldEditPtr>> createdFields;
		createdFields.push_back(CreateField(table_field::kPolygonId, esriFieldTypeInteger, 9));
		createdFields.push_back(CreateField(table_field::kPrefCode, esriFieldTypeString, 2));
		createdFields.push_back(CreateField(table_field::kCityCode, esriFieldTypeString, 3));
		createdFields.push_back(CreateField(table_field::kOazaCode, esriFieldTypeString, 3));
		createdFields.push_back(CreateField(table_field::kAzaCode, esriFieldTypeString, 3));

		return CreateFields(createdFields, fields);
	}

	/*
	 * @brief �s�s�n�}�s���E�t�B�[�`���J�[�\���擾
	 * @param ipFeatureClass [in] �t�B�[�`���N���X
	 * @param ipFeatureCursor [out] �t�B�[�`���J�[�\��
	 * @retval true ����
	 * @retval false ���s
	 */
	bool GetFeatureCursor(
		const IFeatureClassPtr& ipFeatureClass,
		IFeatureCursorPtr& ipFeatureCursor)
	{
		CString subFields;
		subFields.Format(_T("%s,%s,%s,%s,%s"),
			city_admin::kObjectID, city_admin::kCityCode, city_admin::kAddrCode,
			city_admin::kAddrCode2, _T("SHAPE"));

		// ADDRCODE��999999(�ʑ��n�A���q���~�n�Ȃ�)�͗X�֔ԍ��������Ȃ�(�f�[�^�����ł��Ȃ�)�̂ŏ��O
		// �G���A��ʁu�C�v�͏��O
		CString whereClause;
		whereClause.Format(_T("%s <> '%s' and %s <> %d"),
			city_admin::kAddrCode, _T("999999"),
			city_admin::kAreaClass, city_admin::area_class::kSea);

		IQueryFilterPtr ipQueryFilter(CLSID_QueryFilter);
		ipQueryFilter->put_SubFields(CComBSTR(subFields));
		ipQueryFilter->put_WhereClause(CComBSTR(whereClause));

		ipFeatureClass->Search(ipQueryFilter, VARIANT_TRUE, &ipFeatureCursor);
		if( !ipFeatureCursor )
		{
			cerr << "�s�s�n�}�s���E�������s" << endl;
			return false;
		}
		return true;
	}
} // namespace

// ������
bool CCreateZipCodePolygon::init()
{
	if( !m_logManager.OpenRunLog(m_args.m_runLog.c_str()) )
	{
		cerr << "���s���O�̃I�[�v���Ɏ��s�F" << m_args.m_runLog << endl;
		return false;
	}

	if( !m_logManager.OpenErrLog(m_args.m_errLog.c_str()) )
	{
		cerr << "�G���[���O�̃I�[�v���Ɏ��s�F" << m_args.m_errLog << endl;
		return false;
	}

	// ���s���O�Ɏ��s��񏑂��o��
	CVersion version;
	CString versionInfo;
	versionInfo.Format(_T("%s FileVersion: %s"),
		version.GetOriginalFilename(), version.GetFileVersion());
	m_logManager.WriteRunLog(versionInfo);
	// �w��I�v�V����
	m_logManager.WriteRunLog(m_args.GetAllOptionInfo());
	// �����J�n����
	CString startTime = _T("Start :") + m_logManager.GetCurrentTimeString();
	m_logManager.WriteRunLog(startTime);

	// �G���[���O�ɓs�s�n�}�s���E�|���S���ڑ���o��
	m_logManager.WriteErrComment(_T("SINDYSTDLOG"));
	m_logManager.WriteErrComment(m_args.m_cityDb.c_str());

	return true;
}

// ���s
bool CCreateZipCodePolygon::execute()
{
	// ������
	if( !init() )
		return false;

	// �Z���}�X�^�֌W
	if( !JusyoInit() )
		return false;

	// �ȍ~�A�o�̓��[�h���Ƃɏ����𕪂���
	if (ExecuteMode::shp == m_args.m_executeMode)
		return executeShp();
	else if (ExecuteMode::sde == m_args.m_executeMode)
		return executeSde();
	else
		return false;
}

// Shp�o�͗p
bool CCreateZipCodePolygon::executeShp()
{
	// CityAdmin�֌W
	IFeatureClassPtr ipCityAdminFc;
	if( !CityAdminInit(ipCityAdminFc) )
		return false;

	// Shp�o�͐�֌W
	IFeatureClassPtr ipZipCodePolygonFc;
	ITablePtr ipZipCodeTable;
	if( !ShapeInit(ipZipCodePolygonFc, ipZipCodeTable) )
		return false;

	cout << "�������X�g�쐬..." << endl;
	map<CString,vector<pair<CString,long>>> zipCodeBaseMap;
	IFeatureCursorPtr ipSearchCursor;
	if( !GetFeatureCursor(ipCityAdminFc, ipSearchCursor) )
	{
		return false;
	}

	IFeaturePtr ipFeature;
	while( S_OK == ipSearchCursor->NextFeature(&ipFeature) && ipFeature )
	{
		long objectId = 0;
		ipFeature->get_OID(&objectId);

		CComVariant cityCode, addrCode;
		ipFeature->get_Value(m_idxCityCode, &cityCode);
		ipFeature->get_Value(m_idxAddrCode, &addrCode);

		CString adminCityCode = cityCode;
		CString adminAddrCode = addrCode;

		CString zipCode;
		if( !GetZipCode(adminCityCode, adminAddrCode, objectId, zipCode) )
			continue;

		zipCodeBaseMap[zipCode].push_back(make_pair(adminCityCode+adminAddrCode, objectId));

		// ADDRCODE2��000000�͒ʏ̏Z���̐ݒ肪�Ȃ��Ƃ������ƂȂ̂ŏ�����΂�
		CComVariant tsushoAddrCode;
		ipFeature->get_Value(m_idxAddrCode2, &tsushoAddrCode);
		CString tsushoAdminAddrCode = tsushoAddrCode;
		if( 0 == tsushoAdminAddrCode.Compare(_T("000000")) )
			continue;

		CString tsushoZipCode;
		if( !GetZipCode(adminCityCode, tsushoAdminAddrCode, objectId, tsushoZipCode) )
			continue;

		zipCodeBaseMap[tsushoZipCode].push_back(make_pair(adminCityCode+tsushoAdminAddrCode, objectId));
	}

	long maxPolygonId = 0;
	cout << "�f�[�^�o��..." << endl;
	for( const auto& zipCodeBase : zipCodeBaseMap )
	{
		// ����X�֔ԍ��ƂȂ�|���S�����}�[�W����
		IGeometryBagPtr ipMergeGeometryBag(CLSID_GeometryBag);
		ipMergeGeometryBag->putref_SpatialReference(m_cityAdminSpatialReference);
		IGeometryCollectionPtr ipMergeGeometryCollection(ipMergeGeometryBag);

		// �Z���R�[�h�ƃI�u�W�F�N�gID�����ꂼ��i�[
		vector<CString> addrCodes;
		vector<long> targetOids;
		for( const auto& addrOid : zipCodeBase.second )
		{
			targetOids.push_back(addrOid.second);
			addrCodes.push_back(addrOid.first);
		}

		// OBJECTID IN (xx, xx, ...)�łȂ�ׂ������񐔂����Ȃ�����
		// �O�̂��߁A1000�������������悤��useful_headers��enumerate_elements���g��
		map<pair<CString,long>,IGeometryPtr> oidGeometryMap;
		uh::ElementsStrings oidStrings = uh::enumerate_elements(targetOids);
		for(auto& oidString : oidStrings)
		{
			CString whereClause;
			whereClause.Format(_T("%s IN (%s)"),
				city_admin::kObjectID,
				uh::toTStr(oidString).c_str());

			IQueryFilterPtr ipQuery(CLSID_QueryFilter);
			ipQuery->put_WhereClause(CComBSTR(whereClause));
			IFeatureCursorPtr ipCursor;
			ipCityAdminFc->Search(ipQuery, VARIANT_TRUE, &ipCursor);

			IFeaturePtr ipFeature;
			while( S_OK == ipCursor->NextFeature(&ipFeature) && ipFeature )
			{
				IGeometryPtr ipGeometry;
				ipFeature->get_ShapeCopy(&ipGeometry);
				ipMergeGeometryCollection->AddGeometry(ipGeometry);

				// �����Ώ�11���ɑ΂���W�I���g����ێ�
				// TODO: ���̕ӂ�2�x��ԂȂ̂ŁA�����Ɨǂ����@������΂�������̗p����
				long oid = 0;
				ipFeature->get_OID(&oid);
				CComVariant cityCode, addrCode;
				ipFeature->get_Value(m_idxCityCode, &cityCode);
				ipFeature->get_Value(m_idxAddrCode, &addrCode);
				CString elevenCode = CString(cityCode) + CString(addrCode);
				if( addrCodes.end() != find(addrCodes.begin(), addrCodes.end(), elevenCode) )
				{
					oidGeometryMap[make_pair(elevenCode,oid)] = ipGeometry;
				}

				CComVariant tsushoAddrCode;
				ipFeature->get_Value(m_idxAddrCode2, &tsushoAddrCode);
				CString tsushoElevenCode = CString(cityCode) + CString(tsushoAddrCode);
				if( addrCodes.end() != find(addrCodes.begin(), addrCodes.end(), tsushoElevenCode) )
				{
					oidGeometryMap[make_pair(tsushoElevenCode,oid)] = ipGeometry;
				}
			}
		}

		// ConstructUnion�Ń}�[�W
		IPolygon4Ptr ipMergePolygon(CLSID_Polygon);
		ipMergePolygon->putref_SpatialReference(m_cityAdminSpatialReference);
		IEnumGeometryPtr ipEnumMergeGeometry(ipMergeGeometryBag);
		if(FAILED(((ITopologicalOperatorPtr)ipMergePolygon)->ConstructUnion(ipEnumMergeGeometry)))
		{
			m_logManager.WriteErrLog(
				city_admin::kTableName,
				zipCodeBase.second.begin()->second, // oid��zipCodeBase�̐擪�̂��
				Error,
				error_message::kFailMerge);
			break;
		}

		// �}���`�|���S���̊m�F
		IGeometryBagPtr ipGeometryBag;
		ipMergePolygon->get_ConnectedComponentBag(&ipGeometryBag);
		IGeometryCollectionPtr ipGeometryCollection(ipGeometryBag);
		if( !ipGeometryCollection )
		{
			cerr << "�}���`�|���S�������Ɏ��s(�W�I���g���R���N�V�����ł͂Ȃ��H)" << endl;
			continue;
		}

		// �}���`�|���S���łȂ���΂����ŏo��
		long geometryCount = 0;
		ipGeometryCollection->get_GeometryCount(&geometryCount);
		if( 1 == geometryCount )
		{
			++maxPolygonId;
			IGeometryPtr ipMergeGeometry(ipMergePolygon);
			OutputFeatureClass(ipZipCodePolygonFc, maxPolygonId, ipMergeGeometry, zipCodeBase.first);
			sort(addrCodes.begin(), addrCodes.end());
			addrCodes.erase(unique(addrCodes.begin(), addrCodes.end()),addrCodes.end());
			OutputTable(ipZipCodeTable, maxPolygonId, addrCodes);
			continue;
		}

		// �}���`�|���S���Ȃ番��
		for(long index=0; index<geometryCount; ++index)
		{
			IGeometryPtr separatedGeometry;
			ipGeometryCollection->get_Geometry(index, &separatedGeometry);

			separatedGeometry->putref_SpatialReference(m_cityAdminSpatialReference);
			separatedGeometry->SnapToSpatialReference();

			vector<CString> separatedAddrCodes;
			for( auto& data : oidGeometryMap )
			{
				// �������̏Z���R�[�h�ɂ��Ă̂ݏ���(�e�[�u���f�[�^�ŏd�����N���邽��)
				const auto& findResult = 
					find(separatedAddrCodes.begin(), separatedAddrCodes.end(), data.first.first);

				if( findResult != separatedAddrCodes.end() )
					continue;

				// Disjoint�Ȃ�continue
				IRelationalOperatorPtr ipRelOpe(separatedGeometry);
				VARIANT_BOOL isDisjoint = VARIANT_FALSE;
				ipRelOpe->Disjoint(data.second, &isDisjoint);
				if( isDisjoint )
					continue;

				// Contains(����`��܂�)�łȂ��Ȃ�continue
				VARIANT_BOOL isContain = VARIANT_FALSE;
				ipRelOpe->Contains(data.second, &isContain);
				if( !isContain )
					continue;

				// �����܂ŗ�����u�����ς݁v
				separatedAddrCodes.push_back(data.first.first);
			}
			// �o��
			++maxPolygonId;
			OutputFeatureClass(ipZipCodePolygonFc, maxPolygonId, separatedGeometry, zipCodeBase.first);

			sort(separatedAddrCodes.begin(), separatedAddrCodes.end(),
				[](const CString& lhs, const CString& rhs){return lhs.Compare(rhs) < 0;});
			OutputTable(ipZipCodeTable, maxPolygonId, separatedAddrCodes);
		}
	}
	return true;
}

bool CCreateZipCodePolygon::JusyoInit()
{
	cout << "�Z��DB�ڑ�..." << endl;
	addr::CJusyoHelper jusyoHelper;
	if( !jusyoHelper.Connect(
		m_args.m_jusyoUser.c_str(),
		m_args.m_jusyoUser.c_str(),
		m_args.m_jusyoDb.c_str()) )
	{
		cerr << "�Z��DB�ڑ����s" << endl;
		return false;
	}

	cout << "�Z���}�X�^�擾..." << endl;
	CString whereClause;
	whereClause.Format(
		_T("%s is not null and %s = '000000'"),
		areacode_master::kZipCode,
		areacode_master::kHaishiDate
		);

	CString subFields;
	subFields.Format(
		_T("%s, %s, %s, %s, %s"),
		areacode_master::kKenCode,
		areacode_master::kShiCode,
		areacode_master::kOazaCode,
		areacode_master::kAzaCode,
		areacode_master::kZipCode
		);

	addr::CJusyoRecordSet records;
	jusyoHelper.Select(areacode_master::kTableName, whereClause, subFields, records);

	records.begin();
	while( !records.end() )
	{
		CString zipcode = records.GetStringValue(areacode_master::kZipCode);
		CString kenCode = records.GetStringValue(areacode_master::kKenCode);
		CString shiCode = records.GetStringValue(areacode_master::kShiCode);
		CString oazaCode = records.GetStringValue(areacode_master::kOazaCode);
		CString azaCode = records.GetStringValue(areacode_master::kAzaCode);

		m_addrZipCodeMap[kenCode+shiCode+oazaCode+azaCode] = zipcode;
		records.next();
	}

	return true;
}

bool CCreateZipCodePolygon::CityAdminInit(IFeatureClassPtr& ipCityAdminFc)
{
	cout << "�s�s�n�}�s���EDB�ڑ�..." << endl;
	CString workspaceName = CString(m_args.m_cityDb.c_str());
	IFeatureWorkspacePtr ipWorkspace(create_workspace(workspaceName));
	if( !ipWorkspace )
	{
		cerr << "�s�s�n�}�s���E�̃��[�N�X�y�[�X�擾���s�F " <<
			uh::toStr(m_args.m_cityDb) << endl;
		return false;
	}

	ipWorkspace->OpenFeatureClass(CComBSTR(city_admin::kTableName), &ipCityAdminFc);
	if( !ipCityAdminFc )
	{
		cerr << "�s�s�n�}�s���E�̎擾���s" << endl;
		return false;
	}

	ipCityAdminFc->FindField(CComBSTR(city_admin::kCityCode), &m_idxCityCode);
	ipCityAdminFc->FindField(CComBSTR(city_admin::kAddrCode), &m_idxAddrCode);
	ipCityAdminFc->FindField(CComBSTR(city_admin::kAddrCode2), &m_idxAddrCode2);
	if( m_idxCityCode == -1 || m_idxAddrCode == -1 || m_idxAddrCode2 == -1 )
	{
		cerr << "�s�s�n�}�s���E�t�B�[���h�C���f�b�N�X�擾���s" << endl;
		return false;
	}

	if( FAILED(((IGeoDatasetPtr)ipCityAdminFc)->get_SpatialReference(&m_cityAdminSpatialReference)) )
	{
		cerr << "�s�s�n�}�s���E��ԎQ�Ǝ擾���s" << endl;
		return false;
	}
	return true;
}

bool CCreateZipCodePolygon::ShapeInit(IFeatureClassPtr& ipZipCodePolygonFc, ITablePtr& ipZipCodeTable)
{
	cout << "�o�͐�쐬..." << endl;
	ShapeFileManager shpManager;
	if( !shpManager.OpenWorkspace(m_args.m_outputDir.c_str()) )
	{
		cerr << "�f�[�^�o�͐�̃I�[�v���Ɏ��s" << endl;
		return false;
	}
	IFieldsEditPtr ipPolygonFields(CLSID_Fields);
	if( !CreateZipCodePolygonField(ipPolygonFields) ||
		!shpManager.CreateShapeField(ipPolygonFields, m_cityAdminSpatialReference) )
	{
		cerr << "�|���S���t�@�C���̃t�B�[���h�ݒ�Ɏ��s" << endl;
		return false;
	}
	ipZipCodePolygonFc = 
		shpManager.CreateFeatureClass(m_args.m_shapeName.c_str(), ipPolygonFields);
	if( !ipZipCodePolygonFc )
	{
		cerr << "�|���S���t�@�C���̍쐬�Ɏ��s" << endl;
		return false;
	}
	IFieldsEditPtr ipTableFields(CLSID_Fields);
	if( !CreateZipCodeTableField(ipTableFields) )
	{
		cerr << "�e�[�u���t�@�C���̃t�B�[���h�ݒ�Ɏ��s" << endl;
		return false;
	}
	ipZipCodeTable = shpManager.CreateTable(m_args.m_tableName.c_str(), ipTableFields);
	if( !ipZipCodeTable )
	{
		cerr << "�e�[�u���t�@�C���̍쐬�Ɏ��s" << endl;
		return false;
	}
	return true;
}

bool CCreateZipCodePolygon::GetZipCode(
	const CString& cityCode, const CString& addrCode, long oid, CString& zipCode)
{
	CString addrElevenCode = cityCode + addrCode;
	zipCode = TransAddrcode2ZipCode(addrElevenCode);
	if( zipCode.IsEmpty() )
	{
		m_logManager.WriteErrLog(
			city_admin::kTableName,
			oid,
			Error,
			error_message::kNonExistingCode,
			addrElevenCode);
		return false;
	}
	return true;
}

CString CCreateZipCodePolygon::TransAddrcode2ZipCode(const CString& elevenCode )
{
	auto findResult = m_addrZipCodeMap.find(elevenCode);
	return (m_addrZipCodeMap.end() == findResult) ? CString() : findResult->second;
}

void CCreateZipCodePolygon::OutputFeatureClass(IFeatureClassPtr& zipCodePolygon,
	long polygonId, const IGeometryPtr& ipGeometry, const CString& zipCode)
{
	// �o�͐�
	IFieldsPtr newFields;
	zipCodePolygon->get_Fields(&newFields);

	long idxPolygonId = -1, idxZipCode = -1;
	newFields->FindField(CComBSTR(shape_field::kObjectId), &idxPolygonId);
	newFields->FindField(CComBSTR(shape_field::kZipCode), &idxZipCode);

	IFeatureCursorPtr fetureCursor;
	if( FAILED(zipCodePolygon->Insert(VARIANT_TRUE, &fetureCursor)) )
	{
		cerr << "�C���T�[�g�J�[�\���擾���s" << endl;
		return;
	}

	IFeatureBufferPtr featureBuffer;
	zipCodePolygon->CreateFeatureBuffer(&featureBuffer);
	if( !featureBuffer )
	{
		cerr << "CreateFeatureBuffer���s" << endl;
		return;
	}

	if( FAILED(featureBuffer->putref_Shape(ipGeometry)) )
	{
		cerr << "putref_Shape���s PolygonID : " << polygonId << endl;
		return;
	}

	if( FAILED(featureBuffer->put_Value(idxPolygonId, CComVariant(polygonId))) ||
		FAILED(featureBuffer->put_Value(idxZipCode, CComVariant(zipCode))))
	{
		cerr << "put_Value���s PolygonID : " << 	polygonId << endl;
		return;
	}

	CComVariant tempIndex;
	if( FAILED(fetureCursor->InsertFeature(featureBuffer, &tempIndex)) )
	{
		cerr << "InsertFeature���s PolygonID : " << polygonId << endl;
		return;
	}

	if( FAILED(fetureCursor->Flush()) )
	{
		cerr << "Flush���s" << endl;
	}
}

void CCreateZipCodePolygon::OutputFeatureClass(
	IFeatureBufferPtr& featureBuffer, IFeatureCursorPtr& featureCursor,
	const sindy::CFieldMap& fieldMap,
	const IGeometryPtr& ipGeometry, const pair<CString, CString>& zipCodePair)
{
	if (FAILED(featureBuffer->putref_Shape(ipGeometry)))
	{
		cerr << "putref_Shape���s" << endl;
		return;
	}

	// �X�֔ԍ��Z�b�g
	if (FAILED(featureBuffer->put_Value(
		fieldMap.FindField(zipcode_area::kZipCode1),
		CComVariant(zipCodePair.first))))
	{
		cerr << "put_Value���s" << endl;
		return;
	}

	if (FAILED(featureBuffer->put_Value(
		fieldMap.FindField(zipcode_area::kZipCode2),
		CComVariant(zipCodePair.second))))
	{
		cerr << "put_Value���s" << endl;
		return;
	}

	// ���ʑ����Z�b�g
	if (FAILED(featureBuffer->put_Value(
		fieldMap.FindField(zipcode_area::kOperator),
		CComVariant(m_commonAttr.GetLastUserName()))))
	{
		cerr << "put_Value���s" << endl;
		return;
	}
	if (FAILED(featureBuffer->put_Value(
		fieldMap.FindField(zipcode_area::kModifyProgName),
		CComVariant(m_commonAttr.GetToolName()))))
	{
		cerr << "put_Value���s" << endl;
		return;
	}
	if (FAILED(featureBuffer->put_Value(
		fieldMap.FindField(zipcode_area::kProgModifyDate),
		CComVariant(m_commonAttr.GetModifyDate()))))
	{
		cerr << "put_Value���s" << endl;
		return;
	}

	CComVariant tempIndex;
	if (FAILED(featureCursor->InsertFeature(featureBuffer, &tempIndex)))
	{
		cerr << "InsertFeature���s" << endl;
		return;
	}
}

void CCreateZipCodePolygon::OutputTable(
	ITablePtr& zipCodeTable, long polygonId, const vector<CString>& addrCodes)
{
	// �o�͐�
	IFieldsPtr newFields;
	zipCodeTable->get_Fields( &newFields );

	long idxPolygonId = -1, idxPrefCode = -1, idxCityCode = -1,
		idxOazaCode = -1, idxAzaCode = -1;
	newFields->FindField(CComBSTR(table_field::kPolygonId), &idxPolygonId);
	newFields->FindField(CComBSTR(table_field::kPrefCode), &idxPrefCode);
	newFields->FindField(CComBSTR(table_field::kCityCode), &idxCityCode);
	newFields->FindField(CComBSTR(table_field::kOazaCode), &idxOazaCode);
	newFields->FindField(CComBSTR(table_field::kAzaCode), &idxAzaCode);

	_ICursorPtr cursor;
	if( FAILED(zipCodeTable->Insert(VARIANT_TRUE, &cursor)) )
	{
		cerr << "�C���T�[�g�J�[�\���擾���s" << endl;
		return;
	}

	IRowBufferPtr rowBuffer;
	zipCodeTable->CreateRowBuffer(&rowBuffer);
	if( !rowBuffer )
	{
		cerr << "CreateRowBuffer���s" << endl;
		return;
	}

	for( const auto& addrCode : addrCodes )
	{
		if( FAILED(rowBuffer->put_Value(idxPolygonId, CComVariant(polygonId))) ||
			FAILED(rowBuffer->put_Value(idxPrefCode, CComVariant(addrCode.Left(2)))) ||
			FAILED(rowBuffer->put_Value(idxCityCode, CComVariant(addrCode.Mid(2,3)))) ||
			FAILED(rowBuffer->put_Value(idxOazaCode, CComVariant(addrCode.Mid(5,3)))) ||
			FAILED(rowBuffer->put_Value(idxAzaCode, CComVariant(addrCode.Right(3)))) )
		{
			cerr << "put_Value���s PolygonID : " << polygonId << endl;
			continue;
		}

		CComVariant vaTemp;
		if( FAILED(cursor->InsertRow(rowBuffer, &vaTemp)) )
		{
			cerr << "InsertRow���s PolygonID : " << polygonId << endl;
			continue;
		}
	}

	if( FAILED(cursor->Flush()) )
	{
		cerr << "Flush���s" << endl;
	}
}

// Sde�o�͗p
bool CCreateZipCodePolygon::executeSde()
{
	// CityAdmin
	IFeatureClassPtr ipCityAdminFc;
	if (!CityAdminInit(ipCityAdminFc))
		return false;

	// SDE�o�͐�
	CWorkspace zipWorkspace(m_args.m_outputDb.c_str());
	IFeatureClassPtr ipZipCodePolygonFc(zipWorkspace.OpenTable(zipcode_area::kTableName));
	if (!ipZipCodePolygonFc)
	{
		cerr << "�X�֔ԍ��|���S���t�B�[�`���N���X�̎擾���s" << endl;
		return false;
	}

	ISpatialReferencePtr ipZipSpatialReference;
	if (FAILED(((IGeoDatasetPtr)ipZipCodePolygonFc)->get_SpatialReference(&ipZipSpatialReference)))
	{
		cerr << "�X�֔ԍ��|���S����ԎQ�Ǝ擾���s" << endl;
		return false;
	}

	// �t�B�[���h�}�b�v�쐬
	IFieldsPtr ipFields;
	ipZipCodePolygonFc->get_Fields(&ipFields);
	if (!ipFields)
	{
		cerr << "�X�֔ԍ��|���S���t�B�[���h���擾���s" << endl;
		return false;
	}
	sindy::CFieldMap zipPolygonFieldMap;
	zipPolygonFieldMap.CreateFieldMap(ipFields);

	cout << "�������X�g�쐬..." << endl;
	map<pair<CString, CString>, vector<long>> zipCodeBaseMap;
	IFeatureCursorPtr ipSearchCursor;
	if (!GetFeatureCursor(ipCityAdminFc, ipSearchCursor))
	{
		return false;
	}

	IFeaturePtr ipFeature;
	while (S_OK == ipSearchCursor->NextFeature(&ipFeature) && ipFeature)
	{
		long objectId = 0;
		ipFeature->get_OID(&objectId);

		CComVariant cityCode, addrCode;
		ipFeature->get_Value(m_idxCityCode, &cityCode);
		ipFeature->get_Value(m_idxAddrCode, &addrCode);

		CString adminCityCode = cityCode;
		CString adminAddrCode = addrCode;

		CString zipCode;
		if (!GetZipCode(adminCityCode, adminAddrCode, objectId, zipCode))
		{
			cerr << "�X�֔ԍ��擾���s ADDRCODE: " <<
				uh::toStr(adminCityCode.GetString()) <<
				uh::toStr(adminAddrCode.GetString()) << endl;
			continue;
		}

		CComVariant tsushoAddrCode;
		ipFeature->get_Value(m_idxAddrCode2, &tsushoAddrCode);
		CString tsushoAdminAddrCode = tsushoAddrCode;

		CString tsushoZipCode;
		if (0 != tsushoAdminAddrCode.Compare(_T("000000")))
		{
			// �ʏ̏Z���R�[�h��000000�Ȃ�ʏ̗X�֔ԍ���NULL
			// 000000�łȂ��Ȃ�X�֔ԍ��擾
			if (!GetZipCode(adminCityCode, tsushoAdminAddrCode, objectId, tsushoZipCode))
			{
				cerr << "�X�֔ԍ��擾���s ADDRCODE: " <<
					uh::toStr(adminCityCode.GetString()) <<
					uh::toStr(tsushoAdminAddrCode.GetString()) << endl;
				continue;
			}
		}

		// �����ꏏ�Ȃ�ʏ̂̕���NULL�ɂ���
		if (0 == tsushoZipCode.Compare(zipCode))
			tsushoZipCode = _T("");

		zipCodeBaseMap[make_pair(zipCode, tsushoZipCode)].push_back(objectId);
	}

	cout << "�f�[�^�o��..." << endl;

	EditManager editManager(zipWorkspace);
	if (!editManager.StartEdit(false))
	{
		cerr << "�ҏW�J�n���s" << endl;
		editManager.AbortEdit();
		return false;
	}

	IFeatureCursorPtr fetureCursor;
	HRESULT hr = ipZipCodePolygonFc->Insert(VARIANT_TRUE, &fetureCursor);
	if (FAILED(hr))
	{
		cerr << "�C���T�[�g�J�[�\���擾���s" << endl;
		editManager.AbortEdit();
		return false;
	}

	IFeatureBufferPtr featureBuffer;
	ipZipCodePolygonFc->CreateFeatureBuffer(&featureBuffer);
	if (!featureBuffer)
	{
		cerr << "CreateFeatureBuffer���s" << endl;
		editManager.AbortEdit();
		return false;
	}

	for (const auto& zipCodeBase : zipCodeBaseMap)
	{
		// �X�֔ԍ�1�A2�̑g�ݍ��킹������̃|���S�����}�[�W����
		IGeometryBagPtr ipMergeGeometryBag(CLSID_GeometryBag);
		ipMergeGeometryBag->putref_SpatialReference(ipZipSpatialReference);
		IGeometryCollectionPtr ipMergeGeometryCollection(ipMergeGeometryBag);

		// OBJECTID IN (xx, xx, ...)�łȂ�ׂ������񐔂����Ȃ�����
		// �O�̂��߁A1000�������������悤��useful_headers��enumerate_elements���g��
		map<pair<CString, long>, IGeometryPtr> oidGeometryMap;
		uh::ElementsStrings oidStrings = uh::enumerate_elements(zipCodeBase.second);
		for (auto& oidString : oidStrings)
		{
			CString whereClause;
			whereClause.Format(_T("%s IN (%s)"),
				city_admin::kObjectID,
				uh::toTStr(oidString).c_str());

			IQueryFilterPtr ipQuery(CLSID_QueryFilter);
			ipQuery->put_WhereClause(CComBSTR(whereClause));
			IFeatureCursorPtr ipCursor;
			ipCityAdminFc->Search(ipQuery, VARIANT_TRUE, &ipCursor);

			IFeaturePtr ipFeature;
			while (S_OK == ipCursor->NextFeature(&ipFeature) && ipFeature)
			{
				IGeometryPtr ipGeometry;
				ipFeature->get_ShapeCopy(&ipGeometry);
				ipMergeGeometryCollection->AddGeometry(ipGeometry);
			}
		}

		// ConstructUnion�Ń}�[�W
		IPolygon4Ptr ipMergePolygon(CLSID_Polygon);
		ipMergePolygon->putref_SpatialReference(ipZipSpatialReference);
		IEnumGeometryPtr ipEnumMergeGeometry(ipMergeGeometryBag);
		ITopologicalOperatorPtr ipTopo(ipMergePolygon);
		if (FAILED((ipTopo->ConstructUnion(ipEnumMergeGeometry))))
		{
			m_logManager.WriteErrLog(
				city_admin::kTableName,
				zipCodeBase.second.front(), // oid��zipCodeBase�̐擪�̂��
				Error,
				error_message::kFailMerge);
			break;
		}

		// �}���`�|���S���̊m�F
		IGeometryBagPtr ipGeometryBag;
		ipMergePolygon->get_ConnectedComponentBag(&ipGeometryBag);
		IGeometryCollectionPtr ipGeometryCollection(ipGeometryBag);
		if (!ipGeometryCollection)
		{
			cerr << "�}���`�|���S�������Ɏ��s(�W�I���g���R���N�V�����ł͂Ȃ��H)" << endl;
			continue;
		}

		// �}���`�|���S���łȂ���΂����ŏo��
		long geometryCount = 0;
		ipGeometryCollection->get_GeometryCount(&geometryCount);
		if (1 == geometryCount)
		{
			OutputFeatureClass(featureBuffer, fetureCursor, zipPolygonFieldMap,
				ipMergePolygon, zipCodeBase.first);
			continue;
		}

		// �}���`�|���S���Ȃ番��
		for (long index = 0; index < geometryCount; ++index)
		{
			IGeometryPtr separatedGeometry;
			ipGeometryCollection->get_Geometry(index, &separatedGeometry);
			separatedGeometry->putref_SpatialReference(ipZipSpatialReference);
			separatedGeometry->SnapToSpatialReference();

			// �o��
			OutputFeatureClass(featureBuffer, fetureCursor, zipPolygonFieldMap,
				separatedGeometry, zipCodeBase.first);
		}

		if (FAILED(fetureCursor->Flush()))
		{
			cerr << "Flush���s" << endl;
			editManager.AbortEdit();
			return false;
		}
	}
	editManager.StopEdit(true);

	return true;
}
