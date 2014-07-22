#ifndef __ANARCH_X64_ACPI_ROOT_HPP__
#define __ANARCH_X64_ACPI_ROOT_HPP__

#include <anarch/easy-map>
#include <ansa/nocopy>

namespace anarch {

namespace x64 {

class AcpiRoot : public ansa::NoCopy {
public:
  AcpiRoot(PhysAddr tableBase); // @noncritical
  
  // destructor is not declared but is also @noncritical
  
  virtual int GetCount() = 0; // @ambicritical
  virtual PhysAddr GetTable(int i) = 0; // @ambicritical
  
  /**
   * Find the index of the table going by the specified 4-letter name.
   * @return -1 if not found, an index otherwise
   * @ambicritical
   */
  virtual int FindTable(const char * name);
  
protected:
  PhysSize tableSize;
  EasyMap map;
};

}

}

#endif
