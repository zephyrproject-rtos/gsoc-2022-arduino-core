#include <cmsis_core.h>
#include <zephyr/init.h>
int disable_mpu_rasr_xn(void)
{
	uint32_t index;
	/* Kept the max index as 8(irrespective of soc) because the sram
	 * would most likely be set at index 2.
	 */
	for (index = 0U; index < 8; index++) {
		MPU->RNR = index;
#if defined(CONFIG_ARMV8_M_BASELINE) || defined(CONFIG_ARMV8_M_MAINLINE)
		if (MPU->RBAR & MPU_RBAR_XN_Msk) {
			MPU->RBAR ^= MPU_RBAR_XN_Msk;
		}
#else
		if (MPU->RASR & MPU_RASR_XN_Msk) {
			MPU->RASR ^= MPU_RASR_XN_Msk;
		}
#endif /* CONFIG_ARMV8_M_BASELINE || CONFIG_ARMV8_M_MAINLINE */
	}
	return 0;
}

#if defined(CONFIG_BOARD_ARDUINO_NANO_33_BLE)
int disable_bootloader_mpu() {
	// MPU was previously enabled in the bootloader
	// https://github.com/bcmi-labs/zephyr/blob/31cb7dd00fd5bce4c69896b3b2ddf6259d0c0f2b/boards/arm/arduino_nano_33_ble/arduino_nano_33_ble_defconfig#L10C1-L10C15
	__disable_irq();
	disable_mpu_rasr_xn();
	__DMB();
	MPU->CTRL = 0;
	__enable_irq();
    return 0;
}
SYS_INIT(disable_bootloader_mpu, PRE_KERNEL_1, CONFIG_KERNEL_INIT_PRIORITY_DEFAULT);
#endif

#if defined (CONFIG_BOARD_ARDUINO_PORTENTA_H7)
SYS_INIT(disable_mpu_rasr_xn, PRE_KERNEL_1, CONFIG_KERNEL_INIT_PRIORITY_DEFAULT);
#endif