#include "types.h"
#include "stat.h"
#include "user.h"

static void
putc(int fd, char c)
{
  write(fd, &c, 1);
}

static void
printint(int fd, int xx, int base, int sgn, int spaces)
{
  static char digits[] = "0123456789ABCDEF";
  char buf[16];
  int i, neg;
  uint x;

  neg = 0;
  if(sgn && xx < 0){
    neg = 1;
    x = -xx;
  } else {
    x = xx;
  }

  i = 0;
  int counter = 0;
  do{
    buf[i++] = digits[x % base];
    counter++;
  }while((x /= base) != 0);
  if(neg)
    buf[i++] = '-';
  spaces = spaces - counter; //For %nd
  while(--spaces>=0) //Print spaces
    putc(fd, ' ');
  while(--i >= 0)
    putc(fd, buf[i]);
}

// Print to the given fd. Only understands %d, %x, %p, %s.
void
printf(int fd, char *fmt, ...)
{
  char *s;
  int c, i, state;
  uint *ap;
  int sum = 0;
  state = 0;
  ap = (uint*)(void*)&fmt + 1;
  for(i = 0; fmt[i]; i++){
    c = fmt[i] & 0xff;
    if(state == 0){
      if(c == '%'){
        state = '%';
      } else {
        putc(fd, c);
      }
    } else if(state == '%'){
      if((c>= '0' && c<= '9')){
        sum=sum*10 + (int)c - 48;//For %nd
      } else if(c == 'd'){
        printint(fd, *ap, 10, 1, sum);
        ap++;
        sum = 0;
	state = 0;
      } else if(c == 'x' || c == 'p'){
        printint(fd, *ap, 16, 0, 0);
        ap++;
	state = 0;
      } else if(c == 's'){
        s = (char*)*ap;
        ap++;
        if(s == 0)
          s = "(null)";
        while(*s != 0){
          putc(fd, *s);
          s++;
        }
     	state = 0;
      } else if(c == 'c'){
        putc(fd, *ap);
        ap++;
     	state = 0;
      } else if(c == '%'){
        putc(fd, c);
        state = 0;
      } else {
        // Unknown % sequence.  Print it to draw attention.
        putc(fd, '%');
        putc(fd, c);
        state = 0;
      }
    }
  }
}
