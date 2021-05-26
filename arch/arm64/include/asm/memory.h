#ifndef _YIOS_MEMORY_H_
#define _YIOS_MEMORY_H_

/*
 * Memory types available.
 */
#define MT_DEVICE_nGnRnE 0
#define MT_DEVICE_nGnRE 1
#define MT_DEVICE_GRE 2
#define MT_NORMAL_NC 3
#define MT_NORMAL 4
#define MT_NORMAL_WT 5

#define MAIR(attr, mt) ((attr) << ((mt)*8))

#endif
