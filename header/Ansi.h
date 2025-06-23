#ifndef ANSI_H
#define ANSI_H

#include <iomanip>

class Ansi_t   // use ansi features of gnome-terminal, 
{              // or any ansi terminal
   // note: Ubuntu 15.10 gnome-terminal ansi term cursor locations 
   //       are 1-based, with origin 1,1 at top left corner

   enum ANSI : int { ESC = 27 }; // escape 

   public:

      static std::string clrscr(void)
         {
            std::stringstream ss;
            ss << static_cast<char>(ESC)<< "[H"   // home
               << static_cast<char>(ESC)<< "[2J"; // clrbos
            return(ss.str());
         }

      //       r/c are 0 based------v------v------0 based from C++
      static std::string gotoRC(int r, int c)
         {
            std::stringstream ss;
            // Note: row/col of my apps are 0 based (as in C++)
            // here I fix xy to 1 based, by adding 1 while forming output
            ss << static_cast<char>(ESC)<< "[" 
               << (r+1) << ';' << (c+1) << 'H';
            return(ss.str());
         }

   // tbr - add more ansi functions when needed

}; // Ansi_t

#endif
