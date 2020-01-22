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

/**
 * @brief �H�����X�g�Q���Ǘ�����N���X
 * @note  �S�������N�R���e�[�u���̃��R�[�h�Q�ɑ���������
 */
class CRelTableData
{
public:

	/**
	 *	@brief	�H�����X�g1���R�[�h 
	 */
	struct RailwayLineRecord
	{
		long lRailwayLineOid;		//!< �S���H���e�[�u����OID
		CString strRailwayName;		//!< �H����
		CString strCorpName;		//!< ��Ж�

		RailwayLineRecord(){}
		~RailwayLineRecord(){}
	};

public:
	typedef std::vector<RailwayLineRecord>::iterator Iter;
	typedef std::vector<RailwayLineRecord>::const_iterator CIter;

public:
	CRelTableData(void) {};
	~CRelTableData(void) {};

	/**
	 *	@brief	�H�����X�g�f�[�^�̐擪��Ԃ� 
	 */
	CIter begin() const { return m_RailwayLineData.begin(); }

	/**
	 *	@brief	�H�����X�g�f�[�^�̖�����Ԃ� 
	 */
	CIter end() const { return m_RailwayLineData.end(); }

	/**
	 *	@brief	�H�����X�g�f�[�^�̃N���A 
	 */
	void clear() { m_RailwayLineData.clear(); }

	/**
	 *	@brief	�H�����X�g�f�[�^���󂩔ۂ�
	 *	@retval true   �f�[�^����
	 *	@retval false  �f�[�^����ł͂Ȃ�
	 */
	bool empty() const { return m_RailwayLineData.empty(); }

	/**
	 *	@brief	���R�[�h����Ԃ�
	 *  @return ���R�[�h�̃T�C�Y
	 */
	int size() const { return m_RailwayLineData.size(); }

	/**
	 *	@brief	 �w�肵���S���H���e�[�u��OID�̃��R�[�h�����݂��邩
	 *  @param   [in]   lId  �S���H���e�[�u����OID
	 *  @retval  true   ���݂���
	 *  @retval  false  ���݂��Ȃ�
	 */
	bool isExist( long lId ) const;

	/**
	 *	@brief	ID���d�����郌�R�[�h�����擾
	 *  @param  [in]  ��r��̃��R�[�h�Q
	 *  @return �d�����R�[�h��
	 */
	int getDupliCount( const CRelTableData& cData ) const;

	/**
	 *	@brief	�S���H���e�[�u����OID�ł̃��R�[�h����
	 *  @param  [in]  nJoinLineId  ������H��ID
	 *  @return �H�����X�g���R�[�h�̃C�e���[�^
	 */
	Iter find( long lId );

	/**
	 *	@brief	���R�[�h�̒ǉ� 
	 *  @param  [in]  cRecord  �H�����X�g�̃��R�[�h
	 */
	void add(RailwayLineRecord cRecord){ m_RailwayLineData.push_back(cRecord); }

	/**
	 *	@brief	���R�[�h�̍폜 
	 *  @param  [in]  iter  �폜���R�[�h�̃C�e���[�^
	 */
	void erase(CIter iter){ m_RailwayLineData.erase(iter); };

	/**
	 *	@brief	���R�[�h�̍폜(�S���H���e�[�u����OID���w��) 
	 *  @param  [in]  lid  �폜���郌�R�[�h�̓S���H���e�[�u����OID
	 */
	void erase(long lid);

	/**
	 *	@brief	�H�����X�g�f�[�^�̃R�s�[
	 *	@note	�R�s�[��̃R���e�i�͏�������
	 *  @param  [out]  cData  �R�s�[��̃R���e�i
	 */
	void copyData(CRelTableData& cData) const;
	
private:

	std::vector<RailwayLineRecord> m_RailwayLineData;	//!< �H�����X�g�̃f�[�^�Q
};

