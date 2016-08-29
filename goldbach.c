// GoldBach for decomposing even integers to two odd primes
// Shanshan Li

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

typedef struct _seg{
    int bits[256];
    struct _seg *next,*prev;
}seg;

typedef struct pair{
    int first;
    int second;
}pair;

#define BITSPERSEG  (8*256*sizeof(int))

seg *head,*cur,*prev;

seg* whichseg(int j){
    // whichseg computes segment the bit that represents j locates in
    int n=(j-2)/BITSPERSEG;
    cur=head;
    int i;
    for(i=0;i<n;i++){
        cur=cur->next;
    }
    return cur;
}

int whichint(int j){
    //whichint computes the index of array cur->bits that the bit represents j locates in
    int i=(j-2)%BITSPERSEG;
    return i/32;
}

int whichbit(int j){
    //whichbit computes the specific bit in cur->bits[i] that represents j
    int i=(j-2)%BITSPERSEG;
    return i%32;
}

void buildBitMap(int N){
   int i,j; 
   for(i=2;i*i<=N;i++){
        cur=whichseg(2*i);
        int start=(2*i-2)/BITSPERSEG;
        for(j=2*i;j<=N;j+=i){
            if((j-2)/BITSPERSEG>start){
                cur=cur->next;
                start+=1;
            }
            int x=whichint(j);
            int y=whichbit(j);
            unsigned mask = 1 << y;
            cur->bits[x] |= mask;
        }
    }
}

int countOddPrimes(int N){
    int odd_primes=0;
    cur=head;
    int start=(3-2)/BITSPERSEG;
    int i;
    for(i=3;i<=N;i++){
        if((i-2)/BITSPERSEG>start){
            cur=cur->next;
            start+=1;
        }
        int x=whichint(i);
        int y=whichbit(i);
        unsigned mask = 1 << y;
        if((cur->bits[x]&mask)==0) odd_primes++;
        
    }
    return odd_primes;
}

pair decomposeToOddPrimes(int test){
    pair p;
    prev=whichseg(3);
    cur=whichseg(test-3);
    int start=(3-2)/BITSPERSEG;
    int end=(test-3-2)/BITSPERSEG;
    int i;
    for(i=3;i<=test/2;i+=2){
        if((i-2)/BITSPERSEG>start){
            prev=prev->next;
            start+=1;
        }
        if((test-i-2)/BITSPERSEG<end){
            cur=cur->prev;
            end-=1;
        }
        int x0=whichint(i);
        int y0=whichbit(i);
        int x1=whichint(test-i);
        int y1=whichbit(test-i);
        unsigned mask0 = 1 << y0,mask1 = 1<<y1;
        if((prev->bits[x0]&mask0)==0&&(cur->bits[x1]&mask1)==0){
            p.first=i;
            p.second=test-i;
        }
        
    }
    return p;
}

void main(int argc, const char * argv[]) {
    int N,test,count=0;
    if(argc==2) sscanf(argv[1],"%d",&N);
    else scanf("%d",&N);
    
    if(N<5||N%2==1){
        // cannot decompose 4 into two odd primes;
        printf("Enter Even Numbers >5 for Goldbach Tests\n");
        return;
    }
    
    printf("Calculating odd primes up to %d \n", N);
    
    
    // allocate segments to represent all numbers to N;
    head=(seg*)malloc(sizeof(seg));
    memset(head->bits,0,sizeof(head->bits)); // initialize all bits to zero;
    head->bits[0] |= 1; // mark on the bit that represent prime 2
    cur=head;
    int i;
    for (i=1;i<(N+BITSPERSEG-2)/BITSPERSEG;i++) {
        cur->next =(seg*)malloc(sizeof(seg));
        if(prev) cur->prev=prev; //construct a doubly linked list
        prev=cur;
        cur=cur->next;
        memset(cur->bits,0,sizeof(cur->bits)); // initialize all bits to zero;
    }
    cur->prev=prev;
    
    buildBitMap(N);
    
    printf ("Found %d odd primes \n", countOddPrimes(N));
    
    printf("Enter Even Numbers >5 for Goldbach Tests \n");
    
    while(!feof(stdin) && scanf("%d", &test)){
        if(test<5||test%2==1) continue;
        pair p;
        count++;
        p=decomposeToOddPrimes(test);
        printf("Largest %d = %d + %d out %d solutions\n",test,p.first,p.second,count);
    }
    return;
    
}
