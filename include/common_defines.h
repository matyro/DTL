#pragma once

#ifndef COMMON_DEFINES_H
#define COMMON_DEFINES_H


/// GCC uses this value for every version under 4.7
#if __cplusplus >= 1
#define CPP_pre9
#endif

#if __cplusplus >= 199711L
#define CPP_98
#endif

#if __cplusplus >= 201103L
#define CPP_11
#endif

#if __cplusplus >= 201402L
#define CPP_14
#endif

#if __cplusplus >= 201703L
#define CPP_17
#endif




#endif /* end of include guard: COMMON_DEFINES_H */
