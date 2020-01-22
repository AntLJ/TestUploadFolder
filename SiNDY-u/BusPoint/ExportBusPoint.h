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
#include "Util.h"
#include "FieldMap.h"

/**
 * @class   CExportBusPoint
 * @brief	�o�X�f�[�^�G�N�X�|�[�g�̂��߂̃��C���N���X
 *          TODO: �o�X���C�u�������p���@�\�ڐA
 *          TODO: �֘A�e�[�u������̎擾�֐��͑S�ă��C�u�������Ƃ���z��
 */
class CExportBusPoint
{
public:
	/**
	 * @brief	�R���X�g���N�^
 	 * @param	args [in] �e��I�v�V����
	 */
	CExportBusPoint( const Arguments& Args ){
		m_allCompany     = Args.bAllCompany;
		m_strExpLog      = (Args.export_log).c_str();	
		m_strCompanyList = (Args.company_list).c_str();
		m_strExpDir      = (Args.export_dir).c_str();
		m_strBusDB       = (Args.bus_db).c_str();
	};

	/**
	 * @brief	�f�X�g���N�^
	 */
	~CExportBusPoint(){};

	/**
	 * @brief	�e�폈�������s���邽�߂ɍŏ��ɌĂ΂��֐�
	 * @retval	true  ��������
	 * @retval	false �������s
	 */
	bool execute();

private:

	typedef std::map<CString, CComVariant> MAPATTR;     //!< �t�B�[���h���Ɨv�f�i�[�pmap

	typedef std::pair<CString, CString>    CCPAIR;
	typedef std::pair<long, CString>       LCPAIR;
	typedef std::pair<LCPAIR, CCPAIR>      LINEMASTER;
	typedef std::vector<LINEMASTER>        LCCCPVEC;
	typedef std::map<long, MAPATTR>        MAPATTRMAP;

	typedef std::set<long>                 OIDSET;      //!< ID�i�[�pset

	typedef std::map<long, LCCCPVEC>       LINEMASTERS; //!< �o�X�H���}�X�^�i�[�pmap
	typedef std::multimap<long, MAPATTR>   STOPSEQS;    //!< ����o�H�̒�ԏ��i�[�pmultimap
	typedef std::map<long, MAPATTRMAP>     STOPPOINTS;	//!< �o�X��|�C���g�i�[�pmap(2�d�L�[)

	/**
	 * @brief	�o�H�����\�[�g�p�֐��I�u�W�F�N�g
	 * @param	p1, p2	[in]	��r�p�I�u�W�F�N�g
	 */
	struct lessDetail : public std::binary_function<LINEMASTER, LINEMASTER, bool>
	{
		bool operator ()(const LINEMASTER& p1, const LINEMASTER& p2) const {
		
			// first:( first:ObjectID, second:�n���� ), second:( first:�o�H, second:�o�R�n�����l )
			// �o�H�ŏ���
			if( p1.second.first != p2.second.first ){
				return p1.second.first < p2.second.first;
			}

			// ObjectID�ŏ���(�O�̂���)
			return p1.first.first < p2.first.first;
		}
	};

	/**
	 * @brief   �e�평����
	 * @retval  true  ����������
	 * @retval  false ���������s
	 */
	bool Init();

	/**
	 * @brief   �o�X�H���}�X�^����S���Ǝ҃��X�g���쐬����
	 * @retval  true  �쐬����
	 * @retval  false �쐬���s
	 */
	bool CreateCompanyList();

	/**
	 * @brief   �o�͑Ώێ��Ǝ҃��X�g��ǂݍ��݁A���ƎҖ�1�s�������X�g�Ɋi�[
	 * @retval  true  �ǂݍ��ݐ���
	 * @retval  false �ǂݍ��ݎ��s
	 */
	bool LoadList();

	/**
	 * @brief   �o�̓��C���֐�
	 * @param   companyName [in]  �o�͑Ώێ��ƎҖ�
	 */
	void Export( const CString& companyName );

	/**
	 * @brief   �o�X�H���}�X�^����Y�����Ǝ҂̏����擾
	 * @param   companyName [in]  �擾���������ƎҖ�
	 * @param   lineMasters [out] �擾�����o�X�H���}�X�^�̏��i�[�p(�o�H���o�͂Ŏg�p)
	 * @param   lineOIDs    [out] �擾�����o�X�H���}�X�^��ObjectID���X�g(�ٕ\�L���o�͂Ŏg�p)
	 */
	void GetLineMaster( const CString& companyName, LINEMASTERS& lineMasters, OIDSET& lineOIDs );

	/**
	 * @brief   �o�X��o�H���o��
	 * @param   lineMasters [in]  �o�X�H���}�X�^�̏��(�n���ԍ����L�[��map)
	 */
	void ExportDetail( LINEMASTERS& lineMasters );

	/**
	 * @brief   �o�X�◯�����ٕ\�L���o��
	 * @param   lineOIDs [in]  �o�X�H���}�X�^��ObjectID���X�g
	 */
	void ExportChanged( const OIDSET& lineOIDs );

	/**
	 * @brief   �Y����ԏ��ƕR�Â��o�X��|�C���g��(�ύX��)�◯�����擾
	 * @note    �ύX��◯������NULL����Ȃ��ꍇ�́A�ύX��◯�������D��
	 * @param   mapSeqAttr [in]  ��ԏ��̑����R���e�i
	 * @retval  �ύX��◯���� != NULL�F�ύX��◯����
	 *          �ύX��◯����  = NULL�F�◯����
	 */
	CString GetStopName( const MAPATTR& mapSeqAttr );

	/**
	 * @brief   �t�B�[�`������A1/256�b�ܓx�E�o�x���擾
	 * @param   ipFeature [in]  �ʒu���擾�������t�B�[�`��
	 * @param   lat256    [out] 1/256�b�ܓx
	 * @param   lon256    [out] 1/256�b�o�x
	 */
	void Get256LatLon( const IFeaturePtr& ipFeature, int& lat256, int& lon256 );

	/**
	 * @brief   �o�X��o�H���o�̓t�@�C����1�s�o��
	 * @param   lineIdx       [in]  �s�ԍ�
	 * @param   lat256        [in]  1/256�b�ܓx
	 * @param   lon256        [in]  1/256�b�o�x
	 * @param   routeNum      [in]  �n���ԍ�
	 * @param   routeName     [in]  �n����
	 * @param   routeDetail   [in]  �o�H
	 * @param   routeRemarks  [in]  �o�R�n�����l
	 * @param   startStopName [in]  ���◯����(�o�H���̒�ԏ������ԍ���1��(�ύX��)�◯����)
	 * @param   endStopName   [in]  ���◯����(�o�H���̒�ԏ������ԍ����ő��(�ύX��)�◯����)
	 * @param   mapSeqAttr    [in]  ��ԏ��̑����R���e�i
	 * @param   mapStopAttr   [in]  �o�X��|�C���g�̑����R���e�i
	 * @param   routeFlag     [in]  ���[�g�T���p�|�C���g�o�͎�true(�f�t�H���g�Ffalse)
	 * @retval  true  1�s�o�͐���
	 * @retval  false 1�s�o�͎��s
	 */
	bool ExportDetailLine( const long lineIdx, const int lat256, const int lon256, const long routeNum, const CString& routeName, const CString& routeDetail, const CString& routeRemarks, const CString& startStopName, const CString& endStopName, const MAPATTR& mapSeqAttr, const MAPATTR& mapStopAttr, bool routeFlag = false );

	/**
	 * @brief   �o�X�◯�����ٕ\�L���o�̓t�@�C����1�s�o��
	 * @param   lineIdx       [in]  �s�ԍ�
	 * @param   mapStopAttr   [in]  �o�X��|�C���g�̑����R���e�i
	 * @retval  true  1�s�o�͐���
	 * @retval  false 1�s�o�͎��s
	 */
	bool ExportChangedLine( const long lineIdx, const MAPATTR& mapStopAttr );

	/**
	 * @brief   �w��t�B�[���h��ObjectID�ɑΉ������ԏ��Ǘ��e�[�u�����擾
	 * @note    �o�X��|�C���g����́A�o�X��|�C���g��ObjectID�Ƃ���ID�t�B�[���h��
	 * @note    �o�X�H���}�X�^����́A�o�X�H���}�X�^��ObjectID�Ƃ���ID�t�B�[���h��
	 * @param   oID   [in]  �o�X��|�C���g��ObjectID or �o�X�H���}�X�^��ObjectID
	 * @param   field [in]  �t�B�[���h��
	 * @param   count [out] ����������
	 * @retval  count != 0 �F��ԏ��Ǘ��̃J�[�\��
	 * @retval  count  = 0 �Fnullptr
	 */
	_ICursorPtr GetStopSeqs( const long oID, const CString& field, long& count );

	/**
	 * @brief   ���[�g�T���p�|�C���gor��ԏ��ɑΉ�����o�X��|�C���g���擾
	 * @param   oID   [in]  ���[�g�T���p�|�C���gor��ԏ��̃o�X��|�C���gID
	 * @param   count [out] ����������
	 * @retval  count =  1 �F�o�X��|�C���g
	 * @retval  count != 1 �Fnullptr
	 */
	IFeaturePtr GetStopPoint( const long stopID, long& count );

	/**
	 * @brief   �o�X��|�C���g�ɑΉ����郋�[�g�T���p�|�C���g���擾
	            TODO: �K�v�ɉ�����1�ȏ�́A�J�[�\���ŕԂ��悤�ύX
	 * @param   oID   [in]  �o�X��|�C���g��ObjectID
	 * @param   count [out] ����������
	 * @retval  count =  1�F���[�g�T���p�|�C���g
	 * @retval  count != 1�Fnullptr
	 */
	IFeaturePtr GetRoutePoint( const long oID, long& count );

private:

	bool              m_allCompany;        //!< �S���Ǝҏo�̓��[�h
	CString           m_strExpLog;         //!< DB�`�F�b�N���O�t�@�C����
	CString           m_strCompanyList;    //!< �o�͎��ƎҖ����X�g
	CString           m_strExpDir;         //!< �o�͐�f�B���N�g����

	CString           m_strBusDB;          //!< �o�X�֘A�f�[�^�x�[�X�ڑ���

	std::ofstream     m_ofsExp;            //!< DB�`�F�b�N���O
	std::ofstream     m_ofsDetailFile;     //!< �o�X��o�H���o�̓t�@�C��
	std::ofstream     m_ofsChangedFile;    //!< �o�X�◯�����ٕ\�L���o�̓t�@�C��

	IWorkspacePtr     m_ipBusWorkspace;    //!< �o�X�֘A���[�N�X�y�[�X
	
	IFeatureClassPtr  m_ipBusStopPointFC;  //!< �o�X��|�C���g�t�B�[�`���N���X
	ITablePtr         m_ipBusLineMasterTable; //!< �o�X�H���}�X�^�e�[�u��
	ITablePtr         m_ipBusStopSeqTable; //!< �o�X��ԏ��Ǘ��e�[�u��
	IFeatureClassPtr  m_ipBusRoutePointFC; //!< �o�X���[�g�T���p�|�C���g�t�B�[�`���N���X

	CFieldMap         m_busStopPointFM;    //!< �o�X��|�C���g�t�B�[���h�}�b�v
	CFieldMap         m_busLineMasterFM;   //!< �o�X�H���}�X�^�t�B�[���h�}�b�v
	CFieldMap         m_busStopSeqFM;      //!< �o�X��ԏ��Ǘ��t�B�[���h�}�b�v
	CFieldMap         m_busRoutePointFM;   //!< �o�X���[�g�T���p�|�C���g�t�B�[���h�}�b�v

	std::set<CString> m_companySet;        //!< �o�͑Ώێ��ƎҖ����X�g
};
