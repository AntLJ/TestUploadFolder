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
 * @class   CCheckBusPoint
 * @brief	�o�X�f�[�^DB�`�F�b�N�̂��߂̃��C���N���X
 *          TODO: �o�X���C�u�������p���@�\�ڐA
 *          TODO: �`�F�b�N�ނ��o�X���C�u�����ɈڐA���A�{�N���X�͌Ăԑ������ƂȂ�z��
 */
class CCheckBusPoint
{
public:
	/**
	 * @brief	�R���X�g���N�^
 	 * @param	args [in] �e��I�v�V����
	 */
	CCheckBusPoint( const Arguments& Args ){
		m_strChkLog  = (Args.db_check_log).c_str();	
		m_strBusDB   = (Args.bus_db).c_str();
		m_strRoadDB  = (Args.road_db).c_str();

	};

	/**
	 * @brief	�f�X�g���N�^
	 */
	~CCheckBusPoint(){};

	/**
	 * @brief	�e�폈�������s���邽�߂ɍŏ��ɌĂ΂��֐�
	 * @retval	true  ��������
	 * @retval	false �������s
	 */
	bool execute();

private:

	typedef std::pair<CString, CString>    CCPAIR;
	typedef std::pair<CString, long>       CLPAIR;
	typedef std::pair<long, long>          LLPAIR;
	typedef std::vector<long>              LONGVEC;
	typedef std::pair<long, CCPAIR>        LCCPAIR;
	typedef std::pair<CLPAIR, CCPAIR>      CLCCPAIR;
	typedef std::vector<CLPAIR>            CLPVEC;
	typedef std::pair<CString, CCPAIR>     CCCPAIR;
	typedef std::pair<LLPAIR, CCCPAIR>     LLPCCCP;
	typedef std::vector<LLPCCCP>           LLCCCPVEC;
	typedef std::pair<long, LLPAIR>        LLLPAIR;

	typedef std::pair<LLLPAIR, CCCPAIR>    LLLCCCPAIR;
	typedef std::vector<LLLCCCPAIR>        LLLCCCPVEC;
	typedef std::vector<CLCCPAIR>          CLCCPVEC;

	typedef std::map<LLPAIR, LONGVEC>      BUSSTOPP;       //!< �o�X��|�C���g���m�̑�����r�pmap
	typedef std::map<LCCPAIR, CLPVEC>      SAMELINEMASTER; //!< �o�X�H���}�X�^�̎��ƎҖ��A�n���ԍ��A�o�H�����ꂪ�Ȃ����m�F�pmap
	typedef std::map<long, CLCCPVEC>       SAMELINENUM;    //!< �o�X�H���}�X�^�̓���n���ԍ��ŁA�n�������قȂ���̂̊m�F�pmap
	typedef std::map<long, LLLCCCPVEC>     SAMESEQ;        //!< ����H���̒�ԏ��m�F�pmap
	typedef std::map<long, LLCCCPVEC>      SAMELINESEQ;    //!< ��ԏ��̃V�[�P���X�m�F�pmap

	typedef std::map<CString, CComVariant> MAPATTR;        //!< �t�B�[���h���Ɨv�f�i�[�pmap

	/**
	 * @brief   �e�평����
	 * @retval  true  ����������
	 * @retval  false ���������s
	 */
	bool Init();

	/**
	 * @brief   �o�X��|�C���g�`�F�b�N
	 */
	void CheckStopPoint();

	/**
	 * @brief   �o�X���[�g�T���p�|�C���g�`�F�b�N
	 */
	void CheckRoutePoint();

	/**
	 * @brief   �o�X�H���}�X�^�`�F�b�N
	 */
	void CheckLineMaster();

	/**
	 * @brief   �o�X�H���}�X�^���m�̊m�F
	 * @param   sameLineMaster [in]  ���ƎҖ��A�n���ԍ��A�o�H���L�[�ɕK�v�ȏ����l�߂�map
	 * @param   sameLineNum    [in]  �n���ԍ����L�[�ɕK�v�ȏ����l�߂�map
	 */
	void CheckLineMasterAttrs( const SAMELINEMASTER& sameLineMaster, const SAMELINENUM& sameLineNum );

	/**
	 * @brief   �o�X��ԏ��Ǘ��`�F�b�N
	 */
	void CheckStopSeq();

	/**
	 * @brief   ����H���̒�ԏ��m�F
	 * @param   sameSeq [in]  �H��ID���L�[�ɕK�v�ȏ����l�߂�map
	 */
	void CheckSameSeqs( const SAMESEQ& sameSeq );

	/**
	 * @brief   �o�X��|�C���g�ɑΉ����郋�[�g�T���p�|�C���g���擾
	            TODO: �K�v�ɉ�����1�ȏ�́A�J�[�\���ŕԂ��悤�ύX
	 * @param   oID   [in]  �o�X��|�C���g��ObjectID
	 * @param   count [out] ����������
	 * @retval  count =  1�F���[�g�T���p�|�C���g
	 * @retval  count != 1�Fnullptr
	 */
	IFeaturePtr GetRoutePoint( const long& oID, long& count );

	/**
	 * @brief   �t�B�[�`���Ԃ̋���(m)�𑪒�
	 * @param   ipSrcFeature  [in]  ��r���t�B�[�`��
	 * @param   ipDestFeature [in]  ��r��t�B�[�`��
	 * @retval  double        �t�B�[�`���Ԃ̋���(m)
	 */
	double GetDistFeature( const IFeaturePtr& ipSrcFeature, const IFeaturePtr& ipDestFeature );

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
	_ICursorPtr GetStopSeqs( const long& oID, const CString& field, long& count );

	/**
	 * @brief   ��ԏ��Ǘ��e�[�u���̘H�����ID����H�����Ǘ��e�[�u�����擾
     *          TODO: �K�v�ɉ����āA1�ȏ�̓J�[�\��������Ԃ��悤�ύX
	 * @param   oID   [in]  ��ԏ��Ǘ��̘H�����ID
	 * @retval  count != 0 �F��������Row
	 * @retval  count  = 0 �Fnullptr
	 */
	_IRowPtr GetLineMaster( const long& lineID ); 

	/**
	 * @brief   �o�X��|�C���g�̎w��ߖT�̃o�X��|�C���g���擾
	 * @note    �w��ߖT�́A�����20m�Œ�ŁA�o�b�t�@���g�p����
	 * @param   ipFeature [in]  ��ƂȂ�o�X��|�C���g
	 * @param   count     [out] ����������
	 * @retval  count != 0 �F�o�X��|�C���g�̃J�[�\��
	 * @retval  count  = 0 �Fnullptr
	 */
	IFeatureCursorPtr GetNearStopPoints( const IFeaturePtr& ipFeature, long& count );

	/**
	 * @brief   �o�X��|�C���g�̃J�[�\������A�W���|�C���g�����J�E���g
	 * @param   ipCursor [in]  �o�X��|�C���g�̃J�[�\��
	 * @retval  count    �W���|�C���g��
	 */
	long CountPolePoint( const IFeatureCursorPtr& ipCursor );

	/**
	 * @brief   ���[�g�T���p�|�C���gor��ԏ��ɑΉ�����o�X��|�C���g���擾
	 * @param   oID   [in]  ���[�g�T���p�|�C���gor��ԏ��̃o�X��|�C���gID
	 * @param   count [out] ����������
	 * @retval  count =  1 �F�o�X��|�C���g
	 * @retval  count != 1 �Fnullptr
	 */
	IFeaturePtr GetStopPoint( const long& stopID, long& count );

	/**
	 * @brief   ���[�g�T���p�|�C���g�̎w��ߖT�̓��H�����N���擾
	 * @note    �w��ߖT�́A�����0.5m(50cm)�Œ�ŁA�o�b�t�@���g�p����
	 * @param   ipFeature [in]  ��ƂȂ郋�[�g�T���p�|�C���g
	 * @retval  �t�B�[�`��(������Ȃ������ꍇ�́ANULL)
	 */
	IFeaturePtr GetNearRoadLink( const IFeaturePtr& ipFeature );




private:

	CString           m_strChkLog;         //!< DB�`�F�b�N���O�t�@�C����

	CString           m_strBusDB;          //!< �o�X�֘A�f�[�^�x�[�X�ڑ���
	CString           m_strRoadDB;         //!< ���H�f�[�^�x�[�X�ڑ���

	std::ofstream     m_ofsChk;            //!< DB�`�F�b�N���O

	IWorkspacePtr     m_ipBusWorkspace;    //!< �o�X�֘A���[�N�X�y�[�X
	IWorkspacePtr     m_ipRoadWorkspace;   //!< ���H���[�N�X�y�[�X
	
	IFeatureClassPtr  m_ipBusStopPointFC;  //!< �o�X��|�C���g�t�B�[�`���N���X
	ITablePtr         m_ipBusLineMasterTable; //!< �o�X�H���}�X�^�e�[�u��
	ITablePtr         m_ipBusStopSeqTable; //!< �o�X��ԏ��Ǘ��e�[�u��
	IFeatureClassPtr  m_ipBusRoutePointFC; //!< �o�X���[�g�T���p�|�C���g�t�B�[�`���N���X
	IFeatureClassPtr  m_ipRoadLinkFC;      //!< ���H�����N�t�B�[�`���N���X

	CFieldMap         m_busStopPointFM;    //!< �o�X��|�C���g�t�B�[���h�}�b�v
	CFieldMap         m_busLineMasterFM;   //!< �o�X�H���}�X�^�t�B�[���h�}�b�v
	CFieldMap         m_busStopSeqFM;      //!< �o�X��ԏ��Ǘ��t�B�[���h�}�b�v
	CFieldMap         m_busRoutePointFM;   //!< �o�X���[�g�T���p�|�C���g�t�B�[���h�}�b�v
};

