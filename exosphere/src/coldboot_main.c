#include <string.h>
#include "utils.h"
#include "mmu.h"
#include "memory_map.h"
#include "arm.h"
#include "cpu_context.h"

extern uint8_t __pk2ldr_start__[], __pk2ldr_end__[];

void coldboot_main(void) {
    uintptr_t *mmu_l3_table = (uintptr_t *)TZRAM_GET_SEGMENT_ADDRESS(TZRAM_SEGMENT_ID_L3_TRANSLATION_TABLE);
    uintptr_t pk2ldr = TZRAM_GET_SEGMENT_ADDRESS(TZRAM_SEGMENT_ID_PK2LDR);

    /* Clear and unmap pk2ldr (which is reused as exception entry stacks) */
    memset((void *)pk2ldr, 0, __pk2ldr_end__ - __pk2ldr_start__);
    mmu_unmap_range(3, mmu_l3_table, pk2ldr, __pk2ldr_end__ - __pk2ldr_start__);
    tlb_invalidate_all_inner_shareable();

    core_jump_to_lower_el();
}
