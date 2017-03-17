#ifndef CYCLEDETECT_MISCUTILS_H
#define CYCLEDETECT_MISCUTILS_H

#include <boost/filesystem.hpp>

inline std::string normalizePath(std::string P) {
  boost::filesystem::path BoostPath(P);
  BoostPath.normalize();
  P = BoostPath.string();

  while (P.size() > 2 && P[0] == '.' && P[1] == '/')
    P = P.substr(2);

  return P;
}

inline bool hasEnding (std::string const &fullString, std::string const &ending) {
  if (fullString.length() >= ending.length()) {
    return (0 == fullString.compare (fullString.length() - ending.length(), ending.length(), ending));
  } else {
    return false;
  }
}


#endif //CYCLEDETECT_MISCUTILS_H
