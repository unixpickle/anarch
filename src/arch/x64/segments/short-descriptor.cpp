#include "short-descriptor.hpp"

namespace anarch {

namespace x64 {

ShortDescriptor::ShortDescriptor(bool privileged, bool executable)
  : limit(0), baseLow(0), baseMid(0), accessBit(0),
    writable(executable ? 0 : 1), direction(0),
    executable(executable ? 1 : 0), reservedOne(1),
    privilege(privileged ? 0 : 3), present(1), limitHigh(0), reservedZero(0),
    size(1), granularity(0), baseHigh(0) {
}

}

}
