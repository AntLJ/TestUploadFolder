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
 * @file DispOtherRelationTable.h
 * @brief <b>CDispOtherRelationTable�N���X��`�t�@�C��</b>\n
 * @author �n�}�c�a���암�J���O���[�v �Ð�M�G
 * $Id$
 */
#ifndef _DISPOTHERRELATIONTABLE_H_
#define _DISPOTHERRELATIONTABLE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Table.h"
#include "DispOtherRelation.h"

namespace sindy {
namespace dispotherrelation {
/**
 * @class CDispOtherRelationTable
 * @brief <b>���H�����N�̕\����ʂƂ��̑��̑����̑Ή��Ǘ��e�[�u���N���X�i���[���p�j</b>\n
 * ���̃N���X�́AREFERENCE.DISPOTHERRELATION�e�[�u���̃��b�v�N���X�ł��B
 */
#pragma warning(push)
#pragma warning(disable: 4251)
class SINDYLIB_API CDispOtherRelationTable : public CTable  
{
public:
	CDispOtherRelationTable() : m_bCached(false) {}
	virtual ~CDispOtherRelationTable() {}

	CDispOtherRelationTable& operator=(ITable* lp)
	{
		AtlComPtrAssign((IUnknown**)&p, lp);
		return *this;
	}
	CDispOtherRelationTable& operator=(const CDispOtherRelationTable& lp )
	{
		AtlComPtrAssign((IUnknown**)&p, (ITable*)lp );
		return *this;
	}
	/**
	 * @brief <b>���H�����N�̓��H��ʖ�ID�Ɠ��H�����N�̂��̑��̑�����ID�ɑΉ����郌�R�[�h��Ԃ�</b>
	 * �L���b�V������Ă�����̂��猟�����s���܂��B
	 * @param lDispClassID	[in]	���H�����N�̓��H��ʖ�ID
	 * @param lOtherAttrID	[in]	���H�����N�̂��̑��̑�����ID
	 * @return �Y�����R�[�h���Ȃ��ꍇ��NULL��Ԃ�
	 */
	const CDispOtherRelation* Find( long lDispClassID, long lOtherAttrID ) const;

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
	 * @brief <b>���ɃL���b�V������Ă��邩�ǂ���</b>\n
	 * @return �L���b�V������Ă����true
	 */
	bool IsCached() const { return m_bCached; }
private:
	std::map<long, std::map<long, CDispOtherRelation> >	m_mapCache;	//!< �L���b�V���p���X�g
	bool												m_bCached;	//!< �S���R�[�h���L���b�V���������ǂ����̃t���O
};
#pragma warning(pop)
} // dispotherrelation

} // sindy

#endif // _DISPOTHERRELATIONTABLE_H_
