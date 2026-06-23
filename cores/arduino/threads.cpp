/*
 * Copyright (c) Arduino s.r.l. and/or its affiliated companies
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "Arduino.h"

#ifdef CONFIG_MULTITHREADING
void start_static_threads() {
#define _FOREACH_STATIC_THREAD(thread_data)                                                        \
	STRUCT_SECTION_FOREACH (_static_thread_data, thread_data)

	_FOREACH_STATIC_THREAD(thread_data) {
#ifdef CONFIG_TIMER_READS_ITS_FREQUENCY_AT_RUNTIME
		k_timeout_t init_delay = K_MSEC(thread_data->init_delay_ms);
#else
		k_timeout_t init_delay = thread_data->init_delay;
#endif

		k_thread_create(thread_data->init_thread, thread_data->init_stack,
						thread_data->init_stack_size, thread_data->init_entry, thread_data->init_p1,
						thread_data->init_p2, thread_data->init_p3, thread_data->init_prio,
						thread_data->init_options, init_delay);

		k_thread_name_set(thread_data->init_thread, thread_data->init_name);
		thread_data->init_thread->init_data = thread_data;
	}
}
#endif
