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

/**
 * @file WorkPurposeTable.h
 * @brief <b>CWorkPurposeTable�N���X��`�t�@�C��</b>\n
 * @author �n�}�c�a���암�J���O���[�v �Ð�M�G
 * $Id$
 */
#ifndef _WORKPURPOSETABLE_H_
#define _WORKPURPOSETABLE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Table.h"
#include "WorkPurpose.h"

namespace sindy {
namespace workpurpose {
/**
 * @class CWorkPurposeTable
 * @brief <b>��ƖړI�Ǘ��e�[�u���N���X</b>\n
 * ���̃N���X��REFERENCE.WORKPURPOSE�e�[�u���̃��b�v�N���X�ł��B
 */
class SINDYLIB_API CWorkPurposeTable : public CTable  
{
public:
	CWorkPurposeTable() : m_bCached(false) {}
	virtual ~CWorkPurposeTable() {}

	CWorkPurposeTable& operator=(ITable* lp)
	{
		AtlComPtrAssign((IUnknown**)&p, lp);
		return *this;
	}
	CWorkPurposeTable& operator=(const CWorkPurposeTable& lp )
	{
		AtlComPtrAssign((IUnknown**)&p, (ITable*)lp );
		return *this;
	}
	/**
	 * @brief <b>��ƖړI�ԍ��ɑΉ����郌�R�[�h��Ԃ�</b>\n
	 * �L���b�V������Ă�����̂��猟�����s���܂��B
	 * @param lID	[in]	��ƖړI�ԍ�
	 * @return �Y�����R�[�h���Ȃ��ꍇ��NULL��Ԃ�
	 */
	const CWorkPurpose* Find( long lID ) const;

	/**
	 * @brief <b>�e�[�u����S�ăL���b�V������</b>\n
	 * �e�[�u���̓��e��S�ăL���b�V�����܂��B
	 * bDetatch��true�ɂ���ƁA�ǂݍ��񂾃��R�[�h�y��
	 * �e�[�u����DB�ւ̐ڑ���؂藣���܂��B
	 *
	 * @note ����̍�ƖړI�����L���b�V���������ꍇ��lPurposeID�ɍ�ƖړIID���Z�b�g���܂��B
	 * �f�t�H���g�̏�Ԃł͑S��ƖړI���L���b�V�����܂��B
	 * 
	 * @param bDetatch [in,optional] �L���b�V��������ڑ���؂藣���i�f�t�H���g�Ffalse�j
	 * @param lPurposeID [in,optional] ��ƖړI�����肵�ăL���b�V�����쐬����i�f�t�H���g�F-1�j
	 */
	long CreateTableCache( bool bDetatch = false, long lPurposeID = -1 );

	/**
	 * @brief <b>�L���b�V�����N���A����</b>\n
	 */
	void ClearTableCache();

	/**
	 * @brief <b>���ɃL���b�V������Ă��邩�ǂ���</b>\n
	 * @return �L���b�V������Ă����true
	 */
	bool IsCached() const { return m_bCached; }

	//@{ @name �C�e���[�^
	typedef std::map<long,CWorkPurpose>::iterator iterator;
	typedef std::map<long,CWorkPurpose>::const_iterator const_iterator;
	iterator begin() { return m_mapCache.begin(); }
	iterator end() { return m_mapCache.end(); }
	const_iterator begin() const { return m_mapCache.begin(); }
	const_iterator end() const { return m_mapCache.end(); }
	//@}
private:
#pragma warning(push)
#pragma warning(disable: 4251)
	std::map<long,CWorkPurpose>	m_mapCache;		//!< �L���b�V���p���X�g�i�L�[�͔w�i��ʁj
#pragma warning(pop)
	bool						m_bCached;		//!< �S���R�[�h���L���b�V���������ǂ����̃t���O
};

} // workpurpose

} // sindy

#endif // _WORKPURPOSETABLE_H_
