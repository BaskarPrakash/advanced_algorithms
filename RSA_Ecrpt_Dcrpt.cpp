
#include<iostream>
#include<math.h>
using namespace std;

class RSA {
    double n;
    double p;
    double q;
    double m;
    double phi;
    double e;
    double d;
public:
    void generatePandQ();
    bool isPrime(double &);
    void display();
    void generateEandD();
    double gcd(double, double);
    void extEuclidean(double, double, double &, double &);
    double encryption(double);
    double decryption(double);
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
    double start = 0, limit = 0;
    // lets generate a prime number between start and limit
    start = (1 << 12); //
    limit = (1 << 15); //
    double count = 0;
    for (double i = start + 1; i < limit; i++) {
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
    double start = phi / 2; // just take half of the phi value as a start value ..
    double limit = phi;
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
    double x = 0;
    double y = 0;
    extEuclidean(phi, e, x, y);
    // cout << ' x value is = ' << x << endl;
    // cout << ' y value is = ' << y << endl;
    while (y < 0) {
        y += phi;
    }
    d = y;
}

void RSA::extEuclidean(double a, double b, double &lastx, double &lasty) {
    if (b == 0) {
        lastx = 1;
        lasty = 0;
    } else {
        double quotient = a / b;
        double remainder = ceil(fmod(a , b));
        double s = 0;
        double t = 0;
        extEuclidean(b, remainder, s, t);
        lastx = t;
        lasty = s - quotient * t;
    }
}

double RSA::gcd(double a, double b) {
    double temp = 0;
    while (b != 0) {
        temp = a;
        a = b;
        b = ceil(fmod(temp , b));
    }
    return a;
}

bool RSA::isPrime(double &x) {
    if (ceil(fmod(x , 2)) == 0)
        return false;
    double lim = sqrt(x);
    for (double i = 3; i <= lim; i += 2)
        if (ceil(fmod(x , i)) == 0)
            return false;
    return true;
}

double RSA::encryption(double msg) {
    double i = e;
    double e_msg = 1;
    while (i) {
        i--;
        e_msg = ceil(fmod((e_msg * msg) , n));
    }
    return ceil(fmod(e_msg , n));
}

double RSA::decryption(double e_msg) {
    double i = d;
    double d_msg = 1;
    while (i) {
        i--;
        d_msg = ceil(fmod((d_msg * e_msg) , n));
    }
    return fmod(d_msg , n);
}

int main() {
    double message;
    RSA obj;
    obj.generatePandQ();
    obj.generateEandD();
    obj.display();
    cout << "Enter the Message : ";
    cin >> message;
    double e_msg = 0;
    e_msg = obj.encryption(message);
    cout << "Encrypted message is = " << e_msg << endl;
    double d_msg = 0;
    d_msg = obj.decryption(e_msg);
    cout << "Decrypted message is = " << d_msg << endl;
    return 0;
}