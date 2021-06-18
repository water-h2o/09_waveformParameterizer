#ifndef READANTSOUTPUT_H
#define READANTSOUTPUT_H

#include <map>
#include <vector>
#include <string>
#include <algorithm>

#include "myTypedefs.h"

/*
typedef std::vector<std::string> a_column;
typedef std::map<std::string, a_column> a_cell;
typedef std::vector<std::string> a_sentence; // vector of short strings
typedef std::vector<double> a_signal;
*/

// important functions =================================================

a_cell readANTSfile(std::string fname);

void saveCellToFile(a_cell const& in,
               std::string out_fname);

a_signal sumChannels(a_cell const& in); // returns sum of all chans as 1 vector

void signalUpsideDown(a_signal& in);

// tools ===============================================================

void split(const std::string& s, 
                         char c, 
                  a_sentence& v);
                  
inline int b2i(bool b){return b ? 1 : 0;};

a_sentence getKeys(a_cell const& c);

std::vector<int> getLengths(a_cell const& c);

a_sentence getIdx_sen(a_cell const& c,
                         int idx);

std::string makeLine(a_sentence idx_sen); 

a_signal a_column2a_signal(a_column in);

a_column taperOutEmptyElements(a_column in);

#endif // READANTSOUTPUT_H

