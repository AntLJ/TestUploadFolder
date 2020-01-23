#include "stdafx.h"
#include "LogSys.h"
#include "common.h"
#include "TransParam.h"
#include "CheckWarningSign.h"
#include "Executer.h"

#include <WinLib/VersionInfo.h>
#include <WinLib/CStringTokenizer.h>

using namespace sindy::schema;

// ���s���O�E�G���[���O�̍쐬
bool initLog(const TransParam& transParam)
{
	if (! LogSys::getInstance().setProcLog(transParam.getRunLog())) {
		std::cerr << "ERROR\t" << "���s���O�̍쐬�Ɏ��s���܂���\t" << CT2A(transParam.getRunLog()) << std::endl;
		return false;
	}
	if (! LogSys::getInstance().setErrLog(transParam.getErrLog())) {
		std::cerr << "ERROR\t" << "�G���[���O�̍쐬�Ɏ��s���܂���\t" << CT2A(transParam.getErrLog()) << std::endl;
		return false;
	}
	LogSys::getInstance().writeErrLog(false, false, _T("# FREESTYLELOG\n"));
	LogSys::getInstance().writeErrLog(false, false, _T("# �t���O\t���C����\t�I�u�W�F�N�gID\t�G���[���x��\t�G���[���b�Z�[�W\n"));
	return true;
}

// �c�[�����̏o��
void procInfoWrite(TransParam& transParam)
{
	// �c�[�����̕\��
	CVersion toolVersion;
	CString toolInfo;
	toolInfo.Format(_T("%s VERSION:%s\n"), toolVersion.GetOriginalFilename(), toolVersion.GetFileVersion());
	LogSys::getInstance().writeProcLog(true, false, toolInfo);
	// �I�v�V�����̕\��
	LogSys::getInstance().writeProcLog(true, false, CA2CT(transParam.getOptions().c_str()));
}

// �o�̓��O�̍쐬
bool openOutputLog(const TransParam& transParam, std::ofstream	&ofsOutputLog)
{
	ofsOutputLog.open( transParam.getOutputLog());
	if(! ofsOutputLog.is_open()){
		LogSys::getInstance().writeFreeStyleLog(true, err_level::err, _T("�o�̓��O�̃I�[�v���Ɏ��s���܂���"));
		return false;
	}

	return true;
}

// �R�[�h�l�h���C�����X�g�̍쐬
bool domainlist(const IFieldPtr& ipRoadField, std::map<long, CString>& mapDomain)
{
	IDomainPtr ipDomain;
	if(ipRoadField->get_Domain(&ipDomain) != S_OK){
		LogSys::getInstance().writeFreeStyleLog(true, err_level::err, _T("�R�[�h�l�h���C���̎擾�Ɏ��s���܂���"));
		return false;
	}
	ICodedValueDomainPtr ipRoadDomain = ipDomain;

	long count = 0;
	if(FAILED(ipRoadDomain->get_CodeCount(&count))){
		LogSys::getInstance().writeFreeStyleLog(true, err_level::err, _T("�R�[�h�l�h���C���̃R�[�h���̎擾�Ɏ��s���܂���"));
		return false;
	}

	for(int i = 0; i < count; ++i){
		CComVariant rVal;
		CComBSTR rName;
		if(FAILED(ipRoadDomain->get_Value(i, &rVal))){
			LogSys::getInstance().writeFreeStyleLog(true, err_level::err, _T("�R�[�h�l�̎擾�Ɏ��s���܂���"));
			return false;
		}
		if(FAILED(ipRoadDomain->get_Name(i, &rName))){
			LogSys::getInstance().writeFreeStyleLog(true, err_level::err, _T("�R�[�h���̎擾�Ɏ��s���܂���"));
			return false;
		}
		mapDomain.insert(std::make_pair(rVal.lVal, CString(rName)));
	}
	return true;
}

// ���H�����N�f�[�^�̎擾
bool getRoadLink(const TransParam& transParam, const IWorkspacePtr& ipRoadWorkspace, IFeatureClassPtr& ipRoadFC, ISpatialReferencePtr& ipRoadSPRef, std::map<long, CString>& mapDomain, long& roadClassIndex)
{
	// ���H�t�B�[�`���N���X�̎擾
	if (! common_proc::getFeatureClass(ipRoadWorkspace, road_link::kTableName, transParam.getRoadOwner(), ipRoadFC))
		return false;

	// ��ԎQ�Ǝ擾
	if(FAILED(((IGeoDatasetPtr)ipRoadFC)->get_SpatialReference( &ipRoadSPRef))){
		LogSys::getInstance().writeFreeStyleLog(true, err_level::err, _T("��ԎQ�Ƃ̎擾�Ɏ��s���܂���"));
		return false;
	}

	// ���H�����N�e�[�u���̃t�B�[���h�Q�擾
	IFieldsPtr ipRoadFields;
	if (! common_proc::getFields(road_link::kTableName, ipRoadFC, ipRoadFields))
		return false;

	if(ipRoadFC->FindField(CComBSTR(road_link::kRoadClass), &roadClassIndex) != S_OK ) {
		LogSys::getInstance().writeFreeStyleLog(true, err_level::err, _T("���H�W����ʃC���f�b�N�X�̌����Ɏ��s���܂���"));
		return false;
	}

	IFieldPtr ipRoadField;
	if (! common_proc::getField(road_link::kTableName, ipRoadFields, roadClassIndex, ipRoadField))
		return false;

	// ���H��ʃh���C���擾����
	if(! domainlist(ipRoadField,mapDomain))
		return false;

	return true;
}

// �x���W���f�[�^�̎擾
bool getWarningSign(const TransParam& transParam, const IWorkspacePtr& ipWSWorkspace, IFeatureClassPtr& ipWSFC, std::map<long, CAdapt<IPointPtr>>& mapGeo)
{
	// �x���W���t�B�[�`���N���X�擾
	CComBSTR WSTableName = _T("warning_sign");
	if (! common_proc::getFeatureClass(ipWSWorkspace, WSTableName, transParam.getWaringSignOwner(), ipWSFC))
		return false;

	// �t�B�[�`���擾
	CString WSWhereCluse;
	IFeatureCursorPtr ipWSFeatureCursor;
	if (! common_proc::getFeatureCursor(ipWSFC, WSTableName, NULL, esriSpatialRelUndefined,WSWhereCluse, ipWSFeatureCursor))
		return false;

	long OIDIndex = 0;
	if(ipWSFC -> FindField(_T("OBJECTID"),&OIDIndex) != S_OK ) {
		LogSys::getInstance().writeFreeStyleLog(true, err_level::err, _T("�x���W���I�u�W�F�N�gID�C���f�b�N�X�̌����Ɏ��s���܂���"));
		return false;
	}

	IFeaturePtr ipFeature;
	while(ipWSFeatureCursor->NextFeature(&ipFeature) == S_OK ) {
		CComVariant id;
		if(FAILED(ipFeature->get_Value(OIDIndex, &id))){
			LogSys::getInstance().writeFreeStyleLog(true, err_level::err, _T("�x���W���̃I�u�W�F�N�gID�̎擾�Ɏ��s���܂���"));
			return false;
		}
		IGeometryPtr ipGeo;
		if(FAILED(ipFeature->get_ShapeCopy(&ipGeo))) {
			LogSys::getInstance().writeFreeStyleLog(true, err_level::err, _T("�W�I���g���̃R�s�[�̎擾�Ɏ��s���܂���"));
			return false;
		}
		IPointPtr ipPoint(ipGeo);
		mapGeo.insert(std::make_pair(id.lVal, ipPoint));	// �x���W����OID�ƌ`����i�[
	}
	return true;
}

// �S�̏���
bool exec(const TransParam &transParam)
{
	LogSys::getInstance().writeProcLog(true, true, _T("�ϊ��J�n\n"));

	std::map<long, CAdapt<IPointPtr>> mapGeo;		// �x���W���f�[�^���i�[�p
	std::map<long, CString> mapDomain;				// ���H�\����ʃf�[�^�i�[�p

	// �o�̓��O�̍쐬����
	std::ofstream	ofsOutputLog;
	if(! openOutputLog(transParam, ofsOutputLog))
		return false;

	// ���[�N�X�y�[�X�擾����
	IWorkspacePtr ipRoadWorkspace, ipWSWorkspace;
	IFeatureClassPtr ipRoadFC,ipWSFC;
	long roadClassIndex = 0;

	if (! common_proc::getWorkspace(transParam.getRoadDB(), ipRoadWorkspace))
		return false;
	if (! common_proc::getWorkspace(transParam.getWarningSignDB(), ipWSWorkspace))
		return false;

	// �x���W���f�[�^�̎擾����
	if(! getWarningSign(transParam, ipWSWorkspace, ipWSFC,mapGeo))
		return false;

	// ���H�����N�f�[�^�̎擾����
	ISpatialReferencePtr ipRoadSPRef;
	if(! getRoadLink(transParam, ipRoadWorkspace, ipRoadFC, ipRoadSPRef, mapDomain, roadClassIndex))
		return false;

	// �ŋߖT���H�����N�擾����
	if (! checkWarningSign(transParam, ipRoadFC, ipRoadSPRef, roadClassIndex, mapGeo, mapDomain, ofsOutputLog))
		return false;

	LogSys::getInstance().writeProcLog(true, true, _T("�ϊ�����\n"));
	return true;
}