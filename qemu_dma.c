#include "qemu_dma.h"
#include "serial.h"

int memcmp_str(void *str1, void *str2, uint64_t count) {
    unsigned char *s1 = (unsigned char*)str1;
    unsigned char *s2 = (unsigned char*)str2;

    while (count-- > 0) {
        if (*s1++ != *s2++)
        return s1[-1] < s2[-1] ? -1 : 1;
    }
    return 0;
}

static void mmio_write_bsw64(uint64_t addr, uint64_t val) {
    volatile uint64_t *mmio_w = (volatile uint64_t*)addr;
    *mmio_w = __builtin_bswap64(val);
}

static void mmio_write64(uint64_t addr, uint64_t val) {
    volatile uint64_t *mmio_w = (volatile uint64_t*)addr;
    *mmio_w = val;
}

static void mmio_write_bsw16(uint64_t addr, uint16_t val) {
    volatile uint16_t *mmio_w = (volatile uint16_t*)addr;
    *mmio_w = __builtin_bswap16(val);
}

static void mmio_write16(uint64_t addr, uint16_t val) {
    volatile uint16_t *mmio_w = (volatile uint16_t*)addr;
    *mmio_w = val;
}


static uint64_t mmio_read_bsw64(uint64_t addr) {
    return __builtin_bswap64(*((volatile uint64_t*)addr));
}

static uint64_t mmio_read64(uint64_t addr) {
    return *((volatile uint64_t*)addr);
}

static uint16_t mmio_read_bsw16(uint64_t addr) {
    return __builtin_bswap16(*((volatile uint16_t*)addr));
}

static uint16_t mmio_read16(uint64_t addr) {
    return *((volatile uint16_t*)addr);
}

static uint16_t mmio_read_bsw32(uint64_t addr) {
    return __builtin_bswap32(*((volatile uint32_t*)addr));
}

static uint16_t mmio_read32(uint64_t addr) {
    return *((volatile uint32_t*)addr);
}

static void qemu_cfg_dma_transfer(void *address, uint32_t length, uint32_t control) {
    QemuCfgDmaAccess access = { .address = __builtin_bswap64((uint64_t)address), .length = __builtin_bswap32(length), .control = __builtin_bswap32(control) };

    if (length == 0) {
        return;
    }

    __asm__("ISB");

    mmio_write_bsw64(BASE_ADDR_ADDR, (uint64_t)&access);

    while(__builtin_bswap32(access.control) & ~QEMU_CFG_DMA_CTL_ERROR) {}
}

static void qemu_cfg_read(void *buf, int len) {
    qemu_cfg_dma_transfer(buf, len, QEMU_CFG_DMA_CTL_READ);
}

static void qemu_cfg_read_entry(void *buf, int e, int len) {
    uint32_t control = (e << 16) | QEMU_CFG_DMA_CTL_SELECT | QEMU_CFG_DMA_CTL_READ;
    qemu_cfg_dma_transfer(buf, len, control);
}

static void qemu_cfg_write_entry(void *buf, uint32_t e, uint32_t len) {
    uint32_t control = (e << 16) | QEMU_CFG_DMA_CTL_SELECT | QEMU_CFG_DMA_CTL_WRITE;
    qemu_cfg_dma_transfer(buf, len, control);
}

static int qemu_cfg_find_file() {
    uint32_t count, e, select;
    qemu_cfg_read_entry(&count, QEMU_CFG_FILE_DIR, sizeof(count));
    count = __builtin_bswap32(count);

    for (select = 0, e = 0; e < count; e++) {
        struct QemuCfgFile qfile;
        qemu_cfg_read(&qfile, sizeof(qfile));
        if (memcmp_str(qfile.name, "etc/ramfb", 10) == 0)
            select = __builtin_bswap16(qfile.select);
    }
    return select;
}

void check_fw_cfg_avail() {
    mmio_write16(BASE_ADDR_SELECTOR, 0x0000);
    cfg_sig_read.theInt = *((volatile uint32_t*)BASE_ADDR_DATA);
    if (cfg_sig_read.bytes[0] == 'Q' && cfg_sig_read.bytes[1] == 'E' && cfg_sig_read.bytes[2] == 'M' && cfg_sig_read.bytes[3] == 'U') {
        kprint("found cfg \n");
        if (mmio_read_bsw64(BASE_ADDR_ADDR) == 0x51454d5520434647) {
            kprint("guest dma interface enabled \n");
        }
        return;
    }
    kprint("did not find cfg \n");
}

void ramfb_setup_c(uint64_t heap_start) {
    uint32_t select = qemu_cfg_find_file();

    struct QemuRAMFBCfg cfg = {
        .addr   = __builtin_bswap64(heap_start),
        .fourcc = __builtin_bswap32(DRM_FORMAT_XRGB8888),
        .flags  = __builtin_bswap32(0),
        .width  = __builtin_bswap32(FRAMEBUFFER_WIDTH),
        .height = __builtin_bswap32(FRAMEBUFFER_HEIGHT),
        .stride = __builtin_bswap32(FRAMEBUFFER_STRIDE),
    };
    qemu_cfg_write_entry(&cfg, select, sizeof(cfg));
}

void ramfb_cfg_write_workaround(uint32_t select) {
	struct QemuRAMFBCfg cfg = {
		.addr   = __builtin_bswap64(0x4000),
		.fourcc = __builtin_bswap32(DRM_FORMAT_XRGB8888),
		.flags  = __builtin_bswap32(0),
		.width  = __builtin_bswap32(FRAMEBUFFER_WIDTH),
		.height = __builtin_bswap32(FRAMEBUFFER_HEIGHT),
		.stride = __builtin_bswap32(FRAMEBUFFER_STRIDE),
	};
    qemu_cfg_write_entry(&cfg, select, sizeof(cfg));
}
