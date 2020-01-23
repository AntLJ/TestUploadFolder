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

// Bug9215対応：bool2つのpair型⇒bool3つ型に変更
struct POIUSABLEINFO
{
	bool	bQualityF;		// 位置品質向上F
	bool	bPPRateF;		// ピンポイント率向上F
	bool	bImportantF;	// 重要コンテンツF

	POIUSABLEINFO() : bQualityF(false), bPPRateF(false), bImportantF(false){}
};

/**
 * @class	CMasterListCache
 * @brief	リスト内マスター系キャッシュクラス
 */
class CMasterListCache
{
public:
	CMasterListCache(void);
	~CMasterListCache(void);

	/**
	 * @brief	キャッシュ作成
	 * @param	lpcszFile	[in]	マスタリストファイル
	 * @return	bool
	 */
	bool Create( const _TCHAR* lpcszFile );

	/**
	 * @brief	当該POIコンテンツ利用可能情報取得
	 * @param	lGrpCode	[in]	分類コード
	 * @param	lChainCode	[in]	店舗コード
	 * @return	利用可能情報
	 */
	const POIUSABLEINFO* GetPOIMasterInfo( long lGrpCode, long lChainCode )
	{
		std::multimap<long, std::map<long, POIUSABLEINFO> >::const_iterator it = m_mapCache.find( lGrpCode );
		if( it != m_mapCache.end() )
		{
			std::map<long, POIUSABLEINFO>::const_iterator itS = it->second.find(lChainCode);
			return itS != it->second.end()? &(itS->second) : NULL;
		}
		else
			return NULL;
	}

private:

	std::multimap<long, std::map<long, POIUSABLEINFO> >	m_mapCache;	// キー:分類 値:(キー:店舗 値:位置フラグ、PPフラグ)

};
