#include <iostream>
#include "waveformParametrizer.h"

#include <string>
#include <vector>
#include <numeric>   // for std::accumulate
#include <math.h>
#include <algorithm> // for std::max_element, std::max
//~ #include <numeric>   // for std::accumulate

#include "consoleMessages.h"
#include "myTypedefs.h"

using namespace std;




// =====================================================================
// === MAIN FUNCTION ===================================================
// =====================================================================



a_Params getParams(a_signal in, 
                     string filename,
                    nanosec PMresolution,
                        int verbosity){
	
	a_Params Out;
	
	consoleMessage_WP(-1, verbosity);
	
	
	
	//  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
	// A. Before parametrization, cut out bremsstrahlung
	//  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
	
	
	
	consoleMessage_WP(-1, verbosity, 'A');
	
	a_signal in_signal = signalSansSeparatedBremsstrahlung(in,
	                                      Out.hasSeparateBrem);         
	
	
	
	//  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
	// 0. Waveform ID
	//  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
	
	
	
	consoleMessage_WP(0, verbosity);
	
	Out.ID = getID(filename);
	
	
	
	//  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
	// 1. Find total/positive/negative pulse areas
	//  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
	
	
	
	consoleMessage_WP(1, verbosity);
	
	//	// signal should be consistently a positive one, so no
	//	// positive / negative area needed?
	
	Out.signalCumAreas = getSignalCumAreas(in_signal, PMresolution);
	Out.signalLength   = getSignalLength(  in_signal, PMresolution);
	
	
	
	//  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
	// 2. Find pulse height 
	//  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
	
	
	
	consoleMessage_WP(2, verbosity);
	
	getPeakHeightsAndTime(in_signal, PMresolution, 
	          Out.signalPeakHeights, Out.signalPeakTimes, 
	       Out.signalPlateauHeights, Out.signalPlateauTimes);
	
	Out.signalPeakHeightDists = getHeightDistOfPeaks(Out.signalPeakHeights);
	Out.signalPeakTimeDists = getTimeDistOfPeaks(Out.signalPeakTimes);
	
	amp_arb maxPeakHeight = 0;
	nanosec maxPeakTime   = 0;
	
	amp_arb maxPlatHeight = 0;
	nanosec maxPlatTime   = 0;
	
	getMaxPk_HT(Out.signalPeakHeights,
	            Out.signalPeakTimes,
	            maxPeakHeight, maxPeakTime);
	
	getMaxPk_HT(Out.signalPlateauHeights,
	            Out.signalPlateauTimes,
	            maxPlatHeight, maxPlatTime);
	
	Out.signalMaxHeight        = maxPeakHeight;
	Out.signalMaxPeakTime      = maxPeakTime;
	Out.signalMaxPlateauHeight = maxPlatHeight;
	Out.signalMaxPlateauTime   = maxPlatTime;
	
	
	
	//  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
	// 3. Find pulse length -> pulseLength
	//  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
	
	
	
	//	// MERGED WITH 1.
	
	
	
	//  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
	// 4. Find saturation flag -> saturatedFlag, saturatedChannelIDs
	//  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
	
	
	
	//	// EMPTY FOR NOW
	
	
	
	//  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
	// 5. Find top, bottom areas and top/bottom
	//        assymetry -> topBottomAsymmetry, topArea, bottomArea
	//  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
	
	
	
	//	// EMPTY FOR NOW
	
	
	
	//  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
	// 6. Find area fraction times
	//  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
	
	
	
	consoleMessage_WP(6, verbosity);
	
	idx aft5idx, aft95idx;
	Out.afts = getAfts(in_signal, PMresolution, aft5idx, aft95idx);
	
	for(int i = 0; i < Out.afts.size(); i++)
		Out.afts_idx.push_back(Out.afts[i] / PMresolution);
	
	
	
	//  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
	// 7. Find RMSWidth ; Find pulseMean 
	//  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
	
	
	
	consoleMessage_WP(7, verbosity);
	
	amp_arb amp5_95;
	idx     idx5_95;
	
	Out.RMSwidth = getRMSwidth(in_signal, PMresolution, Out.afts,
	                           aft5idx, aft95idx, amp5_95, idx5_95);

	nanosec t5_95 = idx5_95 * PMresolution;

	Out.signalMean = getSignalMean(amp5_95, t5_95);
	
	
	
	//  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
	// 8. find FHWM - calculated between RoI bondaries
	//  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
	
	
	
	consoleMessage_WP(8, verbosity);
	
	Out.FWHM = getFWHM(in_signal, PMresolution, Out.signalMaxHeight);
	
	
	
	//  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
	// 9. Find RMSAmplitude within the pulse boundary
	//  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
	
	
	
	consoleMessage_WP(9, verbosity);
	
	Out.RMSamplitude = getRMSamplitude(in_signal, aft5idx, aft95idx);
	
	
	
	//  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
	// 10. Fit the pulse with double exponential and Gaussian
	//         functions -> expParameters, gausParameters
	//  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
	
	
	
	//	// maybe more appropriate to fit to other functions
	
	
	
	//  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
	// 11. Find pulse height time - identifies the first point where
	//         height is reached -> pulseHeightTime
	//  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
	
	
	
	//	// MERGED WITH 2.
	
	
	
	//  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
	// 12. Find the prompt fraction
	//  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
	
	
	
	consoleMessage_WP(12, verbosity);
	
	Out.promptFraction50ns = getPromptFraction50ns(in_signal, 
	                 PMresolution, Out.afts, Out.signalCumAreas.back());
	
	
	
	//  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
	// 13. Find the fixed integration window areas
	//  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
	
	
	
	consoleMessage_WP(13, verbosity);
	
	Out.fixedWindowAreas = getFixedWindowAreas(in_signal, PMresolution, 
	                                                         Out.afts);
	
	
	
	//  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
	// END. Get parameter names
	//  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
	
	
	
	consoleMessage_WP(-1, verbosity, 'E');
	
	Out.paramNames = getParamNames();
	
	return Out;	
}



// =====================================================================
// === HIGH LEVEL TOOLS ================================================
// =====================================================================




// ------- gP()0. ------------------------------------------------------

string getID(string filename){

	string out;
	
	a_sentence fname_extension_tuplelike;
	splitWP(filename, '.', fname_extension_tuplelike);
	out = fname_extension_tuplelike[0];
	
	return out;
}

// ------- gP()1. ------------------------------------------------------

vector<ampNsec> getSignalCumAreas(a_signal in,
                                   nanosec PMresolution){
	
	vector<ampNsec> out (in.size(), 0.0);
	
	ampNsec cum = 0.0;
	
	for(int i = 0; i < in.size(); i++){
		
		cum += std::abs(in[i]) * PMresolution;
		out[i] = cum;
	}
	
	return out;
}

// ------- gP()2. ------------------------------------------------------

void getPeakHeightsAndTime(a_signal in,
                            nanosec PMresolution,
                   vector<amp_arb>& signalPeakHeights,
                   vector<nanosec>& signalPeakTimes,
                   vector<amp_arb>& signalPlateauHeights,
                   vector<nanosec>& signalPlateauTimes){ 

	//  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
	//  - A. prep 
	//  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
	
	
	
	// EVENTUALLY USE SAVITSKY-GOLAY FOR SMOOTHING?
	
	a_signal y_smooth   = smooth(in , 0.04);
	
	a_signal dy_coarse  = dumbDerivative(y_smooth  );
	a_signal dy_smooth  = smooth(dy_coarse  , 0.06 );
	
	a_signal d2y_coarse = dumbDerivative(dy_smooth );
	a_signal d2y_smooth = smooth(d2y_coarse , 0.06 );
	
	a_signal d3y_coarse = dumbDerivative(d2y_smooth);
	a_signal d3y_smooth = smooth(d3y_coarse , 0.06 );
	
	
	
	//  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
	//  - B. find peaks of signal 
	//  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
	
	
	
	//	// zeros in the 1st derivative correspond to peaks & troughs.
	//	// If the 2nd derivative is larger than zero, then it's a peak
	
	vector<idx> dy_smooth_zeros = getZeroIndices(dy_smooth);
	
	for(int i = 0; i < dy_smooth_zeros.size(); i++){
		
		if(d2y_smooth[dy_smooth_zeros[i]] < 0){
			
			signalPeakHeights.push_back(y_smooth[dy_smooth_zeros[i]]);
			
			nanosec pkTime = dy_smooth_zeros[i] * PMresolution;
			signalPeakTimes.push_back(pkTime);
		}
	}
	
	
	
	//  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
	//  - C. find plateaus of signal 
	//  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
	
	
	
	//	// the signal plateaus are going to produce troughs in the 
	//	// absolute value of the 1st derivative.
	
	a_signal d3y_smooth_fF = signalFlipFlopper(d3y_smooth,
	                                            dy_smooth_zeros);
	
	vector<idx> d2y_smooth_0 = getZeroIndices(d2y_smooth);
	
	if(!d2y_smooth_0.empty()){ 
	
		for(int i = 0; i < d2y_smooth_0.size(); i++){
		
			if(d3y_smooth_fF[d2y_smooth_0[i]] > 0){
			
			  signalPlateauHeights.push_back(y_smooth[d2y_smooth_0[i]]);
			  
			  nanosec plTime = d2y_smooth_0[i] * PMresolution;
			
			  signalPlateauTimes.push_back(plTime);
			}
		}
	}
}

vector<amp_arb> getHeightDistOfPeaks(vector<amp_arb> peakHeights){

	vector<amp_arb> out;
	
	if(peakHeights.size() > 1){
	
		for(int i = 1; i < peakHeights.size(); i++){
		
			out.push_back(peakHeights[i] - peakHeights[i-1]);
		}
	}
	
	return out;
}

vector<nanosec> getTimeDistOfPeaks(vector<nanosec> peakTimes){
	
	vector<nanosec> out;
	
	if(peakTimes.size() > 1){
		
		for(int i = 1; i < peakTimes.size(); i++){
			
			out.push_back(peakTimes[i] - peakTimes[i-1]);
		}
	}
	
	return out;
}

void getMaxPk_HT(std::vector<amp_arb> pkHeights,
                 std::vector<nanosec> pkTimes,
                             amp_arb& maxHeight,
                             nanosec& maxPkTime){

	if (!pkHeights.empty()){

	std::vector<amp_arb>::iterator result;
	result = std::max_element(pkHeights.begin(), pkHeights.end());
	idx max_idx = std::distance(pkHeights.begin(), result);
	
	maxPkTime = pkTimes[max_idx];
	maxHeight = *std::max_element(pkHeights.begin(), pkHeights.end());
	
	} else {
		
		maxHeight = -1;
		maxPkTime = -1;
	}
}

// ------- gP()3. ------------------------------------------------------

// 	// MERGED WITH gP()1

// ------- gP()4. ------------------------------------------------------

// 	// EMPTY FOR NOW

//	// SATURATION FLAG (include later)

//	// SATURATED CHANNELS (include later) 

// ------- gP()5. ------------------------------------------------------

//	// EMPTY FOR NOW

// ------- gP()6. ------------------------------------------------------

vector<nanosec> getAfts(a_signal in,
                         nanosec PMresolution,
                            idx& aft5idx,
                            idx& aft95idx){

	vector<nanosec> out;

	vector<double> fracs {0.01,0.05,0.1,0.25,0.5,0.75,0.9,0.95,0.99};
	
	for(int i = 0; i < fracs.size(); i++){
		
		out.push_back(fracIdx(in, fracs[i]) * PMresolution);
	}
	
	aft5idx  = nanosec2idx(out[1], PMresolution); // aka t2idx (using
	aft95idx = nanosec2idx(out[7], PMresolution); // that name from now)
	
	return out;
}
                                 
// ------- gP()7. ------------------------------------------------------

nanosec getRMSwidth(a_signal in,
                     nanosec PMresolution,
             vector<nanosec> afts,
                         idx aft5idx,
                         idx aft95idx,
                    amp_arb& amp5_95,
                        idx& idx5_95){

	nanosec out;
	
	double acc5_95 = 0;
	amp5_95        = 0;
	idx5_95        = aft95idx - aft5idx + 1; 
	
	nanosec tau = afts[4]; // average time of pulse
	
	for(int i = aft5idx; i < aft95idx ; i++){
		
		nanosec t_i   = i * PMresolution;
		nanosec delta = t_i - tau;
		
		acc5_95 += in[i]*pow(delta,2);
		amp5_95 += in[i];
	}
	
	out = sqrt(acc5_95 / amp5_95);
	
	return out;
}

// ------- gP()8. ------------------------------------------------------

nanosec getFWHM(a_signal in,
                 nanosec PMresolution,
                 amp_arb signalHeight){

	nanosec out;
	
	nanosec halfMaxTime_L;
	nanosec halfMaxTime_R;
	
	for(int i_L = 0; i_L < in.size(); i_L++){
		
		if(in[i_L] >= (double)(signalHeight/2)){
			
			halfMaxTime_L = i_L * PMresolution;
			break;
		}
	}
	
	for(int i_R = in.size()-1; i_R >= 0; i_R--){
		
		if(in[i_R] >= (double)(signalHeight/2)){
			
			halfMaxTime_R = i_R * PMresolution;
			break;
		}
	}
	
	out = halfMaxTime_R - halfMaxTime_L;
	
	return out;
}

// ------- gP()9. ------------------------------------------------------

amp_arb getRMSamplitude(a_signal in,
                             idx aft5idx,
                             idx aft95idx){
	
	amp_arb out;
	
	idx idx5_95 = aft95idx - aft5idx + 1;
	double acc = 0;
	
	for(int i = aft5idx ; i < aft95idx ; i++){
		
		acc += pow(in[i],2);
	}
	
	out = sqrt( (double)(acc / idx5_95) );
	
	return out;
}

// ------- gP()10. -----------------------------------------------------

// THINK OF WHAT TO FIT TO DO

// EXP PARAMETERS (AMPLITUDE, OFFSET, RISE, FALL)

// GAUS PARAMETERS (AMPLITUDE, MEAN, STDEV)

// ------- gP()11. -----------------------------------------------------

// MERGED WITH gP()2

// ------- gP()12. -----------------------------------------------------

double getPromptFraction50ns(a_signal in,
                              nanosec PMresolution,
                      vector<nanosec> afts,
                              ampNsec signalArea){

	double out;
	
	amp_arb tot = signalArea / PMresolution;
	
	idx iLim_L = (afts[1]-10>=0) ? t2idx(afts[1]-10, PMresolution) : 0;
	idx iLim_R = iLim_L + t2idx(50,PMresolution) + 1;
	
	amp_arb acc = 0;
	
	for(int i = iLim_L; i < iLim_R ; i++){
		
		acc += in[i];
	}
	
	out = (double)(acc / tot);
	
	return out;
}

// ------- gP()13. -----------------------------------------------------

vector<ampNsec> getFixedWindowAreas(a_signal in,
                                     nanosec PMresolution,
                             vector<nanosec> afts){

	vector<ampNsec> out;
	
	nanosec piv = afts[1]; // pivot (needed smaller variable names :p)
	vector<nanosec> bds {-50,50,100,200,500,1000,2000,5000}; // bounds
	
	vector<vector<idx>> windowIdxs;
	
	
	
	//  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
	//  - setting up the integration limits
	//  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
	
	
	
	vector<idx> fstWin(2, t2idx(piv, PMresolution) + 1);
	fstWin[0] += (piv+bds[0]>=0) ? t2idx(bds[0],PMresolution) : -piv-1;
	
	windowIdxs.push_back(fstWin);
	
	for(int i = 1; i < bds.size(); i++){
		
		vector<idx> nextWindow(2, t2idx(piv, PMresolution) + 1);
		
		if(t2idx(piv + bds[i],PMresolution) <= in.size()){
			
			nextWindow[1] = t2idx(piv + bds[i], PMresolution);
			
		} else {
			
			nextWindow[1] = in.size();
		}
		
		windowIdxs.push_back(nextWindow);
	}
	
	
	
	//  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
	//  - performing integration
	//  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
	
	
	
	vector<amp_arb> hitsPerWindow;
	
	for(int i = 0; i < windowIdxs.size(); i++){
		
		amp_arb acc = 0;
		
		for(int j = windowIdxs[i][0]; j < windowIdxs[i][1]; j++){
			
			acc += std::abs(in[j]);
		}
		
		hitsPerWindow.push_back(acc);
	}
	
	
	
	//  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
	//  - correcting units
	//  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
	
	
	
	for(int i = 0; i < hitsPerWindow.size(); i++){

		out.push_back(hitsPerWindow[i] * PMresolution);
	}
	
	return out;
}

// ------- gP()other ---------------------------------------------------

// COINCIDENCE (I don't even know what it is)
// it's the number of PMTs that were turned on



// =====================================================================
// === MID LEVEL TOOLS =================================================
// =====================================================================


void splitWP(const string& s, 
                    char c, 
             a_sentence& v){ // from O'reilly
	
	string::size_type i = 0;
	string::size_type j = s.find(c);

	while (j != string::npos) {
		v.push_back(s.substr(i, j-i));
		i = ++j;
		j = s.find(c, j);

		if (j == string::npos)
			v.push_back(s.substr(i, s.length()));
	}
	
	if(v.size() < 1){ // if no matches, return the entire string
      
		v.push_back(s);
       
	}
	
}

a_signal signalSansSeparatedBremsstrahlung(a_signal in,
                                              bool& hasSeparateBrem,
                                                int taperThreshold){
	
	int nChainedNonzeros = 0;
	
	idx start_L = 0;
	idx start_R = in.size();
	
	
	
	//  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
	//  - tapering (IS THIS EVEN USED FOR ANYTHING????)
	//  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
	
	
	
	for(int i_L = 0; i_L < in.size(); i_L++){
		
		nChainedNonzeros = (in[i_L] > 0) ? nChainedNonzeros+1 : 0;
		
		if(nChainedNonzeros > taperThreshold){
			
			start_L = i_L - taperThreshold;
		}
	}
	
	nChainedNonzeros = 0;
	
	for(int i_R = in.size() - 1; i_R >= 0; i_R--){
		
		nChainedNonzeros = (in[i_R] > 0) ? nChainedNonzeros+1 : 0;
		
		if(nChainedNonzeros > taperThreshold){
			
			start_R = i_R + taperThreshold + 1;
		}
	}
	
	
	
	//  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
	//  - finding pulse limits
	//  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
	
	
	
	vector<vector<idx>> pulseLims_LR;   // stores all pulse limits
	
	vector<idx> curr_pulseLims_LR(2,0); // stores 1st and last idx of curr pulse
	
	bool curr_havePulse = true;
	//~ nChainedNonzeros = 0;  // REMOVED FOR NOW, PROBABLY 
	//~ int nChainedZeros = 0; // NOT EVEN NECESSARY
	
	for(int i = 0; i < in.size(); i++){
		
		if(in[i] == 0 && curr_havePulse){
			
			curr_havePulse = false;
			curr_pulseLims_LR[1] = i;
			pulseLims_LR.push_back(curr_pulseLims_LR);
			
			curr_pulseLims_LR[0] = 0;
			curr_pulseLims_LR[1] = 0;
		}
		
		if(in[i] != 0 && !curr_havePulse){
			
			curr_havePulse = true;
			curr_pulseLims_LR[0] = i;
		}
	}
	
	hasSeparateBrem = (pulseLims_LR.size() > 1) ? true : false ;
	
	
	
	//  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
	//  - selecting the pulse with largest area as the signal
	//  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
	
	
	
	vector<amp_arb> hit_sums(pulseLims_LR.size(), 0);
	
	for(int i = 0; i < pulseLims_LR.size(); i++){
		
		a_signal::const_iterator lim_L= in.begin() + pulseLims_LR[i][0];
		a_signal::const_iterator lim_R= in.begin() + pulseLims_LR[i][1];
		a_signal inExcerpt(lim_L, lim_R);
		
		hit_sums[i] = signalAbsSum(inExcerpt);
	}
	
	vector<amp_arb>::const_iterator hsb = hit_sums.begin();
	vector<amp_arb>::const_iterator hse = hit_sums.end();
	vector<amp_arb>::const_iterator max_it = max_element(hsb,hse);
	idx m_i = max_it - hit_sums.begin();
	
	a_signal::const_iterator out_L = in.begin() + pulseLims_LR[m_i][0];
	a_signal::const_iterator out_R = in.begin() + pulseLims_LR[m_i][1];
	a_signal out(out_L, out_R);
	
	return out;
}

a_signal smooth(a_signal in,
                  double smooth_fraction){

	a_signal out(in.size(), 0);

	int n = (int)(in.size()*smooth_fraction);
	
	for(int i = 0; i < (int)(in.size()/2); i++){
		
		int curr_n = (i < (int)(n/2)) ? i : (int)(n/2); // tapering

		out[i] = dumbSmooth(in, i, curr_n);
	}

	for(int i = (int)(in.size()/2); i < in.size(); i++){
		
		int curr_n = ((in.size()-i-1)< (int)(n/2))? (in.size()-i-1):  (int)(n/2);
		
		out[i] = dumbSmooth(in, i, curr_n);
	}
	
	return out;
}

a_signal dumbDerivative(a_signal in){

	a_signal out(in.size(), 0);

	a_signal in_shift(in);
	in_shift.erase(in_shift.begin());
	in_shift.push_back(in_shift.back());

	for(int i = 0; i < in.size(); i++){
		
		out[i] = in_shift[i]-in[i];
	}
	
	return out;
}

vector<idx> getZeroIndices(a_signal in){
	
	vector<idx> out;
	
	a_signal in_shift(in);
	in_shift.erase(in_shift.begin());
	in_shift.push_back(in_shift.back());
	
	for(int i = (int)fracIdx(in,0.01); i < (int)rFracIdx(in,0.01); i++){
		
		if(!sameSign(in_shift[i],in[i]) ){
			
			out.push_back(i);
		}
	}
	
	return out;
}

a_signal signalFlipFlopper(a_signal in,
                        vector<idx> zeroIndices){

	a_signal out(in.size(),0);
	
	vector<bool> currPositive(1,true);
	for(int i = 0; i < zeroIndices.size(); i++){

		currPositive.push_back( (i%2 == 0) ? false : true );
	}
	
	vector<int> flipFlopper; // vector of values 1 and -1
	vector<int> zeroIndicesLastElem {in.size()};
	cat(zeroIndices,zeroIndicesLastElem);
	
	int L_limit = 0;
	int R_limit;
	
	for(int i = 0; i < zeroIndices.size(); i++){
		
		R_limit = zeroIndices[i];
		int curr_size = R_limit - L_limit;
		
		vector<int> curr_flip;
		
		if(currPositive[i]){
			
			vector<int> curr_flip_cat(curr_size, 1);
			cat(curr_flip, curr_flip_cat);
			
		} else {
			
			vector<int> curr_flip_cat(curr_size, -1);
			cat(curr_flip, curr_flip_cat);
		}
		
		cat(flipFlopper, curr_flip);
		
		L_limit = R_limit - 1;
	}
	
	for(int i = 0; i < out.size(); i++){
		
		out[i] = (double)(in[i]*flipFlopper[i]);
	}
	
	return out;
}



// =====================================================================
// === LOW LEVEL TOOLS =================================================
// =====================================================================



amp_arb signalAbsSum(a_signal in){
	
	amp_arb out = 0;
	
	for(int i = 0; i < in.size(); i++) out += std::abs(in[i]);
	
	return out;	
}

idx fracIdx(a_signal in,
              double fraction){

	idx out;
	
	if(fraction < 0.5){
		
		amp_arb tot = signalAbsSum(in);
		amp_arb acc = 0;
		idx i = -1;
		
		do{
			++i;
			
			acc += std::abs(in[i]);
			
		} while ((acc / tot) < fraction);
		
		out = i;
		
	} else {
	
		out = rFracIdx(in, 1 - fraction);
	}
	
	return out;
}

idx rFracIdx(a_signal in,
               double reverseFraction){

	idx out;
	
	if(reverseFraction <= 0.5){
		
		amp_arb tot = signalAbsSum(in);
		amp_arb acc = 0;
		idx i = in.size();
		
		do{
		
			--i;
			acc += std::abs(in[i]);
		
		} while ((acc / tot) <= reverseFraction);
		
		
		out = i;
		
	} else {
	
		out = fracIdx(in, 1 - reverseFraction);
	}
	
	return out;
}

a_sentence getParamNames(){

	a_sentence out;
	
	out.push_back("ID");
	
	out.push_back("separate Bremsstrahlung");
	
	out.push_back("signal area");
	out.push_back("signal length");
	
	out.push_back("signal peak heights");
	out.push_back("signal peak height distances");
	out.push_back("signal max height");
	out.push_back("signal peak times");
	out.push_back("signal peak time distances");
	
	out.push_back("signal plateau heights");
	out.push_back("signal plateau times");
	
	out.push_back("area fraction times");
	out.push_back("prompt fraction at 50ns");
	out.push_back("fixed window areas");
	
	out.push_back("RMS width");
	out.push_back("signal mean");
	out.push_back("FWHM");
	out.push_back("RMS amplitude");

	return out;
}

vector<int> getRepetitions(a_Params In){
	
	vector<int> out;
	
	out.push_back(In.signalPeakHeights.size());
	out.push_back(In.signalPeakHeightDists.size());
	out.push_back(In.signalPeakTimes.size());
	out.push_back(In.signalPeakTimeDists.size());
	out.push_back(In.signalPlateauHeights.size());
	out.push_back(In.signalPlateauTimes.size());
	out.push_back(In.afts.size());
	out.push_back(In.fixedWindowAreas.size());
	
	return out;
}
