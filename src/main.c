#include "libft/libft.h"
#include <dirent.h>
#include <ft_ls.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
file_t *get_dir_content(ls_config *config, char *file, int num_of_files);
int get_dir_file_count(ls_config *config, char *name);

struct argp_option options[] =
{
	{
		.name   = 0,
		.key    = 'l',
		.arg    = 0,
		.flags  = 0,
		.doc    = "use a long listing format", 
		0
	},
	{
		.name   = "recursive",
		.key    = 'R',
		.arg    = 0,
		.flags  = 0,
		.doc    = "list subdirectories recursively", 
		0
	},
	{
		.name   = "all", 
		.key    = 'a',
		.arg    = 0,
		.flags  = 0,
		.doc    = "do not ignore entries starting with .",
		0
	},
	{
		.name   = "reverse",
		.key    = 'r',
		.arg    = 0,
		.flags  = 0,
		.doc    = "reverse order while sorting",
		0
	},
	{
		.name   = 0,
		.key    ='t',
		.arg    = 0,
		.flags  = 0,
		.doc    = "sort by time, newest first",
		0
	},
	{
		.name   = "directory",
		.key    = 'd',
		.arg    = 0,
		.flags  = 0,
		.doc    = "list directories themselves, not their contents", 0},
	{
		.name	 = "human-readable",
		.key	 = 'h',
		.arg	 = 0,
		.flags = 0,
		.doc	 = "with -l and -s, print sizes like 1K 234M 2G etc.",
		0
	},
	{ 0 }
};

/**
 * @brief set_next_entry
 * Sets the next entry to be analized.
 *
 * @param ls_config 
 * @param arg the file argument
 * @return 0 on success 1 if an error occurs
 */
int set_next_entry(ls_config *ls_config)
{
	/*ls_config->files[ls_config->total_entries] = ft_strdup(arg);*/
	/*if (!ls_config->files[ls_config->total_entries])*/
		/*return (1);*/
	++ls_config->total_entries;
	return (0);
}

/**
 * @brief Parse user provided options and setup the config
 * struct
 *
 * @param key ascii character representing the option passed 
 * @param arg full str of the long option
 * @param state argp_state of parsing
 * @return 0 on success.
 */
static int parse_opt(int key, char *arg, struct argp_state *state)
{
	(void)state;
	(void)arg;
	ls_config *ls_config = state->input;
	switch (key) {
		case 'l':
			ls_config->long_format = true;
			break;
		case 'R':
			ls_config->recursive = true;
			break;
		case 'a':
			ls_config->all = true;
			break;
		case 't':
			ls_config->time_sort = true;
			break;
		case 'd':
			ls_config->directory = true;
			break;
		case 'h':
			ls_config->human_readable = true;
			break;
		case ARGP_KEY_ARG:
			set_next_entry(ls_config);
			break;
		case ARGP_KEY_END:
			break;
		default:
			return ARGP_ERR_UNKNOWN;
	}
	return 0;
}

/**
 * @brief init config
 * Initializes the fields of the ls_config
 * struct to the default value.
 *
 * @param config 
 */
void init_config(ls_config *config)
{
	ft_bzero(config, sizeof(ls_config));
	config->isatty = isatty(1);
}

int get_col_count(column_info *column_configs, int file_count, file_t *files, window_t *widths)
{
	int max_idx = widths->window_width / MIN_COLUMN_WIDTH - 1;
	int max_cols = (max_idx < file_count) ? max_idx : file_count;
	/*int inode = (options->show_inode) ? widths->inode_width : 0;*/
	int inode = 1;
	for (int file_idx = 0; file_idx < file_count; ++file_idx) {
		for (int config_idx = 0; config_idx < max_cols; ++config_idx) {
			if (!column_configs[config_idx].valid)
				continue;
			int rows = (file_count + config_idx) / (config_idx + 1);

			if ((rows * (config_idx + 1) - file_count) > rows)
			{
				column_configs[config_idx].valid = 0;
				continue;
			}
			int col = file_idx / rows;
			int name_width = ft_strlen(files[file_idx].name);

			/* add later */
			/*if (options->inside_quotes)*/
			/*	name_width += 2;*/
			/**/
			/*if (options->append_file_indicators && files[file_idx].indicator)*/
			/*	name_width += 1;*/
			/*else if (options->append_file_indicators && files[file_idx].permission[0] == 'd')*/
			/*	name_width += 1;*/
			if (name_width > column_configs[config_idx].max_len[col])
			{
				column_configs[config_idx].line_len +=
					name_width - column_configs[config_idx].max_len[col];
				column_configs[config_idx].max_len[col] = name_width;
			}

			// Consider spaces as well
			if (column_configs[config_idx].line_len +
					(2 * config_idx) + (inode * (config_idx + 1)) > widths->window_width)
				column_configs[config_idx].valid = 0;
		}
	}
	int selected_config = max_cols - 1;
	while (
			selected_config >= 0
			&& (!column_configs[selected_config].valid
			|| !column_configs[selected_config].max_len[selected_config])
		)
		selected_config--;

	return selected_config + 1;
}

void print_tabular(file_t *files, int file_count, window_t *widths, bool begin) {
	column_info *column_configs;
	int ncols;
	int nrows;
	int dir_count = 0;
	int printed;

	column_configs = ft_calloc(file_count, sizeof(column_info));	
	if (!column_configs)
		return ;
	for (int i=0; i < file_count; i++) {
		column_configs[i].valid = 1;
		column_configs[i].max_len = ft_calloc(file_count, sizeof(int));
		if (files[i].is_dir)
			dir_count++;
		if (column_configs[i].max_len == NULL) {
			if (i == 0) {
				free(column_configs);
			}
			else {
				for (int j = i - 1; j >= 0; j--)
					free(column_configs[j].max_len);
				free(column_configs);
			}
			return ;
		}
	}
	ncols = get_col_count(column_configs, file_count, files, widths);
	ncols += ncols == 0 ? 1 : 0;
	if (begin)
		nrows = (file_count + ncols - 1) / ncols;
	else
		nrows = (file_count - dir_count + ncols - 1) / ncols;

	for (int i = 0; i < nrows; i++)
	{
		printed = 0;
		for (int j = 0; j < ncols; j++)
		{
			int file_idx = i + j * nrows;

			/*if (options->show_inode)*/
				/*printf("%*d ", widths->inode_width, files[file_idx].st_ino);*/

			/*print_padded_name(files[file_idx], options, column_configs[ncols - 1].max_len[j]);*/
			// for now //
			if (begin && files[file_idx].is_dir)
				continue;
			if (file_idx < file_count)
				printed += printf("%-*s", column_configs[ncols - 1].max_len[j], files[file_idx].name);
			if (j < ncols - 1)
				printed += printf("  ");
		}
		if (printed)
			printf("\n");
	}
	for (int i=0; i < file_count; i++) {
		free(column_configs[i].max_len);
	}
	free(column_configs);
	// print stuff //
}

void print_ls(file_t *files, int file_count, window_t *widths, bool begin)
{
	if (!file_count)
		return;

	/*if (options->print_style == LIST_FORMAT)*/
	/*    print_list(files, count, options, widths);*/
	/*else if (options->print_style == COMMA_SEPARATED_FORMAT)*/
	/*    print_comma_sep(files, count, options, widths);*/
	/*else*/
	/*for (int i = 0; i < file_count; i++) {*/
		/*printf("%s\n", files[i].name);*/
		/*(void)widths;*/
	/*}*/
	print_tabular(files, file_count, widths, begin);
}

void loop(ls_config *config, int file_count, file_t *files, window_t *widths)
{
	/*
	* here ideally I should sort the files to be printed
	* before the loop
	*/
	/*file_t *dir_children = NULL;*/
	int index;	

	index = -1;
	(void) widths;
	while (++index < file_count) {
		if (files[index].name[0] == '.' && !config->all)
			continue;

		remove_non_alnum_chars(files[index].name, files[index].alphanum_name);
		if (config->long_format) {
			update_widths(widths, &files[index]);
			continue;
		}
		// biggest bottleneck so far is here
		// solutions linked list of uid, guid and str repr to hold these
		// and not ask for them all the time

		/*get_user_uid(files[index].stat.st_uid, files[index].owner_name);*/
		/*get_guid(files[index].stat.st_gid, files[index].group_name);*/
		get_permissions(files[index].stat.st_mode, files[index].permission);
		files[index].indicator = get_indicator(files[index].permission);
		update_widths(widths, &files[index]);
	}

	sort(files, file_count); // sorted
	/*
	 * print regular files together
	 * print directory contents together if file was a directory
	 * if recursive option is chosen call loop on each sub dir.
	 * return
	 */

	for (int i = 0; i < file_count; i++) {
		if (files[i].is_dir) {
			int num_of_files = get_dir_file_count(config, files[i].name);
			file_t *temp = get_dir_content(config, files[i].name, num_of_files);
			if (i > 0)
				printf("\n");
			if (config->print_dir_names)
				printf("%s:\n", files[i].name);
			if (!temp)
				continue;
			print_ls(temp, num_of_files, widths, true);
			free(temp);
			/*if (file_count > 1 && i < file_count - 1)*/
				/*printf("\n");*/
		}
	}
}

int get_dir_file_count(ls_config *config, char *name) {
	DIR *directory;
	size_t num_of_files;
	struct dirent *content;

	num_of_files = 0;
	directory = NULL;
	if (!(directory = opendir(name)))
		return 0;
	while ((content = readdir(directory))) {
		/*printf("FILENAME: %s\n", content->d_name);*/
		if (content->d_name[0] == '.'  && !config->all)
			continue;
		num_of_files++;
	}
	closedir(directory);
	return num_of_files;
}

void fill_files(ls_config *config, file_t *files, char *dir) {
	DIR *directory;
	struct dirent *content;
	int i = 0;

	directory = NULL;
	if (!(directory = opendir(dir)))
		return;
	if (chdir(dir)) {
		printf("error changing current working dir\n");
	}
	while ((content = readdir(directory))) {
		if (content->d_name[0] == '.'  && !config->all)
			continue;

		if (stat(content->d_name, &files[i].stat))
			continue;
		ft_memmove(&files[i].name, content->d_name, ft_strlen(content->d_name));
		i++;
	}
	if (!(ft_strlen(dir) == 1 && !ft_memcmp(".", dir, ft_strlen(dir)))) {
		chdir("..");
	}
	closedir(directory);
	return;
}

file_t *get_dir_content(ls_config *config, char *dir, int num_of_files)
{
	file_t *files;

	if (num_of_files == 0)
		return NULL;
	files = (file_t *)malloc((num_of_files + 1) * sizeof(file_t));
	if (!files)
		return NULL;
	ft_bzero(files, (num_of_files + 1) * sizeof(file_t));
	fill_files(config, files, dir);
	return files;
}

/**
 * @brief set file_t struct with file names
 *
 * @param config  global config
 * @param files array of file_t of total_entries + 1
 * @param argc arg count 
 * @param argv array of args passed to program.

 */
int set_files(ls_config *config, file_t *files, char **argv)
{
	int curr_entry = 0;
	int dir_count = 0, file_count = 0;
	int ret = 0;
	struct stat temp;
	for (int i = 1; argv[i]; i++) {
		if (argv[i][0] != '-') {
			if (curr_entry < config->total_entries) {
				if (stat(argv[i], &temp) == 0) {
					ft_strlcpy(files[curr_entry].name, argv[i], ft_strlen(argv[i]) + 1);
					ft_memmove(&files[curr_entry].stat, &temp, sizeof(temp));
					if (S_ISDIR(temp.st_mode)) {
						files[curr_entry].is_dir = true;
						dir_count++;
					} else {
						file_count++;
					}
					++curr_entry;
				}
				else {
					fprintf(stderr, "ft_ls: cannot access '%s': %s\n",
							argv[i],
							strerror(errno));
					--config->total_entries;
					ret = 2;
				}
			}
		}
	}
	if (dir_count > 1)
		config->print_dir_names = true;
	else if (dir_count >= 1 && file_count >= 1)
		config->print_dir_names = true;
	if (config->recursive)
		config->print_dir_names = true;
	return ret;
}

int create_initial_struct(file_t **files, ls_config *config, char **argv)
{
	int ret = 0;
	if (config->total_entries == 0) {
		config->total_entries++;
		*files = ft_calloc(config->total_entries, sizeof(file_t));
		ft_strlcpy((*files)[0].name, ".", 2);
		if (stat(".", &(*files)[0].stat) == 0) {
			if (S_ISDIR((*files)[0].stat.st_mode))
				(*files)[0].is_dir = true;
		} else {
			ret = 2;
		}
		if (config->recursive)
			config->print_dir_names = true;
	} else {
		*files = ft_calloc(config->total_entries, sizeof(file_t));
		ret = set_files(config, *files, argv);
	}
	return ret;
}

int main(int argc, char *argv[])
{
	int ret;
	file_t *files;
	window_t widths;

	ls_config	  config;
	widths.window_width = get_window_width();
	files = NULL;
	const struct argp argp = {
		.options = options,
		.parser = parse_opt,
		.args_doc = LS_DOC,
		.doc = DESC,
		0
	};
	init_config(&config);
	ret = argp_parse(&argp, argc, argv, 0, 0, &config);
	ret = create_initial_struct(&files, &config, argv);
	sort(files, config.total_entries);
	print_ls(files, config.total_entries, &widths, true);
	// looks like we first sort all regular files and print them and then do the same with folders
	loop(&config, config.total_entries, files, &widths);
	free(files);
	return (ret);
}

/*
 * Steps for ft_ls
 * 1. Parse input
 * 2. Set content using stat
 * 3. Sort
 * 4. Print non dir files if input for ft_ls was more than one
 * 5. Go to directories and print all contents
 * 6. If recursive option is set got 2 but now print all files.
 */
