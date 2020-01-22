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
#include <atlctrls.h>


/**
* @brief ���X�g�{�b�N�X�R���g���[���Ǘ��p�N���X
* @note  �u�w���v�Ɓu�H�����v�̃��X�g�{�b�N�X���Ǘ�����
*/
class CSearchListCtrls
{
public:
	/// �R���X�g���N�^
	CSearchListCtrls(void);
	/// �f�X�g���N�^
	~CSearchListCtrls(void);

	/// �R�t�������ێ�����p�\����
	struct JoinLineInfo
	{
		JoinLineInfo( long id, long railway_line, CString station, CString line ):
			oid(id),railway_line_id(railway_line),station_name(station),line_name(line){}

		long oid;               //!< JOIN_LINE_INFO��OBJECTID
		long railway_line_id;   //!< RAILWAY_LINE��OBJECTID�iJOIN_LINE_INFO.LINEID�j
		CString station_name;   //!< �w���iSTATION_INFO.NAME�j
		CString line_name;      //!< �H�����iRAILWAY_LINE.NAME�j
	};


	/**
	* @brief �������֐�
	* @param station_list [in] �w�����X�g�{�b�N�X��HWND
	* @param line_list [in]    �H�������X�g�{�b�N�X��HWND
	*/
	void init( HWND station_list, HWND line_list );
	/**
	* @brief ���X�g�{�b�N�X�̒��g���X�V����
	* @note  ���ӂ̉wPOI�|�C���g����Ԍ������A�֘A����e�[�u����R�t���ĉw���A�H���������X�g�\������
	* @param ipNodeFeature [in] ���ӌ������钆�S�̃m�[�h�i�|�C���g�j
	* @param dRange [in] �����͈́i�P�ʁFm�j
	* @param [in] ��������wPOI�̃t�B�[�`���N���X
	*/
	void reflesh( IFeaturePtr ipNodeFeature, double dRange, IFeatureClassPtr ipPoiClass );
	/**
	* @brief ���X�g�{�b�N�X�̍��ڂ�I������
	* @note �ێ����Ă���ireflesh()�Ō������ꂽ�j���ɕR�Â��w���A�H�������������AUI��őI�����ꂽ��Ԃɂ���
	* @param join_line_info_id [in] �I��Ώۂ̏�����H��ID
	*/
	void select( long join_line_info_id );
	/**
	* @brief UI�őI������Ă���w���ƘH�����ɕR�Â�������H��ID���擾����
	* @return ������H��ID
	*/
	long GetJoinLineInfoID();
	/**
	* @brief �H��ID�ɕR�Â�������H��ID���擾����
	* @param railway_line_id [in] �S���H��ID
	* @return ������H��ID
	*/
	long GetJoinLineInfoIDFromRailWayLine( long railway_line_id );
	/**
	* @brief ������H��ID�ɕR�Â��H��ID���擾����
	* @param join_line_info [in] ������H��ID
	* @return �S���H��ID
	*/
	long GetRailWayLineIDFromJoinLineInfo( long join_line_info );

	/**
	* @brief �w���̑I�����ύX���ꂽ�Ƃ��ɌĂ΂��i�ׂ��j�֐�
	*/
	void StationSelChange();
	/**
	* @brief �H�����̑I�����ύX���ꂽ�Ƃ��ɌĂ΂��i�ׂ��j�֐�
	*/
	void LineSelChange();

public:
	CString selectedStation; //!< �I������Ă���w��
	CString selectedLine;    //!< �I������Ă���H����

private :
	CListBox m_lvStation;    //!< �w�����X�g�{�b�N�X�R���g���[��
	CListBox m_lvLine;       //!< �H�������X�g�{�b�N�X�R���g���[��

	std::map<long, std::list<JoinLineInfo>> m_JoinLineInfos; //!< �wPOI�|�C���g�ɕR�Â��w���A�H�������
};

