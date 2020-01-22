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
 * @note  �S���m�[�h�R���e�[�u���̃��R�[�h�Q�ɑ���������
 */
class CRelTableData
{
public:

	/**
	 *	@brief	�H�����X�g1���R�[�h 
	 */
	struct LineInfoRecord
	{
		long nJoinLineId;			//!< ������H��OID
		CString strStationName;		//!< �w��
		CString strRailwayName;		//!< �H����
		CString strStationNumber;	//!< �w�ԍ�

		LineInfoRecord() : nJoinLineId(0) {}
		~LineInfoRecord(){}

		bool equalStaNumber(const LineInfoRecord& rec) const
		{
			return ( strStationNumber.Compare(rec.strStationNumber) == 0 );
		}
	};

public:
	typedef std::vector<LineInfoRecord>::iterator Iter;
	typedef std::vector<LineInfoRecord>::const_iterator CIter;

public:
	CRelTableData(void) {};
	~CRelTableData(void) {};

	/**
	 *	@brief	�H�����X�g�f�[�^�̐擪��Ԃ� 
	 */
	CIter begin() const { return m_LineInfoData.begin(); }

	/**
	 *	@brief	�H�����X�g�f�[�^�̖�����Ԃ� 
	 */
	CIter end() const { return m_LineInfoData.end(); }

	/**
	 *	@brief	�H�����X�g�f�[�^�̃N���A 
	 */
	void clear() { m_LineInfoData.clear(); }

	/**
	 *	@brief	�H�����X�g�f�[�^���󂩔ۂ�
	 *	@retval true   �f�[�^����
	 *	@retval false  �f�[�^����ł͂Ȃ�
	 */
	bool empty() const { return m_LineInfoData.empty(); }

	/**
	 *	@brief	���R�[�h����Ԃ�
	 *  @return ���R�[�h�̃T�C�Y
	 */
	int size() const { return m_LineInfoData.size(); }

	/**
	 *	@brief	�w�肵��������H��ID�̃��R�[�h�����݂��邩
	 *  @param   [in]  nJoinLineId  ������H��ID
	 *  @retval  true   ���݂���
	 *  @retval  false  ���݂��Ȃ�
	 */
	bool isExist( int nJoinLineId ) const;

	/**
	 *	@brief	ID���d�����郌�R�[�h�����擾
	 *  @param  [in]  ��r��̃��R�[�h�Q
	 *  @return �d�����R�[�h��
	 */
	int getDupliCount( const CRelTableData& cData ) const;

	/**
	 *	@brief	������H��ID�ł̃��R�[�h����
	 *  @param  [in]  nJoinLineId  ������H��ID
	 *  @return �H�����X�g���R�[�h�̃C�e���[�^
	 */
	Iter find( int nJoinLineId );

	/**
	 *	@brief	���R�[�h�̒ǉ� 
	 *  @param  [in]  cRecord  �H�����X�g�̃��R�[�h
	 */
	void add(LineInfoRecord cRecord){ m_LineInfoData.push_back(cRecord); }

	/**
	 *	@brief	���R�[�h�̍폜 
	 *  @param  [in]  iter  �폜���R�[�h�̃C�e���[�^
	 */
	void erase(CIter iter){ m_LineInfoData.erase(iter); };

	/**
	 *	@brief	���R�[�h�̍폜(JOIN_LINE_ID �w��) 
	 *  @param  [in]  lid  �폜���郌�R�[�h��JOIN_LINE_ID
	 */
	void erase(long lid);

private:

	std::vector<LineInfoRecord> m_LineInfoData;	//!< �H�����X�g�̃f�[�^�Q
};

