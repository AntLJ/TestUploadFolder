#include "stdafx.h"
#include "CheckPolygon.h"
#include <TDC/useful_headers/text_util.h>
#include <WinLib/type_convert.h>

// �v���g�^�C�v�錾
bool IsAdjacent(const IGeometryPtr& baseGeometry, const IGeometryPtr& compGeometry);

using namespace sindy::schema;

namespace ErrorCode
{
	// CheckPolygon�̃G���[�R�[�h��004xxx�Ƃ���
	const CString DoesNotMergePolygon = _T("004001");
}

CheckPolygon::CheckPolygon()
{
}


CheckPolygon::~CheckPolygon()
{
}

bool CheckPolygon::Init()
{
	RUNLOGGER.Info(_T("�|���S���`�F�b�N�������c\n"));

	if (m_args.m_mesh.empty())
	{
		RUNLOGGER.Error(_T("���b�V�����w�肵�Ă�������\n"));
		return false;
	}
	m_meshes = uh::text_util::ToLongList(m_args.m_mesh);
	
	// SDE(�X�֔ԍ��|���S��)�ڑ�
	sindy::CWorkspace zipWorkspace;
	if (!zipWorkspace.Connect(m_args.m_zipDb.c_str()))
	{
		RUNLOGGER.Error(_T("�X�֔ԍ��|���S��DB�ڑ���ւ̐ڑ��Ɏ��s\n"));
		return false;
	}

	// �X�֔ԍ��|���S���擾
	m_ipZipCodeArea = zipWorkspace.OpenTable(zipcode_area::kTableName);
	if (!m_ipZipCodeArea)
	{
		RUNLOGGER.Error(_T("�X�֔ԍ��|���S���t�B�[�`���N���X�擾���s\n"));
		return false;
	}

	// ��ԎQ�Ǝ擾
	((IGeoDatasetPtr)m_ipZipCodeArea)->get_SpatialReference(&m_ipSpRef);
	if (!m_ipSpRef)
	{
		RUNLOGGER.Error(_T("��ԎQ�Ǝ擾���s"));
		return false;
	}

	IFieldsPtr ipFields;
	m_ipZipCodeArea->get_Fields(&ipFields);
	if (!ipFields)
	{
		RUNLOGGER.Error(_T("�X�֔ԍ��|���S���t�B�[���h�Ή��\�擾���s\n"));
		return false;
	}
	if (sindy::errorcode::sindyErr_NoErr != m_zipCodeAreaFieldMap.CreateFieldMap(ipFields))
	{
		RUNLOGGER.Error(_T("�X�֔ԍ��|���S���t�B�[���h�Ή��\�擾���s\n"));
		return false;
	}

	return true;
}

bool CheckPolygon::Check()
{
	RUNLOGGER.Info(_T("�|���S���`�F�b�N���s�c\n"));
	std::cout << "�|���S���̐����`�F�b�N���s�c" << std::endl;

	bool result = true;
	for (auto& mesh : m_meshes)
	{
		RUNLOGGER.Log(_T("���b�V��: %ld �̃`�F�b�N��\n"), mesh);
		std::cout << "���b�V��: " << mesh << " �̃`�F�b�N��" << std::endl;

		IGeometryPtr ipMeshGeometry = MESH2POLY(mesh, m_ipSpRef);
		result &= CheckPolygonDuplication(ipMeshGeometry);
	}

	RUNLOGGER.Info(_T("�`�F�b�N�I��\n"));
	std::cout << "�`�F�b�N�I��" << std::endl;
	return result;
}

bool CheckPolygon::CheckPolygonDuplication(const IGeometryPtr& meshGeometry)
{
	// [�`�F�b�N] �אڂ���|���S���̗X�֔ԍ��̑g�ݍ��킹��������������G���[
	// ���b�V�����̗X�֔ԍ��|���S�������A�X�֔ԍ�1�E2�̃y�A���L�[�Ƃ���map�ɏ����i�[����
	auto& spatialFilter = AheInitSpatialFilter(nullptr, meshGeometry);
	CString subFields = uh::str_util::format(_T("%s, %s, %s, %s"),
		zipcode_area::kObjectID, zipcode_area::kZipCode1, zipcode_area::kZipCode2, zipcode_area::kShape);

	IFeatureCursorPtr ipCursor;
	auto query = AheInitQueryFilter(spatialFilter, subFields);
	if (S_OK != m_ipZipCodeArea->Search(query, VARIANT_TRUE, &ipCursor) || !ipCursor)
	{
		return false;
	}

	std::map<std::pair<CString, CString>, std::vector<std::pair<long, IGeometryPtr>>> mapZipCodeArea;
	long idxZipCode1 = m_zipCodeAreaFieldMap.FindField(zipcode_area::kZipCode1);
	long idxZipCode2 = m_zipCodeAreaFieldMap.FindField(zipcode_area::kZipCode2);
	IFeaturePtr ipFeature;
	while (S_OK == ipCursor->NextFeature(&ipFeature) && ipFeature)
	{
		long oid = 0;
		ipFeature->get_OID(&oid);

		CComVariant varZipCode1, varZipCode2;
		ipFeature->get_Value(idxZipCode1, &varZipCode1);
		ipFeature->get_Value(idxZipCode2, &varZipCode2);
		CString strZipCode1(varZipCode1.bstrVal);
		CString strZipCode2(varZipCode2.bstrVal);
		IGeometryPtr ipGeometry;
		ipFeature->get_ShapeCopy(&ipGeometry);
		// TODO:
		// Recycring��True������get_Shape����ipGeometry�̃A�h���X�F�����ɂȂ��Ă��܂��B
		// ���ꂾ�ƌ�̗אڔ��肪���܂������Ȃ��̂�get_ShapeCopy�ɂ���B
		// Recycring��False��get_Shape�ɂ����ق��������񂾂낤���c��Œ��ׂ�B

		auto key = (strZipCode1 < strZipCode2) ?
			std::make_pair(strZipCode1, strZipCode2) : std::make_pair(strZipCode2, strZipCode1);

		mapZipCodeArea[key].emplace_back(oid, ipGeometry);
	}

	// �G���[�`�F�b�N
	bool hasError = false;
	for (const auto& data : mapZipCodeArea)
	{
		long dataCount = data.second.size();
		if (1 == dataCount)
			continue;

		// �����X�֔ԍ��̑g�ݍ��킹�̃|���S������������ꍇ
		const auto& idGeometry = data.second;
		for (long i = 0; i < dataCount; ++i)
		{
			IRelationalOperatorPtr ipRelOpe(idGeometry[i].second);
			for (long j = i + 1; j < dataCount; ++j)
			{
				// �܂�Disjoint
				VARIANT_BOOL disjoint = VARIANT_FALSE;
				ipRelOpe->Disjoint(idGeometry[j].second, &disjoint);
				if (disjoint)
					continue;

				// �אڃ`�F�b�N
				if (!IsAdjacent(idGeometry[i].second, idGeometry[j].second))
					continue;

				// �אڂ��Ă���Ȃ�G���[�o��
				hasError = true;

				// �X�֔ԍ�1�������ꍇ�́A�����̓s����A1�߂�NULL�ɂȂ��Ă���
				// ���O�̌��h����������Ɨǂ��Ȃ��̂�NULL�͏o�͂��Ȃ��悤�ɂ���
				CString errInfo;
				if (data.first.first.IsEmpty())
					errInfo = uh::str_util::format(_T("%s\t"), data.first.second);
				else
					errInfo = uh::str_util::format(_T("%s\t%s"), data.first.first, data.first.second);

				ERRLOGGER.writeStdLog(
					zipcode_area::kTableName,
					idGeometry[i].first,
					_T(""),
					_T(""),
					_T("ERROR"),
					ErrorCode::DoesNotMergePolygon,
					_T("����X�֔ԍ��̗אڃ|���S�����}�[�W����Ă��Ȃ�"),
					errInfo
				);
			}
		}
	}

	return !hasError;
}

/**
 * @brief �|���S�����א�(�ӂ̈ꕔ�����L���Ă���)���ǂ������m�F����
 * MapCheck��SamePropertyChk�Ɠ���
 * @param baseGeometry [in] �x�[�X�̃|���S��
 * @param compGeometry [in] �����Ώۂ̃|���S��
 * @retval true �אڂ��Ă���
 * @retval false �אڂ��Ă��Ȃ�
 */
bool IsAdjacent(const IGeometryPtr& baseGeometry, const IGeometryPtr& compGeometry)
{
	ITopologicalOperatorPtr ipBaseTopo, ipCompTopo;

	// �|���S���̘g���擾
	IGeometryPtr ipBaseBoundary, ipCompBoundary;
	ipBaseTopo = baseGeometry;
	ipBaseTopo->get_Boundary(&ipBaseBoundary);

	ipCompTopo = compGeometry;
	ipCompTopo->get_Boundary(&ipCompBoundary);

	// �g��Intersect(�|���S�����m�̐ڐ��̃W�I���g��)���擾
	// NoDimension�w�肷��Ɠ�����W�I���g���͌��W�I���g������(���̏ꍇPolyline)
	ipBaseTopo = ipBaseBoundary;
	IGeometryPtr ipResultGeometry(CLSID_GeometryBag);
	ISpatialReferencePtr ipSpRef;
	baseGeometry->get_SpatialReference(&ipSpRef);
	ipResultGeometry->putref_SpatialReference(ipSpRef);
	if (FAILED(ipBaseTopo->Intersect(ipCompBoundary, esriGeometryNoDimension, &ipResultGeometry)) ||
		!ipResultGeometry)
	{
		// �����ɓ����Ă�����אڂł͂Ȃ����Ƃɂ���
		return false;
	}

	VARIANT_BOOL vaEmpty = VARIANT_FALSE;
	ipResultGeometry->get_IsEmpty(&vaEmpty);
	return !(VB2bool(vaEmpty));
}