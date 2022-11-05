#include <filesystem>
#include <vector>

#include "bits_stream.h"

void Archive(const std::vector<std::filesystem::path> &files, const std::filesystem::path &archive_name);
