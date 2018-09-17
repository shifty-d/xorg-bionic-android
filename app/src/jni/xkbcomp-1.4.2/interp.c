#if defined(ANDROID) || defined(__ANDROID__)

#ifndef __LP64__
const char service_interp[] __attribute__((section(".interp"))) = "/system/bin/linker";
#else
const char service_interp[] __attribute__((section(".interp"))) = "/system/bin/linker64";
#endif

#endif
