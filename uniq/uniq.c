#include "types.h"
#include "user.h"
#include "stat.h"
char buf[512];
int strcmpi(const char *p, const char *q)
{
  while(*p && (*p == *q || (*p+32) == *q || *p == (*q+32)))
    p++, q++;
  return (uchar)*p - (uchar)*q;
}
void uniq(int fd, char *name, int count, int caseSensitive) // When File name is given
{
  int i, n;
  char* lineData = (char *)malloc(512); //Stores Current line
  char* prev = (char *)malloc(512); //Store Prev Line
  int localCount = 1;//For option -c
  int j=0, totalLength=512;
  while((n = read(fd, buf, sizeof(buf))) > 0){
    for(i=0; i<n; i++){
      if(buf[i] == '\n'){
        lineData[j] = '\n';//Line has ended
        int compVal;
        if(caseSensitive == 0) //If caseSensitive
	  compVal = strcmp(lineData, prev);//Compare current and prev line
        else //If caseInsensitive
          compVal = strcmpi(lineData, prev);
        if(compVal!=0){
          if(count == 1 && prev[0]!='\0'){ // For option -c (Prev[0] condition for not allowing first time print)
            printf(1, "%4d ", localCount);
          }
          printf(1, "%s", prev);
          free(prev);
          prev = (char *)malloc(totalLength); // Reset Prev
          localCount = 1; // Reset Localcount
          prev = strcpy(prev, lineData);//Copy Prev
        }else{
          localCount++;//For option -c
        }
	j=0;//Reset current line pointer
        free(lineData);
        lineData = (char *)malloc(512);//Reset Current line
      } else if(j==totalLength){//Dynamic memory allocation
	char *temp = (char*)malloc(totalLength*2);
        int d;
        for (d = 0; d < totalLength; d++)
		temp[d] = lineData[d];
        free(lineData);
        lineData = temp;
	totalLength = totalLength*2;
	lineData[j] = buf[i];//Put Data from buffer into variable
        j++; //increment pointer
      } else {
        lineData[j] = buf[i];//Put Data from buffer into variable
        j++; //increment pointer
      }
    }
  }
  //Print last case start
  if(count == 1){
    printf(1, "%4d ", localCount);
  }
  printf(1, "%s", prev);
  //Print last case end
  if(n < 0){
    printf(1, "uniq: read error\n");
    exit();
  }
}
void uniqDuplicate(int fd, char *name, int caseSensitive) // Duplicate print
{
  int i, n;
  char* lineData = (char *)malloc(512); //Stores Current line
  char* prev = (char *)malloc(512); //Store Prev Line
  int j = 0;
  int localCount = 1;//For option -d
  while((n = read(fd, buf, sizeof(buf))) > 0){
    for(i=0; i<n; i++){
      if(buf[i] == '\n'){
        lineData[j] = '\n';//Line has ended
        int compVal;
        if(caseSensitive == 0) //If caseSensitive
	  compVal = strcmp(lineData, prev);//Compare current and prev line
        else //If caseInsensitive
          compVal = strcmpi(lineData, prev);
        if(compVal!=0){
          if(localCount != 1){ // For option -d
            printf(1, "%s", prev);
          }
          prev = (char *)malloc(512); // Reset Prev
          localCount = 1; // Reset Localcount
          prev = strcpy(prev, lineData);//Copy Prev
        }else{
          localCount++;//For option -c
        }
	j=0;//Reset current line pointer
        lineData = (char *)malloc(512);//Reset Current line
      } else{
        lineData[j] = buf[i];//Put Data from buffer into variable
        j++; //increment pointer
      }
    }
  }
  //Print last case start
  if(localCount != 1){
      printf(1, "%s", prev);
  }
  //Print last case end
  if(n < 0){
    printf(1, "uniq: read error\n");
    exit();
  }
}
void uniqInput()
{
  int i;
  char* lineData = (char *)malloc(512);
  char* prev = (char *)malloc(512);
  int j = 0;
  while(strcmp(gets(buf, sizeof(buf)), "\n")!=0){//Gets for standard input
    if(buf[0]=='\0')// For Pipe close
      break;
    for(i=0; i<strlen(buf); i++){
      if(buf[i] == '\n'){
        lineData[j] = '\n';
	int compVal = strcmp(lineData, prev);
        if(compVal!=0){
          printf(1, "%s", lineData);
          prev = (char *)malloc(512);
          prev = strcpy(prev, lineData);
        }
	j=0;
        lineData = (char *)malloc(512);
      } else{
        lineData[j] = buf[i];
        j++;
      }
    }
  }
}
int main(int argc, char *argv[])
{
  int fd, i;

  if(argc <= 1){
    uniqInput();
    exit();
  }
  int count = 0;
  int duplicate = 0;
  int caseSensitive = 0;
  for(i = 1; i < argc; i++){
    if(strcmp(argv[i],"-c")==0){
      count = 1;
      continue;
    }
    if(strcmp(argv[i],"-d")==0){
      duplicate = 1;
      continue;
    }
    if(strcmp(argv[i],"-i")==0){
      caseSensitive = 1;
      continue;
    }
    if((fd = open(argv[i], 0)) < 0){
      printf(1, "uniq: cannot open %s\n", argv[i]);
      exit();
    }
    if(duplicate == 1){
      uniqDuplicate(fd, argv[i], caseSensitive);
    } else{
      uniq(fd, argv[i], count, caseSensitive);
    }
    close(fd);
  }
  exit();
}
