#pragma once

// size_t, NULL
#include <stddef.h>

// <stdio.h> types
#ifndef ssize_t
typedef long ssize_t;
#endif

#ifndef FILE
#include <bits/types/FILE.h>
#endif
