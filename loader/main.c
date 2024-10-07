/*
 * Copyright (C) 2024 Arduino SA
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(app);

#include <zephyr/kernel.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/llext/llext.h>
#include <zephyr/llext/buf_loader.h>
#include <zephyr/shell/shell.h>

#include <stdlib.h>

static int loader(const struct shell *sh)
{
	const struct flash_area *fa;
	int rc;

	/* Test that attempting to open a disabled flash area fails */
	rc = flash_area_open(FIXED_PARTITION_ID(user_sketch), &fa);
	if (rc) {
		printk("Failed to open flash area, rc %d\n", rc);
		return rc;
	}

	char header[16];
	rc = flash_area_read(fa, 0, header, sizeof(header));
	if (rc) {
		printk("Failed to read header, rc %d\n", rc);
		return rc;
	}

	if (!header[0] || header[0] == 0xff) {
		printk("No sketch found\n");
		return -ENOENT;
	}

	char *endptr;
	size_t sketch_buf_len = strtoul(header, &endptr, 10);
	// make sure number got parsed correctly"
	if (header == endptr) {
		printk("Failed to parse sketch size\n");
		return -EINVAL;
	}

	uint8_t debug = endptr[1];
	if (debug != 0 && strcmp(k_thread_name_get(k_current_get()), "main") == 0) {
		// starts the shell
		return 0;
	}

	int header_len = 16;

#if defined(CONFIG_LLEXT_STORAGE_WRITABLE)
	uint8_t* sketch_buf = k_aligned_alloc(4096, sketch_buf_len);

	if (!sketch_buf) {
		printk("Unable to allocate %d bytes\n", sketch_buf_len);
	}

	rc = flash_area_read(fa, header_len, sketch_buf, sketch_buf_len);
	if (rc) {
		printk("Failed to read sketch area, rc %d\n", rc);
		return rc;
	}
#else
	uint32_t offset = FIXED_PARTITION_OFFSET(user_sketch);
	uint8_t* sketch_buf = (uint8_t*)(offset+header_len);
#endif

	struct llext_buf_loader buf_loader = LLEXT_BUF_LOADER(sketch_buf, sketch_buf_len);
	struct llext_loader *ldr = &buf_loader.loader;

	LOG_HEXDUMP_DBG(sketch_buf, 4, "4 byte MAGIC");

	struct llext_load_param ldr_parm = LLEXT_LOAD_PARAM_DEFAULT;
	struct llext *ext;
	int res;

	res = llext_load(ldr, "sketch", &ext, &ldr_parm);
	if (res) {
		printk("Failed to load sketch, rc %d\n", res);
		return res;
	}

	void (*main_fn)() = llext_find_sym(&ext->exp_tab, "main");
	if (!main_fn) {
		printk("Failed to find main function\n");
		return -ENOENT;
	}

	llext_bootstrap(ext, main_fn, NULL);

	return 0;
}

#if CONFIG_SHELL
SHELL_CMD_REGISTER(sketch, NULL, "Run sketch", loader);
#endif

int main(void)
{
	loader(NULL);
	return 0;
}

