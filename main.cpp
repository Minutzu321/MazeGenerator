#include <iostream>
#include <vector>
#include <cstdlib>

using namespace std;

class Celula {

public:
    Celula();
    bool vizitat;
    bool stangaPerete;
    bool dreaptaPerete;
    bool josPerete;
    bool susPerete;
    int x;
    int y;
};

Celula::Celula() {
    vizitat = false;
    stangaPerete = true;
    dreaptaPerete = true;
    josPerete = true;
    susPerete = true;
}

class Labirint {

public:
    int rowNum;
    int columnNum;
    int getX(Celula cel);
    int getY(Celula cel);
    vector<vector<Celula>> matrix;
    void  genMatrix ();
    bool isValid(int x, int y);
    Celula dreaptaMergi(int x, int y);
    Celula stangaMergi(int x, int y);
    Celula susMergi(int x, int y);
    Celula josMergi(int x, int y);
    vector<Celula> veciniPosibili(int x, int y, bool path);
    bool gasesteDrum(int fromX, int fromY, int toX, int toY);
    void spargePerete(Celula currentCelula, Celula nextCelula);
    void genLabirint(int x, int y);
    void genLabirint();
    void printLabirint();
};

int Labirint::getX(Celula cel)  {
    return cel.x;
}

int Labirint::getY(Celula cel)  {
    return cel.y;
}

void Labirint::genMatrix ()
{
    vector<Celula> row (columnNum);
    vector <vector<Celula> > new_matrix;

    for(int i= 0; i < rowNum; i++) {
        new_matrix.push_back(row);
    }

    for(int i = 0; i < rowNum; i++) {
        for(int j = 0; j < columnNum; j++) {
            Celula cel;
            cel.x = i;
            cel.y = j;
            new_matrix.at(i).at(j) = cel;
        }
    }



    this->matrix = new_matrix;
}

bool Labirint::isValid(int x, int y) {
    return x >= 0 && x< rowNum && y >= 0 && y < columnNum;
}

Celula Labirint::dreaptaMergi(int x, int y) {
    int nextX = x;
    int nextY = y + 1;
    return matrix.at(nextX).at(nextY);
}

Celula Labirint::stangaMergi(int x, int y) {
    int nextX = x;
    int nextY = y - 1;
    return matrix.at(nextX).at(nextY);
}

Celula Labirint::susMergi(int x, int y) {
    int nextX = x - 1;
    int nextY = y;
    return matrix.at(nextX).at(nextY);
}

Celula Labirint::josMergi(int x, int y) {
    int nextX = x + 1;
    int nextY = y;
    return matrix.at(nextX).at(nextY);
}

vector<Celula> Labirint::veciniPosibili(int x, int y, bool path) {

    vector<Celula> vecini;

    if(path){
        Celula cel = matrix.at(x).at(y);
        if(isValid(x,y+1) &&(!cel.dreaptaPerete || !dreaptaMergi(x, y).stangaPerete) && !dreaptaMergi(x, y).vizitat){
            vecini.push_back(dreaptaMergi(x, y));
        }
        if(isValid(x,y-1) && (!cel.stangaPerete || !stangaMergi(x, y).dreaptaPerete) && !stangaMergi(x, y).vizitat) {
            vecini.push_back(stangaMergi(x, y));
        }
        if(isValid(x-1,y) && (!cel.susPerete || !susMergi(x, y).josPerete) && !susMergi(x, y).vizitat) {
            vecini.push_back(susMergi(x, y));
        }
        if(isValid(x+1,y) && (!cel.josPerete || !josMergi(x, y).susPerete) && !josMergi(x, y).vizitat) {
            vecini.push_back(josMergi(x, y));
        }

        return vecini;
    }


    if(isValid(x,y+1) && !dreaptaMergi(x, y).vizitat) {
        vecini.push_back(dreaptaMergi(x, y));
    }

    if(isValid(x,y-1) && !stangaMergi(x, y).vizitat) {
        vecini.push_back(stangaMergi(x, y));
    }

    if(isValid(x-1,y) && !susMergi(x, y).vizitat) {
        vecini.push_back(susMergi(x, y));
    }

    if(isValid(x+1,y) && !josMergi(x, y).vizitat) {
        vecini.push_back(josMergi(x, y));
    }
    if(vecini.size()>1)
        for (int i = 0; i <= rand()%vecini.size(); ++i) {
            int randomIndex = rand()%vecini.size();
            Celula el = vecini.at(i);
            vecini.at(i) = vecini.at(randomIndex);
            vecini.at(randomIndex) = el;
        }
    return vecini;
}

void Labirint::spargePerete(Celula currentCelula, Celula nextCelula) {

    if(nextCelula.x == currentCelula.x && nextCelula.y == currentCelula.y + 1 ) { 
        //verifica la dreapta
        currentCelula.dreaptaPerete = false;
        nextCelula.stangaPerete = false;
    }
    if(nextCelula.x == currentCelula.x && nextCelula.y == currentCelula.y - 1 ) {
        //verifica la stanga
        currentCelula.stangaPerete = false;
        nextCelula.dreaptaPerete = false;
    }
    if(nextCelula.x == currentCelula.x - 1 && nextCelula.y == currentCelula.y) {
        //verifica sus
        currentCelula.susPerete = false;
        nextCelula.josPerete = false;
    }
    if(nextCelula.x == currentCelula.x + 1 && nextCelula.y == currentCelula.y) {
        //verifica jos
        currentCelula.josPerete = false;
        nextCelula.susPerete = false;
    }

    matrix.at(currentCelula.x).at(currentCelula.y) = currentCelula;
    matrix.at(nextCelula.x).at(nextCelula.y) = nextCelula;
}

bool Labirint::gasesteDrum(int fromX, int fromY, int toX, int toY)
{
    Celula cel = this->matrix.at(fromX).at(fromY);
    cel.vizitat = true;
    this->matrix.at(fromX).at(fromY) = cel;
    if(fromX==toX && fromY==toY) {
        return true;
    }

    vector<Celula> vecini = veciniPosibili(fromX, fromY, true);

    for (int i = 0; i < vecini.size(); ++i) {
        int nextX = getX(vecini.at(i));
        int nextY = getY(vecini.at(i));



        if(gasesteDrum(nextX, nextY, toX, toY)) return true;
    }


    cel.vizitat = false;
    matrix.at(fromX).at(fromY) = cel;
    return false;
}

void Labirint::genLabirint(int x, int y){
    Celula currentCelula = matrix.at(x).at(y);
    currentCelula.vizitat = true;
    this->matrix.at(x).at(y) = currentCelula;
    vector<Celula> neighbors = veciniPosibili(x,y, false);
    for (int i = 0; i < neighbors.size(); ++i) {
        Celula neighbor = neighbors.at(i);
        if(!this->matrix.at(neighbor.x).at(neighbor.y).vizitat){
            spargePerete(currentCelula, neighbor);
            genLabirint(neighbor.x, neighbor.y);
        }
    }
}

void Labirint::genLabirint(){
    genLabirint(0,0);
    for (int i = 0; i < rowNum; ++i) {
        for (int j = 0; j < columnNum; ++j) {
            Celula cel = matrix.at(i).at(j);
            cel.vizitat = false;
            matrix.at(i).at(j) = cel;
        }
    }
}

void Labirint::printLabirint(){
    for (int i = 0; i < rowNum; i++) {
        for (int k = 0; k < 3; k++) {
            Celula cel;
            for(int j = 0; j < columnNum; j++) {
                cel = matrix.at(i).at(j);
                if (k==1){
                    if(cel.stangaPerete && (isValid(cel.x, cel.y-1) && stangaMergi(cel.x, cel.y).dreaptaPerete) || !isValid(cel.x, cel.y-1)) cout << "|";
                    else cout << " ";
                    if(cel.vizitat) cout << " . ";
                    else cout << "   ";
                    if(cel.y >= columnNum-1) cout << "|";
                }else if(k==0){
                    cout << "+";
                    if(cel.susPerete && (isValid(cel.x-1, cel.y) && susMergi(cel.x, cel.y).josPerete) || !isValid(cel.x-1, cel.y)) cout << "---";
                    else cout << "   ";
                    if(cel.y >= columnNum-1) cout << "+";
                } else if(cel.x >= rowNum-1) {
                    cout << "+" << "---";
                    if(cel.y >= columnNum-1) cout << "+";
                }
            }
            if(k!=2 || cel.x >= rowNum-1)
                cout<<endl;
        }
    }
}

int main(int argc, char **argv) {

    int rowNum, columnNum;

    if(argc >= 3) {

        rowNum = stoi(argv[1]);
        columnNum = stoi(argv[2]);

        if(argc==4)
            srand((unsigned) stoi(argv[3]));
        else
            srand(time(NULL));

        Labirint maze;
        maze.rowNum = rowNum;
        maze.columnNum = columnNum;
        maze.genMatrix();
        maze.genLabirint();
        maze.gasesteDrum(0,0,rowNum-1, columnNum-1);
        maze.printLabirint();

    } else{
        cout<<"input not valid";
    }
}
