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

#pragma once

namespace
{
	/**
	 * @brief �������
	 */
	enum Height : int
	{
		higher = 2,  //<! �n�ʂ�肳��ɍ��� 
		high   = 1,  //<! �n�ʂ�荂��
		ground = 0,  //<! �n�ʂƓ���
		low	= -1, //<! �n�ʂ��Ⴂ
		loweer = -2, //<! �n�ʂ�肳��ɒႢ
	};
}

/**
 * @brief  ���������Ǘ�����N���X
 */
class HeightInfoMgr
{
public:
	/**
	 * @brief  �R���X�g���N�^
	 */
	HeightInfoMgr(void){}

	/**
	 * @brief  �R���X�g���N�^
	 */
	virtual ~HeightInfoMgr(void){}

	/**
	 * @brief   ��������ǂݍ���
	 * @param  path  [in]  ��������`�t�@�C���̃p�X
	 * @return true:����
	 */
	bool read (const uh::tstring& path );

	/**
	 * @brief  ���s�҃����N�̍�������Ԃ�
	 * @param  attr  [in]  ���s�҃����N�̑���
	 * @return�@���s�҃����N�̍������
	 */
	int getWalkHeight( long attr ) const;

	/**
	 * @brief  ���H�����N�̍�������Ԃ�
	 * @param  attr  [in]  ���H�����N�̑���
	 * @return ���H�����N�̍������
	 */
	int getRoadHeight( long attr ) const;

private:
	/// ���s�҃����N�̒Ⴂ�����Q
	std::set<long> m_walkLowAttr;
	/// ���s�҃����N�̍��������Q
	std::set<long> m_walkHighAttr;
	/// ���H�����N�̒Ⴂ�����Q
	std::set<long> m_roadLowAttr;
	/// ���H�����N�̍��������Q
	std::set<long> m_roadHighAttr;
};
