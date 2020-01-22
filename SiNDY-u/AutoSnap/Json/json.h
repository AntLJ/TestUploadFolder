/**
 * @file json.h
 * @brief JSON Object library
 * @copyright Copyright 2015 Increment P Corp.
 * @author Gu
 * @date Jan. 25 2015
 * @version 0.1
 */
#ifndef JSON_H
#define JSON_H

#include <string>
#include <unordered_map>
#include <iostream>
#include <memory>

namespace json
{
/**
 * @brief Base JSON object
 * @details To raw support lex/yacc(flex/bison) as AST,
 * All strings are defined as std::string (multiple bytes),
 * Because the lex does not support UNICODE (Support UTF-8).
 *
 * All JSON objects can be identified via C++ typeid, 
 * So please reserve at least one virtual function for typeid to determine types by vtable.
 *
 * JSON supports only one numeric type : number,
 * Consider the use of C\C++, 
 * Number is defined as three types: "int" "double" and "long long"(64 bits).
 *
 * For performance, Used C++11 feature: unordered map.
 */
class JsonValue
{
public:
	/**
	 * @brief Print the object
	 * @param[in] os Output stream object
	 * @param[in] depth For pretty printing
	 */
	void Print(std::ostream &os, unsigned int depth = 0) const { print(os, depth);}
protected:
	JsonValue(){} //Can not be created directly for there being no real data.
	
	virtual void print(std::ostream &os, unsigned int depth) const = 0;

	//For pretty printing
	inline std::string indent(unsigned int d) const
    {
		return std::string(d, '\t');
    }
};
typedef std::shared_ptr<JsonValue> JsonValuePtr;

/// Boolean object
class JsonBool : public JsonValue
{
public:
	explicit JsonBool(bool b):_b(b) {}
	operator bool() const {return _b;}
private:
	virtual void print(std::ostream &os, unsigned int depth) const
	{
		_b? os << "true" : os << "false"; 
	}
private:
	bool _b;
};
typedef std::shared_ptr<JsonBool> JsonBoolPtr;

/// Null object
class JsonNull : public JsonValue
{
public:
	explicit JsonNull(){}
private:
	virtual void print(std::ostream &os, unsigned int depth) const
	{
		os << "null"; 
	}
};
typedef std::shared_ptr<JsonNull> JsonNullPtr;

/// String object
class JsonString : public JsonValue
{
public:
	explicit JsonString(const std::string& s):_s(s) {}
	operator std::string() const {return _s;}
private:
	virtual void print(std::ostream &os, unsigned int depth) const
	{
		os << '\"';
		//Convert escape chars
		for(auto it = _s.cbegin(); it!=_s.end(); ++it)
		{
			switch(*it)
			{
            case '\"':
				os << "\\\"";
				break;
            case '/':
				os << "\\/";
				break;
            case '\b':
				os << "\\b";
				break;
            case '\f':
				os << "\\f";
				break;
            case '\n':
				os << "\\n";
				break;
            case '\r':
				os << "\\r";
				break;
            case '\t':
				os << "\\t";
				break;
            case '\\':
				if((it+1)!= _s.cend() && *(it+1) == 'u')
				{
					os << "\\";
					break;
				}
				os << "\\\\";
				break;
            default:
				os << *it;
				break;
			}
		}
		os << '\"';
	}
private:
	std::string _s;
};
typedef std::shared_ptr<JsonString> JsonStringPtr;

/// Integer object
class JsonInteger : public JsonValue
{
public:
	explicit JsonInteger(int i):_i(i) {}
	operator int() const {return _i;}
private:
	virtual void print(std::ostream &os, unsigned int depth) const
	{
		os << _i;
	}
private:
	int _i;
};
typedef std::shared_ptr<JsonInteger> JsonIntegerPtr;

/// 64bits number object
class JsonLongInteger : public JsonValue
{
public:
	explicit JsonLongInteger(long long ll):_ll(ll) {}
	operator long long() const {return _ll;}
private:
	virtual void print(std::ostream &os, unsigned int depth) const
	{
		os << _ll;
	}
private:
	long long  _ll;
};
typedef std::shared_ptr<JsonInteger> JsonIntegerPtr;

/// Float object
class JsonDouble : public JsonValue
{
public:
	explicit JsonDouble(double d):_d(d) {}
	operator double() const {return _d;}
private:
	virtual void print(std::ostream &os, unsigned int depth) const
	{
		os << _d;
	}
private:
	double _d;
};
typedef std::shared_ptr<JsonDouble> JsonDoublePtr;

/// Object object
class JsonObject : public JsonValue
{
public:
	//For high speed , using unordered_map
	typedef std::unordered_map<std::string, JsonValuePtr> RawData;
	
	JsonObject(){}
	JsonObject(const std::string& str, JsonValuePtr v) { Add(str, v);}

	/**
	 * @brief Add a member into the object
	 * 
	 * @param[in] str key
	 * @param[in] v value
	 */
	void Add(const std::string& str, JsonValuePtr v) {_m[str] = v;}

	/**
	 * @brief Get the value of member by key
	 * 
	 * @param[in] str key name
	 * @return value
	 */
	const JsonValuePtr operator [](const std::string& str) const
	{
		RawData::const_iterator it = _m.find(str);
		if(it == _m.end()) return nullptr;
		return it->second;
	}

	/**
	 * Get raw data
	 *
	 * return Type
	 */
	operator const RawData&() const{return _m;}

	virtual ~JsonObject() { _m.clear(); }
private:
	virtual void print(std::ostream &os, unsigned int depth) const
	{
		//For simple implementation, put the colon before the member.
		os << "{" << std::endl;
		auto itb = _m.cbegin();
		for(auto it  = _m.cbegin(); it != _m.cend(); ++it)
		{
			os << indent(depth + 1);
			if( it!= itb) os << ",";
			os << "\"" << it->first << "\"";
			os << " : ";
			it->second->Print(os, depth + 1);
			os << std::endl;
		}
		os << indent(depth) << "}";
	}
private:
	RawData _m;
};
typedef std::shared_ptr<JsonObject> JsonObjectPtr;

/// Array object
class JsonArray : public JsonValue
{
public:
	typedef std::vector<JsonValuePtr> RawData;
	
	JsonArray(){}
	JsonArray(JsonValuePtr v){ Add(v); }

	/**
	 * @brief Add a element to the end of the array
	 * 
	 * @param[in] v value
	 */
	void Add(JsonValuePtr v) { _v.push_back(v); }

	/**
	 * @brief Add a element to the begin of the array
	 * 
	 * @param[in] v value
	 */

	void Insert(JsonValuePtr v) { _v.insert(_v.begin(), v); }

	/**
	 * @brief Get the value of element by index
	 * 
	 * @param[in] n Index number
	 * @return value
	 */
	const JsonValuePtr operator[](size_t n) const
	{
		if(n >= _v.size()) return nullptr;
		return _v[n];
	}

	/**
	 * @brief Get the count of elements
	 * 
	 * @return count
	 */
	size_t Size() const
	{
		return _v.size();
	}

	virtual ~JsonArray() { _v.clear(); }
	
	/**
	 * Get raw data
	 *
	 * return Type
	 */
	operator const RawData&() const {return _v;}
private:
	virtual void print(std::ostream &os, unsigned int depth) const
	{
		//For simple implementation, put the colon before the element.
		os << "[" << std::endl;
		auto itb = _v.cbegin();
		for(auto it = _v.cbegin(); it != _v.cend(); ++it)
		{
			os << indent(depth + 1);
			if(it != itb) os << ",";
			(*it)->Print(os, depth + 1);
			os << std::endl;
		}
		os << indent(depth) << "]";
	}
private:
	RawData _v;
};
typedef std::shared_ptr<JsonArray> JsonArrayPtr;

}	//namespace json

#endif