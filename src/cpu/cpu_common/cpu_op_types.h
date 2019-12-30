#ifndef _CPU_OP_TYPES_H_
#define _CPU_OP_TYPES_H_

#include "std_types.h"
#include "target_cpu.h"
#include <string.h>

/************************************************
 * pseudo code common data types
 ************************************************/
static inline const char *DbgStatusUpdateFlag(bool flag)
{
	return ( (flag != FALSE) ? "S" : "" );
}

static inline const char *DbgBoolFlag(bool flag)
{
	return ( (flag != FALSE) ? "T" : "F" );
}
static inline const char *DbgPassedFlag(bool flag)
{
	return ( (flag != FALSE) ? "Done" : "None" );
}

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
#define DBG_FMT_STR	"%s "

typedef struct {
	char* name;
	uint32 regId;
	sint32 regData;
} PseudoCodeRegisterDataType; 						/* Rn, Rd, Rt, etc */
#define DBG_FMT_PseudoCodeRegisterDataType			"%s(R%d(%d)) "
#define DBG_ARG_PseudoCodeRegisterDataType(arg)		(arg)->name, (arg)->regId, (arg)->regData

typedef sint32	PseudoCodeImmediateData32Type; 		/* imm32 */
#define DBG_FMT_PseudoCodeImmediateData32Type		"imm32(%d) "
#define DBG_ARG_PseudoCodeImmediateData32Type(arg)	(*(arg))

typedef uint32	PseudoCodeConditionDataType; 		/* cond */
#define DBG_FMT_PseudoCodeConditionDataType			"%s "
#define DBG_ARG_PseudoCodeConditionDataType(arg)	ConditionString(*(arg))

typedef uint8	PseudoCodeUpdateStatusFlagType; 	/* S */
#define DBG_FMT_PseudoCodeUpdateStatusFlagType		"%s "
#define DBG_ARG_PseudoCodeUpdateStatusFlagType(arg)	DbgStatusUpdateFlag(*(arg))

typedef uint32	PseudoCodeStatusType;
#define DBG_FMT_PseudoCodeStatusType			"status(0x%x) "
#define DBG_ARG_PseudoCodeStatusType(arg)		(*(arg))

typedef struct {
	bool	overflow;
	bool	carry;
	bool	zero;
	bool	negative;
} PseudoCodeStatusFlagType;
#define DBG_FMT_PseudoCodeStatusFlagType			"O(%s)|C(%s)|Z(%s)|N(%s) "
#define DBG_ARG_PseudoCodeStatusFlagType(arg)	DbgBoolFlag((arg)->overflow), DbgBoolFlag((arg)->carry), DbgBoolFlag((arg)->zero), DbgBoolFlag((arg)->negative)
static inline void cpu_conv_status_flag(uint32 status, PseudoCodeStatusFlagType *out)
{
	memset(out, 0, sizeof(PseudoCodeStatusFlagType));
	if (CPU_STATUS_BIT_IS_SET(status, CPU_STATUS_BITPOS_N)) {
		out->negative = TRUE;
	}
	if (CPU_STATUS_BIT_IS_SET(status, CPU_STATUS_BITPOS_V)) {
		out->overflow = TRUE;
	}
	if (CPU_STATUS_BIT_IS_SET(status, CPU_STATUS_BITPOS_C)) {
		out->carry = TRUE;
	}
	if (CPU_STATUS_BIT_IS_SET(status, CPU_STATUS_BITPOS_Z)) {
		out->zero = TRUE;
	}
}
typedef bool PseudoCodeCondPassedType;
#define DBG_FMT_PseudoCodeCondPassedType		"Passed(%s) "
#define DBG_ARG_PseudoCodeCondPassedType(arg)	DbgPassedFlag(*(arg))

#define DBG_FMT_bool		"%s "
#define DBG_ARG_bool(arg)	DbgBoolFlag(*(arg))
#define DBG_FMT_sint32		"%d "
#define DBG_ARG_sint32(arg)	(*(arg))
#define DBG_FMT_uint8		"0x%x "
#define DBG_ARG_uint8(arg)	(*(arg))
#define DBG_FMT_uint32		"0x%x "
#define DBG_ARG_uint32(arg)	(*(arg))

typedef enum {
	SRType_LSL = 0,
	SRType_LSR,
	SRType_ASR,
	SRType_ROR,
	SRType_RRX,
} SRType;

static inline const char *SRTypeString(SRType type)
{
	static const char *values[5] = {
			"LSL", //0
			"LSR", //1
			"ASR", //2
			"ROR", //3
			"RRX", //4
	};
	return values[type];
}
#define DBG_FMT_SRType		"SRType(%s) "
#define DBG_ARG_SRType(arg)	SRTypeString(*(arg))

#define OP_SET_REG(core, arg, op, regName)	\
do {	\
	(arg)->regName.name = #regName;	\
	(arg)->regName.regId = (op)->regName;	\
	(arg)->regName.regData = cpu_get_reg(core, (op)->regName);	\
} while (0)

#define OP_SET_REGID(core, arg, register_id, regName)	\
do {	\
	(arg)->regName.name = #regName;	\
	(arg)->regName.regId = (register_id);	\
	(arg)->regName.regData = cpu_get_reg(core, (register_id));	\
} while (0)

#define OP_INIT_REG(core, arg, regName)	\
do {	\
	(arg)->regName.name = #regName;	\
	(arg)->regName.regId = -1;	\
	(arg)->regName.regData = -1;	\
} while (0)

static inline bool BadMode(uint8 mode, TargetCoreType *core)
{
	bool result;
	switch (mode) {
		case 0b10000:
			result = FALSE; // User mode
			break;
		case 0b10001:
			result = FALSE; // FIQ mode
			break;
		case 0b10010:
			result = FALSE; // IRQ mode
			break;
		case 0b10011:
			result = FALSE; // Supervisor mode
			break;
		case 0b10110:
			result = !HaveSecurityExt(core); // Monitor mode
			break;
		case 0b10111:
			result = FALSE; // Abort mode
			break;
		case 0b11010:
			result = !HaveVirtExt(core); // Hyp mode
			break;
		case 0b11011:
			result = FALSE; // Undefined mode
			break;
		case 0b11111:
			result = FALSE; // System mode
			break;
		default:
		result = TRUE;
	}
	return result;
}
/************************************************/


#endif /* _CPU_OP_TYPES_H_ */
