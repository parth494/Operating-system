Created a shell to be able to 

1) Run any command of Unix. (Used exec system call to run a given command )

```
$ ls
README.md shell.c
```

2) I/O redirection - Programs will be expecting their input on standard input and will write to standard output

```
$ ls > a.txt
$ sort -r < a.txt
shell.c
README.md
```

3) Pipes - Ability to pipe the output of one command into the input of another.

```
$ cat /usr/share/dict/words | grep cat | sed s/cat/dog/ > doggerel.txt
```

PS: To run the shell:

```
$ gcc shell.c -o shell
$ ./shell
```
