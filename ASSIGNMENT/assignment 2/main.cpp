

#include <stdio.h>
/* Q1: compile: gcc -std=c11 -Wall -Wextra Q1_Precedence_DiscountBug.c -o q1 */
static double correct_bill(double price, int qty, int senior) {
    double gross = price * qty;
    double discount = senior ? gross * 0.15 : 0.0;
    double after_discount = gross - discount;
    double gst = after_discount * 0.05;
    return after_discount + gst;
}
int main(void) {
    double p[] = {100.0, 250.0, 99.99}; int q[] = {2,5,3}, s[] = {1,0,1};
    for (int i=0;i<3;i++) {
        double price=p[i]; int qty=q[i], senior=s[i];
        /* Original expression: * and / bind before + and -, left associative. */
        double total=price*qty-price*qty*15/100*senior+price*qty*5/100;
        double net=total>1000?total-50:total;
        double fixed=correct_bill(price,qty,senior), fixed_net=fixed>1000?fixed-50:fixed;
        printf("Bill %d original total %.2f net %.2f | corrected total %.2f net %.2f\\n",
               i+1,total,net,fixed,fixed_net);
    }
    puts("count = count++ + 1 has undefined behaviour: unsequenced modification/access.");
    puts("Use count++ alone. -Wall may warn about sequence-point issues; warnings vary by compiler.");
    return 0;
}



#include <stdio.h>
typedef struct { int senior, eligible, card; } Passenger;
static unsigned long calls;
static int slow_verify(int card) { ++calls; return card; }
static int order_a(Passenger p) { return (p.senior && p.eligible) || slow_verify(p.card); }
static int order_b(Passenger p) { return slow_verify(p.card) || (p.senior && p.eligible); }
static int nested(Passenger p) {
    if (p.senior && p.eligible) return 1;
    if (slow_verify(p.card)) return 1;
    return 0;
}
int main(void) {
    Passenger a[1000];
    for(int i=0;i<1000;i++){a[i].senior=i%5==0;a[i].eligible=i%2==0;a[i].card=i%10==0;}
    int grants=0; calls=0;
    for(int i=0;i<1000;i++) grants+=order_a(a[i]);
    printf("Cheap checks first: server calls=%lu grants=%d\\n",calls,grants);
    calls=0; grants=0;
    for(int i=0;i<1000;i++) grants+=order_b(a[i]);
    printf("Server check first: server calls=%lu grants=%d\\n",calls,grants);
    calls=0; grants=0;
    for(int i=0;i<1000;i++) grants+=nested(a[i]);
    printf("Nested if: server calls=%lu grants=%d\\n",calls,grants);
    calls=0; int x=0; if(x && slow_verify(1)){} printf("0 && verify calls=%lu\\n",calls);
    if(x & slow_verify(1)){} printf("0 & verify calls=%lu\\n",calls);
    /* Audit requirement: call verifier for every passenger, then combine values. */
    calls=0;
    for(int i=0;i<1000;i++){int audited=slow_verify(a[i].card); grants+=(a[i].senior&&a[i].eligible)||audited;}
    printf("Audit-everyone calls=%lu (one per passenger)\\n",calls);
    return 0;
}



#include <stdio.h>
enum { LIGHT=0, FAN=1, AC=2, HEATER=3, PUMP=4 };
unsigned char switch_on(unsigned char r,int d){return (unsigned char)(r | (1u<<d));}
unsigned char switch_off(unsigned char r,int d){return (unsigned char)(r & ~(1u<<d));}
unsigned char toggle(unsigned char r,int d){return (unsigned char)(r ^ (1u<<d));}
int is_on(unsigned char r,int d){return (r & (1u<<d)) != 0u;}
void show(unsigned char r){for(int b=7;b>=0;b--)putchar(((r>>b)&1u)?'1':'0');putchar('\\n');}
int main(void){
    unsigned char reg=0;
    reg=switch_on(reg,LIGHT); /* 1 */
    reg=switch_on(reg,AC);    /* 5 */
    show(reg);
    reg=toggle(reg,FAN);      /* 7 */
    reg=switch_off(reg,LIGHT);/* 6 */
    show(reg);
    printf("AC is %s, PUMP is %s\\n",is_on(reg,AC)?"ON":"OFF",is_on(reg,PUMP)?"ON":"OFF");
    return 0;
}



#include <stdio.h>
char grade_if(int m){
    if(m<0||m>100)return '?';
    if(m>=90)return 'S'; else if(m>=80)return 'A'; else if(m>=70)return 'B';
    else if(m>=60)return 'C'; else if(m>=50)return 'D'; else return 'F';
}
char grade_switch(int m){
    if(m<0||m>100)return '?';
    switch(m/10){
        case 10: case 9:return 'S'; case 8:return 'A'; case 7:return 'B';
        case 6:return 'C'; case 5:return 'D'; default:return 'F';
    }
}
char grade_table(int m){
    static const char g[11]={'F','F','F','F','F','D','C','B','A','S','S'};
    if(m<0||m>100)return '?'; return g[m/10];
}
const char *scholarship(char g,int income){
    if(g=='?'||income<0)return "Invalid input";
    if(income<=200000&&(g=='S'||g=='A'))return "Full scholarship";
    if(income<=400000&&(g=='S'||g=='A'||g=='B'))return "Partial scholarship";
    return "No scholarship";
}
int main(void){
    int a[]={100,90,89,85,84,80,79,75,74,70,69,60,59,50,49,0,-1,101};
    puts("marks if-ladder switch table");
    for(unsigned i=0;i<sizeof a/sizeof a[0];i++)
        printf("%5d %c %c %c\\n",a[i],grade_if(a[i]),grade_switch(a[i]),grade_table(a[i]));
    printf("Scholarship example: %s\\n",scholarship('A',180000));
    return 0;
}



#include <stdio.h>
unsigned long checks=0;
int is_prime(unsigned long n){
    checks=0; if(n<2)return 0;
    for(unsigned long i=2;i<=n/i;i++){ /* avoids overflow from i*i */
        ++checks; if(n%i==0)return 0;
    } return 1;
}
int baseline(unsigned long n,unsigned long *c){
    *c=0;if(n<2)return 0;
    for(unsigned long i=2;i<n;i++){++*c;if(n%i==0)return 0;}return 1;
}
int while_prime(unsigned long n,unsigned long *c){
    *c=0;if(n<2)return 0;unsigned long i=2;
    while(i<=n/i){++*c;if(n%i==0)return 0;++i;}return 1;
}
int do_prime(unsigned long n,unsigned long *c){
    *c=0;if(n<2)return 0;if(n==2||n==3)return 1;
    unsigned long i=2;
    do{if(i>n/i)break;++*c;if(n%i==0)return 0;++i;}while(i<=n/i);
    return 1;
}
int main(void){
    unsigned long a[]={2,3,97,100,7919,1000003};
    for(unsigned k=0;k<sizeof a/sizeof a[0];k++){
        unsigned long n=a[k],f=0,w=0,d=0,b=0;int p=is_prime(n);f=checks;
        while_prime(n,&w);do_prime(n,&d);baseline(n,&b);
        printf("%8lu %-9s for=%lu while=%lu do=%lu baseline=%lu\\n",n,p?"prime":"not prime",f,w,d,b);
    }return 0;
}



#include <stdio.h>
int main(void){
    double principal,rate_percent,payment;
    const int LIMIT=360; double balance,interest_total=0.0;int month=0;
    printf("Enter principal, monthly interest rate (percent), repayment: ");
    if(scanf("%lf %lf %lf",&principal,&rate_percent,&payment)!=3){puts("Invalid numeric input.");return 1;}
    if(principal<=0||rate_percent<0||payment<=0){puts("Invalid loan values.");return 1;}
    double r=rate_percent/100.0;
    if(payment<=principal*r){puts("Loan will not amortise: repayment must exceed monthly interest.");return 1;}
    balance=principal;
    puts("Month Opening Interest Payment Closing");
    while(balance>0.005){
        if(month>=LIMIT){puts("Safety stop: 360-month limit reached; revise terms.");return 2;}
        ++month;double opening=balance,interest=opening*r,due=opening+interest;
        double actual=payment<due?payment:due; /* final month pays only amount due */
        balance=due-actual;if(balance<0.005)balance=0.0;interest_total+=interest;
        printf("%5d %7.2f %8.2f %8.2f %8.2f\\n",month,opening,interest,actual,balance);
    }
    printf("Months=%d Total interest=Rs %.2f\\n",month,interest_total);
    return 0;
}



#include <stdio.h>
static unsigned long original_count,redesign_count;
static int original_prime(unsigned long n){
    original_count=0;if(n<2)return 0;
    for(unsigned long d=2;d<=n/d;d++){++original_count;if(n%d==0)return 0;}return 1;
}
static int redesigned_prime(unsigned long n){
    redesign_count=0;if(n<2)return 0;if(n==2)return 1;
    ++redesign_count;if(n%2==0)return 0;
    for(unsigned long d=3;d<=n/d;d+=2){++redesign_count;if(n%d==0)return 0;}return 1;
}
int main(void){
    unsigned long a[]={97,100,7919,1000003,1000000007};
    puts("number original-count redesigned-count result");
    for(unsigned i=0;i<sizeof a/sizeof a[0];i++){
        int x=original_prime(a[i]);unsigned long oc=original_count;
        int y=redesigned_prime(a[i]);unsigned long rc=redesign_count;
        printf("%10lu %13lu %16lu %s/%s\\n",a[i],oc,rc,x?"prime":"not-prime",y?"prime":"not-prime");
    }
    puts("Redesign checks divisor 2 once, then odd divisors only. Both O(sqrt(n)) time, O(1) extra space.");
    return 0;
}


