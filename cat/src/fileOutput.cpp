#include <iostream>
#include "fileOutput.h"

#include <vector>
#include <string>

#include "consoleMessages.h"
#include "myTypedefs.h"

using namespace std;

// =====================================================================
// === MAIN FUNCTION ===================================================
// =====================================================================



a_sentence getFileHeaderBase(featHed in){

	a_sentence out;

	for(int i = 0; i < in.size(); i++){
		
		for(int j = 0; j < in[i].second ; j++){
			
			if(in[i].second > 1){
			
				out.push_back(in[i].first + std::to_string(j));
			
			} else {
				
				out.push_back(in[i].first);
			}
		}
	}

	return out;
}

vector<int> vectorGatherMaxElements(vector<int> inA,
                                    vector<int> inB){

	vector<int> out;
	
	for(int i = 0 ; i < inA.size(); i++){
		
		out.push_back(std::max(inA[i], inB[i]) );
	}
	
	return out;
}

a_sentence a_Features2a_sentence_hasCat(a_Features In){
	
	a_sentence out;
	
	featHed header = In.ftrNamesReps;
	
	idx head_idx = 0;
	
	out.push_back(In.ID);
	head_idx += 1;
	
	out.push_back(std::to_string(In.Area5_95));
	head_idx += 1;
	
	out.push_back(std::to_string(In.Mean5_95));
	out.push_back(std::to_string(In.Mean10_90));
	out.push_back(std::to_string(In.Mean25_75));
	head_idx += 3;
	
	out.push_back(std::to_string(In.Mean5_50));
	out.push_back(std::to_string(In.Mean5_75));
	out.push_back(std::to_string(In.Mean5_90));
	head_idx += 3;
	
	out.push_back(std::to_string(In.Mean10_95));
	out.push_back(std::to_string(In.Mean25_95));
	out.push_back(std::to_string(In.Mean50_95));
	head_idx += 3;
	
	out.push_back(std::to_string(In.Lgth5_95));
	out.push_back(std::to_string(In.Lgth10_90));
	out.push_back(std::to_string(In.Lgth25_75));
	head_idx += 3;
	
	out.push_back(std::to_string(In.Lgth5_50));
	out.push_back(std::to_string(In.Lgth5_75));
	out.push_back(std::to_string(In.Lgth5_90));
	head_idx += 3;
	
	out.push_back(std::to_string(In.Lgth10_95));
	out.push_back(std::to_string(In.Lgth25_95));
	out.push_back(std::to_string(In.Lgth50_95));
	head_idx += 3;
	
	for(int i = 0; i < min(header[head_idx].second, In.nPeaks); i++){
	
		if(In.signalPeakHeights.size() >= i+1){
		
			out.push_back(std::to_string(In.signalPeakHeights[i]));
			
		} else {
			
			out.push_back("NaN");
		}
	}
	out.push_back(std::to_string(In.signalMaxPeakHeight));
	head_idx += 2;
	
	for(int i = 0; i < min(header[head_idx].second, In.nPeaks); i++){
	
		if(In.signalPeakTimes_after5.size() >= i+1){
		
			out.push_back(std::to_string(In.signalPeakTimes_after5[i]));
			
		} else {
			
			out.push_back("NaN");
		}
	}
	out.push_back(std::to_string(In.signalMaxPeakTime_after5));
	head_idx += 2;
	
	for(int i = 0; i < min(header[head_idx].second, In.nPlateaus); i++){
	
		if(In.signalPlateauHeights.size() >= i+1){
		
			out.push_back(std::to_string(In.signalPlateauHeights[i]));
			
		} else {
			
			out.push_back("NaN");
		}
	}
	for(int i = 0; i < min(header[head_idx].second, In.nPlateaus); i++){
	
		if(In.signalPlateauTimes_after5.size() >= i+1){
		
			out.push_back(std::to_string(In.signalPlateauTimes_after5[i]));
			
		} else {
			
			out.push_back("NaN");
		}
	}
	head_idx += 2;
	
	out.push_back(std::to_string(In.RMSwidth));
	out.push_back(std::to_string(In.RMSamplitude));
	head_idx += 2;
	
	out.push_back(std::to_string(In.t_skewness));
	out.push_back(std::to_string(In.a_skewness));
	head_idx += 2;
	
	return out;	
}

a_sentence a_Features2a_sentence_nonCat(a_Features In){
	
	a_sentence out;
	
	featHed header = In.ftrNamesReps;
	
	idx head_idx = 0;
	
	out.push_back(In.ID);
	head_idx += 1;
	
	out.push_back(std::to_string((int)In.hasSeparateBrem));
	out.push_back(std::to_string(In.nPeaks));
	out.push_back(std::to_string(In.nPlateaus));
	head_idx += 3;
	
	out.push_back(std::to_string(In.Area5_95));
	head_idx += 1;
	
	out.push_back(std::to_string(In.Mean5_95));
	out.push_back(std::to_string(In.Mean10_90));
	out.push_back(std::to_string(In.Mean25_75));
	head_idx += 3;
	
	out.push_back(std::to_string(In.Mean5_50));
	out.push_back(std::to_string(In.Mean5_75));
	out.push_back(std::to_string(In.Mean5_90));
	head_idx += 3;
	
	out.push_back(std::to_string(In.Mean10_95));
	out.push_back(std::to_string(In.Mean25_95));
	out.push_back(std::to_string(In.Mean50_95));
	head_idx += 3;
	
	out.push_back(std::to_string(In.Lgth5_95));
	out.push_back(std::to_string(In.Lgth10_90));
	out.push_back(std::to_string(In.Lgth25_75));
	head_idx += 3;
	
	out.push_back(std::to_string(In.Lgth5_50));
	out.push_back(std::to_string(In.Lgth5_75));
	out.push_back(std::to_string(In.Lgth5_90));
	head_idx += 3;
	
	out.push_back(std::to_string(In.Lgth10_95));
	out.push_back(std::to_string(In.Lgth25_95));
	out.push_back(std::to_string(In.Lgth50_95));
	head_idx += 3;
	
	out.push_back(std::to_string(In.signalMaxPeakHeight));
	out.push_back(std::to_string(In.signalMaxPeakTime_after5));
	head_idx += 2;
	
	out.push_back(std::to_string(In.signalMaxPlateauHeight));
	out.push_back(std::to_string(In.signalMaxPlateauTime_after5));
	head_idx += 2;
	
	out.push_back(std::to_string(In.RMSwidth));
	out.push_back(std::to_string(In.RMSamplitude));
	head_idx += 2;
	
	out.push_back(std::to_string(In.t_skewness));
	out.push_back(std::to_string(In.a_skewness));
	head_idx += 2;
	
	return out;	
}



// =====================================================================
// === HIGH LEVEL TOOLS ================================================
// =====================================================================



// STUFF GOES HERE




// =====================================================================
// === MID LEVEL TOOLS =================================================
// =====================================================================



// STUFF GOES HERE



// =====================================================================
// === LOW LEVEL TOOLS =================================================
// =====================================================================



