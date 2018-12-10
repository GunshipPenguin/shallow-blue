#include "option.h"

std::map<std::string, Option> optionsMap;

Option::Option() :
    _value(""),
    _type(""),
    _defaultValue(""),
    _min(0),
    _max(0),
    _onChange(nullptr) {}

Option::Option(const char *value, OnChange onChange) :
    _value(value),
    _type("string"),
    _defaultValue(value),
    _min(0),
    _max(0),
    _onChange(onChange) {}

Option::Option(int value, int min, int max, OnChange onChange) :
    _value(std::to_string(value)),
    _type("spin"),
    _defaultValue(std::to_string(value)),
    _min(min),
    _max(max),
    _onChange(onChange) {}

Option::Option(bool value, OnChange onChange) :
    _value(value ? "true" : "false"),
    _type("check"),
    _defaultValue(value ? "true" : "false"),
    _min(0),
    _max(0),
    _onChange(onChange) {}

std::string Option::getValue() const {
  return _value;
}

std::string Option::getType() const {
  return _type;
}

std::string Option::getDefaultValue() const {
  return _defaultValue;
}

int Option::getMin() const {
  return _min;
}

int Option::getMax() const {
  return _max;
}

void Option::setValue(std::string value) {
  _value = value;
  if (_onChange != nullptr) _onChange();
}