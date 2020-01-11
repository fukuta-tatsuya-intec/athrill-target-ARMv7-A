﻿#ifndef _DEVICE_H_
#define _DEVICE_H_

#include "std_types.h"
#include "cpu.h"
#include "mpu_config.h"
#include "mpu.h"
#include "std_device_ops.h"

/*
 * デバイス初期化関数
 */
extern void device_init_intc(CpuType *cpu, MpuAddressRegionType *region);
extern void device_init_timer(MpuAddressRegionType *region);
extern void device_init_serial(MpuAddressRegionType *region);

/*
 * デバイスクロック供給
 */
extern void device_supply_clock_intc(DeviceClockType *dev_clock);
extern void device_supply_clock_timer(DeviceClockType *dev_clock);
extern void device_supply_clock_serial(DeviceClockType *dev_clock);

/*
 * 割込み関数
 */
extern void device_raise_int(uint16 intno);

/*
 * メモリアクセス関数
 */
extern int device_io_write8(MpuAddressRegionType *region, uint32 addr, uint8 data);
extern int device_io_write16(MpuAddressRegionType *region, uint32 addr, uint16 data);
extern int device_io_write32(MpuAddressRegionType *region, uint32 addr, uint32 data);

extern int device_io_read8(MpuAddressRegionType *region, uint32 addr, uint8 *data);
extern int device_io_read16(MpuAddressRegionType *region, uint32 addr, uint16 *data);
extern int device_io_read32(MpuAddressRegionType *region, uint32 addr, uint32 *data);

typedef enum {
	DevRegisterIo_Read = 0,
	DevRegisterIo_Write,
} DevRegisterIoType;

typedef struct {
	uint32 coreId;
	uint32 address;
	uint32 size;
} DevRegisterIoArgType;

typedef struct {
	uint32	start_address;			//byte
	uint32	size;					//byte
	void (*io) (DevRegisterIoType io_type, DevRegisterIoArgType *arg);
} DevRegisterMappingType;
extern void dev_register_mapping_write_data(uint32 coreId, uint32 table_num, DevRegisterMappingType *table, uint32 address, uint32 size);
extern void dev_register_mapping_read_data(uint32 coreId, uint32 table_num, DevRegisterMappingType *table, uint32 address, uint32 size);

#endif /* _DEVICE_H_ */
