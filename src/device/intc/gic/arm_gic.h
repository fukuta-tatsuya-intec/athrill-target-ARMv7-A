#ifndef _ARM_GIC_H_
#define _ARM_GIC_H_

#include "std_types.h"
#include "assert.h"
#include "arm_gic_config.h"

typedef enum {
	GicIntrDetectType_LevelSensitive = 0,
	GicIntrDetectType_EdgeTriggered
} GicIntrDetectlType;

typedef enum {
	GicIntrType_PPI = 0,	// Private peripheral interrupt
	GicIntrType_SPI,		// Shared peripheral interrupt
	GicIntrType_SGI,		// Software-generated interrupt
} GicIntrType;

typedef struct {
	const char*					name;
	GicIntrType					type;
	/*
	 * Interrupt ID
	 */
	uint32						intrno;
	bool 						enable;
	uint32						priority;
	GicIntrDetectlType			dtype;
	/*
	 * not support Security Extensions
	 */
} GicInterruptType;

extern GicInterruptType	arm_gic_interrupt_table[GIC_INTR_NUM];

struct GicIntrCpuConnector;
typedef struct {
	uint32						id;
	bool						enable;
	struct GicIntrCpuConnector	*current_irq;
	struct GicIntrCpuConnector	*next_irq;
	uint32						priority_threshold;
	uint32						priority_mask;
} GicCpuInterfaceType;

extern GicCpuInterfaceType	arm_gic_cpu_interface_table[GIC_CPU_NUM];

typedef enum {
	GicIntrHandlingStateType_Inactive 		= 0x00,
	GicIntrHandlingStateType_Pending		= 0x01,
	GicIntrHandlingStateType_Active			= 0x02,
	GicIntrHandlingStateType_ActivePending	= 0x03,
} GicIntrHandlingStateType;

typedef struct GicIntrCpuConnector {
	GicIntrHandlingStateType	state;
	GicInterruptType 			*intr;
	GicCpuInterfaceType			*cpu_inf;
} GicIntrCpuConnectorType;
static inline uint32 CpuInterfaceCurrentPriority(GicCpuInterfaceType *cpu_inf)
{
	ASSERT(cpu_inf->current_irq != NULL);
	return cpu_inf->current_irq->intr->priority;
}

extern GicIntrCpuConnectorType	arm_gic_intr_cpu_connector_table[GIC_CONNECTOR_NUM];
extern void CpuInterfaceIntrAck(GicCpuInterfaceType *cpu_inf);
extern void CpuInterfaceIntrEoi(GicCpuInterfaceType *cpu_inf);
extern void GicInterruptAssertion(uint32 intno);

typedef struct {
	bool						enable;
	uint32						num;
	GicIntrCpuConnectorType		*connector;
} GicDistributorType;

extern GicDistributorType arm_gic_distributor;

extern void GIC_GenerateException(void);
#endif /* _ARM_GIC_H_ */
