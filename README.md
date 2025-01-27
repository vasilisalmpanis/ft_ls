# ft_ls
This project is about recoding the ls core utility to learn more about how to interact with the kernel.

* The project must be written in C.
* The executable file must be named ft_ls.
* You have to handle errors in a sensitive manner. In no way can your program quit  
  in an unexpected manner `Segmentation fault, bus error, double free, etc`. If you  
  are unsure, handle the errors like `ls`.
* The program cannot have memory leaks.

## Allowed Functions

### Libft authorized

```bash
◦ write
◦ opendir
◦ readdir
◦ closedir
◦ stat
◦ lstat
◦ getpwuid
◦ getgrgid
◦ listxattr
◦ getxattr
◦ time
◦ ctime
◦ readlink
◦ malloc
◦ free
◦ perror
◦ strerror
◦ exit

Bonus:
◦ tcgetattr
```

## Mandatory Part
1. Its behavior must be identical to the original ls command with the following variations:  
    * Amongst the numerous options available, we are asking you to create the  
      following: `-l, -R, -a, -r` and `-t`.
    * We strongly recommend that you account for the implications of the  
      option `-R` from the very beginning of your code...
    * You do not have to deal with the multiple column format for the exit when  
      the option `-l` isn’t in the arguments.
    * You are not required to deal with ACL and extended attributes.
    * The overall display, depending on each option, must stay as identical as possible to  
      the system command. We will be cordial when grading either the padding or the pagination,  
      but no information can be miss

```text
GNU argp will be used for option parsing
and display of options.
```
