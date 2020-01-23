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

#include <stdio.h>
#include <vector>
#include <set>
#include <string>
#include <coord_converter.h>
#include "exec_time_handle.h"
#include <atlstr.h>
#include <SiNDYLib/Workspace.h>

#ifndef	_MAIN_ROUTINE_H_
#define	_MAIN_ROUTINE_H_

#define     ON_UPDATEMODE  0x01 //!< �X�V���[�h�w��i�f�t�H���g�̓`�F�b�N���[�h�j
#define     ON_RPARAM      0x02 //!< �n��p�����[�^�ϊ�

#define	RET_COUNT   0x01
#define	RET_CURSOR  0x02

#define  ERR_STAT  _T("ERROR\t")

/**
 * @brief ���C�������N���X
 */
class	main_routine
{
	/**
	 * @brief �֌W��`�p
	 */
	enum {
		contains = 1,
		overlaps,
		within,
		touches,
		equals,
		crosses
	};

	public:
		//------------------------------------------------------------------------
		std::string   m_sDB_Connect1;   //!< DB�ڑ����P
		std::string   m_sFeatureClass1; //!< �t�B�[�`���N���X�P����
		std::string	  m_sLog_File;      //!< ���O�f�[�^�o�̓p�X
		std::string   m_sExeLog_File;   //!< ���s���O�o�̓p�X
		std::string   m_sParam_File;    //!< �n��p�����[�^�t�@�C��
		std::string	  m_sRel_Str;       //!< �֌W�w�蕶����
		long          m_lRel_Type;      //!< �w�肵���I�u�W�F�N�g���m�̊֌W�̌`��

		std::set<long>    m_sOBJECTID_List;   //!< OBJECTID���X�g

		IFeatureClassPtr  m_ipFeatureClass_1; //!< �t�B�[�`���N���X�P

		IWorkspacePtr     ipWorkspace_1;      //!< ���[�N�X�y�[�X

		IWorkspaceEditPtr m_ipWorkspaceEdit_1;//!< �G�f�B�b�g�p

		esriGeometryType  m_ipGeom_Type; //!< �W�I���g���^�C�v

		//------------------------------------------------------------------------
		long					m_lExe_Flag;
		FILE*					m_fErr_Fp;  //!< �G���[���O�t�@�C���p�t�@�C���|�C���^
		FILE*					m_fExe_Fp;  //!< �G���[���O�t�@�C���p�t�@�C���|�C���^
	private:
		exec_time_handle		m_eExeTime; //!< ���s���ԊǗ�

		crd_cnv					m_cCrd;
	private:
		CString       m_sVersion;  //!< �c�[���o�[�W����

	public:
		//!< �R���X�g���N�^
		main_routine () {
			m_lExe_Flag		= 0;
			m_sRel_Str      = "contains";
			m_lRel_Type     = contains;
			m_fErr_Fp		= stderr; // �f�t�H���g�͕W���G���[�o�͂�
			m_fExe_Fp		= stderr; // �f�t�H���g�͕W���G���[�o�͂�
		}
		//!< �f�X�g���N�^
		~main_routine () {
			if( m_fErr_Fp != NULL ) {
				fclose ( m_fErr_Fp );
			}
			if( m_fExe_Fp != NULL ) {
				fclose ( m_fExe_Fp );
			}
		}

		/**
		 * @brief DB�ւ̐ڑ��I�[�v��
		 * @return  true   ����
		 * @return  false  ���s
		 */
		bool	DB_Open	(
		            const CString& c_csConnectStr, //!< [in] �ڑ�������
		            IWorkspacePtr& c_ipWorkspace   //!< [in,out] Workspace�̎Q��
		        );

		/**
		 * @brief ���s���O�t�@�C�����I�[�v������
		 * @return true ����
		 * @return  false  ���s
		 */
		bool    Open_Exe_Log ( void );

		/**
		 * @brief �G���[���O�t�@�C�����I�[�v������
		 * @return true ����
		 * @return  false  ���s
		 */
		bool    Open_Err_Log ( void );

		/**
		 * @brief �c�[���̎g�p�@�̕\��
		 */
		void    print_usage (
		            char** argv //!< [in] ������
		        );

		/**
		 * @brief �����`�F�b�N
		 * @return	����	true
		 * @return	����	false
		 */
		bool    arg_check (
		            int    argc, //!< [in] ������
		            char** argv  //!< [in] ������
		        );

		/**
		 * @brief ���X�g�̓ǂݍ���
		 * @return	����	true
		 * @return	����	false
		 */
		bool    Read_List (
		            const std::string& c_sFileName, //!< [in] �t�@�C����
		            std::set<long>&    c_sList      //!< [in] ���X�g�iID���Ɏg�p�j
		        );

		/**
		 * @brief �w��t�B�[�`���ɑ΂���esriSpatialRelEnum�Ŏw�肵���֌W�����f�[�^���Ƃ��Ă���
		 * @return IFeatureCursorPtr
		 */
		IFeatureCursorPtr   Search_Feature (
		                        const IFeatureClassPtr& c_ipFeCls,     //!< [in] �t�B�[�`���N���X
		                        const IGeometryPtr&     c_ipGeo,       //!< [in] �����p�W�I���g��
		                        esriSpatialRelEnum      c_ipSpatialRel,//!< [in] �֌W�w��
		                        std::string             c_sWhere,      //!< [in] Where��w��
		                        long&                   c_iCount       //!< [out] ������
		                    );

		/**
		 * @brief �I�u�W�F�N�gID���X�g�̍쐬
		 * @return	����	true
		 * @return	����	false
		 */
		bool    MakeOBJECTIDList (
		            const IFeatureClassPtr& c_ipFeCls,        //!< [in] �t�B�[�`���N���X
		            std::set<long>&         c_sOBJECTID_List  //!< [out] OBJECTID���X�g
		        );

		/**
		 * @brief �G�f�B�b�g�̊J�n
		 * @return	����	true
		 * @return	����	false
		 */
		bool    StartEdit ( void );

		/**
		 * @brief �G�f�B�b�g�̏I��
		 * @return	����	true
		 * @return	����	false
		 */
		bool    StopEdit ( void );

		/**
		 * @brief ���̃I�u�W�F�N�g�ɑ΂���^�[�Q�b�g�̊֌W�`�F�b�N
		 * @return �w��֌W������   true
		 * @return �w��֌W���s���� false
		 */
		bool    RelCheck ( 
		            const IGeometryPtr& c_ipSrcGeom, //!< [in] ��r���W�I���g��
		            const IGeometryPtr& c_ipTgtGeom, //!< [in] ��r��W�I���g��
		            long                c_lRel_Type  //!< [in] �֌W�̃^�C�v
				);

		/**
		 * @brief ��\�_���擾
		 * @return 
		 * @return	����	true
		 * @return	����	false
		 */
		bool   GetPoint (
		           const IGeometryPtr& c_ipGeom, //!< [in] �W�I���g��
		           double&             c_dX,     //!< [out] �o�x
		           double&             c_dY      //!< [out] �o�x
		       );


		/**
		 * @brief ��������w�肵�A�A�b�v�f�[�g�p�̃J�[�\��������ė���
		 * @return	����	true
		 * @return	���s	false
		 */
		bool    Update_Where (
		            const IFeatureClassPtr&  c_ipTable,  ///< [in] �������e�[�u��
			        CString&                 c_sWhere,   ///< [in] �w��WHERE��
		            int                      c_iRetType, ///< [in] ����Ԃ���
		            long&                    c_lCount,   ///< [out] �J�E���g
		            IFeatureCursorPtr&       c_ipCursor  ///< [out] �J�[�\��
		        );

		/**
		 * @brief JGD�ڍs�����W�I���g�����쐬����
		 * @return IGeometryPtr
		 */
		IGeometryPtr Make_JGD_Geometry (
		                 IGeometryPtr& c_ipGeom //!< [in] �W�I���g��
		             );

		/**
		 * @brief �ϊ����s��
		 * @note �������ŃG���[�t�@�C���̏��������s��
		 * @return	����	true
		 * @return	����	false
		 */
		bool update_exec ( void );
		bool update_exec_nolist ( void ); // �����Ń��X�g�쐬���Ȃ�
		
		//--------------------------------------------------------------------------------
		/**
		* @brief �f�[�^�̃R���e�i���쐬����
		* @return	�f�[�^��
		*/
		//--------------------------------------------------------------------------------
		int				Make_Data_Container (
							const IFeatureCursorPtr&							    c_ipFCursor,	//!< [IN] ���L�J�[�\��
							std::map<long, CAdapt<IFeaturePtr>, std::less<long> >&	c_mWork_Data	//!< [IN] ��Ɨp�f�[�^
						);
		
		//--------------------------------------------------------------------------------
		/**
		* @brief �G���[���b�Z�[�W
		*/
		//--------------------------------------------------------------------------------
		void	PrnMsg	( FILE* c_cpFp, FILE* c_cpFp2, const char* c_cpFormat, ... ) {
							
					va_list	a_vArgList = NULL;

					va_start(a_vArgList, c_cpFormat );	// ������

					vfprintf_s ( c_cpFp, c_cpFormat, a_vArgList );
					fflush	( c_cpFp );
					if( c_cpFp2 != NULL && c_cpFp != c_cpFp2 ) {
						vfprintf_s ( c_cpFp2, c_cpFormat, a_vArgList );
						fflush	( c_cpFp2 );					
					}
				}
};
//------------------------------------------------------<�����܂�>

#endif	//_MAIN_ROOTIN_H_
