#ifndef GETFILESINDIR_H
#define GETFILESINDIR_H

#include <experimental/filesystem>
#include "myTypedefs.h"

namespace fs = std::experimental::filesystem;

struct path_leaf_string
{
	std::string operator()(const fs::directory_entry& entry) const
	{
		return entry.path().filename().string();
	}
};

// important functions =================================================

void read_directory(const std::string& name,
                            stringvec& v);

#endif // GETFILESINDIR_H

