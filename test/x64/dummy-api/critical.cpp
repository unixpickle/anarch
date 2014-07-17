namespace anarch {

namespace {

bool ignoreCriticality = true;
bool critical = true;

}

namespace dummy {

void SetIgnoreCriticality(bool flag) {
  ignoreCriticality = flag;
}

}

bool IgnoreCriticality() {
  return ignoreCriticality;
}

bool GetCritical() {
  return critical;
}

void SetCritical(bool flag) {
  critical = flag;
}

}
