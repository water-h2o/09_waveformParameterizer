#ifndef FILEOUTPUT_H
#define FILEOUTPUT_H

#include <vector>

#include "myTypedefs.h"

// === MAIN FUNCTIONS ==================================================

a_sentence getFileHeaderBase(featHed in);

a_sentence a_Features2a_sentence_hasCat(a_Features In);
a_sentence a_Features2a_sentence_nonCat(a_Features In);

std::vector<int> vectorGatherMaxElements(std::vector<int> inA,  // I honestly don't know what this was for
                                         std::vector<int> inB);

// === HIGH LEVEL TOOLS ================================================



// === MID LEVEL TOOLS =================================================



// === LOW LEVEL TOOLS =================================================



// === ERROR RETURNS ===================================================

// ERRORS GO HERE

#endif // FILEOUTPUT_H
