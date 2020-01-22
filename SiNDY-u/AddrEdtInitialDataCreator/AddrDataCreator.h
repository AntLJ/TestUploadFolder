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
#include "BaseDataCreator.h"

// �o�T�R�[�h�ɑ΂���m�F�X�e�[�^�X�̑Ή��\
const std::map<CString, int> convertMap = boost::assign::map_list_of
	( _T("1"), 1 )
	( _T("2"), 2 )
	( _T("3"), 3 )
	( _T("4"), 3 )
	( _T("5"), 3 )
	( _T("6"), 3 )
	( _T("7"), 3 )
	( _T("8"), 3 )
	( _T("9"), 0 );

/**
 * @brief   �Z���|���S���E�Z���|�C���g�f�[�^�����N���X�i�h���N���X�j
 */
class AddrDataCreator :	public BaseDataCreator
{

public:
	/**
	 * @brief   �R���X�g���N�^
	 * @param   arg [in]  ����
	 */

	AddrDataCreator(const Arguments& arg);

	/**
	 * @brief   �f�X�g���N�^
	 */
	virtual ~AddrDataCreator() {}

	/**
	 * @brief   ������
	 */
	bool init();

	/**
	 * @brief   �f�[�^����
	 */
	bool create();

private:

	/**
	 * @brief   �g�����́E�ǂݐݒ�
	 * @param   kcode [in]  �g���R�[�h
	 * @param   addrPolygonRec [in/out]  �Z���|���S���R���e�i
	 * @param   isKcode1 [in]  �w��g���R�[�h�͊g���R�[�h1���ǂ����itrue:�g���R�[�h1�Afalse�F�g���R�[�h2�j
	 * @retval  true  ����
	 * @retval  false ���s
	 */
	bool setExtcodeInfo(
		const CString& kcode,
		EdtAddrPolygonRec& addrPolygonRec,
		bool isKcode1
	);

	/**
	 * @brief   �Z���|���S�����ݒ�
	 * @param   feature [in]  �Z���|���S���t�B�[�`��
	 * @param   polyRec [out]  �Z���|���S���R���e�i
	 * @retval  true  ����
	 * @retval  false ���s
	 */
	bool setEdtAddrPolygonRec(
		const IFeaturePtr& feature,
		EdtAddrPolygonRec& polyRec
	);

	/**
	 * @brief   �Z���|�C���g���ݒ�
	 * @param   feature [in]  �Z���|�C���g�t�B�[�`��
	 * @param   addrPolygonRec [in]  �Z���|���S���R���e�i
	 * @param   addrPointRec [out]  �Z���|�C���g�R���e�i
	 * @retval  true  ����
	 * @retval  false ���s
	 */
	bool setEdtAddrPointRec(
		const IFeaturePtr& feature,
		const EdtAddrPolygonRec& addrPolygonRec,
		EdtAddrPointRec& addrPointRec
	);

	/**
	 * @brief   �g���R�[�h���X�g���MAP�擾
	 * @param   extcodeListPath [out]  �g���R�[�h���X�g�t�@�C���p�X
	 * @retval  true  ����
	 * @retval  false ���s
	 */
	bool getExtcodeList(
		const CString& extcodeListPath
	);

	/**
	 * @brief   �Z���|���S���f�[�^�擾
	 * @param   addrPolygonRecs [out]  �Z���|���S���R���e�i�Q
	 * @param   citycode [in]  �s�撬���R�[�h
	 * @param   countInfo [in/out]  �����J�E���^���
	 * @retval  true  ����
	 * @retval  false ���s
	 */
	bool getAddrPolygon(
		std::vector<EdtAddrPolygonRec>& addrPolygonRecs,
		const CString& citycode,
		CountInfo& countInfo
	);

	/**
	 * @brief   �Z���|�C���g�f�[�^�擾
	 * @param   addrPolygonRecs [in]  �Z���|���S���R���e�i�Q
	 * @param   addrPointRecs [out]  �Z���|�C���g�R���e�i�Q
	 * @param   countInfo [in/out]  �����J�E���^���
	 * @retval  true  ����
	 * @retval  false ���s
	 */
	bool getAddrPoint(
		const std::vector<EdtAddrPolygonRec>& addrPolygonRecs,
		std::vector<EdtAddrPointRec>& addrPointRecs,
		CountInfo& countInfo
	);


	/**
	 * @brief   �Z���|���S���̋��E��̏Z���|���S����OID���擾
	 * @param   addrPolygonRec [in]  �Z���|���S���R���e�i
	 * @param   touchOIds [out]  ���E��̏Z���|�C���g��OID�Q
	 * @retval  true  ����
	 * @retval  false ���s
	 */
	bool getBoundaryTouchPoints(
		const EdtAddrPolygonRec& addrPolygonRec,
		std::vector<long>& touchOIds
	);

	/**
	 * @brief   �Z���|���S���f�[�^�C���|�[�g
	 * @param   addrPolygonRecs [in]  �Z���|���S���R���e�i�Q
	 * @param   countInfo [in/out]  �����J�E���^���
	 * @retval  true  ����
	 * @retval  false ���s
	 */
	bool insertAddrPolygon(
		const std::vector<EdtAddrPolygonRec>& addrPolygonRecs,
		CountInfo& countInfo
	);

	/**
	 * @brief   �Z���|�C���g�f�[�^�C���|�[�g
	 * @param   addrPointRecs [in]  �Z���|�C���g�R���e�i�Q
	 * @param   countInfo [in/out]  �����J�E���^���
	 * @retval  true  ����
	 * @retval  false ���s
	 */
	bool insertAddrPoint(
		const std::vector<EdtAddrPointRec>& addrPointRecs,
		CountInfo& countInfo
	);

private:

	/**
	 * @brief   ���ԍ��̔��p��
	 * @param   str [in]  �Ώە�����
	 * @return  ���p���������ԍ�
	 */
	CString toHalfGouNo(
		const CString& str
	);

	/**
	 * @brief   ���ԍ��ݒ�
	 * @param   addrPointRec [in/out]  �Z���|�C���g�R���e�i
	 * @param   addressList [in]  �Z�������񃊃X�g
	 * @param   shinraiF [in]  �M���t���O
	 * @param   hyouki [in]  �\�L
	 */
	void setGouNo(
		EdtAddrPointRec& addrPointRec,
		const std::vector<CString>& addressList,
		const CString& shinraiF,
		const CString& hyouki
	);

	/**
	 * @brief   �m�F�X�e�[�^�X�ݒ�
	 * @param   addrPointRec [in/out]  �Z���|�C���g�R���e�i
	 * @param   sourceCd [in]  �o�T�R�[�h
	 * @retval  true  ����
	 * @retval  false ���s
	 */
	bool setConfirmC(
		EdtAddrPointRec& addrPointRec,
		const CString& sourceCd
	);

private:

	std::list<uh::tstring> m_citycodeList; //!< �s�撬���R�[�h���X�g
	std::map<CString, ExtcodeListRec> m_extcodeListMap; //!< �g���R�[�h���X�g�}�b�v�ikey:14���R�[�h�Avalue:����/�ǂ�
};

