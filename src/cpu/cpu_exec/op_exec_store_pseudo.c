#include "arm_pseudo_code_func.h"

static inline int reg_to_mem(TargetCoreType *core, uint32 address, uint32 size, uint32 regdata)
{
	struct {
		uint8 array[4];
	} data;
	switch (size) {
		case 1:
			data.array[0] = ((uint8)((regdata >> 0U) & 0xFF));
			break;
		case 2:
			data.array[0] = ((uint8)((regdata >> 0U) & 0xFF));
			data.array[1] = ((uint8)((regdata >> 8U) & 0xFF));
			break;
		case 4:
			data.array[0] = ((uint8)((regdata >>  0U) & 0xFF));
			data.array[1] = ((uint8)((regdata >>  8U) & 0xFF));
			data.array[2] = ((uint8)((regdata >> 16U) & 0xFF));
			data.array[3] = ((uint8)((regdata >> 24U) & 0xFF));
			break;
		default:
			return -1;
	}
	return MemA_W(core, address, size, (uint8*)&data);
}

int arm_op_exec_arm_str_imm(struct TargetCore *core,  arm_str_imm_input_type *in, arm_str_imm_output_type *out)
{
	int ret = 0;
	uint32 regdata;
	uint32 *status = cpu_get_status(core);
	uint32 offset_addr = (in->add) ? (in->Rn.regData + in->imm32) : (in->Rn.regData - in->imm32);
	uint32 address = (in->index) ? offset_addr : in->Rn.regData;
	out->next_address = core->pc + INST_ARM_SIZE;
	out->passed = ConditionPassed(in->cond, *status);
	if (out->passed != FALSE) {
		if (in->Rt.regId == CpuRegId_PC) {
			regdata = PCStoreValue(core);
		}
		else {
			regdata = in->Rt.regData;
		}
		ret = reg_to_mem(core, address, in->size, regdata);
		if ((ret == 0) && in->wback) {
			cpu_set_reg(core, in->Rn.regId, offset_addr);
        	out->Rn.regData = offset_addr;
		}
		else if (ret != 0) {
			printf("ERROR:arm_op_exec_arm_str_imm():ret=%d size=%d addr=0x%x data=0x%x\n",
					ret, in->size, address, regdata);
		}
	}
	out->status = *status;
    return ret;
}


int arm_op_exec_arm_str_reg(struct TargetCore *core,  arm_str_reg_input_type *in, arm_str_reg_output_type *out)
{
	int ret = 0;
	uint32 regdata;
	uint32 *status = cpu_get_status(core);
	uint32 offset = Shift(32, in->Rm.regData, in->shift_t, in->shift_n, CPU_ISSET_CY(status));
	uint32 offset_addr = (in->add) ? (in->Rn.regData + offset) : (in->Rn.regData - offset);
	uint32 address = (in->index) ? offset_addr : in->Rn.regData;
	out->next_address = core->pc + INST_ARM_SIZE;
	out->passed = ConditionPassed(in->cond, *status);
	if (out->passed != FALSE) {
		regdata = cpu_get_reg(core, in->Rt.regId);
		ret = reg_to_mem(core, address, in->size, regdata);
		if ((ret == 0) && in->wback) {
			cpu_set_reg(core, in->Rn.regId, offset_addr);
		}
	}
	out->status = *status;
    return ret;
}


int arm_op_exec_arm_push(struct TargetCore *core,  arm_push_input_type *in, arm_push_output_type *out)
{
	int ret = 0;
	uint32 *status = cpu_get_status(core);
	out->next_address = core->pc + INST_ARM_SIZE;
	out->passed = ConditionPassed(in->cond, *status);
	if (out->passed != FALSE) {
		int i;
		uint32 address = (uint32)in->SP.regData - (4 * (in->bitcount));
		uint32 lowestbit = LowestSetBit(8, in->registers);
		for (i = 0; i <= CpuRegId_PC; i++) {
			uint32 data = cpu_get_reg(core, i);
			if ( ((1U << i) & in->registers) != 0 ) {
				if ((i == 13) && (i != lowestbit)) {
					// Only possible for encoding A1
					//MemA[address,4] = bits(32) UNKNOWN;
					ret = MemA_W(core, address, 4, (uint8*)&data);
				}
				else {
					if (in->UnalignedAllowed == TRUE) {
						//MemU[address,4] = R[i]; TODO
						ret = MemA_W(core, address, 4, (uint8*)&data);
					}
					else {
						//MemA[address,4] = R[i];
						ret = MemA_W(core, address, 4, (uint8*)&data);
					}
				}
				if (ret < 0) {
					goto done;
				}
				address = address + 4;
			}
		}
		out->SP.regData = (in->SP.regData - (4 * (in->bitcount)));		
	}
done:
	out->status = *status;
    return ret;
}
