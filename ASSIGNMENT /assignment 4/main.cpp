



#include <stdio.h>
#include <string.h>

typedef struct {
    int day, month, year;
} Date;

typedef struct {
    int systolic;
    int diastolic;
} BloodPressure;

typedef struct {
    char name[50];
    int age;
    char phone[11]; /* 10 digits + '\0' */
    BloodPressure bp;
    Date visitDate;
} Patient;

void addPatient(Patient *p, const char name[], int age, const char phone[],
                BloodPressure bp, Date visitDate)
{
    if (p == NULL) return;

    snprintf(p->name, sizeof p->name, "%s", name);
    p->age = age;
    snprintf(p->phone, sizeof p->phone, "%s", phone);
    p->bp = bp;
    p->visitDate = visitDate;
}

void sortByAge(Patient patients[], int count)
{
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (patients[j].age > patients[j + 1].age) {
                Patient temp = patients[j];
                patients[j] = patients[j + 1];
                patients[j + 1] = temp;
            }
        }
    }
}

void printPatient(const Patient *p)
{
    printf("%-12s Age:%d Phone:%s BP:%d/%d Visit:%02d/%02d/%04d\n",
           p->name, p->age, p->phone, p->bp.systolic, p->bp.diastolic,
           p->visitDate.day, p->visitDate.month, p->visitDate.year);
}

int main(void)
{
    Patient patients[100];
    int count = 0;

    BloodPressure bp1 = {120, 80};
    BloodPressure bp2 = {110, 70};

    addPatient(&patients[count++], "Anu", 25, "9876543210",
               bp1, (Date){12, 10, 2026});
    addPatient(&patients[count++], "Ravi", 40, "9123456780",
               bp2, (Date){13, 10, 2026});

    printf("Before sorting:\n");
    for (int i = 0; i < count; i++) printPatient(&patients[i]);

    sortByAge(patients, count);

    printf("\nAfter sorting by age:\n");
    for (int i = 0; i < count; i++) printPatient(&patients[i]);

    return 0;
}



#include <stdio.h>
#include <string.h>

typedef struct {
    int day, month, year;
} Date;

typedef struct {
    char name[30];
    char city[20];
    int pincode;
} Person;

typedef struct {
    int id;
    float weight;
    Date bookingDate;
    Person sender;
    Person receiver;
    char serviceType[10];
    float charge;
} Parcel;

void print_parcel(const Parcel *p)
{
    printf("%d %.1f kg %s-%d %02d/%02d/%04d\n",
           p->id, p->weight, p->receiver.city, p->receiver.pincode,
           p->bookingDate.day, p->bookingDate.month, p->bookingDate.year);
}

void sort_by_weight(Parcel p[], int n)
{
    for (int i = 0; i < n - 1; i++) {
        int min = i;
        for (int j = i + 1; j < n; j++) {
            if (p[j].weight < p[min].weight) min = j;
        }
        if (min != i) {
            Parcel temp = p[i];
            p[i] = p[min];
            p[min] = temp;
        }
    }
}

void list_by_receiver_city(const Parcel p[], int n, const char city[])
{
    for (int i = 0; i < n; i++) {
        if (strcmp(p[i].receiver.city, city) == 0)
            print_parcel(&p[i]);
    }
}

int main(void)
{
    Parcel list[3] = {
        {501, 4.5f, {12,10,2026}, {"Arun","Madurai",625001},
         {"Bala","Madurai",625001}, "Normal", 80.0f},
        {502, 1.2f, {12,10,2026}, {"Devi","Chennai",600077},
         {"Kavi","Chennai",600077}, "Express", 120.0f},
        {503, 2.8f, {13,10,2026}, {"Rani","Salem",636001},
         {"Maha","Coimbatore",641001}, "Normal", 95.0f}
    };

    printf("Sorted by weight:\n");
    sort_by_weight(list, 3);
    for (int i = 0; i < 3; i++) print_parcel(&list[i]);

    printf("\nParcels for receiver city Chennai:\n");
    list_by_receiver_city(list, 3, "Chennai");

    /* Access expressions for third parcel:
       (i) list[2].receiver.pincode
       (ii) ptr->receiver.pincode, where ptr = &list[2]
    */
    Parcel *ptr = &list[2];
    printf("\nThird parcel PIN using dot: %d\n", list[2].receiver.pincode);
    printf("Third parcel PIN using arrow: %d\n", ptr->receiver.pincode);

    printf("Bytes copied in one structure swap: %zu\n", sizeof(Parcel));
    return 0;
}



#include <stdio.h>

enum kind { TEMPERATURE, HUMIDITY, STATUS };

struct reading {
    enum kind type;
    union {
        float celsius;
        int percent;
        char status[8];
    } value;
};

struct all_values {
    float celsius;
    int percent;
    char status[8];
};

void show(const struct reading *r)
{
    switch (r->type) {
        case TEMPERATURE:
            printf("Temp %.1f C\n", r->value.celsius);
            break;
        case HUMIDITY:
            printf("Humidity %d %%\n", r->value.percent);
            break;
        case STATUS:
            printf("Status %s\n", r->value.status);
            break;
        default:
            printf("Unknown reading\n");
    }
}

int main(void)
{
    struct reading log[3] = {
        {TEMPERATURE, {.celsius = 31.5f}},
        {HUMIDITY, {.percent = 68}},
        {STATUS, {.status = "OK"}}
    };

    printf("sizeof union = %zu, sizeof struct reading = %zu\n",
           sizeof log[0].value, sizeof log[0]);
    printf("sizeof structure with all fields = %zu\n", sizeof(struct all_values));

    for (int i = 0; i < 3; i++) show(&log[i]);

    /* Misuse experiment: writing temperature, then reading humidity.
       This reads an inactive union member; its value is not portable/meaningful. */
    struct reading test = {TEMPERATURE, {.celsius = 31.5f}};
    printf("Misuse experiment (inactive member): humidity = %d\n",
           test.value.percent);

    return 0;
}



#include <stdio.h>

struct account {
    int number;
    float balance;
};

void addInterest(struct account *a, float rate)
{
    if (a == NULL) return;

    printf("Address inside addInterest: %p\n", (void *)a);
    a->balance = a->balance + a->balance * rate / 100.0f;
}

int main(void)
{
    struct account list[3] = {
        {101, 5000.0f}, {102, 12000.0f}, {103, 800.0f}
    };
    struct account *p = list;

    for (int i = 0; i < 3; i++) {
        printf("Address in main:          %p\n", (void *)&list[i]);
        addInterest(&list[i], 4.0f);
    }

    printf("\nUpdated account balances:\n");
    for (int i = 0; i < 3; i++)
        printf("%d %.2f\n", p[i].number, p[i].balance);

    /* Correct pointer member access: p->number and p->balance */
    printf("First account through arrow: %d %.2f\n",
           p->number, p->balance);

    return 0;
}



#include "global_counter.h"

/* Definition exists in exactly one source file. */
int shared_token = 1000;

int next_shared_token(void)
{
    return ++shared_token;
}



#include <stdio.h>
#include <string.h>
#include <time.h>

#define MAX_ITEMS 20
#define MAX_SALES 50
#define MAX_COMBO_PARTS 3

typedef struct {
    int day, month, year;
} Date;

typedef enum { PACKAGED, COOKED, COMBO } ItemKind;

typedef struct {
    Date expiry;
} PackagedInfo;

typedef struct {
    int hour, minute;
} PreparedTime;

typedef struct {
    int partIds[MAX_COMBO_PARTS];
    int quantities[MAX_COMBO_PARTS];
    int partCount;
} ComboInfo;

typedef struct {
    int id;
    char name[30];
    float price;
    int stock;
    ItemKind kind;
    union {
        PackagedInfo packaged;
        PreparedTime cooked;
        ComboInfo combo;
    } details;
} Item;

typedef struct {
    int itemId;
    int quantity;
    float revenue;
} Sale;

typedef struct {
    int itemId;
    char itemName[30];
    float revenue;
} RevenueRow;

int dateSerial(Date d)
{
    struct tm t = {0};
    t.tm_mday = d.day;
    t.tm_mon = d.month - 1;
    t.tm_year = d.year - 1900;
    t.tm_isdst = -1;
    return (int)(mktime(&t) / (24 * 60 * 60));
}

Item *findItem(Item items[], int n, int id)
{
    for (int i = 0; i < n; i++)
        if (items[i].id == id) return &items[i];
    return NULL;
}

int availableStock(Item items[], int n, Item *item)
{
    if (item->kind != COMBO) return item->stock;

    int available = 1000000;
    for (int i = 0; i < item->details.combo.partCount; i++) {
        Item *part = findItem(items, n, item->details.combo.partIds[i]);
        if (part == NULL) return 0;
        int possible = part->stock / item->details.combo.quantities[i];
        if (possible < available) available = possible;
    }
    return available;
}

int recordSale(Item items[], int n, Sale sales[], int *saleCount,
               int itemId, int quantity)
{
    Item *item = findItem(items, n, itemId);
    if (item == NULL || quantity <= 0) return 0;
    if (availableStock(items, n, item) < quantity) {
        printf("Sale rejected for %s: insufficient stock.\n", item->name);
        return 0;
    }

    if (item->kind == COMBO) {
        for (int i = 0; i < item->details.combo.partCount; i++) {
            Item *part = findItem(items, n, item->details.combo.partIds[i]);
            part->stock -= item->details.combo.quantities[i] * quantity;
        }
    } else {
        item->stock -= quantity;
    }

    if (*saleCount < MAX_SALES) {
        sales[*saleCount].itemId = itemId;
        sales[*saleCount].quantity = quantity;
        sales[*saleCount].revenue = item->price * quantity;
        (*saleCount)++;
    }
    printf("Sale recorded: %s x %d, Rs. %.2f\n",
           item->name, quantity, item->price * quantity);
    return 1;
}

void expiryWarnings(Item items[], int n, Date today)
{
    int todayNo = dateSerial(today);
    for (int i = 0; i < n; i++) {
        if (items[i].kind == PACKAGED) {
            int daysLeft = dateSerial(items[i].details.packaged.expiry) - todayNo;
            if (daysLeft >= 0 && daysLeft <= 2)
                printf("Expiry warning: %s expires in %d day(s).\n",
                       items[i].name, daysLeft);
        }
    }
}

void printRevenueSummary(Item items[], int n, Sale sales[], int saleCount)
{
    RevenueRow rows[MAX_ITEMS];
    int rowCount = 0;

    for (int i = 0; i < n; i++) {
        float total = 0.0f;
        for (int j = 0; j < saleCount; j++)
            if (sales[j].itemId == items[i].id)
                total += sales[j].revenue;
        if (total > 0) {
            rows[rowCount].itemId = items[i].id;
            strcpy(rows[rowCount].itemName, items[i].name);
            rows[rowCount].revenue = total;
            rowCount++;
        }
    }

    for (int i = 0; i < rowCount - 1; i++) {
        for (int j = 0; j < rowCount - i - 1; j++) {
            if (rows[j].revenue < rows[j + 1].revenue) {
                RevenueRow temp = rows[j];
                rows[j] = rows[j + 1];
                rows[j + 1] = temp;
            }
        }
    }

    printf("\nDay's revenue summary (high to low):\n");
    for (int i = 0; i < rowCount; i++)
        printf("%s : Rs. %.2f\n", rows[i].itemName, rows[i].revenue);
}

int main(void)
{
    Item items[MAX_ITEMS] = {
        {1, "Juice", 30.0f, 5, PACKAGED, {.packaged = {{21,11,2026}}}},
        {2, "Sandwich", 50.0f, 4, COOKED, {.cooked = {8,30}}},
        {3, "Combo Meal", 75.0f, 0, COMBO,
         {.combo = {{1,2}, {1,1}, 2}}}
    };
    Sale sales[MAX_SALES];
    int saleCount = 0;
    Date today = {19,11,2026};

    printf("Normal sale:\n");
    recordSale(items, 3, sales, &saleCount, 1, 2);

    printf("\nSale exceeding stock:\n");
    recordSale(items, 3, sales, &saleCount, 2, 10);

    printf("\nCombo sale:\n");
    recordSale(items, 3, sales, &saleCount, 3, 1);

    printf("\nExpiry check:\n");
    expiryWarnings(items, 3, today);

    printRevenueSummary(items, 3, sales, saleCount);

    printf("\nRemaining stock: Juice=%d, Sandwich=%d\n",
           items[0].stock, items[1].stock);
    return 0;
}
