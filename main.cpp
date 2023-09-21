#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
using namespace std;

void writeMenu(int mode) {
    cout << "Mozliwe akcje:\n0. Wyjscie\n1. Przejrzyj plik\n2. Szyfr Cezara\n3. Szyfr podstawieniowy\n4. Szyfr Vigenera\n";
    if (mode == 2) cout << "5. Bomba kryptologiczna\n";
}

int modeSelect() {
    int mode = 0;
    do {
        cout << "Wybierz tryb\n1. Szyfrowanie\n2. Deszyfrowanie\n";
        cin >> mode;
        if (mode != 1 && mode != 2) cout << "Blad!\n";
    } while (mode != 1 && mode != 2);
    return mode;
}

void config(int * numberOfLines, string * upperCase, string * lowerCase, string * substitutionInput, string * substitutionOutput, string * keyword) {
    ifstream configFile;
    string nol;
    configFile.open("/Users/bpartyka/CLionProjects/szyfrowanie/config.cfg",ios::in);
    if (!configFile) {
        cout << "Plik konfiguracyjny nie zostal otwarty\n";
    }
    getline(configFile, nol);
    *numberOfLines = stoi(nol);
    getline(configFile, *upperCase);
    getline(configFile, *lowerCase);
    getline (configFile, *substitutionInput);
    getline (configFile, *substitutionOutput);
    getline(configFile, *keyword);
}

void showFile(int numberOfLines, string line[], string cipher[], bool bomb) {
    int page = 0;
    int mode = 0;
    do {
        if (!bomb) {
            cout << "Wyswietl wiadomosc:\n1. Odszyfrowana\n2. Zaszyfrowana\n3. Wyjscie\n\n";
        }
        if (bomb) {
            cout << "Wyswietl wiadomosc:\n1. Zaszyfrowana\n2. Odszyfrowana\n3. Wyjscie\n\n";
        }
        cin >> mode;
        if (mode == 1) {
            do {
                int next = 0;
                cout << "Strona " << page + 1 << "\n";
                for (int i = 5 * page; i < (numberOfLines + 5 * page); i++) {
                    cout << line[i] << "\n";
                }
                cout << "\n1. Wyjscie\n2. Nastepna strona\n3. Poprzednia strona\n\n";
                cin >> next;
                next--;
                if (next == 0)
                    break;
                else if (next == 1)
                    page++;
                else if (next == 2)
                    page--;
            } while (page >= 0);
        }
        else if (mode == 2) {
            do {
                int next = 0;
                cout << "Strona " << page + 1 << "\n";
                for (int i = 5 * page; i < (numberOfLines + 5 * page); i++) {
                    cout << cipher[i] << "\n";
                }
                cout << "\n1. Wyjscie\n2. Nastepna strona\n3. Poprzednia strona\n\n";
                cin >> next;
                next--;
                if (next == 0)
                    break;
                else if (next == 1)
                    page++;
                else if (next == 2)
                    page--;
            } while (page >= 0);
        }
        else if (mode == 3) break;
    } while (mode == 1 || mode == 2);
}

void generateKey(string keyword, string line[256], int textLines, string key[256]) {
    int length = static_cast<unsigned char>(keyword.size());
    int index = 0;
    for (int i = 0; i < textLines; i++) {
        int j = 0;
        do {
            if (isalpha(line[i][j])) {
                key[i] += keyword[index];
                index = ++index % length;
            }
            else
                key[i] += line[i][j];
            j++;
        } while (line[i][j] != '\0');
    }
}

void ceasar(string upperCase, string lowerCase, string line[], string cipher[], int textLines, int mode, int shift) {
    while (shift == 0) {
        cout << "Blad! Podaj inna liczbe!\n";
        cin >> shift;
    }
    if (mode == 2) {
        shift = 0 - shift;
    }
    for (int i = 0; i < textLines; i++) {
        int j = 0;
        do {
            if (!isalpha(line[i][j])) cipher[i] += line[i][j];
            else {
                for (int k = 0; k < 26; k++) {
                    int index = (k + shift) % 26;
                    if (index < 0) index += 26;
                    if (line[i][j] == lowerCase[k]) cipher[i] += lowerCase[index];
                    else if (line[i][j] == upperCase[k]) cipher[i] += upperCase[index];
                }
            }
            j++;
        } while (line[i][j] != '\0');
    }
}

void substitution(string input, string output, string line[256], string cipher[256], int textLines, int mode) {
    if (mode == 1) {
        for (int i = 0; i < textLines; i++) {
        int j = 0;
        do {
            if (!isalpha(line[i][j])) cipher[i] += line[i][j];
            else {
                for (int k = 0; k < 52; k++) {
                    if (line[i][j] == input[k]) cipher[i] += output[k];
                }
            }
            j++;
        } while (line[i][j] != '\0');
        }
    }
    else if (mode == 2) {
        for (int i = 0; i < textLines; i++) {
            int j = 0;
            do {
                if (!isalpha(line[i][j])) cipher[i] += line[i][j];
                else {
                    for (int k = 0; k < 52; k++) {
                        if (line[i][j] == output[k]) cipher[i] += input[k];
                    }
                }
                j++;
            } while (line[i][j] != '\0');
        }
    }
}

void vigener(string line[256], string cipher[256], string key[256], int textLines, int mode) {
    if (mode == 1) {
        for (int i = 0; i < textLines; i++) {
            int j = 0;
            do {
                char plainLetter = line[i][j], keyLetter = key[i][j], temp;
                if (isalpha(plainLetter)) {
                    if (isupper(plainLetter)) {
                        temp = static_cast<char>((plainLetter + keyLetter) % 26 + 65);
                    }
                    else if (islower(plainLetter))
                        plainLetter = static_cast<char>(toupper(plainLetter));
                        temp = static_cast<char>((plainLetter + keyLetter) % 26 + 65);
                        temp = static_cast<char>(tolower(temp));
                }
                else
                    temp = plainLetter;
                cipher[i] += temp;
                j++;
            } while (line[i][j] != '\0');
        }
    }
    else if (mode == 2) {
        for (int i = 0; i < textLines; i++) {
            int j = 0;
            do {
                char plainLetter = line[i][j], keyLetter = key[i][j], temp;
                if (isalpha(plainLetter)) {
                    if (isupper(plainLetter)) {
                        temp = static_cast<char>((plainLetter - keyLetter + 26) % 26 + 65);
                    }
                    else if (islower(plainLetter))
                        plainLetter = static_cast<char>(toupper(plainLetter));
                    temp = static_cast<char>((plainLetter - keyLetter + 26) % 26 + 65);
                    temp = static_cast<char>(tolower(temp));
                }
                else
                    temp = plainLetter;
                cipher[i] += temp;
                j++;
            } while (line[i][j] != '\0');
        }
    }
}

void cryptologicBomb(string line[256], string plainTextPolish[256], string plainTextEnglish[256], string upperCase, string lowerCase, int textLines) {
    int alphabet[26];
    int maxOccurence = -1, index = 0, shiftPolish = 0, shiftEnglish = 0;
    char maxUpperCase, maxLowerCase;
    for (int i = 0; i < 26; i++) {
        alphabet[i] = 0;
    }
    for (int i = 0; i < textLines; i++) {
        int j = 0;
        do {
            if (line[i][j] == 'a' || line[i][j] == 'A') alphabet[0]++;
            if (line[i][j] == 'b' || line[i][j] == 'B') alphabet[1]++;
            if (line[i][j] == 'c' || line[i][j] == 'C') alphabet[2]++;
            if (line[i][j] == 'd' || line[i][j] == 'D') alphabet[3]++;
            if (line[i][j] == 'e' || line[i][j] == 'E') alphabet[4]++;
            if (line[i][j] == 'f' || line[i][j] == 'F') alphabet[5]++;
            if (line[i][j] == 'g' || line[i][j] == 'G') alphabet[6]++;
            if (line[i][j] == 'h' || line[i][j] == 'H') alphabet[7]++;
            if (line[i][j] == 'i' || line[i][j] == 'I') alphabet[8]++;
            if (line[i][j] == 'j' || line[i][j] == 'J') alphabet[9]++;
            if (line[i][j] == 'k' || line[i][j] == 'K') alphabet[10]++;
            if (line[i][j] == 'l' || line[i][j] == 'L') alphabet[11]++;
            if (line[i][j] == 'm' || line[i][j] == 'M') alphabet[12]++;
            if (line[i][j] == 'n' || line[i][j] == 'N') alphabet[13]++;
            if (line[i][j] == 'o' || line[i][j] == 'O') alphabet[14]++;
            if (line[i][j] == 'p' || line[i][j] == 'P') alphabet[15]++;
            if (line[i][j] == 'q' || line[i][j] == 'Q') alphabet[16]++;
            if (line[i][j] == 'r' || line[i][j] == 'R') alphabet[17]++;
            if (line[i][j] == 's' || line[i][j] == 'S') alphabet[18]++;
            if (line[i][j] == 't' || line[i][j] == 'T') alphabet[19]++;
            if (line[i][j] == 'u' || line[i][j] == 'U') alphabet[20]++;
            if (line[i][j] == 'v' || line[i][j] == 'V') alphabet[21]++;
            if (line[i][j] == 'w' || line[i][j] == 'W') alphabet[22]++;
            if (line[i][j] == 'x' || line[i][j] == 'X') alphabet[23]++;
            if (line[i][j] == 'y' || line[i][j] == 'Y') alphabet[24]++;
            if (line[i][j] == 'z' || line[i][j] == 'Z') alphabet[25]++;
            j++;
        } while (line[i][j] != '\0');
    }
    for (int i = 0; i < 26; i++) {
        if (maxOccurence < alphabet[i]) {
            maxOccurence = alphabet[i];
            index = i;
        }
    }
    maxUpperCase = upperCase[index];
    shiftPolish = abs(static_cast<int>('A' - upperCase[index]));
    shiftEnglish = abs(static_cast<int>('E' - upperCase[index]));
    ceasar(upperCase, lowerCase, line, plainTextPolish, textLines, 2, shiftPolish);
    ceasar(upperCase, lowerCase, line, plainTextEnglish, textLines, 2, shiftEnglish);
}

int main(int argc, char *argv[]) {
    string inputFileName, outputFileName, upperCase, lowerCase, substitutionInput, substitutionOutput, line[256], cipher[256], keyword, plainTextPolish[256], plainTextEnglish[256];
    int numberOfLines = 0, textLines = 0, choice = 0, shift = 0, mode = 0;
    config(&numberOfLines, &upperCase, &lowerCase, &substitutionInput, &substitutionOutput, &keyword);
    if (argc == 5) {
        if (*argv[1] == 's') {
            mode = 0;
        }
        else if (*argv[1] == 'd') {
            mode = 1;
        }
        else return -1;
        shift = static_cast<int>(*argv[2]);
        inputFileName = *argv[3];
        outputFileName = *argv[4];
        ifstream inputFile(inputFileName, ios::in);
        if (!inputFile) {
            cout << "Plik wejsciowy nie zostal otwarty\n";
            return -1;
        }
        while (getline(inputFile, line[textLines])) {
            textLines++;
        }
        inputFile.close();
        ofstream outputFile(outputFileName, ios::out);
        if (!outputFile) {
            cout << "Plik wejsciowy nie zostal otwarty\n";
            return -1;
        }
        ceasar(upperCase, lowerCase, line, cipher, textLines, mode, shift);
        return 0;
    }
    else {
        cout << "Podaj sciezke pliku wejsciowego:\n";
        cin >> inputFileName;
        ifstream inputFile(inputFileName, ios::in);
        if (!inputFile) {
            cout << "Plik wejsciowy nie zostal otwarty\n";
            return -1;
        }
        while (getline(inputFile, line[textLines])) {
            textLines++;
        }
        inputFile.close();
        cout << "Podaj sciezke pliku wyjsciowego:\n";
        cin >> outputFileName;
        ofstream outputFile(outputFileName, ios::out);
        if (!outputFile) {
            cout << "Plik wejsciowy nie zostal otwarty\n";
            return -1;
        }
        mode = modeSelect();
        do {
            writeMenu(mode);
            cin >> choice;
            switch (choice) {
                default: {
                    ceasar(upperCase, lowerCase, line, cipher, textLines, mode, shift);
                    for (int i = 0; i < textLines; i++) {
                        outputFile << cipher[i] << endl;
                        outputFile.flush();
                    }
                }
                    showFile(numberOfLines, line, cipher, false);
                    break;
                case 0:
                    break;
                case 1: {
                    showFile(numberOfLines, line, cipher, false);
                    break;
                }
                case 2: { //szyfr cezara
                    ceasar(upperCase, lowerCase, line, cipher, textLines, mode, shift);
                    for (int i = 0; i < textLines; i++) {
                        outputFile << cipher[i] << endl;
                        outputFile.flush();
                    }
                    showFile(numberOfLines, line, cipher, false);
                    break;
                }
                case 3: {
                    substitution(substitutionInput, substitutionOutput, line, cipher, textLines, mode);
                    for (int i = 0; i < textLines; i++) {
                        outputFile << cipher[i] << endl;
                        outputFile.flush();
                    }
                    showFile(numberOfLines, line, cipher, false);
                    break;
                }
                case 4: {
                    string key[256];
                    generateKey(keyword, line, textLines, key);
                    vigener(line, cipher, key, textLines, mode);
                    for (int i = 0; i < textLines; i++) {
                        outputFile << cipher[i] << endl;
                        outputFile.flush();
                    }
                    showFile(numberOfLines, line, cipher, false);
                    break;
                }
                case 5: {
                    int text = 0;
                    cryptologicBomb(line, plainTextPolish, plainTextEnglish, upperCase, lowerCase, textLines);
                    cout << "Proba zdeszyfrowania po polsku\n\n";
                    showFile(numberOfLines, line, plainTextPolish, true);
                    cout << "Proba zdeszyfrowania po angielsku\n\n";
                    showFile(numberOfLines, line, plainTextEnglish, true);
                    do {
                        cout << "Wybierz, ktory tekst jawny jest poprawny:\n1. Polski\n2. Angielski\n";
                        cin >> text;
                        text--;
                        if (text == 0) {
                            for (int i = 0; i < textLines; i++) {
                                outputFile << plainTextPolish[i] << endl;
                                outputFile.flush();
                            }
                        } else if (text == 1) {
                            for (int i = 0; i < textLines; i++) {
                                outputFile << plainTextEnglish[i] << endl;
                                outputFile.flush();
                            }
                        }
                        else {
                            cout << "Blad! Wybierz tekst!";
                            cin >> text;
                        }
                    } while (!(text == 0 || text == 1));
                }
            }
        } while (choice != 0);
        outputFile.close();
        return 0;
    }
}

/* config.cfg:
7
ABCDEFGHIJKLMNOPQRSTUVWXYZ
abcdefghijklmnopqrstuvwxyz
ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz
BCDEFGHIJKLMNOPQRSTUVWXYZAbcdefghijklmnopqrstuvwxyza
KEYWORD
*/