#pragma once

#include "stdafx.h"

#include <TDC/useful_headers/str_util.h>

namespace addr
{
/**
 * @brief 住所コード桁数
 */
enum AddrDigit
{
	addr   = 11, // 11桁
	chiban = 16, // 16桁
	gou    = 20, // 20桁
	Other  = 99  // それ以外
};

/**
 * @brief 文字付き住所コードかどうかを判定
 * @param[in] code 判定対象の住所レコード
 * @retval true 文字付き住所コードである
 * @retval false 文字付き住所コードではない
 */
bool judgeAddrChar(const CString& code);

/**
 * @brief 住所レコードコンテナ
 */
class AddrRec
{
public:

	/**
	 * @brief コンストラクタ
	 * @param[in] jusyocode  住所コード
	 * @param[in] geo        形状
	 */
	AddrRec(const CString& jusyocode, const IGeometryPtr& geo);

	/**
	 * @brief アクセサ（setter：地番名称＋号名称）
	 * @param[in] chibanName 地番名称
	 * @param[in] gouName    号名称
	 */
	void setAddrNames(const CString& chibanName, const CString& gouName)
	{
		setChibanName(chibanName);
		setGouName(gouName);
	}

	/**
	 * @brief アクセサ（setter：地番名称）
	 * @param[in] chibanName 地番名称
	 */
	void setChibanName(const CString& chibanName)
	{
		m_chibanName = chibanName;
	}

	/**
	 * @brief アクセサ（setter：号名称）
	 * @param[in] chibanName 号名称
	 */
	void setGouName(const CString& gouName)
	{
		m_gouName = gouName;
	}

	/**
	 * @brief アクセサ（setter：一致比較時、対象とするかどうか）
	 * @param[in] res 一致比較時の比較対象可否
	 */
	void setIsMatchTarget(bool res)
	{
		m_isMatchTarget = res;
	}

	/**
	 * @brief アクセサ（getter：住所コード）
	 * @retval 住所コード
	 */
	const CString getJusyocode() const { return m_citycode + m_addrcode + m_chibancode + m_goucode; }

	/**
	 * @brief アクセサ（getter：住所コード（11桁））
	 * @retval 住所コード（11桁）
	 */
	const CString getAdrcode11() const { return m_citycode + m_addrcode; }

	/**
	 * @brief アクセサ（getter：市区町村コード）
	 * @retval 市区町村コード
	 */
	const CString& getCitycode() const { return m_citycode; }

	/**
	 * @brief アクセサ（getter：大字・字コード）
	 * @retval 大字・字コード
	 */
	const CString& getAddrcode() const { return m_addrcode; }

	/**
	 * @brief アクセサ（getter：地番コード）
	 * @retval 地番コード
	 */
	const CString& getChibancode() const { return m_chibancode; }

	/**
	 * @brief アクセサ（getter：号コード）
	 * @retval 号コード
	 */
	const CString& getGoucode() const { return m_goucode; }

	/**
	 * @brief アクセサ（getter：地番名称）
	 * @retval 地番名称
	 */
	const CString& getChibanName() const { return m_chibanName; }

	/**
	 * @brief アクセサ（getter：号名称）
	 * @retval 号名称
	 */
	const CString& getGouName() const { return m_gouName; }

	/**
	 * @brief アクセサ（getter：形状）
	 * @retval 形状
	 */
	const IGeometryPtr& getGeometry() const { return m_geo; }

	/**
	 * @brief 文字付き地番かどうか
	 * @retval true 文字付き地番である
	 * @retval false 文字付き地番ではない
	 */
	bool isChibanChar() const { return m_isChibanChar; }

	/**
	 * @brief 文字付き号かどうか
	 * @retval true 文字付き号である
	 * @retval false 文字付き号ではない
	 */
	bool isGouChar() const { return m_isGouChar; }

	/**
	 * @brief 一致比較時、対象とするかどうか
	 * @retval true 対象とする
	 * @retval false 対象としない
	 */
	bool isMatchTarget() const { return m_isMatchTarget; }

	/**
	 * @brief 住所コードの階層レベルを取得
	 * @retval 住所階層レベル
	 */
	 AddrDigit getAddrDigit() const { return m_addrDigit; }

	bool operator ==(const AddrRec& rec) const
	{
		// 住所コード（11桁）と地番名称、号名称が一致する場合、同じとみなす
		return	getAdrcode11() ==rec.getAdrcode11() &&
				getChibanName() == rec.getChibanName() &&
				getGouName() == rec.getGouName();
	}
	bool operator <(const AddrRec& rec) const
	{
		// 住所コード（11桁）と地番名称、号名称が一致する場合、同じとみなす
		return	getJusyocode() < rec.getJusyocode();
	}

private:
	CString m_citycode;    //!< 市区町村コード
	CString m_addrcode;    //!< 大字・字コード
	CString m_chibancode;  //!< 地番コード
	CString m_goucode;     //!< 号コード
	CString m_chibanName;  //!< 地番名称
	CString m_gouName;     //!< 号名称
	IGeometryPtr m_geo;    //!< 形状
	AddrDigit m_addrDigit; //!< 住所階層レベル
	bool m_isChibanChar;   //!< 文字付き地番かどうか
	bool m_isGouChar;      //!< 文字付き号かどうか
	bool m_isMatchTarget;  //!< 一致比較時、対象とするかどうか
};

/**
 * @brief 住所レコードセット
 */
typedef std::vector<AddrRec> AddrRecs;

} 	// namespace addr