#ifndef __ANARCH_X64_ACPI_MODULE_HPP__
#define __ANARCH_X64_ACPI_MODULE_HPP__

#include <ansa/module>
#include "acpi-root.hpp"
#include "apic-table.hpp"
#include "hpet-table.hpp"

namespace anarch {

namespace x64 {

class AcpiModule : public ansa::Module {
public:
  static void InitGlobal();
  static AcpiModule & GetGlobal();
  
  AcpiRoot & GetRoot();
  ApicTable * GetApicTable();
  HpetTable * GetHpetTable();
  
protected:
  virtual ansa::DepList GetDependencies();
  virtual void Initialize();
  
private:
  AcpiRoot * root;
  ApicTable * apicTable = NULL;
  HpetTable * hpetTable = NULL;
  
};

}

}

#endif
