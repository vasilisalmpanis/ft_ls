#include <dirent.h>
#include <ft_ls.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

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
	if (ls_config->total_entries + 1 > 1023)
		return (0);
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

/**
 * @brief deinit config
 * Free all the allocated memory and possibly
 * close all open file desc.
 * @param config 
 */
/*void deinit_config(ls_config *config)*/
/*{*/
/*	int index;*/
/**/
/*	index = 0;*/
/*	}*/
/*}*/

void loop(ls_config *config, file_t *files)
{
	/*
	* here ideally I should sort the files to be printed
	* before the loop
	*/
	int index;	

	index = -1;
	while (++index < config->total_entries) {
		printf("index: %d, total_entries %d\n", index, config->total_entries);
		if (files[index].name[0] == '.' && !config->all)
			continue;
		lstat(files[index].name, (struct stat *)&files[index]);
		remove_non_alnum_chars(files[index].name, files[index].alphanum_name);
		get_user_uid(files[index].stat.st_uid, files[index].owner_name);
		get_guid(files[index].stat.st_gid, files[index].group_name);
	}
	index = 0;
	/*while (index < config->total_entries) {*/
	/*	printf("%s\n",files[index].name);*/
	/*	printf("%s\n",files[index].alphanum_name);*/
	/*	printf("%s\n",files[index].owner_name);*/
	/*	printf("%s\n",files[index].group_name);*/
	/*	index++;*/
	/*}*/
}
/**
 * @brief set file_t struct with file names
 *
 * @param config  global config
 * @param files array of file_t of total_entries + 1
 * @param argc arg count 
 * @param argv array of args passed to program.
 */
void set_files(ls_config *config, file_t *files, int argc, char **argv)
{
	int curr_entry = 0;
	DIR *dir = NULL;
	if (argc == 1) {
		strcpy(files->name, ".");
	}
	else {
		for (int i = 1; argv[i]; i++) {
			if (argv[i][0] != '-') {
				if (curr_entry < config->total_entries) {
					strcpy(files[curr_entry].name, argv[i]);
					if ((dir = opendir(files[curr_entry].name))) {
						files[curr_entry].is_dir = true;
						closedir(dir);
					}
					++curr_entry;
				}
			}
		}
	}
}

int main(int argc, char *argv[])
{
	int ret;
	file_t *files;

	ls_config	  config;
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
	if (config.total_entries == 0)
		++config.total_entries;
	files = ft_calloc(config.total_entries + 1, sizeof(file_t));
	set_files(&config, files, argc, argv);
	loop(&config, files);
	/*deinit_config(&config);*/
	return (ret);
}
