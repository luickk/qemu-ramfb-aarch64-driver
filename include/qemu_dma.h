#include  <stdint.h>

#define QEMU_CFG_FILE_DIR               0x19

// QEMU_CFG_DMA_CONTROL bits
#define QEMU_CFG_DMA_CTL_ERROR   0x01
#define QEMU_CFG_DMA_CTL_READ    0x02
#define QEMU_CFG_DMA_CTL_SKIP    0x04
#define QEMU_CFG_DMA_CTL_SELECT  0x08
#define QEMU_CFG_DMA_CTL_WRITE   0x10

#define BASE_ADDR 0x09020000
#define BASE_ADDR_SELECTOR 0x9020008
#define BASE_ADDR_DATA 0x09020000
#define BASE_ADDR_ADDR 0x9020010

union FwCfgSigRead {
    uint32_t theInt;
    char bytes[sizeof(int)];
};

typedef struct {
    uint32_t control;
    uint32_t length;
    uint64_t address;
} __attribute__((__packed__)) QemuCfgDmaAccess;

struct __attribute__((__packed__)) QemuRAMFBCfg {
    uint64_t addr;
    uint32_t fourcc;
    uint32_t flags;
    uint32_t width;
    uint32_t height;
    uint32_t stride;
};

struct QemuCfgFile {
    uint32_t  size;        /* file size */
    uint16_t  select;      /* write this to 0x510 to read it */
    uint16_t  reserved;
    char name[56];
};

int check_fw_cfg_dma();
void kprint(uint8_t *print_string);

void qemu_cfg_write_entry(void *buf, uint32_t e, uint32_t len);
int qemu_cfg_find_file();