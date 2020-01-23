#pragma once

class CheckUtil
{
public:
	CheckUtil(addr::AddrRecHelper& addrRecHelper);
	virtual ~CheckUtil();

	/**
	 * @brief 11桁＋名称のリストを作る
	 * @param relAddrTbl [in] 住所紐付けテーブル
	 * @param relAddrMap [out] リスト
	 * @retval true 処理成功
	 * @retval false 処理失敗
	 */
	bool MakeAddrYomiList(
		const sindy::CTableContainer & relAddrTbl,
		std::map<CString, addr::AddrRecPairs>& relAddrMap);

	/**
	 * @brief 住所データの存在判定
	 * @param adminAddrRecs [in] 都市地図行政界データ
	 * @param csAddrRecs [in] CSポイントデータ
	 * @param gouPointTbl [in] 号ポイントデータ
	 * @param addrRec [in] 判定対象データ
	 * @retval true 存在する
	 * @retval false 存在しない
	 */
	bool CheckAddressDataExistance(
		const addr::AddrRecPairs& adminAddrRecs,
		const addr::AddrRecs& csAddrRecs,
		sindy::CTableContainer& gouPointTbl,
		const addr::AddrRec& addrRec
		);


	/**
	 * @brief 住所データの存在判定。存在する場合はAddrRecとして取得。
	 * @param adminAddrRecs [in] 都市地図行政界データ
	 * @param csAddrRecs [in] CSポイントデータ
	 * @param gouPointTbl [in] 号ポイントデータ
	 * @param addrRec [in] 判定対象データ
	 * @retval true 存在する
	 * @retval false 存在しない
	 */
	bool CheckAddressDataExistance(
		const addr::AddrRecPairs& adminAddrRecs,
		const addr::AddrRecs& csAddrRecs,
		sindy::CTableContainer& gouPointTbl,
		const addr::AddrRec& addrRec,
		addr::AddrRecs& existAddrRecs
	);


private:
	/**
	 * @brief 11桁から号ポイント取得
	 * @param relAddrTbl [in] 号ポイントデータ(抽出元)
	 * @param relAddrTbl [in] 行政界データ
	 * @param relAddrMap [out] 号ポイントデータ(抽出分)
	 */
	void getGpAddrRecs(
		sindy::CTableContainer& gouPointTbl,
		const addr::AddrRecPairs& caAddrRecPairs,
		addr::AddrRecs& gpAddrRecs
	);

	/**
	 * @brief 11桁住所コードとしての判定
	 * @param caAddrRecPairs [in] 行政界データ
	 * @param csAddrRecs [in] CSポイントデータ
	 * @param rec [in] 判定対象データ
	 * @retval true 行政界、CSポイントと一致する
	 * @retval false 行政界、CSポイントと一致しない
	 */
	bool decideAddrDigitStatus(
		const addr::AddrRecPairs& caAddrRecPairs,
		const addr::AddrRecs& csAddrRecs,
		const addr::AddrRec& rec
	);

	/**
	 * @brief 11桁住所コードとしての判定(判定trueのレコードを返す版)
	 * @param caAddrRecPairs [in] 行政界データ
	 * @param csAddrRecs [in] CSポイントデータ
	 * @param rec [in] 判定対象データ
	 * @param existAddrRecs [out] 判定がtrueになった行政界、CSレコード
	 * @retval true 行政界、CSポイントと一致する
	 * @retval false 行政界、CSポイントと一致しない
	 */
	bool decideAddrDigitStatus(
		const addr::AddrRecPairs& caAddrRecPairs,
		const addr::AddrRecs& csAddrRecs,
		const addr::AddrRec& rec,
		addr::AddrRecs& existAddrRecs
	);

	/**
	 * @brief 16桁住所コードとしての判定
	 * @param caAddrRecPairs [in] 行政界データ
	 * @param gpAddrRecs [in] 号ポイントデータ
	 * @param csAddrRecs [in] CSポイントデータ
	 * @param rec [in] 判定対象データ
	 * @retval true 行政界、CS、号ポイントと一致する
	 * @retval false 行政界、CS、号ポイントと一致しない
	 */
	bool decideChibanDigitStatus(
		const addr::AddrRecPairs& caAddrRecPairs,
		const addr::AddrRecs& gpAddrRecs,
		const addr::AddrRecs& csAddrRecs,
		const addr::AddrRec& rec
	);

	/**
	 * @brief 16桁住所コードとしての判定(判定trueのレコードを返す版)
	 * @param caAddrRecPairs [in] 行政界データ
	 * @param gpAddrRecs [in] 号ポイントデータ
	 * @param csAddrRecs [in] CSポイントデータ
	 * @param rec [in] 判定対象データ
	 * @param existAddrRecs [out] 判定がtrueになった行政界、号、CSレコード
	 * @retval true 行政界、CS、号ポイントと一致する
	 * @retval false 行政界、CS、号ポイントと一致しない
	 */
	bool decideChibanDigitStatus(
		const addr::AddrRecPairs& caAddrRecPairs,
		const addr::AddrRecs& gpAddrRecs,
		const addr::AddrRecs& csAddrRecs,
		const addr::AddrRec& rec,
		addr::AddrRecs& existAddrRecs
	);

	/**
	 * @brief 20桁住所コードとしての判定
	 * @param gpAddrRecs [in] 号ポイントデータ
	 * @param csAddrRecs [in] CSポイントデータ
	 * @param rec [in] 判定対象データ
	 * @retval true CS、号ポイントと一致する
	 * @retval false CS、号ポイントと一致しない
	 */
	bool decideGouDigitStatus(
		const addr::AddrRecs& gpAddrRecs,
		const addr::AddrRecs& csAddrRecs,
		const addr::AddrRec& rec
	);

	/**
	 * @brief 20桁住所コードとしての判定(判定trueのレコードを返す版)
	 * @param gpAddrRecs [in] 号ポイントデータ
	 * @param csAddrRecs [in] CSポイントデータ
	 * @param rec [in] 判定対象データ
	 * @param existAddrRecs [out] 判定がtrueになった号、CSレコード
	 * @retval true CS、号ポイントと一致する
	 * @retval false CS、号ポイントと一致しない
	 */
	bool decideGouDigitStatus(
		const addr::AddrRecs& gpAddrRecs,
		const addr::AddrRecs& csAddrRecs,
		const addr::AddrRec& rec,
		addr::AddrRecs& existAddrRecs
	);

	/**
	 * @brief CSデータかどうか判定
	 * @param addrRecs [in] CSポイントデータ
	 * @param rec [in] 判定対象データ
	 * @retval true CSポイントである
	 * @retval false CSポイントではない
	 */
	bool judgeSameAddrRecByCs(
		const addr::AddrRecs& addrRecs,
		const addr::AddrRec& rec
	);
	
	/**
	 * @brief CSデータかどうか判定(判定trueのレコードを返す版)
	 * @param addrRecs [in] CSポイントデータ
	 * @param rec [in] 判定対象データ
	 * @param existAddrRecs [out] 判定がtrueになったCSレコード
	 * @retval true CSポイントである
	 * @retval false CSポイントではない
	 */
	bool judgeSameAddrRecByCs(
		const addr::AddrRecs& addrRecs,
		const addr::AddrRec& rec,
		addr::AddrRecs& existAddrRecs
	);

private:
	addr::AddrRecHelper m_addrRecHelper; //!< AddrRecHelperクラス
};

