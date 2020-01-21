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

#ifndef ATL2_SEAL_H_
#define ATL2_SEAL_H_

namespace atl2 {

/**
 * @brief �A�h���X���Z�q�̃I�[�o�[���[�h�𖳌��������^��񋟂���N���X�B
 *
 * @par CAdapt�Ƃ̑���_:
 * - �R���X�g���N�^���e���v���[�g�����Ă��邽�߁A�l�̑���Ɋւ��Ă�荂���_���L����B�i���N���X��explicit�錾�͌���j
 * - operator&() �ȊO�A���N���X�̃��\�b�h�����̂܂܎g�p�ł���B
 */
template <typename Base>
class seal : public Base
{
public:
	/**
	 * @brief �f�t�H���g�R���X�g���N�^�B
	 */
	seal() {}

	/**
	 * @brief ���N���X��explicit�錾���l�������P���R���X�g���N�^�B
	 */
	template <typename T>
	seal(const T& rT) : Base(static_cast<const Base&>(rT)) {}

	/// @name �A�h���X���Z�q�I�[�o�[���[�h�̕���B
	//@{
	seal* operator&() { return this; }
	const seal* operator&() const { return this; }
	//@}
};

} // namespace atl2

#endif // ATL2_SEAL_H_
