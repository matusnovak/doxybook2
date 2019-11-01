#pragma once
#include <stdio.h>

/*!
 * @ingroup Utils
 */
#define LOG_DEBUG(MSG, ...) printf(MSG, ##__VA_ARGS__)
