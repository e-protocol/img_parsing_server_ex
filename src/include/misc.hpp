#pragma once
#include <iostream>

/**
 * @brief funtion to print console 
 */
static void PrintTerminal(const std::string& str, bool newLine = true)
{
  std::cout << str;
  if(newLine) std::cout << std::endl;
}