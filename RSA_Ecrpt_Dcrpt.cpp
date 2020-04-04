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
#include<iostream>
#include<math.h>
using namespace std;

class RSA {
    long long int n;
    long long int p;
    long long int q;
    long long int m;
    long long int phi;
    long long int e;
    long long int d;
public:
    void generatePandQ();
    bool isPrime(long long int &);
    void display();
    void generateEandD();
    long long int gcd(long long int, long long int);
    void extEuclidean(long long int, long long int, long long int &, long long int &);
    long long int encryption(long long int);
    long long int decryption(long long int);
};

void RSA::display() {
    cout << "n =\t" << n << endl;
    cout << "p =\t" << p << endl;
    cout << "q =\t" << q << endl;
    cout << "phi =\t" << phi << endl;
    cout << "e =\t" << e << endl;
    cout << "d =\t" << d << endl;
}

void RSA::generatePandQ() {
    long long int start = 0, limit = 0;
    // lets generate a prime number between start and limit
    start = (1 << 12); //
    limit = (1 << 15); //
    long long int count = 0;
    for (long long int i = start + 1; i < limit; i++) {
        if (isPrime(i)) {
            count++;
            if (count == 1)
                p = i;
            else {
                q = i;
                break;
            }
        }
    }
    n = p * q;
    phi = (p - 1)*(q - 1);
}

void RSA::generateEandD() {
    long long int start = phi >> 5; // just take half of the phi value as a start value ..
    long long int limit = phi;
    // cout << 'start is ' << start << endl;
    // cout << 'limit is ' << limit << endl;
    for (; start < limit; start++) {
        if (gcd(phi, start) == 1) {
            //cout << ' gcd 1 foundn';
            e = start;
            break;
        }
    }
    // cout << 'e is = ' << e << endl;
    long long int x = 0;
    long long int y = 0;
    extEuclidean(phi, e, x, y);
    // cout << ' x value is = ' << x << endl;
    // cout << ' y value is = ' << y << endl;
    while (y < 0) {
        y += phi;
    }
    d = y;
}

void RSA::extEuclidean(long long int a, long long int b, long long int &lastx, long long int &lasty) {
    if (b == 0) {
        lastx = 1;
        lasty = 0;
    } else {
        long long int quotient = a / b;
        long long int remainder = a % b;
        long long int s = 0;
        long long int t = 0;
        extEuclidean(b, remainder, s, t);
        lastx = t;
        lasty = s - quotient * t;
    }
}

long long int RSA::gcd(long long int a, long long int b) {
    long long int temp = 0;
    while (b != 0) {
        temp = a;
        a = b;
        b = temp % b;
    }
    return a;
}

bool RSA::isPrime(long long int &x) {
    if (x % 2 == 0)
        return false;
    long long int lim = sqrt(x);
    for (long long int i = 3; i <= lim; i += 2)
        if (x % i == 0)
            return false;
    return true;
}

long long int RSA::encryption(long long int msg) {
    long long int i = e;
    long long int e_msg = 1;
    while (i) {
        i--;
        e_msg = (e_msg * msg) % n;
    }
    return e_msg % n;
}

long long int RSA::decryption(long long int e_msg) {
    long long int i = d;
    long long int d_msg = 1;
    while (i) {
        i--;
        d_msg = (d_msg * e_msg) % n;
    }
    return d_msg % n;
}

int main() {
    long long int message;
    RSA obj;
    obj.generatePandQ();
    obj.generateEandD();
    obj.display();
    cout << "Enter the Message : ";
    cin >> message;
    long long int e_msg = 0;
    e_msg = obj.encryption(message);
    cout << "Encrypted message is = " << e_msg << endl;
    long long int d_msg = 0;
    d_msg = obj.decryption(e_msg);
    cout << "Decrypted message is = " << d_msg << endl;
    return 0;
}