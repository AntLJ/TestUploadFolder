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

//#include <typeinfo.h>
#include <TDC/useful_headers/text_util.h>

#include "LogUtil.h"

//#include "FieldChecker.h"
//#include "FieldTypeConverter.h"

// FieldType の名前が被るので、名前空間を分ける
namespace adam_v2
{
template<typename T>
class FieldChecker;

template<typename T>
class FieldTypeConverter;

/**
 * フィールド共通要素
 */
class CommonFieldType
{
public:
	/**
	 * コンストラクタ
	 */
	CommonFieldType() : 
		_index(0),
		_name(_T("")),
		_text(""),
		_isNull(false),
		_hasNull(false),
		_lengthInt(0),
		_lengthDecimal(0)
	{}
	CommonFieldType(long index, const uh::tstring& name, bool hasNull, long lengthInt, long lengthDecimal)
	{
		this->_index = index;
		this->_name = name;
		this->_text.clear();
		this->_isNull = false;
		this->_hasNull = hasNull;
		this->_lengthInt = lengthInt;
		this->_lengthDecimal = lengthDecimal;
	}

	/**
	 * デストラクタ
	 */
	virtual ~CommonFieldType(){}

	/**
	 * 整合性チェック
	 *
	 * @retval	true	OK
	 * @retval	false	NG
	 */
	virtual bool Check(bool checkIntegrity = false) = 0;

	virtual void Clear()
	{
		_text.clear();
	}

	long GetIndex() const
	{
		return _index;
	}

	uh::tstring GetName() const
	{
		return _name;
	}

protected:
	long			_index;				///< 列番号
	uh::tstring		_name;				///< 列名称(ヘッダ名称)
	std::string		_text;				///< 読み込んだままのデータ

	bool			_isNull;			///< NULLであるかどうか

	//チェック用
	bool			_hasNull;			///< NULL許容かどうか(true:NULL可)
	//bool			_canOmit;			///< 省略可能かどうか(true:省略可)	※省略可は許容しない
	long			_lengthInt;			///< 整数最大桁数 or 最大文字数(0～)
	long			_lengthDecimal;		///< 小数最大桁数(%.Nf)(0～)
};

/**
 * フィールド要素 テンプレート
 */
template<typename T>
class FieldType : public CommonFieldType// , public FieldChecker<T>
{
public:
	/**
	 * デフォルトコンストラクタ
	 */
	FieldType(void) :
		_initial(0),
		_value(0),
		_minValue(0),
		_maxValue(0),
		_validValue(0),
		_isCheckMinMax(false),
		_isCheckValidValue(false)
	{}
	/**
	 * コンストラクタ
	 *
	 * チェックなし
	 *
	 * @param[in]	index			列番号
	 * @param[in]	name			列名称(ヘッダ名称)
	 * @param[in]	initial			初期値
	 * @param[in]	hasNull			NULL許容
	 * @param[in]	lengthInt		整数最大桁数 or 最大文字数
	 * @param[in]	lengthDecimal	小数最大桁数
	 */
	FieldType(long index, const uh::tstring& name, const T& initial, bool hasNull, long lengthInt, long lengthDecimal) : 
		CommonFieldType(index, name, hasNull, lengthInt, lengthDecimal)
	{
		this->_initial = initial;
		this->_value = initial;
		//this->str = str;
		this->_minValue = initial;
		this->_maxValue = initial;
		this->_validValue.clear();
		this->_isCheckMinMax = false;
		this->_isCheckValidValue = false;
	}
	/**
	 * コンストラクタ
	 *
	 * 最大/最小チェック
	 *
	 * @param[in]	index	列番号
	 * @param[in]	name	列名称(ヘッダ名称)
	 * @param[in]	initial		初期値
	 * @param[in]	min			最小値
	 * @param[in]	max			最大値
	 */
	FieldType(long index, const uh::tstring& name, const T& initial, bool hasNull, long lengthInt, long lengthDecimal, const T& min,  const T& max) : 
		CommonFieldType(index, name, hasNull, lengthInt, lengthDecimal)
	{
		this->_initial = initial;
		this->_value = initial;
		//this->str = str;
		this->_minValue = min;
		this->_maxValue = max;
		this->_validValue.clear();
		this->_isCheckMinMax = true;
		this->_isCheckValidValue = false;
	}
	/**
	 * コンストラクタ
	 *
	 * 範囲チェック
	 *
	 * @param[in]	index	列番号
	 * @param[in]	name	列名称(ヘッダ名称)
	 * @param[in]	initial		初期値
	 * @param[in]	validElemts	有効値
	 */
	FieldType(long index, const uh::tstring& name, const T& initial, bool hasNull, long lengthInt, long lengthDecimal, const std::vector<T>& validElemts) : 
		CommonFieldType(index, name, hasNull, lengthInt, lengthDecimal)
	{
		this->_initial = initial;
		this->_value = initial;
		//this->str = str;
		this->_minValue = initial;
		this->_maxValue = initial;
		this->_validValue = validElemts;
		this->_isCheckMinMax = false;
		this->_isCheckValidValue = true;
	}
	
	/**
	 * デストラクタ
	 */
	~FieldType(void){}

	/**
	 * テキスト(読み込みデータ)の設定
	 * 
	 * @param[in]	value	読み込みデータ
	 * 
	 * @retval	true	成功
	 * @retval	false	失敗
	 */
	bool SetText(const std::string& text)
	{
		if(text == "")
		{
			return SetIsNull(true);
		}

		//チェックNGのものは弾く

		//std::string→T変換
		T value = FieldTypeConverter<T>::GetValue(text);
		
		//ジオメトリはここでは確定しないのでチェックできない
		//ジオメトリ確定後に SurveyCSVReader::CheckGeometryIntegrity() にて実施
		if(Check(value, text, false))
		{
			_text = text;
			_value = value;
			
			return true;
		}

		return false;
	}

	/**
	 * 値の設定
	 *
	 * チェックOKなら格納する
	 *
	 * @param[in]	value	指定型の値
	 * 
	 * @retval	true	成功
	 * @retval	false	失敗
	 */
	bool SetValue(const T& value)
	{
		//std::string←T変換
		std::string text = FieldTypeConverter<T>::GetStr(value);

		//ジオメトリはここでは確定しないのでチェックできない
		//ジオメトリ確定後に SurveyCSVReader::CheckGeometryIntegrity() にて実施
		if(Check(value, text, false))
		{
			_text = text;
			_value = value;
			
			return true;
		}
		
		return false;
	}

	/**
	 * 値の取得
	 * 
	 * @pre		SetValue() == true OR SetText() == true
	 * 
	 * @return	値
	 */
	T GetValue() const
	{
		return _value;
	}

	/**
	 * テキストの取得
	 * 
	 * @pre		SetValue() == true OR SetText() == true
	 * 
	 * @return	std::string値
	 */
	std::string GetText() const
	{
		return _text;
	}

	/**
	 * NULL値の設定
	 *
	 * @param[in]	flg	true:NULL/false:NULL以外
	 *
	 * @retval	true	NULL設定 反映成功
	 * @retval	false	NULL設定 反映失敗(NULL許容していない)
	 */
	bool SetIsNull(bool flg)
	{
		if(!flg)
		{
			_isNull = false;
			return true;
		}

		if(_hasNull && flg)
		{
			_isNull = true;

			////設定済み値のクリア
			//Clear();

			return true;
		}
		else
		{
			LogUtil::printError(4, "NULLエラー", uh::toStr(_name), "非NULLフィールドにNULLが指定されています");
		}
		
		return false;
	}

	/**
	 * NULL値が設定されているかどうか
	 *
	 * @retval	true	NULLである
	 * @retval	false	NULLでない
	 */
	bool GetIsNull() const
	{
		return _isNull;
	}

	/**
	 * 全チェック実行
	 *
	 * @retval	true	チェックOK
	 * @retval	false	チェックNG
	 */
	bool Check(bool checkIntegrity = false)
	{
		return Check(_value, _text, checkIntegrity);
	}

	/**
	 * 全チェック実行
	 *
	 * @retval	true	チェックOK
	 * @retval	false	チェックNG
	 */
	bool Check(const T& value, const std::string& text, bool checkIntegrity)
	{
		bool ret = true;

		if(_isCheckMinMax)
		{
			ret &= FieldChecker<T>::CheckMin(value, _minValue, _name);
			ret &= FieldChecker<T>::CheckMax(value, _maxValue, _name);
		}
		if(_isCheckValidValue)
		{
			ret &= FieldChecker<T>::CheckValidValue(value, _validValue, _name);
		}
		ret &= FieldChecker<T>::CheckLength(value, text, _lengthInt, _lengthDecimal, _name);
		if(checkIntegrity)
		{
			ret &= FieldChecker<T>::CheckIntegrity(value, _name);
		}

		return ret;
	}

	/**
	 * 値のクリア
	 */
	void Clear()
	{
		CommonFieldType::Clear();
	}
private:
	T				_initial;				///< 初期値
	T				_value;					///< データの実体

	//チェック用[任意]
	T				_minValue;				///< [任意]最小値
	T				_maxValue;				///< [任意]最大値
	std::vector<T>	_validValue;			///< [任意]有効値

	bool			_isCheckMinMax;			///< 最小/最大チェック有効か
	bool			_isCheckValidValue;		///< 有効値チェックが有効か
};

template<>
void FieldType<int>::Clear()
{
	CommonFieldType::Clear();
	_value = 0;
}

template<>
void FieldType<long>::Clear()
{
	CommonFieldType::Clear();
	_value = 0;
}

template<>
void FieldType<long long>::Clear()
{
	CommonFieldType::Clear();
	_value = 0;
}

template<>
void FieldType<double>::Clear()
{
	CommonFieldType::Clear();
	_value = 0.0;
}

template<>
void FieldType<uh::tstring>::Clear()
{
	CommonFieldType::Clear();
	_value = _T("");
}

// ==================================================
// Location
// ==================================================
/**
 * 位置情報クラス
 */
class Location
{

public:
	FieldType<double> longitude	;	///< 緯度
	FieldType<double> latitude	;	///< 経度
	FieldType<double> height	;	///< 高さ
public:
	/**
	 * デフォルトコンストラクタ
	 */
	Location() :
		//					型									列番号	フィールド名		初期値		NULL許容		整数最大桁数		小数最大桁数		(option)(最小値		最大値) or 有効値
		longitude		(	FieldType<double>				(	INDEX_LON,		_T("LON"),			0.0,		HAS_NULL,		LENGTH_INT_LONLAT,	LENGTH_DEC_LONLAT		))
		,latitude		(	FieldType<double>				(	INDEX_LAT,		_T("LAT"),			0.0,		HAS_NULL,		LENGTH_INT_LONLAT,	LENGTH_DEC_LONLAT		))
		,height			(	FieldType<double>				(	INDEX_HEIGHT,	_T("HEIGHT"),		0.0,		HAS_NULL,		LENGTH_INT_HEIGHT,	LENGTH_DEC_HEIGHT		))
	{}
	///**
	// * コンストラクタ
	// *
	// * @param[in]	lon	経度
	// * @param[in]	lat	緯度
	// * @param[in]	he	高さ
	// *
	// * @note	チェック結果の戻り値なしのため SetLocation(...)を使ってください
	// */
	//Location(const FieldType<double>& lon, const FieldType<double>& lat, const FieldType<double>& he)
	//{
	//	_createLocation();

	//	longitude = lon;
	//	latitude = lat;
	//	height = he;
	//}
	/**
	 * コンストラクタ
	 *
	 * @param[in]	lon	経度 (ex: "135.00000000")
	 * @param[in]	lat	緯度 (ex: "35.00000000")
	 * @param[in]	he	高さ (ex: "5.000")
	 *
	 * @note	チェック結果の戻り値なしのため SetLocation(...)を使ってください
	 */
	Location(const std::string& lon, const std::string& lat, const std::string& he)
	{
		SetLocation(lon, lat, he);
	}
	/**
	 * コンストラクタ
	 *
	 * @param[in]	text	パース前文字列(ex: "135.00000000,35.00000000,5.000")
	 * 
	 * @note	チェック結果の戻り値なしのため SetLocation(...)を使ってください
	 */
	Location(const std::string& text)
	{
		SetLocation(text);
	}
	/**
	 * コンストラクタ
	 *
	 * @param[in]	loc	Location
	 */
	Location(const Location& loc)
	{
		_createLocation();

		longitude = loc.longitude;
		latitude = loc.latitude;
		height = loc.height;
	}
	/**
	 * デストラクタ
	 */
	~Location(){}

	//find用
	bool operator==(const Location &loc) const
	{
		return longitude.GetValue() == loc.longitude.GetValue()
			&& latitude.GetValue() == loc.latitude.GetValue()
			&& height.GetValue() == loc.height.GetValue();
	}

	bool operator!=(const Location &loc) const
	{
		return longitude.GetValue() != loc.longitude.GetValue()
			&& latitude.GetValue() != loc.latitude.GetValue()
			&& height.GetValue() != loc.height.GetValue();
	}
	/**
	 * 値のクリア
	 */
	void Clear()
	{
		longitude.Clear();
		latitude.Clear();
		height.Clear();
	}

	//bool SetLocation(const FieldType<double>& lon, const FieldType<double>& lat, const FieldType<double>& he)
	//{
	//	_createLocation();

	//	longitude = lon;
	//	latitude = lat;
	//	height = he;
	//}

	bool SetLocation(const Location& loc)
	{
		_createLocation();

		bool ret = true;

		longitude = loc.longitude;
		latitude = loc.latitude;
		height = loc.height;

		ret &= longitude.Check();
		ret &= latitude.Check();
		ret &= height.Check();

		return ret;
	}

	bool SetLocation(const std::string& lon, const std::string& lat, const std::string& he)
	{
		_createLocation();

		bool ret = true;
		ret &= longitude.SetText(lon);
		ret &= latitude.SetText(lat);
		ret &= height.SetText(he);

		return ret;
	}

	bool SetLocation(const std::string& text)
	{
		_createLocation();

		std::list<uh::tstring> lists = uh::text_util::ToTStrList(uh::toTStr(text));
		bool ret = true;

		if(lists.size() == 3)
		{
			for(std::list<uh::tstring>::iterator itr = lists.begin(); itr != lists.end(); ++itr)
			{
				ret &= longitude.SetText(uh::toStr(*itr));
				ret &= latitude.SetText(uh::toStr(*(++itr)));
				ret &= height.SetText(uh::toStr(*(++itr)));
			}
		}

		return ret;
	}

	Location GetLocation()
	{
		return *this;
	}

private:
	void _createLocation()
	{
		longitude = FieldType<double>	(	INDEX_LON,		_T("LON"),			0.0,		HAS_NULL,		LENGTH_INT_LONLAT,	LENGTH_DEC_LONLAT		);
		latitude = FieldType<double>	(	INDEX_LAT,		_T("LAT"),			0.0,		HAS_NULL,		LENGTH_INT_LONLAT,	LENGTH_DEC_LONLAT		);
		height = FieldType<double>		(	INDEX_HEIGHT,	_T("HEIGHT"),		0.0,		HAS_NULL,		LENGTH_INT_HEIGHT,	LENGTH_DEC_HEIGHT		);
	}

private:
	// チェック用定数値
	static const long LENGTH_INT_LONLAT		= 3L;	///< 緯度経度 整数最大桁数
	static const long LENGTH_DEC_LONLAT		= 8L;	///< 緯度経度 実数最大桁数
	static const long LENGTH_INT_HEIGHT		= 8L;	///< 高さ 整数最大桁数
	static const long LENGTH_DEC_HEIGHT		= 3L;	///< 高さ 実数最大桁数
	static const bool HAS_NULL				= false;			///< NULL許容するかどうか
public:
	static const long INDEX_LON		= 1L;
	static const long INDEX_LAT		= 2L;
	static const long INDEX_HEIGHT	= 3L;
};
typedef std::shared_ptr<Location> LocationPtr;

// ==================================================
// Geometry
// ==================================================
/**
 * ジオメトリクラス
 */
class Geometry
{
public:
	Geometry(){}
	virtual ~Geometry(){}

	/**
	 * ポイントの追加
	 *
	 * @param[in]	loc	Location
	 *
	 * @retval	true	追加成功
	 * @retval	false	追加失敗(書式エラーあり)
	 */
	virtual bool AddPoint(const Location& loc) = 0;
	/**
	 * ポイントの追加
	 *
	 * @param[in]	loc	パース前文字列(ex: "135.0,35.0,5.0")
	 *
	 * @retval	true	追加成功
	 * @retval	false	追加失敗(書式エラーあり)
	 */
	virtual bool AddPoint(const std::string& loc) = 0;
	/**
	 * ポイントの追加
	 *
	 * @param[in]	lon	経度(ex: "135.0")
	 * @param[in]	lat	緯度(ex: "35.0")
	 * @param[in]	he	高さ(ex: "5.0")
	 *
	 * @retval	true	追加成功
	 * @retval	false	追加失敗(書式エラーあり)
	 */
	virtual bool AddPoint(const std::string& lon, const std::string& lat, const std::string& he) = 0;
	///**
	// * ポイントの追加
	// *
	// * @param[in]	lon	経度
	// * @param[in]	lat	緯度
	// * @param[in]	he	高さ
	// */
	//virtual bool AddPoint(const FieldType<double>& lon, const FieldType<double>& lat, const FieldType<double>& he) = 0;
	
	/**
	 * ポイントの削除
	 *
	 * @param[in]	loc	Location
	 */
	virtual bool ErasePoint(const Location& loc) = 0;
	/**
	 * ポイントの削除
	 *
	 *  @param[in]	loc	デリミタパース前文字列(ex: "135.0,35.0,5.0")
	 */
	virtual bool ErasePoint(const std::string& loc) = 0;
	/**
	 * ポイントの削除
	 *
	 * @param[in]	lon	経度(ex: "135.0")
	 * @param[in]	lat	緯度(ex: "35.0")
	 * @param[in]	he	高さ(ex: "5.0")
	 */
	virtual bool ErasePoint(const std::string& lon, const std::string& lat, const std::string& he) = 0;
	///**
	// * ポイントの削除
	// *
	// * @param[in]	lon	経度
	// * @param[in]	lat	緯度
	// * @param[in]	he	高さ
	// */
	//virtual bool ErasePoint(const FieldType<double>& lon, const FieldType<double>& lat, const FieldType<double>& he) = 0;
	/**
	 * 全ポイント削除
	 */
	virtual bool Clear() = 0;
	///**
	// * ポイントの設定/取得
	// */
	//virtual bool SetGeometry(const Location& loc) = 0;
	//virtual Location GetGeometry() = 0;
	/**
	 * 文字数チェック
	 */
	virtual bool CheckLength() = 0;
	/**
	 * 整合性チェック
	 */
	virtual bool CheckIntegrity() const = 0;

	/**
	 * std::string取得
	 */
	virtual std::string GetStr() const = 0;
protected:
	/**
	 * 登録済みのポイントか
	 */
	bool HasSameLoc(const Location& loc, const std::vector<Location>& locVec)
	{
		std::vector<Location>::const_iterator itr = std::find(locVec.begin(), locVec.end(), loc);
		if(itr != locVec.end())
		{
			return true;
		}

		return false;
	}
};

class Point : public Geometry
{
public:
	Point(){}
	Point(const Location& loc) : _loc(loc){}
	Point(const std::string& loc) : _loc(loc){}
	Point(const std::string& lon, const std::string& lat, const std::string& he)
	{
		_loc.longitude.SetText(lon);
		_loc.latitude.SetText(lat);
		_loc.height.SetText(he);
	}
	//Point(const FieldType<double>& lon, const FieldType<double>& lat, const FieldType<double>& he) : _loc(lon, lat, he){}
	~Point(){}
	bool AddPoint(const Location& loc)
	{
		Location tmpLoc;
		if(tmpLoc.SetLocation(loc))
		{
			_loc = tmpLoc;
			return true;
		}
		return false;
	}
	bool AddPoint(const std::string& loc)
	{
		Location tmpLoc;
		if(tmpLoc.SetLocation(loc))
		{
			_loc = tmpLoc;
			return true;
		}
		return false;
	}
	bool AddPoint(const std::string& lon, const std::string& lat, const std::string& he)
	{
		Location tmpLoc;
		if(tmpLoc.SetLocation(lon, lat, he))
		{
			_loc = tmpLoc;
			return true;
		}
		return false;
	}
	//bool AddPoint(const FieldType<double>& lon, const FieldType<double>& lat, const FieldType<double>& he)
	//{
	//	_loc.longitude = lon;
	//	_loc.latitude = lat;
	//	_loc.height = he;
	//	return true;
	//}
	bool ErasePoint(const Location& loc)
	{
		_loc.Clear();
		return true;
	}
	bool ErasePoint(const std::string& loc)
	{
		_loc.Clear();
		return true;
	}
	bool ErasePoint(const std::string& lon, const std::string& lat, const std::string& he)
	{
		_loc.Clear();
		return true;
	}
	//bool ErasePoint(const FieldType<double>& lon, const FieldType<double>& lat, const FieldType<double>& he)
	//{
	//	_loc.Clear();
	//	return true;
	//}
	bool Clear()
	{
		_loc.Clear();
		return true;
	}
	bool CheckLength()
	{
		//bool ret = true;

		//ret &= _loc.longitude.Check();
		//ret &= _loc.latitude.Check();
		//ret &= _loc.height.Check();

		//return ret;
		//チェック済みのためSkip
		return true;
	}
	bool CheckIntegrity() const
	{
		////lon,lat,h のうちどれかに値が設定されていればOK
		//if(_loc.longitude.GetValue() != 0 || _loc.latitude.GetValue() != 0 || _loc.height.GetValue() != 0)
		//{
		//	return true;
		//}

		return true;
	}
	std::string GetStr() const
	{
		return _loc.longitude.GetText() + "," + _loc.latitude.GetText() + "," + _loc.height.GetText();
	}
	bool SetGeometry(const Location& loc)
	{
		return AddPoint(loc);
	}
	Location GetGeometry() const
	{
		return _loc;
	}
private:
	Location		_loc;
};

class Polyline : public Geometry
{
public:
	Polyline(){}
	Polyline(const Location& loc)
	{
		_loc.clear();
		_loc.push_back(loc);
	}
	Polyline(const std::string& loc)
	{
		_loc.clear();
		Location tmpLoc(loc);
		_loc.push_back(tmpLoc);
	}
	Polyline(const std::string& lon, const std::string& lat, const std::string& he)
	{
		_loc.clear();
		Location tmpLoc(lon, lat, he);
		_loc.push_back(tmpLoc);
	}
	//Polyline(const FieldType<double>& lon, const FieldType<double>& lat, const FieldType<double>& he)
	//{
	//	Location loc(lon, lat, he);
	//	_loc.push_back(loc);
	//}
	~Polyline(){}
	bool AddPoint(const Location& loc)
	{
		Location tmpLoc;
		if(tmpLoc.SetLocation(loc))
		{
			if(!HasSameLoc(tmpLoc, _loc))
			{
				_loc.push_back(tmpLoc);
				return true;
			}
		}

		return false;
	}
	bool AddPoint(const std::string& loc)
	{
		Location tmpLoc;
		if(tmpLoc.SetLocation(loc))
		{
			if(!HasSameLoc(tmpLoc, _loc))
			{
				_loc.push_back(tmpLoc);
				return true;
			}
		}

		return false;

		//Location tmpLoc(loc);
		//return AddPoint(tmpLoc);
	}
	//bool AddPoint(const FieldType<double>& lon, const FieldType<double>& lat, const FieldType<double>& he)
	//{
	//	Location tmpLoc;
	//	if(tmpLoc.SetLocation(lon, lat, he))
	//	{
	//		return AddPoint(tmpLoc);
	//	}
	//	
	//	return false;
	//}
	bool AddPoint(const std::string& lon, const std::string& lat, const std::string& he)
	{
		Location tmpLoc;
		if(tmpLoc.SetLocation(lon, lat, he))
		{
			if(!HasSameLoc(tmpLoc, _loc))
			{
				_loc.push_back(tmpLoc);
				return true;
			}
			else
			{
				LogUtil::printWarning(9,  "ジオメトリエラー", "ジオメトリ構成点が重複しています。該当構成点をスキップします",lon + "," + lat + "," + he);
			}
		}

		//フォーマットでは規定していないのでWAR
		//return false;
		return true;

		//Location tmpLoc(lon, lat, he);
		//return AddPoint(tmpLoc);
	}
	bool ErasePoint(const Location& loc)
	{
		//同一ポイントが存在してたら削除
		if(HasSameLoc(loc, _loc))
		{
			std::vector<Location>::iterator itr;
			for(itr = _loc.begin(); itr != _loc.end();)
			{
				if((*itr) == loc)
				{
					itr = _loc.erase(itr);
					continue;
				}
				itr++;
			}
		}

		return true;
	}
	bool ErasePoint(const std::string& loc)
	{
		Location tmpLoc(loc);
		return ErasePoint(tmpLoc);

		return true;
	}
	bool ErasePoint(const std::string& lon, const std::string& lat, const std::string& he)
	{
		Location loc(lon, lat, he);
		return ErasePoint(loc);
	}
	//bool ErasePoint(const FieldType<double>& lon, const FieldType<double>& lat, const FieldType<double>& he)
	//{
	//	Location loc(lon, lat, he);
	//	return ErasePoint(loc);
	//}
	bool Clear()
	{
		_loc.clear();
		return true;
	}
	bool CheckLength()
	{
		//bool ret = true;

		//for(auto& loc : _loc)
		//{
		//	ret &= loc.longitude.Check();
		//	ret &= loc.latitude.Check();
		//	ret &= loc.height.Check();
		//}
		//return ret;
		//チェック済みのためSkip
		return true;
	}
	bool CheckIntegrity() const
	{
		bool ret = true;

		//2点以上か
		if(_loc.size() < 2)
		{
			LogUtil::printError(8, "ジオメトリエラー\tジオメトリ構成点が2点未満です");
			ret &= false;
		}
		else
		{
			//始点と終点が違うか
			if(_loc.front() == _loc.back())
			{
				LogUtil::printError(7, "ジオメトリエラー\tジオメトリ構成点の始点、終点が一致しています");
				ret &= false;
			}
		}

		return ret;
	}
	std::string GetStr() const
	{
		std::string str;
		int count = 0;
		for(const auto& loc : _loc)
		{
			if(count > 0)
			str += loc.longitude.GetText() + "," + loc.latitude.GetText() + "," + loc.height.GetText();
			count++;
		}
		return str;
	}
	bool SetGeometry(const std::vector<Location>& loc)
	{
		_loc = loc;
		return true;
	}
	std::vector<Location> GetGeometry() const
	{
		return _loc;
	}
private:
	std::vector<Location> _loc;
};

class Polygon : public Geometry
{
public:
	Polygon(){}
	Polygon(const Location& loc)
	{
		_loc.push_back(loc);
	}
	Polygon(const std::string& loc)
	{
		Location tmpLoc(loc);
		_loc.push_back(tmpLoc);
	}
	Polygon(const std::string& lon, const std::string& lat, const std::string& he)
	{
		_loc.clear();
		Location loc(lon, lat, he);
		_loc.push_back(loc);
	}
	//Polygon(const FieldType<double>& lon, const FieldType<double>& lat, const FieldType<double>& he)
	//{
	//	Location loc(lon, lat, he);
	//	_loc.push_back(loc);
	//}
	~Polygon(){}
	bool AddPoint(const Location& loc)
	{
		//始終点は重複するので重複チェックなし
		Location tmpLoc;
		if(tmpLoc.SetLocation(loc))
		{
			_loc.push_back(tmpLoc);
			return true;
		}
		return false;
	}
	bool AddPoint(const std::string& loc)
	{
		Location tmpLoc;
		if(tmpLoc.SetLocation(loc))
		{
			_loc.push_back(tmpLoc);
			return true;
		}

		return false;

		//Location tmpLoc(loc);
		//return AddPoint(loc);
	}
	bool AddPoint(const std::string& lon, const std::string& lat, const std::string& he)
	{
		Location tmpLoc;
		if(tmpLoc.SetLocation(lon, lat, he))
		{
			_loc.push_back(tmpLoc);
			return true;
		}

		return false;

		//Location loc(lon, lat, he);
		//return AddPoint(loc);
	}
	//bool AddPoint(const FieldType<double>& lon, const FieldType<double>& lat, const FieldType<double>& he)
	//{
	//	Location loc(lon, lat, he);
	//	return AddPoint(loc);
	//}
	bool ErasePoint(const Location& loc)
	{
		//同一ポイントが存在してたら削除
		if(HasSameLoc(loc, _loc))
		{
			std::vector<Location>::iterator itr;
			for(itr = _loc.begin(); itr != _loc.end();)
			{
				if((*itr) == loc)
				{
					itr = _loc.erase(itr);
					continue;
				}
				itr++;
			}
		}

		return true;
	}
	bool ErasePoint(const std::string& loc)
	{
		Location tmpLoc(loc);
		return ErasePoint(tmpLoc);

		return true;
	}
	bool ErasePoint(const std::string& lon, const std::string& lat, const std::string& he)
	{
		Location loc(lon, lat, he);
		return ErasePoint(loc);
	}
	//bool ErasePoint(const FieldType<double>& lon, const FieldType<double>& lat, const FieldType<double>& he)
	//{
	//	Location loc(lon, lat, he);
	//	return ErasePoint(loc);
	//}
	bool Clear()
	{
		_loc.clear();
		return true;
	}
	bool CheckLength()
	{
		//bool ret = true;

		//for(auto& loc : _loc)
		//{
		//	ret &= loc.longitude.Check();
		//	ret &= loc.latitude.Check();
		//	ret &= loc.height.Check();
		//}
		//return ret;

		//チェック済みのためSkip
		return true;
	}
	bool CheckIntegrity() const
	{
		bool ret = true;
	
		//3点以上か
		if(_loc.size() < 3)
		{
			LogUtil::printError(11, "ジオメトリエラー\tジオメトリ構成点が3点未満です");
			ret &= false;
		}
		else
		{
			//始点と終点が同じか
			if(_loc.front() != _loc.back())
			{
				LogUtil::printError(10, "ジオメトリエラー\tジオメトリ構成点の始終点が一致していません");
				ret &= false;
			}
		}

		return ret;
	}
	std::string GetStr() const
	{
		std::string str;
		int count = 0;
		for(const auto& loc : _loc)
		{
			if(count > 0)
			str += loc.longitude.GetText() + "," + loc.latitude.GetText() + "," + loc.height.GetText();
			count++;
		}
		return str;
	}
	bool SetGeometry(const std::vector<Location>& loc)
	{
		_loc = loc;
		return true;
	}
	std::vector<Location> GetGeometry() const
	{
		return _loc;
	}
private:
	std::vector<Location> _loc;
};

// ==================================================
// FieldTypeConverter
// ==================================================
/**
 * 型⇔std::string 変換
 */
template<typename T>
class FieldTypeConverter
{
public:
	FieldTypeConverter(){}
	~FieldTypeConverter(){}
public:

	/**
	 * Tからstd::string に変換
	 */
	static std::string GetStr(const T& value)
	{
		return std::to_string(value);
	}

	/**
	 * std::string から T に変換
	 */	
	static T GetValue(const std::string& text)
	{
		return std::stoi(text);
	}
};

// int部分特殊化
template<>
static int FieldTypeConverter<int>::GetValue(const std::string& text)
{
	return  std::stoi(text);
}

// long部分特殊化
template<>
static long FieldTypeConverter<long>::GetValue(const std::string& text)
{
	return  std::stol(text);
}

// long long部分特殊化
template<>
static long long FieldTypeConverter<long long>::GetValue(const std::string& text)
{
	return  std::stoll(text);
}

// double部分特殊化
template<>
static double FieldTypeConverter<double>::GetValue(const std::string& text)
{
	return  std::stod(text);
}

// uh::tstring部分特殊化
template<>
static std::string FieldTypeConverter<uh::tstring>::GetStr(const uh::tstring& value)
{
	return  uh::toStr(value);
}
template<>
static uh::tstring FieldTypeConverter<uh::tstring>::GetValue(const std::string& text)
{
	return  uh::toTStr(text);
}

//Point部分特殊化
template<>
static std::string  FieldTypeConverter<Point>::GetStr(const Point& value)
{
	return value.GetStr();
}
template<>
static Point FieldTypeConverter<Point>::GetValue(const std::string& text)
{
	Point value(text);

	return value;
}

//Polyline部分特殊化
template<>
static std::string  FieldTypeConverter<Polyline>::GetStr(const Polyline& value)
{
	return value.GetStr();
}
template<>
static Polyline FieldTypeConverter<Polyline>::GetValue(const std::string& text)
{
	Polyline value(text);

	return value;
}

//Polygon部分特殊化
template<>
static std::string  FieldTypeConverter<Polygon>::GetStr(const Polygon& value)
{
	return value.GetStr();
}
template<>
static Polygon FieldTypeConverter<Polygon>::GetValue(const std::string& text)
{
	Polygon value(text);

	return value;
}

// ==================================================
// FieldChecker
// ==================================================
/**
 * フォーマットチェッカーテンプレート
 */
template<typename T>
class FieldChecker
{
public:
	FieldChecker(){}
	~FieldChecker(){}
	
	/**
	 * 最小値チェック
	 *
	 * @param[in]	target	チェック対象値
	 * @param[in]	value	チェック値
	 * @param[in]	name	フィールド名(ログ用)
	 *
	 * @retval	true	OK(フォーマット準拠)
	 * @retval	false	NG
	 */
	static bool CheckMin(const T& target, const T& value, const uh::tstring& name)
	{
		//表示用にstring変換
		std::string targetStr = FieldTypeConverter<T>::GetStr(target);
		std::string valueStr = FieldTypeConverter<T>::GetStr(value);

		if(target < value)
		{
			//LogUtil::printError("値:" + targetStr + "\tフォーマットエラー\t最小値:" + valueStr + " 範囲外です");
			LogUtil::printError(2, "最小値エラー", uh::toStr(name) , "入力値:" + targetStr + "\t最小値:" + valueStr);
			return false;
		}
		return true;
	}

	/**
	 * 最大値チェック
	 *
	 * @param[in]	target	チェック対象値
	 * @param[in]	value	チェック値
	 * @param[in]	name	フィールド名(ログ用)
	 *
	 * @retval	true	OK(フォーマット準拠)
	 * @retval	false	NG
	 */
	static bool CheckMax(const T& target, const T& value, const uh::tstring& name)
	{
		//表示用にstring変換
		std::string targetStr = FieldTypeConverter<T>::GetStr(target);
		std::string valueStr = FieldTypeConverter<T>::GetStr(value);

		if(target > value)
		{
			//gUtil::printError("値:" + targetStr + "\tフォーマットエラー\t最大値:" + valueStr + " 範囲外です");
			LogUtil::printError(1, "最大値エラー", uh::toStr(name), "入力値:" + targetStr + "\t最大値:" + valueStr);
			return false;
		}
		return true;
	}

	/**
	 * 有効値チェック
	 *
	 * @param[in]	target	チェック対象値
	 * @param[in]	value	チェック値
	 * @param[in]	name	フィールド名(ログ用)
	 *
	 * @retval	true	OK(フォーマット準拠)
	 * @retval	false	NG
	 */
	static bool CheckValidValue(const T& target, const std::vector<T>& values, const uh::tstring& name)
	{
		//表示用にstring変換
		std::string targetStr = FieldTypeConverter<T>::GetStr(target);
		std::string valueStr;
		long count = 0;
		for(const auto& str: values)
		{
			if(count > 0)
			{
				valueStr += ",";
			}
			valueStr += FieldTypeConverter<T>::GetStr(str);
			count++;
		}

		if( std::find(values.begin(), values.end(), target) == values.end() )
		{
			//LogUtil::printError("値:" + targetStr + "\tフォーマットエラー\t有効値 範囲外です");
			LogUtil::printError(3, "有効値エラー", uh::toStr(name), "入力値:" + targetStr + "\t有効値:" + valueStr);
			return false;
		}
		return true;
	}

	/**
	 * 桁数チェック
	 *
	 * @param[in]	target	チェック対象値
	 * @param[in]	valueInt	チェック値
	 * @param[in]	valueDec	チェック値
	 * @param[in]	name	フィールド名(ログ用)
	 *
	 * @retval	true	OK(フォーマット準拠)
	 * @retval	false	NG
	 */
	static bool CheckLength(const T& target, const std::string& targetStr, long valueInt, long valueDec, const uh::tstring& name)
	{
		//文字
		if(typeid(target) == typeid(uh::tstring))
		{
			//文字列数
			if(targetStr.size() > valueInt)
			{
				//LogUtil::printError("値:" + target + "\tフォーマットエラー\t桁数:" + std::to_string(valueInt) + " 範囲外です");
				LogUtil::printError(5, "最大桁数エラー(文字列)", uh::toStr(name), "値:" + targetStr +"\t入力桁数:" + std::to_string(targetStr.size()) + "\t最大桁数:" + std::to_string(valueInt));
				return false;
			}
			
			return true;
		}
		//整数
		else if(typeid(target) == typeid(int) || typeid(target) == typeid(long) || typeid(target) == typeid(long long))
		{
			//マイナス削除
			std::string strValue = targetStr;
			if(strValue.find("-") != std::string::npos)
			{
				strValue.erase(0, 1);
			}

			if( valueInt > 0 )
			{
				if(strValue.size() > valueInt)
				{

					//LogUtil::printError("値:" + targetStr + "\tフォーマットエラー\t整数桁数:" + std::to_string(strValue.size()) + ", 整数最大桁数:" + std::to_string(valueInt) + " 範囲外です");
					LogUtil::printError(5, "最大桁数エラー(整数)", uh::toStr(name), "値:" + targetStr +"\t入力桁数:" + std::to_string(strValue.size()) + "\t最大桁数:" + std::to_string(valueInt));
					return false;
				}
			}

			//小数桁
			if( valueDec > 0 )
			{
				//LogUtil::printWarning("整数データ型のため、小数桁のチェックは省略します");
				LogUtil::printWarning(5, uh::toStr(name) + "\t整数データ型のため、小数桁のチェックは省略します");
			}
			return true;
		}
		//小数
		else if(typeid(target) == typeid(double))
		{
			//マイナス削除
			std::string strValue = targetStr;
			if(strValue.find("-") != std::string::npos)
			{
				strValue.erase(0, 1);
			}

			size_t length = strValue.size();
			std::string::size_type index = strValue.find( "." );
			if( index == std::string::npos ) 
			{
				LogUtil::printError(5, "最大桁数エラー(小数部)", uh::toStr(name), "値:" + targetStr +"\t入力桁数:" + std::to_string(0) + "\t最大桁数:" + std::to_string(valueDec));
				//return true;
				return false;
			}
			bool ret = true;
			//整数桁
			if( index> valueInt )
			{
				//LogUtil::printError("値:" + targetStr + "\tフォーマットエラー\t整数桁数:" + std::to_string(index) + ", 整数最大桁数:" + std::to_string(valueInt) + " 範囲外です");
				LogUtil::printError(5, "最大桁数エラー(整数部)", uh::toStr(name), "値:" + targetStr +"\t入力桁数:" + std::to_string(index) + "\t最大桁数:" + std::to_string(valueInt));
				ret &= false;
			}
			//小数桁
			if( (length - index - 1) != valueDec )
			{
				//LogUtil::printError("値:" + targetStr + "\tフォーマットエラー\t小数桁数:" + std::to_string(length - index - 1) + ", 小数最大桁数:" + std::to_string(valueDec) + " 範囲外です");
				LogUtil::printError(5, "最大桁数エラー(小数部)", uh::toStr(name), "値:" + targetStr +"\t入力桁数:" + std::to_string(length - index - 1) + "\t最大桁数:" + std::to_string(valueDec));
				ret &= false;
			}

			return ret;
		}
		return true;
	}

	/**
	 * 整合性チェック
	 *
	 * @param[in]	target	チェック対象値
	 * @param[in]	name	フィールド名(ログ用)
	 *
	 * @retval	true	OK(フォーマット準拠)
	 * @retval	false	NG
	 */
	static bool CheckIntegrity(const T& target, const uh::tstring& name)
	{
		return true;
	}
};

//Point特殊化
template<>
static bool FieldChecker<Point>::CheckMin(const Point& target, const Point& value, const uh::tstring& name)
{
	//チェックなし
	return true;
}
template<>
static bool FieldChecker<Point>::CheckMax(const Point& target, const Point& value, const uh::tstring& name)
{
	//チェックなし
	return true;
}
template<>
static bool FieldChecker<Point>::CheckValidValue(const Point& target, const std::vector<Point>& values, const uh::tstring& name)
{
	//チェックなし
	return true;
}
template<>
static bool FieldChecker<Point>::CheckLength(const Point& target, const std::string& targetStr, long valueInt, long valueDec, const uh::tstring& name)
{
	//チェック済みなのでSKIP
	return true;
}
template<>
static bool FieldChecker<Point>::CheckIntegrity(const Point& target, const uh::tstring& name)
{
	return target.CheckIntegrity();	
}

//Polyline特殊化
template<>
static bool FieldChecker<Polyline>::CheckMin(const Polyline& target, const Polyline& value, const uh::tstring& name)
{
	//チェックなし
	return true;
}
template<>
static bool FieldChecker<Polyline>::CheckMax(const Polyline& target, const Polyline& value, const uh::tstring& name)
{
	//チェックなし
	return true;
}
template<>
static bool FieldChecker<Polyline>::CheckValidValue(const Polyline& target, const std::vector<Polyline>& values, const uh::tstring& name)
{
	//チェックなし
	return true;
}
template<>
static bool FieldChecker<Polyline>::CheckLength(const Polyline& target, const std::string& targetStr, long valueInt, long valueDec, const uh::tstring& name)
{
	//チェック済みなのでSKIP
	return true;
}
template<>
static bool FieldChecker<Polyline>::CheckIntegrity(const Polyline& target, const uh::tstring& name)
{
	return target.CheckIntegrity();
}

//Polygon特殊化
template<>
static bool FieldChecker<Polygon>::CheckMin(const Polygon& target, const Polygon& value, const uh::tstring& name)
{
	//チェックなし
	return true;
}
template<>
static bool FieldChecker<Polygon>::CheckMax(const Polygon& target, const Polygon& value, const uh::tstring& name)
{
	//チェックなし
	return true;
}
template<>
static bool FieldChecker<Polygon>::CheckValidValue(const Polygon& target, const std::vector<Polygon>& values, const uh::tstring& name)
{
	//チェックなし
	return true;
}
template<>
static bool FieldChecker<Polygon>::CheckLength(const Polygon& target, const std::string& targetStr, long valueInt, long valueDec, const uh::tstring& name)
{
	//チェック済みなのでSKIP
	return true;
}
template<>
static bool FieldChecker<Polygon>::CheckIntegrity(const Polygon& target, const uh::tstring& name)
{
	return target.CheckIntegrity();

}

};	//namespace adam_v2