#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <string>

std::vector<std::string> split(std::string);
std::string join(const std::vector<std::string> &v, char c);
std::string file_to_string(const std::string &path);
void log_to_file(const std::string &path, const std::string &message, int sd);

#endif