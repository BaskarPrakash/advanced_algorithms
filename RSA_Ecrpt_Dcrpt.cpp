/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   RSA_Ecrpt_Dcrpt.cpp
 * Author: baskarprakash
 *
 * Created on 18 February, 2020, 1:23 AM
 */

#include <cstdlib>
#include <cmath>
#include <string>

using namespace std;

double extended_euclid_gcd(double n1, double n2, double coeffs[]) {
    double rcoeffs[2] = {0.0, 0.0}, gcd;
    if (n1 == 0) {
        coeffs[0] = 0;
        coeffs[1] = 1;
        return n2;
    }
    printf("%lf,%lf,%lf,%lf\n", n1, n2, coeffs[0], coeffs[1]);
    gcd = extended_euclid_gcd(fmod(n2, n1), n1, rcoeffs);
    coeffs[0] = rcoeffs[1] - (n2 / n1) * (rcoeffs[0]);
    coeffs[1] = rcoeffs[0];
    return gcd;
}

double power(double num, double exp, double max) {
    double result = 1;
    while (exp > 0) {
        printf("BASE: %lf, EXP: %lf\n", num, exp);
        if (fmod(exp, 2) == 1) {
            result = fmod((result * num), max);
        }
        exp = ldexp(1, exp);
        num = fmod(num*num, max);
        printf("Fin BASE: %lf, EXP: %lf\n", num, exp);
    }
    return result;
}

void decimal_to_binary(double op1, int aOp[],int count) {
    int result, i = 0;
    do {
        result = fmod(op1, 2);
        op1 /= 2;
        aOp[i] = result;
        i++;
    } while (op1 > 0 & i<count);
}

double modular_exponentiation(double a, double b, double n) {
    
    double c = 0, d = 1;
    int count=0;
    // find out the size of binary b
    count = (int) (log(b) / log(2)) + 1;
    int bb[count],i;
    decimal_to_binary(a,bb,count);

    for (i = count - 1; i >= 0; i--) {
        c = 2 * c;
        d = fmod((d * d), n);
        if (bb[i] == 1) {
            c = c + 1;
            d = fmod((d * a), n);
        }
    }
    return d;
}

int gen_E(double o) {
    double bezCoeffs[2] = {0.0, 0.0};
    int e = 10000;
    while (extended_euclid_gcd(e, o, bezCoeffs) != 1) {
        e++;
    }
    return e;
}

/*
 * 
 */
int main(int argc, char** argv) {

    //2 random prime numbers
    double p = 10933766183632575817611517034730668287155799984632223454138745671121273456287670008290843302875521274970245314593222946129064538358581018615539828479146469.0;
    double q = 10910616967349110231723734078614922645337060882141748968209834225138976011179993394299810159736904468554021708289824396553412180514827996444845438176099727.0;
    double bezCoeffs[2] = {0.0, 0.0};
    double n = p*q;
    double count;
    double totient = (p - 1)*(q - 1);
    double e = gen_E(totient);
    bezCoeffs[0] = 0.0;
    bezCoeffs[1] = 0.0;
    double d = extended_euclid_gcd(e, totient, bezCoeffs);

    double msg = 12;
    printf("Message data = %lf", msg);
    printf("\np = %lf", p);
    printf("\nq = %lf", q);
    printf("\nn = pq = %lf", n);
    printf("\ntotient = %lf", totient);
    printf("\ne = %lf", e);
    printf("\nd = %lf", d);
    double c = modular_exponentiation(msg, e, n);
    double m = modular_exponentiation(c, d, n);


    printf("\nEncrypted data = %lf", c);
    printf("\nOriginal Message Sent = %lf", m);

    return 0;
}

