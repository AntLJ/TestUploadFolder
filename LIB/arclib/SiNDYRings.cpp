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

#include "stdafx.h"
#include "SiNDYRings.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace sindy;

// �\�z/����
//////////////////////////////////////////////////////////////////////

Rings::Rings() :
m_nCount(0),
m_ppRings(0)
{
}

Rings::Rings(const Rings& rRings) :
m_nCount(0),
m_ppRings(0)
{
	operator=(rRings);
}

Rings::~Rings()
{
	clear();
}

// �ݒ�/�擾
//////////////////////////////////////////////////////////////////////

/**
 * @brief �ێ����Ă���֔z����������
 */
void Rings::clear()
{
	delete [] m_ppRings;
	m_ppRings = 0;
	m_nCount = 0;
}

/**
 * @brief �ێ����Ă���ւ̐����擾����
 */
unsigned int Rings::size()
{
	return m_nCount;
}

/**
 * @brief �֔z�������������
 *
 * @note ��� clear() ���邱�ƁB
 *
 * @param nCount [in] �ւ̐��B
 * @retval ���������ꂽ�z��̐擪�|�C���^�B
 */
IRing** Rings::init(unsigned int nCount)
{
	if(nCount) {
		m_ppRings = new IRingPtr[nCount];
		m_nCount = nCount;
	}

	return operator IRing**();
}

// ���Z�q�̃I�[�o�[���C�h
//////////////////////////////////////////////////////////////////////

Rings::operator IRing**()
{
	return reinterpret_cast<IRing**>(m_ppRings);
}

/**
 * @brief �z�񉉎Z
 *
 * @param n [in] �z��ԍ��B
 */
IRing* Rings::operator[](unsigned int n)
{
	return m_ppRings[n];
}

/**
 * @brief �������Z
 *
 * @param rRings [in] �������֏W�B
 */
Rings& Rings::operator=(const Rings& rRings)
{
	clear();
	init(rRings.m_nCount);
	for(int i = 0; i < m_nCount; i++) {
		m_ppRings[i] = rRings.m_ppRings[i];
		m_ppRings[i]->AddRef();
	}
	return *this;
}

//////////////////////////////////////////////////////////////////////
// ExteriorRings
//////////////////////////////////////////////////////////////////////

ExteriorRings::ExteriorRings()
{
}

ExteriorRings::ExteriorRings(IPolygon2* ipPolygon2)
{
	if(ipPolygon2) {
		setExteriorRings(ipPolygon2);
	}
}

void ExteriorRings::setExteriorRings(IPolygon2* ipPolygon2)
{
	long nCount;
	if(ipPolygon2->get_ExteriorRingCount(&nCount) == S_OK && nCount > 0) {
		ipPolygon2->QueryExteriorRingsEx(nCount, init(nCount));
	}
}

Rings& ExteriorRings::operator=(IPolygon2* ipPolygon2)
{
	clear();
	if(ipPolygon2) {
		setExteriorRings(ipPolygon2);
	}

	return *this;
}

Rings& ExteriorRings::operator=(const ExteriorRings& rRings)
{
	return Rings::operator=(rRings);
}
