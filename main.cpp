#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <math.h>

using namespace std;


int Dec16(char num) {

    char MainNumber = num;

    int koef = ((int)MainNumber);
    if (koef >= 65) {
        koef = koef - 55;
    }
    else {
        koef = koef - '0';
    }
    long double degree = pow(16, 0);
    int degree1 = int(degree);
    int Ref = koef * degree1;
    return Ref;




}

string Dec(string num, int Select) { // перевод из 2-9 в 10
    int n;
    string MainNumber = num;
    int Index = MainNumber.find(".");
    if (Index != -1){
        n = Index - 1;
    }else{
        n = MainNumber.length() - 1;
    }

    long double RefreshNumber = 0;
    long double degree;
    for (int i = 0; i < MainNumber.length(); i++) {
        if (MainNumber[i] != '.'){
            int koef = ((int)MainNumber[i]);
            koef = koef - '0';
            degree = pow(Select, n);
            //cout << "n = " << n << " degree " << degree << endl;
            //cout << "n = " << n << endl;
            n--;

            RefreshNumber += koef * degree;
        }

    }
    //cout << RefreshNumber << endl;


    return to_string(RefreshNumber);
}

string Bin(int num) {
    vector<char> remainders;
    while (num != 0) {
        int Remainder = num % 2;
        remainders.push_back(Remainder + 48);
        num = (num - Remainder) / 2;
    }
    reverse(remainders.begin(), remainders.end());
    string ans = "";
    for (int i = 0; i < remainders.size(); i++) {
        ans = ans + remainders[i];
    }
    return ans;
}

string Hex(int num) {
    vector<char> remainders;
    while (num != 0) {
        int Remainder = num % 16;
        if (Remainder >= 10) {
            remainders.push_back((55 + Remainder));
        }
        else {
            remainders.push_back((Remainder)+48);
        }
        num = (num - Remainder) / 16;
    }
    reverse(remainders.begin(), remainders.end());
    string ans = "";
    for (int i = 0; i < remainders.size(); i++) {
        ans = ans + remainders[i];
    }
    return ans;
}

class TranslateNumbers
{
private:
    long double MainNumber;

    string Answer;

    string CPUTable;
    string Sign;
    string Order;
    string Mantissa;

    bool FlagInfinityOfMantissa = true;

    string IEEE754;

    void GetOrder(int orderLen) {
        long double CPUTableInLD = stold(CPUTable);
        //cout << fixed << setprecision(20) <<CPUTableInLD << endl;
        //cout << CPUTable << endl;

        int PDegree = 0;
        while (CPUTableInLD > 10) {
            // cout << fixed <<CPUTableInLD << "  " << PDegree << endl;
            CPUTableInLD = CPUTableInLD / 10;
            PDegree++;
        }

        while (CPUTableInLD < 1) {
            CPUTableInLD = CPUTableInLD * 10;
            PDegree--;
        }

        int Pc = PDegree + (pow(2, orderLen - 1) - 1);

        Order = Bin(Pc);
        while (Order.length() < orderLen) {
            Order = '0' + Order;
        }
        //cout << PDegree << endl;
        //cout << "Pc " << Pc << endl;
        //cout << "Order " << Order << endl;
        //cout << fixed << setprecision(20) <<CPUTableInLD << endl;
    }

    void GetMantissa(int mantissaLen) {
        Mantissa = "";
        cout << CPUTable << endl;
        bool flag = false;
        for (int i = 0; i < CPUTable.length(); i++) {
            if (CPUTable[i] == '1' and flag == false) {
                flag = true;
                continue;
            }
            if (flag == true and CPUTable[i] != '.') {
                Mantissa = Mantissa + CPUTable[i];
                //cout << CPUTable[i];
            }
        }

        if (Mantissa.length() < mantissaLen) {
            FlagInfinityOfMantissa = false;
        }
        if (FlagInfinityOfMantissa and Mantissa[mantissaLen] == '1') {
            FlagInfinityOfMantissa = true;
            //cout << "i man " << endl;
        }
        else {
            FlagInfinityOfMantissa = false;
        }
        while (Mantissa.length() < mantissaLen) {

            Mantissa = Mantissa + '0';
        }
        //cout << " MAntissa " << Mantissa << endl;
        string TestMantissa;
        //cout << "Test " << endl;
        for (int i = 0; i < mantissaLen; i++) {
            TestMantissa = TestMantissa + Mantissa[i];
            //cout << Mantissa[i];
        }
        Mantissa = TestMantissa;

        //cout << Mantissa << " Len " << Mantissa.length() << endl;
    }



    void TranslateTo0xF() {
        string fnum = "0000";
        int count = 0;
        int arg;
        string to10;
        string ieee754 = "";

        string SOM = Sign + Order + Mantissa;
        cout << "SOM " << SOM << " LEN " << SOM.length() << endl;
        for (int i = 0; i < SOM.length(); i++) {
            fnum[count] = SOM[i];
            count++;

            if (count == 4) {

                arg = stoi(fnum);
                if (FlagInfinityOfMantissa and i == SOM.length() - 1) {
                    //cout << " inf " << endl;
                    arg = arg + 1;
                }
                ieee754 = ieee754 + Hex(stoi(Dec(to_string(arg), 2)));
                if (fnum == "0000") {
                    ieee754 = ieee754 + '0';
                }
                count = 0;
            }
        }
        IEEE754 = ieee754;
    }

public:
    TranslateNumbers(long double mainNumber, int counter) {
        if (mainNumber == 0 and counter == 8) {
            cout << "0x00000000" << endl;
            exit(0);
        }
        if (mainNumber == 0 and counter == 16) {
            cout << "0x0000000000000000" << endl;
            exit(0);
        }
        if (mainNumber < 0) {
            mainNumber = mainNumber * -1;
            Sign = '1';
        }
        else {
            Sign = '0';
        }
        int MantissaLen = 0;
        int OrderLen = 0;
        int FullLen = 0;
        if (counter == 8) {
            MantissaLen = 23;
            OrderLen = 8;
            FullLen = 32;
        }
        else if (counter == 16) {
            MantissaLen = 52;
            OrderLen = 11;
            FullLen = 64;
        }
        else {
            exit(0);
        }
        vector<char> remainders;
        int Index = to_string(mainNumber).find(".");
        int PartIntTen = int(mainNumber);       //Разбиения числа на целую часть (PartIntTen).



        while (PartIntTen != 0) {
            int Remainder = PartIntTen % 2;

            if (Remainder >= 10) {

                remainders.push_back((55 + Remainder));
            }
            else {
                remainders.push_back((Remainder)+48);
            }
            PartIntTen = (PartIntTen - Remainder) / 2;
        }
        reverse(remainders.begin(), remainders.end());
        if (remainders.size() == 0)
            remainders.push_back('0');

        if (Index != -1) {
            //cout <<  "Index " << Index << endl;
            remainders.push_back('.');
        }
        /*for (int i = 0; i < remainders.size(); i++){
            cout << remainders[i] << endl;
        }
        cout << endl;*/


        long double Rem = (long double)(mainNumber)-int(mainNumber);

        int count = 0;                      //Вычисление с точностью Counter или пока дробная часть не станет равна нулю.
        while ((Rem != 0) and (count < 100)) {

            Rem = Rem * 2;
            if ((int)Rem >= 10) {
                remainders.push_back((int)Rem + 55);

            }
            else {
                remainders.push_back((int)Rem + 48);

            }

            Rem = (long double)(Rem)-int(Rem);
            count++;


        }


        Answer = "";
        for (int i = 0; i < remainders.size(); i++) {
            Answer.insert(i, 1, (remainders[i]));

        }
        if (Answer.back() == '.')           //Удаление точки, если дробная часть в результате отсуствует.
        {
            Answer.pop_back();
            Answer.pop_back();
        }
        CPUTable = "";
        for (int i = 0; i < remainders.size(); i++) {
            //if (remainders[i] != '.'){
            CPUTable = CPUTable + remainders[i];
            //}

        }
        //cout << CPUTable << endl;

        GetOrder(OrderLen);
        GetMantissa(MantissaLen);
        TranslateTo0xF();

        cout << "Sign " << Sign << endl;
        cout << "Order " << Order << endl;
        cout << "Mantissa " << Mantissa << endl;
        cout << "0x" << IEEE754 << endl;

        //cout << "Infinity " << FlagInfinityOfMantissa << endl;
    }
};

class TranslateHexNumbers {
private:
    string Sign;
    string Order;
    string Mantissa;

    string IEEE754;

    string RequiredNumber;

    int P;

    void GetOrder(int lenOrder) {
        string StrOrder = "";
        for (int i = 1; i < lenOrder + 1; i++) {
            StrOrder = StrOrder + IEEE754[i];
        }

        string PcStr = Dec(StrOrder, 2);
        int P = -(pow(2, lenOrder - 1) - 1 - stoi(PcStr));
        this->P = P;
    }

    void GetMantissa(int MantissaLen, int OrderLen) {
        string NormalizedMantissa = "";
        for (int i = OrderLen + 1; i < MantissaLen + OrderLen + 1; i++) {
            NormalizedMantissa = NormalizedMantissa + IEEE754[i];
        }
        NormalizedMantissa = "1" + NormalizedMantissa;
        //cout << NormalizedMantissa << " " << P << endl;

        if (P < 0){
            for (int i = 0; i < -P - 1; i++){
                NormalizedMantissa = '0' + NormalizedMantissa;
            }
            NormalizedMantissa = "0." + NormalizedMantissa;
        }else if (P > 0){
            string NewNormMant = "";
            for (int i = 0; i < NormalizedMantissa.length(); i++){
                NewNormMant = NewNormMant + NormalizedMantissa[i];
                if (i == P){
                    NewNormMant = NewNormMant + '.';
                }
            }
            NormalizedMantissa = NewNormMant;
        }else{
            string NewNormMant = "";
            for (int i = 0; i < NormalizedMantissa.length(); i++){
                NewNormMant = NewNormMant + NormalizedMantissa[i];
                if (i == 0){
                    NewNormMant = NewNormMant + '.';
                }
            }
            NormalizedMantissa = NewNormMant;
        }
        RequiredNumber = Dec(NormalizedMantissa, 2);
        //cout << RequiredNumber << endl;
    }

    void GetSign(){
        if (IEEE754[0] == '1'){
            RequiredNumber =  '-' + RequiredNumber;
        }
    }


public:
    TranslateHexNumbers(string ieee754) {

        string ieee754in2 = "";
        int OrderLen = 0;
        int MantissaLen = 0;
        if (ieee754.length() == 8) {
            OrderLen = 8;
            MantissaLen = 23;
        }
        else if (ieee754.length() == 16) {
            OrderLen = 11;
            MantissaLen = 52;
        }
        else {
            cout << "Error len" << endl;
        }
        string UnitBit = "";
        for (int i = 0; i < ieee754.length(); i++) {
            UnitBit = Bin(Dec16(ieee754[i]));
            while (UnitBit.length() < 4) {
                UnitBit = '0' + UnitBit;
            }
            if (UnitBit == "") {
                UnitBit = "0000";
            }
            ieee754in2 = ieee754in2 + UnitBit;

        }
        IEEE754 = ieee754in2;

        GetOrder(OrderLen);
        GetMantissa(MantissaLen, OrderLen);
        GetSign();
        cout << RequiredNumber << endl;
    }

};

int main() {
    long double MainNumber;
    string IEEE754;



    cout << "Enter the number into IEEE 754 of float and double (8 and 16 bytes) " << endl;
    cin >> MainNumber;
    TranslateNumbers num(MainNumber, 8);

    cout << " " << endl;

    TranslateNumbers num2(MainNumber, 16);
    cout << " " << endl;
    cout << "Enter the number into float and double (8 and 16 bytes) of IEEE754" << endl;
    cin >> IEEE754;
    TranslateHexNumbers num3(IEEE754);
}
