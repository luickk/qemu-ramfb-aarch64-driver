#include  <stdint.h>

#define FRAMEBUFFER_WIDTH      1024
#define FRAMEBUFFER_HEIGHT     768
#define FRAMEBUFFER_BPP        4 // 32 bit
#define FRAMEBUFFER_STRIDE     (FRAMEBUFFER_BPP * FRAMEBUFFER_WIDTH)
#define FRAMEBUFFER_SIZE       (FRAMEBUFFER_STRIDE * FRAMEBUFFER_HEIGHT)

#define fourcc_code(a, b, c, d) ((uint32_t)(a) | ((uint32_t)(b) << 8) | \
                                 ((uint32_t)(c) << 16) | ((uint32_t)(d) << 24))


#define DRM_FORMAT_RGB565       fourcc_code('R', 'G', '1', '6') /* [15:0] R:G:B 5:6:5 little endian */
#define DRM_FORMAT_RGB888       fourcc_code('R', 'G', '2', '4') /* [23:0] R:G:B little endian */
#define DRM_FORMAT_XRGB8888     fourcc_code('X', 'R', '2', '4') /* [31:0] x:R:G:B 8:8:8:8 little endian */

int ramfb_setup_c(uint64_t fb);
void write_pixel_packed(uint64_t fb_addr, uint16_t x, uint16_t y, uint32_t stride, uint8_t pixel[4]);