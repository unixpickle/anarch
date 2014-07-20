#ifndef __ANARCH_X64_RAW_ISR_HPP__
#define __ANARCH_X64_RAW_ISR_HPP__

extern "C" {

// these three routines are all I need to do in Assembly

void RawNonCodedIsr() __asm__("RawNonCodedIsr");
void RawCodedIsr() __asm__("RawCodedIsr");
void RawPicEoiIsr() __asm__("RawPicEoiIsr");

}

#endif
