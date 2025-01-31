#include <ft_ls.h>

void swap(void *a, void *b, int size) {
	void *temp = ft_calloc(size, 1);
	if (!temp)
		return ;
	ft_memmove(temp, a, size);
	ft_memmove(a, b, size);
	ft_memmove(b, temp, size);
	free(temp);
}

void bubble_sort(void *base, int num_elements, size_t element_size,
		int (*compare)(const void *, const void *)) {
	bool swapped;    
	char *arr = (char *)base;

	for (int i = 0; i < num_elements - 1; i++) {
		swapped = false;
		for (int j = 0; j < num_elements - i - 1; j++) {
			if (compare(arr + j * element_size, arr + (j + 1) * element_size) > 0) {
				swap(arr + j * element_size, arr + (j + 1) * element_size, element_size);
				swapped = true;
			}
		}
		if (!swapped)
			break;
	}
}

int cmp_alphabetical(const void *a, const void *b) {
	char *name1 = ((file_t *)a)->alphanum_name;
	char *name2 = ((file_t *)b)->alphanum_name;

	int len_cmp = ft_strlen(((file_t *)a)->name) - ft_strlen(((file_t *)b)->name);
	if (ft_strlen(name1) || ft_strlen(name2))
		len_cmp = -len_cmp;

	return ft_strcasecmp(name1, name2) ?: len_cmp;
}

void sort(file_t *files, int total_entries) {
	/*printf("num of entries %d\n", config->total_entries);*/
	int (*cmp) (const void *, const void*);
	cmp = cmp_alphabetical;
	bubble_sort(files, total_entries, sizeof(file_t), cmp);
}
