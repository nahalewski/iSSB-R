#pragma once
// The bundled SDL predates NDK 27's removal of ALooper_pollAll.
// Include the platform declarations before redirecting SDL's nonblocking poll.
#include <android/looper.h>
#define ALooper_pollAll ALooper_pollOnce
