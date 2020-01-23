#include "stdafx.h"
#include "MakeSHP.h"
#include "common.h"
#include "LogSys.h"

using namespace shp05::schema::road::height_node;

const long EXTNUM=5;														// Shape�t�@�C���g���q��
const CString EXT[EXTNUM] = {_T("shp"), _T("shx"), _T("dbf"), _T("prj"), _T("cpg")};	// Shape�t�@�C���g���q
const long SHPHEADERSIZE = 100;												// shp�w�b�_�̈�
const long SHPRECSIZE = 8 + 4 + 8 + 8;										// shape���R�[�h�T�C�Y ���R�[�h�w�b�_��[8] + �V�F�[�v�^�C�v[4] + ���WXY[8+8]
const long DBFDEFAULTSIZE = 32 + 32*7 + 1 + 1;								// �\��w�b�_��(32�o�C�g)�ƃt�B�[���h�錾��(�t�B�[���h���~32�o�C�g)�ƃt�B�[���h�̏I������������(1�o�C�g) + �����ɂ�����(1�o�C�g)�������B
const long DBFRECSIZE = 7*9 + 1;											// �t�B�[���h��(7) �~ �t�B�[���h��(9) + �폜����p��1byte
const unsigned long MAX_FILE_SIZE = LONG_MAX;								// �t�@�C�������T�C�Y LONG_MAX:2147483647 ����2GB�B2,147,483,648[byte](2*1024*1024*1024) 

MakeSHP::MakeSHP(void)
	:m_fileCount(0), m_objectidIndex(0), m_linkid1Index(0), m_divid1Index(0), m_relHeight1Index(0),
	m_linkid2Index(0), m_divid2Index(0), m_relHeight2Index(0), m_shpTotalSize(0), m_dbfTotalSize(0)
{ 
}


MakeSHP::~MakeSHP(void)
{
}

bool MakeSHP::Init(LPCTSTR outputDir)
{
	// �����t�@�C���̍폜
	CString fileName = ::kLayerName;
	
	CString fullFileName;
	for (int i = 0; i < EXTNUM; ++i) {
		// �����t�@�C�������݂���ꍇ�����邽�߃��C���h�J�[�h���g�p���Č���
		fullFileName.Format(_T("%s\\%s*.%s"), outputDir, fileName, EXT[i]);
		HANDLE hFind;
		WIN32_FIND_DATA fd;
		hFind = FindFirstFile(CT2CW(fullFileName), &fd); // �ŏ��̃t�@�C������
		if (hFind != INVALID_HANDLE_VALUE) {
			do {
				CString deleteFile;
				deleteFile.Format(_T("%s\\%s"), outputDir, fd.cFileName);
				if (DeleteFile(deleteFile) == 0) {
					LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("�t�@�C���폜�Ɏ��s���܂���"), fullFileName);
					return false;
				}
			} while(FindNextFile(hFind, &fd)); // ���̃t�@�C������
		}
	}
	// ���[�N�X�y�[�X�̎擾
	if (! common_proc::GetWorkspace(outputDir, m_ipWorkspace))
		return false;
	// ��Shape�̍쐬
	CString shpName;
	shpName.Format(_T("%s_%d"), ::kLayerName, m_fileCount);
	++m_fileCount;
	if (! MakeVacuityShp(shpName))
		return false;
	// �C���f�b�N�X�擾
	if (! SetFieldIndex())
		return false;
	return true;
}

bool MakeSHP::InsertRecord(const CnvData& cnvData, const long startID)
{
	IFeatureCursorPtr ipFeatureCursor;
	if (FAILED(m_ipFeatureClass->Insert(VARIANT_TRUE, &ipFeatureCursor))) {
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("�t�B�[�`���J�[�\���̑}���Ɏ��s���܂���"), ::kLayerName);
		return false;
	}
	// �ϊ������f�[�^��insert
	std::map<long, OutSHPRec>::const_iterator outRecItr = (0 == startID) ? cnvData.begin() : cnvData.find(startID);
	for (; outRecItr != cnvData.end(); ++outRecItr) {
		IFeatureBufferPtr ipFeatureBuffer;
		if (FAILED(m_ipFeatureClass->CreateFeatureBuffer(&ipFeatureBuffer))) {
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("�t�B�[�`���o�b�t�@�̍쐬�Ɏ��s���܂���"), ::kLayerName, outRecItr->first);
			return false;
		}
		// �����̐ݒ�
		if (! common_proc::PutValue(ipFeatureBuffer, ::kLayerName, shp05::schema::kObjectId, m_objectidIndex, CComVariant(outRecItr->first)) ||
			! common_proc::PutValue(ipFeatureBuffer, ::kLayerName, ::kLinkId1, m_linkid1Index, CComVariant(outRecItr->second.e_linkid1)) ||
			! common_proc::PutValue(ipFeatureBuffer, ::kLayerName, ::kDivId1, m_divid1Index, CComVariant(outRecItr->second.e_divid1)) ||
			! common_proc::PutValue(ipFeatureBuffer, ::kLayerName, ::kRelHeight1, m_relHeight1Index, CComVariant(outRecItr->second.e_height1)) ||
			! common_proc::PutValue(ipFeatureBuffer, ::kLayerName, ::kLinkId2, m_linkid2Index, CComVariant(outRecItr->second.e_linkid2)) ||
			! common_proc::PutValue(ipFeatureBuffer, ::kLayerName, ::kDivId2, m_divid2Index, CComVariant(outRecItr->second.e_divid2)) ||
			! common_proc::PutValue(ipFeatureBuffer, ::kLayerName, ::kRelHeight2, m_relHeight2Index, CComVariant(outRecItr->second.e_height2)))
			return false;
		// �`��̐ݒ�
		if (FAILED(ipFeatureBuffer->putref_Shape(outRecItr->second.e_ipGeoemtry))) {
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("�`��t�^�Ɏ��s���܂���"), ::kLayerName);
			return false;
		}
		// �t�B�[�`���o�^
		CComVariant id;
		if (FAILED(ipFeatureCursor->InsertFeature(ipFeatureBuffer, &id))) {
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("�t�B�[�`���ǉ��Ɏ��s���܂���"), ::kLayerName);
			return false;
		}
		m_shpTotalSize += SHPRECSIZE;
		m_dbfTotalSize += DBFRECSIZE;
		// ���̃��R�[�h��o�^�����shp or dbf��2GB���z����ꍇ�A�t���b�V������Shape����蒼���B
		if (m_shpTotalSize > (MAX_FILE_SIZE-SHPRECSIZE) || m_dbfTotalSize > (MAX_FILE_SIZE-DBFRECSIZE)) {
			// �����܂ł̓o�^
			if (FAILED(ipFeatureCursor->Flush())) {
				LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("�t�B�[�`���̃t���b�V���Ɏ��s���܂���"), ::kLayerName);
				return false;
			}
			// ��Shape�쐬
			CString shpName;
			shpName.Format(_T("%s_%d"), ::kLayerName, m_fileCount);
			++m_fileCount;
			if (! MakeVacuityShp(shpName))
				return false;
			// ���̃��R�[�h����V����Shape�֓o�^������B
			long nextID = (++outRecItr)->first;
			if (! InsertRecord(cnvData, nextID))
				return false;
			return true;
		}
	}
	// �t���b�V��
	if (FAILED(ipFeatureCursor->Flush())) {
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("�t�B�[�`���̃t���b�V���Ɏ��s���܂���"), ::kLayerName);
		return false;
	}

	return true;
}

bool MakeSHP::ReName(LPCTSTR outputDir)
{
	if (1 != m_fileCount)
		return true;

	CString beforeBaseName, afterBaseName;
	beforeBaseName.Format(_T("%s\\%s_0"), outputDir, ::kLayerName);
	afterBaseName.Format(_T("%s\\%s"), outputDir, ::kLayerName);

	// Shape���ʎq���J�Ԃ�
	for (int i = 0; i < EXTNUM; ++i) {
		CString beforeName, afterName;
		beforeName.Format(_T("%s.%s"), beforeBaseName, EXT[i]);
		afterName.Format(_T("%s.%s"), afterBaseName, EXT[i]);
		if(PathFileExists(beforeName)) {
			if (rename(CT2CA(beforeName), CT2CA(afterName)) != 0) {
				LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("�t�@�C�����ύX�Ɏ��s���܂���"),  beforeName);
				return false;
			}
		}
	}
	return true;
}

bool MakeSHP::MakeVacuityShp(LPCTSTR shpName)
{
	CString errMsg;
	// �����t�B�[���h�쐬
	IFieldsEditPtr ipFields(CLSID_Fields);
	if (! MakeAttrField(shp05::schema::kObjectId, esriFieldTypeInteger, VARIANT_FALSE, VARIANT_FALSE, 0, 9, ipFields) ||
		! MakeAttrField(::kLinkId1, esriFieldTypeInteger, VARIANT_FALSE, VARIANT_FALSE, 0, 9, ipFields) ||
		! MakeAttrField(::kDivId1, esriFieldTypeInteger, VARIANT_FALSE, VARIANT_FALSE, 0, 9, ipFields) ||
		! MakeAttrField(::kRelHeight1, esriFieldTypeInteger, VARIANT_FALSE, VARIANT_FALSE, 0, 9, ipFields) ||
		! MakeAttrField(::kLinkId2, esriFieldTypeInteger, VARIANT_FALSE, VARIANT_FALSE, 0, 9, ipFields) ||
		! MakeAttrField(::kDivId2, esriFieldTypeInteger, VARIANT_FALSE, VARIANT_FALSE, 0, 9, ipFields) ||
		! MakeAttrField(::kRelHeight2, esriFieldTypeInteger, VARIANT_FALSE, VARIANT_FALSE, 0, 9, ipFields))
		return false;
	// �`��t�B�[���h�쐬
	if (! MakeShapeField(esriGeometryPoint, ipFields))
		return false;
	// �t�B�[�`���N���X�쐬
	if (FAILED(((IFeatureWorkspacePtr)m_ipWorkspace)->CreateFeatureClass(CComBSTR(shpName), ipFields, 0, 0, esriFTSimple, CComBSTR(shp05::schema::kShape), CComBSTR(_T("")), &m_ipFeatureClass))) {
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("��Shape�쐬�Ɏ��s���܂���"), shpName);
		return false;
	}
	// Shape�T�C�Y�̃��Z�b�g
	m_shpTotalSize = SHPHEADERSIZE;
	m_dbfTotalSize = DBFDEFAULTSIZE;
	return true;
}

bool MakeSHP::MakeAttrField(LPCTSTR fieldName,
							const esriFieldType fieldType,
							const VARIANT_BOOL editable,
							const VARIANT_BOOL nullable,
							const long fieldLength,
							const long precision,
							IFieldsEditPtr& ipFields)
{
	CString errMsg;
	IFieldEditPtr ipFieldEdit(CLSID_Field);
	if (FAILED(ipFieldEdit->put_Name(CComBSTR(fieldName))) ||
		FAILED(ipFieldEdit->put_Type(fieldType)) ||
		FAILED(ipFieldEdit->put_Editable(editable)) ||
		FAILED(ipFieldEdit->put_IsNullable(nullable))){
			errMsg.Format(_T("�t�B�[���h�쐬�Ɏ��s���܂���:%s"), fieldName);
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, errMsg, ::kLayerName);
			return false;
	}
	if (0 != fieldLength) {
		if (FAILED(ipFieldEdit->put_Length(fieldLength))) {
			errMsg.Format(_T("�t�B�[���h���̐ݒ�Ɏ��s���܂����F%s"), fieldName);
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, errMsg, ::kLayerName);
			return false;
		}
	}
	if (0 != precision)
		ipFieldEdit->PutPrecision(precision);
	if (FAILED(ipFields->AddField((IFieldPtr)ipFieldEdit))) {
		errMsg.Format(_T("�t�B�[���h�ǉ��Ɏ��s���܂���:%s"), fieldName);
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, errMsg, ::kLayerName);
		return false;
	}
	return true;
}

bool MakeSHP::MakeShapeField(const esriGeometryType geoType, IFieldsEditPtr& ipFields)
{

	// GeometryDef�̍쐬
	IGeometryDefEditPtr ipGeoDef(CLSID_GeometryDef);
	IUnknownCoordinateSystemPtr ipUnkCoordSys(CLSID_UnknownCoordinateSystem);
	if (FAILED(ipGeoDef->putref_SpatialReference(ipUnkCoordSys)) ||
		FAILED(ipGeoDef->put_GeometryType(geoType))) {
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("�`���`�ݒ�Ɏ��s���܂���"), ::kLayerName);
		return false;
	}

	// �`��t�B�[���h�̍쐬	
	IFieldEditPtr ipFieldEdit(CLSID_Field);
	if (FAILED(ipFieldEdit->put_Name(CComBSTR(shp05::schema::kShape))) ||
		FAILED(ipFieldEdit->put_Type(esriFieldTypeGeometry)) ||
		FAILED(ipFieldEdit->put_IsNullable(VARIANT_FALSE)) ||
		FAILED(ipFieldEdit->putref_GeometryDef(ipGeoDef))) {
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("�`��t�B�[���h�쐬�Ɏ��s���܂���"), ::kLayerName);
			return false;
	}
	if (FAILED(ipFields->AddField((IFieldPtr)ipFieldEdit))) {
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("�`��t�B�[���h�ǉ��Ɏ��s���܂���"), ::kLayerName);
		return false;
	}
	return true;
}

bool MakeSHP::SetFieldIndex()
{
	// �e��C���f�b�N�X�擾
	IFieldsPtr ipFields;
	if (FAILED(m_ipFeatureClass->get_Fields(&ipFields))) {
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("�t�B�[���h�擾�Ɏ��s���܂���"), ::kLayerName);
		return false;
	}
	if (FAILED(ipFields->FindField(CComBSTR(shp05::schema::kObjectId), &m_objectidIndex)) ||
		FAILED(ipFields->FindField(CComBSTR(::kLinkId1), &m_linkid1Index)) ||
		FAILED(ipFields->FindField(CComBSTR(::kDivId1), &m_divid1Index)) ||
		FAILED(ipFields->FindField(CComBSTR(::kRelHeight1), &m_relHeight1Index)) ||
		FAILED(ipFields->FindField(CComBSTR(::kLinkId2), &m_linkid2Index)) ||
		FAILED(ipFields->FindField(CComBSTR(::kDivId2), &m_divid2Index)) ||
		FAILED(ipFields->FindField(CComBSTR(::kRelHeight2), &m_relHeight2Index)))
		return false;
	return true;
}
