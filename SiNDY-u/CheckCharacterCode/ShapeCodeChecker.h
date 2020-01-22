#pragma once

#include "codechecker.h"
#include <libshape.h>
#include <DBFHandle.h>

/**
 * @brief �����R�[�h�`�F�b�NShape�p�N���X
 */
class ShapeCodeChecker : public CodeChecker
{
public:
	/**
	 * @brief �R���X�g���N�^
	 */
	ShapeCodeChecker(void){};

	/**
	 * @brief �f�X�g���N�^
	 */
	~ShapeCodeChecker(void){};

	/**
	 * @brief �����R�[�h�`�F�b�N�Ώۃt�@�C����ǂݍ���ŕ����R�[�h�`�F�b�N���s���֐�
	 * @param isImmediateExit [in] �s�����o�������I���t���O
	 * @retval true ��������
	 * @retval false �������s
	 */
	bool CheckCharacterCode(bool isImmediateExit);
};
