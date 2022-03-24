#include <stdio.h>
#include <math.h>

#include "./hashtable/hashtable.h"

void swap(int64_t *a, int64_t *b) {
    int64_t aux = *a;
    *a = *b;
    *b = aux;
}

// Find x and y such that ax + by = gcd(a, b)
int64_t eec(int64_t a, int64_t b, int64_t *x, int64_t *y) {
    int64_t q;
    int64_t r;
    int64_t x1 = 0;
    int64_t y1 = 1;
    int64_t aux;

    *x = 1;
    *y = 0;

    while (b) {
        q = a / b;
        r = a % b;
        a = b;
        b = r;

        aux = *x;
        *x = x1;
        x1 = aux - q * x1;
        aux = *y;
        *y = y1;
        y1 = aux - q * y1;
    }

    return a;
}

uint64_t mod_inv(uint64_t a, uint64_t n) {
    int64_t x, y;
    // x*n + y*a = 1
    eec(n, a, &x, &y);

    if (y < 0) {
        y += n;
    }

    return y;
}

uint64_t pow_mod(uint64_t a, uint64_t e, uint64_t n) {
    uint64_t res = 1;
    while (e) {
        if (e % 2 == 1) {
            res = res * a;
            if (res >= n) {
                res %= n;
            }
        }

        a *= a;
        if (a >= n) {
            a %= n;
        }

        e /= 2;
    }

    return res;
}

// Solve a^x = b (mod n)
uint64_t baby_giant(uint64_t a, uint64_t b, uint64_t n) {
    Hashtable *ht;
    uint64_t m = ceil(sqrtl(n));

    ht = init_ht(16);
    
    uint64_t f1 = pow_mod(a, m, n);
    uint64_t f2 = mod_inv(a, n);

    uint64_t acc = 1;
    for (uint64_t i = 0; i < m; i++) {
        put(ht, acc, i);
        acc = (acc * f1) % n;
    }

    acc = 1;
    for (uint64_t i = 0; i < m; i++) {
        uint64_t val = (acc * b) % n;
        uint64_t index;
        uint64_t has_key = get(ht, val, &index);
        acc = (acc * f2) % n;
        if (!has_key) {
            continue;
        }

        return m * index + i; 
    }

    return 0;
}

int main() {
    printf("%ld\n", baby_giant(3, 16, 23));

    return 0;
}
