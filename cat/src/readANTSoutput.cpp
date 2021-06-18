#include <iostream>
#include <readANTSoutput.h>

#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

a_cell readANTSfile(string fname){
	
	a_cell out;
	
	ifstream in_file(fname);
	string line;
	a_sentence line_div;
	
	string PMT_ID;
	a_column curr_col;
	
	int wrap, PMTdef, nxtCol, number;
	int typeOfLine;
	
	while (std::getline(in_file, line) ){
		
		wrap   = 1*b2i(line.find('{') != string::npos || line.find('}') != string::npos);
		//~ cout << "readANTSfile: wrap: " << wrap << endl;
		PMTdef = 2*b2i(line.find(": [") != string::npos);
		//~ cout << "readANTSfile: PMTdef: " << PMTdef << endl;
		nxtCol = 3*b2i(line.find("],") != string::npos);
		//~ cout << "readANTSfile: nxtCol: " << nxtCol << endl;
		number = 4*b2i(line.find("            ") != string::npos);
		//~ cout << "readANTSfile: nxtCol: " << nxtCol << endl;
				
		typeOfLine = wrap + PMTdef + nxtCol + number;
		
		//~ cout << "readANTSfile: typeOfLine: " << typeOfLine << endl;
		
		switch (typeOfLine){
			
			case 1:  
			         //~ cout << "reached delimiter" << endl;
			
					 break;
			
			case 2:  
			         //~ cout << "found def of PMT" << endl;
					
					 split(line, '"', line_div);
					 //~ PMT_ID = "PMT_" + line_div[1];
					 PMT_ID = line_div[1];
					 //~ cout << PMT_ID << endl;;
					 line_div.clear();
			
					 break;
			
			case 3:  
			         //~ cout << "switching columns" << endl;
					
					 out[PMT_ID] = curr_col;
					 curr_col.clear();
					
					 break;
			
			case 4:  
					{
			         string numinline = line.substr(12,line.size()-13);
				     curr_col.push_back(numinline);
					
				     break;
					}
			default: 
			         //~ cout << "reached PMT delimiter" << endl;
			
			         break;
		}
		
	}
	
	return out;
	
}

void saveCellToFile(a_cell const& in, string out_fname){
	
	
	ofstream out_file;
	out_file.open(out_fname);
	
	a_column curr_col;
	
	cout << "saveCellTofile got here 1" << endl;
	
	for(auto const& element : in){
	
		//~ cout << "saveCellTofile got here 2" << endl;
		
		curr_col = element.second;
		
		out_file << element.first + "\t";
		
		//~ cout << "saveCellTofile got here 3" << endl;
		
		for (int i = 0 ; i < curr_col.size() - 1 ; i++){
		
			out_file << curr_col[i] + "\t";
			//~ cout << "saveCellTofile got here 4" << endl;
			
		}
		
		cout << "finished 'column' " << element.first << endl;
		out_file << curr_col.back() + "\n";
		
		curr_col.clear();
	} 

	out_file.close();
	
}

void split(const string& s, 
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

a_sentence getKeys(a_cell const& c){
	
	a_sentence keys;
	
	for(auto const& element : c){
		keys.push_back(element.first);
	} 
	
	return keys;
	
}

vector<int> getLengths(a_cell const& c){
	
	vector<int> lengths;
	a_column curr_col;
	
	for(auto const& element : c){
		
		curr_col = element.second;
		lengths.push_back(curr_col.size());
	} 
	
	return lengths;
}

a_sentence getIdx_sen(a_cell const& c, int idx){

	a_sentence idx_sen;
	a_column curr_col;
	
	for(auto const& element : c){
		
		curr_col = element.second;
		
		if (idx >= curr_col.size()) {
			idx_sen.push_back("");
		} else {
			idx_sen.push_back(curr_col[idx]);
		}
		
	} 
	
	return idx_sen;
	
}

string makeLine(a_sentence idx_sen){

	string line = "";
	
	for (int i = 0; i < (idx_sen.size()-1) ; i++){
		line += idx_sen[i] + "\t";
	}
	
	line += idx_sen.back() + "\n";
	
	return line;
	
}

// =====================================================================
// == Parametrizer functions ===========================================
// =====================================================================

a_signal sumChannels(a_cell const& in){

	//~ cout << "rAo.sC() -> entered" << endl;

	vector<int> lengths = getLengths(in);
	int waveform_length = *std::max_element(std::begin(lengths),
	                                        std::end(  lengths) );

	//~ cout << "rAo.sC() -> obtained waveform_length: " << waveform_length << endl;

	a_sentence keys = getKeys(in);

	//~ cout << "rAo.sC() -> obtained keys" << endl;

	a_signal out(waveform_length, 0);

	for(int i = 0; i < keys.size(); i++){
		
		a_signal curr_signal = a_column2a_signal(in.at(keys[i]));
		
		//~ cout << "rAo.sC() -> curr_signal.size(): " << curr_signal.size() << endl;
		
		for(int j = 0; j < curr_signal.size(); j++){
			
			out[j] += curr_signal[j];
		}
	}
	
	return out; 
}

a_signal a_column2a_signal(a_column in){

	a_signal out;
	
	in = taperOutEmptyElements(in);
	
	//~ cout << "rAo.a_c2a_s -> entered" << endl; 
	
	for(int i = 0; i < in.size(); i++){
		
		//~ cout << "rAo.a_c2a_s -> pushing back " << in[i] << ", element no " << i << " of " << in.size() << endl; 
		
		out.push_back(std::stod(in[i]));
		
		//~ cout << "rAo.a_c2a_s -> pushed back " << in[i] << ", element no " << i << " of " << in.size() << endl; 
		
	}
	
	//~ cout << "rAo.a_c2a_s -> returning" << endl;
	
	return out;

}

a_column taperOutEmptyElements(a_column in){
	
	int taper_L = 0;
	int taper_R = 0;
	
	for(int i = 0; i < in.size(); i++){
		
		if(in[i].size() > 0){
			
			taper_L = i;
			break;
		}
	}
	
	for(int i = in.size() - 1; i >= 0; i--){
		
		if(in[i].size() > 0){
			
			taper_R = i;
			break;
		}
	}
	
	a_column out(in.begin() + taper_L, in.begin() + taper_R);
	
	return out;
	
}

void signalUpsideDown(a_signal& in){
	
	std::reverse( in.begin(), in.end() );
	
	idx fstNonzero = -1;
	idx i = 0;
	
	while(fstNonzero < 0){
		
		if(in[i] != 0) fstNonzero = i;
		
		i += 1;
	}
	
	
	
	in.erase(in.begin(), in.begin() + fstNonzero);
	
	for(int j = 0; j < fstNonzero; j++) in.push_back(0.0);
}
