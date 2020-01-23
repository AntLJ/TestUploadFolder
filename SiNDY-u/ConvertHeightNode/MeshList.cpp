#include "stdafx.h"
#include "MeshList.h"

const long MINBASEMESH = 100000;
const long MAXBASEMESH = 999999;

MeshList::MeshList(void)
{
}


MeshList::~MeshList(void)
{
}

bool MeshList::SetMeshList(LPCTSTR meshListPath)
{
	CString errMsg;
	// �R���e�i������
	m_meshes.clear();
	// �t�@�C���I�[�v��
	std::ifstream ifs(meshListPath);
	if (! ifs.is_open()) {
		errMsg.Format(_T("%s�̓ǂݍ��݂Ɏ��s���܂���"), meshListPath);
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, errMsg);
		return false;
	}
	// ���b�V���擾
	std::string secondMesh;
	while (std::getline(ifs, secondMesh)) {
		CString record = CA2CT(secondMesh.c_str());
		record.Trim();
		if (record.IsEmpty())
			continue;
		// ���l�ϊ�
		long mesh = _ttol(record);
		// �͈̓`�F�b�N(�ׂ����`�F�b�N�͂ł��Ȃ��̂�6�����Ă��Ƃ���)
		if (MINBASEMESH > mesh || MAXBASEMESH < mesh) {
			errMsg.Format(_T("���b�V���R�[�h���s���ł��B�F%d"), mesh);
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, errMsg);
			return false;
		}
		if (! m_meshes.insert(mesh).second) {
			errMsg.Format(_T("���b�V���R�[�h�擾�Ɏ��s���܂����B:%d"), mesh);
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, errMsg);
			return false;
		}
	}
	return true;
}

