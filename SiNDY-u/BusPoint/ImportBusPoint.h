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
#include "BusPointCommon/Connect.h"
#include <WinLib/ExcelHelper.h>


/**
 * @class   CImportBusPoint
 * @brief   �o�X�f�[�^�̃C���|�[�g�̂��߂̃��C���N���X
 *          TODO: �o�X���C�u�������p
 */
class CImportBusPoint
{
public:
	/**
	 * @brief	�R���X�g���N�^
 	 * @param	args [in] �e��I�v�V����
	 */
	CImportBusPoint( const Arguments& args ){
		m_strInFile  = (args.in_list).c_str();
		m_strImpLog  = (args.import_log).c_str();	
		m_strARPLog  = (args.auto_rpoint_log).c_str();
		m_strBusDB   = (args.bus_db).c_str();
		m_strRoadDB  = (args.road_db).c_str();

		m_import     = args.bImport;
		m_autoRPoint = args.bAutoRPoint;
	};

	/**
	 * @brief	�f�X�g���N�^
	 */
	~CImportBusPoint(){};

	/**
	 * @brief	�e�폈�������s���邽�߂ɍŏ��ɌĂ΂��֐�
	 * @retval	true  ��������
	 * @retval	false �������s
	 */
	bool execute();

private:

	typedef std::map<CString, CString>       BUSCONTAINER; //!< �o�X���R�[�h�Ǘ��p�̃t�B�[���h���E�lmap

	typedef std::pair<CString, CString>      CSTRPAIR;     //!< CString�̃y�A

	typedef std::map<CSTRPAIR, BUSCONTAINER> BUSPOINTMAP;  //!< BUS_STOP_POINT�C���|�[�g�pmap
	typedef std::map<CSTRPAIR, BUSCONTAINER> BUSMASTERMAP; //!< BUS_LINE_MASTER�C���|�[�g�pmap
	typedef std::vector<BUSCONTAINER>        BUSSEQVEC;    //!< BUS_STOP_SEQ�C���|�[�g�pvector(��Excel�ǂݍ��ݐ�)

	/**
	 * @brief   �e�평����
	 * @retval  true  ����������
	 * @retval  false ���������s
	 */
	bool Init();

	/**
	 * @brief   �������ʃt�@�C�����X�g��ǂݍ��݁A�t�@�C���p�X1�s�������X�g�Ɋi�[
	 * @retval  true  ��������
	 * @retval  false �������s
	 */
	bool LoadList();

	/**
	 * @brief   �񖼂����X�g�Ɋi�[
	 * @note    �e�V�[�g��e�t�@�C���ԂŖ���񖼂��h�ꂽ�ꍇ���l���A�Ǝ��Ɋi�[����
	 */
	void SetColumnName();

	/**
	 * @brief   BUS_LINE_MASTER��������̌n���ԍ��iROUTE_NUMBER�j��S�Ď擾����
	 */
	void GetExistRouteNumber();

	/**
	 * @brief   BUS_STOP_POINT��������̒◯���R�[�h�ƕW���ԍ��̃y�A��S�Ď擾����
	 */
	void GetExistStopAndPole();

	/**
	 * @brief   �������ʃt�@�C������A�C���|�[�g�p���X�g�쐬
	 * @note    ���ꂼ��̃��X�g�̃��j�[�N�����ɉ����ă��X�g���쐬����
	 * @param   busStopPoints  [out] �o�X��|�C���g�C���|�[�g�p���X�g
	 * @param   busLineMasters [out] �o�X�H���}�X�^�C���|�[�g�p���X�g
	 * @param   busStopSeqs    [out] �o�X��ԏ��Ǘ��C���|�[�g�p���X�g
	 * @retval  true  �쐬����
	 * @retval  false �쐬���s
	 */
	bool LoadExcelData( BUSPOINTMAP& busStopPoints, BUSMASTERMAP& busLineMasters, BUSSEQVEC& busStopSeqs );

	/**
	 * @brief   �������ʃt�@�C����1�s����A�R���e�i�ɓǂݍ���
	 * @param   ipWorkSheet [in]  �������ʃ��[�N�V�[�g
	 * @param   lineIdx     [in]  �s�ԍ�
	 * @param   busInput    [out] ��������1�s�i�[�R���e�i
	 * @retval  true  1�s�ǂݍ��ݐ���
	 * @retval  false 1�s�ǂݍ��ݎ��s
	 */
	bool GetLineFromSheet( const Excel::_WorksheetPtr& ipWorkSheet, const long lineIdx, BUSCONTAINER& busInput );

	/**
	 * @brief   �Z���̒l���擾
	 * @param   sa        [in]  Excel�V�[�g1�s����SafeArray
	 * @param   Idx       [in]  �擾�������Z���̃C���f�b�N�X
	 * @retval  �Z���̒l
	 */
	 CString GetCellStr( CComSafeArray<VARIANT>& sa, long idx )
	 {
		CComVariant vaValue;
		long cellIdx[2] = {1, idx};
		sa.MultiDimGetAt( cellIdx, vaValue );
		return CellToString( vaValue );
	 };

	/**
	 * @brief   �Z���̒l��CString�ŕԂ�
	 * @param   vaValue [in] �Z���̏���������CComVariant�^
	 * @retval  CString�^�̃Z���̒l
	 */
	CString CellToString( const CComVariant& vaValue )
	{
		CString retStr;
		if( VT_R8 == vaValue.vt){
			// �����_�ȉ������i�l�̌ܓ�����邪�A�����̂ݎw��t�H�[�}�b�g�̂��߁A���Ȃ��j
			retStr.Format(_T("%.lf"), vaValue.dblVal);
		}
		else{
			retStr = (VT_BSTR == vaValue.vt)? vaValue.bstrVal : _T("");
		}
		return retStr;
	};

	/**
	 * @brief   �o�X��|�C���g�C���|�[�g�p���X�g�ւ̒l�Z�b�g
	 * @param   busInput     [in]  ��������1�s
	 * @param   busStopPoint [out] �o�X��|�C���g�C���|�[�g1�s
	 * @retval  true  �Z�b�g����
	 * @retval  false �Z�b�g���s
	 */
	bool SetBusStopPoint( const BUSCONTAINER& busInput, BUSCONTAINER& busStopPoint );

	/**
	 * @brief   �o�X�H���}�X�^�C���|�[�g�p���X�g�ւ̒l�Z�b�g
	 * @param   busInput      [in]  ��������1�s
	 * @param   companyName   [in]  �������ʃt�@�C��(���Ǝ�)��
	 * @param   busLineMaster [out] �o�X�H���}�X�^�C���|�[�g1�s
	 * @retval  true  �Z�b�g����
	 * @retval  false �Z�b�g���s
	 */
	bool SetBusLineMaster( const BUSCONTAINER& busInput, const CString& companyName, BUSCONTAINER& busLineMaster );

	/**
	 * @brief   �o�X��ԏ��Ǘ��C���|�[�g�p���X�g�ւ̒l�Z�b�g
	 * @param   busInput     [in]  ��������1�s
	 * @param   busStopPoint [out] �o�X��ԏ��Ǘ��C���|�[�g1�s
	 * @retval  true  �Z�b�g����
	 * @retval  false �Z�b�g���s
	 */
	bool SetBusStopSeq( const BUSCONTAINER& busInput, BUSCONTAINER& busStopSeq );

	/**
	 * @brief   �������ʂ���SDE�̃o�X�֘A�e�[�u���ɃC���|�[�g
	 * @param   busStopPoints  [in] �o�X��|�C���g�C���|�[�g�p���X�g
	 * @param   busLineMasters [in] �o�X�H���}�X�^�C���|�[�g�p���X�g
	 * @param   busStopSeqs    [in] �o�X��ԏ��Ǘ��C���|�[�g�p���X�g
	 * @retval  true  �C���|�[�g����
	 * @retval  false �C���|�[�g���s
	 */
	bool ImportBusPoint( const BUSPOINTMAP& busStopPoints, const BUSMASTERMAP& busLineMasters, const BUSSEQVEC& busStopSeqs );

	/**
	 * @brief   �o�X��|�C���g�C���|�[�g���X�g���R�[�h����A�`��E������FeatureBuffer�ɃZ�b�g
	 * @param   busStopPoint         [in]  �o�X��|�C���g�C���|�[�g���X�g���R�[�h
	 * @param   ipBusStopPointBuffer [out] �C���|�[�g�pFeatureBuffer
	 * @retval  true  �`��E�����̃Z�b�g����
	 * @retval  false ���s
	 */
	bool SetWriteBusStopPoint( const BUSCONTAINER& busStopPoint, IFeatureBufferPtr& ipBusStopPointBuffer );

	/**
	 * @brief   �o�X�H���}�X�^�e�[�u���C���|�[�g���X�g���R�[�h����A������RowBuffer�ɃZ�b�g
	 * @param   busLineMaster         [in]  �o�X�H���}�X�^�e�[�u���C���|�[�g���X�g���R�[�h
	 * @param   ipBusLineMasterBuffer [out] �C���|�[�g�pRowBuffer
	 * @retval  true  �����̃Z�b�g����
	 * @retval  false ���s
	 */
	bool SetWriteBusLineMaster( const BUSCONTAINER& busLineMaster, IRowBufferPtr& ipBusLineMasterBuffer );

	/**
	 * @brief   �o�X��ԏ��Ǘ��e�[�u���C���|�[�g���X�g���R�[�h����A������RowBuffer�ɃZ�b�g
	 * @param   busStopSeq         [in]  �o�X��ԏ��Ǘ��e�[�u���C���|�[�g���X�g���R�[�h
	 * @param   ipBusStopSeqBuffer [out] �C���|�[�g�pRowBuffer
	 * @retval  true  �����̃Z�b�g����
	 * @retval  false ���s
	 */
	bool SetWriteBusStopSeq( const BUSCONTAINER& busStopSeq, IRowBufferPtr& ipBusStopSeqBuffer );

	/**
	 * @brief   �W���|�C���g�ɑΉ����郋�[�g�T���p�|�C���g��ǉ�
	 * @retval  true  �ǉ�����
	 * @retval  false �ǉ����s
	 */
	bool CreateRPoints();

	/**
	 * @brief   �쐬�Ώۂ̃o�X��|�C���gObjectID���X�g���쐬����
	 * @note    ���O�o�͗p��OID�ƍ��킹�āA�o�X��|�C���g�̏����擾����
	 * @param   busPoints [out] �o�X��|�C���g���X�g
	 * @retval  true  ���X�g�쐬����
	 * @retval  false ���s
	 */
	bool CreateOIDList( std::set<std::list<CString>>& busPoints );

	/**
	 * @brief   ���[�g�T���p�|�C���g�̒ǉ��Ώۂ����ׂ�
	 * @param   busPointOID [in] �o�X��|�C���gObjectID
	 * @retval  true  �ǉ��Ώ�
	 * @retval  false �ǉ���Ώہi���Ƀ��[�g�T���p�|�C���g�����݁j
	 */
	bool IsRPointTarget( const long busPointOID );

	/**
	 * @brief   ���[�g�T���p�|�C���g�쐬�Ώۂ̃o�X��|�C���g�����擾����
	 * @param   ipFeature [in]  �o�X��|�C���g
	 * @param   busPoint    [out] �o�X��|�C���g�̃t�B�[���h���
	 * @retval  true  ���擾����
	 * @retval  false ���s
	 */
	bool GetBusPointInfo( const IFeaturePtr& ipFeature, std::list<CString>& busPoint ); 

	/**
	 * @brief   �o�X��|�C���g��OID����A�o�X���[�g�T���p�|�C���g�ǉ��pFeatureBuffer�Ɍ`��E�������Z�b�g
	 * @param   busStopPointOID      [out] �o�X��|�C���gObjectID
	 * @param   ipBusStopPointBuffer [out] �C���|�[�g�pFeatureBuffer
	 * @retval  true  �`��E�����Z�b�g����
	 * @retval  false ���s
	 */
	bool SetWriteBusRoutePoint( const long busStopPointOID, IFeatureBufferPtr& ipBusStopPointBuffer );

	/**
	 * @brief   �ŋߖT�����N��̍ŒZ�����̓_�����߂�
	 * @param   ipFeature [in] �o�X��|�C���g
	 * @retval  IPointPtr �o�X���[�g�T���p�|�C���g�쐬�ʒu�iNULL�F������Ȃ������j
	 */
	IPointPtr GetNearestPoint(const IFeaturePtr& ipFeature);

private:

	CString            m_strInFile;                  //!< �������ʃt�@�C�����X�g��
	CString            m_strImpLog;                  //!< �C���|�[�g���[�h���O�t�@�C����
	CString            m_strARPLog;                  //!< ���[�g�T���p�|�C���g���[�h���O�t�@�C����

	CString            m_strBusDB;                   //!< �o�X�֘A�f�[�^�x�[�X�ڑ���
	CString            m_strRoadDB;                  //!< ���H�f�[�^�x�[�X�ڑ���

	std::ofstream      m_ofsImp;                     //!< �C���|�[�g���O
	std::ofstream      m_ofsARP;                     //!< ���[�g�T���p�|�C���g���O

	std::vector<std::string> m_fileList;             //!< �������ʃt�@�C�����X�g

	bool               m_import;                     //!< �C���|�[�g���[�h
	bool               m_autoRPoint;                 //!< ���[�g�T���p�|�C���g���[�h

	IWorkspaceEditPtr  m_ipBusWorkspace;             //!< �o�X�֘A���[�N�X�y�[�X
	IWorkspacePtr      m_ipRoadWorkspace;            //!< ���H���[�N�X�y�[�X
	
	IFeatureClassPtr   m_ipBusStopPointFC;           //!< �o�X��|�C���g�t�B�[�`���N���X
	ITablePtr          m_ipBusLineMasterTable;       //!< �o�X�H���}�X�^�e�[�u��
	ITablePtr          m_ipBusStopSeqTable;          //!< �o�X��ԏ��Ǘ��e�[�u��
	IFeatureClassPtr   m_ipBusRoutePointFC;          //!< �o�X���[�g�T���p�|�C���g�t�B�[�`���N���X
	IFeatureClassPtr   m_ipRoadLinkFC;               //!< ���H�����N�t�B�[�`���N���X

	CFieldMap          m_busStopPointFM;             //!< �o�X��|�C���g�t�B�[���h�}�b�v
	CFieldMap          m_busLineMasterFM;            //!< �o�X�H���}�X�^�t�B�[���h�}�b�v
	CFieldMap          m_busStopSeqFM;               //!< �o�X��ԏ��Ǘ��t�B�[���h�}�b�v
	CFieldMap          m_busRoutePointFM;            //!< �o�X���[�g�T���p�|�C���g�t�B�[���h�}�b�v

	std::list<CString> m_columnNameList;             //!< �J�������i�[�p���X�g

	std::set<long>     m_existRouteNumbers;          //!< ����BUS_LINE_MASTER�ɑ��݂���n���ԍ����X�g
	std::set<std::pair<long, long>> m_existStopPole; //!< ����BUS_STOP_POINT�ɑ��݂���◯���R�[�h�ƕW���ԍ��̃y�A���X�g
};
