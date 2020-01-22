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
 * @brief �w�|���S���R�t���e�[�u���̏����Ǘ�����N���X
 */
class CRelStationSiteData
{
public:

	/**
	 *	@brief	�w�|���S���R�t���e�[�u��1���R�[�h
	 */
	struct RelStationSiteRecord
	{
		long lGroupId;			//!< �wID
		long lLayerNoC;			//!< ���C���[�ԍ�
		long lLayerOid;			//!< ���C���[��OID

		RelStationSiteRecord(): lGroupId(0L), lLayerNoC(0L), lLayerOid(0L) {}
		~RelStationSiteRecord(){}
	};

public:
	typedef std::vector<RelStationSiteRecord>::iterator Iter;
	typedef std::vector<RelStationSiteRecord>::const_iterator CIter;

public:
	CRelStationSiteData(void) {};
	~CRelStationSiteData(void) {};

	/**
	 *	@brief	�w�|���S���R�t���e�[�u���f�[�^�̐擪��Ԃ� 
	 *  @return �f�[�^�̐擪
	 */
	CIter begin() const { return m_RelStationData.begin(); }

	/**
	 *	@brief	�w�|���S���R�t���e�[�u���f�[�^�̖�����Ԃ� 
	 *  @return �f�[�^�̖���
	 */
	CIter end() const { return m_RelStationData.end(); }

	/**
	 *	@brief	�w�|���S���R�t���e�[�u���f�[�^�̃N���A 
	 */
	void clear() { m_RelStationData.clear(); }

	/**
	 *	@brief	�w�|���S���R�t���e�[�u���̃f�[�^���󂩔ۂ�
	 *	@retval true   �f�[�^����
	 *	@retval false  �f�[�^����ł͂Ȃ�
	 */
	bool empty() const { return m_RelStationData.empty(); }

	/**
	 *	@brief	���R�[�h����Ԃ�
	 *  @return ���R�[�h�̃T�C�Y
	 */
	int size() const { return m_RelStationData.size(); }

	/**
	 *	@brief	 �w�|���S���R�t���e�[�u���̃��R�[�h�����݂��邩
	 *  @param   [in]   record     �w�|���S���R�t���e�[�u���̃��R�[�h
	 *  @retval  true   ���݂���
	 *  @retval  false  ���݂��Ȃ�
	 */
	bool isExist( const RelStationSiteRecord& record ) const;

	/**
	 *	@brief	�d�����郌�R�[�h�����擾����
	 *  @param  [in]  cData  ��r��̃��R�[�h�Q
	 *  @return �d�����R�[�h��
	 */
	int getDupliCount( const CRelStationSiteData& cData ) const;

	/**
	 *	@brief   �w�|���S���R�t���e�[�u���̃��R�[�h����
	 *  @param   [in]   record  �w�|���S���R�t���e�[�u���̃��R�[�h
	 *  @return  �w�|���S���R�t�����R�[�h�̃C�e���[�^
	 */
	Iter find( const RelStationSiteRecord& record );

	/**
	 *	@brief       �w�|���S���R�t���e�[�u���̃��R�[�h����
	 *  @attension   �ێ����Ă��郌�R�[�h���ŃO���[�vID�����j�[�N�ȑO��
	 *  @param       [in]   lGroupId  �O���[�vID
	 *  @return      �w�|���S���R�t�����R�[�h�̃C�e���[�^
	 */
	Iter find( long lGroupId );

	/**
	 *	@brief	���R�[�h�̒ǉ� 
	 *  @param  [in]  cRecord  �H�����X�g�̃��R�[�h
	 */
	void add( RelStationSiteRecord cRecord ){ m_RelStationData.push_back(cRecord); }

	/**
	 *	@brief	���R�[�h�̍폜 
	 *  @param  [in]  iter  �폜���R�[�h�̃C�e���[�^
	 */
	void erase( CIter iter ){ m_RelStationData.erase(iter); };

	/**
	 *	@brief	���R�[�h�̍폜 
	 *  @param  [in]  lGroupId
	 *  @attension �ێ����Ă���R���e�i���ŃO���[�vID�����j�[�N�ȑO��
	 */
	void erase( long lGroupId );

private:

	std::vector<RelStationSiteRecord> m_RelStationData;		//!< �w�|���S���R�t���e�[�u���̃f�[�^
};

