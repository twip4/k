#include "atapi.h"

#include <string.h>

#include "io.h"

u32 atapi_driver;
u32 atapi_bus;

void wait_400ns(int base)
{
    inb(ATA_REG_STATUS(base));
    inb(ATA_REG_STATUS(base));
    inb(ATA_REG_STATUS(base));
    inb(ATA_REG_STATUS(base));
}

int is_atapi_driver(int base)
{
    u8 atapi_sig[4] = { 0x01, 0x01, 0x14, 0xEB };

    u8 sig[4];
    sig[0] = inb(base + 2);
    sig[1] = inb(base + 3);
    sig[2] = inb(base + 4);
    sig[3] = inb(base + 5);

    return !memcmp(atapi_sig, sig, 4);
}

void atapi_init(void)
{
    int bus_base[] = { 0x1F0, 0x170 };
    int driver_base[] = { 0xA0, 0xB0 };

    outb(bus_base[0] + 0x206, 0x04);
    outb(bus_base[0] + 0x206, 0x02);

    outb(bus_base[1] + 0x206, 0x04);
    outb(bus_base[1] + 0x206, 0x02);

    for (int i = 0; i < 2; i++) // ATA BUS
    {
        for (int j = 0; j < 2; j++) // ATA DRIVER
        {
            outb(bus_base[i] + 0x6, driver_base[j]);

            wait_400ns(bus_base[i]);

            if (is_atapi_driver(bus_base[i]))
            {
                atapi_driver = driver_base[j];
                atapi_bus = bus_base[i];

                // printf("0x%x, 0x%x\n", atapi_driver, atapi_bus); // debug
                return;
            }
        }
    }
}

void busy_wait(u16 drive)
{
    while (inb(drive + 7) & 0x80)
        ;
}

void wait_device_selection(u16 drive)
{
    wait_400ns(drive);
    busy_wait(drive);
}

void wait_packet_request(u16 drive)
{
    while (1)
    {
        u8 status = inb(drive + 7);
        if (!(status & 0x80) && (status & 0x08))
            break;
    }
}

void select_drive(u16 bus, u8 slave)
{
    outb(bus + DRIVE_SELECT, slave); // Drive select
    wait_device_selection(bus);
}

int send_packet(struct SCSI_packet* pkt)
{
    select_drive(atapi_bus, atapi_driver);
    busy_wait(atapi_bus);

    outb(ATA_REG_FEATURES(atapi_bus), 0x00);
    outb(ATA_REG_SECTOR_COUNT(atapi_bus), 0x00);
    outb(ATA_REG_LBA_MI(atapi_bus), CD_BLOCK_SZ & 0xFF);
    outb(ATA_REG_LBA_HI(atapi_bus), CD_BLOCK_SZ >> 8);
    outb(ATA_REG_COMMAND(atapi_bus), PACKET);

    wait_packet_request(atapi_bus);

    u8* raw = (u8*)pkt;
    for (int j = 0; j < 6; j++)
        outw(ATA_REG_DATA(atapi_bus), raw[j * 2] | (raw[j * 2 + 1] << 8));

    return 0;
}

void* read_block(u32 lba)
{
    static u8 buf[CD_BLOCK_SZ];
    struct SCSI_packet packet = { .op_code = READ_12,
                                  .flags_lo = 0,
                                  .lba_hi = (lba >> 24) & 0xFF,
                                  .lba_mihi = (lba >> 16) & 0xFF,
                                  .lba_milo = (lba >> 8) & 0xFF,
                                  .lba_lo = (lba >> 0) & 0xFF,
                                  .transfer_length_hi = 0,
                                  .transfer_length_mihi = 0,
                                  .transfer_length_milo = 0,
                                  .transfer_length_lo = 1, // 1 secteur
                                  .flags_hi = 0,
                                  .control = 0 };
    send_packet(&packet);

    while (1)
    {
        u8 status = inb(ATA_REG_STATUS(atapi_bus));
        if (status & ERR)
            return 0;
        if (!(status & BSY) && (status & DRQ))
            break;
    }

    u16* ptr = (u16*)buf;
    for (int i = 0; i < CD_BLOCK_SZ / 2; i++)
        ptr[i] = inw(ATA_REG_DATA(atapi_bus));

    while (inb(ATA_REG_STATUS(atapi_bus)) & BSY)
        ;
    return buf;
}
