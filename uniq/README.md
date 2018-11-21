Implemented uniq command in xv6- uniq is a Unix utility which, when fed a text file, outputs the file with adjacent identical lines collapsed to one.  If a filename is provided on the command line (i.e.,uniq FILE) then uniq should open it, read, filter out, print without repeated lines in this file, and then close it.  If no filename is provided, uniq should read from standard input. The traditional UNIX uniq utility can do lots of things, such as:

-c

:  count and group prefix lines by the number of occurrences

-d

:  only print duplicate lines

-i

:  ignore differences in case when comparing
```
Examples:
$ cat example.txt
No. 1
No. 2
No. 2
No. 2
No. 3
No. 4
No. 5
No. 6
No. 6
No. 2
no. 2

$ uniq example.txt
No. 1
No. 2
No. 3
No. 4
No. 5
No. 6
No. 2
no. 2

$ cat example.txt | uniq
No. 1
No. 2
No. 3
No. 4
No. 5
No. 6
No. 2
no. 2

$ uniq -c example.txt
1 No. 1
3 No. 2
1 No. 3
1 No. 4
1 No. 5
2 No. 6
1 No. 2
1 no. 2

$ uniq -d example.txt
No. 2
No. 6

$ uniq -i example.txt
No. 1
No. 2
No. 3
No. 4
No. 5
No. 6
No. 2

$ uniq -c -i example.txt
1 No. 1
3 No. 2
1 No. 3
1 No. 4
1 No. 5
2 No. 6
2 No. 2
```
