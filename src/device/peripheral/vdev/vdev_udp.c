#include "vdev_private.h"
#include "athrill_mpthread.h"
#include "cpuemu_ops.h"

static Std_ReturnType vdev_udp_get_data8(MpuAddressRegionType *region, CoreIdType core_id, uint32 addr, uint8 *data);
static Std_ReturnType vdev_udp_get_data16(MpuAddressRegionType *region, CoreIdType core_id, uint32 addr, uint16 *data);
static Std_ReturnType vdev_udp_get_data32(MpuAddressRegionType *region, CoreIdType core_id, uint32 addr, uint32 *data);
static Std_ReturnType vdev_udp_put_data8(MpuAddressRegionType *region, CoreIdType core_id, uint32 addr, uint8 data);
static Std_ReturnType vdev_udp_put_data16(MpuAddressRegionType *region, CoreIdType core_id, uint32 addr, uint16 data);
static Std_ReturnType vdev_udp_put_data32(MpuAddressRegionType *region, CoreIdType core_id, uint32 addr, uint32 data);
static Std_ReturnType vdev_udp_get_pointer(MpuAddressRegionType *region, CoreIdType core_id, uint32 addr, uint8 **data);

MpuAddressRegionOperationType	vdev_udp_memory_operation = {
		.get_data8 		= 	vdev_udp_get_data8,
		.get_data16		=	vdev_udp_get_data16,
		.get_data32		=	vdev_udp_get_data32,

		.put_data8 		= 	vdev_udp_put_data8,
		.put_data16		=	vdev_udp_put_data16,
		.put_data32		=	vdev_udp_put_data32,

		.get_pointer	= vdev_udp_get_pointer,
};


static MpthrIdType vdev_thrid;
Std_ReturnType mpthread_init(void);
extern Std_ReturnType mpthread_register(MpthrIdType *id, MpthrOperationType *op);

static Std_ReturnType vdev_thread_do_init(MpthrIdType id);
static Std_ReturnType vdev_thread_do_proc(MpthrIdType id);
static MpthrOperationType vdev_op = {
	.do_init = vdev_thread_do_init,
	.do_proc = vdev_thread_do_proc,
};

static char *remote_ipaddr = "127.0.0.1";

void device_init_vdev_udp(MpuAddressRegionType *region)
{
	Std_ReturnType err;
	uint32 portno;
	(void)cpuemu_get_devcfg_string("DEBUG_FUNC_VDEV_TX_IPADDR", &remote_ipaddr);
	err = cpuemu_get_devcfg_value("DEBUG_FUNC_VDEV_TX_PORTNO", &portno);
	if (err != STD_E_OK) {
		printf("ERROR: can not load param DEBUG_FUNC_VDEV_TX_PORTNO\n");
		ASSERT(err == STD_E_OK);
	}
	vdev_control.config.client_port = (uint16)portno;
	err = cpuemu_get_devcfg_value("DEBUG_FUNC_VDEV_RX_PORTNO", &portno);
	if (err != STD_E_OK) {
		printf("ERROR: can not load param DEBUG_FUNC_VDEV_RX_PORTNO\n");
		ASSERT(err == STD_E_OK);
	}
	vdev_control.config.server_port = (uint16)portno;

	err = udp_comm_create(&vdev_control.config, &vdev_control.comm);
	ASSERT(err == STD_E_OK);

	mpthread_init();

	err = mpthread_register(&vdev_thrid, &vdev_op);
	ASSERT(err == STD_E_OK);

	err = mpthread_start_proc(vdev_thrid);
	ASSERT(err == STD_E_OK);
	return;
}
void device_supply_clock_vdev_udp(DeviceClockType *dev_clock)
{
	uint64 interval;
	uint64 unity_sim_time;

#if 1
	unity_sim_time = vdev_control.vdev_sim_time[VDEV_SIM_INX_YOU] * ((uint64)vdev_control.cpu_freq);
#else
	unity_sim_time = vdev_get_unity_sim_time(dev_clock);
#endif

	vdev_control.vdev_sim_time[VDEV_SIM_INX_ME] = ( dev_clock->clock / ((uint64)vdev_control.cpu_freq) );

	if ((unity_sim_time != 0) && (dev_clock->min_intr_interval != DEVICE_CLOCK_MAX_INTERVAL)) {
		if ((unity_sim_time <= dev_clock->clock)) {
			interval = 2U;
			//printf("UNITY <= MICON:%llu %llu\n", vdev_control.vdev_sim_time[VDEV_SIM_INX_YOU], vdev_control.vdev_sim_time[VDEV_SIM_INX_ME]);
		}
		else {
			//interval = (unity_sim_time - dev_clock->clock) + ((unity_interval_vtime  * ((uint64)vdev_udp_control.cpu_freq)) / 2);
			interval = (unity_sim_time - dev_clock->clock);
			//printf("UNITY > MICON:%llu %llu\n", vdev_control.vdev_sim_time[VDEV_SIM_INX_YOU], vdev_control.vdev_sim_time[VDEV_SIM_INX_ME]);
		}
		if (interval < dev_clock->min_intr_interval) {
			dev_clock->min_intr_interval = interval;
		}
	}
	return;
}

static Std_ReturnType vdev_thread_do_init(MpthrIdType id)
{
	//nothing to do
	return STD_E_OK;
}

static Std_ReturnType vdev_thread_do_proc(MpthrIdType id)
{
	Std_ReturnType err;
	uint32 off = VDEV_RX_DATA_BASE - VDEV_BASE;
	uint64 curr_stime;

	while (1) {
		err = udp_comm_read(&vdev_control.comm);
		if (err != STD_E_OK) {
			continue;
		}
		//gettimeofday(&unity_notify_time, NULL);
		memcpy(&vdev_control.region->data[off], &vdev_control.comm.read_data.buffer[0], vdev_control.comm.read_data.len);
		memcpy((void*)&curr_stime, &vdev_control.comm.read_data.buffer[VDEV_SIM_TIME(VDEV_SIM_INX_ME)], 8U);

		//unity_interval_vtime = curr_stime - vdev_udp_control.vdev_sim_time[VDEV_SIM_INX_YOU];
		//vdev_calc_predicted_virtual_time(vdev_udp_control.vdev_sim_time[VDEV_SIM_INX_YOU], curr_stime);
		vdev_control.vdev_sim_time[VDEV_SIM_INX_YOU] = curr_stime;
#if 0
		{
			uint32 count = 0;
			if ((count % 1000) == 0) {
				printf("%llu, %llu\n", vdev_control.vdev_sim_time[VDEV_SIM_INX_YOU], vdev_control.vdev_sim_time[VDEV_SIM_INX_ME]);
			}
			count++;
		}
#endif
	}
	return STD_E_OK;
}


static Std_ReturnType vdev_udp_get_data8(MpuAddressRegionType *region, CoreIdType core_id, uint32 addr, uint8 *data)
{
	uint32 off = (addr - region->start);
	*data = *((uint8*)(&region->data[off]));
	return STD_E_OK;
}
static Std_ReturnType vdev_udp_get_data16(MpuAddressRegionType *region, CoreIdType core_id, uint32 addr, uint16 *data)
{
	uint32 off = (addr - region->start);
	*data = *((uint16*)(&region->data[off]));
	return STD_E_OK;
}
static Std_ReturnType vdev_udp_get_data32(MpuAddressRegionType *region, CoreIdType core_id, uint32 addr, uint32 *data)
{
	uint32 off = (addr - region->start);
	*data = *((uint32*)(&region->data[off]));
	return STD_E_OK;
}
static Std_ReturnType vdev_udp_put_data8(MpuAddressRegionType *region, CoreIdType core_id, uint32 addr, uint8 data)
{
	uint32 off = (addr - region->start);
	*((uint8*)(&region->data[off])) = data;

	if (addr == VDEV_TX_FLAG(0)) {
		uint32 tx_off = VDEV_TX_DATA_BASE - region->start;
		Std_ReturnType err;
		memcpy(&vdev_control.comm.write_data.buffer[0], &region->data[tx_off], UDP_BUFFER_LEN);
		memcpy(&vdev_control.comm.write_data.buffer[VDEV_SIM_TIME(VDEV_SIM_INX_ME)],  (void*)&vdev_control.vdev_sim_time[VDEV_SIM_INX_ME], 8U);
		memcpy(&vdev_control.comm.write_data.buffer[VDEV_SIM_TIME(VDEV_SIM_INX_YOU)], (void*)&vdev_control.vdev_sim_time[VDEV_SIM_INX_YOU], 8U);
		vdev_control.comm.write_data.len = UDP_BUFFER_LEN;
		//printf("sim_time=%llu\n", vdev_udp_control.vdev_sim_time[VDEV_SIM_INX_ME]);
		err = udp_comm_remote_write(&vdev_control.comm, remote_ipaddr);
		if (err != STD_E_OK) {
			printf("WARNING: vdevput_data8: udp send error=%d\n", err);
		}
	}
	else {
	}

	return STD_E_OK;
}
static Std_ReturnType vdev_udp_put_data16(MpuAddressRegionType *region, CoreIdType core_id, uint32 addr, uint16 data)
{
	uint32 off = (addr - region->start);
	*((uint16*)(&region->data[off])) = data;
	return STD_E_OK;
}
static Std_ReturnType vdev_udp_put_data32(MpuAddressRegionType *region, CoreIdType core_id, uint32 addr, uint32 data)
{
	uint32 off = (addr - region->start);
	*((uint32*)(&region->data[off])) = data;
	return STD_E_OK;
}
static Std_ReturnType vdev_udp_get_pointer(MpuAddressRegionType *region, CoreIdType core_id, uint32 addr, uint8 **data)
{
	uint32 off = (addr - region->start);
	*data = &region->data[off];
	return STD_E_OK;
}
