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

// FieldType �̖��O�����̂ŁA���O��Ԃ𕪂���
namespace adam_v2
{
template<typename T>
class FieldChecker;

template<typename T>
class FieldTypeConverter;

/**
 * �t�B�[���h���ʗv�f
 */
class CommonFieldType
{
public:
	/**
	 * �R���X�g���N�^
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
	 * �f�X�g���N�^
	 */
	virtual ~CommonFieldType(){}

	/**
	 * �������`�F�b�N
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
	long			_index;				///< ��ԍ�
	uh::tstring		_name;				///< �񖼏�(�w�b�_����)
	std::string		_text;				///< �ǂݍ��񂾂܂܂̃f�[�^

	bool			_isNull;			///< NULL�ł��邩�ǂ���

	//�`�F�b�N�p
	bool			_hasNull;			///< NULL���e���ǂ���(true:NULL��)
	//bool			_canOmit;			///< �ȗ��\���ǂ���(true:�ȗ���)	���ȗ��͋��e���Ȃ�
	long			_lengthInt;			///< �����ő包�� or �ő啶����(0�`)
	long			_lengthDecimal;		///< �����ő包��(%.Nf)(0�`)
};

/**
 * �t�B�[���h�v�f �e���v���[�g
 */
template<typename T>
class FieldType : public CommonFieldType// , public FieldChecker<T>
{
public:
	/**
	 * �f�t�H���g�R���X�g���N�^
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
	 * �R���X�g���N�^
	 *
	 * �`�F�b�N�Ȃ�
	 *
	 * @param[in]	index			��ԍ�
	 * @param[in]	name			�񖼏�(�w�b�_����)
	 * @param[in]	initial			�����l
	 * @param[in]	hasNull			NULL���e
	 * @param[in]	lengthInt		�����ő包�� or �ő啶����
	 * @param[in]	lengthDecimal	�����ő包��
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
	 * �R���X�g���N�^
	 *
	 * �ő�/�ŏ��`�F�b�N
	 *
	 * @param[in]	index	��ԍ�
	 * @param[in]	name	�񖼏�(�w�b�_����)
	 * @param[in]	initial		�����l
	 * @param[in]	min			�ŏ��l
	 * @param[in]	max			�ő�l
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
	 * �R���X�g���N�^
	 *
	 * �͈̓`�F�b�N
	 *
	 * @param[in]	index	��ԍ�
	 * @param[in]	name	�񖼏�(�w�b�_����)
	 * @param[in]	initial		�����l
	 * @param[in]	validElemts	�L���l
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
	 * �f�X�g���N�^
	 */
	~FieldType(void){}

	/**
	 * �e�L�X�g(�ǂݍ��݃f�[�^)�̐ݒ�
	 * 
	 * @param[in]	value	�ǂݍ��݃f�[�^
	 * 
	 * @retval	true	����
	 * @retval	false	���s
	 */
	bool SetText(const std::string& text)
	{
		if(text == "")
		{
			return SetIsNull(true);
		}

		//�`�F�b�NNG�̂��̂͒e��

		//std::string��T�ϊ�
		T value = FieldTypeConverter<T>::GetValue(text);
		
		//�W�I���g���͂����ł͊m�肵�Ȃ��̂Ń`�F�b�N�ł��Ȃ�
		//�W�I���g���m���� SurveyCSVReader::CheckGeometryIntegrity() �ɂĎ��{
		if(Check(value, text, false))
		{
			_text = text;
			_value = value;
			
			return true;
		}

		return false;
	}

	/**
	 * �l�̐ݒ�
	 *
	 * �`�F�b�NOK�Ȃ�i�[����
	 *
	 * @param[in]	value	�w��^�̒l
	 * 
	 * @retval	true	����
	 * @retval	false	���s
	 */
	bool SetValue(const T& value)
	{
		//std::string��T�ϊ�
		std::string text = FieldTypeConverter<T>::GetStr(value);

		//�W�I���g���͂����ł͊m�肵�Ȃ��̂Ń`�F�b�N�ł��Ȃ�
		//�W�I���g���m���� SurveyCSVReader::CheckGeometryIntegrity() �ɂĎ��{
		if(Check(value, text, false))
		{
			_text = text;
			_value = value;
			
			return true;
		}
		
		return false;
	}

	/**
	 * �l�̎擾
	 * 
	 * @pre		SetValue() == true OR SetText() == true
	 * 
	 * @return	�l
	 */
	T GetValue() const
	{
		return _value;
	}

	/**
	 * �e�L�X�g�̎擾
	 * 
	 * @pre		SetValue() == true OR SetText() == true
	 * 
	 * @return	std::string�l
	 */
	std::string GetText() const
	{
		return _text;
	}

	/**
	 * NULL�l�̐ݒ�
	 *
	 * @param[in]	flg	true:NULL/false:NULL�ȊO
	 *
	 * @retval	true	NULL�ݒ� ���f����
	 * @retval	false	NULL�ݒ� ���f���s(NULL���e���Ă��Ȃ�)
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

			////�ݒ�ςݒl�̃N���A
			//Clear();

			return true;
		}
		else
		{
			LogUtil::printError(4, "NULL�G���[", uh::toStr(_name), "��NULL�t�B�[���h��NULL���w�肳��Ă��܂�");
		}
		
		return false;
	}

	/**
	 * NULL�l���ݒ肳��Ă��邩�ǂ���
	 *
	 * @retval	true	NULL�ł���
	 * @retval	false	NULL�łȂ�
	 */
	bool GetIsNull() const
	{
		return _isNull;
	}

	/**
	 * �S�`�F�b�N���s
	 *
	 * @retval	true	�`�F�b�NOK
	 * @retval	false	�`�F�b�NNG
	 */
	bool Check(bool checkIntegrity = false)
	{
		return Check(_value, _text, checkIntegrity);
	}

	/**
	 * �S�`�F�b�N���s
	 *
	 * @retval	true	�`�F�b�NOK
	 * @retval	false	�`�F�b�NNG
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
	 * �l�̃N���A
	 */
	void Clear()
	{
		CommonFieldType::Clear();
	}
private:
	T				_initial;				///< �����l
	T				_value;					///< �f�[�^�̎���

	//�`�F�b�N�p[�C��]
	T				_minValue;				///< [�C��]�ŏ��l
	T				_maxValue;				///< [�C��]�ő�l
	std::vector<T>	_validValue;			///< [�C��]�L���l

	bool			_isCheckMinMax;			///< �ŏ�/�ő�`�F�b�N�L����
	bool			_isCheckValidValue;		///< �L���l�`�F�b�N���L����
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
 * �ʒu���N���X
 */
class Location
{

public:
	FieldType<double> longitude	;	///< �ܓx
	FieldType<double> latitude	;	///< �o�x
	FieldType<double> height	;	///< ����
public:
	/**
	 * �f�t�H���g�R���X�g���N�^
	 */
	Location() :
		//					�^									��ԍ�	�t�B�[���h��		�����l		NULL���e		�����ő包��		�����ő包��		(option)(�ŏ��l		�ő�l) or �L���l
		longitude		(	FieldType<double>				(	INDEX_LON,		_T("LON"),			0.0,		HAS_NULL,		LENGTH_INT_LONLAT,	LENGTH_DEC_LONLAT		))
		,latitude		(	FieldType<double>				(	INDEX_LAT,		_T("LAT"),			0.0,		HAS_NULL,		LENGTH_INT_LONLAT,	LENGTH_DEC_LONLAT		))
		,height			(	FieldType<double>				(	INDEX_HEIGHT,	_T("HEIGHT"),		0.0,		HAS_NULL,		LENGTH_INT_HEIGHT,	LENGTH_DEC_HEIGHT		))
	{}
	///**
	// * �R���X�g���N�^
	// *
	// * @param[in]	lon	�o�x
	// * @param[in]	lat	�ܓx
	// * @param[in]	he	����
	// *
	// * @note	�`�F�b�N���ʂ̖߂�l�Ȃ��̂��� SetLocation(...)���g���Ă�������
	// */
	//Location(const FieldType<double>& lon, const FieldType<double>& lat, const FieldType<double>& he)
	//{
	//	_createLocation();

	//	longitude = lon;
	//	latitude = lat;
	//	height = he;
	//}
	/**
	 * �R���X�g���N�^
	 *
	 * @param[in]	lon	�o�x (ex: "135.00000000")
	 * @param[in]	lat	�ܓx (ex: "35.00000000")
	 * @param[in]	he	���� (ex: "5.000")
	 *
	 * @note	�`�F�b�N���ʂ̖߂�l�Ȃ��̂��� SetLocation(...)���g���Ă�������
	 */
	Location(const std::string& lon, const std::string& lat, const std::string& he)
	{
		SetLocation(lon, lat, he);
	}
	/**
	 * �R���X�g���N�^
	 *
	 * @param[in]	text	�p�[�X�O������(ex: "135.00000000,35.00000000,5.000")
	 * 
	 * @note	�`�F�b�N���ʂ̖߂�l�Ȃ��̂��� SetLocation(...)���g���Ă�������
	 */
	Location(const std::string& text)
	{
		SetLocation(text);
	}
	/**
	 * �R���X�g���N�^
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
	 * �f�X�g���N�^
	 */
	~Location(){}

	//find�p
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
	 * �l�̃N���A
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
	// �`�F�b�N�p�萔�l
	static const long LENGTH_INT_LONLAT		= 3L;	///< �ܓx�o�x �����ő包��
	static const long LENGTH_DEC_LONLAT		= 8L;	///< �ܓx�o�x �����ő包��
	static const long LENGTH_INT_HEIGHT		= 8L;	///< ���� �����ő包��
	static const long LENGTH_DEC_HEIGHT		= 3L;	///< ���� �����ő包��
	static const bool HAS_NULL				= false;			///< NULL���e���邩�ǂ���
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
 * �W�I���g���N���X
 */
class Geometry
{
public:
	Geometry(){}
	virtual ~Geometry(){}

	/**
	 * �|�C���g�̒ǉ�
	 *
	 * @param[in]	loc	Location
	 *
	 * @retval	true	�ǉ�����
	 * @retval	false	�ǉ����s(�����G���[����)
	 */
	virtual bool AddPoint(const Location& loc) = 0;
	/**
	 * �|�C���g�̒ǉ�
	 *
	 * @param[in]	loc	�p�[�X�O������(ex: "135.0,35.0,5.0")
	 *
	 * @retval	true	�ǉ�����
	 * @retval	false	�ǉ����s(�����G���[����)
	 */
	virtual bool AddPoint(const std::string& loc) = 0;
	/**
	 * �|�C���g�̒ǉ�
	 *
	 * @param[in]	lon	�o�x(ex: "135.0")
	 * @param[in]	lat	�ܓx(ex: "35.0")
	 * @param[in]	he	����(ex: "5.0")
	 *
	 * @retval	true	�ǉ�����
	 * @retval	false	�ǉ����s(�����G���[����)
	 */
	virtual bool AddPoint(const std::string& lon, const std::string& lat, const std::string& he) = 0;
	///**
	// * �|�C���g�̒ǉ�
	// *
	// * @param[in]	lon	�o�x
	// * @param[in]	lat	�ܓx
	// * @param[in]	he	����
	// */
	//virtual bool AddPoint(const FieldType<double>& lon, const FieldType<double>& lat, const FieldType<double>& he) = 0;
	
	/**
	 * �|�C���g�̍폜
	 *
	 * @param[in]	loc	Location
	 */
	virtual bool ErasePoint(const Location& loc) = 0;
	/**
	 * �|�C���g�̍폜
	 *
	 *  @param[in]	loc	�f���~�^�p�[�X�O������(ex: "135.0,35.0,5.0")
	 */
	virtual bool ErasePoint(const std::string& loc) = 0;
	/**
	 * �|�C���g�̍폜
	 *
	 * @param[in]	lon	�o�x(ex: "135.0")
	 * @param[in]	lat	�ܓx(ex: "35.0")
	 * @param[in]	he	����(ex: "5.0")
	 */
	virtual bool ErasePoint(const std::string& lon, const std::string& lat, const std::string& he) = 0;
	///**
	// * �|�C���g�̍폜
	// *
	// * @param[in]	lon	�o�x
	// * @param[in]	lat	�ܓx
	// * @param[in]	he	����
	// */
	//virtual bool ErasePoint(const FieldType<double>& lon, const FieldType<double>& lat, const FieldType<double>& he) = 0;
	/**
	 * �S�|�C���g�폜
	 */
	virtual bool Clear() = 0;
	///**
	// * �|�C���g�̐ݒ�/�擾
	// */
	//virtual bool SetGeometry(const Location& loc) = 0;
	//virtual Location GetGeometry() = 0;
	/**
	 * �������`�F�b�N
	 */
	virtual bool CheckLength() = 0;
	/**
	 * �������`�F�b�N
	 */
	virtual bool CheckIntegrity() const = 0;

	/**
	 * std::string�擾
	 */
	virtual std::string GetStr() const = 0;
protected:
	/**
	 * �o�^�ς݂̃|�C���g��
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
		//�`�F�b�N�ς݂̂���Skip
		return true;
	}
	bool CheckIntegrity() const
	{
		////lon,lat,h �̂����ǂꂩ�ɒl���ݒ肳��Ă����OK
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
				LogUtil::printWarning(9,  "�W�I���g���G���[", "�W�I���g���\���_���d�����Ă��܂��B�Y���\���_���X�L�b�v���܂�",lon + "," + lat + "," + he);
			}
		}

		//�t�H�[�}�b�g�ł͋K�肵�Ă��Ȃ��̂�WAR
		//return false;
		return true;

		//Location tmpLoc(lon, lat, he);
		//return AddPoint(tmpLoc);
	}
	bool ErasePoint(const Location& loc)
	{
		//����|�C���g�����݂��Ă���폜
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
		//�`�F�b�N�ς݂̂���Skip
		return true;
	}
	bool CheckIntegrity() const
	{
		bool ret = true;

		//2�_�ȏォ
		if(_loc.size() < 2)
		{
			LogUtil::printError(8, "�W�I���g���G���[\t�W�I���g���\���_��2�_�����ł�");
			ret &= false;
		}
		else
		{
			//�n�_�ƏI�_���Ⴄ��
			if(_loc.front() == _loc.back())
			{
				LogUtil::printError(7, "�W�I���g���G���[\t�W�I���g���\���_�̎n�_�A�I�_����v���Ă��܂�");
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
		//�n�I�_�͏d������̂ŏd���`�F�b�N�Ȃ�
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
		//����|�C���g�����݂��Ă���폜
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

		//�`�F�b�N�ς݂̂���Skip
		return true;
	}
	bool CheckIntegrity() const
	{
		bool ret = true;
	
		//3�_�ȏォ
		if(_loc.size() < 3)
		{
			LogUtil::printError(11, "�W�I���g���G���[\t�W�I���g���\���_��3�_�����ł�");
			ret &= false;
		}
		else
		{
			//�n�_�ƏI�_��������
			if(_loc.front() != _loc.back())
			{
				LogUtil::printError(10, "�W�I���g���G���[\t�W�I���g���\���_�̎n�I�_����v���Ă��܂���");
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
 * �^��std::string �ϊ�
 */
template<typename T>
class FieldTypeConverter
{
public:
	FieldTypeConverter(){}
	~FieldTypeConverter(){}
public:

	/**
	 * T����std::string �ɕϊ�
	 */
	static std::string GetStr(const T& value)
	{
		return std::to_string(value);
	}

	/**
	 * std::string ���� T �ɕϊ�
	 */	
	static T GetValue(const std::string& text)
	{
		return std::stoi(text);
	}
};

// int�������ꉻ
template<>
static int FieldTypeConverter<int>::GetValue(const std::string& text)
{
	return  std::stoi(text);
}

// long�������ꉻ
template<>
static long FieldTypeConverter<long>::GetValue(const std::string& text)
{
	return  std::stol(text);
}

// long long�������ꉻ
template<>
static long long FieldTypeConverter<long long>::GetValue(const std::string& text)
{
	return  std::stoll(text);
}

// double�������ꉻ
template<>
static double FieldTypeConverter<double>::GetValue(const std::string& text)
{
	return  std::stod(text);
}

// uh::tstring�������ꉻ
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

//Point�������ꉻ
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

//Polyline�������ꉻ
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

//Polygon�������ꉻ
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
 * �t�H�[�}�b�g�`�F�b�J�[�e���v���[�g
 */
template<typename T>
class FieldChecker
{
public:
	FieldChecker(){}
	~FieldChecker(){}
	
	/**
	 * �ŏ��l�`�F�b�N
	 *
	 * @param[in]	target	�`�F�b�N�Ώےl
	 * @param[in]	value	�`�F�b�N�l
	 * @param[in]	name	�t�B�[���h��(���O�p)
	 *
	 * @retval	true	OK(�t�H�[�}�b�g����)
	 * @retval	false	NG
	 */
	static bool CheckMin(const T& target, const T& value, const uh::tstring& name)
	{
		//�\���p��string�ϊ�
		std::string targetStr = FieldTypeConverter<T>::GetStr(target);
		std::string valueStr = FieldTypeConverter<T>::GetStr(value);

		if(target < value)
		{
			//LogUtil::printError("�l:" + targetStr + "\t�t�H�[�}�b�g�G���[\t�ŏ��l:" + valueStr + " �͈͊O�ł�");
			LogUtil::printError(2, "�ŏ��l�G���[", uh::toStr(name) , "���͒l:" + targetStr + "\t�ŏ��l:" + valueStr);
			return false;
		}
		return true;
	}

	/**
	 * �ő�l�`�F�b�N
	 *
	 * @param[in]	target	�`�F�b�N�Ώےl
	 * @param[in]	value	�`�F�b�N�l
	 * @param[in]	name	�t�B�[���h��(���O�p)
	 *
	 * @retval	true	OK(�t�H�[�}�b�g����)
	 * @retval	false	NG
	 */
	static bool CheckMax(const T& target, const T& value, const uh::tstring& name)
	{
		//�\���p��string�ϊ�
		std::string targetStr = FieldTypeConverter<T>::GetStr(target);
		std::string valueStr = FieldTypeConverter<T>::GetStr(value);

		if(target > value)
		{
			//gUtil::printError("�l:" + targetStr + "\t�t�H�[�}�b�g�G���[\t�ő�l:" + valueStr + " �͈͊O�ł�");
			LogUtil::printError(1, "�ő�l�G���[", uh::toStr(name), "���͒l:" + targetStr + "\t�ő�l:" + valueStr);
			return false;
		}
		return true;
	}

	/**
	 * �L���l�`�F�b�N
	 *
	 * @param[in]	target	�`�F�b�N�Ώےl
	 * @param[in]	value	�`�F�b�N�l
	 * @param[in]	name	�t�B�[���h��(���O�p)
	 *
	 * @retval	true	OK(�t�H�[�}�b�g����)
	 * @retval	false	NG
	 */
	static bool CheckValidValue(const T& target, const std::vector<T>& values, const uh::tstring& name)
	{
		//�\���p��string�ϊ�
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
			//LogUtil::printError("�l:" + targetStr + "\t�t�H�[�}�b�g�G���[\t�L���l �͈͊O�ł�");
			LogUtil::printError(3, "�L���l�G���[", uh::toStr(name), "���͒l:" + targetStr + "\t�L���l:" + valueStr);
			return false;
		}
		return true;
	}

	/**
	 * �����`�F�b�N
	 *
	 * @param[in]	target	�`�F�b�N�Ώےl
	 * @param[in]	valueInt	�`�F�b�N�l
	 * @param[in]	valueDec	�`�F�b�N�l
	 * @param[in]	name	�t�B�[���h��(���O�p)
	 *
	 * @retval	true	OK(�t�H�[�}�b�g����)
	 * @retval	false	NG
	 */
	static bool CheckLength(const T& target, const std::string& targetStr, long valueInt, long valueDec, const uh::tstring& name)
	{
		//����
		if(typeid(target) == typeid(uh::tstring))
		{
			//������
			if(targetStr.size() > valueInt)
			{
				//LogUtil::printError("�l:" + target + "\t�t�H�[�}�b�g�G���[\t����:" + std::to_string(valueInt) + " �͈͊O�ł�");
				LogUtil::printError(5, "�ő包���G���[(������)", uh::toStr(name), "�l:" + targetStr +"\t���͌���:" + std::to_string(targetStr.size()) + "\t�ő包��:" + std::to_string(valueInt));
				return false;
			}
			
			return true;
		}
		//����
		else if(typeid(target) == typeid(int) || typeid(target) == typeid(long) || typeid(target) == typeid(long long))
		{
			//�}�C�i�X�폜
			std::string strValue = targetStr;
			if(strValue.find("-") != std::string::npos)
			{
				strValue.erase(0, 1);
			}

			if( valueInt > 0 )
			{
				if(strValue.size() > valueInt)
				{

					//LogUtil::printError("�l:" + targetStr + "\t�t�H�[�}�b�g�G���[\t��������:" + std::to_string(strValue.size()) + ", �����ő包��:" + std::to_string(valueInt) + " �͈͊O�ł�");
					LogUtil::printError(5, "�ő包���G���[(����)", uh::toStr(name), "�l:" + targetStr +"\t���͌���:" + std::to_string(strValue.size()) + "\t�ő包��:" + std::to_string(valueInt));
					return false;
				}
			}

			//������
			if( valueDec > 0 )
			{
				//LogUtil::printWarning("�����f�[�^�^�̂��߁A�������̃`�F�b�N�͏ȗ����܂�");
				LogUtil::printWarning(5, uh::toStr(name) + "\t�����f�[�^�^�̂��߁A�������̃`�F�b�N�͏ȗ����܂�");
			}
			return true;
		}
		//����
		else if(typeid(target) == typeid(double))
		{
			//�}�C�i�X�폜
			std::string strValue = targetStr;
			if(strValue.find("-") != std::string::npos)
			{
				strValue.erase(0, 1);
			}

			size_t length = strValue.size();
			std::string::size_type index = strValue.find( "." );
			if( index == std::string::npos ) 
			{
				LogUtil::printError(5, "�ő包���G���[(������)", uh::toStr(name), "�l:" + targetStr +"\t���͌���:" + std::to_string(0) + "\t�ő包��:" + std::to_string(valueDec));
				//return true;
				return false;
			}
			bool ret = true;
			//������
			if( index> valueInt )
			{
				//LogUtil::printError("�l:" + targetStr + "\t�t�H�[�}�b�g�G���[\t��������:" + std::to_string(index) + ", �����ő包��:" + std::to_string(valueInt) + " �͈͊O�ł�");
				LogUtil::printError(5, "�ő包���G���[(������)", uh::toStr(name), "�l:" + targetStr +"\t���͌���:" + std::to_string(index) + "\t�ő包��:" + std::to_string(valueInt));
				ret &= false;
			}
			//������
			if( (length - index - 1) != valueDec )
			{
				//LogUtil::printError("�l:" + targetStr + "\t�t�H�[�}�b�g�G���[\t��������:" + std::to_string(length - index - 1) + ", �����ő包��:" + std::to_string(valueDec) + " �͈͊O�ł�");
				LogUtil::printError(5, "�ő包���G���[(������)", uh::toStr(name), "�l:" + targetStr +"\t���͌���:" + std::to_string(length - index - 1) + "\t�ő包��:" + std::to_string(valueDec));
				ret &= false;
			}

			return ret;
		}
		return true;
	}

	/**
	 * �������`�F�b�N
	 *
	 * @param[in]	target	�`�F�b�N�Ώےl
	 * @param[in]	name	�t�B�[���h��(���O�p)
	 *
	 * @retval	true	OK(�t�H�[�}�b�g����)
	 * @retval	false	NG
	 */
	static bool CheckIntegrity(const T& target, const uh::tstring& name)
	{
		return true;
	}
};

//Point���ꉻ
template<>
static bool FieldChecker<Point>::CheckMin(const Point& target, const Point& value, const uh::tstring& name)
{
	//�`�F�b�N�Ȃ�
	return true;
}
template<>
static bool FieldChecker<Point>::CheckMax(const Point& target, const Point& value, const uh::tstring& name)
{
	//�`�F�b�N�Ȃ�
	return true;
}
template<>
static bool FieldChecker<Point>::CheckValidValue(const Point& target, const std::vector<Point>& values, const uh::tstring& name)
{
	//�`�F�b�N�Ȃ�
	return true;
}
template<>
static bool FieldChecker<Point>::CheckLength(const Point& target, const std::string& targetStr, long valueInt, long valueDec, const uh::tstring& name)
{
	//�`�F�b�N�ς݂Ȃ̂�SKIP
	return true;
}
template<>
static bool FieldChecker<Point>::CheckIntegrity(const Point& target, const uh::tstring& name)
{
	return target.CheckIntegrity();	
}

//Polyline���ꉻ
template<>
static bool FieldChecker<Polyline>::CheckMin(const Polyline& target, const Polyline& value, const uh::tstring& name)
{
	//�`�F�b�N�Ȃ�
	return true;
}
template<>
static bool FieldChecker<Polyline>::CheckMax(const Polyline& target, const Polyline& value, const uh::tstring& name)
{
	//�`�F�b�N�Ȃ�
	return true;
}
template<>
static bool FieldChecker<Polyline>::CheckValidValue(const Polyline& target, const std::vector<Polyline>& values, const uh::tstring& name)
{
	//�`�F�b�N�Ȃ�
	return true;
}
template<>
static bool FieldChecker<Polyline>::CheckLength(const Polyline& target, const std::string& targetStr, long valueInt, long valueDec, const uh::tstring& name)
{
	//�`�F�b�N�ς݂Ȃ̂�SKIP
	return true;
}
template<>
static bool FieldChecker<Polyline>::CheckIntegrity(const Polyline& target, const uh::tstring& name)
{
	return target.CheckIntegrity();
}

//Polygon���ꉻ
template<>
static bool FieldChecker<Polygon>::CheckMin(const Polygon& target, const Polygon& value, const uh::tstring& name)
{
	//�`�F�b�N�Ȃ�
	return true;
}
template<>
static bool FieldChecker<Polygon>::CheckMax(const Polygon& target, const Polygon& value, const uh::tstring& name)
{
	//�`�F�b�N�Ȃ�
	return true;
}
template<>
static bool FieldChecker<Polygon>::CheckValidValue(const Polygon& target, const std::vector<Polygon>& values, const uh::tstring& name)
{
	//�`�F�b�N�Ȃ�
	return true;
}
template<>
static bool FieldChecker<Polygon>::CheckLength(const Polygon& target, const std::string& targetStr, long valueInt, long valueDec, const uh::tstring& name)
{
	//�`�F�b�N�ς݂Ȃ̂�SKIP
	return true;
}
template<>
static bool FieldChecker<Polygon>::CheckIntegrity(const Polygon& target, const uh::tstring& name)
{
	return target.CheckIntegrity();

}

};	//namespace adam_v2