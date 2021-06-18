#ifndef PARAMETERFEATURIZER_H
#define PARAMETERFEATURIZER_H

#include <vector>
#include <string>

#include "consoleMessages.h"
#include "myTypedefs.h"

// === MAIN FUNCTIONS ==================================================

featHed getFeatureHeader(bool hasCategorization);

a_Features getFeatures(featHed featureHeader,
                      a_Params In,
                           int verbosity);

// === HIGH LEVEL TOOLS ================================================

a_sentence       getFeatureNamesAbbrev_hasCat();
std::vector<int> getFeatureRepetitions_hasCat();

a_sentence       getFeatureNamesAbbrev_nonCat();
std::vector<int> getFeatureRepetitions_nonCat();

// === MID LEVEL TOOLS =================================================



// === LOW LEVEL TOOLS =================================================



// === ERROR RETURNS ===================================================

// ERRORS GO HERE

#endif // PARAMETERFEATURIZER_H
