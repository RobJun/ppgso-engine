/* Autogenerated by bin2c */

#include "light_frag_glsl.h"

const std::string light_frag_glsl{0x23,0x76,0x65,0x72,0x73,0x69,0x6f,0x6e,0x20,0x33,0x33,0x30,0x20,0x63,0x6f,0x72,0x65,0x0d,0x0a,0x6c,0x61,0x79,0x6f,0x75,0x74,0x20,0x28,0x6c,0x6f,0x63,0x61,0x74,0x69,0x6f,0x6e,0x20,0x3d,0x20,0x30,0x29,0x20,0x6f,0x75,0x74,0x20,0x76,0x65,0x63,0x34,0x20,0x46,0x72,0x61,0x67,0x43,0x6f,0x6c,0x6f,0x72,0x3b,0x0d,0x0a,0x6c,0x61,0x79,0x6f,0x75,0x74,0x20,0x28,0x6c,0x6f,0x63,0x61,0x74,0x69,0x6f,0x6e,0x20,0x3d,0x20,0x31,0x29,0x20,0x6f,0x75,0x74,0x20,0x76,0x65,0x63,0x34,0x20,0x42,0x72,0x69,0x67,0x68,0x74,0x43,0x6f,0x6c,0x6f,0x72,0x3b,0x0d,0x0a,0x0d,0x0a,0x69,0x6e,0x20,0x56,0x53,0x5f,0x4f,0x55,0x54,0x20,0x7b,0x0d,0x0a,0x20,0x20,0x20,0x20,0x76,0x65,0x63,0x33,0x20,0x46,0x72,0x61,0x67,0x50,0x6f,0x73,0x3b,0x0d,0x0a,0x20,0x20,0x20,0x20,0x76,0x65,0x63,0x33,0x20,0x4e,0x6f,0x72,0x6d,0x61,0x6c,0x3b,0x0d,0x0a,0x20,0x20,0x20,0x20,0x76,0x65,0x63,0x32,0x20,0x54,0x65,0x78,0x43,0x6f,0x6f,0x72,0x64,0x73,0x3b,0x0d,0x0a,0x7d,0x20,0x66,0x73,0x5f,0x69,0x6e,0x3b,0x0d,0x0a,0x0d,0x0a,0x75,0x6e,0x69,0x66,0x6f,0x72,0x6d,0x20,0x76,0x65,0x63,0x33,0x20,0x6c,0x69,0x67,0x68,0x74,0x43,0x6f,0x6c,0x6f,0x72,0x3b,0x0d,0x0a,0x0d,0x0a,0x76,0x6f,0x69,0x64,0x20,0x6d,0x61,0x69,0x6e,0x28,0x29,0x0d,0x0a,0x7b,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x0d,0x0a,0x20,0x20,0x20,0x20,0x46,0x72,0x61,0x67,0x43,0x6f,0x6c,0x6f,0x72,0x20,0x3d,0x20,0x76,0x65,0x63,0x34,0x28,0x6c,0x69,0x67,0x68,0x74,0x43,0x6f,0x6c,0x6f,0x72,0x2c,0x20,0x31,0x2e,0x30,0x29,0x3b,0x0d,0x0a,0x20,0x20,0x20,0x20,0x66,0x6c,0x6f,0x61,0x74,0x20,0x62,0x72,0x69,0x67,0x68,0x74,0x6e,0x65,0x73,0x73,0x20,0x3d,0x20,0x64,0x6f,0x74,0x28,0x46,0x72,0x61,0x67,0x43,0x6f,0x6c,0x6f,0x72,0x2e,0x72,0x67,0x62,0x2c,0x20,0x76,0x65,0x63,0x33,0x28,0x30,0x2e,0x32,0x31,0x32,0x36,0x2c,0x20,0x30,0x2e,0x37,0x31,0x35,0x32,0x2c,0x20,0x30,0x2e,0x30,0x37,0x32,0x32,0x29,0x29,0x3b,0x0d,0x0a,0x20,0x20,0x20,0x20,0x69,0x66,0x28,0x62,0x72,0x69,0x67,0x68,0x74,0x6e,0x65,0x73,0x73,0x20,0x3e,0x20,0x31,0x2e,0x30,0x29,0x0d,0x0a,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x42,0x72,0x69,0x67,0x68,0x74,0x43,0x6f,0x6c,0x6f,0x72,0x20,0x3d,0x20,0x76,0x65,0x63,0x34,0x28,0x46,0x72,0x61,0x67,0x43,0x6f,0x6c,0x6f,0x72,0x2e,0x72,0x67,0x62,0x2c,0x20,0x31,0x2e,0x30,0x29,0x3b,0x0d,0x0a,0x09,0x65,0x6c,0x73,0x65,0x0d,0x0a,0x09,0x09,0x42,0x72,0x69,0x67,0x68,0x74,0x43,0x6f,0x6c,0x6f,0x72,0x20,0x3d,0x20,0x76,0x65,0x63,0x34,0x28,0x30,0x2e,0x30,0x2c,0x20,0x30,0x2e,0x30,0x2c,0x20,0x30,0x2e,0x30,0x2c,0x20,0x31,0x2e,0x30,0x29,0x3b,0x0d,0x0a,0x7d,};
