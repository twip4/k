#ifndef ATAPI_H
#define ATAPI_H

#include <k/atapi.h>
#include <stddef.h>

#include "k/types.h"
// Handy register number defines
#define DATA 0
#define ERROR_R 1
#define SECTOR_COUNT 2
#define LBA_LOW 3
#define LBA_MID 4
#define LBA_HIGH 5
#define DRIVE_SELECT 6
#define COMMAND_REGISTER 7

// Control register defines
#define CONTROL 0x206

#define ALTERNATE_STATUS 0

extern u32 atapi_driver;
extern u32 atapi_bus;

void wait_400ns(int base);
int is_atapi_driver(int base);
void atapi_init(void);
void select_drive(u16 bus, u8 slave);
int send_packet(struct SCSI_packet* pkt);
void busy_wait(u16 drive);
void wait_device_selection(u16 drive);
void wait_packet_request(u16 drive);

void* read_block(u32 lba);

#endif /* !ATAPI_H */
