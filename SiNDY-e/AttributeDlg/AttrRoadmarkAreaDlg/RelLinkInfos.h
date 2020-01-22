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

class RelLinkInfos
{
private:
	/**
	 *	�֘A�Ԑ������N���
	 */
	struct RelLink
	{
	public:
		long m_LinkOid;				//!< �����N��OID
		CString m_LinkGid;			//!< �����N��GID
		IPolylinePtr m_Polyline;	//!< �����N�`��

		RelLink(): m_LinkOid(-1){}
		RelLink(long oid, CString gid, IPolylinePtr polyline): 
			m_LinkOid(oid), m_LinkGid(gid), m_Polyline(polyline) {}
		~RelLink(){}

		// OID �Ń��j�[�N�ɂȂ�z��
		bool operator <(const RelLink& rel_node ) const{
			return m_LinkOid < rel_node.m_LinkOid;
		}

		bool operator ==(const RelLink& rel_node ) const{
			return m_LinkOid == rel_node.m_LinkOid;
		}
	};


public:
	RelLinkInfos(void) : m_error(false) {};
	RelLinkInfos(ITablePtr rel_table, ITablePtr link_table, const CString& roadmark_gid) : 
		m_relTable(rel_table), 
		m_linkTable(link_table),
		m_roadmarkGid(roadmark_gid)
		{}
	virtual ~RelLinkInfos(void){};

public:
	/**
	 *  @brief �֘A�e�[�u���̏���ǂݍ���
	 *  @retval true  �ǂݍ��ݐ���
	 *  @retval false �ǂݍ��ݎ��s
	 */
	bool read();

	/**
	 *  @brief  �ēǂݍ��݂��s��
	 *  @param[in] oids �ēǂݍ��݂��s�������N��OID�Q
	 *  @param[out] err_oids �ǂݍ��݃G���[�ƂȂ���OID�Q
	 *  @retval true  �ēǂݍ��ݐ���
	 *  @retval false �ēǂݍ��ݎ��s
	 */
	bool reloadData(const std::vector<long>& oids, std::vector<long>& err_oids);

	/**
	 *  @brief  �ύX�_���e�[�u���֔��f����
	 *  @retval true  �e�[�u���X�V����
	 *  @retval false �e�[�u���X�V���s
	 */
	bool updateTable();

	/**
	 *  @brief �֘A�e�[�u����OID�Q���擾
	 *  @return GID�Q
	 */
	std::vector<long> getOIDs() const;

	/**
	 *  @brief �����N�̌`��Q(�|�����C��)��Ԃ�
	 *  @return �|�����C���Q
	 */
	std::vector<IPolylinePtr> getLinkPolylines() const;

	/**
	 *  @brief �ύX�����邩
	 *  @retval true  �ύX����
	 *  @retval false �ύX�Ȃ�
	 */
	bool isChange() const;

	/**
	 *  @brief �w�肵��OID�ɊY�����郊���N�̌`����擾����
	 *  @param[in] oids  �擾�����������N��OID�Q
	 *  @return    �m�[�h�̃|�C���g�Q
	 */
	std::vector<IPolylinePtr> getPolylines(const std::vector<long>& oids) const;

	/**
	 *  @brief �G���[�����邩
	 *  @detail �_�C�A���O�Ƀc���[�ɂԂ牺�����Ă���e�t�B�[�`���ɂ���
	 *          �֘A�m�[�h�̃G���[��Ԃ�ێ����Ă����K�v�����邽�߂ɁA
	 *          �p�ӂ��Ă���֐�
	 *  @retval true  �ύX����
	 *  @retval false �ύX�Ȃ�
	 */
	bool hasError() const { return m_error; }

private:
	/**
	 *  @brief ���R�[�h�̑��݊m�F
	 *  @param[in]    �����Ώۂ̃��R�[�h
	 *  @retval true  ���R�[�h����������
	 *  @retval false ���R�[�h��������Ȃ�����
	 */
	bool isExist(const RelLink& rel_link);

private:
	CString m_roadmarkGid;		//!< ���H�\���G���AGID

	std::set<RelLink> m_relLinks;		//!< �֘A�����N���Q
	std::set<RelLink> m_prevRelLinks;	//!< �ύX�O�̊֘A�����N���Q

	ITablePtr m_relTable;	//!< �֘A�e�[�u��
	ITablePtr m_linkTable;	//!< �Ԑ������N�t�B�[�`���N���X

	bool m_error;		//!< �G���[�����邩
};

