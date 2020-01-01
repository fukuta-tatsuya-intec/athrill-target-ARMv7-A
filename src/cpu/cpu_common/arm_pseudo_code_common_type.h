#ifndef _ARM_PSEUDO_CODE_COMMON_TYPE_H_
#define _ARM_PSEUDO_CODE_COMMON_TYPE_H_

#include "std_types.h"
#include "target_cpu.h"

/************************************************
 * pseudo code common data types
 ************************************************/
static inline const char *DbgStatusUpdateFlag(bool flag)
{
	return ( (flag != FALSE) ? "S " : "" );
}

static inline const char *DbgBoolFlag(bool flag)
{
	return ( (flag != FALSE) ? "1" : "0" );
}
static inline const char *DbgPassedFlag(bool flag)
{
	return ( (flag != FALSE) ? "" : "[Skip]" );
}

static inline const char *ConditionString(uint8 cond)
{
	static const char *values[15] = {
			"EQ ", //0
			"NE ", //1
			"CS ", //2
			"CC ", //3
			"MI ", //4
			"PL ", //5
			"VS ", //6
			"VC ", //7
			"HI ", //8
			"LS ", //9
			"GE ", //10
			"LT ", //11
			"GT ", //12
			"LE ", //13
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
#define DBG_FMT_PseudoCodeRegisterDataType			"%s%d(%d 0x%x) "
#define DBG_ARG_PseudoCodeRegisterDataType(arg)		(arg)->name, (arg)->regId, (arg)->regData, (arg)->regData

typedef sint32	PseudoCodeImmediateData32Type; 		/* imm32 */
#define DBG_FMT_PseudoCodeImmediateData32Type		"imm32(%d) "
#define DBG_ARG_PseudoCodeImmediateData32Type(arg)	(*(arg))

typedef uint32	PseudoCodeConditionDataType; 		/* cond */
#define DBG_FMT_PseudoCodeConditionDataType			"%s"
#define DBG_ARG_PseudoCodeConditionDataType(arg)	ConditionString(*(arg))

typedef uint8	PseudoCodeUpdateStatusFlagType; 	/* S */
#define DBG_FMT_PseudoCodeUpdateStatusFlagType		"%s"
#define DBG_ARG_PseudoCodeUpdateStatusFlagType(arg)	DbgStatusUpdateFlag(*(arg))

typedef uint32	PseudoCodeStatusType;
#define DBG_FMT_PseudoCodeStatusType			"CPSR(0x%x) "
#define DBG_ARG_PseudoCodeStatusType(arg)		(*(arg))

typedef struct {
	bool	overflow;
	bool	carry;
	bool	zero;
	bool	negative;
} PseudoCodeStatusFlagType;
#define DBG_FMT_PseudoCodeStatusFlagType			"CF[%s%s%s%s] "
#define DBG_ARG_PseudoCodeStatusFlagType(arg)	DbgBoolFlag((arg)->negative), DbgBoolFlag((arg)->zero), DbgBoolFlag((arg)->carry), DbgBoolFlag((arg)->overflow)
static inline void cpu_conv_status_flag(uint32 status, PseudoCodeStatusFlagType *out)
{
	out->negative = FALSE;
	out->overflow = FALSE;
	out->carry = FALSE;
	out->zero = FALSE;
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
#define DBG_FMT_PseudoCodeCondPassedType		"%s "
#define DBG_ARG_PseudoCodeCondPassedType(arg)	DbgPassedFlag(*(arg))

#define DBG_FMT_bool		"%s "
#define DBG_ARG_bool(arg)	DbgBoolFlag(*(arg))
#define DBG_FMT_sint32		"%d "
#define DBG_ARG_sint32(arg)	(*(arg))
#define DBG_FMT_uint8		"0x%x "
#define DBG_ARG_uint8(arg)	(*(arg))
#define DBG_FMT_uint32		"0x%x "
#define DBG_ARG_uint32(arg)	(*(arg))

typedef sint32 PseudoCodeCmpResultType;
#define DBG_FMT_PseudoCodeCmpResultType		"compare(%d) "
#define DBG_ARG_PseudoCodeCmpResultType(arg)	(*(arg))

typedef bool PseudoCodeBoolAddType;
#define DBG_FMT_PseudoCodeBoolAddType		"add(%s) "
#define DBG_ARG_PseudoCodeBoolAddType(arg)	DbgBoolFlag(*(arg))

typedef bool PseudoCodeBoolIndexType;
#define DBG_FMT_PseudoCodeBoolIndexType			"index(%s) "
#define DBG_ARG_PseudoCodeBoolIndexType(arg)	DbgBoolFlag(*(arg))

typedef bool PseudoCodeBoolWbackType;
#define DBG_FMT_PseudoCodeBoolWbackType			"wback(%s) "
#define DBG_ARG_PseudoCodeBoolWbackType(arg)	DbgBoolFlag(*(arg))

static inline const char *InstrSetTypeString(uint8 cond)
{
	static const char *values[15] = {
			"ARM", //0
			"Thumb", //1
			"Jazelle", //2
			"ThumbEE", //3
	};
	return values[cond];
}
typedef InstrSetType PseudoCodeInstrSetType;
#define DBG_FMT_PseudoCodeInstrSetType		"type(%s) "
#define DBG_ARG_PseudoCodeInstrSetType(arg)	InstrSetTypeString(*(arg))


static inline const char *PseudoCodeSizeTypeString(uint32 size)
{
	switch (size) {
		case 1:
			return "";
		case 2:
			return "";
		case 4:
			return "";
		default:
			return "?";
	}
}
typedef uint32 PseudoCodeSizeType;
#define DBG_FMT_PseudoCodeSizeType		"%s"
#define DBG_ARG_PseudoCodeSizeType(arg)	PseudoCodeSizeTypeString(*(arg))

static inline const char *PseudoCodeSignTypeString(bool sign)
{
	return "";
}
typedef bool PseudoCodeSignType;
#define DBG_FMT_PseudoCodeSignType		"%s"
#define DBG_ARG_PseudoCodeSignType(arg)	PseudoCodeSignTypeString(*(arg))

typedef uint8 PseudoCodeBitCountType;
#define DBG_FMT_PseudoCodeBitCountType		"bitcount(%u) "
#define DBG_ARG_PseudoCodeBitCountType(arg)	(*(arg))

typedef uint8 PseudoCodeUnalignedAllowedType;
#define DBG_FMT_PseudoCodeUnalignedAllowedType		"UnalignedAllowd(%s) "
#define DBG_ARG_PseudoCodeUnalignedAllowedType(arg)	DbgBoolFlag(*(arg))

typedef uint32 PseudoCodeRegisterListType;
#define DBG_FMT_PseudoCodeRegisterListType		"rlist(0x%x) "
#define DBG_ARG_PseudoCodeRegisterListType(arg)	(*(arg))

typedef bool PseudoCodeRegisterFormType;
#define DBG_FMT_PseudoCodeRegisterFormType		"RegisterForm(%s) "
#define DBG_ARG_PseudoCodeRegisterFormType(arg)	DbgBoolFlag(*(arg))

typedef uint32 PseudoCodeOpcodeType;
#define DBG_FMT_PseudoCodeOpcodeType		"opcode(0x%x) "
#define DBG_ARG_PseudoCodeOpcodeType(arg)	(*(arg))

typedef enum {
	SRType_LSL = 0,
	SRType_LSR,
	SRType_ASR,
	SRType_ROR,
	SRType_RRX,
} SRType;

typedef SRType PseudoCodeSRType;

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
#define DBG_FMT_PseudoCodeSRType		"SRType(%s) "
#define DBG_ARG_PseudoCodeSRType(arg)	SRTypeString(*(arg))
typedef uint32 PseudoCodeShiftNType;
#define DBG_FMT_PseudoCodeShiftNType		"shift_n(%d) "
#define DBG_ARG_PseudoCodeShiftNType(arg)	(*(arg))

#endif /* _ARM_PSEUDO_CODE_COMMON_TYPE_H_ */