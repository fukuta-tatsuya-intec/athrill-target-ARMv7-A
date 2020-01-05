#include "cpu_dec/arm_mcdecoder.h"
#include "arm_pseudo_code_debug.h"
#include "arm_pseudo_code_func.h"


int arm_op_exec_arm_bic_imm_a1(struct TargetCore *core)
{
	arm_OpCodeFormatType_arm_bic_imm_a1 *op = &core->decoded_code->code.arm_bic_imm_a1;

	arm_bic_imm_input_type in;
	arm_bic_imm_output_type out;
	out.status = *cpu_get_status(core);

	in.instrName = "BIC";
	in.cond = op->cond;
	in.S = op->S;
	OP_SET_REG(core, &in, op, Rd);
	OP_SET_REG(core, &in, op, Rn);
	cpu_conv_status_flag(out.status, &out.status_flag);
	in.imm32 = ARMExpandImm(op->imm12, out.status_flag.carry);

	OP_SET_REG(core, &out, op, Rd);
	out.next_address = core->pc;
	out.passed = FALSE;

	int ret = arm_op_exec_arm_bic_imm(core, &in, &out);
	DBG_ARM_BIC_IMM(core, &in, &out);

	core->pc = out.next_address;
	return ret;
}

int arm_op_exec_arm_orr_imm_a1(struct TargetCore *core)
{
	arm_OpCodeFormatType_arm_orr_imm_a1 *op = &core->decoded_code->code.arm_orr_imm_a1;

	arm_orr_imm_input_type in;
	arm_orr_imm_output_type out;
	out.status = *cpu_get_status(core);

	in.instrName = "ORR";
	in.cond = op->cond;
	in.S = op->S;
	OP_SET_REG(core, &in, op, Rd);
	OP_SET_REG(core, &in, op, Rn);
	cpu_conv_status_flag(out.status, &out.status_flag);
	in.imm32 = ARMExpandImm(op->imm12, out.status_flag.carry);

	OP_SET_REG(core, &out, op, Rd);
	out.next_address = core->pc;
	out.passed = FALSE;

	int ret = arm_op_exec_arm_orr_imm(core, &in, &out);
	DBG_ARM_ORR_IMM(core, &in, &out);

	core->pc = out.next_address;
	return ret;
}


int arm_op_exec_arm_bfc_a1(struct TargetCore *core)
{
	arm_OpCodeFormatType_arm_bfc_a1 *op = &core->decoded_code->code.arm_bfc_a1;

	arm_bfc_input_type in;
	arm_bfc_output_type out;
	out.status = *cpu_get_status(core);

	in.instrName = "BFC";
	in.cond = op->cond;
	in.msbit = op->msb;
	in.lsbit = op->lsb;
	OP_SET_REG(core, &in, op, Rd);

	OP_SET_REG(core, &out, op, Rd);
	out.next_address = core->pc;
	out.passed = FALSE;

	int ret = arm_op_exec_arm_bfc(core, &in, &out);
	DBG_ARM_BFC(core, &in, &out);

	core->pc = out.next_address;
	return ret;
}



int arm_op_exec_arm_utxb_a1(struct TargetCore *core)
{
	arm_OpCodeFormatType_arm_utxb_a1 *op = &core->decoded_code->code.arm_utxb_a1;

	arm_utx_input_type in;
	arm_utx_output_type out;
	out.status = *cpu_get_status(core);

	in.instrName = "UTXB";
	in.size = 1U;
	in.cond = op->cond;
	OP_SET_REG(core, &in, op, Rd);
	//rotation = UInt(rotate:’000’);
	in.rotate = (op->rotate << 3U);
	OP_SET_REG(core, &in, op, Rm);

	out.next_address = core->pc;
	out.passed = FALSE;
	OP_SET_REG(core, &out, op, Rd);

	int ret = arm_op_exec_arm_utx(core, &in, &out);
	DBG_ARM_UTX(core, &in, &out);

	core->pc = out.next_address;
	return ret;
}


int arm_op_exec_arm_utxh_a1(struct TargetCore *core)
{
	arm_OpCodeFormatType_arm_utxh_a1 *op = &core->decoded_code->code.arm_utxh_a1;

	arm_utx_input_type in;
	arm_utx_output_type out;
	out.status = *cpu_get_status(core);

	in.instrName = "UTXB";
	in.size = 2U;
	in.cond = op->cond;
	OP_SET_REG(core, &in, op, Rd);
	//rotation = UInt(rotate:’000’);
	in.rotate = (op->rotate << 3U);
	OP_SET_REG(core, &in, op, Rm);

	out.next_address = core->pc;
	out.passed = FALSE;
	OP_SET_REG(core, &out, op, Rd);

	int ret = arm_op_exec_arm_utx(core, &in, &out);
	DBG_ARM_UTX(core, &in, &out);

	core->pc = out.next_address;
	return ret;
}
