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

// sindylib_core,sindylib_base
#include <TDC/sindylib_core/Workspace.h>
#include <TDC/sindylib_base/TableFinder.h>
#include <TDC/sindylib_base/TableContainer.h>
#include <TDC/sindylib_base/RowContainer.h>

// illustlib
#include "TDC/illustlib/IllustLinkRow.h"
#include "TDC/illustlib/IllustLinkTable.h"
#include "TDC/illustlib/IllustNameCreator.h"
#include "TDC/illustlib//IllustOutputInfo.h"

#include <sindy/schema/illust.h>


/**
 * @brief �m�[�h���X�g�쐬�N���X
 */
class CNodelistCreator
{
private:
	/**
	 *	@brief �m�[�hID�ƃ��b�V���ԍ��̑Ή����R�[�h
	 *	@note  �����N��o�͏����Ŏg�p
	 */
	struct NodeInfo
	{
		long          id;	//!< �m�[�hID
		std::set<int> meshes; //!< 2�����b�V���R�[�h

		NodeInfo():id(-1){}

		// ���b�V���R�[�h�ǉ�
		void addMesh( int nMesh ){ meshes.insert(nMesh); }	
		// 2�����b�V����̃m�[�h���ۂ�
		bool isOn2ndMesh() const { return meshes.size() >= 2; }	

		// ���ʂ̃��b�V���R�[�h�擾
		int getCommonMesh( const NodeInfo& info ) const {
				std::set<int> common_mesh;
				std::set_intersection(std::begin(meshes),std::end(meshes),std::begin(info.meshes),std::end(info.meshes), std::inserter(common_mesh, std::end(common_mesh)));
				if( common_mesh.size() != 1 )
					return -1;
				return *common_mesh.begin();
		}
		// ���b�V���R�[�h�Q�̔�r
		bool isEqualMesh( const NodeInfo& info ) const { return meshes == info.meshes; }
	};

public:
	/**
	 * @brief �R���X�g���N�^
	 */
	CNodelistCreator(void){}
	/**
	 * @brief �f�X�g���N�^
	 */
	~CNodelistCreator(void){}

public:

	/**
	 * @brief ������
	 * @param[in]  strDBConnect  DB�ڑ��ݒ�
	 * @param[in]  strOutputPath �m�[�h���X�g�o�̓t�H���_�p�X
	 * @param[in]  strLatLon     �ܓx�E�o�x�o�͐ݒ�
	 * @param[out] strErrMsg     �G���[���b�Z�[�W
	 * @retval     true          ����������
	 * @retval     false         ���������s
	 */
	bool init( const CString& strDBConnect, const CString& strOutputPath, const CString& strLatLon, CString& strErrMsg );
	/**
	 * @brief  ������(���H�m�[�h�̐ڑ���DB���C���X�g�����N�Ƃ͕ʂɎw��)
	 * @detail ILLUST_LINK��ROAD_NODE�͓����f�[�^�Z�b�g�ɑ��݂���z�肾���A�@�\�Ƃ��Ă͗p�ӂ��Ă���
	 * @param[in]  strDBConnect		 DB�ڑ��ݒ�(ILLUST_LINK)
	 * @param[in]  strRoadDBConnect  DB�ڑ��ݒ�(ROAD_NODE)
	 * @param[in]  strOutputPath     �m�[�h���X�g�o�̓t�H���_�p�X
	 * @param[in]  strLatLon        �ܓx�E�o�x�o�͐ݒ�
	 * @param[out] strErrMsg         �G���[���b�Z�[�W
	 * @retval     true              ����������
	 * @retval     false             ���������s
	 */
	bool init( const CString& strDBConnect, const CString& strRoadDBConnect, const CString& strOutputPath, const CString& strLatLon, CString& strErrMsg );

	/**
	 * @brief �m�[�h���X�g�쐬
	 * @param[in]  info      �m�[�h���X�g�o�͏��
	 * @param[in]  rule      �C���X�g���̐������[��
	 * @param[out] strErrMsg  �G���[���b�Z�[�W
	 * @retval     true       �m�[�h���X�g�쐬����
	 * @retval     false      �m�[�h���X�g�쐬���s
	 */
	bool create( const CIllustOutputInfo::OutputInfo info, const std::vector<CString>& rule, CString& strErrMsg );

private:

	/**
	 * @brief  �w��񐔂���DB�ւ̐ڑ����g���C����
	 * @param[in]   strDBConnect  DB�ڑ����
	 * @param[in]   nTryCount     �ڑ����g���C����� (ex. 3���w�肷��Ɛڑ��Ɏ��s�����ꍇ�A2��܂Ń��g���C����)
	 * @param[out]  ipWorkspace   �ڑ���̃��[�N�X�y�[�X���
	 * @retval     true           �ڑ�����
	 * @retval     false          �ڑ����s
	 */
	bool connectSeveralTimes( const CString& strDBConnect, int nTryCount, sindy::CWorkspace& cWorkspace );

	/**
	 * @brief  �ݒ�t�@�C���L�ڂ̎w��t�B�[���h�̒l���擾����
	 * @detail �w��t�B�[���h��"ILLUST_NAME"�Ǝw�肳��Ă����ꍇ�́A�t�B�[���h�l�ł͂Ȃ��C���X�g�����o�͂���
	 * @param[in]  cNameCreator   �C���X�g�������I�u�W�F�N�g
	 * @param[in]  spRow          ILLUST_LINK 1���R�[�h
	 * @param[in]  outputFields   �l���o�͂���Ώۃt�B�[���h���Q
	 * @param[out] OutString      �o�͒l�Q
	 * @retval     true           �l�̎擾����
	 * @retval     false          �l�̎擾���s
	 */
	bool getFieldValue( sindy::CIllustNameCreator cNameCreator, sindy::CSPIllustLinkRow spRow , const std::vector<CString>& OutputFields, std::vector<CString>& OutString );

	/**
	 * @brief  �t�B�[���h�̒l��ϊ����ĕԂ�
	 * @detail �C���X�g��ʂ���������̂ݑΏ�
	 * @param[in]  spRow          ILLUST_LINK 1���R�[�h
	 * @param[in] strFieldName   �t�B�[���h��
	 * @return    �ϊ���̒l
	 */
	CString convFieldValue( sindy::CSPIllustLinkRow spRow, const CString& strFieldName );

	/**
	 * @brief  �ܓx�o�x���o�͂���
	 * @detail init�֐��ňܓx�o�x�̏o�͎w�肪����Ă��Ȃ��ꍇ�͏o�͂��Ȃ�
	 * @param[in]  spRow          ILLUST_LINK 1���R�[�h
	 * @param[out] OutString      �o�͒l�Q
	 * @retval     true   �o�͐���
	 * @retval     false  �o�͎��s
	 */
	bool getLatLon( sindy::CSPIllustLinkRow spRow, std::vector<CString>& OutString );

	/**
	 * @brief  �����N����o�͂���
	 * @detail 2�����b�V�����E��̃m�[�h�̏ꍇ�͗����b�V����2�m�[�h�����o�͂���
	 * @detail 2�m�[�h�̏o�͏���1�O�̃m�[�h���画�f����
	 * @param[in]  spRow          ILLUST_LINK 1���R�[�h
	 * @param[in]  nStartNode     �o�͊J�n�m�[�h
	 * @param[in]  nEndNode       �o�͏I���m�[�h
	 * @param[out] OutString      �o�͒l�Q
	 * @retval     true   �o�͐���
	 * @retval     false  �o�͎��s
	 */
	bool getLQ( sindy::CSPIllustLinkRow spRow, int nStartNode, int nEndNode, std::vector<CString>& OutString );

	/**
	 * @brief   �m�[�hID���烁�b�V���R�[�h�Q���擾����
	 * @param[in]  lNodeId  �m�[�hID
	 * @return     ���b�V���R�[�h
	 */
	std::vector<int> getMeshcodes( long lNodeId );

	/**
	 * @brief   �m�[�hID��������ܓx�o�x���擾����
	 * @param[in]  lNodeId  �m�[�hID
	 * @return     �����ܓx�o�x
	 */
	WKSPoint getLatLonByNodeId( long lNodeId );

	/**
	 * @brief   �擪�m�[�h��1�O�̃m�[�h�������郁�b�V���R�[�h���擾����
	 * @detail  �擪�m�[�h��2�����b�V����ɑ��݂���ꍇ���l������
	 * @param[in]  infos  ILLUST_LINK�̃����N��(�m�[�hID�Q)
	 * @param[in]  iter   ILLUST_LINK�̃����N��(�m�[�hID�Q)�̐擪�C�e���[�^
	 * @return     ���b�V���R�[�h
	 */
	int getPreRoadNodeMeshcode( std::vector<NodeInfo>& infos,  std::vector<NodeInfo>::iterator iter );

	/**
	 * @brief   ILLUST_LINK�̃����N��(�m�[�hID�Q)����ID���i�[����Ă��閖���t�B�[���h�̃C���f�b�N�X���擾����
	 * @param[in]  spRow  ILLUST_LINK 1���R�[�h
	 * @return     �i�[����Ă��閖���̃C���f�b�N�X(�t�B�[���h��"ROAD_NODE_XX"��"XX")
	 */
	int getLastEnableNodeField( sindy::CSPIllustLinkRow spRow ); 

	/**
	 * @brief  �m�[�h��2�����b�V����ɂ��邩�𔻒肷��
	 * @param[in]  lNodeId  �m�[�hID
	 * @retval     true   2�����b�V����ɂ���
	 * @retval     false  2�����b�V����ɂȂ�  
	 */
	bool isOn2ndMesh( long lNodeId );

	/**
	 * @brief  �m�[�h���X�g�o�̓X�g���[�����擾����
	 * @detail ���Ƀt�@�C�������݂��Ă���ꍇ�́A�ǋL���[�h�Ŏ擾����
	 * @param[in]  strFolderPath  �o�̓t�H���_
	 * @return  �o�̓X�g���[��
	 */
	std::ofstream getOutputFileStream( const CString& strFolderPath );

private:

	sindy::CTableFinder m_ipTableFinder;		//!< �e�[�u���t�@�C���_
	CString             m_strOutputPath;		//!< �m�[�h���X�g�o�̓t�H���_�p�X
	CString             m_LatLon;				//!< �ܓx�o�x�o�͐ݒ�
};
