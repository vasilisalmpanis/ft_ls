#pragma once

#include <argp.h>
#include <stdbool.h>
#include <stdint.h>
#include <unistd.h>

#include "docs.h"

typedef struct ls_config {
	bool all;	  /* -a option */
	bool long_format; /* -l option */
	bool recursive;	  /* -R option */
	bool human_readable;
} ls_config;
