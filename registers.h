#ifndef REGISTERS_H
#define REGISTERS_H

typedef enum
{
    r0 = 0,
    r1 = 1,
    r2 = 2,
    r3 = 3,
    r4 = 4,
    r5 = 5,
    r6 = 6,
    r7 = 7
} Register;

char *register_names[] = {"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7"};

int register_addreses[] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07};

typedef struct
{
    unsigned int zero_flag : 1;
    unsigned int negetive_flag : 1;
    unsigned int carry_flag : 1;
    unsigned int overflow_flag : 1;
} psw_register;

#endif
