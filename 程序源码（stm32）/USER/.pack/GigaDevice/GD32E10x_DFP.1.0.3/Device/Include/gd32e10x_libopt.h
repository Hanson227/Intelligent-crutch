/*
    Copyright (C) 2017 GigaDevice

    2017-03-13, V1.0.0, firmware for GD32E10x
*/

#ifndef GD32E10X_LIBOPT_H
#define GD32E10X_LIBOPT_H

/* Comment the line below to disable peripheral header file inclusion */
#include "RTE_Components.h"

#ifdef RTE_DEVICE_STDPERIPHERALS_ADC
#include "gd32e10x_adc.h"
#endif
#ifdef RTE_DEVICE_STDPERIPHERALS_BKP
#include "gd32e10x_bkp.h"
#endif
#ifdef RTE_DEVICE_STDPERIPHERALS_CAN
#include "gd32e10x_can.h"
#endif
#ifdef RTE_DEVICE_STDPERIPHERALS_CRC
#include "gd32e10x_crc.h"
#endif
#ifdef RTE_DEVICE_STDPERIPHERALS_CTC
#include "gd32e10x_ctc.h"
#endif
#ifdef RTE_DEVICE_STDPERIPHERALS_DAC
#include "gd32e10x_dac.h"
#endif
#ifdef RTE_DEVICE_STDPERIPHERALS_DBG
#include "gd32e10x_dbg.h"
#endif
#ifdef RTE_DEVICE_STDPERIPHERALS_DMA
#include "gd32e10x_dma.h"
#endif
#ifdef RTE_DEVICE_STDPERIPHERALS_EXMC
#include "gd32e10x_exmc.h"
#endif
#ifdef RTE_DEVICE_STDPERIPHERALS_EXTI
#include "gd32e10x_exti.h"
#endif
#ifdef RTE_DEVICE_STDPERIPHERALS_FMC
#include "gd32e10x_fmc.h"
#endif
#ifdef RTE_DEVICE_STDPERIPHERALS_FWDGT
#include "gd32e10x_fwdgt.h"
#endif
#ifdef RTE_DEVICE_STDPERIPHERALS_GPIO
#include "gd32e10x_gpio.h"
#endif
#ifdef RTE_DEVICE_STDPERIPHERALS_I2C
#include "gd32e10x_i2c.h"
#endif
#ifdef RTE_DEVICE_STDPERIPHERALS_MISC
#include "gd32e10x_misc.h"
#endif
#ifdef RTE_DEVICE_STDPERIPHERALS_PMU
#include "gd32e10x_pmu.h"
#endif
#ifdef RTE_DEVICE_STDPERIPHERALS_RCU
#include "gd32e10x_rcu.h"
#endif
#ifdef RTE_DEVICE_STDPERIPHERALS_RTC
#include "gd32e10x_rtc.h"
#endif
#ifdef RTE_DEVICE_STDPERIPHERALS_SPI
#include "gd32e10x_spi.h"
#endif
#ifdef RTE_DEVICE_STDPERIPHERALS_TIMER
#include "gd32e10x_timer.h"
#endif
#ifdef RTE_DEVICE_STDPERIPHERALS_USART
#include "gd32e10x_usart.h"
#endif
#ifdef RTE_DEVICE_STDPERIPHERALS_WWDGT
#include "gd32e10x_wwdgt.h"
#endif

#endif /* GD32E10X_LIBOPT_H */

