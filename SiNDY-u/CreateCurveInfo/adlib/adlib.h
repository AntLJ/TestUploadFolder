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
#include <set>
#include <string>
#include <boost/function.hpp>

/**
 * @brief �l�I�ɂ悭�����֐��Q���A�u����ɍ����ĂƂ肠�����܂Ƃ߂�����
 * @note	TODO: �����������̂�WinLib�ɒu���ׂ��Ƃ̎��������̂ŁA������ֈړ��\��B
 */
namespace adlib
{
	//----- �ėptypedef -----//
	typedef long OID;					///< �I�u�W�F�N�gID
	typedef long NodeID;
	typedef long LinkID;
	typedef std::set<LinkID> LinkIDs;

	typedef long MeshCode;
	typedef std::set< MeshCode > MeshCodes;

	typedef std::basic_string<TCHAR> tstring;	///< std::string��TCHAR��

	/**
	 * @brief ���t�Ɖ񐔂�t�^���Ċ����t�@�C���Ɣ��Ȃ��t�@�C�������쐬����
	 * @note	���̃t�@�C�����̊g���q�̑O�Ɂu_yyyy-mm-dd_n�v��}���Bn��1�`cMaxCount�܂ł̐��̂����A�����t�@�C�����Ɣ��Ȃ��ŏ��̐��B
	 *			�o�T�FSiNDYQToMPQ::editLogFileName()
	 * @param[in,out]	cFilePath	�Ώۃt�@�C���p�X
	 * @param[in]		cMaxCount	�t�^����ԍ��̍ő�l�i�f�t�H���g��1000�j
	 */
	// TODO: �񐔂ł͂Ȃ��A�b�P�ʂ̎��Ԃ�t�^���������֗��B
	void identifiedFileName( boost::filesystem::path& cFilePath, long cMaxCount = 1000 );

	/**
	 * @brief FREESTYLELOG�w�b�_���o��
	 * @note	�o�T�FSiNDYQToMPQ�v���W�F�N�g��Utility.h
	 * @param[in]	�o�̓X�g���[��
	 * @param[in]	�w�b�_�̂Q�s�ڂɗ񋓂���t�B�[���h��������̏W��
	 */
	void outputSindyFreeStyleLogHeader( std::ostream& co, std::vector<std::string> cFieldNames );

	/**
	 * @brief ���P�[�����u���{�ꃍ�P�[���̐��l��؂�t�@�Z�b�g������C���P�[���ɕύX�������P�[���v�ɕύX����
	 * @note	http://www.freeml.com/cppll_novice/1550/latest ���B
	 *			���{�ꃍ�P�[�����g���������ǐ��l���J���}�ŋ�؂��Ă͍���Ƃ����ꍇ�Ɏg�p�B
	 *			�o�T�FSiNDYQToMPQ�v���W�F�N�g��Utility.h
	 */
	void setJapaneseLocaleWithCNumpunctFacet(void);

	/**
	 * @brief ���[�N�X�y�[�X�̎擾
	 * @note	�o�T�FSiNDYQToMPQ
	 * @param[in]	cDBStr	�ڑ���w�蕶����
	 * @param[in]	cErrorFunc	���s���ɌĂяo���֐�
	 * @return	�擾�������[�N�X�y�[�X
	 */
	IWorkspacePtr getWorkspace( tstring cDBStr, boost::function<void (std::string)> cErrorFunc );

	/**
	 * @brief �t�B�[�`���N���X�̎擾
	 * @note	�o�T�FSiNDYQToMPQ
	 * @param[in]	cpWorkspace	�擾�����[�N�X�y�[�X
	 * @param[in]	cTableName	�擾����t�B�[�`���N���X�̃e�[�u����
	 * @param[in]	cErrorFunc	���s���ɌĂяo���֐�
	 * @return	�擾�����t�B�[�`���N���X
	 */
	IFeatureClassPtr getFeatureClass(IWorkspace* cpWorkspace, tstring cTableName, boost::function<void (std::string)> cErrorFunc);

	/**
	 * @brief �����񂪂Q�����b�V���R�[�h�炵�����i6���̐����j�ł��邩�ۂ��𔻒肷��
	 * @note	�o�T�FSiNDYQToMPQ
	 * @param[in]	cStr	�Ώە�����
	 * @return	���茋��
	 */
	bool isBaseMeshCode( std::string cStr );

	typedef long BaseMeshCode;
	typedef std::set<BaseMeshCode> BaseMeshList;
	/**
	 * @brief �Q�����b�V�����X�g�ǂݍ���
	 * @note	���b�V�����X�g�t�@�C�����烁�b�V���R�[�h�Q��ǂݍ��݁A�R���e�i�Ɋi�[���ĕԂ�
	 *			�o�T�FSiNDYQToMPQ
	 * @param[in]	cBaseMeshListFilePath	�Ώۃ��b�V�����X�g�̃t�@�C���p�X
	 * @param[in]	cOpenErrorFunc			�t�@�C�����J���Ȃ������ꍇ�ɌĂяo���֐�
	 * @param[in]	cFormatErrorFunc		�t�@�C���̓��e���s���ȏꍇ�ɌĂяo���֐�
	 */
	BaseMeshList loadMeshList(
		boost::filesystem::path cBaseMeshListFilePath,
		boost::function<void (std::string)> cOpenErrorFunc,
		boost::function<void (long)> cFormatErrorFunc );

	/**
	 * @brief ���b�V���t�B�[�`���N���X����w�肳�ꂽ���b�V���R�[�h�������b�V���t�B�[�`�����擾����
	 * @note	�o�T�FSiNDYQToMPQ::getMeshFeature()
	 * @param[in]	cpBaseMeshClass	�Ώ�BaseMesh�t�B�[�`���N���X
	 * @param[in]	cMeshCode		�ΏۂQ�����b�V���R�[�h
	 * @return	���b�V���t�B�[�`���i�擾�ł��Ȃ������ꍇ��NULL�j
	 */
	IFeaturePtr getMeshFeature( IFeatureClassPtr cpBaseMeshClass, long cMeshCode );

	/**
	 * @brief �t�B�[���h�C���f�b�N�X��Ԃ��i[Bug8736]�Œǉ��j
	 * @note	�o�T�FSindyHighwayCheck��hwy_check_utility::fieldIndex()
	 *			TODO: Row�ł͂Ȃ�Table�������Ɏ��ł������K�v�B
	 *			TODO: �Ăяo�����̉ǐ�����̂��߁A�e�[�u�������w�肵�Ȃ��Ă������悤�ɂ������BAheGetOwnerTableName()���g�p�H
	 * @param[in]	cpRow		�Ώ�Row
	 * @param[in]	cTableName	�e�[�u����
	 * @param[in]	cFieldName	�t�B�[���h��
	 * @return	�t�B�[���h�C���f�b�N�X
	 */
	long fieldIndex( _IRowPtr cpRow, tstring cTableName, tstring cFieldName );

	/**
	 * @brief �l�̌ܓ������l��Ԃ�
	 * @note	�u���̐��̎l�̌ܓ��v�̒�`�͖��m�Ɍ��܂��Ă��Ȃ��炵�����A�����ł͐��̏ꍇ�Ɠ�����Βl�ɂȂ�悤�ɒ�`����B
	 * @param[in]	cNum	�Ώےl
	 * @return	����
	 */
	long roundOff( double cNum );

	/**
	 * @brief Road_Link.UpDownClass_C��HWY���ۂ��𔻒肷��
	 * @param[in]	cUpDownClass	�Ώ�UpDownClass_C
	 * @return	���茋��
	 */
	bool isHwy( long cUpDownClass );

	/**
	 * @brief �Ώۃ����N��HWY���ۂ��𔻒肷��
	 * @note	������Road_Link.UpDownClass_C�B
	 * @param[in]	cpLink	�Ώۃ����N
	 * @return	���茋��
	 */
	bool isHwy( IFeaturePtr cpLink );

	/**
	 * @brief �w�肳�ꂽ���b�V���̎���9���b�V���̃��b�V���R�[�h�Q��Ԃ�
	 * @param[in]	cMeshCode	�Ώۃ��b�V��
	 * @return	���b�V���R�[�h�Q
	 */
	MeshCodes surroundingMeshCodes( long cMeshCode );

	/// ���郁�b�V���ɑ΂���A���̂����P�̃��b�V���̈ʒu�֌W
	namespace mesh_relation
	{
		enum ECode
		{
			kSame,			///< ����
			kEast,
			kWest,
			kNorth,
			kSouth,
			kNorthEast,
			kNorthWest,
			kSouthEast,
			kSouthWest,
			kNone,			///< �ӂł��_�ł��ڐG���Ă��Ȃ�
		};
	} // namespace mesh_relation

	/// �Q�����b�V���R�[�h�v�Z�N���X
	class CMeshCode
	{
	private:
		long m_MeshCode;	///< �Q�����b�V���R�[�h

	public:
		/// �R���X�g���N�^
		CMeshCode(
			long cMeshCode
		) :
			m_MeshCode( cMeshCode )
		{}

		/// ���b�V���R�[�h��Ԃ�
		long code() const { return m_MeshCode; }

		/**
		 * @brief ����1���b�V���ړ�
		 * @return	�ړ���̎��g
		 */
		CMeshCode& east();

		/**
		 * @brief ����1���b�V���ړ�
		 * @return	�ړ���̎��g
		 */
		CMeshCode& west();

		/**
		 * @brief �k��1���b�V���ړ�
		 * @return	�ړ���̎��g
		 */
		CMeshCode& north();

		/**
		 * @brief ���1���b�V���ړ�
		 * @return	�ړ���̎��g
		 */
		CMeshCode& south();
#if 0	// TODO: �K�v�Ȃ���
		/**
		 * @brief ���ֈړ�
		 * @param[in]	cDelta	�ړ���
		 * @return	�ړ���̎��g
		 */
		CMeshCode& east( long cDelta );

		/**
		 * @brief �k�ֈړ�
		 * @param[in]	cDelta	�ړ���
		 * @return	�ړ���̎��g
		 */
		CMeshCode& north( long cDelta );

		/**
		 * @brief ���ֈړ�
		 * @param[in]	cDelta	�ړ���
		 * @return	�ړ���̎��g
		 */
		CMeshCode& west( long cDelta ) { return east( -cDelta ); }

		/**
		 * @brief ��ֈړ�
		 * @param[in]	cDelta	�ړ���
		 * @return	�ړ���̎��g
		 */
		CMeshCode& south( long cDelta ) { return north( -cDelta ); }
#endif

		/**
		 * @brief �w�肳�ꂽ���b�V���Ƃ̈ʒu�֌W�𔻒肷��
		 * @param[in]	cMeshCode	�Ώۃ��b�V��
		 * @return	�ʒu�֌W
		 */
		mesh_relation::ECode relation( long cMeshCode );
	};


} // namespace adlib
