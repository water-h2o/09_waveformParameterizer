#include <iostream>
#include "consoleMessages.h"

#include <string>

using namespace std;

void consoleMessage_WP(int section,
                       int verbosity,
                      char subsection){
	
	if (verbosity > 2){
		switch(section){
		
			case -1 :
				
				switch(subsection){
					case 'A' :
						
						cout <<"wP.gP() -> A. Before parametrization, ";
						cout <<"cut out bremsstrahlung" << endl;
						break;
					case 'E' :
					
						cout << "wP.gP() -> END. Get parameter names";
						cout << endl;
						break;
					default:
						
						cout << "wP.gP() -> started" << endl;
				}
				break;
			case 0 :
				
				cout << "wP.gP() -> 0. Waveform ID" << endl;
				break;
			case 1 :
				
				cout << "wP.gP() -> 1. Find total/positive/negative ";
				cout << "pulse areas, as well as total lgth" << endl;
				break;
			case 2 :
				
				cout << "wP.gP() -> 2. Find peak and plateau ";
				cout << "heights and times" << endl;
				break;
			case 3 :
				
				cout << "NOTHING HERE YET" << endl;
				break;
			case 4 :
				
				cout << "NOTHING HERE YET" << endl;
				break;
			case 5 :
				
				cout << "NOTHING HERE YET" << endl;
				break;
			case 6 :
				
				cout << "wP.gP() -> 6. Find area fraction times";
				cout << endl;
				break;
			case 7 :
				
				cout << "wP.gP() -> 7. Find RMSWidth ; Find pulseMean ";
				cout << endl;
				break;
			case 8 :
				
				cout << "wP.gP() -> 8. find FHWM - calculated between ";
				cout << "RoI bondaries" << endl;
				break;
			case 9 :
				
				cout << "wP.gP() -> 9. Find RMSAmplitude within the "; 
				cout << "pulse boundary" << endl;
				break;
			case 10 :
				
				cout << "NOTHING HERE YET" << endl;
				break;
			case 11 :
			
				cout << "NOTHING HERE YET" << endl;
				break;
			case 12 :
				
				cout << "wP.gP() -> 12. Find the prompt fraction";
				cout << endl;
				break;
			case 13 :
				
				cout << "wP.gP() -> 13. Find the fixed integration ";
				cout << "window areas" << endl;
				break;
		}
	}
}

void consoleMessage_PF(int section,
                       int verbosity,
                      char subsection){

	if(verbosity > 2){
		switch(section){
			
			case 1 :
				
				cout << "pF.gF() -> A. initialization" << endl;
				break;
			case 2 :
				
				cout << "pF.gF() -> B. Assigning trivial features"; 
				cout << endl;
				break;
			case 3 :
				
				cout << "pF.gF() -> C. Inferring nontrivial features";
				cout << endl;
				break;

		}
	}
}

void consoleMessage_main(int section,
                         int verbosity,
                        char subsection,
                      string v_i){
						  
	if (verbosity > 1){
		switch(section){
			
			case 1 :
				
				switch(subsection){
				
					case 'C' : // has cat
					
						cout << "initialized tree" << endl;
						break;
					
					case 'N' : // non cat
					
						cout << "initialized file" << endl;
						break;
				}
			
			case 2 : 
			
				switch(subsection){
				
					case 'C' : // has cat
					
						cout << "generated all the file headers";
						cout << endl;
						break;
					
					case 'N' : // non cat
					
						cout << "generated the file header" << endl;
						break;
				}
				
			case 3 :
				
				switch(subsection){
					
					case 'A' :
					
						cout << "created cell for " << v_i << endl;
						break;
						
					case 'B' :
					
						cout << "summed the channels for " << v_i;
						cout << endl;
						break;
						
					case 'C' :
					
						cout << "flipping the signal upside down";
						cout << endl;
						break;
						
					case 'D' :
					
						cout << "obtained the parameters for " << v_i;
						cout << endl;
						break;
						
					case 'E' :
					
						cout << "obtained the features for " << v_i;
						cout << endl;
						break;
						
					case 'Y' : // has cat
					
						cout << "wrote line for " << v_i;
						cout << " in the corresponding category file";
						cout << endl << endl; 
						break;
					
					case 'Z' : // non cat
					
						cout << "wrote line for " << v_i;
						cout << endl << endl;
						break;
				}
				
			case 4 : 
			
				switch(subsection){
				
					case 'C' : // has cat
					
						cout << "closed tree" << endl;
						break;
					
					case 'N' : // non cat
					
						cout << "closed output file" << endl;
						break;
				}
		}
	}
}
