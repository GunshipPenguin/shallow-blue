#ifndef UCI_H
#define UCI_H

#include "board.h"
#include "move.h"
#include "search.h"
#include "option.h"
#include "book.h"
#include <sstream>
#include <fstream>
#include <memory>

/**
 * @brief Class for handling UCI input/output.
 */
namespace Uci {
/**
 * @brief Initializes UCI options with their default values
 */
void init();

/**
 * @brief Starts listening for UCI input and responds to it when received
 */
void start();
};

#endif
