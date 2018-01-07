#ifndef OPTIONMANAGER_H
#define OPTIONMANAGER_H

#include <map>

typedef void (*OnChange)();

class Option {
public:
  Option();
  Option(bool, bool, OnChange=nullptr); // Check
  Option(int, int, int, OnChange=nullptr); // Spin
  Option(const char*, OnChange=nullptr); // String

  std::string getValue() const;
  std::string getType() const;
  std::string getDefaultValue() const;
  int getMax() const;
  int getMin() const;

  void setValue(std::string);

private:
  std::string _value;
  std::string _type;
  std::string _defaultValue;
  int _min;
  int _max;
  OnChange _onChange;
};

extern std::map<std::string, Option> optionsMap;

#endif