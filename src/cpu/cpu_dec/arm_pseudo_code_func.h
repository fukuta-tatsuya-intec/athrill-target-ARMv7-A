#ifndef _ARM_PSEUDO_CODE_FUNC_H_
#define _ARM_PSEUDO_CODE_FUNC_H_

#include "arm_pseudo_code.h"

extern int arm_op_exec_arm_add_imm(struct TargetCore *core,  arm_add_imm_input_type *in, arm_add_imm_output_type *out);
extern int arm_op_exec_arm_add_reg(struct TargetCore *core,  arm_add_reg_input_type *in, arm_add_reg_output_type *out);
extern int arm_op_exec_arm_sub_imm(struct TargetCore *core,  arm_sub_imm_input_type *in, arm_sub_imm_output_type *out);
extern int arm_op_exec_arm_sub_reg(struct TargetCore *core,  arm_sub_reg_input_type *in, arm_sub_reg_output_type *out);
extern int arm_op_exec_arm_cmp_imm(struct TargetCore *core,  arm_cmp_imm_input_type *in, arm_cmp_imm_output_type *out);
extern int arm_op_exec_arm_cmp_reg(struct TargetCore *core,  arm_cmp_reg_input_type *in, arm_cmp_reg_output_type *out);
extern int arm_op_exec_arm_adr_imm(struct TargetCore *core,  arm_adr_imm_input_type *in, arm_adr_imm_output_type *out);
extern int arm_op_exec_arm_add_spimm(struct TargetCore *core,  arm_add_spimm_input_type *in, arm_add_spimm_output_type *out);
extern int arm_op_exec_arm_subs_pclr(struct TargetCore *core,  arm_subs_pclr_input_type *in, arm_subs_pclr_output_type *out);
extern int arm_op_exec_arm_mov_imm(struct TargetCore *core,  arm_mov_imm_input_type *in, arm_mov_imm_output_type *out);
extern int arm_op_exec_arm_mvn_imm(struct TargetCore *core,  arm_mvn_imm_input_type *in, arm_mvn_imm_output_type *out);
extern int arm_op_exec_arm_mov_reg(struct TargetCore *core,  arm_mov_reg_input_type *in, arm_mov_reg_output_type *out);
extern int arm_op_exec_arm_mvn_reg(struct TargetCore *core,  arm_mvn_reg_input_type *in, arm_mvn_reg_output_type *out);
extern int arm_op_exec_arm_bl_imm(struct TargetCore *core,  arm_bl_imm_input_type *in, arm_bl_imm_output_type *out);
extern int arm_op_exec_arm_b_imm(struct TargetCore *core,  arm_b_imm_input_type *in, arm_b_imm_output_type *out);
extern int arm_op_exec_arm_bx_reg(struct TargetCore *core,  arm_bx_reg_input_type *in, arm_bx_reg_output_type *out);
extern int arm_op_exec_arm_bl_reg(struct TargetCore *core,  arm_bl_reg_input_type *in, arm_bl_reg_output_type *out);
extern int arm_op_exec_arm_ldr_imm(struct TargetCore *core,  arm_ldr_imm_input_type *in, arm_ldr_imm_output_type *out);
extern int arm_op_exec_arm_ldr_literal(struct TargetCore *core,  arm_ldr_literal_input_type *in, arm_ldr_literal_output_type *out);
extern int arm_op_exec_arm_ldrd_imm(struct TargetCore *core,  arm_ldrd_imm_input_type *in, arm_ldrd_imm_output_type *out);
extern int arm_op_exec_arm_ldr_reg(struct TargetCore *core,  arm_ldr_reg_input_type *in, arm_ldr_reg_output_type *out);
extern int arm_op_exec_arm_push(struct TargetCore *core,  arm_push_input_type *in, arm_push_output_type *out);
extern int arm_op_exec_arm_stmfd(struct TargetCore *core,  arm_stmfd_input_type *in, arm_stmfd_output_type *out);
extern int arm_op_exec_arm_stm(struct TargetCore *core,  arm_stm_input_type *in, arm_stm_output_type *out);
extern int arm_op_exec_arm_pop(struct TargetCore *core,  arm_pop_input_type *in, arm_pop_output_type *out);
extern int arm_op_exec_arm_ldm(struct TargetCore *core,  arm_ldm_input_type *in, arm_ldm_output_type *out);
extern int arm_op_exec_arm_str_imm(struct TargetCore *core,  arm_str_imm_input_type *in, arm_str_imm_output_type *out);
extern int arm_op_exec_arm_strd_imm(struct TargetCore *core,  arm_strd_imm_input_type *in, arm_strd_imm_output_type *out);
extern int arm_op_exec_arm_str_reg(struct TargetCore *core,  arm_str_reg_input_type *in, arm_str_reg_output_type *out);
extern int arm_op_exec_arm_nop(struct TargetCore *core,  arm_nop_input_type *in, arm_nop_output_type *out);
extern int arm_op_exec_arm_msr_imm(struct TargetCore *core,  arm_msr_imm_input_type *in, arm_msr_imm_output_type *out);
extern int arm_op_exec_arm_msr2_imm(struct TargetCore *core,  arm_msr2_imm_input_type *in, arm_msr2_imm_output_type *out);
extern int arm_op_exec_arm_msr2_reg(struct TargetCore *core,  arm_msr2_reg_input_type *in, arm_msr2_reg_output_type *out);
extern int arm_op_exec_arm_mrs(struct TargetCore *core,  arm_mrs_input_type *in, arm_mrs_output_type *out);
extern int arm_op_exec_arm_mcr(struct TargetCore *core,  arm_mcr_input_type *in, arm_mcr_output_type *out);
extern int arm_op_exec_arm_mrc(struct TargetCore *core,  arm_mrc_input_type *in, arm_mrc_output_type *out);
extern int arm_op_exec_arm_bic_imm(struct TargetCore *core,  arm_bic_imm_input_type *in, arm_bic_imm_output_type *out);
extern int arm_op_exec_arm_orr_imm(struct TargetCore *core,  arm_orr_imm_input_type *in, arm_orr_imm_output_type *out);
extern int arm_op_exec_arm_orr_reg(struct TargetCore *core,  arm_orr_reg_input_type *in, arm_orr_reg_output_type *out);
extern int arm_op_exec_arm_movt(struct TargetCore *core,  arm_movt_input_type *in, arm_movt_output_type *out);
extern int arm_op_exec_arm_lsl_imm(struct TargetCore *core,  arm_lsl_imm_input_type *in, arm_lsl_imm_output_type *out);
extern int arm_op_exec_arm_lsl_reg(struct TargetCore *core,  arm_lsl_reg_input_type *in, arm_lsl_reg_output_type *out);
extern int arm_op_exec_arm_lsr_imm(struct TargetCore *core,  arm_lsr_imm_input_type *in, arm_lsr_imm_output_type *out);
extern int arm_op_exec_arm_bfc(struct TargetCore *core,  arm_bfc_input_type *in, arm_bfc_output_type *out);
extern int arm_op_exec_arm_and_imm(struct TargetCore *core,  arm_and_imm_input_type *in, arm_and_imm_output_type *out);
extern int arm_op_exec_arm_and_reg(struct TargetCore *core,  arm_and_reg_input_type *in, arm_and_reg_output_type *out);
extern int arm_op_exec_arm_uxt(struct TargetCore *core,  arm_uxt_input_type *in, arm_uxt_output_type *out);
extern int arm_op_exec_arm_rsb_reg(struct TargetCore *core,  arm_rsb_reg_input_type *in, arm_rsb_reg_output_type *out);
extern int arm_op_exec_arm_sbc_reg(struct TargetCore *core,  arm_sbc_reg_input_type *in, arm_sbc_reg_output_type *out);
extern int arm_op_exec_arm_mul(struct TargetCore *core,  arm_mul_input_type *in, arm_mul_output_type *out);
extern int arm_op_exec_arm_tst_imm(struct TargetCore *core,  arm_tst_imm_input_type *in, arm_tst_imm_output_type *out);
extern int arm_op_exec_arm_tst_reg(struct TargetCore *core,  arm_tst_reg_input_type *in, arm_tst_reg_output_type *out);
extern int arm_op_exec_arm_sxtb(struct TargetCore *core,  arm_sxtb_input_type *in, arm_sxtb_output_type *out);
extern int arm_op_exec_arm_asr_imm(struct TargetCore *core,  arm_asr_imm_input_type *in, arm_asr_imm_output_type *out);
extern int arm_op_exec_arm_srs(struct TargetCore *core,  arm_srs_input_type *in, arm_srs_output_type *out);
extern int arm_op_exec_arm_rfe(struct TargetCore *core,  arm_rfe_input_type *in, arm_rfe_output_type *out);
#endif /* !_ARM_PSEUDO_CODE_FUNC_H_ */