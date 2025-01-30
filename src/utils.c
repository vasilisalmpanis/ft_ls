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

int get_window_width()
{
    struct winsize w;
    ioctl(0, TIOCGWINSZ, &w);
    return w.ws_col;
}

void update_widths(window_t *widths, file_t *file) {
	widths->inode_width = MAX(widths->inode_width, NUM_LEN((int)file->stat.st_ino));
	widths->nlink_width = MAX(widths->nlink_width, NUM_LEN((int)file->stat.st_nlink));
	widths->ownerid_width = MAX(widths->ownerid_width, NUM_LEN(file->stat.st_uid));
	widths->ownername_width = MAX(widths->ownername_width, (int)ft_strlen(file->owner_name));
	widths->groupid_width = MAX(widths->groupid_width, NUM_LEN(file->stat.st_gid));
	widths->groupname_width = MAX(widths->groupname_width, (int)ft_strlen(file->group_name));
	int size_width = NUM_LEN((int)file->stat.st_size);
	if (S_ISCHR(file->stat.st_mode) || S_ISBLK(file->stat.st_mode))
		size_width = NUM_LEN(major(file->stat.st_rdev)) + NUM_LEN(minor(file->stat.st_rdev)) + 2; // major, minor
	widths->size_width = MAX(widths->size_width, size_width);
	widths->total_blocks += file->stat.st_blocks / 2;
}

/**
 * @brief return file indicator based on permission string
 *
 * @param permission ptr to string
 * @return char indicator
 */
char get_indicator(char *permission)
{
    switch (permission[0])
    {
    case '-':
        if (permission[3] == 'x' || permission[6] == 'x' || permission[9] == 'x')
            return '*'; // executable

        return 0;
    case 'd': 
        return '/'; // directory
    case 'l':
        return '@'; // symbolic link
    case 's':
        return '='; // socket
    case 'p':
        return '|'; // pipe
    default:
        return 0;
    }
}

void get_permissions(mode_t mode, char *permission)
{
    if (S_ISREG(mode))
        permission[0] = '-';
    else if (S_ISDIR(mode))
        permission[0] = 'd';
    else if (S_ISLNK(mode))
        permission[0] = 'l';
    else if (S_ISCHR(mode))
        permission[0] = 'c';
    else if (S_ISBLK(mode))
        permission[0] = 'b';
    else if (S_ISSOCK(mode))
        permission[0] = 's';
    else if (S_ISFIFO(mode))
        permission[0] = 'p';

    permission[1] = (mode & S_IRUSR) ? 'r' : '-';
    permission[2] = (mode & S_IWUSR) ? 'w' : '-';
    permission[3] = (mode & S_IXUSR) ? 'x' : '-';

    permission[4] = (mode & S_IRGRP) ? 'r' : '-';
    permission[5] = (mode & S_IWGRP) ? 'w' : '-';
    permission[6] = (mode & S_IXGRP) ? 'x' : '-';

    permission[7] = (mode & S_IROTH) ? 'r' : '-';
    permission[8] = (mode & S_IWOTH) ? 'w' : '-';
    permission[9] = (mode & S_IXOTH) ? 'x' : '-';
    /*permission[10] = 0;*/
}
