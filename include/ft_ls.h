#pragma once

#include <argp.h>
#include <bits/posix1_lim.h>
#include <stdbool.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/stat.h>
#include <linux/limits.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <string.h>
#include <sys/sysmacros.h>

#include "docs.h"
#include "../ft_printf/ft_printf.h"

#define MAX(a,b) (((a) > (b)) ? (a) : (b))
#define NUM_LEN(n) (snprintf(0, 0, "%+d", (n)) - 1)
#define MIN_COLUMN_WIDTH 3

typedef struct
{
    int valid;
    int line_len;
    int *max_len; // Max possible length for each col
} column_info;

typedef struct window_t {
	int window_width;
	int inode_width;
	int nlink_width;
	int ownername_width;
	int ownerid_width;
	int groupname_width;
	int groupid_width;
	int size_width;
	int total_blocks;
} window_t;

typedef struct file_t {
	struct stat stat;
	char name[PATH_MAX];
	char alphanum_name[PATH_MAX];
	char permission[11];
	char owner_name[LOGIN_NAME_MAX];
	char group_name[LOGIN_NAME_MAX];
	char indicator;
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
	bool print_dir_names;
	int  total_entries;
} ls_config;

void remove_non_alnum_chars(char *name, char *alnumname);
void get_user_uid(int uid, char *user);
void get_guid(int gid, char *group);
int count_file_num(ls_config *config, char *directory);
void debug_config(ls_config *config);
int get_window_width();
void update_widths(window_t *widths, file_t *file);
char get_indicator(char *permission);
void get_permissions(mode_t mode, char *permission);

/* sort */
void sort(file_t *files, int total_entries);
