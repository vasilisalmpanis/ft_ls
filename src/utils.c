#include "../include/ft_ls.h"
#include <linux/limits.h>
#include <pwd.h>

/**
 * @brief count how many files we have to print
 *
 * @param config  global config
 * @param directory the directory to count for
 * @return amount of files.
 */
int count_file_num(ls_config *config, char *directory) {
	struct dirent *file;
	int file_num;
	DIR *dir;

	file_num = 0;
	dir = opendir(directory);
	if (!dir)
		return -1;
	while ((file = readdir(dir))) {
		if (ft_strncmp(file->d_name, ".", ft_strlen(file->d_name)) ||
			ft_strncmp(file->d_name, "..", ft_strlen(file->d_name))) {
			if (config->all)
				++file_num;
		}
	}
	closedir(dir);
	return file_num;
}

/**
 * @brief debug_config
 * Print out the contents of ls_config
 * for debugging purposes in a structured
 * way.
 *
 * @param config 
 */
void debug_config(ls_config *config)
{
	/*int index;*/

	/*index = 0;*/
	printf("l: %d\n", config->long_format);
	printf("R: %d\n", config->recursive);
	printf("a: %d\n", config->all);
	printf("t: %d\n", config->time_sort);
	printf("d: %d\n", config->directory);
	printf("h: %d\n", config->human_readable);
	printf("entries: %d\n", config->total_entries);
	printf("isatty: %d\n", config->isatty);
	/*while (config->files[index]) {*/
	/*	printf("file: %s\n", config->files[index]);*/
	/*	index++;*/
	/*}*/
}


/**
 * @brief remove non alnma characters from string
 *
 * @param name pointer to name array
 * @param alnumname dest array
 */
void remove_non_alnum_chars(char *name, char *alnumname)
{
    int len = ft_strlen(name);
    int j = 0;

    for (int i = 0; i < len; ++i)
        if (ft_isalnum(name[i]))
            alnumname[j++] = name[i];
}

/**
 * @brief get the user name
 *
 * @param uid acquired from lstat
 * @param user ptr to username array
 */
void get_user_uid(int uid, char *user)
{
	struct passwd *pws;

	pws = getpwuid(uid);
	ft_strlcpy(user, pws->pw_name, PATH_MAX);
}

/**
 * @brief get the group name
 *
 * @param gid acquired from lstat
 * @param group ptr to group array
 */
void get_guid(int gid, char *group)
{
	struct group *grp;

	grp = getgrgid(gid);
	ft_strlcpy(group, grp->gr_name, PATH_MAX);
}
