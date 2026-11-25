

#include <stdio.h>
#include <string.h>

typedef struct { int day, month, year; } Date;
typedef struct {
    int donorId;
    char name[50], bloodGroup[4], phone[11];
    int age;
    Date lastDonation;
} Donor;
typedef struct { char bloodGroup[4]; int units; } Stock;
typedef struct { int eligibleCount; int youngestDonorId; } EligibilityResult;

int registerDonor(Donor *d, int id, const char *name, const char *group,
                  int age, const char *phone, Date last);
int isEligible(const Donor *d, Date today);
int findEligible(const Donor donors[], int n, const char *group, Date today,
                 int *youngestId);
EligibilityResult findEligibleResult(const Donor donors[], int n,
                                     const char *group, Date today);
Stock *findStock(Stock stock[], int n, const char *group);
int updateStock(Stock *s, int change);
int saveDonors(const char *filename, const Donor donors[], int n);
int loadDonors(const char *filename, Donor donors[], int capacity);

int registerDonor(Donor *d, int id, const char *name, const char *group,
                  int age, const char *phone, Date last)
{
    if (!d || !name || !group || !phone) return 0;
    d->donorId = id; d->age = age; d->lastDonation = last;
    snprintf(d->name, sizeof d->name, "%s", name);
    snprintf(d->bloodGroup, sizeof d->bloodGroup, "%s", group);
    snprintf(d->phone, sizeof d->phone, "%s", phone);
    return 1;
}

/* Simplified policy for demonstration: at least 90 days since last donation. */
int isEligible(const Donor *d, Date today)
{
    int oldDays = d->lastDonation.year * 365 + d->lastDonation.month * 30
                + d->lastDonation.day;
    int nowDays = today.year * 365 + today.month * 30 + today.day;
    return (nowDays - oldDays) >= 90;
}

int findEligible(const Donor donors[], int n, const char *group, Date today,
                 int *youngestId)
{
    int count = 0, youngestAge = 1000;
    if (youngestId) *youngestId = -1;
    for (int i = 0; i < n; i++) {
        if (strcmp(donors[i].bloodGroup, group) == 0 &&
            isEligible(&donors[i], today)) {
            count++;
            if (donors[i].age < youngestAge) {
                youngestAge = donors[i].age;
                if (youngestId) *youngestId = donors[i].donorId;
            }
        }
    }
    return count;
}

EligibilityResult findEligibleResult(const Donor donors[], int n,
                                     const char *group, Date today)
{
    EligibilityResult r;
    r.eligibleCount = findEligible(donors, n, group, today,
                                   &r.youngestDonorId);
    return r;
}

Stock *findStock(Stock stock[], int n, const char *group)
{
    for (int i = 0; i < n; i++)
        if (strcmp(stock[i].bloodGroup, group) == 0) return &stock[i];
    return NULL;
}

int updateStock(Stock *s, int change)
{
    if (!s || s->units + change < 0) return 0;
    s->units += change;
    return 1;
}

int saveDonors(const char *filename, const Donor donors[], int n)
{
    FILE *fp = fopen(filename, "w");
    if (!fp) { perror("fopen donor file"); return 0; }
    for (int i = 0; i < n; i++)
        fprintf(fp, "%d|%s|%s|%d|%s|%d/%d/%d\n",
                donors[i].donorId, donors[i].name, donors[i].bloodGroup,
                donors[i].age, donors[i].phone, donors[i].lastDonation.day,
                donors[i].lastDonation.month, donors[i].lastDonation.year);
    fclose(fp);
    return 1;
}

int loadDonors(const char *filename, Donor donors[], int capacity)
{
    FILE *fp = fopen(filename, "r");
    int n = 0;
    if (!fp) { perror("fopen donor file"); return -1; }
    while (n < capacity &&
           fscanf(fp, "%d|%49[^|]|%3[^|]|%d|%10[^|]|%d/%d/%d\n",
                  &donors[n].donorId, donors[n].name, donors[n].bloodGroup,
                  &donors[n].age, donors[n].phone, &donors[n].lastDonation.day,
                  &donors[n].lastDonation.month, &donors[n].lastDonation.year) == 8)
        n++;
    fclose(fp);
    return n;
}

int main(void)
{
    Donor donors[3];
    Stock stocks[2] = {{{"O+"}, 10}, {{"A-"}, 5}};
    Date today = {19, 11, 2026};
    registerDonor(&donors[0], 101, "Anu", "O+", 24, "9876543210", (Date){1,1,2026});
    registerDonor(&donors[1], 102, "Bala", "O+", 31, "9876543211", (Date){1,1,2026});
    registerDonor(&donors[2], 103, "Devi", "A-", 22, "9876543212", (Date){1,10,2026});
    EligibilityResult r = findEligibleResult(donors, 3, "O+", today);
    printf("Eligible O+ donors: %d; youngest donor ID: %d\n",
           r.eligibleCount, r.youngestDonorId);
    updateStock(findStock(stocks, 2, "O+"), -2);
    printf("O+ stock after issue: %d units\n", stocks[0].units);
    saveDonors("donors.txt", donors, 3);
    return 0;
}



#include <stdio.h>
#include <stdlib.h>

int digitalRoot(long long n)
{
    if (n < 0) n = -n;
    if (n < 10) return (int)n;
    int sum = 0;
    while (n > 0) { sum += (int)(n % 10); n /= 10; }
    return digitalRoot(sum);
}

long long powerLinear(long long base, int n)
{
    if (n == 0) return 1;
    return base * powerLinear(base, n - 1);
}

long long powerFast(long long base, int n)
{
    if (n == 0) return 1;
    long long half = powerFast(base, n / 2);
    long long result = half * half;
    return (n % 2 == 0) ? result : result * base;
}

long long powerIterative(long long base, int n)
{
    long long result = 1;
    while (n > 0) {
        if (n % 2) result *= base;
        base *= base;
        n /= 2;
    }
    return result;
}

int main(void)
{
    printf("Digital root of 98765 = %d\n", digitalRoot(98765));
    printf("2^10 linear = %lld\n", powerLinear(2, 10));
    printf("2^10 fast = %lld\n", powerFast(2, 10));
    printf("2^10 iterative = %lld\n", powerIterative(2, 10));
    printf("Digital root of 0 = %d\n", digitalRoot(0));
    printf("3^0 = %lld; 3^1 = %lld\n", powerFast(3, 0), powerFast(3, 1));
    return 0;
}



#include <stdio.h>
#include <string.h>

const char *groups[8] = {"A+", "A-", "B+", "B-", "AB+", "AB-", "O+", "O-"};

int *lowestStock(int stock[], int n)
{
    if (!stock || n <= 0) return NULL;
    int *lowest = &stock[0];
    for (int i = 1; i < n; i++)
        if (stock[i] < *lowest) lowest = &stock[i];
    return lowest;
}

int groupIndex(const char *group)
{
    for (int i = 0; i < 8; i++)
        if (strcmp(groups[i], group) == 0) return i;
    return -1;
}

int main(int argc, char *argv[])
{
    int stock[8] = {20, 5, 18, 7, 9, 3, 25, 6};
    int *p = stock;

    printf("p + 3 is address of stock[3]\n");
    printf("*(p + 3) = %d; p[3] = %d\n", *(p + 3), p[3]);
    printf("*p++ = %d; pointer then advances to next element\n", *p++);

    int *low = lowestStock(stock, 8);
    if (low) printf("Lowest stock = %d\n", *low);

    if (argc == 1) {
        printf("Usage: %s find GROUP | issue GROUP UNITS\n", argv[0]);
        return 1;
    }
    if (strcmp(argv[1], "find") == 0 && argc == 3) {
        int idx = groupIndex(argv[2]);
        if (idx < 0) { fprintf(stderr, "Invalid blood group\n"); return 1; }
        printf("%s has %d units\n", groups[idx], stock[idx]);
    } else if (strcmp(argv[1], "issue") == 0 && argc == 4) {
        int idx = groupIndex(argv[2]), units = 0;
        if (idx < 0 || sscanf(argv[3], "%d", &units) != 1 || units <= 0) {
            fprintf(stderr, "Invalid group or unit count\n"); return 1;
        }
        if (units > stock[idx]) { fprintf(stderr, "Insufficient stock\n"); return 1; }
        stock[idx] -= units;
        printf("Issued %d units of %s; remaining=%d\n", units, groups[idx], stock[idx]);
    } else {
        fprintf(stderr, "Invalid arguments. Use find GROUP or issue GROUP UNITS\n");
        return 1;
    }
    return 0;
}



#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int id, age;
    char name[50], group[4], phone[11];
} Donor;

typedef struct {
    Donor *data;
    size_t size, capacity;
} DonorList;

int addDonor(DonorList *list, Donor donor)
{
    if (list->size == list->capacity) {
        size_t newCapacity = (list->capacity == 0) ? 2 : list->capacity * 2;
        Donor *temp = realloc(list->data, newCapacity * sizeof *list->data);
        if (temp == NULL) {
            perror("realloc");
            return 0; /* old list->data remains valid */
        }
        list->data = temp;
        list->capacity = newCapacity;
    }
    list->data[list->size++] = donor;
    return 1;
}

int main(void)
{
    DonorList list = {NULL, 0, 0};
    for (int i = 0; i < 10; i++) {
        Donor d = {7000 + i, 20 + i, "SampleDonor", "O+", "9876543210"};
        if (!addDonor(&list, d)) {
            free(list.data);
            return 1;
        }
        printf("Added %d size=%zu capacity=%zu\n",
               d.id, list.size, list.capacity);
    }
    printf("Allocated bytes = %zu\n", list.capacity * sizeof(Donor));
    free(list.data);
    list.data = NULL;
    list.size = list.capacity = 0;
    return 0;
}



#include <stdio.h>
#include <string.h>

typedef struct { int groupCode; int units; } Stock;

int saveDonorsText(const char *filename)
{
    FILE *fp = fopen(filename, "w");
    if (!fp) { perror("open donors text"); return 0; }
    fprintf(fp, "1001|Anu|O+|24|9876543210|01/01/2026\n");
    fprintf(fp, "1002|Bala|A-|31|9876543211|02/02/2026\n");
    fclose(fp);
    return 1;
}

int saveStockBinary(const char *filename, const Stock stock[], size_t n)
{
    FILE *fp = fopen(filename, "wb");
    if (!fp) { perror("open stock binary"); return 0; }
    size_t written = fwrite(stock, sizeof *stock, n, fp);
    fclose(fp);
    return written == n;
}

int updateUnits(const char *filename, int index, int newUnits)
{
    FILE *fp = fopen(filename, "r+b");
    Stock s;
    if (!fp) { perror("open stock binary"); return 0; }
    if (fseek(fp, (long)index * (long)sizeof(Stock), SEEK_SET) != 0 ||
        fread(&s, sizeof s, 1, fp) != 1) {
        fclose(fp); return 0;
    }
    s.units = newUnits;
    if (fseek(fp, -(long)sizeof(Stock), SEEK_CUR) != 0 ||
        fwrite(&s, sizeof s, 1, fp) != 1) {
        fclose(fp); return 0;
    }
    fclose(fp);
    return 1;
}

int main(void)
{
    Stock stock[5] = {{1,40},{2,25},{3,12},{4,8},{5,30}};
    if (!saveDonorsText("donors.txt")) return 1;
    if (!saveStockBinary("stock.dat", stock, 5)) return 1;

    printf("Update record 3: %s\n", updateUnits("stock.dat", 3, 20) ? "done" : "failed");
    printf("Update record 9: %s\n", updateUnits("stock.dat", 9, 20) ? "done" : "failed");

    FILE *fp = fopen("stock.dat", "rb");
    if (!fp) { perror("read stock"); return 1; }
    Stock s;
    while (fread(&s, sizeof s, 1, fp) == 1)
        printf("%d:%d ", s.groupCode, s.units);
    printf("\n");
    fclose(fp);
    return 0;
}



#include <stdio.h>
long long calls;

long long power_linear(long long base, int exp)
{
    calls++;
    if (exp == 0) return 1;
    return base * power_linear(base, exp - 1);
}

long long power_fast(long long base, int exp)
{
    calls++;
    if (exp == 0) return 1;
    long long half = power_fast(base, exp / 2); /* compute only once */
    return (exp % 2 == 0) ? half * half : half * half * base;
}

long long power_iterative(long long base, int exp)
{
    long long result = 1;
    while (exp > 0) {
        if (exp % 2) result *= base;
        base *= base;
        exp /= 2;
    }
    return result;
}

int main(void)
{
    calls = 0;
    printf("linear: %lld in %lld calls\n", power_linear(2, 30), calls);
    /* Evaluation order in printf is unspecified; calculate before printing. */
    calls = 0;
    long long linear = power_linear(2, 30);
    printf("linear verified: %lld in %lld calls\n", linear, calls);

    calls = 0;
    long long fast = power_fast(2, 30);
    printf("fast: %lld in %lld calls\n", fast, calls);

    printf("iterative: %lld\n", power_iterative(2, 30));
    return 0;
}





#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(void)
{
    char fixedName[20];
    char line[100];

    printf("Enter a donor name (fgets safely reads up to 99 chars): ");
    if (!fgets(line, sizeof line, stdin)) {
        fprintf(stderr, "No input received\n");
        return 1;
    }
    line[strcspn(line, "\n")] = '\0';

    if (strlen(line) >= sizeof fixedName) {
        printf("Name is too long for fixedName[%zu]; rejecting safely.\n",
               sizeof fixedName);
        return 1;
    }
    strcpy(fixedName, line);
    printf("Accepted donor name: %s\n", fixedName);

    FILE *fp = fopen("missing_donor_file.txt", "r");
    if (!fp) perror("Could not open donor data file");

    printf("argc=%d\n", 0); /* CLI validation example is in A3 file. */
    return 0;
}



#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct { int group_code; int units; } Stock;

int update_units(FILE *fp, int index, int units)
{
    Stock s;
    if (index < 0) return 0;
    if (fseek(fp, (long)index * (long)sizeof(Stock), SEEK_SET) != 0)
        return 0;
    if (fread(&s, sizeof s, 1, fp) != 1) return 0;
    s.units = units;
    if (fseek(fp, -(long)sizeof(Stock), SEEK_CUR) != 0) return 0;
    if (fwrite(&s, sizeof s, 1, fp) != 1) return 0;
    fflush(fp);
    return 1;
}

int main(void)
{
    FILE *fp = tmpfile();
    if (!fp) { perror("tmpfile"); return 1; }

    Stock initial[5] = {{1,40},{2,25},{3,12},{4,8},{5,30}};
    if (fwrite(initial, sizeof(Stock), 5, fp) != 5) {
        perror("fwrite"); fclose(fp); return 1;
    }

    printf("update record 3: %s\n", update_units(fp, 3, 20) ? "done" : "failed");
    printf("update record 9: %s\n", update_units(fp, 9, 20) ? "done" : "failed");

    rewind(fp);
    Stock s;
    while (fread(&s, sizeof s, 1, fp) == 1)
        printf("%d:%d ", s.group_code, s.units);
    printf("\n");
    fclose(fp);
    return 0;
}



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct { int id; char group[4]; int age; } Donor;
typedef struct { Donor *data; size_t size, capacity; } DonorList;

int add_donor(DonorList *list, int id, const char *group, int age)
{
    if (list->size == list->capacity) {
        size_t newCapacity = list->capacity == 0 ? 2 : list->capacity * 2;
        Donor *p = realloc(list->data, newCapacity * sizeof *list->data);
        if (p == NULL) return 0;
        list->data = p;
        list->capacity = newCapacity;
    }
    list->data[list->size].id = id;
    snprintf(list->data[list->size].group, sizeof list->data[list->size].group,
             "%s", group);
    list->data[list->size].age = age;
    list->size++;
    return 1;
}

int main(void)
{
    DonorList list = {NULL, 0, 0};
    const char *groups[] = {"O+", "A+", "B-", "AB+", "O-"};
    clock_t start = clock();

    for (int i = 0; i < 5; i++) {
        if (!add_donor(&list, 7001 + i, groups[i], 20 + i)) {
            perror("realloc"); free(list.data); return 1;
        }
        printf("added %d (%s): size = %zu, capacity = %zu\n",
               list.data[i].id, list.data[i].group, list.size, list.capacity);
    }

    printf("Report: donors eligible for next-week review (demonstration list)\n");
    for (size_t i = 0; i < list.size; i++)
        if (list.data[i].age >= 18)
            printf("Donor ID %d, group %s\n", list.data[i].id, list.data[i].group);

    double elapsed = (double)(clock() - start) / CLOCKS_PER_SEC;
    printf("Allocated memory: %zu bytes\n", list.capacity * sizeof(Donor));
    printf("Elapsed CPU time: %.6f seconds\n", elapsed);

    free(list.data);
    list.data = NULL;
    list.size = list.capacity = 0;
    return 0;
}


