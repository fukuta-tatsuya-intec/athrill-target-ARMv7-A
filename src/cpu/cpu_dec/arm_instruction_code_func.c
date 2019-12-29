#include "../../src/cpu/cpu_exec/op_exec_debug.h"
#include "cpu_dec/arm_mcdecoder.h"
#include "target_cpu.h"
#include "cpu_ops.h"
#include "bus.h"
#include "arm_pseudo_code_func.h"



int arm_op_exec_arm_add_imm_a1(struct TargetCore *core)
{
	arm_OpCodeFormatType_arm_add_imm_a1 *op = &core->decoded_code->code.arm_add_imm_a1;

	arm_add_imm_input_type in;
	arm_add_imm_output_type out;
	out.status = *cpu_get_status(core);

	//TODO arguments setting..
	in.instrName = "ARM_ADD_IMM_A1";

	in.cond = op->cond;

	in.S = op->S;

	in.Rd = op->Rd;

	in.Rn = op->Rn;

	in.imm32 = op->imm32;

	out.next_address = core->pc;
	out.passed = FALSE;

	out.result = -1;

	out.status_flag = -1;

	
	int ret = arm_op_exec_arm_add_imm(core, &in, &out);
	DBG_ARM_ADD_IMM(core, &in, &out);

	core->pc = out.next_address;
	return ret;
}


int arm_op_exec_arm_add_spimm_a1(struct TargetCore *core)
{
	arm_OpCodeFormatType_arm_add_spimm_a1 *op = &core->decoded_code->code.arm_add_spimm_a1;

	arm_add_spimm_input_type in;
	arm_add_spimm_output_type out;
	out.status = *cpu_get_status(core);

	//TODO arguments setting..
	in.instrName = "ARM_ADD_SPIMM_A1";

	in.cond = op->cond;

	in.S = op->S;

	in.Rd = op->Rd;

	in.SP = op->SP;

	in.imm32 = op->imm32;

	out.next_address = core->pc;
	out.passed = FALSE;

	out.result = -1;

	out.status_flag = -1;

	
	int ret = arm_op_exec_arm_add_spimm(core, &in, &out);
	DBG_ARM_ADD_SPIMM(core, &in, &out);

	core->pc = out.next_address;
	return ret;
}


int arm_op_exec_arm_adr_a1(struct TargetCore *core)
{
	arm_OpCodeFormatType_arm_adr_a1 *op = &core->decoded_code->code.arm_adr_a1;

	arm_adr_imm_input_type in;
	arm_adr_imm_output_type out;
	out.status = *cpu_get_status(core);

	//TODO arguments setting..
	in.instrName = "ARM_ADR_A1";

	in.cond = op->cond;

	in.Rd = op->Rd;

	in.imm32 = op->imm32;

	in.add = op->add;

	out.next_address = core->pc;
	out.passed = FALSE;

	out.result = -1;

	out.status_flag = -1;

	
	int ret = arm_op_exec_arm_adr_imm(core, &in, &out);
	DBG_ARM_ADR_IMM(core, &in, &out);

	core->pc = out.next_address;
	return ret;
}


int arm_op_exec_arm_subs_pclr_a1(struct TargetCore *core)
{
	arm_OpCodeFormatType_arm_subs_pclr_a1 *op = &core->decoded_code->code.arm_subs_pclr_a1;

	arm_subs_pclr_input_type in;
	arm_subs_pclr_output_type out;
	out.status = *cpu_get_status(core);

	//TODO arguments setting..
	in.instrName = "ARM_SUBS_PCLR_A1";

	in.cond = op->cond;

	in.opcode = op->opcode;

	in.Rn = op->Rn;

	in.Rm = op->Rm;

	in.register_form = op->register_form;

	in.shift_t = op->shift_t;

	in.shift_n = op->shift_n;

	in.imm32 = op->imm32;

	out.next_address = core->pc;
	out.passed = FALSE;

	out.result = -1;

	out.status_flag = -1;

	
	int ret = arm_op_exec_arm_subs_pclr(core, &in, &out);
	DBG_ARM_SUBS_PCLR(core, &in, &out);

	core->pc = out.next_address;
	return ret;
}


int arm_op_exec_push_1(struct TargetCore *core)
{
	arm_OpCodeFormatType_push_1 *op = &core->decoded_code->code.push_1;

	arm_add_imm_input_type in;
	arm_add_imm_output_type out;
	out.status = *cpu_get_status(core);

	//TODO arguments setting..
	in.instrName = "PUSH_1";

	in.cond = op->cond;

	in.S = op->S;

	in.Rd = op->Rd;

	in.Rn = op->Rn;

	in.imm32 = op->imm32;

	out.next_address = core->pc;
	out.passed = FALSE;

	out.result = -1;

	out.status_flag = -1;

	
	int ret = arm_op_exec_arm_add_imm(core, &in, &out);
	DBG_ARM_ADD_IMM(core, &in, &out);

	core->pc = out.next_address;
	return ret;
}


int arm_op_exec_arm_mov_imm_a1(struct TargetCore *core)
{
	arm_OpCodeFormatType_arm_mov_imm_a1 *op = &core->decoded_code->code.arm_mov_imm_a1;

	arm_mov_imm_input_type in;
	arm_mov_imm_output_type out;
	out.status = *cpu_get_status(core);

	//TODO arguments setting..
	in.instrName = "ARM_MOV_IMM_A1";

	in.cond = op->cond;

	in.S = op->S;

	in.Rd = op->Rd;

	in.imm32 = op->imm32;

	out.next_address = core->pc;
	out.passed = FALSE;

	out.result = -1;

	out.status_flag = -1;

	
	int ret = arm_op_exec_arm_mov_imm(core, &in, &out);
	DBG_ARM_MOV_IMM(core, &in, &out);

	core->pc = out.next_address;
	return ret;
}


int arm_op_exec_arm_mov_imm_a2(struct TargetCore *core)
{
	arm_OpCodeFormatType_arm_mov_imm_a2 *op = &core->decoded_code->code.arm_mov_imm_a2;

	arm_mov_imm_input_type in;
	arm_mov_imm_output_type out;
	out.status = *cpu_get_status(core);

	//TODO arguments setting..
	in.instrName = "ARM_MOV_IMM_A2";

	in.cond = op->cond;

	in.S = op->S;

	in.Rd = op->Rd;

	in.imm32 = op->imm32;

	out.next_address = core->pc;
	out.passed = FALSE;

	out.result = -1;

	out.status_flag = -1;

	
	int ret = arm_op_exec_arm_mov_imm(core, &in, &out);
	DBG_ARM_MOV_IMM(core, &in, &out);

	core->pc = out.next_address;
	return ret;
}


int arm_op_exec_arm_mov_reg_a1(struct TargetCore *core)
{
	arm_OpCodeFormatType_arm_mov_reg_a1 *op = &core->decoded_code->code.arm_mov_reg_a1;

	arm_mov_reg_input_type in;
	arm_mov_reg_output_type out;
	out.status = *cpu_get_status(core);

	//TODO arguments setting..
	in.instrName = "ARM_MOV_REG_A1";

	in.cond = op->cond;

	in.S = op->S;

	in.Rd = op->Rd;

	in.Rm = op->Rm;

	out.next_address = core->pc;
	out.passed = FALSE;

	out.result = -1;

	out.status_flag = -1;

	
	int ret = arm_op_exec_arm_mov_reg(core, &in, &out);
	DBG_ARM_MOV_REG(core, &in, &out);

	core->pc = out.next_address;
	return ret;
}


int arm_op_exec_arm_bl_a1(struct TargetCore *core)
{
	arm_OpCodeFormatType_arm_bl_a1 *op = &core->decoded_code->code.arm_bl_a1;

	arm_bl_imm_input_type in;
	arm_bl_imm_output_type out;
	out.status = *cpu_get_status(core);

	//TODO arguments setting..
	in.instrName = "ARM_BL_A1";

	in.cond = op->cond;

	in.imm32 = op->imm32;

	in.type = op->type;

	out.next_address = core->pc;
	out.passed = FALSE;

	out.LR = -1;

	out.result = -1;

	
	int ret = arm_op_exec_arm_bl_imm(core, &in, &out);
	DBG_ARM_BL_IMM(core, &in, &out);

	core->pc = out.next_address;
	return ret;
}


int arm_op_exec_arm_blx_a2(struct TargetCore *core)
{
	arm_OpCodeFormatType_arm_blx_a2 *op = &core->decoded_code->code.arm_blx_a2;

	arm_bl_imm_input_type in;
	arm_bl_imm_output_type out;
	out.status = *cpu_get_status(core);

	//TODO arguments setting..
	in.instrName = "ARM_BLX_A2";

	in.cond = op->cond;

	in.imm32 = op->imm32;

	in.type = op->type;

	out.next_address = core->pc;
	out.passed = FALSE;

	out.LR = -1;

	out.result = -1;

	
	int ret = arm_op_exec_arm_bl_imm(core, &in, &out);
	DBG_ARM_BL_IMM(core, &in, &out);

	core->pc = out.next_address;
	return ret;
}


int arm_op_exec_arm_str_imm_a1(struct TargetCore *core)
{
	arm_OpCodeFormatType_arm_str_imm_a1 *op = &core->decoded_code->code.arm_str_imm_a1;

	arm_str_imm_input_type in;
	arm_str_imm_output_type out;
	out.status = *cpu_get_status(core);

	//TODO arguments setting..
	in.instrName = "ARM_STR_IMM_A1";

	in.cond = op->cond;

	in.imm32 = op->imm32;

	in.add = op->add;

	in.index = op->index;

	in.wback = op->wback;

	in.Rn = op->Rn;

	in.Rt = op->Rt;

	out.next_address = core->pc;
	out.passed = FALSE;

	out.Rn = -1;

	
	int ret = arm_op_exec_arm_str_imm(core, &in, &out);
	DBG_ARM_STR_IMM(core, &in, &out);

	core->pc = out.next_address;
	return ret;
}


int arm_op_exec_arm_ldr_imm_a1(struct TargetCore *core)
{
	arm_OpCodeFormatType_arm_ldr_imm_a1 *op = &core->decoded_code->code.arm_ldr_imm_a1;

	arm_ldr_imm_input_type in;
	arm_ldr_imm_output_type out;
	out.status = *cpu_get_status(core);

	//TODO arguments setting..
	in.instrName = "ARM_LDR_IMM_A1";

	in.cond = op->cond;

	in.imm32 = op->imm32;

	in.add = op->add;

	in.index = op->index;

	in.wback = op->wback;

	in.Rn = op->Rn;

	in.Rt = op->Rt;

	out.next_address = core->pc;
	out.passed = FALSE;

	out.result = -1;

	
	int ret = arm_op_exec_arm_ldr_imm(core, &in, &out);
	DBG_ARM_LDR_IMM(core, &in, &out);

	core->pc = out.next_address;
	return ret;
}
