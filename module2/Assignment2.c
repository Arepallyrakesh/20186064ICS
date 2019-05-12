#include <stdio.h>
#include <stdlib.h>
 
#define YES 1
#define NO  0
 
int hexToInt(char s[]) {
    int hexdigit, i, inhex, n;    
    i=0;
     
    if(s[i] == '0') {
        ++i;
        if(s[i] == 'x' || s[i] == 'X'){            
            ++i;
        }
    }
     
    n = 0;
    inhex = YES;
    for(; inhex == YES; ++i) {
        if(s[i] >= '0' && s[i] <= '9') {            
            hexdigit = s[i] - '0';
        } else if(s[i] >= 'a' && s[i] <= 'f') {            
            hexdigit = s[i] - 'a' + 10;
        } else if(s[i] >= 'A' && s[i] <= 'F') {
            hexdigit = s[i] - 'A' + 10;
        } else {
            inhex = NO;
        }
         
        if(inhex == YES) {
            n = 16 * n + hexdigit;
        }
    }
     
    return n;
}
 
int main(int argc, char** argv) {
     

    char hex[4];
    int i=0;
    char c;
    while(i<5){
    	hex[i]=getchar();
    	i++;
    }
    int digit = hexToInt(hex);
    printf("The Integer is %d", digit);
     
    return 0;
}