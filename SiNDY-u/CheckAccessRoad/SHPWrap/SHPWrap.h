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

/*
** SHPWrap.h shapelib ラップクラス
**   Programmed by Takashi Togawa
**   EDITION: 2001/10/01 作成開始
**            2002/02/05 NO_DBF_... の追加(対Windowsライブラリ用)
**
*/

/*! @file
  shapelib wrapper class
  @author Takashi Togawa
  @date   2002/01/18
*/

#ifndef ___SHP_WRAP_H___
#define ___SHP_WRAP_H___

/*
** インクルードファイル
*/

#include <shapefil.h>
#include <string>
#include <vector>
#include "TSmartPointer.h" // スマートポインタ
#include "TOwnerPointer.h" // 所有権付きポインタ

#if defined(WIN32) && !defined(USE_NEW_SHAPELIB)
#define NO_DBF_GET_FIELD_INDEX
#define NO_DBF_IS_ATTRIBUTE_NULL
#define NO_DBF_WRITE_NULL_ATTRIBUTE
#define NO_DBF_GET_NATIVE_FIELD_TYPE
#endif /* WIN32 && USE_NEW_SHAPELIB */

class CSHPObj;
class CSHPHdl;
class CSHPTree;
class CDBFHdl;

#ifndef ___SWSTRING___
/*!
  文字列クラス
  NAMESPACE_STDstring を typedef したダケです。
*/
typedef std::string swstring;

#define ___SWSTRING___
#endif /* ___SWSTRING ___ */

/*!
  座標構造体
  CSHP関係の座標のIN/OUTは、この構造体を使用して行なう。
*/

struct CSHPPoint
{
	double eX; //!< X座標
	double eY; //!< Y座標
	double eZ; //!< Z座標
	double eM; //!< 寸法

public:
	//! デフォルトコンストラクタ
	CSHPPoint() {}
	//! 値指定コンストラクタ
	/*!
	  @param cX [in] X座標
	  @param cY [in] Y座標
	  @param cZ [in] Z座標(省略時は0.0)
	  @param cM [in] 寸法(省略時は0.0)
	*/
	CSHPPoint(double cX, double cY, double cZ = 0.0, double cM = 0.0) :
		eX(cX), eY(cY), eZ(cZ), eM(cM) {}
	//! コピーコンストラクタ
	CSHPPoint(const CSHPPoint& cPoint) :
		eX(cPoint.eX), eY(cPoint.eY), eZ(cPoint.eZ), eM(cPoint.eM) {}
	//! デストラクタ
	~CSHPPoint() {} // なにもしない
	// = 演算子
	CSHPPoint& operator = (const CSHPPoint& cPoint)
		{ eX = cPoint.eX; eY = cPoint.eY; eZ = cPoint.eZ; eM = cPoint.eM; return(*this); }
	// 値の設定
	/*!
	  @param cX [in] X座標
	  @param cY [in] Y座標
	  @param cZ [in] Z座標(省略時は0.0)
	  @param cM [in] 寸法(省略時は0.0)
	*/
	void setVert(double cX, double cY, double cZ = 0.0, double cM = 0.0)
		{ eX = cX; eY = cY; eZ = cZ; eM = cM; }
};

//! CSHPointのスマートポインタ。
typedef TSmartPointer<CSHPPoint> CSHPPointPtr;

/*!
  座標構造体配列
  点列を表す構造体。CSHP関係の点列のIN/OUTは、この構造体を使用して行う。
  vectorを継承しているので、operator[](int i), size() 等はそのままつかえます。
*/

struct CSHPPointArray : public std::vector<CSHPPoint>
{
	//! デフォルトコンストラクタ
	CSHPPointArray();
	//! 値指定コンストラクタ
	/*!
	  @param cSize [in] 座標数
	  @param cXArray [in] X座標配列
	  @param cYArray [in] Y座標配列
	  @param cZArray [in] Z座標配列(NULLで省略可能)
	  @param cMArray [in] 寸法配列(NULLで省略可能)
	*/
	explicit CSHPPointArray(int cSize, const double cXArray[], const double cYArray[], const double cZArray[] = NULL, const double cMArray[] = NULL);
	//! コピーコンストラクタ
	CSHPPointArray(const CSHPPointArray& cArray);
	//! デストラクタ
	~CSHPPointArray();
	//! = 演算子
	CSHPPointArray& operator = (const CSHPPointArray& cArray);
	//! 値の追加
	/*!
	  @param cX [in] X座標
	  @param cY [in] Y座標
	  @param cZ [in] Z座標(省略時は0.0)
	  @param cM [in] 寸法(省略時は0.0)
	*/
	void append(double cX, double cY, double cZ = 0.0, double cM = 0.0);
	//! 配列の追加
	/*!
	  @param cPointArray [in] 追加する配列
	*/
	void append(const CSHPPointArray& cPointArray);
	//! 配列の作成
	/*!
	  基本的にこのメソッドは、shapelibとの互換のために存在します。
	  点列の要素にアクセスしたい場合は、opeartor [] (int i); を使用してください。
	  @parma cSize [out] 配列のサイズ
	  @param cXArray [out] X座標配列(この値はfree()しなければならない)
	  @param cYArray [out] Y座標配列(この値はfree()しなければならない)
	  @param cZArray [out] Z座標配列(この値はfree()しなければならない)
	  @param cMArray [out] 寸法配列(この値はfree()しなければならない)
	  @return 配列の要素数
	*/
	int output(int* cSize, double** cXArray, double** cYArray, double** cZArray, double** cMArray) const;
	//! 配列からの取り込み
	/*!
	  @param cSize [in] 座標数
	  @param cXArray [in] X座標配列
	  @param cYArray [in] Y座標配列
	  @param cZArray [in] Z座標配列(NULLで省略可能)
	  @param cMArray [in] 寸法配列(NULLで省略可能)
	*/
	void input(int cSize, const double cXArray[], const double cYArray[], const double cZArray[] = NULL, const double cMArray[] = NULL);
	//! 配列のクリア
	void clear();

	//! 点列の向きを逆にする
	void reverse();
};

/*!
  CSHPPointArrayのスマートポインタ
  CSHPPointArrayの参照カウント付きポインタ。
  CSHPPointArrayArrayで使用している。
*/
typedef TSmartPointer<CSHPPointArray> CSHPPointArrayPtr;

/*!
  点列(CSHPPointArray)の配列
  点列構造体の配列クラス。
  複数の点列をまとめて扱う場合に使用する。
*/
struct CSHPPointArrayArray : public std::vector<CSHPPointArrayPtr>
{
	//! デフォルトコンストラクタ
	CSHPPointArrayArray();
	//! デストラクタ
	~CSHPPointArrayArray();

	//! 値の追加
	/*!
		@param CSHPPointArrayのスマートポインタ
		@return なし
	*/
	void append(CSHPPointArrayPtr cPointArray);
	/*!
		@param CSHPPointArrayのポインタ\n
		スマートポインタからポインタを取得してこのメソッドを使用するのは不可。
		基本的にnewの文字列だけの使用。
		@return なし
	*/
	void append(CSHPPointArray* cPointArray);

	//! 配列のクリア
	void clear();
};

/*!
  パート構造体
  1つの点列(CSHPPointArray)を、複数のパートに分る場合に使用する構造体。
*/

struct CSHPPart
{
	int eStart; //!< パート開始インデックス
	int eType;  //!< パートタイプ

public:
	//! デフォルトコンストラクタ
	CSHPPart() {}
	//! 値指定コンストラクタ１
	/*!
		@param cStart [in] 開始インデックス
		@return なし
	*/
	CSHPPart(int cStart) :
		eStart(cStart), eType(SHPP_RING) {}
	//! 値指定コンストラクタ２
	/*!
		@param cStart [in] パート開始インデックス
		@param cType [in] パートタイプ指定
		@return なし
	*/
	CSHPPart(int cStart, int cType) :
		eStart(cStart), eType(cType) {}
	//! コピーコンストラクタ
	CSHPPart(const CSHPPart& cPart) :
		eStart(cPart.eStart), eType(cPart.eType) {}
	//! デストラクタ
	~CSHPPart() {}
	//! = 演算子
	CSHPPart& operator = (const CSHPPart& cPart)
		{ eStart = cPart.eStart; eType = cPart.eType; return(*this); }
	//! 値の設定１
	/*!
	  @param cStart [in] パート開始インデックス
	  @return なし
	*/
	void setPart(int cStart)
		{ eStart = cStart; eType = SHPP_RING; }
	//! 値の設定２
	/*!
	  @param cStart [in] パート開始インデックス
	  @param cType [in] パートタイプ指定
	  @return なし
	*/
	void setPart(int cStart, int cType)
		{ eStart = cStart; eType = cType; }
};

/*!
  CSHPPartのスマートポインタ。
  CSHPPartの参照カウント付きポインタ。特に使用していない。
*/
typedef TSmartPointer<CSHPPart> CSHPPartPtr;

/*!
  パート構造体配列
  CSHPPartの配列クラス。複数のCSHPPartをまとめて扱う場合に使用する。
  vectorを継承しているので、operator[](int i), size() 等はそのままつかえます。
*/

struct CSHPPartArray : public std::vector<CSHPPart>
{
public:
	//! デフォルトコンストラクタ
	CSHPPartArray();
	//! 値指定コンストラクタ
	/*!
	  @param cSize [in] パート数
	  @param cStart [in] パート開始インデックス配列
	  @param cType [in] パートタイプ配列
	*/
	CSHPPartArray(int cSize, const int cStart[], const int cType[]);
	//! コピーコンストラクタ
	CSHPPartArray(const CSHPPartArray& cArray);
	//! デストラクタ
	~CSHPPartArray();
	//! = 演算子
	CSHPPartArray& operator = (const CSHPPartArray& cArray);
	//! 値の追加１
	/*!
	  @param cStart [in] 開始インデックス
	  @return なし
	*/
	void append(int cStart);
	//! 値の追加２
	/*!
	  @param cStart [in] 開始インデックス
	  @param cType [in] パートタイプ指定
	  @return なし
	*/
	void append(int cStart, int cType);
	//! 配列の作成
	/*!
	  @param cSize [out] 配列サイズ
	  @param cSArray [out] 開始インデックス配列(free()が必要)
	  @param cTArray [out] パートタイプ配列(free()が必要)
	  @return 配列の要素数
	*/
	int output(int* cSize, int** cSArray, int** cTArray) const;
	//! 配列から読み込み
	/*!
	  @param cSize [in] 配列の要素数
	  @param cSArray [in] 開始インデックス配列
	  @param cTArray [in] パートタイプ配列
	*/
	void input(int cSize, const int cSArray[], const int cTArray[]);
	//! 全要素クリア
	void clear();
};

/*!
  CSHPPartArrayのスマートポインタ。
  CSHPPartArrayの参照カウント付きポインタ。特に使用していない。
*/

typedef TSmartPointer<CSHPPartArray> CSHPPartArrayPtr;

/*!
  _shp_object_delete
  SHPObject の所有権付きポインタ用deleteファンクションオブジェクト
*/

struct _shp_object_delete : std::unary_function<SHPObject, void>
{
	void operator () (SHPObject* cPointer)
		{ if(cPointer) SHPDestroyObject(cPointer); }
};

/*!
  SHPObject の所有権つきポインタ
  CSPObjで使用する所有権付きポインタ。ユーザは使用しない。
*/

typedef TOwnerPointer<SHPObject, bool, _shp_object_delete> SHPObjectPtr;

/*!
  struct SHPObject ラッパークラス
  shapeのオブジェクトを管理するクラス
*/

class CSHPObj
{
	SHPObjectPtr mObject; //!< SHPオブジェクト

private:
	//! SHPObjectの取得
	/*!
	  @return SHPObjectのポインタ
	*/
	SHPObjectPtr getObject();

	friend class CSHPHdl;

protected:
	//! コピーコンストラクタ(禁止)
	CSHPObj(const CSHPObj&);
	//! 代入演算子(禁止)
	CSHPObj& operator = (const CSHPObj&);

public:
	//! デフォルトコンストラクタ
	/*!
	  NULLオブジェクトの作成
	 */
	CSHPObj();
	//! 値指定コンストラクタ
	/*!
	  @param cSHPType [in] SHPタイプ
	  @param cShapeID [in] シェープID
	  @param cPartArray [in] パート配列
	  @param cPointArray [in] 座標配列
	*/
	CSHPObj(int cSHPType, int cShapeID, const CSHPPartArray& cPartArray, const CSHPPointArray& cPointArray);
	//! 値指定コンストラクタ(自動マルチパート作成)
	/*!
		@param cSHPType [in] SHPタイプ
		@param cPointArrayArray [in] 点列の配列
	*/
	CSHPObj(int cSHPType, const CSHPPointArrayArray& cPointArrayArray);
	//! 値指定コンストラクタ
	/*!
	  @param cSHPType [in] SHPタイプ
	  @param cPointArray [in] 座標配列
	  ここでは、座標配列の寸法(eM)は無視される。
	*/
	CSHPObj(int cSHPType, const CSHPPointArray& cPointArray);
	//! 値指定コンストラクタ(ポイントから作成)
	/*!
	  @param cSHPType [in] SHPタイプ
	  @param cPoint [in] 座標
	 */
	CSHPObj(int cSHPType, const CSHPPoint& cPoint);
	//! SHPObject* から作成
	/*!
	  CSHPHdl用のコンストラクタ。通常は使用しない。
	  所有権を指定できる。
	  @param cObject [in] SHPObjectのポインタ
	 */
	CSHPObj(SHPObject* cObject);
	//! デストラクタ
	~CSHPObj();
	//! voidポインタへのキャスト
	// operator void* () { return((void*)mObject); }
	//! オブジェクトの作成
	/*!
	  @param cSHPType [in] SHPタイプ
	  @param cShapeID [in] シェープID
	  @param cPartArray [in] パート配列
	  @param cPointArray [in] 座標配列
	  @return true=成功, false=失敗
	*/
	bool create(int cSHPType, int cShapeID, const CSHPPartArray& cPartArray, const CSHPPointArray& cPointArray);
	//! オブジェクトの作成(点列の配列から作成)
	/*!
	  @param cSHPType [in] SHPタイプ
	  @param cPointArrayArray [in] 点列の配列
	  @return true=成功, false=失敗
	*/
	bool create(int cSHPType, const CSHPPointArrayArray& cPointArrayArray);
	//! オブジェクトの作成(点列から作成)
	/*!
	  @param cSHPType [in] SHPタイプ
	  @param cPointArray [in] 座標配列
	  ここでは、座標配列の寸法(eM)は無視される。
	  @return true=成功, false=失敗
	*/
	bool create(int cSHPType, const CSHPPointArray& cPointArray);
	//! オブジェクトの作成(ポイントから作成)
	/*!
	  @param cSHPType [in] SHPタイプ
	  @param cPoint [in] 座標
	  @return true=成功, false=失敗
	*/
	bool create(int cSHPType, const CSHPPoint& cPoint);
	//! オブジェクトの削除
	/*!
	  所有権のある場合のみオブジェクトを削除する。
	  所有権がない場合は、ポインタをNULLにするだけ。
	*/
	void clear();
	//! ポイント列の取得
	/*!
	  @param cPointArray [out] ポイント配列
	  @return true=成功, false=失敗
	*/
	bool getPointArray(CSHPPointArray& cPointArray);
	//! パート列の取得
	/*!
	  @param cPartArray [out] パート配列
	  @return true=成功, false=失敗
	*/
	bool getPartArray(CSHPPartArray& cPartArray);
};

/*!
  _shp_handle_delete、
  SHPHandle スマートポインタ用deleteファンクションオブジェクト
*/

struct _shp_handle_delete : std::unary_function<SHPInfo, void>
{
	void operator () (SHPInfo* cPointer) const
		{ if(cPointer) SHPClose(cPointer); }
};

/*!
  SHPHandleのスマートポインタ。
  ユーザが使用することはない。
*/

typedef TSmartPointer<SHPInfo, int, _shp_handle_delete> SHPHandlePtr;

/*!
  struct SHPHandle ラッパークラス
  shapeのファイルハンドルクラス。
*/

class CSHPHdl
{
	SHPHandlePtr mHandle; //!< SHPハンドルのスマートポインタ

private:
	//! ハンドルの取得
	/*!
	  @return SHPHandleのポインタ
	*/
	SHPHandlePtr getHandle();

	friend class CSHPTree;

protected:
	//! コピーコンストラクタ(禁止)
	CSHPHdl(const CSHPHdl&);
	//! 代入演算子(禁止)
	CSHPHdl& operator = (const CSHPHdl&);

public:
	//! デフォルトコンストラクタ
	CSHPHdl();
	//! ファイルをオープンするコンストラクタ
	/*!
	  @param cShapeFile [in] ファイル名
	  @param cAccess [in] アクセス指定
	  fopen()等と同じ値を指定する、"r"=読み込み, "w"=書き込み, "rw"=読み書き
	*/
	CSHPHdl(const swstring& cShapeFile, const swstring& cAccess);
	//! 新規作成コンストラクタ
	/*!
	  @param cShapeFile [in] ファイル名
	  @param cShapeType [in] シェープタイプ
	*/
	CSHPHdl(const swstring& cShapeFlie, int cShapeType);
	//! ハンドル指定コンストラクタ
	/*!
	  通常は使用しない。SHPHandleポインタを直接渡すコンストラクタ
	  @param cHandle [in] SHPHandleポインタ
	*/
	CSHPHdl(SHPHandle cHandle);
	//! デストラクタ
	~CSHPHdl();

	//! ハンドルオープン
	/*!
	  @param cShapeFile [in] ファイル名
	  @param cAccess [in] アクセス指定
	  fopen()等と同じ値を指定する、"r"=読み込み, "w"=書き込み, "rw"=読み書き
	  @return true=成功, false=失敗
	*/
	bool open(const swstring& cShapeFile, const swstring& cAccess);

	//! ハンドルの作成
	/*!
	  @param cShapeFile [in] ファイル名
	  @param cShapeType [in] シェープタイプ
	  @return true=成功, false=失敗
	 */
	bool create(const swstring& cShapeFile, int cShapeType);

	//! ハンドル情報の取得
	/*!
	  @param cEntities [out] レコード数
	  @param cShapeType [out] シェープタイプ
	  @param cMinBound [out] double[4]を渡す(緯度が４つ返ってくる)
	  @param cMaxBound [out] double[4]を渡す(経度が４つ返ってくる)
	  @return なし
	*/
	void getInfo(int* cEntities, int* cShapeType, double* cMinBound, double* cMaxBound);

	//! オブジェクトの読み込み
	/*!
	  @param cObj [out] 取得したオブジェクト
	  @param cShape [in] レコードインデックス
	  @return なし
	*/
	// shapelibでコピーを作成してかえしているようなので
	// ownerフラグは真のまま
	void readObject(CSHPObj& cObj, int cShape);

	//! オブジェクトの書き込み
	/*!
	  @param cObj [in] 書き込むオブジェクト
	  @param cShape [in] レコードインデックス
	  @return 成功時は、レコードインデックス
	  失敗時は -1 を返す
	*/
	// これもownerフラグは真のまま
	// 結局ownerフラグを変更する必要はなくなった。
	int writeObject(CSHPObj& cObj, int cShape);

	//! ハンドルのクローズ
	void close();

	/*
	** static関数
	*/

	//! SHPTypeNameの取得
	/*!
	  @param cSHPType [in] SHPType
	  @return SHPTypeの名称文字列
	*/
	static swstring GetTypeName(int cSHPType);
	
	//! SHPPartTypeNameの取得
	/*!
	  @param cSHPPartType [in] SHPPartType
	  @return SHPPartTypeの名称文字列
	*/
	static swstring GetPartTypeName(int cSHPPartType);
};

/*!
  _shp_tree_delete、
  SHPTreeスマートポインタ用deleteファンクションオブジェクト
*/

struct _shp_tree_delete : std::unary_function<SHPTree, void>
{
	void operator () (SHPTree* cPointer)
		{ if(cPointer) SHPDestroyTree(cPointer); }
};

/*!
  SHPTreePtr SHPTreeのスマートポインタ
*/

// typedef TSmartPointer<SHPTree, int, _shp_tree_delete> SHPTreePtr;

/*!
  CSHPTree
  SHPTreeラッパークラス
*/

// class CSHPTree
// {
// 	SHPHandlePtr mHandle; //!< SHPHandleスマートポインタ
// 	SHPTreePtr   mTree;   //!< SHPTreeスマートポインタ

// protected:
// 	//! コピーコンストラクタ(禁止)
// 	CSHPTree(const CSHPTree&);
// 	//! = 演算子(禁止)
// 	CSHPTree& operator = (const CSHPTree&);

// public:
// 	//! デフォルトコンストラクタ
// 	CSHPTree();
// 	//! ハンドル指定コンストラクタ
// 	/*!
// 	  @param cHdl [in] ハンドルクラス
// 	  @param cDimension [in] ？
// 	  @param cMaxDepth [in] ？
//	  @param cMinBound [in] double[4]を渡す(緯度が４つ)
//	  @param cMaxBound [in] double[4]を渡す(経度が４つ)
// 	*/
// 	CSHPTree(CSHPHdl& cHdl, int cDimension, int cMaxDepth, double* cMinBound, double* cMaxBound);
// 	//! デストラクタ
// 	~CSHPTree();

// 	//! 全データのクリア(Treeの破壊)
// 	void clear();
// 	//! Treeの新規作成
// 	/*!
// 	  @param cHdl [in] ハンドルクラス
// 	  @param cDimention [in] ？
// 	  @param cMaxDepth [in] ？
//	  @param cMinBound [in] double[4]を渡す(緯度が４つ)
//	  @param cMaxBound [in] double[4]を渡す(経度が４つ)
// 	  @return true=成功, false=失敗
// 	*/
// 	bool create(CSHPHdl& cHdl, int cDimension, int cMaxDepth, double* cMinBound, double* cMaxBound);

// 	//! Treeの書き出し
// 	/*!
// 	  @param cFilename [in] 出力ファイル名
// 	  @return ture=成功, false=失敗
// 	*/
// 	bool write(const swstring& cFilename);
// 	//! Treeの読み込み
// 	/*!
// 	  @param cFilename [in] 入力ファイル名
// 	  @return ture=成功, false=失敗
// 	*/
// 	bool read(const swstring& cswstring);

// 	//! オブジェクトの追加１
// 	/*!
// 	  @param cObj [in] 追加するオブジェクト
// 	  追加されるオブジェクトは所有権を失う
// 	  @return true=成功, false=失敗
// 	*/
// 	bool addObject(CSHPObj& cObj);
// 	//! オブジェクトの追加２
// 	/*!
// 	  @param cObj [in] 追加するオブジェクト。
// 	  追加されるとオブジェクトは所有権を失う。
// 	  @return true=成功, false=失敗
// 	*/
// 	bool addShapeID(CSHPObj& cObj);

// 	//! オブジェクトの削除
// 	/*!
// 	  @param cShapeID
// 	  @return true=成功, false=成功
// 	*/
// 	bool removeShapeID(int cShapeID);

// 	//! ノードの整列？
// 	/*!
// 	  @return なし
// 	*/
// 	bool trimExtraNodes();
// };

/*!
 _dbf_info_delete
 DBFInfoスマートポインタ用delete関数オブジェクト
*/

struct _dbf_info_delete : std::unary_function<DBFInfo, void>
{
	void operator () (DBFInfo* cPointer)
		{ if(cPointer) DBFClose(cPointer); }
};

/*!
  DBFInfoPtr DBFInfoスマートポインタ
*/

typedef TSmartPointer<DBFInfo, int, _dbf_info_delete> DBFInfoPtr;

/*!
  DBFInfo ラッパークラス
  shapeの属性ファイルのハンドルクラス。
*/

class CDBFHdl
{
	DBFInfoPtr mHandle;

protected:
	//! コピーコンストラクタ(禁止)
	CDBFHdl(const CDBFHdl&);
	//! =演算子(禁止)
	CDBFHdl& operator = (const CDBFHdl& cHdl);

	// DBFHanlde指定コンストラクタ
	CDBFHdl(DBFHandle cHandle);

public:
	//! デフォルトコンストラクタ
	CDBFHdl();
	//! 作成コンストラクタ
	/*!
	  @param cFilename [in] ファイル名の指定
	*/
	CDBFHdl(const swstring& cFilename);
	//! ファイルオープンコンストラクタ
	/*!
	  @param cFilename [in] ファイル名の指定
	  @param cAccess [in] アクセス指定
	  fopen()等と同じ値を指定する、"r"=読み込み, "w"=書き込み, "rw"=読み書き
	*/
	CDBFHdl(const swstring& cFilename, const swstring& cAccess);

	//! デストラクタ
	~CDBFHdl();

	//! 作成
	/*!
	  @param cFilename [in] ファイル名
	  @return true=成功, false=失敗
	*/
	bool create(const swstring& cFilename);
	//! オープン
	/*!
	  @param cFilename [in] ファイル名
	  @param cAccess [in] アクセス指定
	  fopen()等と同じ値を指定する、"r"=読み込み, "w"=書き込み, "rw"=読み書き
	  @return true=成功, false=失敗
	*/
	bool open(const swstring& cFilename, const swstring& cAccess);
	//! クローズ
	void close();
	//! Field数の取得
	/*!
	  @return フィールド数
	*/
	int getFieldCount();
	//! レコード数の取得
	/*!
	  @return レコード数
	*/
	int getRecordCount();
	//! フィールドの追加
	/*!
	  @param cFieldName [in] フィールド名
	  @param cFieldType [in] フィールドタイプ
	  @param cWidth [in] フィールドの幅
	  @param cDecimals [in] フィールドの精度
	  @return 作成したフィールドのインデックス,-1の場合はエラー
	*/
	int addField(const swstring& cFieldName, DBFFieldType cFieldType, int cWidth, int cDecimals);
	//! フィールド情報の取得
	/*!
	  @param cField [in] フィールド
	  @param cFieldName [out] フィールド名
	  @param cWidth [out] フィールドの幅
	  @param cDecimals [out] 実数の精度
	  @return フィールドタイプ
	*/
	DBFFieldType getFieldInfo(int cField, swstring* cFieldName = NULL, int* cWidth = NULL, int* cDecimals = NULL);

	//! フィールドインデックスの取得
	/*!
	  @param cFieldName [in] フィールド名
	  @return フィールドインデックス
	*/
#ifndef NO_DBF_GET_FIELD_INDEX
	int getFieldIndex(const swstring& cFieldName);
#endif /* NO_DBF_GET_FIELD_INDEX */

	//! 整数属性の取得
	/*!
	  @param cShape [in] shape指定
	  @param cField [in] フィールド指定
	  @return 整数属性
	*/
	int readIntegerAttribute(int cShape, int cFiled);

	//! 実数属性の取得
	/*!
	  @param cShape [in] shape指定
	  @param cField [in] フィールド指定
	  @return 実数属性
	*/
	double readDoubleAttribute(int cShape, int cFiled);

	//! 文字列属性の取得
	/*!
	  @param cShape [in] shape指定
	  @param cField [in] フィールド指定
	  @return 文字列属性
	*/
	swstring readStringAttribute(int cShape, int cField);

	//! 属性のNULLチェック
	/*!
	  @param cShape [in] shape指定
	  @prama cField [in] フィールド指定
	  @param 0以外=NULL
	*/
#ifndef NO_DBF_IS_ATTRIBUTE_NULL
	int isAttributeNULL(int cShape, int cField);
#endif /* NO_DBF_IS_ATTRIBUTE_NULL */

	//! 整数属性の書き込み
	/*!
	  @param cShape [in] shape指定
	  @param cField [in] フィールド指定
	  @param cFieldValue [in] 属性値
	  @return 0以外 = 成功
	*/
	int writeIntegerAttribute(int cShape, int cField, int cFieldValue);

	//! 実数属性の書き込み
	/*!
	  @param cShape [in] shape指定
	  @param cField [in] フィールド指定
	  @param cFieldValue [in] 属性値
	  @return 0以外 = 成功
	*/
	int writeDoubleAttribute(int cShape, int cField, const double& cFieldValue);

	//! 文字列属性の書き込み
	/*!
	  @param cShape [in] shape指定
	  @param cField [in] フィールド指定
	  @param cFieldValue [in] 属性値
	  @return 0以外 = 成功
	*/
	int writeStringAttribute(int cShape, int cField, const swstring& cFieldValue);

	//! NULL属性の書き込み
	/*!
	  @param cShape [in] shape指定
	  @param cField [in] フィールド指定
	  @return 0以外 = 成功
	*/
#ifndef NO_DBF_WRITE_NULL_ATTRIBUTE
	int writeNULLAttribute(int cShape, int cField);
#endif /* NO_DBF_WRITE_NULL_ATTRIBUTE */

	//! Tupleの読み込み
	/*!
	  @param cEntry [in] エントリ
	  @return ？
	*/
	swstring readTuple(int cEntry);

	//! Tupleの書き込み
	/*!
	  @param cEntry [in] エントリ
	  @param cRawTuple [in] ？
	  @return 0以外 = 成功
	*/
	int writeTuple(int cEntry, void* cRawTuple);

	//! クローンの作成
	/*!
	  @param cFilename [in] ファイル名
	  @return CDBFHdl 作成したハンドル
	*/
	CDBFHdl* cloneEmpty(const swstring& cFilename);

	//! native field typeの取得
	/*!
	  @param cField [in] フィールド指定
	  @return フィールド
	*/
#ifndef NO_DBF_GET_NATIVE_FIELD_TYPE
	char getNativeFieldType(int cField);
#endif /* NO_DBF_GET_NATIVE_FIELD_TYPE */
};

#endif /* ___SHP_WRAP_H___ */
