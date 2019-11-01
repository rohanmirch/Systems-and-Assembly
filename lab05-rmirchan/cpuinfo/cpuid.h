#ifndef CPUID_H
#define CPUID_H

#include <stdint.h>

typedef struct regs_t {
    uint32_t eax;
    uint32_t ebx;
    uint32_t ecx;
    uint32_t edx;
} regs_t;

const char * get_brand_string(uint32_t brand_index);

uint32_t cpuid_0(char *vendor_string, uint32_t *max_ext_value);

void cpuid_4(uint32_t ecx, regs_t *regs);
void cpuid_n(uint32_t eax, regs_t *regs);

uint32_t num_cores_in_package(void);

typedef struct cpuid_1_info {
    uint32_t ext_family_id;
    uint32_t ext_model_id;
    uint32_t proc_type;
    uint32_t family_id;
    uint32_t model;
    uint32_t stepping_id;
    uint32_t brand_index;
    uint32_t clflush_line_size;
    uint32_t cache_line_size;
    uint32_t max_addressable_ids;
    uint32_t initial_apic_id;
    uint32_t flag_apic;
    uint32_t flag_x2apic;
    uint32_t flag_multithreading;
} cpuid_1_info;

void cpuid_1(cpuid_1_info *info);

void enumerate_caches(void);

#endif /* CPUID_H */

