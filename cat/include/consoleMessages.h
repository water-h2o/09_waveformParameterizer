#ifndef CONSOLEMESSAGES_H
#define CONSOLEMESSAGES_H

#include <string>

// === TYPEDEFS ========================================================



// === STRUCTS =========================================================



// === MAIN FUNCTIONS ==================================================

void consoleMessage_WP(int section,     // serves to clean up code a bit
                       int verbosity,
                      char subsection = '?'); 
                      
void consoleMessage_PF(int section,
                       int verbosity,
                      char subsection = '?');
                      
void consoleMessage_main(int section,
                         int verbosity,
                        char subsection = '?',
                 std::string v_i = "N/A");

// === MID LEVEL TOOLS =================================================



// === LOW LEVEL TOOLS =================================================



// === ERROR RETURNS ===================================================

// ERRORS GO HERE

#endif // CONSOLEMESSAGES_H
