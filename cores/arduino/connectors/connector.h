/*
 * Copyright (c) 2026 TOKITA Hiroshi
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#if DT_NODE_EXISTS(DT_NODELABEL(arduino_header)) &&                                                \
	DT_NODE_HAS_COMPAT(DT_NODELABEL(arduino_header), arduino_header_r3)
#include "arduino_header_r3.h"
#else
#error "Only arduino-header-r3 connector is supported"
#endif

#define ZARD_CHECK_GPIO_CTLR(node_id)                                                              \
	COND_CODE_1(DT_NODE_HAS_PROP(node_id, gpio_controller), (node_id,), ())

#define ZARD_ALL_GPIO_CTLR DT_FOREACH_NODE(ZARD_CHECK_GPIO_CTLR)

#define ZARD_IDX_IF_MATCH(i, n)                                                                    \
	COND_CODE_1(DT_SAME_NODE(n, GET_ARG_N(UTIL_INC(i), ZARD_ALL_GPIO_CTLR)), (i), ())

#define ZARD_MATCH_IDX(n) LISTIFY(NUM_VA_ARGS_LESS_1(ZARD_ALL_GPIO_CTLR), ZARD_IDX_IF_MATCH, (), n)

#define ZARD_GET_NGPIOS(i, ...) DT_PROP(GET_ARG_N(UTIL_INC(i), __VA_ARGS__), ngpios)
#define ZARD_SUM_NGPIOS(...)    LISTIFY(NUM_VA_ARGS(__VA_ARGS__), ZARD_GET_NGPIOS, (+), __VA_ARGS__)

#define ZARD_GLOBAL_GPIO_OFFSET_(ph)                                                               \
	ZARD_SUM_NGPIOS(GET_ARGS_FIRST_N(ZARD_MATCH_IDX(ph), ZARD_ALL_GPIO_CTLR))

#define ZARD_GLOBAL_GPIO_OFFSET(ph)                                                                \
	COND_CODE_1(IS_EQ(NUM_VA_ARGS(ZARD_GLOBAL_GPIO_OFFSET_(ph)), 0),                           \
					     (0), (ZARD_GLOBAL_GPIO_OFFSET_(ph)))

#define ZARD_CONNECTOR_PIN_IS_DIGITAL(node, num)                                                   \
	UTIL_CAT(UTIL_CAT(ZARD_,                                                                       \
					  UTIL_CAT(DT_STRING_UPPER_TOKEN_BY_IDX(node, compatible, 0), _IS_DIGITAL_)),  \
			 num)

#define ZARD_CONNECTOR_PIN_IS_ANALOG(node, num)                                                    \
	UTIL_CAT(                                                                                      \
		UTIL_CAT(ZARD_, UTIL_CAT(DT_STRING_UPPER_TOKEN_BY_IDX(node, compatible, 0), _IS_ANALOG_)), \
		num)

#define ZARD_CONNECTOR_PIN_NAME_D(node, num)                                                       \
	UTIL_CAT(UTIL_CAT(ZARD_,                                                                       \
					  UTIL_CAT(DT_STRING_UPPER_TOKEN_BY_IDX(node, compatible, 0), _PIN_NAME_D_)),  \
			 num)

#define ZARD_CONNECTOR_PIN_NAME_A(node, num)                                                       \
	UTIL_CAT(UTIL_CAT(ZARD_,                                                                       \
					  UTIL_CAT(DT_STRING_UPPER_TOKEN_BY_IDX(node, compatible, 0), _PIN_NAME_A_)),  \
			 num)

#define ZARD_CONN_DN_ENUMS(n, p, i)                                                                \
	COND_CODE_1(ZARD_CONNECTOR_PIN_IS_DIGITAL(DT_NODELABEL(ZARD_CONNECTOR),                    \
		     DT_MAP_ENTRY_CHILD_SPECIFIER_BY_IDX(n, p, i, 0)),                             \
		    (ZARD_CONNECTOR_PIN_NAME_D(DT_NODELABEL(ZARD_CONNECTOR),                       \
		     DT_MAP_ENTRY_CHILD_SPECIFIER_BY_IDX(n, p, i, 0)) =                            \
	             ZARD_GLOBAL_GPIO_OFFSET(DT_MAP_ENTRY_PARENT_BY_IDX(n, p, i)) +                \
		     DT_MAP_ENTRY_PARENT_SPECIFIER_BY_IDX(n, p, i, 0),), ())

#define ZARD_CONN_AN_ENUMS(n, p, i)                                                                \
	COND_CODE_1(ZARD_CONNECTOR_PIN_IS_ANALOG(DT_NODELABEL(ZARD_CONNECTOR),                     \
		     DT_MAP_ENTRY_CHILD_SPECIFIER_BY_IDX(n, p, i, 0)),                             \
		    (ZARD_CONNECTOR_PIN_NAME_A(DT_NODELABEL(ZARD_CONNECTOR),                       \
		     DT_MAP_ENTRY_CHILD_SPECIFIER_BY_IDX(n, p, i, 0)) =                            \
	             ZARD_GLOBAL_GPIO_OFFSET(DT_MAP_ENTRY_PARENT_BY_IDX(n, p, i)) +                \
		     DT_MAP_ENTRY_PARENT_SPECIFIER_BY_IDX(n, p, i, 0),), ())
