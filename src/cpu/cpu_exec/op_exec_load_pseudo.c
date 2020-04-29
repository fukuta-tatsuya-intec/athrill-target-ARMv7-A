#include "arm_pseudo_code_func.h"

static inline int mem_to_reg(TargetCoreType *core, uint32 address, uint32 size, uint32 *result)
{
	int ret = 0;
	uint8 data[4];
	ret = MemU_R(core, address, size, data);
	if (ret != 0) {
		return -1;
	}
	switch (size) {
		case 1:
			*result = 	(uint32)data[0];
			break;
		case 2:
			*result =	((uint32)data[0] << 0U) |
						((uint32)data[1] << 8U) ;
			break;
		case 4:
			*result =	((uint32)data[0] << 0U)  |
						((uint32)data[1] << 8U)  |
						((uint32)data[2] << 16U) |
						((uint32)data[3] << 24U) ;
			break;
		default:
			ret = -1;
			break;
	}
	return ret;
}

int arm_op_exec_arm_ldr_imm(struct TargetCore *core,  arm_ldr_imm_input_type *in, arm_ldr_imm_output_type *out)
{
	int ret = 0;
	uint32 result;
	uint32 *status = cpu_get_status(core);
	out->next_address = core->pc + INST_ARM_SIZE;
	out->passed = ConditionPassed(in->cond, *status);
	if (out->passed != FALSE) {
		//offset_addr = if add then (R[n] + imm32) else (R[n] - imm32);
		uint32 offset_addr = (in->add) ? (in->Rn.regData + in->imm32) : (in->Rn.regData - in->imm32);
		//address = if index then offset_addr else R[n];
		uint32 address = (in->index) ? offset_addr : in->Rn.regData;
		ret = mem_to_reg(core, address, 4, &result);
		if (ret != 0) {
			printf("ERROR:arm_op_exec_arm_ldr_imm():ret=%d size=%d addr=0x%x\n",
					ret, 4, address);
			goto done;
		}
		if (in->Rt.regId == CpuRegId_PC) {
			//if t == 15 then
			//if address<1:0> == ‘00’ then LoadWritePC(data); else UNPREDICTABLE;
			if ((address & 0x3) == 0x0) {
				ret = LoadWritePC(&out->next_address, status, result);
			}
			else {
				//UNPREDICTABLE
				ret = -1;
			}
		}
		else if (UnalignedSupport() || ((address & 0x3) == 0x00)) {
			//elsif UnalignedSupport() || address<1:0> == ‘00’ then
			cpu_set_reg(core, in->Rt.regId, result);
			//R[t] = data;
		}
		else {
			//else // Can only apply before ARMv7
			//R[t] = ROR(data, 8*UInt(address<1:0>));
			result = ROR(32, result, 8 * UInt((address & 0x3)) );
			cpu_set_reg(core, in->Rt.regId, result);
		}
		//if wback then R[n] = offset_addr;
		if ((ret == 0) && in->wback) {
			cpu_set_reg(core, in->Rn.regId, offset_addr);
        	out->Rn.regData = offset_addr;
		}
	}
done:
	out->status = *status;
	return ret;
}

int arm_op_exec_arm_ldr_literal(struct TargetCore *core,  arm_ldr_literal_input_type *in, arm_ldr_literal_output_type *out)
{
	int ret = 0;
	uint32 result;
	uint32 *status = cpu_get_status(core);
	out->next_address = core->pc + INST_ARM_SIZE;
	out->passed = ConditionPassed(in->cond, *status);
	if (out->passed != FALSE) {
		uint32 base = Align(in->PC.regData, 4);
		uint32 address = (in->add) ? (base + in->imm32) : (base - in->imm32);
		ret = mem_to_reg(core, address, 4, &result);
		if (ret != 0) {
			goto done;
		}
		if (in->Rt.regId == CpuRegId_PC) {
			if ((address & 0x3) == 0x0) {
				ret = LoadWritePC(&out->next_address, status, result);
			}
			else {
				//UNPREDICTABLE
				ret = -1;
			}
		}
		else if (UnalignedSupport() || ((address & 0x3) == 0x00)) {
			cpu_set_reg(core, in->Rt.regId, result);
		}
		else {
			result = ROR(32, result, 8 * UInt((address & 0x3)) );
			cpu_set_reg(core, in->Rt.regId, result);
		}
		out->Rt.regData = result;
	}
done:
	out->status = *status;
	return ret;
}
int arm_op_exec_arm_ldrd_imm(struct TargetCore *core,  arm_ldrd_imm_input_type *in, arm_ldrd_imm_output_type *out)
{
	int ret = 0;
	uint32 result1;
	uint32 result2;
	uint32 *status = cpu_get_status(core);
	uint32 offset_addr = (in->add) ? (in->Rn.regData + in->imm32) : (in->Rn.regData - in->imm32);
	uint32 address = (in->index) ? offset_addr : in->Rn.regData;
	out->next_address = core->pc + INST_ARM_SIZE;
	out->passed = ConditionPassed(in->cond, *status);
	if (out->passed != FALSE) {
		ret = mem_to_reg(core, address, 4, &result1);
		if (ret != 0) {
			goto done;
		}
		ret = mem_to_reg(core, address + 4, 4, &result2);
		if (ret != 0) {
			goto done;
		}
		if (CPU_STATUS_BIT_IS_SET(*status, CPU_STATUS_BITPOS_E)) {
			//R[t] = data<63:32>;
			//R[t2] = data<31:0>;
			cpu_set_reg(core, in->Rt1.regId, result2);
			cpu_set_reg(core, in->Rt2.regId, result1);
        	out->Rt1.regData = result2;
        	out->Rt2.regData = result1;
		}
		else {
			//R[t] = MemA[address,4];
			//R[t2] = MemA[address+4,4];
			cpu_set_reg(core, in->Rt1.regId, result1);
			cpu_set_reg(core, in->Rt2.regId, result2);
        	out->Rt1.regData = result1;
        	out->Rt2.regData = result2;
		}
		if ((ret == 0) && in->wback) {
			cpu_set_reg(core, in->Rn.regId, offset_addr);
        	out->Rn.regData = offset_addr;
		}
	}
done:
	out->status = *status;
	return ret;
}

int arm_op_exec_arm_ldrb_imm(struct TargetCore *core,  arm_ldrb_imm_input_type *in, arm_ldrb_imm_output_type *out)
{
	int ret = 0;
	uint32 result;
	uint32 *status = cpu_get_status(core);
	//offset_addr = if add then (R[n] + imm32) else (R[n] - imm32);
	uint32 offset_addr = (in->add) ? (in->Rn.regData + in->imm32) : (in->Rn.regData - in->imm32);
	//address = if index then offset_addr else R[n];
	uint32 address = (in->index) ? offset_addr : in->Rn.regData;
	out->next_address = core->pc + INST_ARM_SIZE;
	out->passed = ConditionPassed(in->cond, *status);
	if (out->passed != FALSE) {
		//R[t] = ZeroExtend(MemU[address,1], 32);
		ret = mem_to_reg(core, address, 1, &result);
		if (ret != 0) {
			goto done;
		}
    	out->Rt.regData = (uint32)((uint8)result);
		cpu_set_reg(core, in->Rt.regId, out->Rt.regData);
		if ((ret == 0) && in->wback) {
			cpu_set_reg(core, in->Rn.regId, offset_addr);
        	out->Rn.regData = offset_addr;
		}
	}
done:
	out->status = *status;
	return ret;
}
int arm_op_exec_arm_ldrh_imm(struct TargetCore *core,  arm_ldrh_imm_input_type *in, arm_ldrh_imm_output_type *out)
{
	int ret = 0;
	uint32 result;
	uint32 *status = cpu_get_status(core);
	uint32 offset_addr = (in->add) ? (in->Rn.regData + in->imm32) : (in->Rn.regData - in->imm32);
	uint32 address = (in->index) ? offset_addr : in->Rn.regData;
	out->next_address = core->pc + INST_ARM_SIZE;
	out->passed = ConditionPassed(in->cond, *status);
	if (out->passed != FALSE) {
		ret = mem_to_reg(core, address, 2, &result);
		if (ret != 0) {
			goto done;
		}
		//R[t] = ZeroExtend(data, 32);
    	out->Rt.regData = (uint32)((uint16)result);
		cpu_set_reg(core, in->Rt.regId, out->Rt.regData);
		if ((ret == 0) && in->wback) {
			cpu_set_reg(core, in->Rn.regId, offset_addr);
        	out->Rn.regData = offset_addr;
		}
	}
done:
	out->status = *status;
	return ret;
}

int arm_op_exec_arm_ldrsh_imm(struct TargetCore *core,  arm_ldrsh_imm_input_type *in, arm_ldrsh_imm_output_type *out)
{
	int ret = 0;
	uint32 result;
	uint32 *status = cpu_get_status(core);
	uint32 offset_addr = (in->add) ? (in->Rn.regData + in->imm32) : (in->Rn.regData - in->imm32);
	uint32 address = (in->index) ? offset_addr : in->Rn.regData;
	out->next_address = core->pc + INST_ARM_SIZE;
	out->passed = ConditionPassed(in->cond, *status);
	if (out->passed != FALSE) {
		ret = mem_to_reg(core, address, 2, &result);
		if (ret != 0) {
			goto done;
		}
		//R[t] = SignExtend(data, 32);
    	out->Rt.regData = (sint32)((sint16)result);
		cpu_set_reg(core, in->Rt.regId, out->Rt.regData);
		if ((ret == 0) && in->wback) {
			cpu_set_reg(core, in->Rn.regId, offset_addr);
        	out->Rn.regData = offset_addr;
		}
	}
done:
	out->status = *status;
	return ret;
}


int arm_op_exec_arm_ldrsh_reg(struct TargetCore *core,  arm_ldrsh_reg_input_type *in, arm_ldrsh_reg_output_type *out)
{
	int ret = 0;
	uint32 result;
	uint32 *status = cpu_get_status(core);
	uint32 offset = Shift(32, in->Rm.regData, in->shift_t, in->shift_n, CPU_ISSET_CY(status));
	uint32 offset_addr = (in->add) ? (in->Rn.regData + offset) : (in->Rn.regData - offset);
	uint32 address = (in->index) ? offset_addr : in->Rn.regData;
	out->next_address = core->pc + INST_ARM_SIZE;
	out->passed = ConditionPassed(in->cond, *status);
	if (out->passed != FALSE) {
		ret = mem_to_reg(core, address, 2, &result);
		if (ret != 0) {
			goto done;
		}
		//R[t] = SignExtend(data, 32);
    	out->Rt.regData = (sint32)((sint16)result);
		cpu_set_reg(core, in->Rt.regId, out->Rt.regData);
		if ((ret == 0) && in->wback) {
			cpu_set_reg(core, in->Rn.regId, offset_addr);
        	out->Rn.regData = offset_addr;
		}
	}
done:
	out->status = *status;
	return ret;
}

int arm_op_exec_arm_ldr_reg(struct TargetCore *core,  arm_ldr_reg_input_type *in, arm_ldr_reg_output_type *out)
{
	int ret = 0;
	uint32 result;
	uint32 *status = cpu_get_status(core);
	out->next_address = core->pc + INST_ARM_SIZE;
	out->passed = ConditionPassed(in->cond, *status);
	if (out->passed != FALSE) {
		//offset = Shift(R[m], shift_t, shift_n, APSR.C);
		uint32 offset = Shift(32, in->Rm.regData, in->shift_t, in->shift_n, CPU_ISSET_CY(status));
		//offset_addr = if add then (R[n] + offset) else (R[n] - offset);
		uint32 offset_addr = (in->add) ? (in->Rn.regData + offset) : (in->Rn.regData - offset);
		//address = if index then offset_addr else R[n];
		uint32 address = (in->index) ? offset_addr : in->Rn.regData;
		ret = mem_to_reg(core, address, 4, &result);
		if (ret != 0) {
			goto done;
		}
		if (in->Rt.regId == CpuRegId_PC) {
			//if t == 15 then
			//if address<1:0> == ‘00’ then LoadWritePC(data); else UNPREDICTABLE;
			if ((address & 0x3) == 0x0) {
				ret = LoadWritePC(&out->next_address, status, result);
			}
			else {
				//UNPREDICTABLE
				ret = -1;
			}
		}
		else if (UnalignedSupport() || ((address & 0x3) == 0x00)) {
			//elsif UnalignedSupport() || address<1:0> == ‘00’ then
			cpu_set_reg(core, in->Rt.regId, result);
			//R[t] = data;
		}
		else {
			//else // Can only apply before ARMv7
			//R[t] = ROR(data, 8*UInt(address<1:0>));
			result = ROR(32, result, 8 * UInt((address & 0x3)) );
			cpu_set_reg(core, in->Rt.regId, result);
		}
		//if wback then R[n] = offset_addr;
		if ((ret == 0) && in->wback) {
			cpu_set_reg(core, in->Rn.regId, offset_addr);
        	out->Rn.regData = offset_addr;
		}
	}
done:
	out->status = *status;
	return ret;
}
int arm_op_exec_arm_ldrb_reg(struct TargetCore *core,  arm_ldrb_reg_input_type *in, arm_ldrb_reg_output_type *out)
{
	int ret = 0;
	uint32 result;
	uint32 *status = cpu_get_status(core);
	uint32 offset = Shift(32, in->Rm.regData, in->shift_t, in->shift_n, CPU_ISSET_CY(status));
	uint32 offset_addr = (in->add) ? (in->Rn.regData + offset) : (in->Rn.regData - offset);
	uint32 address = (in->index) ? offset_addr : in->Rn.regData;
	out->next_address = core->pc + INST_ARM_SIZE;
	out->passed = ConditionPassed(in->cond, *status);
	if (out->passed != FALSE) {
		ret = mem_to_reg(core, address, 1, &result);
		if (ret != 0) {
			goto done;
		}
		//R[t] = ZeroExtend(MemU[address,1],32);
    	out->Rt.regData = (sint32)((uint8)result);
		cpu_set_reg(core, in->Rt.regId, out->Rt.regData);
		if ((ret == 0) && in->wback) {
			cpu_set_reg(core, in->Rn.regId, offset_addr);
        	out->Rn.regData = offset_addr;
		}
	}
done:
	out->status = *status;
	return ret;
}
int arm_op_exec_arm_ldrh_reg(struct TargetCore *core,  arm_ldrh_reg_input_type *in, arm_ldrh_reg_output_type *out)
{
	int ret = 0;
	uint32 result;
	uint32 *status = cpu_get_status(core);
	uint32 offset = Shift(32, in->Rm.regData, in->shift_t, in->shift_n, CPU_ISSET_CY(status));
	uint32 offset_addr = (in->add) ? (in->Rn.regData + offset) : (in->Rn.regData - offset);
	uint32 address = (in->index) ? offset_addr : in->Rn.regData;
	out->next_address = core->pc + INST_ARM_SIZE;
	out->passed = ConditionPassed(in->cond, *status);
	if (out->passed != FALSE) {
		ret = mem_to_reg(core, address, 2, &result);
		if (ret != 0) {
			goto done;
		}
		//R[t] = ZeroExtend(data, 32);
    	out->Rt.regData = (sint32)((uint16)result);
		cpu_set_reg(core, in->Rt.regId, out->Rt.regData);
		if ((ret == 0) && in->wback) {
			cpu_set_reg(core, in->Rn.regId, offset_addr);
        	out->Rn.regData = offset_addr;
		}
	}
done:
	out->status = *status;
	return ret;
}

int arm_op_exec_arm_pop(struct TargetCore *core,  arm_pop_input_type *in, arm_pop_output_type *out)
{
	int ret = 0;
	uint32 *status = cpu_get_status(core);
	out->next_address = core->pc + INST_ARM_SIZE;
	out->passed = ConditionPassed(in->cond, *status);
	if (out->passed != FALSE) {
		int i;
		uint32 address = (uint32)in->SP.regData;
		for (i = 0; i <= CpuRegId_PC; i++) {
			uint32 data;
			if ( ((1U << i) & in->registers) != 0 ) {
				//R[i] = if UnalignedAllowed then MemU[address,4] else MemA[address,4];
				if (in->UnalignedAllowed == TRUE) {
					ret = MemA_R(core, address, 4, (uint8*)&data);
				}
				else {
					ret = MemA_R(core, address, 4, (uint8*)&data);
				}
				if (ret < 0) {
					goto done;
				}
				if (i != CpuRegId_PC) {
					cpu_set_reg(core, i, data);
				}
				else {
					if (in->UnalignedAllowed == TRUE) {
						if ((address & 0x3) == 0) {
							LoadWritePC(&out->next_address, status, data);
						}
						else {
							//UNPREDICTABLE;
							ret = -1;
						}
					}
					else {
						LoadWritePC(&out->next_address, status, data);
					}
				}
				address = address + 4;
			}
		}
		if ( (in->registers & (1U << 13)) == 0 ) {
			out->SP.regData = (in->SP.regData + (4 * (in->bitcount)));
			cpu_set_reg(core, out->SP.regId, out->SP.regData);
		}
		else {
			//UNPREDICTABLE;
			ret = -1;
		}
	}
done:
	out->status = *status;
    return ret;
}


int arm_op_exec_arm_ldm(struct TargetCore *core,  arm_ldm_input_type *in, arm_ldm_output_type *out)
{
	int ret = 0;
	uint32 *status = cpu_get_status(core);
	out->next_address = core->pc + INST_ARM_SIZE;
	out->passed = ConditionPassed(in->cond, *status);
	if (out->passed != FALSE) {
		int i;
		uint32 data;
		uint32 address = (uint32)in->Rn.regData;
		for (i = 0; i < CpuRegId_PC; i++) {
			if ( ((1U << i) & in->registers) != 0 ) {
				ret = MemA_R(core, address, 4, (uint8*)&data);
				if (ret < 0) {
					goto done;
				}
				//R[i] = MemA[address,4]; address = address + 4;
				cpu_set_reg(core, i, data);
				address = address + 4;
			}
		}
		if ( (in->registers & (1U << CpuRegId_PC)) != 0 ) {
			//LoadWritePC(MemA[address,4]);
			ret = MemA_R(core, address, 4, (uint8*)&data);
			if (ret < 0) {
				goto done;
			}
			LoadWritePC(&out->next_address, status, data);
		}
		//if wback && registers<n> == ‘0’ then R[n] = R[n] + 4*BitCount(registers);
		//if wback && registers<n> == ‘1’ then R[n] = bits(32) UNKNOWN
		if (in->wback && ((in->registers & (1U << in->Rn.regId)) == 0 )) {
			//R[n] = R[n] + 4*BitCount(registers);
			out->Rn.regData += 4 * in->bitcount;
			cpu_set_reg(core, out->Rn.regId, out->Rn.regData);
		}
	}
done:
	out->status = *status;
    return ret;
}

int arm_op_exec_arm_ldmib(struct TargetCore *core,  arm_ldmib_input_type *in, arm_ldmib_output_type *out)
{
	int ret = 0;
	uint32 *status = cpu_get_status(core);
	out->next_address = core->pc + INST_ARM_SIZE;
	out->passed = ConditionPassed(in->cond, *status);
	if (out->passed != FALSE) {
		int i;
		uint32 data;
		//address = R[n] + 4;
		uint32 address = (uint32)in->Rn.regData + 4;
		for (i = 0; i < CpuRegId_PC; i++) {
			if ( ((1U << i) & in->registers) != 0 ) {
				ret = MemA_R(core, address, 4, (uint8*)&data);
				if (ret < 0) {
					goto done;
				}
				//R[i] = MemA[address,4]; address = address + 4;
				cpu_set_reg(core, i, data);
				address = address + 4;
			}
		}
		if ( (in->registers & (1U << CpuRegId_PC)) != 0 ) {
			//LoadWritePC(MemA[address,4]);
			ret = MemA_R(core, address, 4, (uint8*)&data);
			if (ret < 0) {
				goto done;
			}
			LoadWritePC(&out->next_address, status, data);
		}
		//if wback && registers<n> == ‘0’ then R[n] = R[n] + 4*BitCount(registers);
		//if wback && registers<n> == ‘1’ then R[n] = bits(32) UNKNOWN;
		if (in->wback && ((in->registers & (1U << in->Rn.regId)) == 0 )) {
			//R[n] = R[n] + 4*BitCount(registers);
			out->Rn.regData += 4 * in->bitcount;
			cpu_set_reg(core, out->Rn.regId, out->Rn.regData);
		}
	}
done:
	out->status = *status;
	return ret;
}
