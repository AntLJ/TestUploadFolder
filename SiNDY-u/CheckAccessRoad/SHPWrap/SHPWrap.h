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
** SHPWrap.h shapelib ���b�v�N���X
**   Programmed by Takashi Togawa
**   EDITION: 2001/10/01 �쐬�J�n
**            2002/02/05 NO_DBF_... �̒ǉ�(��Windows���C�u�����p)
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
** �C���N���[�h�t�@�C��
*/

#include <shapefil.h>
#include <string>
#include <vector>
#include "TSmartPointer.h" // �X�}�[�g�|�C���^
#include "TOwnerPointer.h" // ���L���t���|�C���^

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
  ������N���X
  NAMESPACE_STDstring �� typedef �����_�P�ł��B
*/
typedef std::string swstring;

#define ___SWSTRING___
#endif /* ___SWSTRING ___ */

/*!
  ���W�\����
  CSHP�֌W�̍��W��IN/OUT�́A���̍\���̂��g�p���čs�Ȃ��B
*/

struct CSHPPoint
{
	double eX; //!< X���W
	double eY; //!< Y���W
	double eZ; //!< Z���W
	double eM; //!< ���@

public:
	//! �f�t�H���g�R���X�g���N�^
	CSHPPoint() {}
	//! �l�w��R���X�g���N�^
	/*!
	  @param cX [in] X���W
	  @param cY [in] Y���W
	  @param cZ [in] Z���W(�ȗ�����0.0)
	  @param cM [in] ���@(�ȗ�����0.0)
	*/
	CSHPPoint(double cX, double cY, double cZ = 0.0, double cM = 0.0) :
		eX(cX), eY(cY), eZ(cZ), eM(cM) {}
	//! �R�s�[�R���X�g���N�^
	CSHPPoint(const CSHPPoint& cPoint) :
		eX(cPoint.eX), eY(cPoint.eY), eZ(cPoint.eZ), eM(cPoint.eM) {}
	//! �f�X�g���N�^
	~CSHPPoint() {} // �Ȃɂ����Ȃ�
	// = ���Z�q
	CSHPPoint& operator = (const CSHPPoint& cPoint)
		{ eX = cPoint.eX; eY = cPoint.eY; eZ = cPoint.eZ; eM = cPoint.eM; return(*this); }
	// �l�̐ݒ�
	/*!
	  @param cX [in] X���W
	  @param cY [in] Y���W
	  @param cZ [in] Z���W(�ȗ�����0.0)
	  @param cM [in] ���@(�ȗ�����0.0)
	*/
	void setVert(double cX, double cY, double cZ = 0.0, double cM = 0.0)
		{ eX = cX; eY = cY; eZ = cZ; eM = cM; }
};

//! CSHPoint�̃X�}�[�g�|�C���^�B
typedef TSmartPointer<CSHPPoint> CSHPPointPtr;

/*!
  ���W�\���̔z��
  �_���\���\���́BCSHP�֌W�̓_���IN/OUT�́A���̍\���̂��g�p���čs���B
  vector���p�����Ă���̂ŁAoperator[](int i), size() ���͂��̂܂܂����܂��B
*/

struct CSHPPointArray : public std::vector<CSHPPoint>
{
	//! �f�t�H���g�R���X�g���N�^
	CSHPPointArray();
	//! �l�w��R���X�g���N�^
	/*!
	  @param cSize [in] ���W��
	  @param cXArray [in] X���W�z��
	  @param cYArray [in] Y���W�z��
	  @param cZArray [in] Z���W�z��(NULL�ŏȗ��\)
	  @param cMArray [in] ���@�z��(NULL�ŏȗ��\)
	*/
	explicit CSHPPointArray(int cSize, const double cXArray[], const double cYArray[], const double cZArray[] = NULL, const double cMArray[] = NULL);
	//! �R�s�[�R���X�g���N�^
	CSHPPointArray(const CSHPPointArray& cArray);
	//! �f�X�g���N�^
	~CSHPPointArray();
	//! = ���Z�q
	CSHPPointArray& operator = (const CSHPPointArray& cArray);
	//! �l�̒ǉ�
	/*!
	  @param cX [in] X���W
	  @param cY [in] Y���W
	  @param cZ [in] Z���W(�ȗ�����0.0)
	  @param cM [in] ���@(�ȗ�����0.0)
	*/
	void append(double cX, double cY, double cZ = 0.0, double cM = 0.0);
	//! �z��̒ǉ�
	/*!
	  @param cPointArray [in] �ǉ�����z��
	*/
	void append(const CSHPPointArray& cPointArray);
	//! �z��̍쐬
	/*!
	  ��{�I�ɂ��̃��\�b�h�́Ashapelib�Ƃ̌݊��̂��߂ɑ��݂��܂��B
	  �_��̗v�f�ɃA�N�Z�X�������ꍇ�́Aopeartor [] (int i); ���g�p���Ă��������B
	  @parma cSize [out] �z��̃T�C�Y
	  @param cXArray [out] X���W�z��(���̒l��free()���Ȃ���΂Ȃ�Ȃ�)
	  @param cYArray [out] Y���W�z��(���̒l��free()���Ȃ���΂Ȃ�Ȃ�)
	  @param cZArray [out] Z���W�z��(���̒l��free()���Ȃ���΂Ȃ�Ȃ�)
	  @param cMArray [out] ���@�z��(���̒l��free()���Ȃ���΂Ȃ�Ȃ�)
	  @return �z��̗v�f��
	*/
	int output(int* cSize, double** cXArray, double** cYArray, double** cZArray, double** cMArray) const;
	//! �z�񂩂�̎�荞��
	/*!
	  @param cSize [in] ���W��
	  @param cXArray [in] X���W�z��
	  @param cYArray [in] Y���W�z��
	  @param cZArray [in] Z���W�z��(NULL�ŏȗ��\)
	  @param cMArray [in] ���@�z��(NULL�ŏȗ��\)
	*/
	void input(int cSize, const double cXArray[], const double cYArray[], const double cZArray[] = NULL, const double cMArray[] = NULL);
	//! �z��̃N���A
	void clear();

	//! �_��̌������t�ɂ���
	void reverse();
};

/*!
  CSHPPointArray�̃X�}�[�g�|�C���^
  CSHPPointArray�̎Q�ƃJ�E���g�t���|�C���^�B
  CSHPPointArrayArray�Ŏg�p���Ă���B
*/
typedef TSmartPointer<CSHPPointArray> CSHPPointArrayPtr;

/*!
  �_��(CSHPPointArray)�̔z��
  �_��\���̂̔z��N���X�B
  �����̓_����܂Ƃ߂Ĉ����ꍇ�Ɏg�p����B
*/
struct CSHPPointArrayArray : public std::vector<CSHPPointArrayPtr>
{
	//! �f�t�H���g�R���X�g���N�^
	CSHPPointArrayArray();
	//! �f�X�g���N�^
	~CSHPPointArrayArray();

	//! �l�̒ǉ�
	/*!
		@param CSHPPointArray�̃X�}�[�g�|�C���^
		@return �Ȃ�
	*/
	void append(CSHPPointArrayPtr cPointArray);
	/*!
		@param CSHPPointArray�̃|�C���^\n
		�X�}�[�g�|�C���^����|�C���^���擾���Ă��̃��\�b�h���g�p����͕̂s�B
		��{�I��new�̕����񂾂��̎g�p�B
		@return �Ȃ�
	*/
	void append(CSHPPointArray* cPointArray);

	//! �z��̃N���A
	void clear();
};

/*!
  �p�[�g�\����
  1�̓_��(CSHPPointArray)���A�����̃p�[�g�ɕ���ꍇ�Ɏg�p����\���́B
*/

struct CSHPPart
{
	int eStart; //!< �p�[�g�J�n�C���f�b�N�X
	int eType;  //!< �p�[�g�^�C�v

public:
	//! �f�t�H���g�R���X�g���N�^
	CSHPPart() {}
	//! �l�w��R���X�g���N�^�P
	/*!
		@param cStart [in] �J�n�C���f�b�N�X
		@return �Ȃ�
	*/
	CSHPPart(int cStart) :
		eStart(cStart), eType(SHPP_RING) {}
	//! �l�w��R���X�g���N�^�Q
	/*!
		@param cStart [in] �p�[�g�J�n�C���f�b�N�X
		@param cType [in] �p�[�g�^�C�v�w��
		@return �Ȃ�
	*/
	CSHPPart(int cStart, int cType) :
		eStart(cStart), eType(cType) {}
	//! �R�s�[�R���X�g���N�^
	CSHPPart(const CSHPPart& cPart) :
		eStart(cPart.eStart), eType(cPart.eType) {}
	//! �f�X�g���N�^
	~CSHPPart() {}
	//! = ���Z�q
	CSHPPart& operator = (const CSHPPart& cPart)
		{ eStart = cPart.eStart; eType = cPart.eType; return(*this); }
	//! �l�̐ݒ�P
	/*!
	  @param cStart [in] �p�[�g�J�n�C���f�b�N�X
	  @return �Ȃ�
	*/
	void setPart(int cStart)
		{ eStart = cStart; eType = SHPP_RING; }
	//! �l�̐ݒ�Q
	/*!
	  @param cStart [in] �p�[�g�J�n�C���f�b�N�X
	  @param cType [in] �p�[�g�^�C�v�w��
	  @return �Ȃ�
	*/
	void setPart(int cStart, int cType)
		{ eStart = cStart; eType = cType; }
};

/*!
  CSHPPart�̃X�}�[�g�|�C���^�B
  CSHPPart�̎Q�ƃJ�E���g�t���|�C���^�B���Ɏg�p���Ă��Ȃ��B
*/
typedef TSmartPointer<CSHPPart> CSHPPartPtr;

/*!
  �p�[�g�\���̔z��
  CSHPPart�̔z��N���X�B������CSHPPart���܂Ƃ߂Ĉ����ꍇ�Ɏg�p����B
  vector���p�����Ă���̂ŁAoperator[](int i), size() ���͂��̂܂܂����܂��B
*/

struct CSHPPartArray : public std::vector<CSHPPart>
{
public:
	//! �f�t�H���g�R���X�g���N�^
	CSHPPartArray();
	//! �l�w��R���X�g���N�^
	/*!
	  @param cSize [in] �p�[�g��
	  @param cStart [in] �p�[�g�J�n�C���f�b�N�X�z��
	  @param cType [in] �p�[�g�^�C�v�z��
	*/
	CSHPPartArray(int cSize, const int cStart[], const int cType[]);
	//! �R�s�[�R���X�g���N�^
	CSHPPartArray(const CSHPPartArray& cArray);
	//! �f�X�g���N�^
	~CSHPPartArray();
	//! = ���Z�q
	CSHPPartArray& operator = (const CSHPPartArray& cArray);
	//! �l�̒ǉ��P
	/*!
	  @param cStart [in] �J�n�C���f�b�N�X
	  @return �Ȃ�
	*/
	void append(int cStart);
	//! �l�̒ǉ��Q
	/*!
	  @param cStart [in] �J�n�C���f�b�N�X
	  @param cType [in] �p�[�g�^�C�v�w��
	  @return �Ȃ�
	*/
	void append(int cStart, int cType);
	//! �z��̍쐬
	/*!
	  @param cSize [out] �z��T�C�Y
	  @param cSArray [out] �J�n�C���f�b�N�X�z��(free()���K�v)
	  @param cTArray [out] �p�[�g�^�C�v�z��(free()���K�v)
	  @return �z��̗v�f��
	*/
	int output(int* cSize, int** cSArray, int** cTArray) const;
	//! �z�񂩂�ǂݍ���
	/*!
	  @param cSize [in] �z��̗v�f��
	  @param cSArray [in] �J�n�C���f�b�N�X�z��
	  @param cTArray [in] �p�[�g�^�C�v�z��
	*/
	void input(int cSize, const int cSArray[], const int cTArray[]);
	//! �S�v�f�N���A
	void clear();
};

/*!
  CSHPPartArray�̃X�}�[�g�|�C���^�B
  CSHPPartArray�̎Q�ƃJ�E���g�t���|�C���^�B���Ɏg�p���Ă��Ȃ��B
*/

typedef TSmartPointer<CSHPPartArray> CSHPPartArrayPtr;

/*!
  _shp_object_delete
  SHPObject �̏��L���t���|�C���^�pdelete�t�@���N�V�����I�u�W�F�N�g
*/

struct _shp_object_delete : std::unary_function<SHPObject, void>
{
	void operator () (SHPObject* cPointer)
		{ if(cPointer) SHPDestroyObject(cPointer); }
};

/*!
  SHPObject �̏��L�����|�C���^
  CSPObj�Ŏg�p���鏊�L���t���|�C���^�B���[�U�͎g�p���Ȃ��B
*/

typedef TOwnerPointer<SHPObject, bool, _shp_object_delete> SHPObjectPtr;

/*!
  struct SHPObject ���b�p�[�N���X
  shape�̃I�u�W�F�N�g���Ǘ�����N���X
*/

class CSHPObj
{
	SHPObjectPtr mObject; //!< SHP�I�u�W�F�N�g

private:
	//! SHPObject�̎擾
	/*!
	  @return SHPObject�̃|�C���^
	*/
	SHPObjectPtr getObject();

	friend class CSHPHdl;

protected:
	//! �R�s�[�R���X�g���N�^(�֎~)
	CSHPObj(const CSHPObj&);
	//! ������Z�q(�֎~)
	CSHPObj& operator = (const CSHPObj&);

public:
	//! �f�t�H���g�R���X�g���N�^
	/*!
	  NULL�I�u�W�F�N�g�̍쐬
	 */
	CSHPObj();
	//! �l�w��R���X�g���N�^
	/*!
	  @param cSHPType [in] SHP�^�C�v
	  @param cShapeID [in] �V�F�[�vID
	  @param cPartArray [in] �p�[�g�z��
	  @param cPointArray [in] ���W�z��
	*/
	CSHPObj(int cSHPType, int cShapeID, const CSHPPartArray& cPartArray, const CSHPPointArray& cPointArray);
	//! �l�w��R���X�g���N�^(�����}���`�p�[�g�쐬)
	/*!
		@param cSHPType [in] SHP�^�C�v
		@param cPointArrayArray [in] �_��̔z��
	*/
	CSHPObj(int cSHPType, const CSHPPointArrayArray& cPointArrayArray);
	//! �l�w��R���X�g���N�^
	/*!
	  @param cSHPType [in] SHP�^�C�v
	  @param cPointArray [in] ���W�z��
	  �����ł́A���W�z��̐��@(eM)�͖��������B
	*/
	CSHPObj(int cSHPType, const CSHPPointArray& cPointArray);
	//! �l�w��R���X�g���N�^(�|�C���g����쐬)
	/*!
	  @param cSHPType [in] SHP�^�C�v
	  @param cPoint [in] ���W
	 */
	CSHPObj(int cSHPType, const CSHPPoint& cPoint);
	//! SHPObject* ����쐬
	/*!
	  CSHPHdl�p�̃R���X�g���N�^�B�ʏ�͎g�p���Ȃ��B
	  ���L�����w��ł���B
	  @param cObject [in] SHPObject�̃|�C���^
	 */
	CSHPObj(SHPObject* cObject);
	//! �f�X�g���N�^
	~CSHPObj();
	//! void�|�C���^�ւ̃L���X�g
	// operator void* () { return((void*)mObject); }
	//! �I�u�W�F�N�g�̍쐬
	/*!
	  @param cSHPType [in] SHP�^�C�v
	  @param cShapeID [in] �V�F�[�vID
	  @param cPartArray [in] �p�[�g�z��
	  @param cPointArray [in] ���W�z��
	  @return true=����, false=���s
	*/
	bool create(int cSHPType, int cShapeID, const CSHPPartArray& cPartArray, const CSHPPointArray& cPointArray);
	//! �I�u�W�F�N�g�̍쐬(�_��̔z�񂩂�쐬)
	/*!
	  @param cSHPType [in] SHP�^�C�v
	  @param cPointArrayArray [in] �_��̔z��
	  @return true=����, false=���s
	*/
	bool create(int cSHPType, const CSHPPointArrayArray& cPointArrayArray);
	//! �I�u�W�F�N�g�̍쐬(�_�񂩂�쐬)
	/*!
	  @param cSHPType [in] SHP�^�C�v
	  @param cPointArray [in] ���W�z��
	  �����ł́A���W�z��̐��@(eM)�͖��������B
	  @return true=����, false=���s
	*/
	bool create(int cSHPType, const CSHPPointArray& cPointArray);
	//! �I�u�W�F�N�g�̍쐬(�|�C���g����쐬)
	/*!
	  @param cSHPType [in] SHP�^�C�v
	  @param cPoint [in] ���W
	  @return true=����, false=���s
	*/
	bool create(int cSHPType, const CSHPPoint& cPoint);
	//! �I�u�W�F�N�g�̍폜
	/*!
	  ���L���̂���ꍇ�̂݃I�u�W�F�N�g���폜����B
	  ���L�����Ȃ��ꍇ�́A�|�C���^��NULL�ɂ��邾���B
	*/
	void clear();
	//! �|�C���g��̎擾
	/*!
	  @param cPointArray [out] �|�C���g�z��
	  @return true=����, false=���s
	*/
	bool getPointArray(CSHPPointArray& cPointArray);
	//! �p�[�g��̎擾
	/*!
	  @param cPartArray [out] �p�[�g�z��
	  @return true=����, false=���s
	*/
	bool getPartArray(CSHPPartArray& cPartArray);
};

/*!
  _shp_handle_delete�A
  SHPHandle �X�}�[�g�|�C���^�pdelete�t�@���N�V�����I�u�W�F�N�g
*/

struct _shp_handle_delete : std::unary_function<SHPInfo, void>
{
	void operator () (SHPInfo* cPointer) const
		{ if(cPointer) SHPClose(cPointer); }
};

/*!
  SHPHandle�̃X�}�[�g�|�C���^�B
  ���[�U���g�p���邱�Ƃ͂Ȃ��B
*/

typedef TSmartPointer<SHPInfo, int, _shp_handle_delete> SHPHandlePtr;

/*!
  struct SHPHandle ���b�p�[�N���X
  shape�̃t�@�C���n���h���N���X�B
*/

class CSHPHdl
{
	SHPHandlePtr mHandle; //!< SHP�n���h���̃X�}�[�g�|�C���^

private:
	//! �n���h���̎擾
	/*!
	  @return SHPHandle�̃|�C���^
	*/
	SHPHandlePtr getHandle();

	friend class CSHPTree;

protected:
	//! �R�s�[�R���X�g���N�^(�֎~)
	CSHPHdl(const CSHPHdl&);
	//! ������Z�q(�֎~)
	CSHPHdl& operator = (const CSHPHdl&);

public:
	//! �f�t�H���g�R���X�g���N�^
	CSHPHdl();
	//! �t�@�C�����I�[�v������R���X�g���N�^
	/*!
	  @param cShapeFile [in] �t�@�C����
	  @param cAccess [in] �A�N�Z�X�w��
	  fopen()���Ɠ����l���w�肷��A"r"=�ǂݍ���, "w"=��������, "rw"=�ǂݏ���
	*/
	CSHPHdl(const swstring& cShapeFile, const swstring& cAccess);
	//! �V�K�쐬�R���X�g���N�^
	/*!
	  @param cShapeFile [in] �t�@�C����
	  @param cShapeType [in] �V�F�[�v�^�C�v
	*/
	CSHPHdl(const swstring& cShapeFlie, int cShapeType);
	//! �n���h���w��R���X�g���N�^
	/*!
	  �ʏ�͎g�p���Ȃ��BSHPHandle�|�C���^�𒼐ړn���R���X�g���N�^
	  @param cHandle [in] SHPHandle�|�C���^
	*/
	CSHPHdl(SHPHandle cHandle);
	//! �f�X�g���N�^
	~CSHPHdl();

	//! �n���h���I�[�v��
	/*!
	  @param cShapeFile [in] �t�@�C����
	  @param cAccess [in] �A�N�Z�X�w��
	  fopen()���Ɠ����l���w�肷��A"r"=�ǂݍ���, "w"=��������, "rw"=�ǂݏ���
	  @return true=����, false=���s
	*/
	bool open(const swstring& cShapeFile, const swstring& cAccess);

	//! �n���h���̍쐬
	/*!
	  @param cShapeFile [in] �t�@�C����
	  @param cShapeType [in] �V�F�[�v�^�C�v
	  @return true=����, false=���s
	 */
	bool create(const swstring& cShapeFile, int cShapeType);

	//! �n���h�����̎擾
	/*!
	  @param cEntities [out] ���R�[�h��
	  @param cShapeType [out] �V�F�[�v�^�C�v
	  @param cMinBound [out] double[4]��n��(�ܓx���S�Ԃ��Ă���)
	  @param cMaxBound [out] double[4]��n��(�o�x���S�Ԃ��Ă���)
	  @return �Ȃ�
	*/
	void getInfo(int* cEntities, int* cShapeType, double* cMinBound, double* cMaxBound);

	//! �I�u�W�F�N�g�̓ǂݍ���
	/*!
	  @param cObj [out] �擾�����I�u�W�F�N�g
	  @param cShape [in] ���R�[�h�C���f�b�N�X
	  @return �Ȃ�
	*/
	// shapelib�ŃR�s�[���쐬���Ă������Ă���悤�Ȃ̂�
	// owner�t���O�͐^�̂܂�
	void readObject(CSHPObj& cObj, int cShape);

	//! �I�u�W�F�N�g�̏�������
	/*!
	  @param cObj [in] �������ރI�u�W�F�N�g
	  @param cShape [in] ���R�[�h�C���f�b�N�X
	  @return �������́A���R�[�h�C���f�b�N�X
	  ���s���� -1 ��Ԃ�
	*/
	// �����owner�t���O�͐^�̂܂�
	// ����owner�t���O��ύX����K�v�͂Ȃ��Ȃ����B
	int writeObject(CSHPObj& cObj, int cShape);

	//! �n���h���̃N���[�Y
	void close();

	/*
	** static�֐�
	*/

	//! SHPTypeName�̎擾
	/*!
	  @param cSHPType [in] SHPType
	  @return SHPType�̖��̕�����
	*/
	static swstring GetTypeName(int cSHPType);
	
	//! SHPPartTypeName�̎擾
	/*!
	  @param cSHPPartType [in] SHPPartType
	  @return SHPPartType�̖��̕�����
	*/
	static swstring GetPartTypeName(int cSHPPartType);
};

/*!
  _shp_tree_delete�A
  SHPTree�X�}�[�g�|�C���^�pdelete�t�@���N�V�����I�u�W�F�N�g
*/

struct _shp_tree_delete : std::unary_function<SHPTree, void>
{
	void operator () (SHPTree* cPointer)
		{ if(cPointer) SHPDestroyTree(cPointer); }
};

/*!
  SHPTreePtr SHPTree�̃X�}�[�g�|�C���^
*/

// typedef TSmartPointer<SHPTree, int, _shp_tree_delete> SHPTreePtr;

/*!
  CSHPTree
  SHPTree���b�p�[�N���X
*/

// class CSHPTree
// {
// 	SHPHandlePtr mHandle; //!< SHPHandle�X�}�[�g�|�C���^
// 	SHPTreePtr   mTree;   //!< SHPTree�X�}�[�g�|�C���^

// protected:
// 	//! �R�s�[�R���X�g���N�^(�֎~)
// 	CSHPTree(const CSHPTree&);
// 	//! = ���Z�q(�֎~)
// 	CSHPTree& operator = (const CSHPTree&);

// public:
// 	//! �f�t�H���g�R���X�g���N�^
// 	CSHPTree();
// 	//! �n���h���w��R���X�g���N�^
// 	/*!
// 	  @param cHdl [in] �n���h���N���X
// 	  @param cDimension [in] �H
// 	  @param cMaxDepth [in] �H
//	  @param cMinBound [in] double[4]��n��(�ܓx���S��)
//	  @param cMaxBound [in] double[4]��n��(�o�x���S��)
// 	*/
// 	CSHPTree(CSHPHdl& cHdl, int cDimension, int cMaxDepth, double* cMinBound, double* cMaxBound);
// 	//! �f�X�g���N�^
// 	~CSHPTree();

// 	//! �S�f�[�^�̃N���A(Tree�̔j��)
// 	void clear();
// 	//! Tree�̐V�K�쐬
// 	/*!
// 	  @param cHdl [in] �n���h���N���X
// 	  @param cDimention [in] �H
// 	  @param cMaxDepth [in] �H
//	  @param cMinBound [in] double[4]��n��(�ܓx���S��)
//	  @param cMaxBound [in] double[4]��n��(�o�x���S��)
// 	  @return true=����, false=���s
// 	*/
// 	bool create(CSHPHdl& cHdl, int cDimension, int cMaxDepth, double* cMinBound, double* cMaxBound);

// 	//! Tree�̏����o��
// 	/*!
// 	  @param cFilename [in] �o�̓t�@�C����
// 	  @return ture=����, false=���s
// 	*/
// 	bool write(const swstring& cFilename);
// 	//! Tree�̓ǂݍ���
// 	/*!
// 	  @param cFilename [in] ���̓t�@�C����
// 	  @return ture=����, false=���s
// 	*/
// 	bool read(const swstring& cswstring);

// 	//! �I�u�W�F�N�g�̒ǉ��P
// 	/*!
// 	  @param cObj [in] �ǉ�����I�u�W�F�N�g
// 	  �ǉ������I�u�W�F�N�g�͏��L��������
// 	  @return true=����, false=���s
// 	*/
// 	bool addObject(CSHPObj& cObj);
// 	//! �I�u�W�F�N�g�̒ǉ��Q
// 	/*!
// 	  @param cObj [in] �ǉ�����I�u�W�F�N�g�B
// 	  �ǉ������ƃI�u�W�F�N�g�͏��L���������B
// 	  @return true=����, false=���s
// 	*/
// 	bool addShapeID(CSHPObj& cObj);

// 	//! �I�u�W�F�N�g�̍폜
// 	/*!
// 	  @param cShapeID
// 	  @return true=����, false=����
// 	*/
// 	bool removeShapeID(int cShapeID);

// 	//! �m�[�h�̐���H
// 	/*!
// 	  @return �Ȃ�
// 	*/
// 	bool trimExtraNodes();
// };

/*!
 _dbf_info_delete
 DBFInfo�X�}�[�g�|�C���^�pdelete�֐��I�u�W�F�N�g
*/

struct _dbf_info_delete : std::unary_function<DBFInfo, void>
{
	void operator () (DBFInfo* cPointer)
		{ if(cPointer) DBFClose(cPointer); }
};

/*!
  DBFInfoPtr DBFInfo�X�}�[�g�|�C���^
*/

typedef TSmartPointer<DBFInfo, int, _dbf_info_delete> DBFInfoPtr;

/*!
  DBFInfo ���b�p�[�N���X
  shape�̑����t�@�C���̃n���h���N���X�B
*/

class CDBFHdl
{
	DBFInfoPtr mHandle;

protected:
	//! �R�s�[�R���X�g���N�^(�֎~)
	CDBFHdl(const CDBFHdl&);
	//! =���Z�q(�֎~)
	CDBFHdl& operator = (const CDBFHdl& cHdl);

	// DBFHanlde�w��R���X�g���N�^
	CDBFHdl(DBFHandle cHandle);

public:
	//! �f�t�H���g�R���X�g���N�^
	CDBFHdl();
	//! �쐬�R���X�g���N�^
	/*!
	  @param cFilename [in] �t�@�C�����̎w��
	*/
	CDBFHdl(const swstring& cFilename);
	//! �t�@�C���I�[�v���R���X�g���N�^
	/*!
	  @param cFilename [in] �t�@�C�����̎w��
	  @param cAccess [in] �A�N�Z�X�w��
	  fopen()���Ɠ����l���w�肷��A"r"=�ǂݍ���, "w"=��������, "rw"=�ǂݏ���
	*/
	CDBFHdl(const swstring& cFilename, const swstring& cAccess);

	//! �f�X�g���N�^
	~CDBFHdl();

	//! �쐬
	/*!
	  @param cFilename [in] �t�@�C����
	  @return true=����, false=���s
	*/
	bool create(const swstring& cFilename);
	//! �I�[�v��
	/*!
	  @param cFilename [in] �t�@�C����
	  @param cAccess [in] �A�N�Z�X�w��
	  fopen()���Ɠ����l���w�肷��A"r"=�ǂݍ���, "w"=��������, "rw"=�ǂݏ���
	  @return true=����, false=���s
	*/
	bool open(const swstring& cFilename, const swstring& cAccess);
	//! �N���[�Y
	void close();
	//! Field���̎擾
	/*!
	  @return �t�B�[���h��
	*/
	int getFieldCount();
	//! ���R�[�h���̎擾
	/*!
	  @return ���R�[�h��
	*/
	int getRecordCount();
	//! �t�B�[���h�̒ǉ�
	/*!
	  @param cFieldName [in] �t�B�[���h��
	  @param cFieldType [in] �t�B�[���h�^�C�v
	  @param cWidth [in] �t�B�[���h�̕�
	  @param cDecimals [in] �t�B�[���h�̐��x
	  @return �쐬�����t�B�[���h�̃C���f�b�N�X,-1�̏ꍇ�̓G���[
	*/
	int addField(const swstring& cFieldName, DBFFieldType cFieldType, int cWidth, int cDecimals);
	//! �t�B�[���h���̎擾
	/*!
	  @param cField [in] �t�B�[���h
	  @param cFieldName [out] �t�B�[���h��
	  @param cWidth [out] �t�B�[���h�̕�
	  @param cDecimals [out] �����̐��x
	  @return �t�B�[���h�^�C�v
	*/
	DBFFieldType getFieldInfo(int cField, swstring* cFieldName = NULL, int* cWidth = NULL, int* cDecimals = NULL);

	//! �t�B�[���h�C���f�b�N�X�̎擾
	/*!
	  @param cFieldName [in] �t�B�[���h��
	  @return �t�B�[���h�C���f�b�N�X
	*/
#ifndef NO_DBF_GET_FIELD_INDEX
	int getFieldIndex(const swstring& cFieldName);
#endif /* NO_DBF_GET_FIELD_INDEX */

	//! ���������̎擾
	/*!
	  @param cShape [in] shape�w��
	  @param cField [in] �t�B�[���h�w��
	  @return ��������
	*/
	int readIntegerAttribute(int cShape, int cFiled);

	//! ���������̎擾
	/*!
	  @param cShape [in] shape�w��
	  @param cField [in] �t�B�[���h�w��
	  @return ��������
	*/
	double readDoubleAttribute(int cShape, int cFiled);

	//! �����񑮐��̎擾
	/*!
	  @param cShape [in] shape�w��
	  @param cField [in] �t�B�[���h�w��
	  @return �����񑮐�
	*/
	swstring readStringAttribute(int cShape, int cField);

	//! ������NULL�`�F�b�N
	/*!
	  @param cShape [in] shape�w��
	  @prama cField [in] �t�B�[���h�w��
	  @param 0�ȊO=NULL
	*/
#ifndef NO_DBF_IS_ATTRIBUTE_NULL
	int isAttributeNULL(int cShape, int cField);
#endif /* NO_DBF_IS_ATTRIBUTE_NULL */

	//! ���������̏�������
	/*!
	  @param cShape [in] shape�w��
	  @param cField [in] �t�B�[���h�w��
	  @param cFieldValue [in] �����l
	  @return 0�ȊO = ����
	*/
	int writeIntegerAttribute(int cShape, int cField, int cFieldValue);

	//! ���������̏�������
	/*!
	  @param cShape [in] shape�w��
	  @param cField [in] �t�B�[���h�w��
	  @param cFieldValue [in] �����l
	  @return 0�ȊO = ����
	*/
	int writeDoubleAttribute(int cShape, int cField, const double& cFieldValue);

	//! �����񑮐��̏�������
	/*!
	  @param cShape [in] shape�w��
	  @param cField [in] �t�B�[���h�w��
	  @param cFieldValue [in] �����l
	  @return 0�ȊO = ����
	*/
	int writeStringAttribute(int cShape, int cField, const swstring& cFieldValue);

	//! NULL�����̏�������
	/*!
	  @param cShape [in] shape�w��
	  @param cField [in] �t�B�[���h�w��
	  @return 0�ȊO = ����
	*/
#ifndef NO_DBF_WRITE_NULL_ATTRIBUTE
	int writeNULLAttribute(int cShape, int cField);
#endif /* NO_DBF_WRITE_NULL_ATTRIBUTE */

	//! Tuple�̓ǂݍ���
	/*!
	  @param cEntry [in] �G���g��
	  @return �H
	*/
	swstring readTuple(int cEntry);

	//! Tuple�̏�������
	/*!
	  @param cEntry [in] �G���g��
	  @param cRawTuple [in] �H
	  @return 0�ȊO = ����
	*/
	int writeTuple(int cEntry, void* cRawTuple);

	//! �N���[���̍쐬
	/*!
	  @param cFilename [in] �t�@�C����
	  @return CDBFHdl �쐬�����n���h��
	*/
	CDBFHdl* cloneEmpty(const swstring& cFilename);

	//! native field type�̎擾
	/*!
	  @param cField [in] �t�B�[���h�w��
	  @return �t�B�[���h
	*/
#ifndef NO_DBF_GET_NATIVE_FIELD_TYPE
	char getNativeFieldType(int cField);
#endif /* NO_DBF_GET_NATIVE_FIELD_TYPE */
};

#endif /* ___SHP_WRAP_H___ */
