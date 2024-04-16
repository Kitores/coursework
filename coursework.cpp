#include <iostream>
#include <iomanip>
#include <fstream>
#include <stdio.h>
#include <cmath>
using namespace std;

bool checkNum(char ch);
int Prohod_1(ifstream& in, ofstream& out, ofstream& floatPointsOut, int* pRowss);
void zapolnenieMassiva(float** Points, int m, ifstream& PointsFRead, ofstream& protocol);
void Prohod_2(ifstream& floatpoints, ofstream& PointsFWrite, ofstream& protocol, int m);
void FigureSearching(float** Points, int rows, ofstream& protocol, ofstream& itog);
void CountFigures(float*** Figures, float** Points, int rows, int figure, ofstream& protocol,
ofstream& itog);
void MaxPoints(float*** Figures, int figure, ofstream& itog);

int main() {
    ifstream in("hhin.txt");
    ofstream protocol("protocol.txt");
    ofstream floatPointsOut("floatpoints.txt");
    ifstream floatPointsIn;
    ofstream PointsFWrite;
    ifstream PointsFRead;
    ofstream itog;
    int rows = 1500;
    int* pRows = &rows;
    cout << *pRows;
    int i = 0;
    int m = 0;
    m = Prohod_1(in, protocol, floatPointsOut, pRows);
    cout << m;
    in.close();
    floatPointsOut.close();
    float** Points = new float*[m];
    for (int i = 0; i < m; i++){
        Points[i] = new float[3];
    }
    PointsFWrite.open("points.txt");
    floatPointsIn.open("floatpoints.txt");
    Prohod_2(floatPointsIn, PointsFWrite, protocol, m);
    floatPointsIn.close();
    PointsFWrite.close();
    PointsFRead.open("points.txt");
    zapolnenieMassiva(Points, m, PointsFRead, protocol);
    PointsFRead.close();
    itog.open("itog.txt");
    FigureSearching(Points, m, protocol, itog);
    protocol.close();
    itog.close();

}
bool checkNum(char ch){
    if ((ch >= '0') && (ch <= '9')) return true;
    else return false;
}
int Prohod_1(ifstream& in, ofstream& protocol, ofstream& floatPoints, int* pRows) {
    protocol << "----------------------- Работает функция Prohod_1 -----------------------";
    bool flag = true;
    int spacepos = 0;
    int quanity = 0;
    bool LetterError = false;
    char ch = '/';
    int k = 0;
    int countLet{0}, counterNumInRow{0};
    int SpaceOrNCounter = 0;
    int lenNum = 0;
    int rowF = 0;
    in.unsetf(ios::skipws);
    while (1){
        if (in.eof()) break;
        in >> ch;
        if((ch >= 'A') && (ch <= 'z')){
            countLet++;
            if(lenNum > 0){ // если число прервала буква
                counterNumInRow++;
            }
            if (counterNumInRow < 2)LetterError = true;
            if (counterNumInRow == 0 ? flag = true : flag = false);
            lenNum = 0;
        }
        else if (checkNum(ch)){
            if(lenNum == 0){
                counterNumInRow++;
            }
            lenNum++;
            if ((SpaceOrNCounter <= 1) && (counterNumInRow <= 2) && (!LetterError)) floatPoints << ch;
            if ((in.peek() == ' ') && (counterNumInRow == 1)) { // Если за первым числом строки следует пробел
                while(in.peek() == ' ') {
                    in >> ch;
                    }
                if ( checkNum(in.peek()) ) {
                    floatPoints << ' ';
                    SpaceOrNCounter++;
                }
                lenNum = 0;
            }
            if (!checkNum(in.peek())) lenNum = 0;
        }
        else if (ch == '-') {
            floatPoints << ch;
        }
        else if (ch == ' ') {
            SpaceOrNCounter++;
            if(lenNum > 0){
                counterNumInRow++;
            }

            lenNum = 0;
        }
        else if (ch == '\n') {
            SpaceOrNCounter = 0;

            floatPoints << '\n';
            protocol << '\n';
            lenNum = 0;

            if (LetterError) protocol << rowF << " строка - буквы перед цифрами" << " ошибка";
            else if ((counterNumInRow == 2) && (LetterError == false)) {
                protocol << rowF << " строка - " << " 2 числа";
                if (countLet > 0) {
                    protocol << " и " << countLet << " буквы!" << right << setw(5) << "Буквы отбрасываются";
                }
                else protocol << "!";
                k++;
                quanity++;
            }
            else if (counterNumInRow > 2) {
                protocol << rowF << " строка - " << " более 2 чисел ";
                if (countLet > 0) {
                    protocol << " и " << countLet << " буквы!" << right << setw(5) << "Лишние цифры и буквы отбрасываются";
                }
                else if (countLet == 0) protocol << "Лишние цифры отбрасываются";
                k++;
                quanity++;
            }
            else if (flag) protocol << rowF << " строка - присутствуют только буквы" << " ошибка";
            else if ((in.peek() == ' ')) { // Если за первым числом строки следует пробел
                while(in.peek() == ' ') {
                    in >> ch;
                }
            }
            else if (counterNumInRow == 1) {
                protocol << rowF << " строка - " << " 1 число! " << setw(10) << left << " - ошибка";
            }
            else if (counterNumInRow == 0) {
                protocol << rowF << " строка - " << " не найдено чисел!" << setw(10) << left << " - ошибка";
            }
            else {
                protocol << rowF << " строка не подходит!" << " " << counterNumInRow;
            }
            countLet = 0;
            counterNumInRow = 0;
            rowF++;
            flag = false;
            LetterError = false;
        }

        else if (ch == '.') {
            lenNum++;
            floatPoints << ch;
        }
    }
    protocol << "\n\n Итого: " << quanity << " строк подходят!";
    *pRows = k;
    protocol << "\n----------------------- Функция Prohod_1 завершила работу! -----------------------";
    return quanity;
}
void Prohod_2(ifstream& floatpoints, ofstream& PointsFWrite, ofstream& protocol, int m) {
    protocol << "\n\n----------------------- Функция Prohod_2 начала работу! -----------------------\nФайл points.txt заполняется координатами точек\n";
    int elem = 0;
    float tmp = 0;
    int NuminRow = 0;
    floatpoints.setf(ios :: skipws);
    while(1) {
        if (!(floatpoints.eof()) && !(elem == 2 * m)) {
            floatpoints >> tmp;
            NuminRow++;

            if ( !((NuminRow == 1) && (floatpoints.peek() == '\n')) ) {
                PointsFWrite << tmp;
                protocol << "\nВ файл points.txt добавлено число " << tmp;
                elem++;
                if (NuminRow == 2) {
                    PointsFWrite << '\n';
                    NuminRow = 0;
                }
                else PointsFWrite << ' ';
            }
            else NuminRow = 0;
        }
        else break;
    }
    protocol << "\n----------------------- Функция Prohod_2 завершила работу! -----------------------";
}
void zapolnenieMassiva(float** Points, int m, ifstream& PointsFRead, ofstream& protocol) {
    protocol << "\n\n----------------------- Функция zapolnenieMassiva начала работу! -----------------------\n";
    PointsFRead.setf(ios::skipws);
    int i = 0;
    int elem = 0;
    int j = 0;
    float tmp = 0;
    int rowF = 0;
    int k = 0;
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < 3; j++) {
            if (j == 2) Points[i][j] = 0;
            else {
                PointsFRead >> tmp;
                Points[i][j] = tmp;
            }
            protocol << "в файл points.txt записано число " << Points[i][j] << '\n';
            cout << Points[i][j] << ' ';
        }
        cout << '\n';
    }
    protocol << "Результат работы функции - заполненный массив: \n";

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < 3; j++) {
            protocol << setw(5) << right << Points[i][j] << " " ;
        }
        protocol << '\n';
    }
    protocol << "\n\n----------------------- Функция zapolnenieMassiva закончила работу! -----------------------\n";
}
void FigureSearching(float** Points, int rows, ofstream& protocol, ofstream& itog) {
    protocol << "\n\n----------------------- Функция Figuresearching начала работу! -----------------------\n";
    int n = 100;
    int figure = 0;
    float tmp = 0;
    int zeroCounter = 0;
    float r1 = 0;
    float r2 = 0;
    int q = 0;
    float*** Figures = new float**[n];
    for (int i = 0; i < n; i++){
        Figures[i] = new float*[7];
        for (int j = 0; j < 7; j++) {
            Figures[i][j] = new float[2]{0, 0};
        }
    }
    float X1{0}, X2{0}, Y1{0}, Y2{0}, X3{0}, Y3{0}, X4{0}, Y4{0}, X5{0}, Y5{0}, X6{0}, Y6{0};
    float k1{0}, k2{0}, k3{0}, k4{0};
    for (int i = 0; i < rows; i++) {
        if (Points[i][2] == 2)continue;
        X1 = Points[i][0];
        Y1 = Points[i][1];
        Points[i][2] = 1;
        for (int j = 0; j < rows; j++) {
            if (j == i) continue;
            if ((Points[j][2] == 1) || (Points[j][2] == 2)) continue;
            X2 = Points[j][0];
            Y2 = Points[j][1];
            if (X1 == X2) k1 = -1000;
            else if (Y1 == Y2) k1 = 0.001;
            else k1 = (Y2 - Y1) / (X2 - X1);
            protocol << "Коэффициент k1 для точек ("<< X1 << ',' << Y1 << ")," << '(' << X2 << ',' << Y2 << ") "<< "равен " << k1 << '\n';
            for (int k = 0; k < rows; k++) {
                if ((k == i) || (k == j)) continue;
                if ((Points[k][2] == 1) || (Points[j][2] == 2)) continue;
                X3 = Points[k][0];
                Y3 = Points[k][1];
                if (X3 == X1) k2 = -1000;
                else if (Y3 == Y1) k2 = 0.001;
                else k2 = (Y3 - Y1) / (X3 - X1);
                if ((k1 * k2 ) == -1) cout << "Прямой угол 1";
                else continue;
                protocol << "Коэффициент k2 для точек ("<< X1 << ',' << Y1 << ")," << '(' << X3 << ',' << Y3 << ") "<< "равен " << k2 << '\n';
                for (int z = 0; z < rows; z++) {
                    if ((z == i) || (z == j) || (z == k)) continue;
                    if ((Points[z][2] == 1) || (Points[j][2] == 2)) continue;
                    X4 = Points[z][0];
                    Y4 = Points[z][1];
                    //Points[z][2] = 1;
                    if (X3 == X4) k3 = -1000;
                    else if (Y3 == Y4) k3 = 0.001;
                    else k3 = (Y4 - Y3) / (X4 - X3);
                    if (X4 == X2) k4 = -1000;
                    else if (Y4 == Y2) k4 = 0.001;
                    else k4 = (Y4 - Y2) / (X4 - X2);
                    protocol << "Коэффициент k3 для точек ("<< X3 << ',' << Y3 << ")," << '(' << X4 << ',' << Y4 << ") "<< "равен " << k3 << '\n';
                    protocol << "Коэффициент k4 для точек ("<< X2 << ',' << Y2 << ")," << '(' << X4 << ',' << Y4 << ") "<< "равен " << k4 << '\n';
                    if ( ((k2 * k3) == -1) && ((k3 * k4) == -1) ) {
                        protocol << "\nНайден прямоугольник с координатами точек: (" << X1 << ',' << Y1 << ") (" << X2 << ',' << Y2 << ") (" << X3 << ',' << Y3 << ") (" << X4 << ',' << Y4 << ")\n\n" ;
                        for (int t = 0; t < rows; t++) {
                            if ((Points[t][2] == 1) || (Points[j][2] == 2)) continue;
                            if ((t == i)|| (t == j) || (t == k) || (t == z)) continue;
                            X5 = Points[t][0];
                            Y5 = Points[t][1];
                            if ( ((X2 + X1)/2 == X5) && ((Y2 + Y1)/2 == Y5)) {
                                r1 = (sqrtf( pow((X2 - X1), 2) + pow((Y2 - Y1), 2))) / 2;
                                for (int l = 0; l < rows; l++) {
                                    if ((Points[l][2] == 1) || (Points[j][2] == 2)) continue;
                                    if ((l == i)|| (l == j) || (l == k) || (l == z) || (l == t)) continue;
                                    X6 = Points[l][0];
                                    Y6 = Points[l][1];
                                    if ( ((X3 + X4)/2 == X6) && ((Y3 + Y4)/2 == Y6)) {
                                        r2 = (sqrtf( pow((X4 - X3), 2) + pow((Y4 - Y3), 2))) / 2 ;
                                        if (((sqrtf( pow((X5 - X6), 2) + pow((Y5 - Y6), 2) )) >= r1 + r2) ) {
                                            itog << "\n\n\nНайдена фигура с координатами: " << '(' << X1 << ',' <<
                                            Y1 << ") (" << X2 << ',' << Y2 << ") (" << X3 << ',' << Y3 << ") (" << X4 << ',' << Y4 << ") (" << X5 << ',' << Y5 << ") (" << X6 << ',' << Y6 << ")\n";
                                            protocol << "\n\n\nНайдена фигура с координатами: " << '(' << X1 << ',' << Y1 << ") (" << X2 << ',' << Y2 << ") (" << X3 << ',' << Y3 << ") (" << X4 << ',' << Y4 << ") (" << X5 << ',' << Y5 << ") (" << X6 << ',' << Y6 << ")\n";
                                            if ((X1 > X3) || (X2 > X4)) {
                                                tmp = X1;
                                                X1 = X3;
                                                X3 = tmp;
                                                tmp = Y1;
                                                Y1 = Y3;
                                                Y3 = tmp;
                                                tmp = X2;
                                                X2 = X4;
                                                X4 = tmp;
                                                tmp = Y2;
                                                Y2 = Y4;
                                                Y4 = tmp;
                                                tmp = X5;
                                                X5 = X6;
                                                X6 = tmp;
                                                tmp = Y5;
                                                Y5 = Y6;
                                                Y6 = tmp;
                                            }
                                            if ((Y1 > Y2) || (Y3 > Y4)) {
                                                tmp = Y1;
                                                Y1 = Y2;
                                                Y2 = tmp;
                                                tmp = X1;
                                                X1 = X2;
                                                X2 = tmp;

                                                tmp = Y3;
                                                Y3 = Y4;
                                                Y4 = tmp;
                                                tmp = X3;
                                                X3 = X4;
                                                X4 = tmp;
                                            }
                                            Figures[figure][0][0] = X1;
                                            Figures[figure][0][1] = Y1;
                                            Figures[figure][1][0] = X2;
                                            Figures[figure][1][1] = Y2;
                                            Figures[figure][2][0] = X3;
                                            Figures[figure][2][1] = Y3;
                                            Figures[figure][3][0] = X4;
                                            Figures[figure][3][1] = Y4;
                                            Figures[figure][4][0] = X5;
                                            Figures[figure][4][1] = Y5;
                                            Figures[figure][5][0] = X6;
                                            Figures[figure][5][1] = Y6;
                                            Points[l][2] = 2;
                                            Points[t][2] = 2;
                                            Points[k][2] = 2;
                                            Points[z][2] = 2;
                                            Points[i][2] = 2;
                                            Points[j][2] = 2;
                                            figure++;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < 6; j++) {
            if ( (Figures[i][j][0] == 0) && (Figures[i][j][1] == 0)) zeroCounter++;
        }
        if (zeroCounter == 6) {
            delete[] Figures[i];
            zeroCounter = 0;
            q++;
        }
    }
    CountFigures(Figures, Points, rows, figure, protocol, itog);
}
void CountFigures(float*** Figures, float** Points, int rows, int figure, ofstream& protocol, ofstream& itog) {
    protocol << "\n\n----------------------- Функция CountFigures начала работу! -----------------------\n";
    for (int i = 0; i < figure; i++) { // фигурыы Из массива
        Figures[i][6][0] += 1;
        protocol << "\nРассматривается фигура " << i + 1 << " с координатами "<<
        '('<<Figures[i][0][0] << ',' << Figures[i][0][1]<< ") (" <<Figures[i][1][0] << ',' << Figures[i][1][1]<< ") ("
        <<Figures[i][2][0] << ',' << Figures[i][2][1]<< ") (" <<Figures[i][3][0] << ',' << Figures[i][3][1]<< ") ("
        <<Figures[i][4][0] << ',' << Figures[i][4][1]<< ") (" << Figures[i][5][0] << ',' << Figures[i][5][1]<< ')' << "\n\n\n";
        itog << "\nРассматривается фигура " << i + 1 << " с координатами "<< '('<<Figures[i][0][0]
        << ',' << Figures[i][0][1]<< ") (" <<Figures[i][1][0] << ',' << Figures[i][1][1]<< ") (" <<Figures[i][2][0] << ','
        << Figures[i][2][1]<< ") (" <<Figures[i][3][0] << ',' << Figures[i][3][1]<< ") (" <<Figures[i][4][0] << ',' <<
        Figures[i][4][1]<< ") (" << Figures[i][5][0] << ',' << Figures[i][5][1]<< ')' << "\n\n\n";

        for (int j = 0; j < rows; j++) {
            protocol << "Рассматривается точка " << Points[j][0] << ',' << Points[j][1] << '\n'; // точки из массива
            if (Figures[i][0][0] == Figures[i][1][0]) { // X1 = X2 параллельность осям
                if ( (Points[j][0] >= Figures[i][0][0]) && (Points[j][0] <= Figures[i][2][0]) && (Points[j][1] >= Figures[i][0][1]) && (Points[j][1] <= Figures[i][1][1]) ) {
                    protocol << " Точка (" << Points[j][0] << ',' << Points[j][1] << ") лежит в прямоугольнике, стороны которого параллельны осям!\n";
                    if ((pow(((Figures[i][1][1] - Figures[i][0][1])/2),2) == (pow((Points[j][0] - Figures[i][4][0]),2) + pow((Points[j][1] - Figures[i][4][1]),2) )) || (pow(((Figures[i][3][1] - Figures[i][2][1]) / 2),2) == (pow((Points[j][0] - Figures[i][5][0]),2) + pow((Points[j][1] - Figures[i][5][1]),2))) || ( (pow(((Figures[i][3][1] - Figures[i][2][1]) / 2),2) < (pow((Points[j][0] - Figures[i][5][0]),2) + pow((Points[j][1] - Figures[i][5][1]),2))) && (pow(((Figures[i][1][1] - Figures[i][0][1])/2),2) < (pow((Points[j][0] - Figures[i][4][0]),2) + pow((Points[j][1] - Figures[i][4][1]),2) )))) {
                    protocol << " Точка (" << Points[j][0] << ',' << Points[j][1] << ") лежит в фигуре! " << i + 1 << '\n';
                    itog << " Точка (" << Points[j][0] << ',' << Points[j][1] << ") лежит в фигуре! " << i + 1 << '\n';
                    Figures[i][6][1] += 1;
                    }
                }
            }
            else {
                if ((((Points[j][0] - Figures[i][0][0])/(Figures[i][1][0] - Figures[i][0][0])) <= ((Points[j][1] - Figures[i][0][1]) / (Figures[i][1][1] - Figures[i][0][1])) ) && (((Points[j][0] - Figures[i][0][0])/(Figures[i][2][0] - Figures[i][0][0])) <= ((Points[j][1] - Figures[i][0][1]) / (Figures[i][2][1] - Figures[i][0][1]))) && (((Points[j][0] - Figures[i][2][0]) / (Figures[i][3][0] - Figures[i][2][0])) >= ((Points[j][1] - Figures[i][2][1]) / (Figures[i][3][1] - Figures[i][2][1])) ) && ( ((Points[j][0] - Figures[i][3][0]) / (Figures[i][1][0] - Figures[i][3][0])) <= ((Points[j][1] - Figures[i][3][1]) / (Figures[i][1][1] - Figures[i][3][1]) ))) {
                    protocol << " Точка (" << Points[j][0] << ',' << Points[j][1] << ") лежит в прямоугольнике1!\n"; 

                    if ( ((((pow((Figures[i][1][0] - Figures[i][0][0]), 2)) + (pow((Figures[i][1][1] - Figures[i][0][1]),2)))/4) == ((pow((Points[j][0] - Figures[i][4][0]), 2 )) + (pow((Points[j][1] - Figures[i][4][1]), 2))) ) || ( ( ((pow((Figures[i][3][0] - Figures[i][2][0]), 2)) + (pow((Figures[i][3][1] -  Figures[i][2][1]),2))) / 4) == ((pow((Points[j][0] - Figures[i][5][0]), 2)) + (pow((Points[j][1] - Figures[i][5][1]), 2))) ) || ((((pow((Figures[i][1][0] - Figures[i][0][0]), 2)) + (pow((Figures[i][1][1] - Figures[i][0][1]),2)))/4) < ((pow((Points[j][0] - Figures[i][4][0]), 2 )) + (pow((Points[j][1] - Figures[i][4][1]), 2))) ) && ( ( ((pow((Figures[i][3][0] - Figures[i][2][0]), 2)) + (pow((Figures[i][3][1] - Figures[i][2][1]),2))) / 4) < ((pow((Points[j][0] - Figures[i][5][0]), 2)) + (pow((Points[j][1] - Figures[i][5][1]), 2))) ) ){
                        protocol << " Точка (" << Points[j][0] << ',' << Points[j][1] << ") лежит в фигуре! " << i + 1 << '\n';
                        itog << " Точка (" << Points[j][0] << ',' << Points[j][1] << ") лежит в фигуре! " << i + 1 << '\n';
                        Figures[i][6][1] += 1;
                    }
                }
            }
        }
    }
    protocol << "\n\n----------------------- Функция CountFigures закончила работу! -----------------------\n";
    MaxPoints(Figures, figure, itog);
}
void MaxPoints(float*** Figures, int figure, ofstream& itog){
    int k = 0;
    int** ItogFigures = new int*[figure];
    for (int i = 0; i < figure; i++) {
        ItogFigures[i] = new int[2];
    }
    int max = 0;
    int tmp = 0;
    for(int i = 0; i < figure; i++) { // Определение фигуры с максимальным количеством точек
        tmp = Figures[i][6][1];
        if(tmp > max) {
            max = tmp;
        }
    }
    for(int i = 0; i < figure; i++) {
        if((Figures[i][6][1] == max) && (max != 4)) {
            itog << "В фигуре "<< i + 1 << " с координатами: "<< '('<<Figures[i][0][0] << ',' << Figures[i][0][1]<< ") (" <<Figures[i][1][0] << ',' << Figures[i][1][1]<< ") (" <<Figures[i][2][0] << ',' << Figures[i][2][1]<< ") (" <<Figures[i][3][0] << ',' << Figures[i][3][1]<< ") (" <<Figures[i][4][0] << ',' << Figures[i][4][1]<< ") (" << Figures[i][5][0] << ',' << Figures[i][5][1]<< ')' << " обнаружено самое большое число точек - "<< max - 4 << "\n\n\n";
            ItogFigures[k][0] = i; // номер фигуры
            ItogFigures[k][1] = max; // число точек в ней
            k++;
        }
    }
    if (max == 4) itog << "\nВ фигурах не обнаружено точек";
    for (int i = 0; i < figure; i++) {
    delete [] ItogFigures[i];
    }
}