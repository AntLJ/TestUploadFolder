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
 * @file iterator.h
 * @brief 反復子系テンプレートライブラリ。
 *
 * - @ref adder_iterator_function
 * - @ref iterator_helper_function
 * - @ref container_traits_group
 * - @ref arctl::selected_layer_iterator
 * - @ref arctl::recursive_layer_iterator
 */

#ifndef ARCTL_ITERATOR_H_
#define ARCTL_ITERATOR_H_

#include <arctl/iterator/iterators.h>
#include <arctl/iterator/add_iterator.h>
#include <arctl/iterator/delete_iterator.h>

#if !( defined( ARCTL_NO_USE_ARCMAPUI) || defined( ARCTL_NO_USE_CARTO ) )
#include <arctl/iterator/selected_layer_iterator.h>
#endif // !( defined( ARCTL_NO_USE_ARCMAPUI) || defined( ARCTL_NO_USE_CARTO ) )

#if !defined( ARCTL_NO_USE_CARTO )
#include <arctl/iterator/recursive_layer_iterator.h>
#endif // !defined( ARCTL_NO_USE_CARTO )

#endif // ARCTL_ITERATOR_H_
