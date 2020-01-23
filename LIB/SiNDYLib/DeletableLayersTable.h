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
 * @file DeletableLayersTable.h
 * @brief <b>CDeletableLayersTable�N���X��`�t�@�C��</b>\n
 * @author �n�}�c�a���암�J���O���[�v �Ð�M�G
 * $Id$
 */
#ifndef _DELETABLELAYERSTABLE_H_
#define _DELETABLELAYERSTABLE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Table.h"
#include "DeletableLayers.h"

class CListString;
namespace sindy {
namespace deletablelayers {
/**
 * @class CDeletableLayersTable
 * @brief <b>�폜�\���C���Ǘ��e�[�u���N���X�i���[���p�j</b>\n
 * ���̃N���X��REFERENCE.DELETABLELAYERS�e�[�u���̃��b�v�N���X�ł��B
 */
#pragma warning(push)
#pragma warning(disable: 4251)
class SINDYLIB_API CDeletableLayersTable : public CTable  
{
public:
	CDeletableLayersTable() : m_bCached(false) {}
	virtual ~CDeletableLayersTable(){}

	CDeletableLayersTable& operator=(ITable* lp)
	{
		AtlComPtrAssign((IUnknown**)&p, lp);
		return *this;
	}
	CDeletableLayersTable& operator=(const CDeletableLayersTable& lp )
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
	const CDeletableLayers* Find( long lPurposeID, long lLayerNameID ) const;

	/**
	 * @brief <b>�폜�\���C���̈ꗗ��Ԃ�</b>\n
	 * �i�[�R���e�i�̓N���A���܂���̂ŁA�����̃A�C�e��������ꍇ�A���̂܂�
	 * �ǉ�����܂��B�܂��A�d�����l�����܂���̂ŁA�l������ꍇ�� sort�Aunique
	 * ���Ă��������B
	 * @param lPurposeID	[in]		��ƖړIID
	 * @param listLayerName	[out]		�폜�\���C�����X�g
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode GetDeletableLayers( long lPurposeID, CListString& listLayerName ) const;

	/**
	 * @brief <b>�쐬�\���C�����ǂ����`�F�b�N����</b>\n
	 * @warning ���̊֐��ł́A�����ɂ�鐧���̓`�F�b�N�ΏۊO�ł��B
	 * �ʏ��IsDeletable���g�p���Ă��������B
	 * @param lPurposeID	[in]	��ƖړIID
	 * @param lLayerNameID	[in]	�`�F�b�N�Ώۂ̃��C��ID
	 * @retval true �폜�\
	 * @retval false �폜�s�\
	 */
	bool IsDeletableLayer( long lPurposeID, long lLayerNameID ) const { return ( Find( lPurposeID, lLayerNameID ) ) ? true : false; }

	/**
	 * @brief <b>�n���ꂽ�I�u�W�F�N�g���폜�\���ǂ������`�F�b�N����</b>\n
	 * @param lPurposeID	[in]	��ƖړIID
	 * @param lLayerNameID	[in]	���C����ID
	 * @param cRow			[in]	�`�F�b�N�ΏۃI�u�W�F�N�g
	 * @param nLen			[in]	lpszMessage���[�\������
	 * @param lpszMessage	[out]	�`�F�b�N���ʃ��b�Z�[�W
	 * @return sindyeEditableType
	 */
	sindyeEditableType IsDeletable( long lPurposeID, long lLayerNameID, const CRow& cRow, int nLen, LPTSTR lpszMessage ) const;

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
	std::map<long, std::map<long, CDeletableLayers> >	m_mapCache;	//!< �L���b�V���p���X�g
	bool												m_bCached;	//!< �S���R�[�h���L���b�V���������ǂ����̃t���O
};
#pragma warning(pop)
} // deletablelayers

} // sindy

#endif // _DELETABLELAYERSTABLE_H_
