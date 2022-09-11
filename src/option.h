#ifndef OPTIONMANAGER_H
#define OPTIONMANAGER_H

#include <map>
#include <string>

/**
 * @brief Type of callback function for when an option is changed
 */
typedef void (*OnChange)();

/**
 * @brief Represents a UCI option
 */
class Option {
 public:
  /**
   * @brief Constructs a new empty option
   */
  Option();

  /**
   * @brief Constructs a new UCI check option with the specified default value
   * and OnChange callback
   * 
   * @param value Initial and default value of this option 
   * @param onChange Pointer to function to be called when this option changes
   */
  Option(bool, OnChange= nullptr);

  /**
   * @brief Constructs a new UCI spin option with the specified default value
   * and OnChange callback
   * 
   * @param value Initial and default value of this option
   * @param min Minimum value of this spin option
   * @param max Maximum value of this spin option
   * @param onChange Pointer to function to be called when this option changes
   */
  Option(int, int, int, OnChange= nullptr);

  /**
   * @brief Constructs a new UCI string option with the specified default value
   * and OnChange callback
   * 
   * @param value Initial and default value of this option
   * @param onChange Pointer to function to be called when this option changes
   */
  Option(const char *, OnChange= nullptr); // String

  /**
   * @brief Gets the current value of this option
   * 
   * @return The current value of this option 
   */
  std::string getValue() const;

  /**
   * @brief Returns the type of this option
   * 
   * @return The type of this option
   */
  std::string getType() const;

  /**
   * @brief Returns the default value of this option
   * 
   * @return The default value of this option 
   */
  std::string getDefaultValue() const;

  /**
   * @brief Returns the minimum value of this option
   * 
   * The return value of this method is only valid if this is a "spin" option.
   * 
   * @return The maximum value of this option 
   */
  int getMin() const;

  /**
   * @brief Returns the maximum value of this option
   * 
   * The return value of this method is only valid if this is a "spin" option.
   * 
   * @return The maximum value of this option 
   */
  int getMax() const;

  /**
   * @brief Sets the value of this option to the specified string
   * 
   * @param value The value to set this option to
   */
  void setValue(std::string);

 private:
  /**
   * @brief Current value of this option
   */
  std::string _value;

  /**
   * @brief Type of this option
   * 
   * Currently, this can be one of "check", "spin" or "string"
   */
  std::string _type;

  /**
   * @brief Default value of this option
   */
  std::string _defaultValue;

  /**
   * @brief Minimum value of this option (only valid if this is a "spin"
   * option)
   */
  int _min;

  /**
   * @brief Maximum value of this option (only valid if this is a "spin"
   * option)
   */
  int _max;

  /**
   * @brief Pointer to function to be called when this option changes
   * 
   * If this is nullptr, no function will be called when this option changes.
   */
  OnChange _onChange;
};

/**
 * @brief Globally accessable mapping of option names to options
 */
extern std::map<std::string, Option> optionsMap;

#endif