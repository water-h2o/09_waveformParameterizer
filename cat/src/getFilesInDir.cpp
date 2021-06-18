#include <iostream>
#include <getFilesInDir.h>
#include <experimental/filesystem>
#include <algorithm>

using namespace std;

void read_directory(const std::string& name,
                            stringvec& v){

	fs::path p(name);
    fs::directory_iterator start(p);
    fs::directory_iterator end;
    transform(start, end, std::back_inserter(v), path_leaf_string());

}
