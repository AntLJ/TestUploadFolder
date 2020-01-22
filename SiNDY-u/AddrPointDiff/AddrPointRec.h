#pragma once

#include "stdafx.h"
#include "Utility.h"
#include "AddressSort.h"

#include <ArcHelperEx/AheGlobalsMisc.h>

/**
 * @brief 住所ポイント種別
 */
enum AddrPointType
{
	GouPoint,     // 号ポイント
	CsAddrPoint,  // CSポイント
	GsAddrPoint,  // 中間ファイル住所ポイント
	Other         // その他
};

/**
 * @brief 住所ポイントコンテナ
 */
class AddrPointRec
{
public:

	/**
	 * @brief コンストラクタ
	 */
	AddrPointRec() :
		oid(-1), addrPointType(AddrPointType::Other), geo(nullptr), 
		isInBuilding(false), addrExistAttr(AddrExistAttr::NoExist),
		isExistOtherPointInSameBld(false) {}

	/**
	 * @brief コンストラクタ
	 * @param[in] pointType       住所ポイント種別
	 * @param[in] objectId        オブジェクトID
	 * @param[in] addrCode        住所11桁コード
	 * @param[in] addressStr      住所文字列
	 * @param[in] geometry        形状
	 * @param[in] geoBuildingId   Geospace家形ID
	 */
	AddrPointRec::AddrPointRec(
		AddrPointType pointType,
		long objectId,
		const CString& addrcode11,
		const CString& addressStr,
		const IGeometryPtr& geometry,
		const CString& geoBuildingId );

	/**
	 * @brief コンストラクタ
	 * @param[in] other 住所ポイントコンテナ
	 */
	explicit AddrPointRec(const AddrPointRec&& other) :
		oid(other.oid), addrPointType(other.addrPointType), addrcode(other.addrcode), 
		address(other.address), addressComp(other.addressComp), geo(AheGetClone(other.geo)), 
		isInBuilding(other.isInBuilding), geoBldId(other.geoBldId), 
		addrExistAttr(other.addrExistAttr),
		isExistOtherPointInSameBld(other.isExistOtherPointInSameBld) { }

	/**
	 * @brief デストラクタ
	 */
	virtual ~AddrPointRec() {}

	/**
	 * @brief 対象の住所ポイントの住所文字列に値が近い住所文字列を持つ住所ポイントを返す
	 * @param[in] dist      距離
	 * @param[in] isTarget  検索対象かどうかどうか
	 * @return 住所文字列ソートコンテナ
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

	long oid;                           //!< オブジェクトID
	AddrPointType addrPointType;        //!< 生成元の住所ポイント種類
	CString addrcode;                   //!< 住所11桁コード
	CString address;                    //!< 住所文字列
	CString addressComp;                //!< 住所文字列（データ比較用）
	IGeometryPtr geo;                   //!< 住所ポイント位置情報
	AddrExistAttr addrExistAttr;    //!< 同一住所存在フラグ
	bool isInBuilding;                  //!< 建物内存在フラグ
	CString geoBldId;                   //!< Geospace家形ID
	bool isExistOtherPointInSameBld;    //!< 同一建物内存在フラグ
};

/**
 * @brief 住所ポイントセット
 */
typedef std::vector<AddrPointRec> AddrPointRecs;

/**
 * @brief 住所ポイントセット（キー別）
 */
typedef std::map<CString, AddrPointRecs> AddrPointRecsByCode;

/**
 * @brief 住所ポイントセット管理コンテナ
 */
class AddrPointRecSet
{
public:

	/**
	 * @brief コンストラクタ
	 */
	AddrPointRecSet() {}

	/**
	 * @brief コンストラクタ
	 * @param[in] addrcode  住所11桁コード
	 */
	AddrPointRecSet(const CString& addrcode) : m_addrcode(addrcode) {}
	
	/**
	 * @brief デストラクタ
	 */
	virtual ~AddrPointRecSet() {}

	/**
	 * @brief 指定した住所ポイントを保持する
	 * @param[in] addrPointRec 住所ポイント種別
	 */
	void setAddrPointRecs(const AddrPointRec& addrPointRec);

	/**
	 * @brief 保持された住所ポイント群を取得する
	 * @param[in] addrPointRecs 住所ポイントセット
	 */
	void getAddrPointRecs(AddrPointRecs& addrPointRecs);

	/**
	 * @brief 保持された住所ポイント群を取得する
	 * @param[in] addrPointRecs 住所ポイントセット（キー別）
	 */
	void getAddrPointRecs(AddrPointRecsByCode& addrPointRecs);

private:

	CString m_addrcode;                  //!< 住所11桁コード
	AddrPointRecsByCode m_addrPointRecs; //!< 住所ポイントセット（Geospace家形ID別）
};

/**
 * @brief 住所ポイント（住所文字列ソートコンテナ付き）
 */
typedef std::pair<AddrPointRec*, AddressSort> AddrPointWithSortKey;

/**
 * @brief 対象の住所ポイントの住所文字列に値が近い住所文字列を持つ住所ポイントを返す
 * @param[in] recs           住所ポイント（住所文字列コンテナ付き）リスト
 * @param[in] ipcPointWithSk iPC住所ポイント（住所文字列コンテナ付き）
 * @param[in] isConsiderDist 選定時、距離を考慮するか（true:考慮する、false:考慮しない）
 * @return 住所文字列の値が一番近い住所ポイント（住所文字列コンテナ付き）
 */
AddrPointWithSortKey selectNearestAddress(
		std::vector<AddrPointWithSortKey>& recs,
		AddrPointWithSortKey& ipcPointWithSk,
		bool isConsiderDist);