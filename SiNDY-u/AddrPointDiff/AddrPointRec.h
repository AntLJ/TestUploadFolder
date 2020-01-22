#pragma once

#include "stdafx.h"
#include "Utility.h"
#include "AddressSort.h"

#include <ArcHelperEx/AheGlobalsMisc.h>

/**
 * @brief �Z���|�C���g���
 */
enum AddrPointType
{
	GouPoint,     // ���|�C���g
	CsAddrPoint,  // CS�|�C���g
	GsAddrPoint,  // ���ԃt�@�C���Z���|�C���g
	Other         // ���̑�
};

/**
 * @brief �Z���|�C���g�R���e�i
 */
class AddrPointRec
{
public:

	/**
	 * @brief �R���X�g���N�^
	 */
	AddrPointRec() :
		oid(-1), addrPointType(AddrPointType::Other), geo(nullptr), 
		isInBuilding(false), addrExistAttr(AddrExistAttr::NoExist),
		isExistOtherPointInSameBld(false) {}

	/**
	 * @brief �R���X�g���N�^
	 * @param[in] pointType       �Z���|�C���g���
	 * @param[in] objectId        �I�u�W�F�N�gID
	 * @param[in] addrCode        �Z��11���R�[�h
	 * @param[in] addressStr      �Z��������
	 * @param[in] geometry        �`��
	 * @param[in] geoBuildingId   Geospace�ƌ`ID
	 */
	AddrPointRec::AddrPointRec(
		AddrPointType pointType,
		long objectId,
		const CString& addrcode11,
		const CString& addressStr,
		const IGeometryPtr& geometry,
		const CString& geoBuildingId );

	/**
	 * @brief �R���X�g���N�^
	 * @param[in] other �Z���|�C���g�R���e�i
	 */
	explicit AddrPointRec(const AddrPointRec&& other) :
		oid(other.oid), addrPointType(other.addrPointType), addrcode(other.addrcode), 
		address(other.address), addressComp(other.addressComp), geo(AheGetClone(other.geo)), 
		isInBuilding(other.isInBuilding), geoBldId(other.geoBldId), 
		addrExistAttr(other.addrExistAttr),
		isExistOtherPointInSameBld(other.isExistOtherPointInSameBld) { }

	/**
	 * @brief �f�X�g���N�^
	 */
	virtual ~AddrPointRec() {}

	/**
	 * @brief �Ώۂ̏Z���|�C���g�̏Z��������ɒl���߂��Z������������Z���|�C���g��Ԃ�
	 * @param[in] dist      ����
	 * @param[in] isTarget  �����Ώۂ��ǂ����ǂ���
	 * @return �Z��������\�[�g�R���e�i
	 */
	AddressSort getAddressSort(double dist, bool isTarget)
	{
		return AddressSort( addressComp, dist, isTarget );
	}

	bool operator ==(const AddrPointRec& b) const
	{
		return addrcode == b.addrcode && addressComp == b.addressComp;
	}

	bool operator <(const AddrPointRec& rec) const
	{
		if( addrPointType != rec.addrPointType )
			return addrPointType < rec.addrPointType;

		if( addrcode != rec.addrcode )
			return addrcode < rec.addrcode;

		return oid < rec.oid;
	}

	long oid;                           //!< �I�u�W�F�N�gID
	AddrPointType addrPointType;        //!< �������̏Z���|�C���g���
	CString addrcode;                   //!< �Z��11���R�[�h
	CString address;                    //!< �Z��������
	CString addressComp;                //!< �Z��������i�f�[�^��r�p�j
	IGeometryPtr geo;                   //!< �Z���|�C���g�ʒu���
	AddrExistAttr addrExistAttr;    //!< ����Z�����݃t���O
	bool isInBuilding;                  //!< ���������݃t���O
	CString geoBldId;                   //!< Geospace�ƌ`ID
	bool isExistOtherPointInSameBld;    //!< ���ꌚ�������݃t���O
};

/**
 * @brief �Z���|�C���g�Z�b�g
 */
typedef std::vector<AddrPointRec> AddrPointRecs;

/**
 * @brief �Z���|�C���g�Z�b�g�i�L�[�ʁj
 */
typedef std::map<CString, AddrPointRecs> AddrPointRecsByCode;

/**
 * @brief �Z���|�C���g�Z�b�g�Ǘ��R���e�i
 */
class AddrPointRecSet
{
public:

	/**
	 * @brief �R���X�g���N�^
	 */
	AddrPointRecSet() {}

	/**
	 * @brief �R���X�g���N�^
	 * @param[in] addrcode  �Z��11���R�[�h
	 */
	AddrPointRecSet(const CString& addrcode) : m_addrcode(addrcode) {}
	
	/**
	 * @brief �f�X�g���N�^
	 */
	virtual ~AddrPointRecSet() {}

	/**
	 * @brief �w�肵���Z���|�C���g��ێ�����
	 * @param[in] addrPointRec �Z���|�C���g���
	 */
	void setAddrPointRecs(const AddrPointRec& addrPointRec);

	/**
	 * @brief �ێ����ꂽ�Z���|�C���g�Q���擾����
	 * @param[in] addrPointRecs �Z���|�C���g�Z�b�g
	 */
	void getAddrPointRecs(AddrPointRecs& addrPointRecs);

	/**
	 * @brief �ێ����ꂽ�Z���|�C���g�Q���擾����
	 * @param[in] addrPointRecs �Z���|�C���g�Z�b�g�i�L�[�ʁj
	 */
	void getAddrPointRecs(AddrPointRecsByCode& addrPointRecs);

private:

	CString m_addrcode;                  //!< �Z��11���R�[�h
	AddrPointRecsByCode m_addrPointRecs; //!< �Z���|�C���g�Z�b�g�iGeospace�ƌ`ID�ʁj
};

/**
 * @brief �Z���|�C���g�i�Z��������\�[�g�R���e�i�t���j
 */
typedef std::pair<AddrPointRec*, AddressSort> AddrPointWithSortKey;

/**
 * @brief �Ώۂ̏Z���|�C���g�̏Z��������ɒl���߂��Z������������Z���|�C���g��Ԃ�
 * @param[in] recs           �Z���|�C���g�i�Z��������R���e�i�t���j���X�g
 * @param[in] ipcPointWithSk iPC�Z���|�C���g�i�Z��������R���e�i�t���j
 * @param[in] isConsiderDist �I�莞�A�������l�����邩�itrue:�l������Afalse:�l�����Ȃ��j
 * @return �Z��������̒l����ԋ߂��Z���|�C���g�i�Z��������R���e�i�t���j
 */
AddrPointWithSortKey selectNearestAddress(
		std::vector<AddrPointWithSortKey>& recs,
		AddrPointWithSortKey& ipcPointWithSk,
		bool isConsiderDist);