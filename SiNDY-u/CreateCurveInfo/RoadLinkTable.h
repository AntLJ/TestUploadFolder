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

/**
 * @note
 * RoadLinkTable.h/cpp��LQEditTool����̃R�s�[���ꕔ�C���������́B
 * ������sindylib_base�̈ꕔ�Ƃ��Ă����ꃉ�C�u�����������\�肾���A
 * ���̑O��LQEditTool�ō��ꂽ���̂��ė��p�����������̂ŁA�b��[�u�Ƃ��ăR�s�[���g�p�B
 * TODO: ��Ƀ��C�u���������ꂽ�炻����ɒu��������B
 */
#pragma once

#include "TableContainer.h"

namespace sindy {

/**
 * @brief ���C�u����������������
 *
 * RowClassFactory�ATableClassFactory��u�������܂�
 */
void init_sindynlib();

/**
 * @brief ���C�u�����̌�n��������
 *
 * �O���[�o���ɂ����Ă���SiRiUS���W���[���̌�n�������܂�
 * �A�v���P�[�V������DLL���I������O�ɕK���ĂԕK�v������܂��B
 */
void uninit_sindynlib();

/**
 * @brief �����N��^�C�v
 */
namespace linkqueue_type {
	enum ECode {
		kNone         =    0, //!< �Ȃ�
		kLRTurn       =    1, //!< �E���܋֎~
		kGuide        =    2, //!< �����U��
		kDirguide     =    4, //!< ���ʈē�
		kVICS         =    8, //!< VICS
		kLane         =   16, //!< ���[�����
		kByway        =   32, //!< ������
		kRoute        =   64, //!< �A�N�Z�X�^���p����
		kUTurn        =  128, //!< U�^�[���\�ӏ�
		kInternavi    =  256, //!< �C���^�[�i�rVICS
		kIPCVICS      =  512, //!< IPC VICS
		kIntersection = 1024, //!< �i�������ʌ����_
		kAll          = 2047, //!< �S��
	};
} // linkqueue_type

#define CAST_ROW boost::static_pointer_cast<CRowContainer>

/**
 * @brief ���H�l�b�g���[�N�p�̃��f�����쐬����
 *
 * model�ɓ��H�l�b�g���[�N�p�̃��f���i�֘A�j���`���܂��B
 * @note ���̂Ƃ���A��`����Ă���͓̂��H�ƃ����N��݂̂ł��B
 *
 * @param model [out] CModel
 */
void GetSindynModel( CModel& model );

/** 
 * @biref linkqueue_type ���� LQ�e�[�u�����̃��X�g�ɕϊ�����
 */
std::list<LPCTSTR> linkqueuetype2lqtablename( linkqueue_type::ECode emCode );

/**
 * @brief LQ�e�[�u��������INF�e�[�u�����ɕϊ�����
 */
LPCTSTR lq2inf( LPCTSTR lpcszLQ );

/**
 * @brief INF�e�[�u��������LQ�e�[�u�����ɕϊ�����
 */
LPCTSTR inf2lq( LPCTSTR lpcszInf );

/**
 * @brief ���H�����N�Ǘ��N���X
 */
class CRoadLinkTable : public CTableContainer
{
public:
	explicit CRoadLinkTable() : CTableContainer() {}
	virtual ~CRoadLinkTable(){}
	CRoadLinkTable( const CRoadLinkTable& obj ) : CTableContainer( obj ){}
	CRoadLinkTable( ITable* lp, const CTableFinder& cFinder ) : CTableContainer( lp, cFinder ){}
	CRoadLinkTable( IFeatureClass* lp, const CTableFinder& cFinder ) : CTableContainer( lp, cFinder ){}
	CRoadLinkTable( sindyTableType::ECode emTableType, ITable *lp, const CTableFinder& cFinder ) : CTableContainer( emTableType, lp, cFinder ){}
	CRoadLinkTable( sindyTableType::ECode emTableType, IFeatureClass *lp, const CTableFinder& cFinder ) : CTableContainer( emTableType, lp, cFinder ){}
	CRoadLinkTable& operator =( ITable* lp ){ return static_cast<CRoadLinkTable&>(CTableContainer::operator=(lp)); }
	CRoadLinkTable& operator =( IFeatureClass* lp ){ return static_cast<CRoadLinkTable&>(CTableContainer::operator=(lp)); }
	CRoadLinkTable& operator =( const CTableBase& obj ){ return static_cast<CRoadLinkTable&>(CTableContainer::operator=(obj)); }
	CRoadLinkTable& operator =( const CRoadLinkTable& obj ){
		CTableContainer::operator=(obj);
		return *this;
	}
	operator ITable*() const { return (ITablePtr)p; }

	/**
	 * @brief sort�p�v���f�B�P�[�g�iLQ�̃V�[�P���X���Ƀ\�[�g�j
	 */
	class LessLQSequence
	{
	public:
		bool operator()( CSPContainerBase& c1, CSPContainerBase& c2 ) const;
	};

	/**
	 * @brief �ڑ�����Ă��邩�ǂ���
	 */
	bool IsConnected() const { return (ITable*)*this; }

	/**
	 * @brief ���H�����N��OBJECTID�Ŋ֘A�I�u�W�F�N�g����������
	 *
	 * @warning ���̂Ƃ���A�֘A�I�u�W�F�N�g�Ƃ��Ĉ����Ă���̂̓����N��݂̂ł��B
	 * as_start,start_node_id�͍��̂Ƃ��떢�����ł��B�Z�b�g���Ă����̌��ʂ�����܂���B
	 *
	 * @param listOID [in] ���H�����N��ID
	 * @param detach [in,optional] Row�I�u�W�F�N�g�̃C���X�^���X��؂藣���ꍇ��true�i�f�t�H���g�Ffalse�j
	 * @param unique [in,optional] �ǉ������̏ꍇ��true�i�f�t�H���g�Ffalse�j
	 * @param emType [in,optional] �����Ώۃ����N��i�f�t�H���g�Flinkqueue_type::kAll�j
	 * @param as_start [in,optional] oid�𗬓������NID�Ƃ�����̂Ɍ��肷��i�f�t�H���g�Ffalse�j
	 * @param start_node_id [in,optional] as_start = true �̏ꍇ�ɗ����̎n�_�ƂȂ�m�[�hID
	 */
	void Select( const std::list<long>&  listOID, bool detach = false, bool unique = false, linkqueue_type::ECode emType = linkqueue_type::kAll, bool as_start = false, long start_node_id = 0 );

	/**
	 * @brief ����Ă��Ȃ�LQ�̌���
	 * @note
	 * ���H�����N�̎q���Ƃ���LQ���A����ɂ��̎q���Ƃ���INF���o�^����Ă���O��B
	 * ROAD_LINK
	 *         |- LQ_*   <- ���H�Ɋ֘A����LQ�̂�
	 *                |- INF_*
	 * 
	 * �����܂łŁA���H�Ɋ֘A����LQ�ƁA����Ɋ֘A����INF�܂ł͌����ς݂Ȃ̂ŁA
	 * INF�Ɋ֘A����LQ��S�Ď擾����B
	 * ROAD_LINK ...
	 * INF_*
	 *     |- LQ_*
	 *     |- LQ_*  INF�Ɋ֘A����LQ��S�Ď擾����
	 *        ...
	 * @param detach [in,optional] Row�I�u�W�F�N�g�̃C���X�^���X��؂藣���ꍇ��true�i�f�t�H���g�Ffalse�j
	 * @param unique [in,optional] �ǉ������̏ꍇ��true�i�f�t�H���g�Ffalse�j
	 * @param emType [in,optional] �����Ώۃ����N��i�f�t�H���g�Flinkqueue_type::kAll�j
	 */
	void SelectLQs( bool detach/* = false*/, bool unique/* = false*/, linkqueue_type::ECode emType/* = linkqueue_type::kAll*/ );

	/**
	 * @brief LQ�AINF�̌`���⊮����
	 *
	 * ���Ɍ����ς݂�LQ�AINF�ɑΉ�����`��𓹘H�����N����擾���܂��B
	 * �擾�����`��́A���ꂼ���CRowContainer�ɑ΂���GetShape()���邱�ƂŎ擾�ł��܂��B
	 *
	 * @warning ���ӓ_�Ƃ��āAROAD_LINK�̎q���Ƃ��ēo�^����Ă���LQ�ƁAINF�̎q���Ƃ��ēo�^����Ă���
	 * LQ�Ƃ͕ʕ��iIRow���x���ł͈ꏏ�����ACRowContainer���x���ł͕ʕ��j�ł���A�`���INF�̎q������
	 * �����t�^���܂���B
	 *
	 * @param emType [in] �����Ώۃ����N��i�f�t�H���g�Flinkqueue_type::kAll�j
	 */
	void SelectLQShape( linkqueue_type::ECode emType = linkqueue_type::kAll );
};

typedef boost::shared_ptr<CRoadLinkTable> CSPRoadLinkTable;

} // sindy
