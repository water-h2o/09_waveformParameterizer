#include <iostream>
#include "parameterFeaturizer.h"

#include <string>
#include <vector>

#include "consoleMessages.h"
#include "myTypedefs.h"

using namespace std;



// =====================================================================
// === MAIN FUNCTIONS ==================================================
// =====================================================================



featHed getFeatureHeader(bool hasCategorization){
	
	featHed out;
	
	a_sentence  featureNamesAbbrev;
	vector<int> featureRepetitions;
	
	if(hasCategorization){
	
		featureNamesAbbrev = getFeatureNamesAbbrev_hasCat();
		featureRepetitions = getFeatureRepetitions_hasCat();
	
	} else {
		
		featureNamesAbbrev = getFeatureNamesAbbrev_nonCat();
		featureRepetitions = getFeatureRepetitions_nonCat();
	}
	
	for(int i = 0; i < featureNamesAbbrev.size(); i++){
		
		out.push_back(std::make_pair(featureNamesAbbrev[i],
		                             featureRepetitions[i]));
	}
	
	return out;
	
}

a_Features getFeatures(featHed featureHeader,
                      a_Params In,
                           int verbosity){

	a_Features Out;
	
	//  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
	// A. Initialization
	//  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
	
	consoleMessage_PF(1, verbosity);
	
	nanosec afts5  = In.afts[1];
	nanosec afts10 = In.afts[2];
	nanosec afts25 = In.afts[3];
	nanosec afts50 = In.afts[4];
	nanosec afts75 = In.afts[5];
	nanosec afts90 = In.afts[6];
	nanosec afts95 = In.afts[7];
	
	idx idx5  = In.afts_idx[1];
	idx idx10 = In.afts_idx[2];
	idx idx25 = In.afts_idx[3];
	idx idx50 = In.afts_idx[4];
	idx idx75 = In.afts_idx[5];
	idx idx90 = In.afts_idx[6];
	idx idx95 = In.afts_idx[7];
	
	//  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
	// B. Assigning trivial features
	//  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
	
	consoleMessage_PF(2, verbosity);
	
	Out.ftrNamesReps = featureHeader;
	
	Out.ID = In.ID;
	
	Out.hasSeparateBrem = In.hasSeparateBrem; // treemaker 1
	
	/*
	
	Out.signalPeakHeights    = In.signalPeakHeights;
	Out.signalMaxPeakHeight  = In.signalMaxHeight;
	Out.signalPlateauHeights = In.signalPlateauHeights;
	
	*/ // MOVED DOWN TO PART C. BC NOW I NEED TO APPEND STUFF TO IT
	
	Out.signalMaxPlateauHeight = In.signalMaxPlateauHeight; // non-cat
	
	Out.RMSwidth     = In.RMSwidth;
	Out.RMSamplitude = In.RMSamplitude;
	
	//  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
	// C. Inferring nontrivial features
	//  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
	
	consoleMessage_PF(3, verbosity);
	
	Out.nPeaks    = In.signalPeakHeights.size();    // treemaker 2
	Out.nPlateaus = In.signalPlateauHeights.size(); // treemaker 3
	
	ampNsec A5_95 = In.signalCumAreas[idx95] - In.signalCumAreas[idx5];
	ampNsec A10_90= In.signalCumAreas[idx90] - In.signalCumAreas[idx10];
	ampNsec A25_75= In.signalCumAreas[idx75] - In.signalCumAreas[idx25];
	
	ampNsec A5_50 = In.signalCumAreas[idx50] - In.signalCumAreas[idx5];
	ampNsec A5_75 = In.signalCumAreas[idx75] - In.signalCumAreas[idx5];
	ampNsec A5_90 = In.signalCumAreas[idx90] - In.signalCumAreas[idx5];
	
	ampNsec A10_95= In.signalCumAreas[idx95] - In.signalCumAreas[idx10];
	ampNsec A25_95= In.signalCumAreas[idx95] - In.signalCumAreas[idx25];
	ampNsec A50_95= In.signalCumAreas[idx95] - In.signalCumAreas[idx50];
	
	Out.Area5_95 = A5_95;
	
	Out.Lgth5_95  = afts95 - afts5;
	Out.Lgth10_90 = afts90 - afts10;
	Out.Lgth25_75 = afts75 - afts25;
	
	Out.Lgth5_50  = afts50 - afts5;
	Out.Lgth5_75  = afts75 - afts5;
	Out.Lgth5_90  = afts90 - afts5;
	
	Out.Lgth10_95 = afts95 - afts10;
	Out.Lgth25_95 = afts95 - afts25;
	Out.Lgth50_95 = afts95 - afts50;
	
	Out.Mean5_95  = A5_95  / Out.Lgth5_95;
	Out.Mean10_90 = A10_90 / Out.Lgth10_90;
	Out.Mean25_75 = A25_75 / Out.Lgth25_75;
	
	Out.Mean5_50  = A5_50  / Out.Lgth5_50;
	Out.Mean5_75  = A5_75  / Out.Lgth5_75;
	Out.Mean5_90  = A5_90  / Out.Lgth5_90;
	
	Out.Mean10_95 = A10_95 / Out.Lgth10_95;
	Out.Mean25_95 = A25_95 / Out.Lgth25_95;
	Out.Mean50_95 = A50_95 / Out.Lgth50_95;
	
	// all features everything version
	
	Out.signalPeakHeights    = In.signalPeakHeights;
	if (Out.nPeaks < 3){
		for(int i = 0 ; i < (3 - Out.nPeaks) ; i ++){
			Out.signalPeakHeights.push_back(-1.0);
		}
	}
	
	Out.signalMaxPeakHeight  = In.signalMaxHeight;
	Out.signalPlateauHeights = In.signalPlateauHeights;
	if (Out.nPlateaus < 2){
		for(int i = 0 ; i < (2 - Out.nPlateaus) ; i ++){
			Out.signalPlateauHeights.push_back(-1.0);
		}
	}
	
	/*
	for(int i = 0; i < Out.nPeaks; i++)
		Out.signalPeakTimes_after5.push_back(In.signalPeakTimes[i]-afts5);
	
	Out.signalMaxPeakTime_after5 = In.signalMaxPeakTime - afts5;
	
	for(int i = 0; i < Out.nPlateaus; i++)
		Out.signalPlateauTimes_after5.push_back(In.signalPlateauTimes[i]-afts5);
	*/
	
	for(int i = 0; i < 3 ; i++){ // all features everything version
		
		if (i < Out.nPeaks){
			Out.signalPeakTimes_after5.push_back( \
			                        In.signalPeakTimes[i]-afts5);
		} else {
			Out.signalPeakTimes_after5.push_back(-1.0);
		}
	}
	
	for(int i = 0 ; i < 2 ; i++){
		if (i < Out.nPlateaus){
			Out.signalPlateauTimes_after5.push_back(
			                           In.signalPlateauTimes[i]-afts5);
		} else {
			Out.signalPlateauTimes_after5.push_back(-1.0);
		}
	}
	
	Out.signalMaxPlateauTime_after5 = In.signalMaxPlateauTime;
	if(Out.signalMaxPlateauTime_after5 != -1)
		Out.signalMaxPlateauTime_after5 -= afts5;
			// non-categorization feature	
	
	Out.t_skewness=(afts50-Out.signalMaxPeakTime_after5) / Out.RMSwidth;
	Out.a_skewness= (Out.Mean50_95/Out.Mean5_50) - 1;
	
	return Out;

}



// =====================================================================
// === HIGH LEVEL TOOLS ================================================
// =====================================================================


a_sentence getFeatureNamesAbbrev_hasCat(){
	
	a_sentence out;
	
	out.push_back("ID"    );
	
	out.push_back("A5_95 ");
	
	out.push_back("M5_95" );  // mean [5%, 95%]
	out.push_back("M10_90");
	out.push_back("M25_75");
	
	out.push_back("M5_50" );
	out.push_back("M5_75" );
	out.push_back("M5_90" );
	
	out.push_back("M10_95");
	out.push_back("M25_95");
	out.push_back("M50_95");
	
	out.push_back("L5_95" );
	out.push_back("L10_90");
	out.push_back("L25_75");
	
	out.push_back("L5_50" );
	out.push_back("L5_75" );
	out.push_back("L5_90" );
	
	out.push_back("L10_95");
	out.push_back("L25_95");
	out.push_back("L50_95");
	
	out.push_back("pk_Hi" );
	out.push_back("pk_Xh" );  // max peak height
	out.push_back("pk_ns" );
	out.push_back("pk_Xns");  // max peak time
	
	out.push_back("pltHi" );
	out.push_back("pltXh" );  // max peak height
	out.push_back("pltNs" );
	out.push_back("pltXns");  // max peak time
	
	out.push_back("RMSw"  );
	out.push_back("RMSa"  );
	
	out.push_back("tSkew" );
	out.push_back("aSkew" );
	
	return out;
	
}

vector<int> getFeatureRepetitions_hasCat(){
	
	vector<int> out;
	
	out.push_back(1); // ID
	
	out.push_back(1); // Area5_95
	
	out.push_back(1); // Mean5_95
	out.push_back(1); // Mean10_90
	out.push_back(1); // Mean25_75
	
	out.push_back(1); // Mean5_50
	out.push_back(1); // Mean5_75
	out.push_back(1); // Mean5_90
	
	out.push_back(1); // Mean10_95
	out.push_back(1); // Mean25_95
	out.push_back(1); // Mean50_95
	
	out.push_back(1); // Lgth5_95
	out.push_back(1); // Lgth10_90
	out.push_back(1); // Lgth25_75
	
	out.push_back(1); // Lgth5_50
	out.push_back(1); // Lgth5_75
	out.push_back(1); // Lgth5_90
	
	out.push_back(1); // Lgth10_95
	out.push_back(1); // Lgth25_95
	out.push_back(1); // Lgth50_95
	
	out.push_back(3); // signalPeakHeights 
	out.push_back(1); // signalMaxPeakHeight
	out.push_back(3); // signalPeakTimes_after5 
	out.push_back(1); // signalMaxPeakTime_after5
	
	out.push_back(2); // signalPlateauHeights 
	out.push_back(1); // signalMaxPlateauHeight
	out.push_back(2); // signalPlateauTimes_after5 
	out.push_back(1); // signalMaxPlateauTime_after5
	
	out.push_back(1); // RMSwidth
	out.push_back(1); // RMSamplitude
	
	out.push_back(1); // t_skewness
	out.push_back(1); // a_skewness
	
	return out;
	
}

a_sentence getFeatureNamesAbbrev_nonCat(){
	
	a_sentence out;
	
	out.push_back("ID"    );
	
	
	out.push_back("bremTF");  // bremsstrahlung --> true, false?
	out.push_back("nPks"  );
	out.push_back("nPlat" );
	
	out.push_back("A5_95" );
	
	out.push_back("M5_95" );  // mean [5%, 95%]
	out.push_back("M10_90");
	out.push_back("M25_75");
	
	out.push_back("M5_50" );
	out.push_back("M5_75" );
	out.push_back("M5_90" );
	
	out.push_back("M10_95");
	out.push_back("M25_95");
	out.push_back("M50_95");
	
	out.push_back("L5_95" );
	out.push_back("L10_90");
	out.push_back("L25_75");
	
	out.push_back("L5_50" );
	out.push_back("L5_75" );
	out.push_back("L5_90" );
	
	out.push_back("L10_95");
	out.push_back("L25_95");
	out.push_back("L50_95");
	
	out.push_back("pk_Hi" );
	out.push_back("pk_Xh" );  // max peak height
	out.push_back("pk_ns" );
	out.push_back("pk_Xns");  // max peak time
	
	out.push_back("pltHi" );
	out.push_back("pltXh" );  // max peak height
	out.push_back("pltNs" );
	out.push_back("pltXns");  // max peak time
	
	out.push_back("RMSw"  );
	out.push_back("RMSa"  );
	
	out.push_back("tSkew" );
	out.push_back("aSkew" );
	
	return out;
	
}

vector<int> getFeatureRepetitions_nonCat(){
	
	vector<int> out;
	
	out.push_back(1); // ID
	
	
	out.push_back(1); // hasSeparateBrem
	out.push_back(1); // nPeaks
	out.push_back(1); // nPlateaus
	
	out.push_back(1); // Area5_95
	
	out.push_back(1); // Mean5_95
	out.push_back(1); // Mean10_90
	out.push_back(1); // Mean25_75
	
	out.push_back(1); // Mean5_50
	out.push_back(1); // Mean5_75
	out.push_back(1); // Mean5_90
	
	out.push_back(1); // Mean10_95
	out.push_back(1); // Mean25_95
	out.push_back(1); // Mean50_95
	
	out.push_back(1); // Lgth5_95
	out.push_back(1); // Lgth10_90
	out.push_back(1); // Lgth25_75
	
	out.push_back(1); // Lgth5_50
	out.push_back(1); // Lgth5_75
	out.push_back(1); // Lgth5_90
	
	out.push_back(1); // Lgth10_95
	out.push_back(1); // Lgth25_95
	out.push_back(1); // Lgth50_95
	
	out.push_back(3); // signalPeakHeights 
	out.push_back(1); // signalMaxPeakHeight
	out.push_back(3); // signalPeakTimes_after5 
	out.push_back(1); // signalMaxPeakTime_after5
	
	out.push_back(2); // signalPlateauHeights 
	out.push_back(1); // signalMaxPlateauHeight
	out.push_back(2); // signalPlateauTimes_after5 
	out.push_back(1); // signalMaxPlateauTime_after5
	
	out.push_back(1); // RMSwidth
	out.push_back(1); // RMSamplitude
	
	out.push_back(1); // t_skewness
	out.push_back(1); // a_skewness
	
	return out;
	
}



// =====================================================================
// === MID LEVEL TOOLS =================================================
// =====================================================================







// =====================================================================
// === LOW LEVEL TOOLS =================================================
// =====================================================================



