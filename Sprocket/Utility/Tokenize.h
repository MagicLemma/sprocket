#pragma once

#include <vector>
#include <string>

namespace Sprocket {

std::vector<std::string> tokenize(const std::string& str,
                                  const std::string& delimiter = " ");

}