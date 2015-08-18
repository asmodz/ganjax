#ifndef GANJAX_STDINT_H
#define GANJAX_STDINT_H

#define RAM_SEGMENT 0x07E0
#define KERNEL_SEGMENT 0x0500;
#define VIDEO_SEGMENT 0xB800;

typedef __int8  int8_t;
typedef __int16 int16_t;
typedef __int32 int32_t;
typedef __int64 int64_t;

typedef unsigned __int8  uint8_t;
typedef unsigned __int16 uint16_t;
typedef unsigned __int32 uint32_t;

static __segment video_area  = VIDEO_SEGMENT;
static __segment kernel_area = KERNEL_SEGMENT;
static __segment ram_area    = RAM_SEGMENT;

typedef char  __based(video_area)* int8_video_t;
typedef short __based(video_area)* int16_video_t;

typedef char  __based(ram_area)* int8_ram_t;
typedef short __based(ram_area)* int16_ram_t;

typedef char  __based(kernel_area)* int8_kernel_t;
typedef short __based(kernel_area)* int16_kernel_t;

typedef unsigned char  __based(video_area)* uint8_video_t;
typedef unsigned short __based(video_area)* uint16_video_t;

typedef unsigned char  __based(ram_area)* uint8_ram_t;
typedef unsigned short __based(ram_area)* uint16_ram_t;

typedef unsigned char  __based(kernel_area)* uint8_kernel_t;
typedef unsigned short __based(kernel_area)* uint16_kernel_t;

#define true 1
#define false 0
#define NULL 0

#endif
