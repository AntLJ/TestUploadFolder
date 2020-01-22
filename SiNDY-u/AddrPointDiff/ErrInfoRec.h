#pragma once

#include "stdafx.h"
#include "AddrPointRec.h"

#include <ArcHelperEx/AheGeometryOp.h>

const CString STATUS = _T("0");       // ��
const CString ERR_LEVEL = _T("INFO"); // �G���[���x��

/**
 * @brief �G���[���R���e�i
 */
class ErrInfoRec
{
public:

	/**
	 * @brief �R���X�g���N�^
	 * @param[in] ipcPoint     �Z���|�C���g�iiPC�Z���j
	 * @param[in] midPoint     �Z���|�C���g�i���ԃt�@�C���Z���j
	 * @param[in] diffPattern  �����p�^�[��
	 * @param[in] midLayerName ���ԃt�@�C���Z�����C����
	 */
	ErrInfoRec(
			const AddrPointRec& ipcPoint,
			const AddrPointRec& midPoint,
			const CString& diffPattern,
			const CString& midLayerName = _T("")
			);

	/**
	 * @brief �G���[���O�o�͕�����擾
	 * @return �G���[���O�o�͕�����
	 */
	CString getMessageStr() const
	{
		std::vector<CString> valueList = boost::assign::list_of
			( STATUS )
			( m_layer )
			( m_oid )
			( m_layerOther )
			( m_oidOther )
			( ERR_LEVEL )
			( m_diffPattern )
			( m_geoBldId )
			( m_ipcAddrcode )
			( m_ipcAddress )
			( m_midAddrcode )
			( m_midAddress )
			( m_dist )
			;

		return uh::str_util::join(valueList, _T("\t"));
	};

	bool operator <(const ErrInfoRec& rec) const
	{
		// Geospace�ƌ`ID �� �����p�^�[�� �� �Z���|�C���g��� �� �I�u�W�F�N�gID�̏��ŏ����\�[�g

		// Geospace�ƌ`ID����̂��̂͌�ɂ���悤�ɂ���
		if( m_geoBldId.IsEmpty() != rec.m_geoBldId.IsEmpty() )
			return !m_geoBldId.IsEmpty();

		if( m_geoBldId != rec.m_geoBldId )
			return m_geoBldId < rec.m_geoBldId;

		if( _tstol(m_diffPattern) != _tstol(rec.m_diffPattern) )
			return _tstol(m_diffPattern) < _tstol(rec.m_diffPattern);

		if( m_type != rec.m_type )
			return m_type < rec.m_type;

		return  _tstol(m_oid) < _tstol(rec.m_oid);
	}

	bool operator ==(const ErrInfoRec& rec) const
	{
		return getMessageStr() == rec.getMessageStr();
	}

private:

	CString m_layer;        //!< ���C����
	CString m_oid;          //!< �I�u�W�F�N�gID
	CString m_layerOther;   //!< ���C����2
	CString m_oidOther;     //!< �I�u�W�F�N�gID 2
	CString m_diffPattern;  //!< �����p�^�[��
	CString m_geoBldId;     //!< Geospace�ƌ`ID
	CString m_ipcAddrcode;  //!< �Z��11���R�[�h�iiPC�Z���j
	CString m_ipcAddress;   //!< �Z��������iiPC�Z���j
	CString m_midAddrcode;  //!< �Z��11���R�[�h�i���ԃt�@�C���Z���j
	CString m_midAddress;   //!< �Z��������i���ԃt�@�C���Z���j
	CString m_dist;         //!< �Z���|�C���g2�_�ԋ���
	AddrPointType m_type;   //!< �Z���|�C���g���

	/**
	 * @brief iPC�Z�����C�����擾
	 * @return iPC�Z�����C����
	 */
	CString getLayerName(AddrPointType type) const
	{
		return (AddrPointType::GouPoint == type) ? 
					sindy::schema::gou_point::kTableName :
					( AddrPointType::CsAddrPoint == type ) ?
						sindy::schema::cs_addr_point::kTableName :
						_T("");
	};

	/**
	 * @brief �Z���|�C���g�iiPC�Z���A���ԃt�@�C���Z���j�ݒ�
	 * @param[in] ipcPoint         �Z���|�C���g�iiPC�Z���j
	 * @param[in] midPoint         �Z���|�C���g�i���ԃt�@�C���Z���j
	 * @param[in] midLayerName     ���ԃt�@�C���Z�����C����
	 * @param[in] isExportDist     �Z���|�C���g��2�_�ԋ������o�͂��邩�itrue:����Afalse:���Ȃ��j
	 * @param[in] isExportGeoBldId Geospace�ƌ`ID�����o�͂��邩�itrue:����Afalse:���Ȃ��j
	 */
	void setIpcAndMidPoints(
			const AddrPointRec& ipcPoint,
			const AddrPointRec& midPoint,
			const CString& midLayerName,
			bool isExportDist,
			bool isExportGeoBldId
			);

	/**
	 * @brief �Z���|�C���g�iiPC�Z���̂݁j�ݒ�
	 * @param[in] ipcPoint         �Z���|�C���g�iiPC�Z���j
	 * @param[in] isExportGeoBldId Geospace�ƌ`ID�����o�͂��邩�itrue:����Afalse:���Ȃ��j
	 */
	void setOnlyIpcPoint(
			const AddrPointRec& ipcPoint,
			bool isExportGeoBldId
			);

	/**
	 * @brief �Z���|�C���g�i���ԃt�@�C���Z���̂݁j�ݒ�
	 * @param[in] midPoint     �Z���|�C���g�i���ԃt�@�C���Z���j
	 * @param[in] midLayerName ���ԃt�@�C���Z�����C����
	 */
	void setOnlyMidPoint(
			const AddrPointRec& midPoint,
			const CString& midLayerName
			);
};