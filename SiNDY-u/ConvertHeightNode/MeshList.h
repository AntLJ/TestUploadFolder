#pragma once
#include "LogSys.h"


/**
 *	@brief	���b�V�����X�g���N���X
 */
class MeshList
{
public:
	MeshList(void);
	~MeshList(void);
	/*
	 *	@brief	���b�V�����X�g�擾
	 *	@param	meshListPath [in] ���b�V�����X�g�t���p�X
	 *	@retval	true ����
	 *	@retval	false ���s
	 */
	bool SetMeshList(LPCTSTR meshListPath);

	/*
	 *	@brief	���b�V���R���e�i�̐擪���w���C�e���[�^�擾(const_iterator)
	 *	@retval �R���X�g�C�e���[�^
	 */
	std::set<long>::const_iterator begin()
	{
		return m_meshes.begin();
	}

	/*
	 *	@brief	���b�V���R���e�i�̏I�[���w���C�e���[�^�擾(const_iterator)
	 *	@retval �R���X�g�C�e���[�^
	 */
	std::set<long>::const_iterator end()
	{
		return m_meshes.end();
	}


private:
	std::set<long> m_meshes;

};

