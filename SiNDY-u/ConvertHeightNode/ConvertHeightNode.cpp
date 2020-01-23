#include "stdafx.h"
#include "ConvertHeightNode.h"
#include "MeshList.h"
#include "BaseMesh.h"
#include "HeightNodeInfo.h"
#include "RoadWalkLinkInfo.h"
#include "CnvData.h"
#include "MakeSHP.h"


// ���s
bool CConvertHeightNode::execute()
{
	// ���b�V�����X�g�ǂݍ���
	MeshList meshlist;
	if (! meshlist.SetMeshList(m_args.m_meshList.c_str())) 
		return false;
	
	// ���[�N�X�y�[�X�擾
	IWorkspacePtr ipMeshWorkspace, ipHeightNodeWorkspace;
	if (! common_proc::GetWorkspace(m_args.m_baseMeshDB.c_str(), ipMeshWorkspace) ||
		! common_proc::GetWorkspace(m_args.m_heightNoodeDB.c_str(), ipHeightNodeWorkspace))
		return false;
	// �t�B�[�`���N���X�擾
	IFeatureClassPtr ipBaseMeshFC,ipHeightNodeFC;
	if (! common_proc::GetFeatureClass(ipMeshWorkspace, sindy::schema::basemesh::kTableName, m_args.m_baseMeshSchema.c_str(), ipBaseMeshFC))
		return false;
	if (! common_proc::GetFeatureClass(ipHeightNodeWorkspace, sindy::schema::height_node::kTableName, m_args.m_heightNodeSchema.c_str(), ipHeightNodeFC))
		return false;
	// ���΍����m�[�h��ԎQ�Ǝ擾
	ISpatialReferencePtr ipSpRef;
	if (S_OK != ((IGeoDatasetPtr)ipHeightNodeFC)->get_SpatialReference(&ipSpRef))
		return false;
	// �e��N���X�̏�����(�ÓI�����o�̐ݒ�)
	HeightNodeInfo initHeightNodeInfo(ipHeightNodeFC, ipSpRef);
	if (! initHeightNodeInfo.Init())
		return false;
	RoadWalkLinkInfo initRoadWalkInfo;
	initRoadWalkInfo.Init(m_args.m_inputDir.c_str());
	CnvData initCnvData;
	initCnvData.Init(m_args.m_buffer);
	MakeSHP makeSHP;
	if (! makeSHP.Init(m_args.m_outputDir.c_str()))
		return false;

	// ���b�V�����̏���
	CString procMsg;
	bool ret = true;
	for (const auto mesh :meshlist) {
		procMsg.Format(_T("%d\t"), mesh);
		LogSys::GetInstance().WriteProcLog(true, false, procMsg);
		// �����Ώ����b�V���`��擾
		BaseMesh baseMesh(ipBaseMeshFC);
		if (! baseMesh.SetGeometry(mesh)) {
			LogSys::GetInstance().WriteProcLog(true, false,_T("NG\n"));
			ret = false;
			continue;
		}
		// �����Ώۑ��΍������擾
		HeightNodeInfo heightNodeInfo(ipHeightNodeFC, ipSpRef);
		if (! heightNodeInfo.SetHeightNodeInfo(baseMesh.GetGeometry())){
			LogSys::GetInstance().WriteProcLog(true, false,_T("NG\n"));
			ret = false;
			continue;
		}
		// ���H�E���s�҃����N���擾
		RoadWalkLinkInfo roadWalkLinkInfo;
		if (! roadWalkLinkInfo.SetRoadWalkLinkInfo(mesh)){
			LogSys::GetInstance().WriteProcLog(true, false,_T("NG\n"));
			ret = false;
			continue;
		}
		// �ėp���΍���Shape�ϊ�
		CnvData cnvData;
		if (! cnvData.ConverteHeightNode(mesh, heightNodeInfo, roadWalkLinkInfo)){
			LogSys::GetInstance().WriteProcLog(true, false,_T("NG\n"));
			ret = false;
			continue;
		}

#ifdef _DEBUG_DIV //�@2GB�����e�X�g�p�R�[�h(2GB���z����f�[�^�����󑶍݂��Ȃ����߁A���[�v�����Ė������2GB�������N����)
		long divid_num = 0;
		// SHP����DBF�̂ق�������2GB���z����̂ŁADBF�Ńe�X�g
		// DBF�̃w�b�_�[���E���R�[�h�T�C�Y���l�������33554428���R�[�h��2GB���z����
		// �� 257(�w�b�_��) + n(���R�[�h��) * 64(���R�[�h�T�C�Y) + 1(�I�[�̌Œ�o�C�g) > 2147483647 �ˁ@n = 33554428
		divid_num = 33554428;
		// 2GB���z���邽�߂ɉ���񂷂��v�Z
		long loop_size = (divid_num + cnvData.size()-1)/cnvData.size();
		long total_records = 0;
		for (long i = 0; i < loop_size; ++i) {
#endif

		// Shape�ϊ�
		if (! makeSHP.InsertRecord(cnvData)){
			LogSys::GetInstance().WriteProcLog(true, false,_T("NG\n"));
			ret = false;
			continue;
		}
		LogSys::GetInstance().WriteProcLog(true, false, _T("OK\n"));

#ifdef _DEBUG_DIV
		total_records += cnvData.size();
		}
		CString outputTotalRecords;
		outputTotalRecords.Format(_T("���v�ϊ����R�[�h:%d\n"), total_records);
		LogSys::GetInstance().WriteProcLog(true, false, outputTotalRecords);
#endif
	}
	// Shape��1�t�@�C���݂̂̏ꍇ�̃��l�[������
	if (! makeSHP.ReName(m_args.m_outputDir.c_str()))
		return false;
	return ret;
}