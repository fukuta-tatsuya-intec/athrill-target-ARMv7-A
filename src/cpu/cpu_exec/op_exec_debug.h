#ifndef _OP_EXEC_DEBUG_H_
#define _OP_EXEC_DEBUG_H_

#include "dbg_log.h"
#include "cpu_op_types.h"

#define SKIP_RESULT(cond) ( ((cond) != FALSE) ? "FALSE" : "TRUE" )
#define SIGN_FLAG(S)	( ((S) != FALSE) ? "S" : "" )


static inline const char *ConditionString(uint8 cond)
{
	static const char *values[15] = {
			"EQ", //0
			"NE", //1
			"CS", //2
			"CC", //3
			"MI", //4
			"PL", //5
			"VS", //6
			"VC", //7
			"HI", //8
			"LS", //9
			"GE", //10
			"LT", //11
			"GT", //12
			"LE", //13
			"", //14
	};
	return values[cond];
}
static inline void DBG_ARM_ADD_IMM(ArmAddImmArgType *arg, uint32 Rd, uint32 Rn, uint32 result, bool passed)
{
	DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(),
		"%s%s%s Rd(R%d(%d)), Rn(R%d(%d)), imm12(%d): %d (skip=%s)\n",
		arg->instrName, SIGN_FLAG(arg->S), ConditionString(arg->cond), (arg)->Rd, Rd, (arg)->Rn, Rn,
		arg->imm32, result, SKIP_RESULT(passed)));
}

static inline void DBG_ARM_MOV_IMM(ArmMovImmArgType *arg, uint32 Rd, uint32 result, bool passed)
{
	DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(),
		"%s%s%s Rd(R%d(%d)), imm(%d): %d (skip=%s)\n",
		arg->instrName, SIGN_FLAG(arg->S), ConditionString(arg->cond), (arg)->Rd, Rd,
		arg->imm32, result, SKIP_RESULT(passed)));
}
static inline void DBG_ARM_BRANCH_IMM(ArmBranchImmArgType *arg, uint32 next_address, bool passed)
{
	DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(),
		"%s%s imm(%d): 0x%x (skip=%s)\n",
		arg->instrName, ConditionString(arg->cond), (arg)->imm32,
		next_address, SKIP_RESULT(passed)));
}
static inline void DBG_ARM_STR_IMM(ArmStoreImmArgType *arg, uint32 Rt, uint32 Rn, uint32 address, uint32 data, bool passed)
{
	DBG_PRINT((DBG_EXEC_OP_BUF(), DBG_EXEC_OP_BUF_LEN(),
		"%s%s Rt(R%d(%d)), Rn(R%d(0x%x)), imm12(%s%d) wback=%s: %d@0x%x (skip=%s)\n",
		arg->instrName, ConditionString(arg->cond), (arg)->Rt, Rt, (arg)->Rn, Rn,
		arg->add_flag ? "+" : "-", (arg)->imm32,
		arg->wback_flag ? "TRUE" : "FALSE",
		data, address, SKIP_RESULT(passed)));
}

#endif /* _OP_EXEC_DEBUG_H_ */
