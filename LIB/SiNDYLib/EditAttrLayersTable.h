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
 * @file EditAttrLayersTable.h
 * @brief <b>CEditAttrLayersTable�N���X��`�t�@�C��</b>\n
 * @author �n�}�c�a���암�J���O���[�v �Ð�M�G
 * $Id$
 */
#ifndef _EDITATTRLAYERSTABLE_H_
#define _EDITATTRLAYERSTABLE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Table.h"
#include "EditAttrLayers.h"
#include <map>

class CListString;
namespace sindy {
namespace editattrlayers {
/**
 * @class CEditAttrLayersTable
 * @brief <b>�����ҏW�\���C���e�[�u���N���X�i���[���p�j</b>\n
 * ���̃N���X�́AREFERENCE.EDITATTRLAYERS�e�[�u���̃��b�v�N���X�ł��B
 */
#pragma warning(push)
#pragma warning(disable: 4251)
class SINDYLIB_API CEditAttrLayersTable : public CTable  
{
public:
	CEditAttrLayersTable() : m_bCached(false) {}
	virtual ~CEditAttrLayersTable() {}

	CEditAttrLayersTable& operator=(ITable* lp)
	{
		AtlComPtrAssign((IUnknown**)&p, lp);
		return *this;
	}
	CEditAttrLayersTable& operator=(const CEditAttrLayersTable& lp )
	{
		AtlComPtrAssign((IUnknown**)&p, (ITable*)lp );
		return *this;
	}
	/**
	 * @brief <b>��ƖړI�ԍ��ƃ��C�����ɑΉ����郌�R�[�h��Ԃ�</b>
	 * �L���b�V������Ă�����̂��猟�����s���܂��B
	 * @param lPurposeID	[in]	��ƖړIID
	 * @param lLayerNameID	[in]	���C����ID
	 * @return �Y�����R�[�h���Ȃ��ꍇ��NULL��Ԃ�
	 */
	const CEditAttrLayers* Find( long lPurposeID, long lLayerNameID ) const;

	/**
	 * @brief <b>�����ҏW�\���C���̈ꗗ��Ԃ�</b>\n
	 * �i�[�R���e�i�̓N���A���܂���̂ŁA�����̃A�C�e��������ꍇ�A���̂܂�
	 * �ǉ�����܂��B�܂��A�d�����l�����܂���̂ŁA�l������ꍇ�� sort�Aunique
	 * ���Ă��������B
	 * @param lPurposeID	[in]		��ƖړIID
	 * @param listLayerName	[out]		�����ҏW�\���C�����X�g
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode GetEditAttrLayers( long lPurposeID, CListString& listLayerName ) const;

	/**
	 * @brief <b>�����ҏW�\���C�����ǂ����`�F�b�N����</b>\n
	 * @warning ���̊֐��ł́A�����ɂ�鐧���̓`�F�b�N�ΏۊO�ł��B
	 * �ʏ��IsAttributeEditable���g�p���Ă��������B
	 * @param lPurposeID	[in]	��ƖړIID
	 * @param lLayerNameID	[in]	�`�F�b�N�Ώۂ̃��C��ID
	 * @retval true �����ҏW�\
	 * @retval false �����ҏW�s�\
	 */
	bool IsEditAttrLayer( long lPurposeID, long lLayerNameID ) const { return ( Find( lPurposeID, lLayerNameID ) ) ? true : false; }

	/**
	 * @brief <b>�n���ꂽ�I�u�W�F�N�g�������ҏW�\���ǂ������`�F�b�N����</b>\n
	 * @param lPurposeID	[in]	��ƖړIID
	 * @param lLayerNameID	[in]	���C����ID
	 * @param cRow			[in]	�`�F�b�N�ΏۃI�u�W�F�N�g
	 * @param nLen			[in]	lpszMessage���[�\������
	 * @param lpszMessage	[out]	�`�F�b�N���ʃ��b�Z�[�W
	 * @return sindyeEditableType
	 */
	sindyeEditableType IsAttributeEditable( long lPurposeID, long lLayerNameID, const CRow& cRow, int nLen, LPTSTR lpszMessage ) const;

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
private:
	std::map<long, std::map<long, CEditAttrLayers> >	m_mapCache;	//!< �L���b�V���p���X�g
	bool												m_bCached;	//!< �S���R�[�h���L���b�V���������ǂ����̃t���O
};
#pragma warning(pop)
} // editattrlayers

} // sindy

#endif // _EDITATTRLAYERSTABLE_H_
