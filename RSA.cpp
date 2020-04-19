
/* 
 * File:   RSA.cpp
 * Authors: Aravind Rao Hejamady            - CS19M502
 *          Baskar Prakash Srinivasa Murthy - CS19M503
 *          Janardan Jayaraman              - CS19M505
 * 
 * Description: RSA Simple code to generate prime numbers along with encryption 
 *              and decryption of RSA Algorithm.
 *              Using Inflnt.h library. 
 * 
 * Reference : https://github.com/sercantutar/infint
 * 
 * Created on 18 February, 2020, 1:23 AM
 */

#include "Inflnt.h"
#include<iostream>
namespace RSA {
    static InfInt random_number();
    static InfInt Generate_Random_Prime();
    static bool fermat_test(InfInt test_value);
    static bool miller_rabin_test(InfInt test_value);
    static InfInt euclidean_gcd(InfInt first_value, InfInt second_value);
    static InfInt extended_euclidean_inverse(InfInt value, InfInt mod);
    static InfInt power_mod(InfInt base, InfInt power, InfInt mod);
    
    // Adjust the Prime Number range to set the bit size of the encryption and decryption key.
    static const InfInt MAX_NUMBER = "999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999";
    static const InfInt MIN_NUMBER = "100000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000";

    struct RSA_Key {
        InfInt prime_1, prime_2;
        InfInt encrypt_power;
        InfInt decrypt_power;
        InfInt mod;
    };

    RSA_Key Generate_RSA_Key() {
        RSA_Key gen_key;
        
        gen_key.prime_1 = Generate_Random_Prime();
        std::cout  << std::endl << "p - Prime 1: " << gen_key.prime_1 << std::endl;
        
        gen_key.prime_2 = Generate_Random_Prime();
        std::cout  << std::endl << "q - Prime 2: " << gen_key.prime_2 << std::endl;

        gen_key.mod = gen_key.prime_1 * gen_key.prime_2;
        std::cout << "N - (p * q) : " << gen_key.mod << std::endl;
        
        InfInt tmp = random_number() % ((gen_key.prime_1 - 1)*(gen_key.prime_2 - 1) - MIN_NUMBER) + MIN_NUMBER;
        while (euclidean_gcd(tmp, (gen_key.prime_1 - 1)*(gen_key.prime_2 - 1)) != 1) {
            tmp = random_number() % ((gen_key.prime_1 - 1)*(gen_key.prime_2 - 1) - MIN_NUMBER) + MIN_NUMBER;
        }

        gen_key.encrypt_power = tmp;
        std::cout << "e - Encryption Key : " << gen_key.encrypt_power << std::endl;
        
        gen_key.decrypt_power = extended_euclidean_inverse(gen_key.encrypt_power, (gen_key.prime_1 - 1)*(gen_key.prime_2 - 1));
        std::cout << "d - Decryption Key : " << gen_key.decrypt_power << std::endl;
        
        return gen_key;
    }

    RSA_Key Generate_RSA_Key(RSA_Key& private_key) {
        RSA_Key gen_key;
        gen_key.encrypt_power = private_key.encrypt_power;
        gen_key.mod = private_key.mod;
        return gen_key;
    }

    InfInt RSA_Encrypt_Data(unsigned int data, RSA_Key& key) {
        return power_mod(InfInt(data), key.encrypt_power, key.mod);
    }


    InfInt RSA_Decrypt_Data(InfInt data, RSA_Key& key) {
        return power_mod(data, key.decrypt_power, key.mod);
    }

    static InfInt Generate_Random_Prime() {
        std::cout << "Running Primality Testing."  << std::flush;
        
        std::vector<InfInt> tested_numbers;
        tested_numbers.push_back(random_number() % (MAX_NUMBER - 3) + 3);
        
        while (tested_numbers[tested_numbers.size() - 1] % 2 == 0 ||
                fermat_test(tested_numbers[tested_numbers.size() - 1]) == false ||
                miller_rabin_test(tested_numbers[tested_numbers.size() - 1]) == false) {
            InfInt tmp;
            tmp = random_number() % (MAX_NUMBER - MIN_NUMBER) + MIN_NUMBER;
            while (std::find(tested_numbers.begin(), tested_numbers.end(), tmp) != tested_numbers.end()) {
                tmp = random_number() % (MAX_NUMBER - MIN_NUMBER) + MIN_NUMBER;
            }
            tested_numbers.push_back(tmp);
            std::cout << "."  << std::flush;
        }
        return tested_numbers[tested_numbers.size() - 1];
        std::cout << "\n"  << std::flush;
    }

    static InfInt random_number() {
        std::string random = "";
        unsigned int dig = rand();
        for (unsigned int i = 0; i < MIN_NUMBER.numberOfDigits() + (dig % (MAX_NUMBER - MIN_NUMBER).numberOfDigits()); i++) {
            random.push_back((rand() % 10) + '0');
        }
        InfInt num(random);
        num %= (MAX_NUMBER - MIN_NUMBER);
        return num;
    }

    static bool fermat_test(InfInt test_value) {
        if (power_mod(2, test_value - 1, test_value) != 1)
            return false;
        return true;
    }

    static bool miller_rabin_witness(InfInt test_value, InfInt witness) {
        InfInt q = test_value - 1;
        InfInt k = 0;
        while (q % 2 == 0) {
            k++;
            q = q / 2;
        }
        witness = power_mod(witness, q, test_value);
        if (witness == 1 || witness == test_value - 1) {
            return false; 
        }
        for (InfInt i = 0; i < k - 1; i++) {
            if (witness % test_value == witness - 1) {
                return false; 
            }
            witness = power_mod(witness, 2, test_value);
        }
        return true;
    }

    static bool miller_rabin_test(InfInt test_value) {
        std::vector<InfInt> tested_values;

        for (int i = 0; i < 10; i++) {
            if ((InfInt) tested_values.size() >= test_value - 3)
                break;

            InfInt tmp = random_number() % (test_value - 2) + 2;
            while (std::find(tested_values.begin(), tested_values.end(), tmp) != tested_values.end()) {
                tmp = random_number() % (test_value - 3) + 2;
            }
            tested_values.push_back(tmp);

            if (miller_rabin_witness(test_value, tested_values[tested_values.size() - 1]) == true) {
                return false; 
            }
        }
        return true; 
    }

    static InfInt euclidean_gcd(InfInt first_value, InfInt second_value) {
        InfInt divisor, dividend, remainder, quotient;
        if (first_value > second_value) {
            divisor = first_value;
            dividend = second_value;
        } else if (second_value > first_value) {
            divisor = second_value;
            dividend = first_value;
        } else {
            return first_value;
        }
        quotient = divisor / dividend;
        remainder = divisor % dividend;
        InfInt previous_remainder = remainder;
        while (remainder != 0) {
            previous_remainder = remainder;
            divisor = dividend;
            dividend = remainder;
            quotient = divisor / dividend;
            remainder = divisor % dividend;
        }
        return previous_remainder;
    }

    static InfInt extended_euclidean_inverse(InfInt value, InfInt mod) {
        InfInt u = 1;
        InfInt g = value;
        InfInt x = 0;
        InfInt y = mod;
        while (y != 0) {
            InfInt q = g / y;
            InfInt t = g % y;
            InfInt s = u - q * x;
            u = x;
            g = y;
            x = s;
            y = t;
        }
        if (u > 0) {
            return (InfInt) u;
        } else {
            while (u < 0) {
                u += mod / g;
            }
            return (InfInt) u;
        }
    }

    static InfInt power_mod(InfInt base, InfInt power, InfInt mod) {
        InfInt a = base;
        InfInt b = 1;
        while (power > 0) {
            if (power % 2 == 1) {
                b = (b * a) % mod;
            }
            a = (a * a) % mod;
            power = power / 2;
        }
        return b;
    }
};

int main() {
   
    RSA::RSA_Key key = RSA::Generate_RSA_Key();

    unsigned int data ;
    std::cout << "Enter Message as integer [0 to 4294967295] :";
    std::cin >> data;
    InfInt edata;
    edata = RSA::RSA_Encrypt_Data(data, key);
    std::cout << "Encrypted: " << edata << std::endl;
    edata = RSA::RSA_Decrypt_Data(edata, key);
    std::cout << "Decrypted: " << edata << std::endl;
    
    return 0;
}