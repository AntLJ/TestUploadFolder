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


namespace sindy{

// �����N���Έʒu
enum LinkRelationPosition
{
	kNotFound = 0,	// ������Ȃ�
	kRight = 1,		// �E
	kLeft  = 2		// ��
};


/**
 * @brief  ��̃|�����C�����猩�āA�^�[�Q�b�g�̃|�����C�������E�ǂ���Ɉʒu���邩�𔻒肷��
 * @param [in]    ipBasePolyline     ���E�𔻒肷�鑤(�x�[�X��)�̃|�����C��
 * @param [in]    ipTargetPolyline   ���E�𔻒肳��鑤(�^�[�Q�b�g��)�̃|�����C��
 * @param [in]    dSearchMeter       ��������(���[�g��)
 * @param [out]   eRelationInfo      ���E�ǂ���Ɉʒu���邩(1:�E, 2:��, 0:������Ȃ�)
 * @param [out]	  dMinMeter	 ���苗��(dgree) ���E����Ɏg�p���������̒���(�ŏ�����)
 * @retval true   ����Ɍ�������(������Ȃ������ꍇ�� true�� �Ԃ�)
 * @retval false  �������s���Ɏ��s
 * @detail �A���S���Y���͈ȉ����Q�ƂƂ���
 *         http://preon.mr.ipc.pioneer.co.jp/svn/source/trunk/SiNDY-u/mkLinkRelation/doc/�\�t�g�E�F�A�J������(mkLinkRelation).xlsx
 */
bool AheJudgeLinkRelation( const IPolylinePtr& ipBasePolyline,
						   const IPolylinePtr& ipTargetPolyline, 
						   double dSearchMeter,
						   LinkRelationPosition& eRelationInfo,
						   double& dMinMeter );

/**
 * @brief  ��̃|�����C�����猩�āA�^�[�Q�b�g�̃|�����C�������E�ǂ���Ɉʒu���邩�𔻒肷��
 * @note   ���E����Ɏg�p���������̋������K�v�Ȃ��ꍇ�Ɏg�p
 * @param [in]    ipBasePolyline     ���E�𔻒肷�鑤(�x�[�X��)�̃|�����C��
 * @param [in]    ipTargetPolyline   ���E�𔻒肳��鑤(�^�[�Q�b�g��)�̃|�����C��
 * @param [in]    dSearchMeter       ��������(���[�g��)
 * @param [out]   eRelationInfo      ���E�ǂ���Ɉʒu���邩(1:�E, 2:��, 0:������Ȃ�)
 * @retval true   ����Ɍ�������(������Ȃ������ꍇ�� true�� �Ԃ�)
 * @retval false  �������s���Ɏ��s
 */
bool AheJudgeLinkRelation( const IPolylinePtr& ipBasePolyline, const IPolylinePtr& ipTargetPolyline, double dSearchMeter, LinkRelationPosition& eRelationInfo );


/**
 * @brief AheJudgeLinkRelation�̏d�Ȃ蔻���
 * @param [note] �x�[�X�|�����C���Ɍ����������o�b�t�@���|���A�^�[�Q�b�g�|�����C���Ƃ̋��ʕ������擾���A<br>
 *               �擾����������dRoundOffMeter�����ł���΁A���E������ł��Ȃ��������ƂƂ���B<br>
 *				 �� �A���A�����N��������dNoCheckMeter�����̏ꍇ�́AAheJudgeLinkRelation�̌��ʂ����̂܂ܕԂ��B
 * @param [in]    ipBasePolyline     ���E�𔻒肷�鑤(�x�[�X��)�̃|�����C��
 * @param [in]    ipTargetPolyline   ���E�𔻒肳��鑤(�^�[�Q�b�g��)�̃|�����C��
 * @param [in]    dSearchMeter       ��������(���[�g��)
 * @parmm [in]    dRoundOffMeter     �d�Ȃ蔻�苗��(���[�g��)
 * @param [in]    dNoCheckMeter      �d�Ȃ蔻�菜�O����(���[�g��)
 * @param [out]   eRelationInfo      ���E�ǂ���Ɉʒu���邩(1:�E, 2:��, 0:������Ȃ�)
 * @retval true   ����Ɍ�������(������Ȃ������ꍇ�� true�� �Ԃ�)
 * @retval false  �������s���Ɏ��s
 */
bool AheJudgeLinkRelationBuf( const IPolylinePtr& ipBasePolyline, const IPolylinePtr& ipTargetPolyline, double dSearchMeter, double dRoundOffMeter, double dNoCheckMeter, LinkRelationPosition& eRelationInfo );


/**
 * @brief  ��̃|�����C�����猩�āA�^�[�Q�b�g�̃|�����C�������E�ǂ���Ɉʒu���邩�𔻒肷��
 * @param [in]    ipBaseGeo          ���E�𔻒肷�鑤(�x�[�X��)�̃|�����C��
 * @param [in]    ipTargetGeo        ���E�𔻒肳��鑤(�^�[�Q�b�g��)�̃|�����C��
 * @param [out]   nRelationInfo      ���E�ǂ���Ɉʒu���邩(1:�E, 2:��)
 * @retval true   ����Ɍ�������
 * @retval false  �������s���Ɏ��s
 * @detail AheJudgeLinkRelation�ɔ�׊ȈՓI�Ȏ�@�ł���A�K�����E�ǂ��炩�ɔ��肳��郁���b�g�����邪�A���萸�x��������
 *         �A���S���Y���͈ȉ����Q�ƂƂ���
 *         http://preon.mr.ipc.pioneer.co.jp/svn/source/trunk/SiNDY-u/mkLinkRelation/doc/�\�t�g�E�F�A�J������(mkLinkRelation).xlsx
 */
bool AheSimpleJudgeLinkRelation( const IPolylinePtr& ipBasePolyline, const IPolylinePtr& ipTargetPolyline, unsigned int& nRelationInfo );

} // sindy
