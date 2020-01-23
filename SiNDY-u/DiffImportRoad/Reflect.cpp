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

#include "StdAfx.h"
#include "Reflect.h"
#include "Import.h"
#include "MeshMerge.h"
#include "SeparatePolygon.h"

namespace Reflect{
	//�����̓w�b�_�Q��
	const long NO_CHG_MODE = 0x1;
	const long TDC_MODE = 0x02;

	long g_Mode = 0;
	long g_CurrentMesh = -1;
	bool g_IsContinue = false;
	MESH_SET g_MeshSet;
	long g_NowProccess = -1;
	CString g_Operator;
	CString g_FixUser;

} //namespace Reflect

using namespace Reflect;

// �R���X�g���N�^
CReflect::CReflect(void)
: m_OldPath(_T(""))
, m_NewPath(_T(""))
{
}

// �f�X�g���N�^
CReflect::~CReflect(void)
{
	g_MeshSet.clear();
}

// �������֐��i�ŏ��̈�񂾂��ł������́j
bool CReflect::init_once(const CString cConnectStr[2], LPCTSTR cOldPath, LPCTSTR cNewPath, LPCTSTR cMeshList, long cMode)
{
	g_Mode = cMode;
	m_ConnectStr[0] = cConnectStr[0];
	m_ConnectStr[1] = cConnectStr[1];
	m_OldPath = cOldPath;
	m_NewPath = cNewPath;
	if(!fnFirstOnlyInit(cMeshList)){
		return false;
	}
	return true;
}

// �������֐��i���f���Ă��A�����J�n�O�ɕK���Ă΂��j
bool CReflect::init(void)
{
	//�ڑ��ƃt�B�[�`���N���X�ނ̏���
	if(!m_LayerManage.init(m_ConnectStr)){
		return false;
	}
	return true;
}

// �ŏ���1��̂ݎ��s����΂悢�������֐�
bool CReflect::fnFirstOnlyInit(LPCTSTR cMeshList)
{
#ifdef _DEBUG
	g_Operator = _T("for_debug");
	g_FixUser = _T("FIX201301");
#else
	LPCTSTR aOperator = _tgetenv(_T("OPERATOR"));
	if(aOperator != NULL){
		g_Operator = aOperator;
	}else{
		_ftprintf(stderr, _T("#���ϐ��uOPERATOR�v���Z�b�g����Ă��܂���\n"));
		_tprintf(_T("���ϐ��uOPERATOR�v���Z�b�g����Ă��܂���\n"));
		return false;
	}
	LPCTSTR aFixUser = _tgetenv(_T("FIX_USER"));
	if(aFixUser != NULL){
		g_FixUser = aFixUser;
	}else{
		_ftprintf(stderr, _T("#���ϐ��uFIX_USER�v���Z�b�g����Ă��܂���\n"));
		_tprintf(_T("���ϐ��uFIX_USER�v���Z�b�g����Ă��܂���\n"));
		return false;
	}
#endif

	if(_taccess(m_OldPath, 0x04) != 0){
		_ftprintf(stderr, _T("#ERROR �ҏW�O�f�[�^�̃p�X�����݂��Ȃ����A�N�Z�X�ł��܂���,%s\n"), m_OldPath);
		_tprintf( _T("ERROR �ҏW�O�f�[�^�̃p�X�����݂��Ȃ����A�N�Z�X�ł��܂���,%s\n"), m_OldPath);
		return false;
	}
	if(_taccess(m_NewPath, 0x04) != 0){
		_ftprintf(stderr, _T("#ERROR �ҏW��f�[�^�̃p�X�����݂��Ȃ����A�N�Z�X�ł��܂���,%s\n"), m_NewPath);
		_tprintf( _T("ERROR �ҏW��f�[�^�̃p�X�����݂��Ȃ����A�N�Z�X�ł��܂���,%s\n"), m_NewPath);
		return false;
	}
	if(m_OldPath == m_NewPath){
		_ftprintf(stderr, _T("#ERROR �ҏW�O�̃f�[�^�p�X�ƕҏW��f�[�^�̃p�X�������ł�,%s,%s\n"), m_OldPath, m_NewPath);
		_tprintf(_T("ERROR �ҏW�O�̃f�[�^�p�X�ƕҏW��f�[�^�̃p�X�������ł�,%s,%s\n"), m_OldPath, m_NewPath);
		return false;
	}

	//���b�V�����X�g�ǂݍ���
	if(!fnMeshLoad(cMeshList)){
		return false;
	}
	g_NowProccess = 0;
	return true;
}


/*
	�ҏW�������f��ƃ��C���֐��B���́i�ȑO�͌ʂ������j�@�\���g�ݍ��܂�Ă���
	�E������荞��
	�E�s�v�\���_�폜
	�E���b�V�����E�}�[�W
	�E�}���`�p�[�g�|���S������
	�E�s�v�\���_�폜
*/
bool CReflect::execReflect(void)
{
	//�����C���|�[�g
	if(g_NowProccess == 0){
		CImport aImport(&m_LayerManage);
		if(aImport.init(m_OldPath, m_NewPath)){
			if(!aImport.executeALL(5)){
				return false;
			}
		}
	}

	//�s�v�\���_�폜+���b�V�����E�}�[�W
	if(g_NowProccess == 1){
		CMeshMerge aMeshMerge(&m_LayerManage);
		if(aMeshMerge.init()){
			//�|���S�������鎞�������s
			if(!aMeshMerge.executeALL()){
				return false;
			}
		}
	}

	////�}���`�p�[�g�|���S������
	if(g_NowProccess == 2){
		CSeparatePolygon aSeparatePolygon(&m_LayerManage);
		if(aSeparatePolygon.init()){
			//�|���S�������鎞�������s
			if(!aSeparatePolygon.executeALL()){
				return false;
			}
		}
	}
	m_LayerManage.unInit();	//�S���j��(�ؒf)
	return g_IsContinue;
}

// ���b�V�����X�g�̓ǂݍ��݊֐�
bool CReflect::fnMeshLoad(LPCTSTR cMeshList)
{
	if(g_MeshSet.size() == 0){
		FILE *pFile;
		if((pFile = _tfopen(cMeshList, _T("rt"))) != NULL){
			char aBuf[16];
			while(fgets(aBuf, 16, pFile) != NULL){
				if(aBuf[0] == '#'){
					continue;
				}
				long aMesh = atol(aBuf);
				g_MeshSet.insert(aMesh);
			}
			fclose(pFile);
		}else{
			_ftprintf(stderr, _T("#ERROR,���X�g�t�@�C�����J���܂���,%s\n"), cMeshList);
			_tprintf(_T("ERROR,���X�g�t�@�C�����J���܂���,%s\n"), cMeshList);
			return false;
		}
	}
	return true;
}

