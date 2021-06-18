#include <iostream>

#include <vector>
#include <string>

#include <algorithm>
#include <iterator>
#include <fstream>
#include <stdlib.h>
#include <time.h>
#include <numeric> // for std::accumulate

#include "consoleMessages.h"      // MOST console messages routed here
#include "myTypedefs.h"           // all the typedefs of the program
#include "getFilesInDir.h"        // get filenames from resources/input
#include "readANTSoutput.h"       // read the waveforms
#include "waveformParametrizer.h" // parametrization of waveforms
#include "parameterFeaturizer.h"  // parameter featurization
#include "fileOutput.h"           // exporting features

using namespace std;

# ifndef __CINT__

// =====================================================================
// == FUNCTION FORESHADOWING ===========================================
// =====================================================================



// -- HIGH LEVEL -------------------------------------------------------

void PFwithCategories(stringvec v,
                            int verbosity);

void PFsansCategories(stringvec v,
                            int verbosity);

// -- MID LEVEL --------------------------------------------------------

sTree3D getCategorizationFileNames();

rTree3D openCategorizationFiles(sTree3D catFnames);

void writeLineToFile(ofstream& strm, 
                          bool hasCategorization,
                    a_Features In);
                    
void writeLineToFile(refLeaf strm,
                        bool hasCategorization,
                  a_Features In);

void writeFileHeaderSans(a_sentence fileHeaderBase,
                          ofstream& strm);

void writeAllHeadersTree(a_sentence fileHeaderBase,
                            rTree3D catFiles);

void closeCategorizationFiles(rTree3D catFiles);

// -- LOW LEVEL --------------------------------------------------------

void writeFileHeaderTree(int nPeaks,         // THIS FUNK WILL LIKELY HAVE
                         int nPlateaus,      // TO BE CHANGED IN FURTHER
                  a_sentence fileHeaderBase, // VERSIONS OF THIS 
                     refLeaf strm);          // PARATURIZER



// =====================================================================
// == THE MAIN FUNCTION ITSELF =========================================
// =====================================================================



int main(int argc, char *argv[]){

	int verbosity = 0; // ascending levels of verbosity

	srand(time(0));

	cout << endl;

	stringvec v;
	read_directory("./resources/input", v);
	sort(v.begin(), v.end());

	cout << "finished reading the input filenames." << endl;
	
	if (verbosity > 0){
		
		for(int i = 0; i < v.size(); i++){
			
			cout << "* " << v[i] << endl;
		}
		cout << endl;
	}

	//~ cout << endl << "Running paraturization with categories" << endl;
	//~ PFwithCategories(v, verbosity);
	
	cout << endl << "Running paraturization sans categories" << endl;
	PFsansCategories(v, verbosity);
	
	return 0;
	
}



// =====================================================================
// == HIGH LEVEL FUNCTIONS =============================================
// =====================================================================



void PFwithCategories(stringvec v,
                            int verbosity){

	sTree3D catFnames = getCategorizationFileNames();
	rTree3D catFiles  = openCategorizationFiles(catFnames);
	consoleMessage_main(1, verbosity,'C');
	
	featHed featureHeader = getFeatureHeader(true);
	a_sentence fileHeaderBase = getFileHeaderBase(featureHeader);
	writeAllHeadersTree(fileHeaderBase, catFiles);
	consoleMessage_main(2, verbosity,'C');

	for (int  i = 0; i < v.size(); i++){

		string in_fname = "./resources/input/" + v[i];
		if(verbosity > -1){
			
			cout << "current file: " << v[i];
			cout << "\t progress: ";
			cout << std::to_string((int)((float)i/(float)v.size()*100));
			cout << "%" << endl;
		}
		
		a_cell cell = readANTSfile(in_fname);
		consoleMessage_main(3, verbosity, 'A', v[i]);
	
		a_signal fullWaveform = sumChannels(cell);
		consoleMessage_main(3, verbosity, 'B', v[i]);
		
		if((rand() % 2) == 1){
			consoleMessage_main(3, verbosity, 'C');
			
			signalUpsideDown(fullWaveform);
			v[i] = "UD_" + v[i];
		}
		
		a_Params curr_Params     = getParams(fullWaveform, 
		                                     v[i], 5, verbosity);
		consoleMessage_main(3, verbosity, 'D', v[i]);
		
		a_Features curr_Features = getFeatures(featureHeader,
		                                       curr_Params,
		                                       verbosity);
		consoleMessage_main(3, verbosity, 'E', v[i]);
		
		idx brem_idx = (int)(curr_Features.hasSeparateBrem);
		idx nPks_idx = curr_Features.nPeaks;
		nPks_idx = nPks_idx < 3 ? nPks_idx : 9;
		idx nPlt_idx = curr_Features.nPlateaus;
		nPlt_idx = nPlt_idx < 2 ? nPlt_idx : 9;
		writeLineToFile(catFiles[brem_idx][nPks_idx][nPlt_idx],
		                true,
		                curr_Features);
		consoleMessage_main(3, verbosity, 'Y', v[i]);
	}

	closeCategorizationFiles(catFiles);
	consoleMessage_main(4, verbosity, 'C');
}

void PFsansCategories(stringvec v,
                            int verbosity){

	ofstream outf_sansCategories;
	outf_sansCategories.open("./resources/output/AFOEA_sansCategories.txt",
	                         std::ofstream::out | std::ofstream::trunc);
	consoleMessage_main(1, verbosity, 'N');
	
	featHed    featureHeader  = getFeatureHeader(false);
	a_sentence fileHeaderBase = getFileHeaderBase(featureHeader);
	writeFileHeaderSans(fileHeaderBase, outf_sansCategories);
	consoleMessage_main(2, verbosity, 'N');

	for (int  i = 0; i < v.size(); i++){
	
		string in_fname = "./resources/input/" + v[i];
		cout << "current file: " << v[i];
		cout << "\t progress: ";
		cout << std::to_string((int)((float)i/(float)v.size() * 100));
		cout << "%" << endl;
		
		a_cell cell           = readANTSfile(in_fname);
		consoleMessage_main(3, verbosity, 'A', v[i]);
	
		a_signal fullWaveform = sumChannels(cell);
		consoleMessage_main(3, verbosity, 'B', v[i]);
		
		if((rand() % 2) == 1){
		
			consoleMessage_main(3, verbosity, 'C');
			signalUpsideDown(fullWaveform);
			v[i] = "UD_" + v[i];
		}
		
		a_Params curr_Params     = getParams(fullWaveform, 
		                                     v[i], 5, verbosity);
		consoleMessage_main(3, verbosity, 'D', v[i]);
		
		a_Features curr_Features = getFeatures(featureHeader,
		                                       curr_Params,
		                                       verbosity);
		consoleMessage_main(3, verbosity, 'E', v[i]);
		
		writeLineToFile(outf_sansCategories, false, curr_Features);
		consoleMessage_main(3, verbosity, 'Z', v[i]);
	}

	outf_sansCategories.close(); 
	consoleMessage_main(4, verbosity, 'N');
}



// =====================================================================
// == MID LEVEL FUNCTIONS ==============================================
// =====================================================================



sTree3D getCategorizationFileNames(){
	
	sTree3D out;
	
	sTree2D N_brem, Y_brem;
	
	sTree1D placeholder, N1pks, N2pks, NMpks, Y1pks, Y2pks, YMpks;
	
	//  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  - 
	//  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  - 
	
	placeholder.push_back("N/A");
	
	//  no bremsstrahlung  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -   
	
	// 0 peaks
	// nothing
	
	// 1 peaks
	N1pks.push_back("./resources/output/AFOE_Nbrem_1pk_0plt.txt"); // 0 plt
	N1pks.push_back("./resources/output/AFOE_Nbrem_1pk_1plt.txt"); // 1 plt
	
	for(int i=2; i<9; i++) N1pks.push_back("N/A");            // 2-8 plt
	
	N1pks.push_back("./resources/output/AFOE_Nbrem_1pk_Mplt.txt"); // 9 plt
	
	// 2 peaks
	N2pks.push_back("./resources/output/AFOE_Nbrem_2pk_0plt.txt"); // 0 plt
	N2pks.push_back("./resources/output/AFOE_Nbrem_2pk_1plt.txt"); // 1 plt
	
	for(int i=2; i<9; i++) N2pks.push_back("N/A");            // 2-8 plt
	
	N2pks.push_back("./resources/output/AFOE_Nbrem_2pk_Mplt.txt"); // 9 plt
	
	// 3-8 peaks
	//nothing
	
	// 9 (the nine is used to represent "many") peaks
	NMpks.push_back("./resources/output/AFOE_Nbrem_Mpk_0plt.txt"); // 0 plt
	NMpks.push_back("./resources/output/AFOE_Nbrem_Mpk_1plt.txt"); // 1 plt
	
	for(int i=2; i<9; i++) NMpks.push_back("N/A");            // 2-8 plt
	
	NMpks.push_back("./resources/output/AFOE_Nbrem_Mpk_Mplt.txt"); // 9 plt

	//  yes bremsstrahlung -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  

	// 0 peaks
	// nothing
	
	// 1 peaks
	Y1pks.push_back("./resources/output/AFOE_Ybrem_1pk_0plt.txt"); // 0 plt
	Y1pks.push_back("./resources/output/AFOE_Ybrem_1pk_1plt.txt"); // 1 plt
	
	for(int i=2; i<9; i++) Y1pks.push_back("N/A");            // 2-8 plt
	
	Y1pks.push_back("./resources/output/AFOE_Ybrem_1pk_Mplt.txt"); // 9 plt
	
	// 2 peaks
	Y2pks.push_back("./resources/output/AFOE_Ybrem_2pk_0plt.txt"); // 0 plt
	Y2pks.push_back("./resources/output/AFOE_Ybrem_2pk_1plt.txt"); // 1 plt
	
	for(int i=2; i<9; i++) Y2pks.push_back("N/A");            // 2-8 plt
	
	Y2pks.push_back("./resources/output/AFOE_Ybrem_2pk_Mplt.txt"); // 9 plt
	
	// 3-8 peaks
	// nothing
	
	// 9 peaks
	YMpks.push_back("./resources/output/AFOE_Ybrem_Mpk_0plt.txt"); // 0 plt
	YMpks.push_back("./resources/output/AFOE_Ybrem_Mpk_1plt.txt"); // 1 plt
	
	for(int i=2; i<9; i++) YMpks.push_back("N/A");            // 2-8 plt
	
	YMpks.push_back("./resources/output/AFOE_Ybrem_Mpk_Mplt.txt"); // 9 plt

	//  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  - 
	//  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  - 
	
	N_brem.push_back(placeholder);                            // 0 pks
	
	N_brem.push_back(N1pks);                                  // 1 pks
	N_brem.push_back(N2pks);                                  // 2 pks
	
	for(int i = 3; i < 9; i++) N_brem.push_back(placeholder); // 3-8 pks
	
	N_brem.push_back(NMpks);                                  // 9 pks
	
	//  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  - 
	
	Y_brem.push_back(placeholder);                            // 0 pks
	
	Y_brem.push_back(Y1pks);                                  // 1 pks
	Y_brem.push_back(Y2pks);                                  // 2 pks
	
	for(int i = 3; i < 9; i++) Y_brem.push_back(placeholder); // 3-8 pks
	
	Y_brem.push_back(YMpks);                                  // 9 pks
	
	//  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  - 
	//  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  - 
	
	out.push_back(N_brem); // 0 == (int)hasSeparateBrem if it's false
	out.push_back(Y_brem); // 1 == (int)hasSeparateBrem if it's true
	
	return out;
	
}

rTree3D openCategorizationFiles(sTree3D catFnames){
	
	rTree3D out;
	
	for(int brem = 0; brem <= 1; brem++){
		
		rTree2D bremStreams;
		
		for(int nPks = 0; nPks < catFnames[brem].size(); nPks++){
			
			rTree1D pkStreams;
			
			for(int nPlt=0; nPlt<catFnames[brem][nPks].size(); nPlt++){
			
				if (catFnames[brem][nPks][nPlt].compare("N/A") != 0){
					
					refLeaf strm(new ofstream);
					
					strm->open(catFnames[brem][nPks][nPlt], 
					           std::ofstream::out \
					           | std::ofstream::trunc);
				
					pkStreams.push_back(strm);
					
				} else {
					
					pkStreams.push_back(nullptr); // THIS MIGHT WORK ???
				}
			}
			
			bremStreams.push_back(pkStreams);
		}
		
		out.push_back(bremStreams);
	}
	
	return out;
}

void writeLineToFile(ofstream& strm,
                          bool hasCategorization,
                    a_Features In){
	
	a_sentence curr_line;
	
	if (hasCategorization){
		
		curr_line = a_Features2a_sentence_hasCat(In);
		
	} else {
		
		curr_line = a_Features2a_sentence_nonCat(In);
	}
	
		
	for(int i = 0; i < curr_line.size(); i++){
			
		strm << curr_line[i] << "\t";
	}
	strm << "\n";
}

void writeLineToFile(refLeaf strm,
                        bool hasCategorization,
                  a_Features In){

	a_sentence curr_line;
	
	if (hasCategorization){
		
		curr_line = a_Features2a_sentence_hasCat(In);
		
	} else {
		
		curr_line = a_Features2a_sentence_nonCat(In);
	}
	
		
	for(int i = 0; i < curr_line.size(); i++){
			
		*strm << curr_line[i] << "\t";
	}
	*strm << "\n";
}

void writeAllHeadersTree(a_sentence fileHeaderBase,
                            rTree3D catFiles){
	
	for(int brem = 0; brem <= 1; brem++){
		
		for(int nPks = 0; nPks < catFiles[brem].size(); nPks++){
			
			for(int nPlt=0; nPlt < catFiles[brem][nPks].size(); nPlt++){
				
				if(catFiles[brem][nPks][nPlt] != nullptr){
					
					writeFileHeaderTree(nPks, nPlt, fileHeaderBase,
					                    catFiles[brem][nPks][nPlt]);
				}
			}
		}
	}
}

void closeCategorizationFiles(rTree3D catFiles){
	
	for(int brem = 0; brem <= 1; brem++){
		
		for(int nPks = 0; nPks < catFiles[brem].size(); nPks++){
			
			for(int nPlt=0; nPlt < catFiles[brem][nPks].size(); nPlt++){
				
				if(catFiles[brem][nPks][nPlt] != nullptr){
					
					catFiles[brem][nPks][nPlt]->close();
				}
			}
		}
	}
}



// =====================================================================
// == LOW LEVEL FUNCTIONS ==============================================
// =====================================================================



void writeFileHeaderTree(int nPeaks, 
                         int nPlateaus,
                  a_sentence fileHeaderBase, 
                     refLeaf strm){

	a_sentence fHTree = fileHeaderBase;

	a_sentence::iterator it;

	/*
	if(nPeaks <= 2){
		
		it = find(fHTree.begin(), fHTree.end(), "pk_Hi2");
		fHTree.erase(fHTree.begin() + distance( fHTree.begin() , it ));
		
		it = find(fHTree.begin(), fHTree.end(), "pk_ns2");
		fHTree.erase(fHTree.begin() + distance( fHTree.begin() , it ));
		
		if (nPeaks == 1){
			
			it = find(fHTree.begin(), fHTree.end(), "pk_Hi1");
			fHTree.erase(fHTree.begin() + distance(fHTree.begin(),it));
			
			it = find(fHTree.begin(), fHTree.end(), "pk_ns1");
			fHTree.erase(fHTree.begin() + distance(fHTree.begin(),it));
		}
	}
	
	if(nPlateaus <= 1){
		
		it = find(fHTree.begin(), fHTree.end(), "pltHi1");
		fHTree.erase(fHTree.begin() + distance( fHTree.begin() , it ));
		
		it = find(fHTree.begin(), fHTree.end(), "pltNs1");
		fHTree.erase(fHTree.begin() + distance( fHTree.begin() , it ));
		
		if(nPlateaus == 0){
			
			it = find(fHTree.begin(), fHTree.end(), "pltHi0");
			fHTree.erase(fHTree.begin() + distance(fHTree.begin(),it));
			
			it = find(fHTree.begin(), fHTree.end(), "pltNs0");
			fHTree.erase(fHTree.begin() + distance(fHTree.begin(),it));
		}
	}
	*/ // ALL FEATURES ON EVERYTHING VERSION YEEEEAH
	
	for(int i = 0; i < fHTree.size(); i++){
		*strm << fHTree[i] << "\t";
	}
	*strm << "\n"; // I THINK THIS SHOULD WORK
}

void writeFileHeaderSans(a_sentence fileHeaderBase,
                          ofstream& strm){

	for(int i = 0; i < fileHeaderBase.size(); i++){
		strm << fileHeaderBase[i] << "\t";
	}
	strm << "\n";
}

# endif
