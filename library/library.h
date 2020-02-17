#pragma once

#ifdef IB_LIB
int __delcspec(dllexport) foosticles(int blah);
#else
int __declspec(dllimport) foosticles(int blah);
#endif