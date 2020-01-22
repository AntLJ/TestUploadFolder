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
#include <WinLib/ExcelHelper.h>
#include "BusPointCommon/RegexChk.h"
#include "BusPointCommon/Time.h"

/**
 * @class	CImportBusPoint
 * @brief	�o�X�f�[�^�̃`�F�b�N���C���|�[�g�̂��߂̃��C���N���X
 */
class CBusPointExcelChk
{
public:
	/**
	 * @brief	�R���X�g���N�^
 	 * @param	vmArgs [in] �e��I�v�V����
	 */
	CBusPointExcelChk( const Arguments& Args ){
		m_strInFile  = (Args.in_list).c_str();
		m_strChkLog  = (Args.check_log).c_str();
	};

	/**
	 * @brief	�f�X�g���N�^
	 */
	~CBusPointExcelChk(){};

	/**
	 * @brief	�e�폈�������s���邽�߂ɍŏ��ɌĂ΂��֐�
	 * @retval	true  ��������
	 * @retval	false �������s
	 */
	bool execute();

private:

	typedef std::map<CString, CString>     BUSCONTAINER; //!< �o�X���R�[�h�Ǘ��p�̃t�B�[���h���E�lmap
	typedef std::vector<BUSCONTAINER>      BUSCONTVEC;   //!< �o�X���R�[�h�Q��Vector
	typedef std::pair<CString, BUSCONTVEC> BUSSHEETPAIR; //!< �o�X���R�[�h�Q�ƃV�[�g����Pair
	typedef std::vector<BUSSHEETPAIR>      BUSSHEETVEC;  //!< �o�X�V�[�g�Q��Vector
	typedef std::map<CString, BUSSHEETVEC> BUSFILEMAP;   //!< �o�X�̎��Ǝ҃t�@�C������Excel�V�[�g�Q��Map

	typedef std::pair<CString, CString>    CCPAIR;
	typedef std::pair<CCPAIR, CCPAIR>      CCPAIRPAIR;


	/**
	 * @brief	�e�평����
	 * @retval	true  ��������
	 * @retval	false �������s
	 */
	bool Init();

	/**
	 * @brief	�������ʃt�@�C�����X�g��ǂݍ��݁A�t�@�C���p�X1�s�������X�g�Ɋi�[
	 * @retval	true  ��������
	 * @retval	false �������s
	 */
	bool LoadList();
	
	/**
	 * @brief	�񖼂����X�g�Ɋi�[
	 * @note	�e�V�[�g��e�t�@�C���ԂŖ���񖼂��h�ꂽ�ꍇ���l���A�Ǝ��Ɋi�[����
	 */
	void SetColumnName();

	/**
	 * @brief	�������ʃt�@�C������A���R�[�h�����t�@�C�����E�V�[�g���ŊK�w�L�[�̃R���e�i�Ɋi�[
	 * @param	busInputs [out] �t�@�C�����E�V�[�g���ŊK�w�L�[�̒������ʃt�@�C�����R�[�h�R���e�i
	 */
	void LoadExcelData( BUSFILEMAP& busInputs );

	/**
	 * @brief	�������ʃt�@�C����1�s����A�R���e�i�ɓǂݍ���
	 * @param	ipWorkSheet [in]  �������ʃ��[�N�V�[�g
	 * @param	lineIdx     [in]  �s�ԍ�
	 * @param	busInput    [out] �������ʊi�[�R���e�i
	 * @retval	true  1�s�ǂݍ��ݐ���
	 * @retval	false 1�s�ǂݍ��ݎ��s
	 */
	bool GetLineFromSheet( const Excel::_WorksheetPtr& ipWorkSheet, long lineIdx, BUSCONTAINER& busInput );

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
	 * @brief	�Z���̒l��CString�ŕԂ�
	 * @param	vaValue   [in] �Z���̏���������CComVariant�^
	 * @retval	retStr    CString�^�̃Z���̒l
	 */
	CString CellToString( const CComVariant& vaValue )
	{
		CString retStr;
		if( VT_R8 == vaValue.vt){
			retStr.Format(_T("%.lf"), vaValue.dblVal);				
		}
		else{
			retStr = (VT_BSTR == vaValue.vt)? vaValue.bstrVal : _T("");
		}
		return retStr;
	};

	/**
	 * @brief	�������ʃt�@�C���`�F�b�N
	 * @param	busInputs [in] �t�@�C�����E�V�[�g���ŊK�w�L�[�̒������ʃt�@�C�����R�[�h�R���e�i
	 * @retval	true  �G���[����
	 * @retval	false �G���[�L��
	 */
	bool InputCheck( const BUSFILEMAP& busInputs );

	/**
	 * @brief	�P�ꃌ�R�[�h���`�F�b�N
	 * @note	NULL�s����, �T�C�Y�I�[�o�[, �t�B�[���h�Ԑ��� ��
	 * @param	busInput [in]  �������ʊi�[�R���e�i
	 * @param	errMsgs  [out] �G���[���b�Z�[�W��list
	 * @retval	true  �G���[����
	 * @retval	false �G���[�L��
	 */
	bool CheckByLine( const BUSCONTAINER& busInput, std::list<CString>& errMsgs );

	/**
	 * @brief	�V�[�g���`�F�b�N
	 * @note	�V�[�P���X����, ���R�[�h�ԕs���� ��
	 * @param	busInputs [in]  �������ʊi�[�R���e�i
	 * @param	errMsgs   [out] �G���[���b�Z�[�W��list
	 * @retval	true  �G���[����
	 * @retval	false �G���[�L��
	 */
	bool CheckBySheet( const BUSSHEETPAIR& busInputs, std::list<CString>& errMsgs ); 

	/**
	 * @brief	�t�@�C�����`�F�b�N
	 * @note	����◯���ł̃t�B�[���h�s����
	 * @param	busInputs [in]  �������ʊi�[�R���e�i
	 * @param	errMsgs   [out] �G���[���b�Z�[�W��list
	 * @retval	true  �G���[����
	 * @retval	false �G���[�L��
	 */
	bool CheckByFileSameStop( const std::map<CCPAIR, BUSCONTVEC>& busInputs, std::list<CString>& errMsgs ); 

	/**
	 * @brief	�t�@�C�����`�F�b�N
	 * @note	����ܓx�o�x�Œ◯���R�[�hor�W���ԍ��m�F
	 * @param	busInputs [in]  �������ʊi�[�R���e�i
	 * @param	errMsgs   [out] �G���[���b�Z�[�W��list
	 * @retval	true  �G���[����
	 * @retval	false �G���[�L��
	 */
	bool CheckByFileSameCoord( const std::map<CCPAIR, BUSCONTVEC>& busInputs, std::list<CString>& errMsgs ); 

private:

	CString            m_strInFile;         //!< �������ʃt�@�C�����X�g��
	CString            m_strChkLog;         //!< �`�F�b�N���[�h���O�t�@�C����

	std::ofstream      m_ofsChk;            //!< �`�F�b�N���O

	std::vector<std::string> m_fileList;    //!< �������ʃt�@�C�����X�g

	std::list<CString> m_columnNameList;    //!< �J�������i�[�p���X�g
};
