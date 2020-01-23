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

#include "stdafx.h"
#include "Arguments.h"
#include <TDC/useful_headers/directory_util.h>
#include <memory>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/optional.hpp>
#include "util.h"

namespace properties {
	/**
	 * @brief �ݒ荀��
	 */
	enum value {
		stationDB,
		roadDB,
		walkDB,
		meshDB,
		poiDB,
		underDB,
		meshList,
		heightNode_ini,
		target,
		day,
		time,
		dist_threshold,
		neighbor_threshold,
		refer_id_threshold,
		angle_threshold,
		buddy_dist_threshold,
		buddy_angle_threshold,
		sindyTolerance_threshold,
		microLogicalConnection
	};
	
	/**
	 * @brief �`�F�b�N�Ώ�
	 */
	enum check_target
	{
		all,
		humancar,
		walkable,
		height_node,
		link_relation,
		under,
		other,
		release,
		pedxing,
		error,
	};

} // properties_value

/**
 * @class	CProperties
 * @brief	�ݒ�l�Ǘ��N���X
 */
class CProperties
{
private:
	boost::property_tree::ptree m_proper_tree; //!< �ݒ�t�@�C���̓��e�̊i�[��

public:
	CProperties(){}
	virtual ~CProperties(){}

	/**
	 * @brief	�ݒ�t�@�C���ǂݍ���
	 * @param	fileName	[in]	�ݒ�t�@�C���p�X
	 * @param	dataType	[in]	�f�[�^�^�C�v
	 * @retval	true	����
	 * @retval	false	���s
	 */
	bool Read(const CString& fileName, const CString& dataType);

	/**
	 * @brief	�ݒ�l�擾
	 * @param	propValue	[in]	�擾�������ݒ�l�̃^�O��Enum�^
	 * @retval	�ݒ�l�iCString�j
	 */
	CString GetValue(const properties::value propValue) const ;

	/**
	 * @brief	�ݒ�l�擾
	 * @param	propValue	[in]	�擾�������ݒ�l�̃^�O��Enum�^
	 * @retval	�ݒ�l�idouble�j
	 */
	double GetValueDouble(const properties::value propValue) const;

	/**
	 * @brief	�ݒ�l�擾
	 * @param	propValue	[in]	�擾�������ݒ�l�̃^�O��Enum�^
	 * @retval	�ݒ�l�i�`�F�b�N�^�C�v��Enum�^�j
	 */
	properties::check_target GetValueTarget(const properties::value propValue) const;

	/**
	 * @brief �ݒ�l���琶�������ҏW�����t�B���^���擾
	 * @return �ҏW�����t�B���^
	 */
	CComVariant getModifyFilter() const;

private:

	/**
	 * @brief	�^�O������擾
	 * @param	propValue	[in]	�擾�������^�O��Enum�^
	 * @retval	�^�O������
	 */
	std::string GetTag(const properties::value propValue) const ;

	/**
	 * @brief	�ݒ�l�`�F�b�N
	 * @param	dataType	[in]	�f�[�^�^�C�v
	 * @retval	true	����
	 * @retval	false	���s
	 */
	bool CheckValue(const CString& dataType);

	/**
	 * @brief	�t�@�C���p�X�ݒ�l�`�F�b�N
	 * @param	propValue	[in]	�`�F�b�N����^�O��Enum�^
	 * @retval	true	����
	 * @retval	false	���s
	 */
	bool CheckFileValue(const properties::value propValue);

	/**
	 * @brief	�f�B���N�g���p�X�ݒ�l�`�F�b�N
	 * @param	propValue	[in]	�`�F�b�N����^�O��Enum�^
	 * @retval	true	����
	 * @retval	false	���s
	 */
	bool CheckDirValue(const properties::value propValue);

	/**
	 * @brief	������ݒ�l�`�F�b�N
	 * @param	propValue	[in]	�`�F�b�N����^�O��Enum�^
	 * @retval	true	����
	 * @retval	false	���s
	 */
	bool CheckStringValue(const properties::value propValue);

	/**
	 * @brief	�`�F�b�N�^�C�v�ݒ�l�`�F�b�N
	 * @param	propValue	[in]	�`�F�b�N����^�O��Enum�^
	 * @retval	true	����
	 * @retval	false	���s
	 */
	bool CheckTargetValue(const properties::value propValue);

	/**
	 * @brief	�W���G���[�ւ̃��b�Z�[�W�o��
	 * @param	propValue	[in]	�^�O
	 * @param	message	[in]	���b�Z�[�W
	 */
	void OutputStdErr(const properties::value propValue, const CString message);

};
