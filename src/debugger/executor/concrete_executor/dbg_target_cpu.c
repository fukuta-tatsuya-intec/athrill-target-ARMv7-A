#include "concrete_executor/target/dbg_target_cpu.h"
#include "cpu.h"
#include "symbol_ops.h"

#include <stdio.h>


static void print_register(const char* regname, uint32 addr, char* opt)
{
	 uint32 funcaddr;
	 int funcid;
	 uint32 gladdr;
	 int glid;

	 funcid = symbol_pc2funcid(addr, &funcaddr);
	 if (funcid >= 0) {
		printf("%s		0x%x %s(+0x%x)", regname, addr, symbol_funcid2funcname(funcid), addr - funcaddr);
	 }
	 else {
		glid = symbol_addr2glid(addr, &gladdr);
		if (glid >= 0) {
			printf("%s		0x%x %s(+0x%x)", regname, addr, symbol_glid2glname(glid), addr - gladdr);
		}
		else {
			printf("%s		0x%x", regname, addr);
		}
	 }
	 if (opt != NULL) {
		 printf(" %s\n", opt);
	 }
	 else {
		 printf("\n");
	 }
	return;
}

void dbg_target_print_cpu(uint32 coreId)
{
	int i;
	uint32 pc;
	char buffer[128];

	printf("***CPU<%d>***\n", coreId);
	pc = virtual_cpu.cores[coreId].core.reg.pc;

	print_register("PC", pc, NULL);
	for (i = 0; i < 32; i++) {
		 char *opt = NULL;
		 sprintf(buffer, "R%d", i);
		 switch (i) {
		 case 3:
			 opt = "Stack Pointer";
			 break;
		 case 10:
			 opt = "Return Value";
			 break;
		 case 6:
			 opt = "Arg1";
			 break;
		 case 7:
			 opt = "Arg2";
			 break;
		 case 8:
			 opt = "Arg3";
			 break;
		 case 9:
			 opt = "Arg4";
			 break;
		 default:
			 break;
		 }
		 print_register(buffer, virtual_cpu.cores[coreId].core.reg.r[i], opt);
	 }
	 print_register("EIPC", sys_get_cpu_base(&virtual_cpu.cores[coreId].core.reg)->r[SYS_REG_EIPC], NULL);
	 printf("EIPSW		0x%x\n", sys_get_cpu_base(&virtual_cpu.cores[coreId].core.reg)->r[SYS_REG_EIPSW]);
	 printf("ECR		0x%x\n", sys_get_cpu_base(&virtual_cpu.cores[coreId].core.reg)->r[SYS_REG_ECR]);
	 printf("PSW		0x%x\n", sys_get_cpu_base(&virtual_cpu.cores[coreId].core.reg)->r[SYS_REG_PSW]);
	 printf("SCCFG		0x%x\n", sys_get_cpu_base(&virtual_cpu.cores[coreId].core.reg)->r[SYS_REG_SCCFG]);
	 print_register("SCBP", sys_get_cpu_base(&virtual_cpu.cores[coreId].core.reg)->r[SYS_REG_SCBP], NULL);
	 printf("EIIC		0x%x\n", sys_get_cpu_base(&virtual_cpu.cores[coreId].core.reg)->r[SYS_REG_EIIC]);
	 printf("FEIC		0x%x\n", sys_get_cpu_base(&virtual_cpu.cores[coreId].core.reg)->r[SYS_REG_FEIC]);
	 print_register("FEPC", sys_get_cpu_base(&virtual_cpu.cores[coreId].core.reg)->r[SYS_REG_FEPC], NULL);
	 printf("FEPSW 		0x%x\n", sys_get_cpu_base(&virtual_cpu.cores[coreId].core.reg)->r[SYS_REG_FEPSW]);
	 printf("CTBP		0x%x\n", sys_get_cpu_base(&virtual_cpu.cores[coreId].core.reg)->r[SYS_REG_CTBP]);
	 printf("EIWR		0x%x\n", virtual_cpu.cores[coreId].core.reg.sys.sysreg[CPU_SYSREG_EIWR]);
	 printf("FEWR		0x%x\n", virtual_cpu.cores[coreId].core.reg.sys.sysreg[CPU_SYSREG_FEWR]);
	 printf("BSEL		0x%x\n", virtual_cpu.cores[coreId].core.reg.sys.sysreg[CPU_SYSREG_BSEL]);
	 //printf("SW_CTL		0x%x\n", virtual_cpu.cores[coreId].core.reg.sys.grp[SYS_GRP_CPU][SYS_GRP_CPU_BNK_1].r[SYS_REG_SW_CTL]);
	 //printf("SW_CFG		0x%x\n", virtual_cpu.cores[coreId].core.reg.sys.grp[SYS_GRP_CPU][SYS_GRP_CPU_BNK_1].r[SYS_REG_SW_CFG]);
	 //printf("SW_BASE		0x%x\n", virtual_cpu.cores[coreId].core.reg.sys.grp[SYS_GRP_CPU][SYS_GRP_CPU_BNK_1].r[SYS_REG_SW_BASE]);

	 cpu_debug_print_mpu_status(coreId);
	 return;
}
