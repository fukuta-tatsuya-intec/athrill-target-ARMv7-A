#include <stdio.h>

#include "intc.h"
#include "device.h"
#include "std_cpu_ops.h"
#include "cpu_common/cpu_ops.h"
#include "mpu.h"

IntcControlType intc_control;
static MpuAddressRegionType *intc_region;

static void intc_raise_pending_intr(TargetCoreType *cpu);

static Std_ReturnType intc_get_data8(MpuAddressRegionType *region, CoreIdType core_id, uint32 addr, uint8 *data);
static Std_ReturnType intc_get_data16(MpuAddressRegionType *region, CoreIdType core_id, uint32 addr, uint16 *data);
static Std_ReturnType intc_get_data32(MpuAddressRegionType *region, CoreIdType core_id, uint32 addr, uint32 *data);
static Std_ReturnType intc_put_data8(MpuAddressRegionType *region, CoreIdType core_id, uint32 addr, uint8 data);
static Std_ReturnType intc_put_data16(MpuAddressRegionType *region, CoreIdType core_id, uint32 addr, uint16 data);
static Std_ReturnType intc_put_data32(MpuAddressRegionType *region, CoreIdType core_id, uint32 addr, uint32 data);
static Std_ReturnType intc_get_pointer(MpuAddressRegionType *region, CoreIdType core_id, uint32 addr, uint8 **data);

MpuAddressRegionOperationType	intc_memory_operation = {
		.get_data8 		= 	intc_get_data8,
		.get_data16		=	intc_get_data16,
		.get_data32		=	intc_get_data32,

		.put_data8 		= 	intc_put_data8,
		.put_data16		=	intc_put_data16,
		.put_data32		=	intc_put_data32,

		.get_pointer	= 	intc_get_pointer,
};


void device_init_intc(CpuType *cpu, MpuAddressRegionType *region)
{
	int i;
	uint32 regaddr;
	uint32 *pregaddr;
	uint8 *xxIcn;
	uint16 *imr0_6;
	uint8 *imr7;
	uint8 *ispr;
	uint32 coreId = 0;
	int core_id_num = CPU_CONFIG_GET_CORE_ID_NUM();

	intc_control.cpu = cpu;
	intc_region = region;
	/*
	 * intc_controlの初期化
	 */
	/*
	 * NMI初期化
	 */
	intc_control.nmi.intwdt2_hasreq = FALSE;
	intc_control.nmi.nmi_reqnum = 0;

	for (coreId = 0; coreId < core_id_num; coreId++) {

		/*
		 * マスカブル割り込み初期化
		 */
		for (i = 0; i < INTC_NUM; i++) {
			intc_control.work[coreId].is_waiting_lvl[i] = INTC_NUM_INTLVL;
			intc_control.work[coreId].waiting_lvl_num[i] = 0;
		}
		intc_control.work[coreId].current_intno = -1;
		intc_control.work[coreId].waiting_int_num = 0;

		intc_control.work[coreId].saved_intno_off = 0;
		for (i = 0; i < INTC_NUM_INTLVL; i++) {
			intc_control.work[coreId].saved_intno_stack[i] = 0;
		}
		/*
		 * xxIcnの初期化
		 */
		for (i = 0; i < INTC_NUM; i++) {
			regaddr = intc_regaddr_icn(i);
			(void)intc_get_pointer(intc_region, coreId, (regaddr & intc_region->mask), (uint8**)&pregaddr);
			xxIcn = (uint8*)pregaddr;

			*xxIcn = 0x47;
		}

		/*
		 * IMR0-IMR7の初期化
		 */
		(void)intc_get_pointer(intc_region, coreId, (INTC_REG_IMR0 & intc_region->mask), (uint8**)&pregaddr);
		imr0_6 = (uint16*)pregaddr;
		*imr0_6 = 0xFFFF;

		(void)intc_get_pointer(intc_region, coreId, (INTC_REG_IMR1 & intc_region->mask), (uint8**)&pregaddr);
		imr0_6 = (uint16*)pregaddr;
		*imr0_6 = 0xFFFF;

		(void)intc_get_pointer(intc_region, coreId, (INTC_REG_IMR2 & intc_region->mask), (uint8**)&pregaddr);
		imr0_6 = (uint16*)pregaddr;
		*imr0_6 = 0xFFFF;

		(void)intc_get_pointer(intc_region, coreId, (INTC_REG_IMR3 & intc_region->mask), (uint8**)&pregaddr);
		imr0_6 = (uint16*)pregaddr;
		*imr0_6 = 0xFFFF;

		(void)intc_get_pointer(intc_region, coreId, (INTC_REG_IMR4 & intc_region->mask), (uint8**)&pregaddr);
		imr0_6 = (uint16*)pregaddr;
		*imr0_6 = 0xFFFF;

		(void)intc_get_pointer(intc_region, coreId, (INTC_REG_IMR5 & intc_region->mask), (uint8**)&pregaddr);
		imr0_6 = (uint16*)pregaddr;
		*imr0_6 = 0xFFFF;

		(void)intc_get_pointer(intc_region, coreId, (INTC_REG_IMR6 & intc_region->mask), (uint8**)&pregaddr);
		imr0_6 = (uint16*)pregaddr;
		*imr0_6 = 0xFFFF;

		(void)intc_get_pointer(intc_region, coreId, (INTC_REG_IMR7 & intc_region->mask), (uint8**)&pregaddr);
		imr7 = (uint8*)pregaddr;
		*imr7 = 0x1F;

		/*
		 * ISPRの初期化
		 */
		(void)intc_get_pointer(intc_region, coreId, (INTC_REG_ISPR & intc_region->mask), (uint8**)&pregaddr);
		ispr = (uint8*)pregaddr;
		*ispr = 0x00;

	}

	return;
}
void device_supply_clock_intc(DeviceClockType *dev_clock)
{
	uint32 coreId;
	int core_id_num = CPU_CONFIG_GET_CORE_ID_NUM();

	dev_clock->clock++;
	for (coreId = 0; coreId < core_id_num; coreId++) {
		if (intc_control.work[coreId].current_intno != -1) {
			dev_clock->intclock++;
			break;
		}
	}

	for (coreId = 0; coreId < core_id_num; coreId++) {
		intc_raise_pending_intr(&intc_control.cpu->cores[coreId].core);
	}

	return;
}
static void set_wait_intno(uint32 coreId, int intno, uint32 lvl)
{
	if (intc_control.work[coreId].is_waiting_lvl[intno] == INTC_NUM_INTLVL) {
		intc_control.work[coreId].is_waiting_lvl[intno] = lvl;
		intc_control.work[coreId].waiting_lvl_num[intno] = 1;
		intc_control.work[coreId].waiting_int_num++;
#if 0
		printf("set_wait_intno:coreId=%d intno=%d lvl=%d waiting num=%d current_intno=%d\n",
				coreId, intno, intc_control.work[coreId].is_waiting_lvl[intno],
				intc_control.work[coreId].waiting_int_num,
				intc_control.work[coreId].current_intno);
#endif
	}
	else {
#if 0
		printf("set_wait_intno:coreId=%d intno=%d : can not set waiting num=%d current_intno=%d\n",
				coreId, intno, intc_control.work[coreId].waiting_int_num,
				intc_control.work[coreId].current_intno);
#endif
	}
}
static void clr_wait_intno(CoreIdType core_id, int intno)
{
	if (intc_control.work[core_id].waiting_lvl_num[intno] > 0) {
		intc_control.work[core_id].waiting_lvl_num[intno]--;
		if (intc_control.work[core_id].waiting_lvl_num[intno] == 0) {
			intc_control.work[core_id].is_waiting_lvl[intno] = INTC_NUM_INTLVL;
		}
		intc_control.work[core_id].waiting_int_num--;
#if 0
		printf("clr_wait_intno:coreId=%d intno=%d lvl_num=%d is_waiting_lvl=%d\n",
				core_id, intno, intc_control.work[core_id].waiting_lvl_num[intno],
				intc_control.work[core_id].is_waiting_lvl[intno]);
#endif
	}
}

static int is_masked(TargetCoreType *cpu, uint32 intno)
{
	uint32 regaddr;
	uint32 *pregaddr;
	uint8 *xxIcn;
	uint8 wdata;

	regaddr = intc_regaddr_icn(intno);
	(void)intc_get_pointer(intc_region, cpu->core_id, (regaddr & intc_region->mask), (uint8**)&pregaddr);

	xxIcn = (uint8*)pregaddr;

	wdata = *xxIcn;
	return INTC_ICN_ISSET_MK(wdata);
}

static int get_maxpri_itno(TargetCoreType *cpu)
{
	int i;
	int next_intno = -1;
	uint32 tmp_lvl;
	uint32 minlvl;

	if (intc_control.work[cpu->core_id].waiting_int_num == 0) {
		return -1;
	}

	if (intc_control.work[cpu->core_id].current_intno >= 0) {
		/*
		 * 現在実行中のものよりも高い優先度をもつものを取得する
		 */
		minlvl = (((uint32)intc_control.work[cpu->core_id].is_waiting_lvl[intc_control.work[cpu->core_id].current_intno]) << 16) | ((INTC_DEFAULT_PRIORITY(intc_control.work[cpu->core_id].current_intno) << 0));
	}
	else {
		/*
		 * もっとも高い優先度をもつものを取得する
		 */
		minlvl = 0xFFFFFFFF;
	}
	for (i = 0; i < INTC_NUM; i++) {
		if (intc_control.work[cpu->core_id].is_waiting_lvl[i] == INTC_NUM_INTLVL) {
			continue;
		}
		if ((intc_control.work[cpu->core_id].current_intno > 0) && (intc_control.work[cpu->core_id].current_intno == i)) {
			/*
			 * すでに実行中のものはチェック対象外．
			 */
			continue;
		}
		/*
		 * マスクされているものは対象外とする
		 */
		if (is_masked(cpu, i) == TRUE) {
			//printf("cpu(%d) intno(%d) is masked\n", cpu->core_id, i);
			continue;
		}
		/*
		 * ディフォールトの優先度と同時比較するため，32ビットで評価する．
		 * ※ディフォールトの優先度は割り込み優先度よりも低い評価となるため，下位ビットに設定する
		 * ※値が小さい方が優先度が高いことに注意
		 *     優先度　　デフォルト優先度
		 * 最小：    0    |       0
		 * 最大：    7    |     115
		 *
		 */
		tmp_lvl = (((uint32)intc_control.work[cpu->core_id].is_waiting_lvl[i]) << 16) | ((INTC_DEFAULT_PRIORITY(i) << 0));

		//printf("tmplvl=%d minlvl=%d next_intno=%d intc_control.is_waiting_lvl[%d]=%d\n",
		//		tmp_lvl, minlvl, next_intno, i, intc_control.is_waiting_lvl[i]);
		if (tmp_lvl < minlvl) {
			minlvl = tmp_lvl;
			next_intno = i;
		}
	}
	return next_intno;
}

static void intc_raise(TargetCoreType *cpu, uint32 intno)
{
	uint32 wdata32;
	uint8 *ispr;
	uint32 *pregaddr;
	uint8 lvl;
	//printf("intc_raise INT(%d) enter\n", intno);

	(void)intc_get_pointer(intc_region, cpu->core_id, (INTC_REG_ISPR & intc_region->mask), (uint8**)&pregaddr);

	ispr = (uint8*)pregaddr;

	lvl = intc_control.work[cpu->core_id].is_waiting_lvl[intno];

	/*
	 * PSW.NP
	 */
	if (CPU_ISSET_NP(&cpu->reg) == TRUE) {
		//printf("can not raise INT(%d) because NP is set\n", intno);
		return;
	}
	/*
	 * PSW.ID
	 */
	if ((CPU_ISSET_ID(&cpu->reg) == TRUE)) {
		//printf("can not raise INT(%d) because ID is set, halt=%u\n", intno,
		//		intc_control.cpu->cores[CPU_CONFIG_CORE_ID_0].core.is_halt);
		return;
	}

	/*
	 * マスカブル割り込み要求発生
	 */
	if (intc_control.work[cpu->core_id].current_intno != -1) {
		intc_control.work[cpu->core_id].saved_intno_stack[intc_control.work[cpu->core_id].saved_intno_off] = intc_control.work[cpu->core_id].current_intno;
		intc_control.work[cpu->core_id].saved_intno_off++;
	}

	intc_control.work[cpu->core_id].current_intno = intno;
	//printf("intc_raise:current_intno_changed off=%u:%d\n", intc_control.saved_intno_off, intc_control.current_intno);

	sys_get_cpu_base(&cpu->reg)->r[SYS_REG_EIPC] = cpu->reg.pc;
	sys_get_cpu_base(&cpu->reg)->r[SYS_REG_EIPSW] = sys_get_cpu_base(&cpu->reg)->r[SYS_REG_PSW];
	wdata32 = (sys_get_cpu_base(&cpu->reg)->r[SYS_REG_ECR] & 0xFFFF0000);
	wdata32 |= INTC_MASK_ECR_CODE(intno);
	sys_get_cpu_base(&cpu->reg)->r[SYS_REG_ECR] = wdata32;

	CPU_SET_ID(&cpu->reg);
	CPU_CLR_EP(&cpu->reg);
	*ispr = (*ispr) | (1 << lvl);

	cpu->reg.pc = INTC_MASK_INTR_ADDR(intno);

	//printf("intc_raise:cpu(%d) intno(%d)\n", cpu->core_id, intno);
	//if (intno >= 20) {
	//	printf("RAISED INT(%d):waiting_num=%d\n", intno, intc_control.waiting_lvl_num[intno]);
	//}

	intc_control.cpu->cores[cpu->core_id].core.is_halt = FALSE;
	return;
}

static void common_raise_intr(uint32 intno, uint32 coreId)
{
	uint32 regaddr;
	uint32 *pregaddr;
	uint8 *xxIcn;
	uint8 wdata;
	uint8 lvl;

	regaddr = intc_regaddr_icn(intno);

	(void)intc_get_pointer(intc_region, coreId, (regaddr & intc_region->mask), (uint8**)&pregaddr);

	xxIcn = (uint8*)pregaddr;

	/*
	 * xxIF = 1
	 */
	wdata = *xxIcn;
	INTC_ICN_SET_IF(wdata);
	*xxIcn = wdata;
	lvl = INTC_ICN_PR(*xxIcn);

	/*
	 * 管理上，すべての割り込み要求はwaitingに入れておく．
	 */
	set_wait_intno(coreId, intno, lvl);

	intc_control.cpu->cores[coreId].core.is_halt = FALSE;
	return;
}

void intc_cpu_trigger_interrupt(CoreIdType core_id, int intno)
{
	uint32 triggered_coreId;

	triggered_coreId = (core_id == 0) ? 1 : 0;

	common_raise_intr(intno, triggered_coreId);
	return;
}

int intc_raise_intr(uint32 intno)
{
	uint32 coreId;

	for (coreId = 0; coreId < CPU_CONFIG_GET_CORE_ID_NUM(); coreId++) {
		common_raise_intr(intno, coreId);
	}
	return 0;
}
void intc_clr_currlvl_ispr(CoreIdType core_id)
{
	uint32 regaddr;
	uint32 *pregaddr;
	uint8 *ispr;
	uint8 *xxIcn;
	uint8 lvl;
	uint8 wdata;

	if (intc_control.work[core_id].current_intno == -1) {
		return;
	}

	(void)intc_get_pointer(intc_region, core_id, (INTC_REG_ISPR & intc_region->mask), (uint8**)&pregaddr);

	ispr = (uint8*)pregaddr;

	regaddr = intc_regaddr_icn(intc_control.work[core_id].current_intno);
	(void)intc_get_pointer(intc_region, core_id, (regaddr & intc_region->mask), (uint8**)&pregaddr);

	xxIcn = (uint8*)pregaddr;


	lvl = intc_control.work[core_id].is_waiting_lvl[intc_control.work[core_id].current_intno];
	*ispr = (*ispr) & ~(1 << lvl);

	/*
	 * ハードウェアマニュアル上は，Ｉｃｎのリセット契機が明記されていないが，
	 * 割込み終了契機でリセットすることにした．
	 */
	wdata = *xxIcn;
	INTC_ICN_CLR_IF(wdata);
	*xxIcn = wdata;


	/*
	 * 割込み終了タイミングでしか呼び出されないため，現在の割り込み情報はクリアする．
	 */
	clr_wait_intno(core_id, intc_control.work[core_id].current_intno);

	if (intc_control.work[core_id].saved_intno_off > 0) {
		intc_control.work[core_id].saved_intno_off--;
		intc_control.work[core_id].current_intno = intc_control.work[core_id].saved_intno_stack[intc_control.work[core_id].saved_intno_off];
		//printf("current_intno_changed1 off=%u:%d\n", intc_control.work[core_id].saved_intno_off, intc_control.work[core_id].current_intno);
	}
	else {
		intc_control.work[core_id].current_intno = -1;
		//printf("current_intno_changed2 off=%u:%d\n", intc_control.work[core_id].saved_intno_off, intc_control.work[core_id].current_intno);
	}
	return;
}

static bool can_raise_inwtdt2(TargetCoreType *cpu)
{
	return CPU_ISSET_NP(&cpu->reg);
}
static void raise_intwdt2(TargetCoreType *cpu)
{
	cpu->is_halt = FALSE;

	sys_get_cpu_base(&cpu->reg)->r[SYS_REG_FEPC] = 0x0;
	sys_get_cpu_base(&cpu->reg)->r[SYS_REG_FEPSW] = sys_get_cpu_base(&cpu->reg)->r[SYS_REG_PSW];

	sys_get_cpu_base(&cpu->reg)->r[SYS_REG_ECR] |= (INTC_FECC_INTWDT2 << 16);

	CPU_SET_NP(&cpu->reg);
	CPU_CLR_EP(&cpu->reg);
	CPU_SET_ID(&cpu->reg);

	cpu->reg.pc = INTC_FECC_INTWDT2;

	//printf("RAISED INTWDT2\n");
	return;
}
static void raise_nmi(TargetCoreType *cpu)
{
	if (CPU_ISSET_NP(&cpu->reg) == TRUE) {
		return;
	}
	cpu->is_halt = FALSE;

	sys_get_cpu_base(&cpu->reg)->r[SYS_REG_FEPC] = cpu->reg.pc;
	sys_get_cpu_base(&cpu->reg)->r[SYS_REG_FEPSW] = sys_get_cpu_base(&cpu->reg)->r[SYS_REG_PSW];

	sys_get_cpu_base(&cpu->reg)->r[SYS_REG_ECR] |= (INTC_FECC_NMI << 16);

	CPU_SET_NP(&cpu->reg);
	CPU_CLR_EP(&cpu->reg);
	CPU_SET_ID(&cpu->reg);


	cpu->reg.pc = INTC_FECC_NMI;

	//printf("RAISED NMI\n");
	return;
}

/*
 *
 * 現在実行中およびペンディング中の割込みを全てチェックして，
 * 最高優先度のものを実行する．
 */
static void intc_raise_pending_intr(TargetCoreType *cpu)
{
	/*
	 * INTWDT2割込みチェック
	 */
	if (intc_control.nmi.intwdt2_hasreq == TRUE) {
		if (can_raise_inwtdt2(cpu) == FALSE) {
			/*
			 * 新たな要求受付は禁止するため処理終了．
			 */
			return;
		}
		raise_intwdt2(cpu);
		return;
	}
	/*
	 * NMI割込みチェック
	 */
	if (intc_control.nmi.nmi_reqnum > 0) {
		raise_nmi(cpu);
		/*
		 * NMI割込みありの場合は，マスカブル割り込み受付は禁止するため処理終了．
		 */
		return;
	}

	/*
	 * マスカブル割り込みチェック
	 */
	int maxlvl_intno;

	maxlvl_intno = get_maxpri_itno(cpu);

	if (maxlvl_intno < 0) {
		//printf("not found intno\n");
		return;
	}
	if (maxlvl_intno == intc_control.work[cpu->core_id].current_intno) {
		//printf("can not raise INT(%d) because already intr is set\n", maxlvl_intno);
		return;
	}

	intc_raise(cpu, maxlvl_intno);
	return;
}

int intc_raise_nmi(TargetCoreType *cpu, uint32 nmino)
{
	if (nmino == INTC_NMINO_INTWDT2) {
		intc_control.nmi.intwdt2_hasreq = TRUE;
	}
	else {
		intc_control.nmi.nmi_reqnum++;
	}
	return 0;
}

void intc_clr_nmi(TargetCoreType *cpu)
{
	if (intc_control.nmi.intwdt2_hasreq == TRUE) {
		cpu_reset(cpu->core_id);
	}
	else {
		intc_control.nmi.nmi_reqnum--;
	}
	return;
}

static int get_imrno(uint32 regaddr)
{
	int imrno = -1;
	switch (regaddr) {
	case MASK_INTC_ADDR(INTC_REG_IMR0):
		imrno = 0;
		break;
	case MASK_INTC_ADDR(INTC_REG_IMR1):
		imrno = 1;
		break;
	case MASK_INTC_ADDR(INTC_REG_IMR2):
		imrno = 2;
		break;
	case MASK_INTC_ADDR(INTC_REG_IMR3):
		imrno = 3;
		break;
	case MASK_INTC_ADDR(INTC_REG_IMR4):
		imrno = 4;
		break;
	case MASK_INTC_ADDR(INTC_REG_IMR5):
		imrno = 5;
		break;
	case MASK_INTC_ADDR(INTC_REG_IMR6):
		imrno = 6;
		break;
	case MASK_INTC_ADDR(INTC_REG_IMR7):
		imrno = 7;
		break;
	default:
		break;
	}
	return imrno;
}
static int get_xxICn(uint32 regaddr)
{
	if (regaddr < MASK_INTC_ADDR(INTC_REG_xxICnStr)) {
		return -1;
	}
	else if (regaddr > MASK_INTC_ADDR(INTC_REG_xxICnEnd)) {
		return -1;
	}
	return (regaddr - MASK_INTC_ADDR(INTC_REG_xxICnStr)) / 2;
}

static void intc_hook_update_reg8(CoreIdType core_id, uint32 regaddr, uint8 data)
{
	int imrno;
	uint8 *xxICn;
	uint32 regaddr32;
	uint32 *pregaddr32;
	int i;
	int intno;

	//printf("intc_hook_update_reg8:regaddr=0x%x data=0x%x\n", regaddr, data);
	imrno = get_imrno(regaddr);
	if (imrno >= 0) {
		intno = imrno * 16;
		for (i = 0; i < 8; i++, intno++) {
			if (intno == INTC_PINTNO_NOUSE) {
				continue;
			}
			if (intno > INTC_INTNO_MAX) {
				continue;
			}
			regaddr32 = intc_regaddr_icn(intno);
			(void)intc_get_pointer(intc_region, core_id, (regaddr32 & intc_region->mask), (uint8**)&pregaddr32);

			xxICn = (uint8*)pregaddr32;
			if (data & 1 << i) {
				*xxICn |= (1 << 6);
				//printf("xxIcn(intno=%d):set\n", intno);
			}
			else {
				*xxICn &= ~(1 << 6);
				//printf("xxIcn(intno=%d):clear\n", intno);
				//clr_wait_intno(pintno);
			}
		}
	}
	else {
		intno = get_xxICn(regaddr);
		//printf("intc_hook_update_reg8:regaddr=0x%x %d\n", regaddr, intno);
		if (intno >= 0) {
			intc_clr_currlvl_ispr(core_id);
			//printf("xxIcn(intno=%d):clear\n", intno);
		}
	}
	return;
}
static void intc_hook_update_reg16(CoreIdType core_id, uint32 regaddr, uint16 data)
{
	int imrno;
	uint8 *xxICn;
	uint32 regaddr32;
	uint32 *pregaddr32;
	int i;
	int intno;

	imrno = get_imrno(regaddr);
	if (imrno < 0) {
		return;
	}
	//printf("intc_hook_update_reg16:regaddr=0x%x data=0x%x\n", regaddr, data);
	intno = imrno * 16;
	for (i = 0; i < 16; i++, intno++) {
		if (intno == INTC_PINTNO_NOUSE) {
			continue;
		}
		if (intno > INTC_INTNO_MAX) {
			continue;
		}
		regaddr32 = intc_regaddr_icn(intno);
		(void)intc_get_pointer(intc_region, core_id, (regaddr32 & intc_region->mask), (uint8**)&pregaddr32);

		xxICn = (uint8*)pregaddr32;
		if (data & 1 << i) {
			*xxICn |= (1 << 6);
			//printf("xxIcn(intno=%d):set\n", intno);
		}
		else {
			*xxICn &= ~(1 << 6);
			//printf("xxIcn(intno=%d):clear\n", intno);
		}
	}
	return;
}


static Std_ReturnType intc_get_data8(MpuAddressRegionType *region, CoreIdType core_id, uint32 addr, uint8 *data)
{
	uint32 off = (addr - region->start);
	uint8 *datap = &region->data[off + (core_id * (region->size))];
	*data = *((uint8*)(datap));
	return STD_E_OK;
}
static Std_ReturnType intc_get_data16(MpuAddressRegionType *region, CoreIdType core_id, uint32 addr, uint16 *data)
{
	uint32 off = (addr - region->start);
	uint8 *datap = &region->data[off + (core_id * (region->size))];
	*data = *((uint16*)(datap));

	return STD_E_OK;
}
static Std_ReturnType intc_get_data32(MpuAddressRegionType *region, CoreIdType core_id, uint32 addr, uint32 *data)
{
	uint32 off = (addr - region->start);
	uint8 *datap = &region->data[off + (core_id * (region->size))];

	*data = *((uint32*)(datap));
	return STD_E_OK;
}
static Std_ReturnType intc_put_data8(MpuAddressRegionType *region, CoreIdType core_id, uint32 addr, uint8 data)
{
	uint32 off = (addr - region->start);
	uint8 *datap = &region->data[off + (core_id * (region->size))];

	intc_hook_update_reg8(core_id, addr, data);
	*((uint8*)(datap)) = data;

	return STD_E_OK;
}
static Std_ReturnType intc_put_data16(MpuAddressRegionType *region, CoreIdType core_id, uint32 addr, uint16 data)
{
	uint32 off = (addr - region->start);
	uint8 *datap = &region->data[off + (core_id * (region->size))];

	intc_hook_update_reg16(core_id, addr, data);

	*((uint16*)(datap)) = data;
	return STD_E_OK;
}
static Std_ReturnType intc_put_data32(MpuAddressRegionType *region, CoreIdType core_id, uint32 addr, uint32 data)
{
	uint32 off = (addr - region->start);
	uint8 *datap = &region->data[off + (core_id * (region->size))];

	*((uint32*)(datap)) = data;
	return STD_E_OK;
}
static Std_ReturnType intc_get_pointer(MpuAddressRegionType *region, CoreIdType core_id, uint32 addr, uint8 **data)
{
	uint32 off = (addr - region->start);
	uint8 *datap = &region->data[off + (core_id * (region->size))];

	*data = datap;
	return STD_E_OK;
}

