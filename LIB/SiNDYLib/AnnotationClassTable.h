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
 * @file AnnotationClassTable.h
 * @brief CAnnotationClassTable�N���X��`�t�@�C��
 * @author �n�}�c�a���암�J���O���[�v �Ð�M�G
 * $Id$
 */
#ifndef _ANNOTATIONCLASSTABLE_H_
#define _ANNOTATIONCLASSTABLE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Table.h"
#include "AnnotationClass.h"
#include <list>

namespace sindy {
namespace annotation_class {

extern double g_dGetRowByAnnoCodeTime;	//!< GetRowByAnnoCode�ɂ����鑍����

/**
 * @class CAnnotationClassTable
 * @brief <b>���L���[���e�[�u���N���X</b>\n
 * ���L���[���e�[�u���iANNOTATION_CLASS�j�̃��b�p�[�N���X�ł��B
 * �ҏW���ɂ����钍�L���[���̓K�p�A�y�у��[���ҏW�̃T�|�[�g�����܂��B
 */
#pragma warning(push)
#pragma warning(disable: 4251)
class SINDYLIB_API CAnnotationClassTable : public CTable  
{
public:
	CAnnotationClassTable() : m_bCached(false) {}
	virtual ~CAnnotationClassTable() {}

	CAnnotationClassTable& operator=(ITable* lp)
	{
		AtlComPtrAssign((IUnknown**)&p, lp);
		return *this;
	}
	CAnnotationClassTable& operator=(const CAnnotationClassTable& lp )
	{
		AtlComPtrAssign((IUnknown**)&p, (ITable*)lp );
		return *this;
	}
	/**
	 * @brief <b>���L��ʂŌ������s��</b>
	 * �L���b�V������Ă�����̂��猟�����s���܂��B
	 * @param emTableType	[in]	�e�[�u���^�C�v
	 * @param emCode		[in]	���L���
	 * @return �Y�����R�[�h���Ȃ��ꍇ��NULL��Ԃ�
	 */
	const CAnnotationClass* GetRowByAnnoCode( sindyTableType::ECode emTableType, schema::annotation::annotation_code::ECode emCode ) const;

	/**
	 * @brief <b>�e�[�u����S�ăL���b�V������</b>\n
	 * �e�[�u���̓��e��S�ăL���b�V�����܂��B
	 * bDetatch��true�ɂ���ƁA�ǂݍ��񂾃��R�[�h�y��
	 * �e�[�u����DB�ւ̐ڑ���؂藣���܂��B
	 */
	long CreateTableCache( bool bDetatch = false );

	/**
	 * @brief <b>�L���b�V�����N���A����</b>\n
	 */
	void ClearTableCache();

	/**
	 * @brief <b>�ڑ����Ă���e�[�u����؂藣��</b>\n
	 * �L���b�V�����쐬������Ƀ��[��DB�ւ̐ڑ���؂�
	 * �����ꍇ�Ɏg�p���Ă��������B
	 */
	//void DetatchTable(){ Release(); }

	/**
	 * @brief <b>���ɃL���b�V������Ă��邩�ǂ���</b>\n
	 * @return �L���b�V������Ă����true
	 */
	bool IsCached() const { return m_bCached; }

	//@{ @name �L���b�V���R���e�i�Q�ƌn��`
	typedef std::list<CAnnotationClass>::const_iterator const_iterator;
	const_iterator begin() const { return m_listCache.begin(); }
	const_iterator end() const { return m_listCache.end(); }
	//@}
private:
	std::list<CAnnotationClass>	m_listCache;	//!< �L���b�V���p���X�g
	bool						m_bCached;		//!< �S���R�[�h���L���b�V���������ǂ����̃t���O
};
#pragma warning(pop)

} // annotation_class

} // sindy

#endif // _ANNOTATIONCLASSTABLE_H_
