#ifndef UTIL_H
#define UTIL_H

// ----------------------------------------------------------------------------
// Include block.
// ----------------------------------------------------------------------------

// Standard library include.
#include <iostream>
#include <string>
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// Namespace block.
// ----------------------------------------------------------------------------

/// @brief This class is for all the utility funcitons needed for this project.
namespace Util
{
   // -------------------------------------------------------------------------
   // Namespace Functions block.
   // -------------------------------------------------------------------------

   /// @brief This function takes a string and turns it into an int.
   /// @param str The string we want to make an int.
   /// @return Returns the string but as an int.
   size_t CStrToInt(std::string str)
   {
      try 
      {
         size_t number = std::stoul(str);  // Convert string to unsigned long
         //std::cout << "Converted number: " << number << std::endl;
         return number;
      } catch (const std::invalid_argument& e) 
      {
         std::cerr << "Invalid argument: " << e.what() << std::endl;
      } catch (const std::out_of_range& e) 
      {
         std::cerr << "Out of range: " << e.what() << std::endl;
      }

      return -1;
   }

   /// @brief This function reads a UTF8 string and turns it into an
   ///        an actual string the computer can read.
   /// @param original_string The original string with maybe UTF8
   ///                        characters.
   /// @param sub_str_length The length of the string.
   /// @return The new string the computer can interpret.
   std::string UTF8SubStr(const std::string original_string, 
                          const int         sub_str_length)
   {
      int len = 0, byteIndex = 0;
      const char* aStr = original_string.c_str();
      size_t origSize = original_string.size();

      for (byteIndex = 0; byteIndex < origSize; byteIndex++)
      {
         if((aStr[byteIndex] & 0xc0) != 0x80)
            len += 1;

         if(len > sub_str_length)
            break;
      }

      return original_string.substr(0, byteIndex);
   }
   // -------------------------------------------------------------------------
} // Util
// ----------------------------------------------------------------------------

#endif
