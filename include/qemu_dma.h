#include  <stdint.h>

#define FRAMEBUFFER_WIDTH      1024
#define FRAMEBUFFER_HEIGHT     768
#define FRAMEBUFFER_BPP        4
#define FRAMEBUFFER_STRIDE     (FRAMEBUFFER_BPP * FRAMEBUFFER_WIDTH)
#define FRAMEBUFFER_SIZE       (FRAMEBUFFER_STRIDE * FRAMEBUFFER_HEIGHT)

#define fourcc_code(a, b, c, d) ((uint32_t)(a) | ((uint32_t)(b) << 8) | \
                                 ((uint32_t)(c) << 16) | ((uint32_t)(d) << 24))


#define DRM_FORMAT_RGB565       fourcc_code('R', 'G', '1', '6') /* [15:0] R:G:B 5:6:5 little endian */
#define DRM_FORMAT_RGB888       fourcc_code('R', 'G', '2', '4') /* [23:0] R:G:B little endian */
#define DRM_FORMAT_XRGB8888     fourcc_code('X', 'R', '2', '4') /* [31:0] x:R:G:B 8:8:8:8 little endian */

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
int ramfb_setup_c(uint64_t heap_start);
void kprint(uint8_t *print_string);