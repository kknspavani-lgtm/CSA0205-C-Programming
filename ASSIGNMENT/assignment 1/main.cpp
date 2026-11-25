



/*
TASK 1 - Problem analysis and data identification
Compile: gcc -std=c11 -Wall -Wextra Task01_ProblemAnalysis.c -o task01
Run:     ./task01
*/
#include <stdio.h>

int main(void)
{
    /* Inputs: household ID, category, previous and current meter readings.
       Outputs: units consumed, slab energy charge, fixed charge, total bill.
       Tariff values are fixed for one run and should be defined centrally.
       Meter values are 0..65535. Paise is used for exact money arithmetic. */
    const long fixed_charge_paise = 5000L;
    const int slab1_limit = 100;
    const int slab2_limit = 200;
    const int rate1_paise = 0;
    const int rate2_paise = 225;
    const int rate3_paise = 450;

    unsigned int previous = 1200U, current = 1530U;
    unsigned int units = current - previous; /* normal non-rollover example */
    long energy_paise = 0L;

    if (units > (unsigned)slab1_limit)
        energy_paise += (long)(units - (unsigned)slab1_limit) *
                        (long)rate2_paise;
    if (units > (unsigned)slab2_limit)
        energy_paise += (long)(units - (unsigned)slab2_limit) *
                        (long)(rate3_paise - rate2_paise);

    /* For this illustrative calculation, add the first 100 at rate 1,
       the next 100 at rate 2, and units above 200 at rate 3. */
    energy_paise = (units < 100U ? (long)units * rate1_paise : 0L)
                 + (units > 100U ? (long)((units < 200U ? units : 200U) - 100U)
                                      * rate2_paise : 0L)
                 + (units > 200U ? (long)(units - 200U) * rate3_paise : 0L);

    printf("Household: H1001, category: Domestic\n");
    printf("Previous=%u Current=%u Units=%u\n", previous, current, units);
    printf("Energy charge=Rs %ld.%02ld\n",
           energy_paise / 100L, energy_paise % 100L);
    printf("Fixed charge=Rs %ld.%02ld\n",
           fixed_charge_paise / 100L, fixed_charge_paise % 100L);
    printf("Total=Rs %ld.%02ld\n",
           (fixed_charge_paise + energy_paise) / 100L,
           (fixed_charge_paise + energy_paise) % 100L);

    puts("\nText alternative: read digit strings, validate each character,");
    puts("convert d digits in O(d) time; text uses about d bytes plus terminator,");
    puts("whereas an unsigned 16-bit integer uses 2 bytes.");
    puts("Assumptions: one register rollover at most between readings;");
    puts("readings are valid; fixed charge applies to every household.");
    return 0;
}



/*
TASK 2 - Data types, limits and rollover
Compile: gcc -std=c11 -Wall -Wextra Task02_DataTypes.c -o task02
*/
#include <stdio.h>
#include <limits.h>
#include <float.h>

typedef unsigned short meter_t;

unsigned int consumption(meter_t previous, meter_t current)
{
    /* Conversion to unsigned short performs modulo USHRT_MAX+1 arithmetic. */
    return (unsigned short)(current - previous);
}

int main(void)
{
    printf("sizeof(char)=%zu int=%zu long=%zu long long=%zu\n",
           sizeof(char), sizeof(int), sizeof(long), sizeof(long long));
    printf("sizeof(meter_t)=%zu bytes, maximum reading=%u\n",
           sizeof(meter_t), (unsigned)USHRT_MAX);
    printf("INT_MAX=%d LONG_MAX=%ld LLONG_MAX=%lld\n",
           INT_MAX, LONG_MAX, LLONG_MAX);
    printf("FLT_DIG=%d DBL_DIG=%d\n", FLT_DIG, DBL_DIG);
    printf("Consumption: %u units\n", consumption(1200, 1530));
    printf("Consumption: %u units\n", consumption(65400, 120));

    /* Floating-point illustration: binary floating point cannot represent
       most decimal fractions exactly. Integer paise remains exact. */
    float total_float = 0.0f;
    long long total_paise = 0;
    for (int i = 0; i < 4000; ++i) {
        total_float += 162.50f;
        total_paise += 16250LL;
    }
    printf("4000 bills using float: Rs %.2f\n", total_float);
    printf("4000 bills using paise: Rs %lld.%02lld\n",
           total_paise / 100LL, total_paise % 100LL);
    return 0;
}



/*
TASK 3 - Central tariff constants
*/
#include <stdio.h>
#define SLAB1_UNITS 100
#define SLAB2_UNITS 200
#define RATE1_PAISE 0
#define RATE2_PAISE 225
#define RATE3_PAISE 450
#define FIXED_CHARGE_PAISE 5000L

long compute_bill(int units)
{
    long paise = FIXED_CHARGE_PAISE;
    if (units < 0) return 0L;
    if (units <= SLAB1_UNITS)
        paise += (long)units * RATE1_PAISE;
    else if (units <= SLAB2_UNITS)
        paise += (long)SLAB1_UNITS * RATE1_PAISE
               + (long)(units - SLAB1_UNITS) * RATE2_PAISE;
    else
        paise += (long)SLAB1_UNITS * RATE1_PAISE
               + (long)(SLAB2_UNITS - SLAB1_UNITS) * RATE2_PAISE
               + (long)(units - SLAB2_UNITS) * RATE3_PAISE;
    return paise;
}

int main(void)
{
    int tests[] = {0, 100, 150, 200, 350};
    for (size_t i = 0; i < sizeof tests / sizeof tests[0]; ++i) {
        long p = compute_bill(tests[i]);
        printf("%3d units -> Rs %ld.%02ld\n", tests[i], p / 100L, p % 100L);
    }
    return 0;
}



/*
TASK 4 - Safe macros
Preprocessor evidence: gcc -E Task04_SafeMacros.c
*/
#include <stdio.h>
#define RUPEES_TO_PAISE(r) ((long long)(r) * 100LL)
#define SQUARE(x) ((x) * (x))
#define MAX_VALUE(a, b) (((a) > (b)) ? (a) : (b))

static inline long long square_fn(long long x) { return x * x; }
static inline int max_fn(int a, int b) { return a > b ? a : b; }

int main(void)
{
    int a = 4, b = 7, i = 2;
    printf("SQUARE(a+1)=%d\n", SQUARE(a + 1));
    printf("MAX_VALUE(a,b)=%d\n", MAX_VALUE(a, b));
    printf("Rupees 50 -> %lld paise\n", RUPEES_TO_PAISE(50));
    /* Do not pass i++ to MAX_VALUE: a macro may evaluate an argument twice. */
    printf("inline square=%lld inline max=%d\n",
           square_fn(5), max_fn(a, b));
    printf("i before side-effect-safe use=%d\n", i);
    return 0;
}



/*
TASK 5 - Header file and conditional debugging
Compile normal: gcc -std=c11 -Wall -Wextra Task05_HeaderDebug.c -o task05
Compile debug:  gcc -std=c11 -Wall -Wextra -DDEBUG Task05_HeaderDebug.c -o task05
*/
#include <stdio.h>
#include "billing.h"

long long compute_bill(int units)
{
    if (units < 0) return 0LL;
    long long energy = 0LL;
    if (units > SLAB1_UNITS)
        energy += (long long)(units < SLAB2_UNITS ? units - SLAB1_UNITS :
                              SLAB2_UNITS - SLAB1_UNITS) * RATE2_PAISE;
    if (units > SLAB2_UNITS)
        energy += (long long)(units - SLAB2_UNITS) * RATE3_PAISE;
#ifdef DEBUG
    fprintf(stderr, "[DEBUG] units=%d energy_paise=%lld\n", units, energy);
#endif
    return FIXED_CHARGE_PAISE + energy;
}

int main(void)
{
    int units = 350;
    long long bill = compute_bill(units);
    printf("Bill for %d units = Rs %lld.%02lld\n",
           units, bill / 100LL, bill % 100LL);
    return 0;
}



/*
TASK 6 - scanf validation and formatted output
*/
#include <stdio.h>

int main(void)
{
    unsigned int previous, current;
    unsigned int units;
    long long bill_paise;

    printf("Enter previous and current meter readings (0..65535): ");
    if (scanf("%u %u", &previous, &current) != 2) {
        fprintf(stderr, "Invalid input: enter two whole-number readings.\n");
        return 1;
    }
    if (previous > 65535U || current > 65535U) {
        fprintf(stderr, "Reading outside 16-bit meter range.\n");
        return 1;
    }

    units = (current - previous) & 65535U;
    bill_paise = 5000LL;
    if (units > 100U)
        bill_paise += (long long)(units < 200U ? units - 100U : 100U) * 225LL;
    if (units > 200U)
        bill_paise += (long long)(units - 200U) * 450LL;

    printf("Units consumed: %u\n", units);
    printf("Bill: Rs %lld.%02lld\n",
           bill_paise / 100LL, bill_paise % 100LL);
    return 0;
}



/*
TASK 7 - Type conversion and precision demonstrations
*/
#include <stdio.h>
#include <limits.h>

int main(void)
{
    printf("Surprise 1: 5/2 = %d (integer division discards .5)\n", 5 / 2);
    printf("Fix: 5.0/2 = %.1f\n", 5.0 / 2);
    printf("Surprise 2: (double)(5/2) = %.1f\n", (double)(5 / 2));
    printf("Fix: (double)5/2 = %.1f\n", (double)5 / 2);
    printf("Surprise 3: (int)3.99 = %d (fraction truncated)\n", (int)3.99);
    printf("Rounding to nearest paise: %.0f paise\n", 225.5);

    long large = (long)INT_MAX + 100L;
    printf("A long value can exceed INT_MAX: %ld; casting to int is unsafe.\n",
           large);
    puts("Use long long for accumulated paise; round fractional tariff charges");
    puts("once per charge using a documented rule, then store integer paise.");
    return 0;
}



/*
TASK 8 - Compilation stages
Commands:
gcc -E Task08_CompilationStages.c -o Task08_preprocessed.i
gcc -S Task08_CompilationStages.c -o Task08_assembly.s
gcc -c Task08_CompilationStages.c -o Task08_object.o
gcc Task08_object.o -o task08
gcc -Wall -Wextra Task08_CompilationStages.c -o task08
*/
#include <stdio.h>
#define FIXED_PAISE 5000LL

static long long add_fixed_charge(long long energy)
{
    return energy + FIXED_PAISE;
}

int main(void)
{
    printf("Total paise=%lld\n", add_fixed_charge(25313LL));
    return 0;
}



/*
TASK 9 - Complete billing program with validation and tests
Input readings are 16-bit meter values. One rollover is handled modulo 65536.
*/
#include <stdio.h>
#include <limits.h>

#define METER_MODULUS 65536U
#define FIXED_CHARGE_PAISE 5000LL
#define SLAB1_UNITS 100U
#define SLAB2_UNITS 200U
#define RATE1_PAISE 0LL
#define RATE2_PAISE 225LL
#define RATE3_PAISE 450LL

static unsigned int units_used(unsigned int previous, unsigned int current)
{
    return (current - previous) % METER_MODULUS;
}

static long long bill_paise(unsigned int units)
{
    long long energy = 0LL;
    if (units <= SLAB1_UNITS)
        energy = (long long)units * RATE1_PAISE;
    else if (units <= SLAB2_UNITS)
        energy = (long long)SLAB1_UNITS * RATE1_PAISE
               + (long long)(units - SLAB1_UNITS) * RATE2_PAISE;
    else
        energy = (long long)SLAB1_UNITS * RATE1_PAISE
               + (long long)(SLAB2_UNITS - SLAB1_UNITS) * RATE2_PAISE
               + (long long)(units - SLAB2_UNITS) * RATE3_PAISE;
    return FIXED_CHARGE_PAISE + energy;
}

static void print_bill(unsigned int prev, unsigned int curr)
{
    unsigned int units = units_used(prev, curr);
    long long amount = bill_paise(units);
    printf("%5u -> %5u | %5u units | Rs %lld.%02lld\n",
           prev, curr, units, amount / 100LL, amount % 100LL);
}

int main(void)
{
    puts("Normal and boundary tests:");
    print_bill(1000, 1000);       /* zero */
    print_bill(1000, 1100);       /* 100 */
    print_bill(1000, 1200);       /* 200 */
    print_bill(1000, 1350);       /* 350 */
    print_bill(65400, 120);       /* rollover: 256 */

    puts("\nInteractive validation test:");
    unsigned int previous, current;
    printf("Enter previous and current readings (0..65535): ");
    if (scanf("%u %u", &previous, &current) != 2) {
        puts("ERROR: non-numeric input.");
        return 1;
    }
    if (previous > USHRT_MAX || current > USHRT_MAX) {
        puts("ERROR: reading is outside the meter range.");
        return 1;
    }
    /* A lower current value is treated as rollover. If rollover is not
       physically plausible in the billing interval, flag it for review. */
    if (current < previous)
        puts("NOTICE: rollover detected; verify the interval and meter log.");
    print_bill(previous, current);

    puts("\nFive-household hand-check example:");
    unsigned int sample_units[] = {0, 100, 150, 200, 350};
    long long sum = 0LL;
    for (size_t i = 0; i < sizeof sample_units / sizeof sample_units[0]; ++i) {
        long long b = bill_paise(sample_units[i]);
        sum += b;
        printf("%u units: Rs %lld.%02lld\n", sample_units[i],
               b / 100LL, b % 100LL);
    }
    printf("Five-bill total: Rs %lld.%02lld\n", sum / 100LL, sum % 100LL);
    return 0;
}



/*
TASK 10 - Scale and accumulator comparison
The example uses a constant sample bill of 16250 paise.
*/
#include <stdio.h>
#include <limits.h>
#include <float.h>

int main(void)
{
    const long long one_bill_paise = 16250LL;
    const int counts[] = {4000, 60000, 800000};
    printf("LONG_MAX=%ld LLONG_MAX=%lld DBL_DIG=%d\n",
           LONG_MAX, LLONG_MAX, DBL_DIG);

    for (size_t j = 0; j < sizeof counts / sizeof counts[0]; ++j) {
        long long exact_paise = 0LL;
        double double_rupees = 0.0;
        float float_rupees = 0.0f;
        for (int i = 0; i < counts[j]; ++i) {
            exact_paise += one_bill_paise;
            double_rupees += 162.50;
            float_rupees += 162.50f;
        }
        printf("%d bills: paise total=%lld; double=%.2f; float=%.2f\n",
               counts[j], exact_paise, double_rupees, (double)float_rupees);
    }
    puts("Check overflow using count * maximum_bill_paise against LLONG_MAX.");
    return 0;
}



/*
TASK 11 - Recommended original monthly billing design
*/
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

#define FIXED_CHARGE_PAISE INT64_C(5000)
#define SLAB1 100
#define SLAB2 200
#define RATE2 225
#define RATE3 450

static int64_t calculate_bill(uint32_t units)
{
    int64_t energy = 0;
    if (units > SLAB1)
        energy += (int64_t)(units < SLAB2 ? units - SLAB1 : SLAB2 - SLAB1) * RATE2;
    if (units > SLAB2)
        energy += (int64_t)(units - SLAB2) * RATE3;
    return FIXED_CHARGE_PAISE + energy;
}

int main(void)
{
    uint32_t units = 350U;
    int64_t amount = calculate_bill(units);
    printf("Recommended design: units=%" PRIu32 ", bill=Rs %" PRId64 ".%02" PRId64 "\n",
           units, amount / 100, amount % 100);
    puts("Use fixed-width unsigned meter readings and signed 64-bit paise totals.");
    puts("Keep tariff definitions in one header; revise centrally and retest.");
    puts("For frequent revisions, load validated tariff configuration at startup.");
    return 0;
}



/*
TASK 12 - Smart-meter redesign: watt-hours, 15-minute intervals,
time-of-day peak rate (+20%), and run-time tariff configuration.

Compile: gcc -std=c11 -Wall -Wextra Task12_SmartMeterTOU.c -o task12
Run:     ./task12 [optional tariff.txt]

Example tariff.txt (paise per kWh):
base_rate_paise_per_kwh=225
fixed_charge_paise=5000
*/
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <string.h>

typedef struct {
    int64_t base_rate_paise_per_kwh;
    int64_t fixed_charge_paise;
} Tariff;

static int load_tariff(const char *path, Tariff *t)
{
    FILE *fp = fopen(path, "r");
    if (!fp) return 0;
    char key[80];
    long long value;
    int got_rate = 0, got_fixed = 0;

    while (fscanf(fp, " %79[^=]=%lld", key, &value) == 2) {
        if (strcmp(key, "base_rate_paise_per_kwh") == 0 && value >= 0) {
            t->base_rate_paise_per_kwh = (int64_t)value;
            got_rate = 1;
        } else if (strcmp(key, "fixed_charge_paise") == 0 && value >= 0) {
            t->fixed_charge_paise = (int64_t)value;
            got_fixed = 1;
        }
    }
    fclose(fp);
    return got_rate && got_fixed;
}

/* watt_hours is interval energy. Peak interval (18:00 through 21:59)
   receives 20% surcharge. Round to nearest paise per interval. */
static int64_t interval_charge(int64_t watt_hours, int hour, const Tariff *t)
{
    int64_t rate = t->base_rate_paise_per_kwh;
    if (hour >= 18 && hour < 22)
        rate = (rate * 120 + 50) / 100; /* nearest whole paise */
    /* paise/kWh * Wh / 1000; nearest paise */
    return (watt_hours * rate + 500) / 1000;
}

int main(int argc, char **argv)
{
    Tariff tariff = {225, 5000};
    if (argc > 1 && !load_tariff(argv[1], &tariff)) {
        fprintf(stderr, "Could not load valid tariff file: %s\n", argv[1]);
        return 1;
    }

    /* One house: 35,040 intervals/year. Demonstration values only.
       1 kWh = 1000 Wh; use int64_t for yearly energy and money. */
    int64_t yearly_watt_hours = 0;
    int64_t yearly_energy_paise = 0;
    for (int interval = 0; interval < 35040; ++interval) {
        int hour = (interval % 96) / 4; /* 96 quarter-hours per day */
        int64_t wh = 250;               /* sample 0.25 kWh interval */
        yearly_watt_hours += wh;
        yearly_energy_paise += interval_charge(wh, hour, &tariff);
    }

    int64_t total = yearly_energy_paise + tariff.fixed_charge_paise;
    printf("Yearly energy: %" PRId64 " Wh (%.2f kWh)\n",
           yearly_watt_hours, (double)yearly_watt_hours / 1000.0);
    printf("Energy charge: Rs %" PRId64 ".%02" PRId64 "\n",
           yearly_energy_paise / 100, yearly_energy_paise % 100);
    printf("Total incl. fixed charge: Rs %" PRId64 ".%02" PRId64 "\n",
           total / 100, total % 100);
    puts("Design decision: MODIFY/REDESIGN using 64-bit Wh and paise,");
    puts("runtime tariff configuration, and explicit time-of-day classification.");
    return 0;
}
