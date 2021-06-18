#ifndef MYTYPEDEFS_H
#define MYTYPEDEFS_H

#include <utility> // std::pair, std::make_pair
#include <string>
#include <map>
#include <vector>
#include <fstream>
#include <functional>
#include <memory>



// == TYPEDEFS =========================================================



typedef std::vector<std::string> stringvec; // used only in getFilesInDir.h

typedef std::vector<std::string>        a_column;
typedef std::map<std::string, a_column> a_cell;
typedef std::vector<std::string>        a_sentence; // vector of short strings
typedef std::vector<double>             a_signal;


typedef double nanosec; // units of time (nanoseconds)
typedef double amp_arb; // units of amplitude (arbitrary amplitude unit)
typedef double ampNsec; // units of area (amp_arb x nanosec)
typedef int    idx;     // for indices


typedef std::pair<std::string, int> nameNum; // element of feature list
                                             // .first  == name
                                             // .second == repetitions
typedef std::vector<nameNum>        featHed; // feature struct "header"


typedef std::string          strLeaf; // for categorization
typedef std::vector<strLeaf> sTree1D;
typedef std::vector<sTree1D> sTree2D;
typedef std::vector<sTree2D> sTree3D;


//typedef std::reference_wrapper<std::ofstream> refLeaf; // ref  to stream
typedef std::shared_ptr<std::ofstream> refLeaf; // ref  to ofstream
typedef std::vector<refLeaf>           rTree1D; // 1 deg ofstream tree
typedef std::vector<rTree1D>           rTree2D; // 2 deg ofstream tree
typedef std::vector<rTree2D>           rTree3D; // 3 deg ofstream tree



// == STRUCTS ========================================================== 



struct Params{
					
	         std::string ID;
	         
		            bool hasSeparateBrem;
	         
	std::vector<ampNsec> signalCumAreas; // cumulative area
	             nanosec signalLength;
	             
	std::vector<amp_arb> signalPeakHeights;
	std::vector<amp_arb> signalPeakHeightDists;
	             amp_arb signalMaxHeight;
	std::vector<nanosec> signalPeakTimes;
	std::vector<nanosec> signalPeakTimeDists;
	             nanosec signalMaxPeakTime;
	             
	std::vector<amp_arb> signalPlateauHeights;
	             amp_arb signalMaxPlateauHeight;
	std::vector<nanosec> signalPlateauTimes;    
	             nanosec signalMaxPlateauTime; 
	             
	std::vector<nanosec> afts; // 0.01 0.05 0.1 0.25 0.5 0.75 0.9 0.95 0.99
	                           // 0    1    2   3    4   5    6   7    8    <-- AS OF 29 09 19
	    std::vector<idx> afts_idx;
	
	              double promptFraction50ns;
	std::vector<ampNsec> fixedWindowAreas;
	
	             nanosec RMSwidth;
	             amp_arb signalMean;
	             nanosec FWHM;
	             amp_arb RMSamplitude;
	
	          a_sentence paramNames;
};
typedef struct Params a_Params; // for storing params of waveform


struct Features{
	
	             featHed ftrNamesReps; // (name, repetitions)
	
	         std::string ID;
	         
		            bool hasSeparateBrem; // treemaker 1 (or non-cat)
	                 int nPeaks;          // treemaker 2 (or non-cat)
	                 int nPlateaus;       // treemaker 3 (or non-cat)
	                 
	             ampNsec Area5_95;
	         
	             amp_arb Mean5_95; // centered means
	             amp_arb Mean10_90;
	             amp_arb Mean25_75;
	             
	             amp_arb Mean5_50; // left-leaning means
	             amp_arb Mean5_75;
	             amp_arb Mean5_90;
	             
	             amp_arb Mean10_95; // right-leaning means
	             amp_arb Mean25_95;
	             amp_arb Mean50_95;
	             
	             nanosec Lgth5_95; // centered lengths
	             nanosec Lgth10_90;
	             nanosec Lgth25_75;
	             
	             nanosec Lgth5_50; // left-leaning lengths
	             nanosec Lgth5_75;
	             nanosec Lgth5_90;
	             
	             nanosec Lgth10_95; // right-leaning lengths
	             nanosec Lgth25_95;
	             nanosec Lgth50_95;
	             
	std::vector<amp_arb> signalPeakHeights;           // onlyCat feature
	             amp_arb signalMaxPeakHeight;
	std::vector<nanosec> signalPeakTimes_after5;      // onlyCat feature       
	             nanosec signalMaxPeakTime_after5;    
	
	std::vector<amp_arb> signalPlateauHeights;        // onlyCat feature
	             amp_arb signalMaxPlateauHeight;      // non-cat feature
	std::vector<nanosec> signalPlateauTimes_after5;   // onlyCat feature
	             nanosec signalMaxPlateauTime_after5; // non-cat feature
	
	             nanosec RMSwidth;
	             //amp_arb signalMean;
	             //nanosec FWHM;
	             amp_arb RMSamplitude;
	             
	              double t_skewness; // almost Pearson 1st skewness
	              double a_skewness; // R amplitude vs. L amplitude
	
};
typedef struct Features a_Features; // from the extracted parameters, 
                                    // the features of interest are
                                    // extracted, and stored in here

#endif // MYTYPEDEFS_H
