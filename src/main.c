#include <ft_ls.h>

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

void loop(ls_config *config, file_t *files, window_t *widths)
{
	/*
	* here ideally I should sort the files to be printed
	* before the loop
	*/
	/*file_t *dir_children = NULL;*/
	int index;	

	index = -1;
	(void) widths;
	while (++index < config->total_entries) {
		if (files[index].name[0] == '.' && !config->all)
			continue;

		remove_non_alnum_chars(files[index].name, files[index].alphanum_name);
		if (config->long_format) {
			update_widths(widths, &files[index]);
			continue;
		}
		get_user_uid(files[index].stat.st_uid, files[index].owner_name);
		get_guid(files[index].stat.st_gid, files[index].group_name);
		get_permissions(files[index].stat.st_mode, files[index].permission);
		files[index].indicator = get_indicator(files[index].permission);
		update_widths(widths, &files[index]);
	}

	sort(files, config); // sorted
	/*
	 * print regular files together
	 * print directory contents together if file was a directory
	 * if recursive option is chosen call loop on each sub dir.
	 * return
	 */
	// set_files from dir

	index = 0;
	while (index < config->total_entries) {
		if (files[index].is_dir) {
			index++;
			continue;
		}
		ft_printf("%s ",files[index].name);
		if (!config->long_format) {
			++index;
			continue;
		}
		ft_printf("%s ",files[index].owner_name);
		ft_printf("%s ",files[index].group_name);
		ft_printf("%s ",files[index].permission);
		ft_printf("%c\n",files[index].indicator);
		++index;
	}
	ft_printf("\n");
	index = 0;
	while (index < config->total_entries) {
		if (!files[index].is_dir) {
			index++;
			continue;
		}
		ft_printf("%s ",files[index].name);
		if (!config->long_format) {
			++index;
			continue;
		}
		ft_printf("%s ",files[index].owner_name);
		ft_printf("%s ",files[index].group_name);
		ft_printf("%s ",files[index].permission);
		ft_printf("%c\n",files[index].indicator);
		++index;
	}
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
	struct stat temp;
	if (argc == 1) {
		ft_strlcpy(files->name, ".", 1);
	}
	else {
		for (int i = 1; argv[i]; i++) {
			if (argv[i][0] != '-') {
				if (curr_entry < config->total_entries) {
					if (stat(argv[i], &temp) == 0) {
						ft_strlcpy(files[curr_entry].name, argv[i], ft_strlen(argv[i]) + 1);
						ft_memmove(&files[curr_entry].stat, &temp, sizeof(temp));
						if (S_ISDIR(temp.st_mode))
							files[curr_entry].is_dir = true;
						++curr_entry;
					}
					else {
						fprintf(stderr, "ft_ls: cannot access '%s': %s\n",
							argv[i],
							strerror(errno));
						--config->total_entries;
					}
				}
			}
		}
	}
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
	if (config.total_entries == 0)
		++config.total_entries;
	files = ft_calloc(config.total_entries + 1, sizeof(file_t));
	set_files(&config, files, argc, argv);
	loop(&config, files, &widths);
	/*deinit_config(&config);*/
	return (ret);
}
