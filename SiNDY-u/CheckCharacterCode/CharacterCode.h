#include "stdafx.h"

/**
 * @brief �����R�[�h�͈̓N���X
 */
class CharacterCode
{
public:
	/**
	 * @brief �R���X�g���N�^
	 */
	CharacterCode() : CodeRangeMin(0), CodeRangeMax(0){}

public:
	unsigned long CodeRangeMin;		//!< �����R�[�h�͈͎w��̎n�܂�i�R�[�h�����������j
	unsigned long CodeRangeMax;		//!< �����R�[�h�͈͎w��̏I���i�R�[�h���傫�����j
};
