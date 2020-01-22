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

class RelNodeInfos
{
private:
	/**
	 *	�֘A�m�[�h���
	 */
	struct RelNode
	{
	public:
		long m_NodeOid;		//!< �m�[�h��OID
		CString m_NodeGid;	//!< �m�[�h��GID
		IPointPtr m_Point;	//!< �|�C���g

		RelNode(): m_NodeOid(-1){}
		RelNode(long oid, CString gid, IPointPtr point): 
			m_NodeOid(oid), m_NodeGid(gid), m_Point(point) {}
		~RelNode(){}

		// OID �Ń��j�[�N�ɂȂ�z��
		bool operator <(const RelNode& rel_node ) const{
			return m_NodeOid < rel_node.m_NodeOid;
		}

		bool operator ==(const RelNode& rel_node ) const{
			return m_NodeOid == rel_node.m_NodeOid;
		}
	};


public:
	RelNodeInfos(void) : m_Error(false) {};
	RelNodeInfos(ITablePtr rel_table, ITablePtr node_table, const CString& roadsign_gid) : 
		m_RelTable(rel_table), 
		m_NodeTable(node_table),
		m_RoadsignGid(roadsign_gid)
		{}
	virtual ~RelNodeInfos(void){};

public:
	/**
	 *  @brief �֘A�e�[�u���̏���ǂݍ���
	 *  @retval true  �ǂݍ��ݐ���
	 *  @retval false �ǂݍ��ݎ��s
	 */
	bool read();

	/**
	 *  @brief  �ēǂݍ��݂��s��
	 *  @param[in] oids �ēǂݍ��݂��s���m�[�h��OID�Q
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
	 *  @brief �m�[�h�̃|�C���g�Q��Ԃ�
	 *  @return �|�C���g�Q
	 */
	std::vector<IPointPtr> getNodePoints() const;

	/**
	 *  @brief �ύX�����邩
	 *  @retval true  �ύX����
	 *  @retval false �ύX�Ȃ�
	 */
	bool isChange() const;

	/**
	 *  @brief �w�肵��OID�ɊY������m�[�h�̌`����擾����
	 *  @param[in] oids  �擾�������m�[�h��OID�Q
	 *  @return    �m�[�h�̃|�C���g�Q
	 */
	std::vector<IPointPtr> getPoints(const std::vector<long>& oids) const;

	/**
	 *  @brief �G���[�����邩
	 *  @detail �_�C�A���O�Ƀc���[�ɂԂ牺�����Ă���e�t�B�[�`���ɂ���
	 *          �֘A�m�[�h�̃G���[��Ԃ�ێ����Ă����K�v�����邽�߂ɁA
	 *          �p�ӂ��Ă���֐�
	 *  @retval true  �ύX����
	 *  @retval false �ύX�Ȃ�
	 */
	bool hasError() const { return m_Error; }

private:

	/**
	 *  @brief ���R�[�h�̑��݊m�F
	 *  @param[in]    �����Ώۂ̃��R�[�h
	 *  @retval true  ���R�[�h����������
	 *  @retval false ���R�[�h��������Ȃ�����
	 */
	bool isExist(const RelNode& rel_node);

private:


	CString m_RoadsignGid;		//!< ���H�W���|�C���gGID

	std::set<RelNode> m_RelNodes;		//!< �֘A�m�[�h���Q
	std::set<RelNode> m_prevRelNodes;	//!< �ύX�O�̊֘A�m�[�h���Q

	ITablePtr m_RelTable;	//!< �֘A�e�[�u��
	ITablePtr m_NodeTable;	//!< �֘A����m�[�h�̃t�B�[�`���N���X

	bool m_Error;		//!< �G���[�����邩
};

