#include "qrsaencryption.h"
//#include "BigInt.hpp"

template<class INT>
INT eulerFunc(const INT &p, const INT &q) {
    return (p - 1) * (q - 1);
}

template<class INT>
INT mul(const INT &a, const INT &b, const INT &m){
    if(b == 1)
        return a;
    if(b % 2 == 0){
        INT t = mul(a, b / 2, m);
        return (2 * t) % m;
    }
    return (mul(a, b - 1, m) + a) % m;
}

template<class INT>
INT pows(const INT &a, const INT &b, const INT &m){
    if(b == 0)
        return 1;
    if(b % 2 == 0){
        INT t = pows(a, b / 2, m);
        return mul(t , t, m) % m;
    }
    return ( mul(pows(a, b - 1, m) , a, m)) % m;
}

template<class INT>
bool gcd(INT a, INT b) {
    INT c;
    while ( a != 0 ) {
      c = a;
      a = b % a;
      b = c;
    }
    return b;
}

template<class INT>
INT randNumber() {
    srand(static_cast<unsigned int>(time(nullptr)));

    int longDiff = 1;
    INT minVal = 0;

    if (typeid (INT).hash_code() == typeid (uint64_t).hash_code()) {
        longDiff = 1;
        minVal = 0x10000000;
    } else if (typeid (INT).hash_code() == typeid (uint128_t).hash_code()) {
        longDiff = 2;
        minVal = 0x1000000000000000;

    }/* else if (typeid (INT).hash_code() == typeid (uint256_t).hash_code()) {
        longDiff = 8;
        minVal = "0x1000000000000000000000000000000000000000000000000000000000000000";
    }*/

    INT res = 1;

    while (longDiff > 0 || minVal > res) {
        longDiff--;
        res *= static_cast<unsigned int>(rand());
    }

    return res;
}

template<class INT>
bool ferma(INT x){
    if(x == 2)
        return true;


    for(int i = 0; i < 100; i++){
        INT a = (randNumber<INT>() % (x - 2)) + 2;
        if (gcd<INT>(a, x) != 1)
            return false;
        if( pows(a, x-1, x) != 1)
            return false;
    }
    return true;
}

template<class INT>
bool isPrime(INT n) {
    if(n < 2)
        return false;

    if(n == 2)
        return true;

    if(n % 2 == 0)
        return false;

    for(INT i = 3; (i * i) <= n; i+=2) {
        if(n % i == 0 ) return false;
    }

    return true;
}

// to du
template<class INT>
INT randPrimeNumber(INT no = 0) {
    INT n = randNumber<INT>();
    while (n == no) {
        n = randNumber<INT>();
    };

    while (!ferma(n)) {
       n++;
    }

    return n;
}



template<class INT>
INT ExtEuclid(INT a, INT b)
{
    INT x = 0, y = 1, u = 1, v = 0, gcd = b, m, n, q, r;
    while (a != 0) {
        q = gcd / a;
        r = gcd % a;
        m = x - u * q;
        n = y - v * q;
        gcd = a;
        a = r;
        x = u;
        y = v;
        u = m;
        v = n;
    }
    return y;
}

template<class INT>
bool keyGenerator(QByteArray &pubKey,
                 QByteArray &privKey) {

    INT p = randPrimeNumber<INT>();
    INT q = randPrimeNumber<INT>(p);

    INT modul = p * q;
    INT eilor = eulerFunc(p, q);
    INT e;

    do {
        e = randPrimeNumber<INT>();

    } while((gcd<INT>(eilor, e) != 1) || eilor < e);

    INT d = 0;

    d = ExtEuclid(eilor, e);
    while(d < 0) {
        d = d + eilor;
    }

    if (typeid (INT).hash_code() == typeid (uint64_t).hash_code()) {
       pubKey.append(reinterpret_cast<char*>(&e), sizeof(e));
       pubKey.append(reinterpret_cast<char*>(&modul), sizeof(modul));
       privKey.append(reinterpret_cast<char*>(&d), sizeof(d));
       privKey.append(reinterpret_cast<char*>(&modul), sizeof(modul));

    }

    return false;

}

QRSAEncryption::QRSAEncryption()
{

}

QByteArray QRSAEncryption::encode(const QByteArray &rawData, const QByteArray &pubKey)
{

}

QByteArray QRSAEncryption::decode(const QByteArray &rawData, const QByteArray &privKey)
{

}


bool QRSAEncryption::generatePairKey(QByteArray &pubKey,
                                     QByteArray &privKey,
                                     QRSAEncryption::Rsa rsa)
{
    switch (rsa) {
    case RSA_128: {
        if (!keyGenerator<uint64_t>(pubKey, privKey)) {
            return false;
        }
        break;
    }
    case RSA_256: {
        if (!keyGenerator<uint128_t>(pubKey, privKey)) {
            return false;
        }
        break;
    }
//    case RSA_512: {
//        using INT = uint256_t;
//        break;
//    }
    default: return false;
    }


    return true;
}


