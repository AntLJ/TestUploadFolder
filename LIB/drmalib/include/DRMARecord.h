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

// DRMARecord.h: DRMARecord �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRMARECORD_H__7E2A08D7_1F8F_4078_9805_6839D7512A95__INCLUDED_)
#define AFX_DRMARECORD_H__7E2A08D7_1F8F_4078_9805_6839D7512A95__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DRMAOriginalValues.h"
#include "DRMAValues.h"

/**
 * @brief DRMA���ڕϊ��x���N���X
 *
 * DRMA�f�[�^�ɑ��݂���ȉ��̃f�[�^���A����ʓI�Ȍ`���̃f�[�^�ɕϊ�����֐��̏W����:
 *	- EBCDIC10�i��
 *		- �����l�ɕϊ�	
 *	- EBCDIC16�i��
 *		- �����l�ɕϊ�
 *	- EBCDIC���p�J�i������
 *		- (Win32)ASCII���p�J�i������ɕϊ�
 *	- JIS�S�p������
 *		- (Win32)SJIS�S�p������ɕϊ�
 */
class DRMAItemConverter 
{
public:
// DRMA�f�[�^�ϊ�
	static bool setValueTo(const _DRMADec* cOriginalItem, int cSize, int& cItem);
	static bool setValueTo(const _DRMAHex* cOriginalItem, int cSize, int& cItem);
	static bool setValueTo(const _DRMAKanji* cOriginalItem, int cSize, char* cItem);
	static bool setValueTo(const _DRMAKana* cOriginalItem, int cSize, char* cItem);

// ��ʃf�[�^�ϊ�
	static WideChar jisToSjis(const WideChar& cJis);
};

/**
 * @brief DRMA���ڕϊ��x���N���X�g��
 *
 * PREPARE_SET_VALUE�A�y��SET_VALUE�}�N����p���āA�^�ϊ����s���₷������e���v���[�g�N���X
 */
template<typename TFormat>
class DRMAItemConverterTemplate : protected DRMAItemConverter
{
public:
	typedef TFormat Format; ///< �����R�[�h�̃t�H�[�}�b�g

	/**
	 * @brief ���ڒǉ����[�v
	 *
	 * @param cOriginalList [in] �ϊ������ڃ��X�g
	 * @param cList [in] �ǉ��捀�ڃ��X�g�istd::vector�݊��N���X�j
	 * @param cItemCount [in] �����ڐ�
	 * @param cLoopCount [in] �P���R�[�h�ӂ�̍��ڐ�
	 */
	template<typename TOriginalList, typename TList, typename TCount>																 
	static void appendLoop(const TOriginalList& cOriginalList, TList& cList, TCount& cItemCount, int cLoopCount)
	{
		for(int i = 0; i < cLoopCount && (int)cList.size() < cItemCount; i++) {
			cList.push_back(TList::value_type(cOriginalList[i]));
		}
	}
};

/**
 * @brief DRMA���R�[�h�N���X
 */
class DRMARecord
{
public:
	int eRecordId; ///< ���R�[�hID
};

template<typename TFormat>
class DRMARecordT : public DRMARecord, public DRMAItemConverterTemplate<TFormat>
{
};

/// �l�ݒ�ȗ����}�N��
#define PREPARE_SET_VALUE(original) const Format& _rOriginal = original
#define SET_VALUE(item) setValueTo(_rOriginal.item, sizeof(_rOriginal.item), item)
#define SET_VALUE_FROM(original, item) setValueTo(original.item, sizeof(original.item), item)
#define SET_VALUE_FROM_TO(original, item) setValueTo(original, sizeof(original), item)

#define APPEND_LOOP(_list, _itemcount, _loopcount) appendLoop(_rOriginal._list, _list, _itemcount, _loopcount)
#define APPEND_LOOP_FROM(_original, _list, _itemcount, _loopcount) appendLoop(_original._list, _list, _itemcount, _loopcount)

#endif // !defined(AFX_DRMARECORD_H__7E2A08D7_1F8F_4078_9805_6839D7512A95__INCLUDED_)
