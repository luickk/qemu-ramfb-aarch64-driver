#include  <stdint.h>

#define fourcc_code(a, b, c, d) ((uint32_t)(a) | ((uint32_t)(b) << 8) | \
                                 ((uint32_t)(c) << 16) | ((uint32_t)(d) << 24))


#define DRM_FORMAT_RGB565       fourcc_code('R', 'G', '1', '6') /* [15:0] R:G:B 5:6:5 little endian */
#define DRM_FORMAT_RGB888       fourcc_code('R', 'G', '2', '4') /* [23:0] R:G:B little endian */
#define DRM_FORMAT_XRGB8888     fourcc_code('X', 'R', '2', '4') /* [31:0] x:R:G:B 8:8:8:8 little endian */

typedef struct {
    uint64_t fb_addr;
    uint32_t fb_width;
    uint32_t fb_height;
    uint32_t fb_bpp;

    uint32_t fb_stride;
    uint32_t fb_size;
} fb_info;

int ramfb_setup(fb_info *fb);

void write_xrgb256_pixel(fb_info *fb, uint16_t x, uint16_t y, uint8_t pixel[4]);
void write_rgb256_pixel(fb_info *fb, uint16_t x, uint16_t y, uint8_t pixel[3]);
void draw_rgb256_map(fb_info *fb, uint32_t x_res, uint32_t y_res, uint8_t *rgb_map);