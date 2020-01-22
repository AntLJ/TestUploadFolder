#pragma once

#include "stdafx.h"
#include "AddrPointRec.h"

#include <ArcHelperEx/AheGeometryOp.h>

const CString STATUS = _T("0");       // 状況
const CString ERR_LEVEL = _T("INFO"); // エラーレベル

/**
 * @brief エラー情報コンテナ
 */
class ErrInfoRec
{
public:

	/**
	 * @brief コンストラクタ
	 * @param[in] ipcPoint     住所ポイント（iPC住所）
	 * @param[in] midPoint     住所ポイント（中間ファイル住所）
	 * @param[in] diffPattern  差分パターン
	 * @param[in] midLayerName 中間ファイル住所レイヤ名
	 */
	ErrInfoRec(
			const AddrPointRec& ipcPoint,
			const AddrPointRec& midPoint,
			const CString& diffPattern,
			const CString& midLayerName = _T("")
			);

	/**
	 * @brief エラーログ出力文字列取得
	 * @return エラーログ出力文字列
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
		// Geospace家形ID ＞ 差分パターン ＞ 住所ポイント種別 ＞ オブジェクトIDの順で昇順ソート

		// Geospace家形IDが空のものは後にくるようにする
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

	CString m_layer;        //!< レイヤ名
	CString m_oid;          //!< オブジェクトID
	CString m_layerOther;   //!< レイヤ名2
	CString m_oidOther;     //!< オブジェクトID 2
	CString m_diffPattern;  //!< 差分パターン
	CString m_geoBldId;     //!< Geospace家形ID
	CString m_ipcAddrcode;  //!< 住所11桁コード（iPC住所）
	CString m_ipcAddress;   //!< 住所文字列（iPC住所）
	CString m_midAddrcode;  //!< 住所11桁コード（中間ファイル住所）
	CString m_midAddress;   //!< 住所文字列（中間ファイル住所）
	CString m_dist;         //!< 住所ポイント2点間距離
	AddrPointType m_type;   //!< 住所ポイント種別

	/**
	 * @brief iPC住所レイヤ名取得
	 * @return iPC住所レイヤ名
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
	 * @brief 住所ポイント（iPC住所、中間ファイル住所）設定
	 * @param[in] ipcPoint         住所ポイント（iPC住所）
	 * @param[in] midPoint         住所ポイント（中間ファイル住所）
	 * @param[in] midLayerName     中間ファイル住所レイヤ名
	 * @param[in] isExportDist     住所ポイントの2点間距離を出力するか（true:する、false:しない）
	 * @param[in] isExportGeoBldId Geospace家形IDをを出力するか（true:する、false:しない）
	 */
	void setIpcAndMidPoints(
			const AddrPointRec& ipcPoint,
			const AddrPointRec& midPoint,
			const CString& midLayerName,
			bool isExportDist,
			bool isExportGeoBldId
			);

	/**
	 * @brief 住所ポイント（iPC住所のみ）設定
	 * @param[in] ipcPoint         住所ポイント（iPC住所）
	 * @param[in] isExportGeoBldId Geospace家形IDをを出力するか（true:する、false:しない）
	 */
	void setOnlyIpcPoint(
			const AddrPointRec& ipcPoint,
			bool isExportGeoBldId
			);

	/**
	 * @brief 住所ポイント（中間ファイル住所のみ）設定
	 * @param[in] midPoint     住所ポイント（中間ファイル住所）
	 * @param[in] midLayerName 中間ファイル住所レイヤ名
	 */
	void setOnlyMidPoint(
			const AddrPointRec& midPoint,
			const CString& midLayerName
			);
};