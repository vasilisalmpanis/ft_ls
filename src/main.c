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
int set_next_entry(ls_config *ls_config, char *arg) {
	if (ls_config->total_entries + 1 > 1023)
		return (0);
	ls_config->files[ls_config->total_entries] = ft_strdup(arg);
	if (!ls_config->files[ls_config->total_entries])
		return (1);
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
			set_next_entry(ls_config, arg);
			break;
		case ARGP_KEY_END:
			if (ls_config->files[0] == NULL) {
				ls_config->files[0] = ft_strdup(".");
				if (!ls_config->files[0])
					exit(1);
				++ls_config->total_entries;
			}
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
void init_config(ls_config *config) {
	ft_bzero(config, sizeof(ls_config));
	config->all = false;
}

/**
 * @brief deinit config
 * Free all the allocated memory and possibly
 * close all open file desc.
 * @param config 
 */
void deinit_config(ls_config *config) {
	int index;

	index = 0;
	while (config->files[index]) {
		free(config->files[index]);
		index++;
	}
}


/**
 * @brief debug_config
 * Print out the contents of ls_config
 * for debugging purposes in a structured
 * way.
 *
 * @param config 
 */
void debug_config(ls_config *config) {
	int index;

	index = 0;
	printf("l: %d\n", config->long_format);
	printf("R: %d\n", config->recursive);
	printf("a: %d\n", config->all);
	printf("t: %d\n", config->time_sort);
	printf("d: %d\n", config->directory);
	printf("h: %d\n", config->human_readable);
	printf("entries: %d\n", config->total_entries);
	while (config->files[index]) {
		printf("file: %s\n", config->files[index]);
		index++;
	}
}

int main(int argc, char *argv[])
{
	int ret;

	ls_config	  config;
	const struct argp argp = {
		.options = options,
		.parser = parse_opt,
		.args_doc = LS_DOC,
		.doc = DESC,
		0
	};
	init_config(&config);
	ret = argp_parse(&argp, argc, argv, 0, 0, &config);
	debug_config(&config);
	deinit_config(&config);
	return (ret);
}
