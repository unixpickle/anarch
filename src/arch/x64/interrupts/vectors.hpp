#ifndef __ANARCH_X64_VECTORS_HPP__
#define __ANARCH_X64_VECTORS_HPP__

namespace anarch {

namespace x64 {

namespace IntVectors {

const uint8_t Pit = 0x20;
const uint8_t LapicTimer = 0x21;
const uint8_t Invlpg = 0x22;
const uint8_t Calibrate = 0xee;
const uint8_t Panic = 0xef;

};

}

}

#endif
