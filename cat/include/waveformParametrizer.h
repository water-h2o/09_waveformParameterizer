//#ifndef DIFFUSION_H
#ifndef WAVEFORMPARAMETRIZER_H
#define WAVEFORMPARAMETRIZER_H

#include <vector>
#include <string>
#include <cmath>     // because of std::abs
#include <numeric>   // for std::accumulate
#include <algorithm> // for std::max_element

#include "myTypedefs.h"

// === MAIN FUNCTIONS ==================================================

a_Params getParams(a_signal in, 
                std::string filename,
				    nanosec PMresolution,
				        int verbosity);

// === HIGH LEVEL TOOLS ================================================

// ------- gP()A. ------------------------------------------------------ 

a_signal signalSansSeparatedBremsstrahlung(a_signal in,
                                              bool& hasSeparateBrem,
                                                int taperThreshold = 2);

// ------- gP()0. ------------------------------------------------------

std::string getID(std::string filename);

// ------- gP()1. ------------------------------------------------------

std::vector<ampNsec> getSignalCumAreas(a_signal in,
                                        nanosec PMresolution);

inline nanosec getSignalLength(a_signal in,
   nanosec PMresolution){return PMresolution * in.vector::size();};

// ------- gP()2. ------------------------------------------------------

void getPeakHeightsAndTime(a_signal in,                   // input
                            nanosec PMresolution,         // input
              std::vector<amp_arb>& signalPeakHeights,    // "output"
              std::vector<nanosec>& signalPeakTimes,      // "output"
              std::vector<amp_arb>& signalPlateauHeights, // "output"
              std::vector<nanosec>& signalPlateauTimes);  // "output"

std::vector<amp_arb> getHeightDistOfPeaks(
                      std::vector<amp_arb> peakHeights);

std::vector<nanosec> getTimeDistOfPeaks(std::vector<nanosec> peakTimes);
                                  
inline amp_arb getMaxHeight(std::vector<amp_arb> peakHeights){
	return *std::max_element(peakHeights.begin(), peakHeights.end()); };

void getMaxPk_HT(std::vector<amp_arb> peakHeights,
                 std::vector<nanosec> peakTimes,
                             amp_arb& maxHeight,
                             nanosec& pkTime);

// ------- gP()3. ------------------------------------------------------

//	// MERGED WITH gP()1

// ------- gP()4. ------------------------------------------------------

//	// EMPTY FOR NOW

//	// SATURATION FLAG (include later)

//	// SATURATED CHANNELS (include later) 

// ------- gP()5. ------------------------------------------------------

//	// EMPTY FOR NOW

// ------- gP()6. ------------------------------------------------------

std::vector<nanosec> getAfts(a_signal in,           // input
                              nanosec PMresolution, // input
                                 idx& aft5idx,      // "output"
                                 idx& aft95idx);     // "output"
                                 
// ------- gP()7. ------------------------------------------------------

nanosec getRMSwidth(a_signal in,           // input
                     nanosec PMresolution, // input
        std::vector<nanosec> afts,         // input
                         idx aft5idx,      // input
                         idx aft95idx,      // input
                    amp_arb& amp5_95,      // "output"
                        idx& idx5_95);     // "output"

inline amp_arb getSignalMean(ampNsec A5_95, 
                             nanosec T5_95){ return A5_95/T5_95; };

// ------- gP()8. ------------------------------------------------------

nanosec getFWHM(a_signal in,
                 nanosec PMresolution,
                 amp_arb signalHeight);

// ------- gP()9. ------------------------------------------------------

amp_arb getRMSamplitude(a_signal in,
                             idx aft5idx,
                             idx aft95idx);

// ------- gP()10. -----------------------------------------------------

//	// THINK OF WHAT TO FIT TO DO

//	// EXP PARAMETERS (AMPLITUDE, OFFSET, RISE, FALL)

//	// GAUS PARAMETERS (AMPLITUDE, MEAN, STDEV)

// ------- gP()11. -----------------------------------------------------

//	// MERGED WITH gP()2

// ------- gP()12. -----------------------------------------------------

double getPromptFraction50ns(a_signal in,
                              nanosec PMresolution,
                 std::vector<nanosec> afts,
                              ampNsec signalArea);

// ------- gP()13. -----------------------------------------------------

std::vector<ampNsec> getFixedWindowAreas(a_signal in,
                                          nanosec PMresolution,
                             std::vector<nanosec> afts);

// ------- gP()other ---------------------------------------------------

//	// COINCIDENCE (I don't even know what it is)

// === MID LEVEL TOOLS =================================================

void splitWP(const std::string& s, 
                           char c, 
                    a_sentence& v);

inline amp_arb dumbSmooth(a_signal in, 
           idx i,
           int n){return std::accumulate(in.begin() + i - n,
			in.begin() + i+1 + n, (double) 0)/(double) (n*2+1); } ;  

a_signal smooth(a_signal in,
                  double smooth_fraction);
//	// smoothing function aping the MATLAB one

//	// smooth_fraction in interval [0,1], represents the fraction of
//	// the signal to use during smoothing.

a_signal dumbDerivative(a_signal in);

std::vector<idx> getZeroIndices(a_signal in);

a_signal signalFlipFlopper(a_signal in,
                   std::vector<idx> zeroIndices);

// === LOW LEVEL TOOLS =================================================

inline amp_arb signalmaxAmplitude(a_signal in){
	return *std::max_element(in.begin(), in.end()); };

inline amp_arb signalSum(a_signal in){
	return std::accumulate(in.begin(), in.end(), (double)0); };

amp_arb signalAbsSum(a_signal in);

idx fracIdx(a_signal in,
              double fraction);

idx rFracIdx(a_signal in,
               double reverseFraction);

template <typename T>
inline void cat(std::vector<T>& L, 
                std::vector<T> R){ L.insert(L.end(), 
					                        R.begin(), 
					                        R.end() ); };

inline int signOf(amp_arb a){return (a >= 0) ? 1 : -1 ;};
inline bool sameSign(amp_arb a,
           amp_arb b){return (signOf(a) == signOf(b)) ? true : false ;};

inline idx nanosec2idx(nanosec in,
           nanosec PMresolution){return (int)(in / PMresolution); };
inline idx t2idx(nanosec in, 
          nanosec PMresolution){return nanosec2idx(in, PMresolution); };

a_sentence getParamNames();

std::vector<int> getRepetitions(a_Params In);

// === ERROR RETURNS ===================================================

// ERRORS GO HERE

#endif // WAVEFORMPARAMETRIZER_H
