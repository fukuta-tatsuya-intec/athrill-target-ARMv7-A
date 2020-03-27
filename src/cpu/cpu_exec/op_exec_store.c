#include "cpu_dec/arm_mcdecoder.h"
#include "arm_pseudo_code_debug.h"
#include "arm_pseudo_code_func.h"

int arm_op_exec_arm_str_imm_a1(struct TargetCore *core)
{
	arm_OpCodeFormatType_arm_str_imm_a1 *op = &core->decoded_code->code.arm_str_imm_a1;

	arm_str_imm_input_type in;
	arm_str_imm_output_type out;
	out.status = *cpu_get_status(core);

	in.instrName = "STR";
	in.cond = op->cond;
	in.imm32 = (uint32)(op->imm12);
	in.index = (op->P != 0);
	in.add = (op->U != 0);
	in.wback = ((op->P == 0) || (op->W != 0));
	in.size = 4U;
	OP_SET_REG(core, &in, op, Rn);
	OP_SET_REG(core, &in, op, Rt);

	if (in.wback && ((op->Rn == CpuRegId_PC) || (op->Rn == op->Rt))) {
		//if wback && (n == 15 || n == t) then UNPREDICTABLE;
		//TODO
		return -1;
	}

	out.next_address = core->pc;
	out.passed = FALSE;
	OP_SET_REG(core, &out, op, Rn);
	
	int ret = arm_op_exec_arm_str_imm(core, &in, &out);
	DBG_ARM_STR_IMM(core, &in, &out);

	core->pc = out.next_address;
	return ret;
}


int arm_op_exec_arm_strb_imm_a1(struct TargetCore *core)
{
	arm_OpCodeFormatType_arm_strb_imm_a1 *op = &core->decoded_code->code.arm_strb_imm_a1;

	arm_str_imm_input_type in;
	arm_str_imm_output_type out;
	out.status = *cpu_get_status(core);

	in.instrName = "STRB";

	in.cond = op->cond;
	in.imm32 = (uint32)(op->imm12);
	in.index = (op->P != 0);
	in.add = (op->U != 0);
	in.wback = ((op->P == 0) || (op->W != 0));
	in.size = 1U;
	OP_SET_REG(core, &in, op, Rn);
	OP_SET_REG(core, &in, op, Rt);

	//if P == ‘0’ && W == ‘1’ then SEE STRBT;
	if ((op->P == 0) && (op->W != 0)) {
		//TODO
		return -1;
	}
	if (op->Rt == CpuRegId_PC) {
		//UNPREDICTABLE
		return -1;
	}

	if (in.wback && ((op->Rn == CpuRegId_PC) || (op->Rn == op->Rt))) {
		//if wback && (n == 15 || n == t) then UNPREDICTABLE;
		//TODO
		return -1;
	}

	out.next_address = core->pc;
	out.passed = FALSE;
	OP_SET_REG(core, &out, op, Rn);
	
	int ret = arm_op_exec_arm_str_imm(core, &in, &out);
	DBG_ARM_STR_IMM(core, &in, &out);

	core->pc = out.next_address;
	return ret;
}


int arm_op_exec_arm_strh_imm_a1(struct TargetCore *core)
{
	arm_OpCodeFormatType_arm_strh_imm_a1 *op = &core->decoded_code->code.arm_strh_imm_a1;

	arm_str_imm_input_type in;
	arm_str_imm_output_type out;
	out.status = *cpu_get_status(core);

	in.instrName = "STRH";

	in.cond = op->cond;
	in.imm32 = (uint32)(op->imm8);
	in.index = (op->P != 0);
	in.add = (op->U != 0);
	in.wback = ((op->P == 0) || (op->W != 0));
	in.size = 2U;
	OP_SET_REG(core, &in, op, Rn);
	OP_SET_REG(core, &in, op, Rt);

	//if P == ‘0’ && W == ‘1’ then SEE STRBT;
	if ((op->P == 0) && (op->W != 0)) {
		//TODO
		return -1;
	}
	if (op->Rt == CpuRegId_PC) {
		//UNPREDICTABLE
		return -1;
	}

	if (in.wback && ((op->Rn == CpuRegId_PC) || (op->Rn == op->Rt))) {
		//if wback && (n == 15 || n == t) then UNPREDICTABLE;
		//TODO
		return -1;
	}

	out.next_address = core->pc;
	out.passed = FALSE;
	OP_SET_REG(core, &out, op, Rn);

	int ret = arm_op_exec_arm_str_imm(core, &in, &out);
	DBG_ARM_STR_IMM(core, &in, &out);

	core->pc = out.next_address;
	return ret;
}

int arm_op_exec_arm_strd_imm_a1(struct TargetCore *core)
{
	arm_OpCodeFormatType_arm_strd_imm_a1 *op = &core->decoded_code->code.arm_strd_imm_a1;

	arm_strd_imm_input_type in;
	arm_strd_imm_output_type out;
	out.status = *cpu_get_status(core);

	in.instrName = "STRD";
	in.cond = op->cond;
	in.imm32 = op->imm8;
	in.index = (op->P != 0);
	in.add = (op->U != 0);
	in.wback = ((op->P == 0) || (op->W != 0));


	//if Rt<0> == ‘1’ then UNPREDICTABLE;
	if ((op->Rt & 0x1) != 0) {
		//UNPREDICTABLE
		return -1;
	}
	//if P == ‘0’ && W == ‘1’ then UNPREDICTABLE;
	if ((op->P == 0) && (op->W != 0)) {
		//TODO
		return -1;
	}
	//if wback && (n == 15 || n == t || n == t2) then UNPREDICTABLE;
	if (in.wback && ((op->Rn == CpuRegId_PC) || (op->Rn == op->Rt) || (op->Rn == (op->Rt + 1)))) {
		//TODO
		return -1;
	}
	//if t2 == 15 then UNPREDICTABLE
	if ((op->Rt + 1)== CpuRegId_PC) {
		//UNPREDICTABLE
		return -1;
	}
	OP_SET_REG(core, &in, op, Rn);
	OP_SET_REGID(core, &in, op->Rt, Rt1);
	OP_SET_REGID(core, &in, op->Rt + 1, Rt2);

	out.next_address = core->pc;
	out.passed = FALSE;
	OP_SET_REG(core, &out, op, Rn);

	int ret = arm_op_exec_arm_strd_imm(core, &in, &out);
	DBG_ARM_STRD_IMM(core, &in, &out);

	core->pc = out.next_address;
	return ret;
}

int arm_op_exec_arm_str_reg_a1(struct TargetCore *core)
{
	arm_OpCodeFormatType_arm_str_reg_a1 *op = &core->decoded_code->code.arm_str_reg_a1;

	arm_str_reg_input_type in;
	arm_str_reg_output_type out;
	out.status = *cpu_get_status(core);

	in.instrName = "STR";
	in.size = 4U;
	in.sign = FALSE;
	in.cond = op->cond;

	in.index = (op->P != 0);
	in.add = (op->U != 0);
	in.wback = ((op->P == 0) || (op->W != 0));
	DecodeImmShift(op->type, op->imm5, &in.shift_t, &in.shift_n);
	OP_SET_REG(core, &in, op, Rn);
	OP_SET_REG(core, &in, op, Rt);
	OP_SET_REG(core, &in, op, Rm);

	out.next_address = core->pc;
	out.passed = FALSE;

	int ret = arm_op_exec_arm_str_reg(core, &in, &out);
	DBG_ARM_STR_REG(core, &in, &out);

	core->pc = out.next_address;
	return ret;
}

int arm_op_exec_arm_strb_reg_a1(struct TargetCore *core)
{
	arm_OpCodeFormatType_arm_strb_reg_a1 *op = &core->decoded_code->code.arm_strb_reg_a1;

	arm_str_reg_input_type in;
	arm_str_reg_output_type out;
	out.status = *cpu_get_status(core);

	//TODO arguments setting..
	in.instrName = "STRB";
	in.cond = op->cond;
	in.index = (op->P != 0);
	in.add = (op->U != 0);
	in.wback = ((op->P == 0) || (op->W != 0));
	DecodeImmShift(op->type, op->imm5, &in.shift_t, &in.shift_n);
	in.size = 1U;
	in.sign = FALSE;
	OP_SET_REG(core, &in, op, Rn);
	OP_SET_REG(core, &in, op, Rt);
	OP_SET_REG(core, &in, op, Rm);
	//if t == 15 || m == 15 then UNPREDICTABLE;
	if ((op->Rt == CpuRegId_PC) || (op->Rm == CpuRegId_PC)) {
		return -1;
	}
	//if wback && (n == 15 || n == t) then UNPREDICTABLE;
	else if (in.wback && ((op->Rn == CpuRegId_PC) || (op->Rn == op->Rt))) {
		return -1;
	}
	//not supported following condition
	//if ArchVersion() < 6 && wback && m == n then UNPREDICTABLE;

	out.next_address = core->pc;
	out.passed = FALSE;
	
	int ret = arm_op_exec_arm_str_reg(core, &in, &out);
	DBG_ARM_STR_REG(core, &in, &out);

	core->pc = out.next_address;
	return ret;
}

int arm_op_exec_arm_push_a1(struct TargetCore *core)
{
	arm_OpCodeFormatType_arm_push_a1 *op = &core->decoded_code->code.arm_push_a1;

	arm_push_input_type in;
	arm_push_output_type out;
	out.status = *cpu_get_status(core);

	in.instrName = "PUSH";
	in.cond = op->cond;
	in.bitcount = BitCount(op->register_list);
	in.UnalignedAllowed = FALSE;
	in.registers = op->register_list;
	OP_SET_REGID(core, &in, CpuRegId_SP, SP);

	out.next_address = core->pc;
	out.passed = FALSE;
	OP_SET_REGID(core, &out, CpuRegId_SP, SP);

	if (in.bitcount < 2) {
		// TODO SEE STMDB / STMFD;
		printf("Unsupported code: STMDB / STMFD¥n");
		return -1;
	}
	
	int ret = arm_op_exec_arm_push(core, &in, &out);
	DBG_ARM_PUSH(core, &in, &out);

	if (ret == 0) {
		core->pc = out.next_address;
		cpu_set_reg(core, CpuRegId_SP, out.SP.regData);
	}
	return ret;
}

int arm_op_exec_arm_push_a2(struct TargetCore *core)
{
	arm_OpCodeFormatType_arm_push_a2 *op = &core->decoded_code->code.arm_push_a2;

	arm_push_input_type in;
	arm_push_output_type out;

	if (op->Rt == 13) {
		//UNPREDICTABLE
		printf("UNPREDICTABLE: arm_op_exec_arm_push_a2¥n");
		return -1;
	}
	out.status = *cpu_get_status(core);

	in.instrName = "PUSH";
	in.cond = op->cond;
	in.bitcount = 1U;
	in.UnalignedAllowed = TRUE;
	in.registers = (1 << (op->Rt));
	OP_SET_REGID(core, &in, CpuRegId_SP, SP);

	out.next_address = core->pc;
	out.passed = FALSE;
	OP_SET_REGID(core, &out, CpuRegId_SP, SP);

	int ret = arm_op_exec_arm_push(core, &in, &out);
	DBG_ARM_PUSH(core, &in, &out);

	if (ret == 0) {
		core->pc = out.next_address;
		cpu_set_reg(core, CpuRegId_SP, out.SP.regData);
	}
	return ret;
}


int arm_op_exec_arm_stmfd_a1(struct TargetCore *core)
{
	arm_OpCodeFormatType_arm_stmfd_a1 *op = &core->decoded_code->code.arm_stmfd_a1;

	arm_stmfd_input_type in;
	arm_stmfd_output_type out;
	out.status = *cpu_get_status(core);

	in.instrName = "STMFD";

	in.cond = op->cond;
	in.bitcount = BitCount(op->register_list);
	in.wback = (op->W == 1);
	in.registers = op->register_list;
	OP_SET_REG(core, &in, op, Rn);

	out.next_address = core->pc;
	out.passed = FALSE;
	OP_SET_REG(core, &out, op, Rn);

	int ret = arm_op_exec_arm_stmfd(core, &in, &out);
	DBG_ARM_STMFD(core, &in, &out);

	if (ret == 0) {
		cpu_set_reg(core, out.Rn.regId, out.Rn.regData);
	}

	core->pc = out.next_address;
	return ret;
}

int arm_op_exec_arm_stmib_a1(struct TargetCore *core)
{
	arm_OpCodeFormatType_arm_stmib_a1 *op = &core->decoded_code->code.arm_stmib_a1;

	arm_stmib_input_type in;
	arm_stmib_output_type out;
	out.status = *cpu_get_status(core);

	in.instrName = "STMIB";
	in.cond = op->cond;
	in.bitcount = BitCount(op->register_list);
	in.wback = (op->W == 1);
	in.registers = op->register_list;
	OP_SET_REG(core, &in, op, Rn);

	out.next_address = core->pc;
	out.passed = FALSE;
	OP_SET_REG(core, &out, op, Rn);

	int ret = arm_op_exec_arm_stmib(core, &in, &out);
	DBG_ARM_STMIB(core, &in, &out);

	core->pc = out.next_address;
	return ret;
}

int arm_op_exec_arm_stm_a1(struct TargetCore *core)
{
	arm_OpCodeFormatType_arm_stm_a1 *op = &core->decoded_code->code.arm_stm_a1;

	arm_stm_input_type in;
	arm_stm_output_type out;
	out.status = *cpu_get_status(core);

	in.instrName = "STM";
	in.cond = op->cond;
	in.wback = (op->W != 0);
	in.bitcount = BitCount(op->register_list);
	in.registers = op->register_list;
	OP_SET_REG(core, &in, op, Rn);

	out.next_address = core->pc;
	out.passed = FALSE;
	OP_SET_REG(core, &out, op, Rn);

	int ret = arm_op_exec_arm_stm(core, &in, &out);
	DBG_ARM_STM(core, &in, &out);

	core->pc = out.next_address;
	return ret;
}

int arm_op_exec_arm_srs_a1(struct TargetCore *core)
{
	arm_OpCodeFormatType_arm_srs_a1 *op = &core->decoded_code->code.arm_srs_a1;

	arm_srs_input_type in;
	arm_srs_output_type out;
	out.status = *cpu_get_status(core);

	in.instrName = "SRS";
	in.wback = (op->W != 0);
	in.increment = (op->U == 1);
	in.wordhigher = (op->P == op->U);
	in.mode = op->mode;
	OP_SET_REGID(core, &in, CpuRegId_SP, SP);
	in.SP.regData = cpu_get_reg_mode(core, CpuRegId_SP, in.mode);
	OP_SET_REGID(core, &in, CpuRegId_LR, LR);
	in.SPSR = *cpu_get_saved_status(core);

	out.SP = in.SP;
	out.next_address = core->pc;
	out.passed = FALSE;

	int ret = arm_op_exec_arm_srs(core, &in, &out);
	DBG_ARM_SRS(core, &in, &out);

	core->pc = out.next_address;
	return ret;
}
