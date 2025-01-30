#pragma once

#include <argp.h>
#include <bits/posix1_lim.h>
#include <stdbool.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/stat.h>
#include <linux/limits.h>
#include <sys/types.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>

#include "docs.h"
#include "../libft/libft.h"

typedef struct file_t {
	struct stat stat;
	char name[PATH_MAX];
	char alphanum_name[PATH_MAX];
	char permission[11];
	char owner_name[LOGIN_NAME_MAX];
	char group_name[LOGIN_NAME_MAX];
	bool is_dir;
} file_t;

typedef struct ls_config {
	bool all;	  	/* -a option */
	bool long_format; 	/* -l option */
	bool recursive;	  	/* -R option */
	bool human_readable;	/* -h option */
	bool directory;		/* -d option */
	bool time_sort;		/* -t option */
	bool isatty;
	int  total_entries;
} ls_config;

void remove_non_alnum_chars(char *name, char *alnumname);
void get_user_uid(int uid, char *user);
void get_guid(int gid, char *group);
int count_file_num(ls_config *config, char *directory);
void debug_config(ls_config *config);
