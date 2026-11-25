

/* Q3 - Merging Two Merit Lists
   Compile: gcc Q3_Merit_List_Merge.c -o q3
   Run: ./q3
*/
#include <stdio.h>
#include <stdlib.h>

long comparisons;
int merge(const int a[], int na, const int b[], int nb, int out[]) {
    int i=0,j=0,k=0;
    comparisons=0;
    while (i<na && j<nb) {
        comparisons++;
        /* >= keeps campus A first when scores are equal. */
        if (a[i] >= b[j]) out[k++]=a[i++];
        else out[k++]=b[j++];
    }
    while (i<na) out[k++]=a[i++];
    while (j<nb) out[k++]=b[j++];
    return k;
}
int main(void) {
    int campusA[]={98,91,85,72}, campusB[]={95,91,80}, merged[7];
    int n=merge(campusA,4,campusB,3,merged);
    for(int i=0;i<n;i++) printf("%d ",merged[i]);
    printf("\nComparisons = %ld\n",comparisons);

    /* Empty-list test */
    int empty_result[3], other[]={88,77,66};
    n=merge(NULL,0,other,3,empty_result);
    printf("One empty list: ");
    for(int i=0;i<n;i++) printf("%d ",empty_result[i]);
    putchar('\n');
    return 0;
}



/* Q4 - Two-Dimensional Arrays: District Rainfall Records
   Compile: gcc Q4_District_Rainfall.c -o q4
   Run: ./q4
*/
#include <stdio.h>

#define DISTRICTS 38
#define MONTHS 12

void row_totals(const double rain[][MONTHS], double totals[], int rows) {
    for(int i=0;i<rows;i++) {
        totals[i]=0;
        for(int j=0;j<MONTHS;j++) totals[i]+=rain[i][j];
    }
}
void monthly_averages(const double rain[][MONTHS], double avg[], int rows) {
    for(int j=0;j<MONTHS;j++) {
        double sum=0;
        for(int i=0;i<rows;i++) sum+=rain[i][j];
        avg[j]=sum/rows;
    }
}
void find_maximum(const double rain[][MONTHS], int rows,
                  double *maximum, int *district, int *month) {
    *maximum=rain[0][0]; *district=0; *month=0;
    for(int i=0;i<rows;i++) for(int j=0;j<MONTHS;j++)
        if(rain[i][j]>*maximum) {
            *maximum=rain[i][j]; *district=i; *month=j;
        }
}
double state_average(const double rain[][MONTHS], int rows) {
    double sum=0;
    for(int i=0;i<rows;i++) for(int j=0;j<MONTHS;j++) sum+=rain[i][j];
    return sum/(rows*MONTHS);
}
int main(void) {
    /* Demonstration data; replace/read these values for the actual 38x12 dataset. */
    double rain[DISTRICTS][MONTHS];
    for(int i=0;i<DISTRICTS;i++)
        for(int j=0;j<MONTHS;j++)
            rain[i][j]=(double)((i*17+j*23)%250);

    double totals[DISTRICTS], averages[MONTHS], maximum, overall;
    int wet_district, wet_month;
    row_totals(rain,totals,DISTRICTS);
    monthly_averages(rain,averages,DISTRICTS);
    find_maximum(rain,DISTRICTS,&maximum,&wet_district,&wet_month);
    overall=state_average(rain,DISTRICTS);

    printf("Annual totals (district 1 to 38):\n");
    for(int i=0;i<DISTRICTS;i++) printf("District %2d: %.1f mm\n",i+1,totals[i]);
    printf("Monthly averages: ");
    for(int j=0;j<MONTHS;j++) printf("%.1f ",averages[j]);
    printf("\nState average rainfall = %.2f mm\n",overall);
    printf("Wettest: district %d, month %d, rainfall %.1f mm\n",
           wet_district+1,wet_month+1,maximum);
    puts("Districts below 60% of state average annual rainfall:");
    for(int i=0;i<DISTRICTS;i++)
        if(totals[i] < 0.60 * overall * MONTHS)
            printf("%d ",i+1);
    putchar('\n');
    return 0;
}



/* Q5 - Processing Text Without the Library
   Compile: gcc Q5_SMS_String_Processing.c -o q5
   Run: ./q5
*/
#include <stdio.h>
#include <ctype.h>
#include <string.h> /* Used only in the comparison demonstration below. */

int word_count(const char s[]) {
    int count=0, inside=0;
    for(int i=0;s[i]!='\0';i++) {
        if(!isspace((unsigned char)s[i])) {
            if(!inside) count++;
            inside=1;
        } else inside=0;
    }
    return count;
}
int is_palindrome(const char s[]) {
    int left=0,right=0;
    while(s[right]!='\0') right++;
    right--;
    while(left<right) {
        while(left<right && !isalpha((unsigned char)s[left])) left++;
        while(left<right && !isalpha((unsigned char)s[right])) right--;
        if(tolower((unsigned char)s[left]) !=
           tolower((unsigned char)s[right])) return 0;
        left++; right--;
    }
    return 1;
}
void uppercase_manual(char s[]) {
    for(int i=0;s[i]!='\0';i++)
        if(s[i]>='a' && s[i]<='z') s[i]=(char)(s[i]-'a'+'A');
}
void reverse_manual(char s[]) {
    int n=0;
    while(s[n]!='\0') n++;
    for(int i=0;i<n/2;i++) {
        char t=s[i]; s[i]=s[n-1-i]; s[n-1-i]=t;
    }
}
int main(void) {
    printf("Words: %d\n",word_count(" Meet at the library at 5 PM "));
    printf("%d %d\n",is_palindrome("Madam, I'm Adam"),is_palindrome("Hello"));
    printf("Tab-separated words: %d\n",word_count("one\ttwo\tthree"));

    char message[100]="Good morning students";
    uppercase_manual(message);
    printf("Uppercase: %s\n",message);
    reverse_manual(message);
    printf("Reversed: %s\n",message);

    /* Safe input example: fgets prevents unbounded input. */
    char input[100];
    printf("Enter a short feedback message (optional): ");
    if(fgets(input,sizeof input,stdin)) {
        input[strcspn(input,"\n")]='\0';
        printf("Input has %d words\n",word_count(input));
    }
    return 0;
}



/* Q6 - A Directory of Names: Sorting and Searching Strings
   Compile: gcc Q6_Directory_Name_Search.c -o q6
   Run: ./q6
*/
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_NAMES 500
#define NAME_LEN 30

int comparisons=0;
int compare_counted(const char a[],const char b[]) {
    comparisons++;
    return strcmp(a,b);
}
void remove_newline(char s[]) {
    s[strcspn(s,"\n")]='\0';
}
void sort_names(char names[][NAME_LEN],int n) {
    for(int i=1;i<n;i++) {
        char key[NAME_LEN];
        strcpy(key,names[i]);
        int j=i-1;
        while(j>=0 && compare_counted(names[j],key)>0) {
            strcpy(names[j+1],names[j]);
            j--;
        }
        strcpy(names[j+1],key);
    }
}
int binary_search(char names[][NAME_LEN],int n,const char key[]) {
    int low=0,high=n-1;
    while(low<=high) {
        int mid=low+(high-low)/2;
        int cmp=compare_counted(names[mid],key);
        if(cmp==0) return mid;
        if(cmp<0) low=mid+1;
        else high=mid-1;
    }
    return -1;
}
int main(void) {
    char names[MAX_NAMES][NAME_LEN];
    int n;
    printf("Number of residents (1-%d): ",MAX_NAMES);
    if(scanf("%d",&n)!=1 || n<1 || n>MAX_NAMES) {
        puts("Invalid number."); return 1;
    }
    getchar();
    for(int i=0;i<n;i++) {
        printf("Name %d: ",i+1);
        if(!fgets(names[i],NAME_LEN,stdin)) return 1;
        remove_newline(names[i]);
    }
    comparisons=0;
    sort_names(names,n);
    printf("\nSorted names (strcmp calls during sort: %d):\n",comparisons);
    for(int i=0;i<n;i++) {
        printf("%s\n",names[i]);
        if(i>0 && strcmp(names[i],names[i-1])==0)
            printf("  DUPLICATE: %s\n",names[i]);
    }
    char key[NAME_LEN];
    printf("\nSearch name: ");
    if(!fgets(key,sizeof key,stdin)) return 1;
    remove_newline(key);
    comparisons=0;
    int pos=binary_search(names,n,key);
    printf("Search result index=%d, strcmp calls=%d\n",pos,comparisons);
    return 0;
}



/* Q7 - Student Council Election Counting
   Compile: gcc Q7_Election_Counting.c -o q7
   Run: ./q7
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_CANDIDATES 12
#define MAX_VOTES 500000

typedef struct {
    int candidate;
    int votes;
} Result;

int main(void) {
    int candidates,votes;
    int count[MAX_CANDIDATES+1]={0};
    int valid=0,invalid=0;
    printf("Enter number of candidates (1-%d): ",MAX_CANDIDATES);
    if(scanf("%d",&candidates)!=1 || candidates<1 || candidates>MAX_CANDIDATES) {
        puts("Invalid candidate count."); return 1;
    }
    printf("Enter number of votes (0-%d): ",MAX_VOTES);
    if(scanf("%d",&votes)!=1 || votes<0 || votes>MAX_VOTES) {
        puts("Invalid vote count."); return 1;
    }
    int *ballots=malloc((size_t)(votes?votes:1)*sizeof(int));
    if(!ballots) { perror("malloc"); return 1; }
    puts("Enter each vote as a candidate number:");
    for(int i=0;i<votes;i++) {
        if(scanf("%d",&ballots[i])!=1) {
            puts("Invalid input."); free(ballots); return 1;
        }
    }

    clock_t start=clock();
    for(int i=0;i<votes;i++) {
        int v=ballots[i];
        if(v>=1 && v<=candidates) { count[v]++; valid++; }
        else invalid++;
    }
    clock_t end=clock();

    Result result[MAX_CANDIDATES];
    for(int i=0;i<candidates;i++) {
        result[i].candidate=i+1;
        result[i].votes=count[i+1];
    }
    /* Sort candidates by votes descending; candidate number breaks ties. */
    for(int i=0;i<candidates-1;i++)
        for(int j=i+1;j<candidates;j++)
            if(result[j].votes>result[i].votes) {
                Result t=result[i]; result[i]=result[j]; result[j]=t;
            }

    printf("\nValid votes: %d\nInvalid votes: %d\n",valid,invalid);
    puts("Candidate results (descending votes):");
    for(int i=0;i<candidates;i++)
        printf("Candidate %d: %d votes\n",result[i].candidate,result[i].votes);

    if(result[0].votes==0) puts("No valid votes: no winner.");
    else {
        int tie=0;
        for(int i=1;i<candidates && result[i].votes==result[0].votes;i++) tie++;
        if(tie) {
            printf("Tie among %d candidates with %d votes.\n",tie+1,result[0].votes);
        } else printf("Winner: Candidate %d with %d votes.\n",
                      result[0].candidate,result[0].votes);
    }
    printf("Counting CPU time: %.6f seconds\n",
           (double)(end-start)/CLOCKS_PER_SEC);
    free(ballots);
    return 0;
}



/* Q1 - Searching a Library Catalogue
   Compile: gcc Q1_Library_Search.c -o q1
   Run: ./q1
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int steps;

int linear_search(const int a[], int n, int key) {
    steps = 0;
    for (int i = 0; i < n; i++) {
        steps++;
        if (a[i] == key) return i;
    }
    return -1;
}

/* Array must be sorted in ascending order. */
int binary_search(const int a[], int n, int key) {
    int low = 0, high = n - 1;
    steps = 0;
    while (low <= high) {
        int mid = low + (high - low) / 2;
        steps++;
        if (a[mid] == key) return mid;
        if (a[mid] < key) low = mid + 1;
        else high = mid - 1;
    }
    return -1;
}

int main(void) {
    int sorted[] = {1021,1048,1103,1190,1222,1305,1377,1402,1460,1533};
    int unsorted[] = {1222,1021,1460,1103,1533,1305,1048,1402,1190,1377};
    int keys[] = {1021,1305,1533,1400};
    int n = sizeof sorted / sizeof sorted[0];

    puts("LIBRARY CATALOGUE SEARCH");
    for (int k = 0; k < 4; k++) {
        int p = linear_search(unsorted, n, keys[k]);
        printf("Linear: book %d -> index %d, comparisons = %d\n", keys[k], p, steps);
        p = binary_search(sorted, n, keys[k]);
        printf("Binary: book %d -> index %d, comparisons = %d\n", keys[k], p, steps);
    }

    /* Demonstrate generated large sorted catalogue sizes. */
    int sizes[] = {1000, 10000, 100000};
    srand(1);
    for (int s = 0; s < 3; s++) {
        int count = sizes[s];
        int *a = malloc((size_t)count * sizeof(int));
        if (!a) { perror("malloc"); return 1; }
        for (int i = 0; i < count; i++) a[i] = 100000 + i * 3;
        int found = a[count / 2], missing = -1;
        binary_search(a, count, found);
        printf("n=%d binary successful comparisons=%d; ", count, steps);
        binary_search(a, count, missing);
        printf("unsuccessful comparisons=%d\n", steps);
        free(a);
    }
    return 0;
}



/* Q2 - Sorting the Examination Hall Seating Register
   Compile: gcc Q2_Seating_Sorts.c -o q2
   Run: ./q2
*/
#include <stdio.h>
#include <string.h>

typedef struct { long comparisons, swaps, shifts; } Stats;

void bubble_sort(int a[], int n, Stats *s) {
    s->comparisons = s->swaps = s->shifts = 0;
    for (int i = 0; i < n - 1; i++) {
        int changed = 0;
        for (int j = 0; j < n - 1 - i; j++) {
            s->comparisons++;
            if (a[j] > a[j + 1]) {
                int t = a[j]; a[j] = a[j + 1]; a[j + 1] = t;
                s->swaps++; changed = 1;
            }
        }
        if (!changed) break;
    }
}
void selection_sort(int a[], int n, Stats *s) {
    s->comparisons = s->swaps = s->shifts = 0;
    for (int i = 0; i < n - 1; i++) {
        int min = i;
        for (int j = i + 1; j < n; j++) {
            s->comparisons++;
            if (a[j] < a[min]) min = j;
        }
        if (min != i) {
            int t = a[i]; a[i] = a[min]; a[min] = t; s->swaps++;
        }
    }
}
void insertion_sort(int a[], int n, Stats *s) {
    s->comparisons = s->swaps = s->shifts = 0;
    for (int i = 1; i < n; i++) {
        int key = a[i], j = i - 1;
        while (j >= 0) {
            s->comparisons++;
            if (a[j] > key) { a[j + 1] = a[j]; s->shifts++; j--; }
            else break;
        }
        a[j + 1] = key;
    }
}
void print_array(const int a[], int n) {
    for (int i = 0; i < n; i++) printf("%d ", a[i]);
    putchar('\n');
}
int main(void) {
    int original[] = {42, 17, 31, 8, 25, 13};
    int n = sizeof original / sizeof original[0], a[6];
    Stats s;
    memcpy(a, original, sizeof a); bubble_sort(a, n, &s);
    printf("Bubble: "); print_array(a,n);
    printf("Comparisons=%ld swaps=%ld\n",s.comparisons,s.swaps);
    memcpy(a, original, sizeof a); selection_sort(a, n, &s);
    printf("Selection: "); print_array(a,n);
    printf("Comparisons=%ld swaps=%ld\n",s.comparisons,s.swaps);
    memcpy(a, original, sizeof a); insertion_sort(a, n, &s);
    printf("Insertion: "); print_array(a,n);
    printf("Comparisons=%ld shifts=%ld\n",s.comparisons,s.shifts);

    /* Input order experiment: sorted, nearly sorted, reverse, random. */
    int cases[4][6] = {
        {1,2,3,4,5,6}, {1,2,4,3,5,6},
        {6,5,4,3,2,1}, {4,1,6,2,5,3}
    };
    const char *labels[] = {"Sorted","Nearly sorted","Reverse","Random"};
    for (int c=0;c<4;c++) {
        memcpy(a,cases[c],sizeof a); insertion_sort(a,n,&s);
        printf("%s input - insertion comparisons=%ld shifts=%ld\n",
               labels[c],s.comparisons,s.shifts);
    }
    return 0;
}


