
#include "Number.hh"
#include <cassert>

template <typename T> void Number::init(T &member, const T &val) {
  new (&member) std::string(val);
}

void Number::destroy() {
  if (m_type == ValueType::STRING) {
    using std::string;
    string_val.~string();
  }
}

void Number::copy(const Number &copy) {
  switch (copy.m_type) {
  case ValueType::INTEGER:
    int_val = copy.int_val;
    break;
  case ValueType::FLOAT:
    float_val = copy.float_val;
    break;
  case ValueType::CHAR:
    char_val = copy.char_val;
    break;
  case ValueType::STRING:
    init(string_val, copy.string_val);
  default:
    assert(false);
    break;
  }
}
