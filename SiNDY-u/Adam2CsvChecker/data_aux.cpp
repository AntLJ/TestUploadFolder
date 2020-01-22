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


#include "data_aux.h"
#include "LogSystem.h"

#include "CheckLog.h"
#include "data_checker.h"

#include <iostream>
#include <fstream>
#include <set>
using namespace std;

int lessPointArr::dimension = 2;


bool push(const std::vector<std::string>& arr, std::vector<Row>& data, bool& )
{
	Row r;
	r.attr = arr;
	data.push_back(r);
	return true;
}
bool push(const std::vector<std::string>& arr, std::vector<Feature>& data, bool& fok)
{
	size_t n = arr.size();
	if (n < 3) { return false; }

	if (n == 3)
	{
		// 属性行が不正だった場合 次のレコードは1つ前のポイントにマージされるので注意!!
		if (data.empty()) { return false; }
		if (! fok) { return false; }

		auto& last = data.back();
		last.shape.push_back(Position(arr));
		return true;
	}
	else
	{
		Feature f;
		f.attr = arr;
		f.shape.push_back(Position(arr));
		data.push_back(f);
		fok = true;
		return true;
	}
}

template<typename _Typ>
bool fLoad(const TCHAR* fname, const CDataChecker* checker, std::vector<_Typ>& list)
{
	assert(fname && checker);

	std::ifstream ifs(fname);
	if (! ifs) { return false; }

	std::vector<std::string> header;

	long    prev = 0;   // 1つ前のOID
	int     err  = 0;
	int     war  = 0;
	int     n    = 0;
	bool    fok  = true;

	bool isDeleteRecord = false;
	string line;
	while (ifs && std::getline(ifs, line))
	{
		line = charset(line, CP_UTF8, CP_ACP);

		n++;
		if (n == 1)
		{
			if (line.empty()) { assert(false); --n; continue; }
			if (line[0] != '#') { LOG_E("[ERROR][%s]ヘッダが存在しません.", checker->name()); return false; }

			split(line, kDELIMITER, header);
			checker->setHeader(header);
			LOG_D("[INFO][%s]ヘッダ: %s", checker->name(), line.c_str());
			continue;
		}

		if (line.empty() || line[0] == '#') { continue; }

		std::vector<std::string> rec;
		split(line, kDELIMITER, rec);

		long oid_index = checker->isFeature() ? kFEATURE_OID : kTABLE_OID;
		long oid = (! checker->isFeature()) ? GetLong<kTABLE_OID>(rec) : ((rec.size() > kFEATURE_OID) ? GetLong<kFEATURE_OID>(rec) : -1);

		// 更新コードが削除(2)のレコードは読み込まない
		if (oid > 0) {
			isDeleteRecord = checker->hasDeleteCode(rec);
		}
		if (isDeleteRecord)
			continue;

		if (oid > 0)
		{
			if (oid <= prev)
			{
				war++;
				CheckLog::GetInstance()->printWarning(LOGCODE_ERROR_OID_ASC, LOGMSG_ERROR_OID_ASC, checker->name(), oid, header[oid_index], to_string(oid));
				LOG_W("[WARNING][%s.%d]OIDが昇順になっていません. [PREV=%d CURR=%d]", checker->name(), oid, prev, oid);
			}
			else { prev = oid; }
		}
		if (! checker->isSafe(rec, oid))
		{
			err++;
			LOG_E("[ERROR][%s.%d]フォーマットまたは属性値で違反が発生しました. [FILE_LINE=%d]", checker->name(), oid, n);
			if (rec.size() > 3) { fok = false; }
			continue;
		}
		if (! push(rec, list, fok))
		{
			err++;
			LOG_E("[ERROR][%s.%d]レコードの追加に失敗しました. [FILE_LINE=%d]", checker->name(), oid, n);
		}
	}

	LOG_I("[INFO][%s]警告数  : %d", checker->name(), war);
	LOG_I("[INFO][%s]エラー数: %d", checker->name(), err);

	return (err == 0);
}


bool fIsEqualAttr(const Feature& lhs, const Feature& rhs, const std::vector<long>& indices)
{
	bool is_err = false;
	if (lhs.attr.size() != rhs.attr.size())
	{
		assert(lhs.attr.size() > kFEATURE_OID && rhs.attr.size() > kFEATURE_OID);
		LOG_W("データ数が違うデータが比較されました.[LEFT_OID=%s, RIGHT_OID=%s]", lhs.attr[kFEATURE_OID].c_str(), rhs.attr[kFEATURE_OID].c_str());
		is_err = true;
	}

	for (const auto& index : indices)
	{
		if (lhs.attr.size() <= index) { LOG_W("%d列のデータが存在しません.[LEFT_OID =%s]", index + 1, lhs.attr[kFEATURE_OID].c_str()); continue; }
		if (rhs.attr.size() <= index) { LOG_W("%d列のデータが存在しません.[RIGHT_OID=%s]", index + 1, rhs.attr[kFEATURE_OID].c_str()); continue; }

		if (lhs.attr[index] != rhs.attr[index])
		{
			LOG_W("%d列の値が不一致です.[LEFT_OID=%s, RIGHT_OID=%s][LEFT_V=%s, RIGHT_V=%s]", index + 1, lhs.attr[kFEATURE_OID].c_str(), rhs.attr[kFEATURE_OID].c_str(), lhs.attr[index].c_str(), rhs.attr[index].c_str());
			is_err = true;
		}
	}

	return !is_err;
}

bool fIsEqualPosition3D(const Feature& lhs, const Feature& rhs, bool is_warning)
{
	bool is_err = false;

	if (lhs.shape.empty())
	{
		assert(lhs.attr.size() > kFEATURE_OID);
		if (is_warning) { LOG_W("形状の座標が不正です.[OID=%s][POS=%s]", lhs.attr[kFEATURE_OID].c_str(), lhs.shape[0].toString().c_str()); }
		is_err = true;
	}
	if (rhs.shape.empty())
	{
		assert(rhs.attr.size() > kFEATURE_OID);
		if (is_warning) { LOG_W("形状の座標が不正です.[OID=%s][POS=%s]", rhs.attr[kFEATURE_OID].c_str(), rhs.shape[0].toString().c_str()); }
		is_err = true;
	}

	if (lhs.shape != rhs.shape)
	{
		assert(lhs.attr.size() > kFEATURE_OID && rhs.attr.size() > kFEATURE_OID);
		if (is_warning) { LOG_W("形状の座標が不一致です.[LEFT_OID=%s, RIGHT_OID=%s][LEFT_POS=%s, RIGHT_POS=%s]", lhs.attr[kFEATURE_OID].c_str(), rhs.attr[kFEATURE_OID].c_str(), lhs.shape[0].toString().c_str(), rhs.shape[0].toString().c_str()); }
		is_err = true;
	}

	return !is_err;
}
bool fIsEqualPosition2D(const Feature& lhs, const Feature& rhs, bool is_warning)
{
	bool is_err = false;

	if (lhs.shape.empty())
	{
		assert(lhs.attr.size() > kFEATURE_OID);
		if (is_warning) { LOG_W("形状の座標が不正です.[OID=%s][POS=%s]", lhs.attr[kFEATURE_OID].c_str(), lhs.shape[0].toString().c_str()); }
		is_err = true;
	}
	if (rhs.shape.empty())
	{
		assert(rhs.attr.size() > kFEATURE_OID);
		if (is_warning) { LOG_W("形状の座標が不正です.[OID=%s][POS=%s]", rhs.attr[kFEATURE_OID].c_str(), rhs.shape[0].toString().c_str()); }
		is_err = true;
	}

	size_t nl = lhs.shape.size();
	size_t nr = rhs.shape.size();
	if (nl != nr)
	{
		assert(lhs.attr.size() > kFEATURE_OID && rhs.attr.size() > kFEATURE_OID);
		if (is_warning) { LOG_W("形状の座標が不一致です.[LEFT_OID=%s, RIGHT_OID=%s][LEFT_POS=%s, RIGHT_POS=%s]", lhs.attr[kFEATURE_OID].c_str(), rhs.attr[kFEATURE_OID].c_str(), lhs.shape[0].toString().c_str(), rhs.shape[0].toString().c_str()); }
		is_err = true;
	}
	else
	{
		for (size_t i=0 ; i<nl ; i++)
		{
			// Position::XY() で operator != () でもいいかも...
			if (! lhs.shape[i].equal2(rhs.shape[i]))
			{
				assert(lhs.attr.size() > kFEATURE_OID && rhs.attr.size() > kFEATURE_OID);
				if (is_warning) { LOG_W("形状の座標が不一致です.[LEFT_OID=%s, RIGHT_OID=%s][LEFT_POS=%s, RIGHT_POS=%s]", lhs.attr[kFEATURE_OID].c_str(), rhs.attr[kFEATURE_OID].c_str(), lhs.shape[0].toString().c_str(), rhs.shape[0].toString().c_str()); }
				is_err = true;
			}
		}
	}

	return !is_err;
}

double fCalcHubeny(const Position& lhs, const Position& rhs, double& distx, double& disty)
{
	// WGS84
	const double A   = 6378137.0;		// 長半径(m)
	const double B   = 6356752.314245;	// 短半径(m)
	const double EA2 = 0.00669437999019741354678198566736;	// (第一離心率 e) ^2

	auto fRAD2DEG = [](const double angle) { return (angle * 180.0 / 3.1415926535897932384626433832795); };
	auto fDEG2RAD = [](const double angle) { return (angle / 180.0 * 3.1415926535897932384626433832795); };

	double dx = fDEG2RAD(rhs.lon - lhs.lon);
	double dy = fDEG2RAD(rhs.lat - lhs.lat);
	double uy = fDEG2RAD((rhs.lat + lhs.lat) / 2.0);	// 緯度平均
	
	double W = sqrt(1 - EA2 * pow(sin(uy), 2));
	double M = A * (1 - EA2) / pow(W, 3);			// 子午線曲率半径
	double N = A / W;								// 卯酉線曲率半径

	distx = N * dx * cos(uy);
	disty = M * dy;

	return hypot(distx, disty);
}
double fCalcHubeny(const Position& lhs, const Position& rhs)
{
	double dx, dy;
	return fCalcHubeny(lhs, rhs, dx, dy);
}

template<typename _Typ>
bool fBuildDic(const std::vector<_Typ>& arr, long index[2], std::map<long, std::map<long, const _Typ*> >& dic)
{
	// 2つのフィールドから一意に決まる場合に利用できます
	// ※一意にならない場合 「後勝ち」
	LOG_D("(%d,%d)列で辞書化します.", index[0], index[1]);

	long less     = 0;
	long conflict = 0;
	for (const auto& itr : arr)
	{
		if (itr.attr.size() <= index[0] || itr.attr.size() <= index[1]) { less++; continue; }

		long key[2] = {
			atol(itr.attr[index[0]].c_str()),
			atol(itr.attr[index[1]].c_str())
		};
		dic[key[1]][key[2]] = &itr;
	}

	return (less == 0 && conflict == 0);
}

template<typename _Typ>
bool fBuildDic(const std::vector<_Typ>& arr, long index, std::map<long, const _Typ*>& dic, bool output_log)
{
	LOG_D("%d列目で辞書化します.", index + 1);

	long less     = 0;
	long conflict = 0;
	for (const auto& itr : arr)
	{
		if (itr.attr.size() <= index) { less++; continue; }

		long key = atol(itr.attr[index].c_str());
		if (! dic.insert(std::map<long, const _Typ*>::value_type(key, &itr)).second)
		{
			if (output_log) { LOG_E("値[%d]は重複しています.", key); }
			conflict++;
		}
	}

	return (less == 0 && conflict == 0);
}

template<typename _Typ>
bool fBuildDic(const std::vector<_Typ>& arr, long index, std::map<long, std::vector<const _Typ*> >& confs)
{
	LOG_D("%d列目で辞書化します.", index + 1);

	long less     = 0;
	for (const auto& itr : arr)
	{
		if (itr.attr.size() <= index) { less++; continue; }

		long key = atol(itr.attr[index].c_str());
		confs[key].push_back(&itr);
	}

	return (less == 0);
}



// 明示的実体化
template bool fBuildDic(const std::vector<Row    >& arr, long index[2], std::map<long, std::map<long, const Row*    > >& dic);
template bool fBuildDic(const std::vector<Feature>& arr, long index[2], std::map<long, std::map<long, const Feature*> >& dic);

template bool fBuildDic(const std::vector<Row    >& arr, long index, std::map<long, const Row    *>& dic, bool output_log);
template bool fBuildDic(const std::vector<Feature>& arr, long index, std::map<long, const Feature*>& dic, bool output_log);
template bool fBuildDic(const std::vector<Row    >& arr, long index, std::map<long, std::vector<const Row    *> >& confs);
template bool fBuildDic(const std::vector<Feature>& arr, long index, std::map<long, std::vector<const Feature*> >& confs);

template bool fLoad(const TCHAR* fname, const CDataChecker* checker, std::vector<Row    >& list);
template bool fLoad(const TCHAR* fname, const CDataChecker* checker, std::vector<Feature>& list);
