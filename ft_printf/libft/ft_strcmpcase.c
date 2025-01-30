#include "libft.h"

int ft_strcasecmp(const char *s1, const char *s2) {
    unsigned char c1, c2;
    
    while (*s1 && *s2) {
        c1 = ft_tolower((unsigned char)*s1);
        c2 = ft_tolower((unsigned char)*s2);
        
        if (c1 != c2)
            return (c1 - c2);
            
        s1++;
        s2++;
    }
    return ((unsigned char)*s1 - (unsigned char)*s2);
}
