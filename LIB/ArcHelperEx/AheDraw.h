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

/**
* @file AheDraw.h
*
* @brief Arc Helper �`��n�O���[�o���֐���`�t�@�C��
*/
#if	!defined( __AHEDRAW_H__)
#define __AHEDRAW_H__	//!< �d���C���N���[�h�h�~

/////////////////////////////////////////////////////////////////////////////
//
// �F�y�уV���{���Ɋւ���֐�
//
/////////////////////////////////////////////////////////////////////////////
/**
 * @brief IColorPtr ���擾����
 *
 * @param red				[in]			�F(�ԁj
 * @param green				[in]			�F(�΁j
 * @param blue				[in]			�F(�j
 *
 * @return IColorPtr
 */
IColorPtr AheGetColor( LONG red, LONG green, LONG blue );

/**
 * @brief IColorPtr ���擾����
 *
 * @param color				[in]			RGB �}�N���ō쐬���� COLORREF
 *
 * @return IColorPtr
 */
IColorPtr AheGetColor( COLORREF color );

/**
 * @brief �w�肵���`������|�C���g�V���{�����쐬����
 *
 * @param size,				[in]			�|�C���g�V���{���̃T�C�Y
 * @param red				[in]			�|�C���g�V���{���̐F(�ԁj
 * @param green				[in]			�|�C���g�V���{���̐F(�΁j
 * @param blue				[in]			�|�C���g�V���{���̐F(�j
 * @param style				[in]			�|�C���g�̃X�^�C��
 * @param bDrawXOR			[in]			XOR �ŕ`�悷�邩�ǂ���
 * @param bOutLine			[in]			�A�E�g���C����`�悷�邩�ǂ���
 *	
 * @return ISymbolPtr
 */
ISymbolPtr AheGetPointSymbol( double size, LONG red, LONG green, LONG blue, esriSimpleMarkerStyle style, BOOL bDrawXOR, BOOL bOutLine);

/**
 * @brief �w�肵���`������|�C���g�V���{�����쐬����
 *
 * @param size,				[in]			�|�C���g�V���{���̃T�C�Y
 * @param color				[in]			RGB �}�N���ō쐬���� COLORREF
 * @param style				[in]			�|�C���g�̃X�^�C��
 * @param bDrawXOR			[in]			XOR �ŕ`�悷�邩�ǂ���
 * @param bOutLine			[in]			�A�E�g���C����`�悷�邩�ǂ���
 *	
 * @return ISymbolPtr
 */
ISymbolPtr AheGetPointSymbol( double size, COLORREF color, esriSimpleMarkerStyle style, BOOL bDrawXOR, BOOL bOutLine );

/**
 * @brief �w�肵���`������|�C���g�V���{�����쐬����
 *
 * @param width,			[in]			���C���V���{���̕�
 * @param red				[in]			���C���V���{���̐F(�ԁj
 * @param green				[in]			���C���V���{���̐F(�΁j
 * @param blue				[in]			���C���V���{���̐F(�j
 * @param style				[in]			���C���̃X�^�C��
 * @param bDrawXOR			[in]			XOR �ŕ`�悷�邩�ǂ���
 *	
 * @return ISymbolPtr
 */
ISymbolPtr AheGetLineSymbol( double width, LONG red, LONG green, LONG blue, esriSimpleLineStyle style, BOOL bDrawXOR );

/**
 * @brief �w�肵���`������|�C���g�V���{�����쐬����
 *
 * @param width,			[in]			���C���V���{���̕�
 * @param color				[in]			RGB �}�N���ō쐬���� COLORREF
 * @param style				[in]			���C���̃X�^�C��
 * @param bDrawXOR			[in]			XOR �ŕ`�悷�邩�ǂ���
 *	
 * @return ISymbolPtr
 */
ISymbolPtr AheGetLineSymbol( double width, COLORREF color, esriSimpleLineStyle style, BOOL bDrawXOR );

/**
 * @brief �|���S���V���{�����쐬����
 *
 * @param red				[in]			�|���S���V���{���̐F(�ԁj
 * @param green				[in]			�|���S���V���{���̐F(�΁j
 * @param blue				[in]			�|���S���V���{���̐F(�j
 * @param style				[in]			�|���S���̃X�^�C��
 * @param bDrawXOR			[in]			XOR �ŕ`�悷�邩�ǂ���
 *
 * @return ISymbolPtr
 */
ISymbolPtr AheGetPolygonSymbol( LONG red, LONG green, LONG blue, esriSimpleFillStyle style, BOOL bDrawXOR );

/**
 * @brief �|���S���V���{�����쐬����
 *
 * @param color				[in]			RGB �}�N���ō쐬���� COLORREF
 * @param style				[in]			�|���S���̃X�^�C��
 * @param bDrawXOR			[in]			XOR �ŕ`�悷�邩�ǂ���
 *
 * @return ISymbolPtr
 */
ISymbolPtr AheGetPolygonSymbol( COLORREF color, esriSimpleFillStyle style, BOOL bDrawXOR );
/////////////////////////////////////////////////////////////////////////////
//
// �t�B�[�`���`��
//
/////////////////////////////////////////////////////////////////////////////

/**
 * @brief �w�肵���`������t�B�[�`����`�悷��
 *
 * @param pIDisplay			[in]			IDisplay �̃C���X�^���X
 * @param ipGeom			[in]			�`�悵�����`��
 * @param bDoStartStop		[in,optional]	StartDrawing, StopDrawing ���邩�ǂ����i�f�t�H���g TRUE�j
 */
void AheDrawFeature( IDisplayPtr pIDisplay, IGeometryPtr ipGeom, BOOL bDoStartStop = TRUE );

/**
 * @brief �|�C���g�A���C���A�|���S���̓_���\��
 *
 * @param pIDisplay	[in]			IDisplay �̃C���X�^���X
 * @param ipFeature			[in]			�_���\������Ώۃt�B�[�`��
 * @param bDrawXOR			[in]			XOR �ŕ`�悷�邩�ǂ���
 * @param size				[in]			�T�C�Y
 * @param red				[in]			�_�̐F(�ԁj
 * @param green				[in]			�_�̐F(�΁j
 * @param blue				[in]			�_�̐F(�j
 * @param style				[in]			�_�̃X�^�C��
 * @param bOutLine			[in]			�A�E�g���C����`�悷�邩�ǂ���
 * @param bDoStartStop		[in,optional]	StartDrawing, StopDrawing ���邩�ǂ����i�f�t�H���g TRUE�j
 */
void AheDrawVertex( IDisplayPtr pIDisplay, IFeaturePtr ipFeature, BOOL bXOR = FALSE, LONG size = 3, LONG red = 200, LONG green = 200, LONG blue = 200, esriSimpleMarkerStyle style = esriSMSSquare, BOOL bOutLine = FALSE, BOOL bDoStartStop = TRUE );

/**
 * @brief �W�I���g���̒��_��\��
 *
 * @param pIDisplay	[in]			IDisplay �̃C���X�^���X
 * @param ipFeatGeom		[in]			�_���\������ΏۃW�I���g��
 * @param bDrawXOR			[in]			XOR �ŕ`�悷�邩�ǂ���
 * @param size				[in]			�T�C�Y
 * @param red				[in]			�_�̐F(�ԁj
 * @param green				[in]			�_�̐F(�΁j
 * @param blue				[in]			�_�̐F(�j
 * @param style				[in]			�_�̃X�^�C��
 * @param bOutLine			[in]			�A�E�g���C����`�悷�邩�ǂ���
 * @param bDoStartStop		[in,optional]	StartDrawing, StopDrawing ���邩�ǂ����i�f�t�H���g TRUE�j
 */
void AheDrawGeomVertex( IDisplayPtr pIDisplay, IGeometryPtr ipFeatGeom, BOOL bXOR, LONG size, LONG red, LONG green, LONG blue, esriSimpleMarkerStyle style, BOOL bOutLine, BOOL bDoStartStop = TRUE );

/**
 * @brief �|�C���g��\��
 *
 * @param pIDisplay	[in]			IDisplay �̃C���X�^���X
 * @param ipPoint			[in]			�_���\������ΏۃW�I���g��
 * @param bDrawXOR			[in]			XOR �ŕ`�悷�邩�ǂ���
 * @param size				[in]			�T�C�Y
 * @param red				[in]			�_�̐F(�ԁj
 * @param green				[in]			�_�̐F(�΁j
 * @param blue				[in]			�_�̐F(�j
 * @param style				[in]			�_�̃X�^�C��
 * @param bOutLine			[in]			�A�E�g���C����`�悷�邩�ǂ���
 * @param bDoStartStop		[in,optional]	StartDrawing, StopDrawing ���邩�ǂ����i�f�t�H���g TRUE�j
 */
void AheDrawPoint( IDisplayPtr ipScreenDisplay, IPointPtr ipPoint, BOOL bXOR = FALSE, LONG size = 3, LONG red = 200, LONG green = 200, LONG blue = 200, esriSimpleMarkerStyle style = esriSMSSquare, BOOL bOutLine = FALSE, BOOL bDoStartStop = TRUE );	

/**
 * @brief �|�C���g�R���N�V������\��
 *
 * @param pIDisplay	[in]			IDisplay �̃C���X�^���X
 * @param ipPointCol		[in]			�\������|�C���g�R���N�V����
 * @param bDrawXOR			[in]			XOR �ŕ`�悷�邩�ǂ���
 * @param size				[in]			�T�C�Y
 * @param red				[in]			�_�̐F(�ԁj
 * @param green				[in]			�_�̐F(�΁j
 * @param blue				[in]			�_�̐F(�j
 * @param style				[in]			�_�̃X�^�C��
 * @param bOutLine			[in]			�A�E�g���C����`�悷�邩�ǂ���
 * @param bDoStartStop		[in,optional]	StartDrawing, StopDrawing ���邩�ǂ����i�f�t�H���g TRUE�j
 */
void AheDrawPoints( IDisplayPtr pIDisplay, IPointCollectionPtr ipPointCol, BOOL bXOR, LONG size, LONG red, LONG green, LONG blue, esriSimpleMarkerStyle style, BOOL bOutLine, BOOL bDoStartStop = TRUE );

void AheDrawMultipoint( IDisplayPtr pIDisplay, IGeometryPtr ipGeom, BOOL bXOR, LONG size, LONG red, LONG green, LONG blue, esriSimpleMarkerStyle style, BOOL bOutLine, BOOL bDoStartStop = TRUE );

/////////////////////////////////////////////////////////////////////////////
//
// ���C���n�`��
//
/////////////////////////////////////////////////////////////////////////////
/**
 * @brief �|�����C����\��
 *
 * @param pIDisplay			[in]			IDisplay �̃C���X�^���X
 * @param ipPolyline		[in]			�\������|�����C��
 * @param bDoStartStop		[in,optional]	StartDrawing, StopDrawing ���邩�ǂ����i�f�t�H���g TRUE�j
 */
void AheDrawPolyline( IDisplayPtr pIDisplay, IPolylinePtr ipPolyline, BOOL bDoStartStop = TRUE );


/**
 * @brief �|�����C����\��
 *
 * @param pIDisplay			[in]			IDisplay �̃C���X�^���X
 * @param ipPolyline		[in]			�\������|�����C��
 * @param rgbColor			[in]			�\���F
 * @param nWidth			[in]			��
 * @param emStyle			[in]			�\���X�^�C��
 * @param bDoStartStop		[in,optional]	StartDrawing, StopDrawing ���邩�ǂ����i�f�t�H���g TRUE�j
 * @param bXOR				[in,optional]	XOR �ŕ`�悷�邩�ǂ����i�f�t�H���g FALSE�j
 */
void AheDrawPolyline( IDisplayPtr pIDisplay, IPolylinePtr piPolyline, COLORREF rgbColor, int nWidth, esriSimpleLineStyle enStyle, BOOL bDoStartStop = TRUE, BOOL bXOR = FALSE );

/**
 * @brief ���t���|�����C����\��
 *
 * @param ipScreenDisplay	[in]			IDisplayPtr
 * @param ipPolyline		[in]			�\������|�����C��
 * @param rgbColor			[in]			�\���F
 * @param bDoStartStop		[in]			StartDrawing, StopDrawing ���邩�ǂ���
 * @param bDrawArrow		[in,optional]	����`�悷�邩�ǂ����i�f�t�H���g�FTRUE�j
 * @param bOffset			[in,optional]	�I�t�Z�b�g�\�����邩�ǂ����i�f�t�H���g�FFALSE�j
 * @param bOffsetReverse	[in,optional]	�I�t�Z�b�g���t�Ɏ�邩�ǂ����i�f�t�H���g�FFALSE�j
 */
void AheDrawArrowline( IDisplayPtr ipScreenDisplay, IPolylinePtr ipPolyline, COLORREF rgbColor, BOOL bDoStartStop, BOOL bDrawArrow = TRUE, BOOL bOffset = FALSE, BOOL bOffsetReverse = FALSE );
void AheDrawArrowline2( IDisplayPtr ipScreenDisplay, std::list<LQ> Data, std::map<COLORREF, SYMBOL> mapColor, LONG lDispDrawMode, DOUBLE dbArrowSize);
/////////////////////////////////////////////////////////////////////////////
//
// �|���S���n�`��
//
/////////////////////////////////////////////////////////////////////////////

/**
 * @brief �|���S����\��
 *
 * @param pIDisplay			[in]			IDisplay �̃C���X�^���X
 * @param ipPolygon			[in]			�\������|���S��
 * @param bDoStartStop		[in,optional]	StartDrawing, StopDrawing ���邩�ǂ����i�f�t�H���g TRUE�j
 */
void AheDrawPolygon( IDisplayPtr pIDisplay, IPolygonPtr ipPolygon, BOOL bDoStartStop = TRUE );

/**
 * @brief �|���S����\��
 *
 * @param pIDisplay			[in]			IDisplay �̃C���X�^���X
 * @param ipPolygon			[in]			�\������|���S��
 * @param rgbColor			[in]			�\���F
 * @param nWidth			[in]			��
 * @param emStyle			[in]			�\���X�^�C��
 * @param bDoStartStop		[in,optional]	StartDrawing, StopDrawing ���邩�ǂ����i�f�t�H���g TRUE�j
 * @param bXOR				[in,optional]	XOR �ŕ`�悷�邩�ǂ����i�f�t�H���g FALSE�j
 */
void AheDrawPolygon( IDisplayPtr pIDisplay, IPolygonPtr piPolygon, COLORREF rgbColor, int nWidth, esriSimpleLineStyle enStyle, BOOL bDoStartStop = TRUE, BOOL bXOR = FALSE );

/**
 * @brief �|���S����\���Q
 *
 * @param pIDisplay			[in]			IDisplay �̃C���X�^���X
 * @param ipPolygon			[in]			�\������|���S��
 * @param bDoStartStop		[in,optional]	StartDrawing, StopDrawing ���邩�ǂ����i�f�t�H���g TRUE�j
 */
void AheDrawPolygon2( IDisplayPtr pIDisplay, IPolygonPtr ipPolygon, BOOL bDoStartStop = TRUE );

/**
 * @brief �|���S���� XOR �ŕ\��
 *
 * @param pIDisplay			[in]			IDisplay �̃C���X�^���X
 * @param ipPolygon			[in]			�\������|���S��
 * @param bDoStartStop		[in,optional]	StartDrawing, StopDrawing ���邩�ǂ����i�f�t�H���g TRUE�j
 */
void AheDrawPolygonXOR( IDisplayPtr pIDisplay, IPolygonPtr ipPolygon, BOOL bDoStartStop = TRUE );

/**
 * @brief �t�B�[�`���� XOR �ŕ`�悷��
 *
 * @param pIDisplay			[in]			IDisplay �̃C���X�^���X
 * @param ipFeature			[in]			�\������t�B�[�`��
 * @param dbBufferSize		[in]			�o�b�t�@����T�C�Y
 * @param bDrawVertex		[in]			���_��`�悷�邩�ǂ���
 * @param bDoStartStop		[in,optional]	StartDrawing, StopDrawing ���邩�ǂ����i�f�t�H���g TRUE�j
 */
void AheDrawFeatureXOR( IDisplayPtr pIDisplay, IFeaturePtr ipFeature, double dbBufferSize, BOOL bDrawVertex, BOOL bDoStartStop = TRUE );
/////////////////////////////////////////////////////////////////////////////
//
// ���C���n�G�������g�ǉ�
//
/////////////////////////////////////////////////////////////////////////////
/**
 * @brief �|�����C���̃G�������g�ǉ�
 *
 * @param ipContainer		[in]			IGraphicsContainerPtr
 * @param ipGeometry		[in]			IGeometryPtr
 * @param rgbColor			[in]			RGB �}�N���ō쐬���� COLORREF
 * @param nWidth			[in]			��
 * @param emStyle			[in]			�X�^�C��
 *
 * @return IElementPtr
 */
IElementPtr AheAddLineElement( IGraphicsContainerPtr ipContainer, IGeometryPtr ipGeometry, COLORREF rgbColor, int nWidth, esriSimpleLineStyle emStyle );
/////////////////////////////////////////////////////////////////////////////
//
// �|�C���g�n�G�������g�ǉ�
//
/////////////////////////////////////////////////////////////////////////////
/**
 * @brief �|�C���g�̃G�������g�ǉ�
 *
 * @param ipContainer		[in]			IGraphicsContainerPtr
 * @param ipGeometry		[in]			IGeometryPtr
 * @param rgbColor			[in]			RGB �}�N���ō쐬���� COLORREF
 * @param nSize				[in]			�T�C�Y
 * @param emStyle			[in]			�X�^�C��
 *
 * @return IElementPtr
 */
_ISetPtr AheAddPointElement( IGraphicsContainerPtr ipContainer, IGeometryPtr ipGeometry, COLORREF rgbColor, int nSize, esriSimpleMarkerStyle emStyle );
/////////////////////////////////////////////////////////////////////////////
//
// �|���S���n�G�������g�ǉ�
//
/////////////////////////////////////////////////////////////////////////////
/**
 * @brief �|���S���̃G�������g�ǉ�
 *
 * @param ipContainer		[in]			IGraphicsContainerPtr
 * @param ipGeometry		[in]			IGeometryPtr
 * @param rgbColor			[in]			RGB �}�N���ō쐬���� COLORREF
 * @param emStyle			[in]			�X�^�C��
 *
 * @return IElementPtr
 */
IElementPtr AheAddPolygonElement( IGraphicsContainerPtr ipContainer, IGeometryPtr ipGeometry, COLORREF rgbColor, esriSimpleFillStyle emStyle );

#endif	//__AHEDRAW_H__
