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

#ifndef __SINDY_RINGS_H__
#define __SINDY_RINGS_H__

#include "exportimport.h"

namespace sindy {

/**
 * @brief �֏W�N���X�B
 */
class SINDYLIB_API Rings
{
public:
// �\�z/����
	Rings();
	Rings(const Rings& rRings);
	~Rings();

// �ݒ�/�擾
	void clear();

	unsigned int size();

// ���Z�q�̃I�[�o�[���C�h
	operator IRing**();
	IRing* operator[](unsigned int n);
	Rings& operator=(const Rings& rRings);

protected:
// �ݒ�/�擾
	IRing** init(unsigned int nCount);

private:
// �ϐ�
	long m_nCount; ///< �֐��B
	IRingPtr* m_ppRings; ///< �֔z��B
};

/**
 * @brief �O�֏W�N���X�B
 */
class SINDYLIB_API ExteriorRings : public Rings
{
public:
// �\�z/����
	ExteriorRings();
	ExteriorRings(IPolygon2* ipPolygon2);

// ���Z�q�̃I�[�o�[���C�h
	Rings& operator=(IPolygon2* ipPolygon2);
	Rings& operator=(const ExteriorRings& rRings);

private:
	void setExteriorRings(IPolygon2* ipPolygon2);
};

} // namespace sindy

#endif // __SINDY_RINGS_H__  //
