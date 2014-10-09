#ifndef CONFIG_H
#define CONFIG_H

#define RAM_START 0x40000000
#define RAM_SIZE  0x04000000

#define TLB_ADDR  (RAM_START + RAM_SIZE - 0x8000)

#endif
