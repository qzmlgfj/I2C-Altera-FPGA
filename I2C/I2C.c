#include "stdio.h"
#include "unistd.h"
//#include "../Core_bsp/system.h"
//#include "../Core_bsp/HAL/inc/io.h"
//#include "../Core_bsp/drivers/inc/altera_avalon_pio_regs.h"
//#include "../Core_bsp/HAL/inc/alt_types.h"
#include "I2C.h"

void start(void)
{
    IOWR_ALTERA_AVALON_PIO_DIRECTION(SDA_BASE, OUT);
    IOWR_ALTERA_AVALON_PIO_DATA(SDA_BASE, 1);
    IOWR_ALTERA_AVALON_PIO_DATA(SCL_BASE, 1);
    usleep(10);
    IOWR_ALTERA_AVALON_PIO_DATA(SDA_BASE, 0);
    usleep(5);
}

void stop()
{
    IOWR_ALTERA_AVALON_PIO_DIRECTION(SDA_BASE, OUT);
    IOWR_ALTERA_AVALON_PIO_DATA(SDA_BASE, 0);
    IOWR_ALTERA_AVALON_PIO_DATA(SCL_BASE, 0);
    usleep(10);
    IOWR_ALTERA_AVALON_PIO_DATA(SCL_BASE, 1);
    usleep(5);
    IOWR_ALTERA_AVALON_PIO_DATA(SDA_BASE, 1);
    usleep(10);
}

//Receive slave's acknowledge
void ack() 
{
    alt_u8 tmp;
    IOWR_ALTERA_AVALON_PIO_DATA(SCL_BASE, 0);
    IOWR_ALTERA_AVALON_PIO_DIRECTION(SDA_BASE, IN);
    usleep(10);
    IOWR_ALTERA_AVALON_PIO_DATA(SCL_BASE, 1);
    usleep(5);
    tmp = IORD_ALTERA_AVALON_PIO_DATA(SDA_BASE);
    usleep(5);
    IOWR_ALTERA_AVALON_PIO_DATA(SCL_BASE, 0);
    usleep(10);
    while (tmp)
        ;
}


void WriteI2C(alt_u8 dat)
{
    alt_u8 tmp, i;
    IOWR_ALTERA_AVALON_PIO_DIRECTION(SDA_BASE, OUT);
    for (i = 0; i < 8; i++)
    {
        IOWR_ALTERA_AVALON_PIO_DATA(SCL_BASE, 0);
        usleep(5);
        tmp = (dat & 0x80) ? 1 : 0;
        dat <<= 1;
        IOWR_ALTERA_AVALON_PIO_DATA(SDA_BASE, tmp);
        usleep(5);
        IOWR_ALTERA_AVALON_PIO_DATA(SCL_BASE, 1);
        usleep(10);
    }
}

alt_u8 ReadI2C()
{
    alt_u8 i, dat = 0;
    IOWR_ALTERA_AVALON_PIO_DIRECTION(SDA_BASE, IN);
    for (i = 0; i < 8; i++)
    {
        IOWR_ALTERA_AVALON_PIO_DATA(SCL_BASE, 0);
        usleep(10);
        IOWR_ALTERA_AVALON_PIO_DATA(SCL_BASE, 1);
        usleep(5);
        dat <<= 1;
        dat |= IORD_ALTERA_AVALON_PIO_DATA(SDA_BASE);
        usleep(5);
    }
    usleep(5);
    IOWR_ALTERA_AVALON_PIO_DATA(SCL_BASE, 0);
    IOWR_ALTERA_AVALON_PIO_DATA(SDA_BASE, 0); //Send master's acknowledge
    usleep(10);
    IOWR_ALTERA_AVALON_PIO_DATA(SCL_BASE, 1);
    usleep(10);
    IOWR_ALTERA_AVALON_PIO_DATA(SCL_BASE, 0);
    return dat;
}

//Write 8 bits
void writeReg(alt_u16 addr, alt_u8 dat)
{
    alt_u8 cmd;
    alt_u8 addr_h1 = addr >> 8; //First half of the address
    alt_u8 addr_h2 = addr;      //Second half of the address

    cmd = 0x52; //master-write-to-the-slave
    start();
    WriteI2C(cmd);
    ack();
    WriteI2C(addr_h1);
    ack();
    WriteI2C(addr_h2);
    ack();
    WriteI2C(dat);
    ack();
    stop();
}

//Read 8 bits
alt_u8 readReg(alt_u16 addr)
{
    alt_u8 cmd, dat;
    alt_u8 addr_h1 = addr >> 8; //First half of the address
    alt_u8 addr_h2 = addr;      //Second half of the address

    cmd = 0x52; //master-write-to-the-slave
    start();
    WriteI2C(cmd);
    ack();
    WriteI2C(addr_h1);
    ack();
    WriteI2C(addr_h2);
    ack();
    stop();

    start();
    cmd |= 0x01; //master-read-from-the-slave
    start();
    WriteI2C(cmd);
    ack();
    dat = ReadI2C();
    stop();
    return dat;
}

//Write 16 bits
void writeReg16Bit(alt_u16 addr, alt_u16 dat)
{
    alt_u8 dat_h1 = dat >> 8; //First half of the data
    alt_u8 dat_h2 = dat;      //Second half of the data

    writeReg(addr, dat_h1);
    writeReg(addr + 1, dat_h2);
}

//Read 16 bits
alt_u16 readReg16Bit(alt_u16 addr)
{
    alt_u8 dat_h1;
    alt_u8 dat_h2;
    alt_u16 dat;

    dat_h1 = readReg(addr);
    dat_h2 = readReg(addr + 1);

    dat = (alt_u16)(dat_h1 << 8) | dat_h2;
    return dat;
}
