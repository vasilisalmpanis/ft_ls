#pragma once

#include <argp.h>
#include <stdbool.h>
#include <stdint.h>
#include <unistd.h>

#include "docs.h"
#include "../libft/libft.h"

typedef struct ls_config {
	bool all;	  	/* -a option */
	bool long_format; 	/* -l option */
	bool recursive;	  	/* -R option */
	bool human_readable;	/* -h option */
	bool directory;		/* -d option */
	bool time_sort;		/* -t option */
	bool isatty;
	char *files[1024];	/* array of files to print */
	int  total_entries;
} ls_config;
