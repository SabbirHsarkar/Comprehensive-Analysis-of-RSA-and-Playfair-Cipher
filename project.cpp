#include <iostream>
#include <cmath>
#include <cstring>
#include <string>
using namespace std;

#define SIZE 30

// -------------------------- RSA Functions --------------------------

int phi(int n) {
    int result = n;
    for (int i = 2; i <= sqrt(n); i++) {
        if (n % i == 0) {
            while (n % i == 0) {
                n /= i;
            }
            result -= result / i;
        }
    }
    if (n > 1) {
        result -= result / n;
    }
    return result;
}

int gcd(int a, int b) {
    if (b == 0) {
        return a;
    }
    return gcd(b, a % b);
}

int modpow(int a, int b, int m) {
    int result = 1;
    while (b > 0) {
        if (b & 1) {
            result = (result * a) % m;
        }
        a = (a * a) % m;
        b >>= 1;
    }
    return result;
}

int findE(int phiN) {
    int e = 2;
    while (e < phiN) {
        if (gcd(e, phiN) == 1) {
            return e;
        }
        e++;
    }
    return -1;  // If no valid `e` is found, return -1
}

int findD(int e, int phiN) {
    int d = 0;
    while ((d * e) % phiN != 1) {
        d++;
    }
    return d;
}

// -------------------------- Playfair Functions --------------------------

void toLowerCase(char plain[], int ps) {
    for (int i = 0; i < ps; i++) {
        if (plain[i] > 64 && plain[i] < 91)
            plain[i] += 32;
    }
}

int removeSpaces(char* plain, int ps) {
    int i, count = 0;
    for (i = 0; i < ps; i++)
        if (plain[i] != ' ')
            plain[count++] = plain[i];
    plain[count] = '\0';
    return count;
}

void generateKeyTable(char key[], int ks, char keyT[5][5]) {
    int i, j, k, flag = 0;

    int dicty[26] = {0};
    for (i = 0; i < ks; i++) {
        if (key[i] != 'j')
            dicty[key[i] - 97] = 2;
    }
    dicty['j' - 97] = 1;

    i = 0;
    j = 0;

    for (k = 0; k < ks; k++) {
        if (dicty[key[k] - 97] == 2) {
            dicty[key[k] - 97] -= 1;
            keyT[i][j] = key[k];
            j++;
            if (j == 5) {
                i++;
                j = 0;
            }
        }
    }

    for (k = 0; k < 26; k++) {
        if (dicty[k] == 0) {
            keyT[i][j] = (char)(k + 97);
            j++;
            if (j == 5) {
                i++;
                j = 0;
            }
        }
    }
}

void search(char keyT[5][5], char a, char b, int arr[]) {
    int i, j;

    if (a == 'j')
        a = 'i';
    else if (b == 'j')
        b = 'i';

    for (i = 0; i < 5; i++) {
        for (j = 0; j < 5; j++) {
            if (keyT[i][j] == a) {
                arr[0] = i;
                arr[1] = j;
            }
            else if (keyT[i][j] == b) {
                arr[2] = i;
                arr[3] = j;
            }
        }
    }
}

int mod5(int a) { return (a % 5); }

int prepare(char str[], int ptrs) {
    if (ptrs % 2 != 0) {
        str[ptrs++] = 'z';
        str[ptrs] = '\0';
    }
    return ptrs;
}

void encrypt(char str[], char keyT[5][5], int ps) {
    int i, a[4];

    for (i = 0; i < ps; i += 2) {
        search(keyT, str[i], str[i + 1], a);

        if (a[0] == a[2]) {
            str[i] = keyT[a[0]][mod5(a[1] + 1)];
            str[i + 1] = keyT[a[0]][mod5(a[3] + 1)];
        }
        else if (a[1] == a[3]) {
            str[i] = keyT[mod5(a[0] + 1)][a[1]];
            str[i + 1] = keyT[mod5(a[2] + 1)][a[1]];
        }
        else {
            str[i] = keyT[a[0]][a[3]];
            str[i + 1] = keyT[a[2]][a[1]];
        }
    }
}

void encryptByPlayfairCipher(char str[], char key[]) {
    char ps, ks, keyT[5][5];

    ks = strlen(key);
    ks = removeSpaces(key, ks);
    toLowerCase(key, ks);

    ps = strlen(str);
    toLowerCase(str, ps);
    ps = removeSpaces(str, ps);

    ps = prepare(str, ps);

    generateKeyTable(key, ks, keyT);

    encrypt(str, keyT, ps);
}

// -------------------------- Main Program --------------------------

int main() {
    int choice;

    cout << "Select the Encryption Method: \n";
    cout << "1. RSA Encryption\n";
    cout << "2. Playfair Cipher\n";
    cout << "Enter your choice (1 or 2): ";
    cin >> choice;

    if (choice == 1) {
        // RSA Encryption
        int p, q, e, d, n, phiN, m, c, decrypted;

        cout << "-------------------- RSA Encryption --------------------" << endl;
        cout << "Enter prime numbers p and q (e.g., 11 13): ";
        cin >> p >> q;

        n = p * q;
        phiN = (p - 1) * (q - 1);

        // Automatically find e based on phi(n)
        e = findE(phiN);
        if (e == -1) {
            cout << "Couldn't find an appropriate value for e." << endl;
            return 1;
        }

        d = findD(e, phiN);

        cout << "Public key: {" << e << ", " << n << "}" << endl;
        cout << "Private key: {" << d << ", " << n << "}" << endl;

        cout << "\n""Enter the message (numeric): ";
        cin >> m;

        c = modpow(m, e, n);  // Encrypt
        decrypted = modpow(c, d, n);  // Decrypt

        cout << "\n""   Original message: " << m << endl;
        cout << "   Encrypted message: " << c << endl;
        cout << "   Decrypted message: " << decrypted << endl;
    }
    else if (choice == 2) {
        // Playfair Cipher
        cout << "-------------------- Playfair Cipher --------------------" << endl;
        char str[SIZE], key[SIZE];

        cout << "Enter key text for Playfair Cipher (e.g., Monarchy): ";
        cin >> key;

        cout << "Enter plain text for Playfair Cipher (e.g., instruments): ";
        cin >> str;

        encryptByPlayfairCipher(str, key);

        cout << "\n""Cipher text: " << str << endl;
    }
    else {
        cout << "Invalid choice! Please restart and select either 1 or 2." << endl;
    }

    return 0;
}
