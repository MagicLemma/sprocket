#pragma once

#include <vector>
#include <string>

namespace Sprocket {

std::vector<std::string> Tokenize(const std::string& str,
                                  const std::string& delimiter = " ");

}