#include <iostream>
#include <vector>
#include <cstdlib>
#include <fstream>

using namespace std;

const int ATTRIBUTES_CNT = 17;
const int CLASSIFIERS_CNT = 10;
const int PROBABILITIES_CNT = ((ATTRIBUTES_CNT - 1) * 3 + 1);

struct NaiveBayesClassifier {
    vector<vector<char> > data;
    vector<vector<double> > probabilities;
    NaiveBayesClassifier() {
        probabilities = vector<vector<double> > (PROBABILITIES_CNT, vector<double>(2 , 0.0));
    }
};

int readData(vector<NaiveBayesClassifier> &sets, string fileName) {
    ifstream file;
    file.open(fileName);
    if (!file) {
        cerr << "error! cannot open the file" << endl;
        exit(1);
    }
    
    int beforeLastElement = ATTRIBUTES_CNT - 2;
    string str;
    int dataSize = 0;
    if (file.good()){
        while (!getline(file, str, ',').eof()) {
            ++dataSize;
            vector<char> attr(ATTRIBUTES_CNT); 
            for (int i = 0; i < ATTRIBUTES_CNT; ++i) {
                if (str == "'y'") {
                    attr[i] = 'y';
                } else if (str == "'n'") {
                    attr[i] = 'n';
                } else if (str == "?") {
                    attr[i] = '?';
                } else if (str == "'republican'") {
                    attr[i] = 'r';
                } else if (str == "'democrat'") {
                    attr[i] = 'd';
                } else {
                    cerr << "There is a spelling mistake in the file" << endl;
                    exit(1);
                }
                if (i < beforeLastElement) {
                    getline(file, str, ',');
                } else if (i == beforeLastElement) {
                    getline(file, str, '\n');
                }
            }

            short a = rand() % CLASSIFIERS_CNT;
            sets[a].data.push_back(attr);

            bool isRepublican = (attr[beforeLastElement + 1] == 'r');
            for (int i = 0; i < beforeLastElement; ++i) {
                if (attr[i] == 'y') {
                    for (int m = 0; m < 10; ++m) {
                        if (m == a) {
                            continue;
                        }
                        sets[m].probabilities[i * 3][isRepublican] += 1;
                    }
               } else if (attr[i] == 'n') {
                    for (int m = 0; m < 10; ++m) {
                        if (m == a) {
                            continue;
                        }
                        sets[m].probabilities[(i * 3) + 1][isRepublican] += 1;
                    }
               } else {
                    for (int m = 0; m < 10; ++m) {
                        if (m == a) {
                            continue;
                        }
                        sets[m].probabilities[(i * 3) + 2][isRepublican] += 1;
                    }
               }

            }

            for (int m = 0; m < 10; ++m) {
                if (m != a) {
                    sets[m].probabilities[17][isRepublican] += 1;
                }
            }
            
        }
    }

    file.close();
    return dataSize;
}

void makeTables(vector<NaiveBayesClassifier> &sets) {
    
    for (int n = 0; n < 10; ++n) {
        for (int i = 0; i < (PROBABILITIES_CNT - 1); i += 3) {
            
            bool zeroProbability = false;
            for (int j = 0; j < 2; ++j) {
                int del = 0;
                for (int k = 0; k < 3; ++k) {
                    zeroProbability = zeroProbability || (!sets[n].probabilities[i + k][j]);
                    del += sets[n].probabilities[i + k][j];

                }
                del += zeroProbability;
                for (int k = 0; k < 3; ++k) {
                    sets[n].probabilities[i + k][j] = (sets[n].probabilities[i + k][j] + zeroProbability) / del;
                }

            }
        }
    }
}

int democratOrRepublican(vector<NaiveBayesClassifier> sets) {
    int trueAnswers = 0;
    for (int i = 0; i < 10; ++i) {
        
        for (int dataNumber = 0; dataNumber < sets[i].data.size(); ++dataNumber) {
            double probRepublican = sets[i].probabilities[16][1];
            double probDemocrat = sets[i].probabilities[16][0];
            for (int j = 0; j < 16; ++j) {
                char c = sets[i].data[dataNumber][j];
                if (c == 'y') {
                    probRepublican *= sets[i].probabilities[(j * 3)][1];
                    probDemocrat *= sets[i].probabilities[(j * 3)][0];
                } else if (c == 'n') {
                    probRepublican *= sets[i].probabilities[(j * 3) + 1][1];
                    probDemocrat *= sets[i].probabilities[(j * 3) + 1][0];
                } else {
                    probRepublican *= sets[i].probabilities[(j * 3) + 1][1];
                    probDemocrat *= sets[i].probabilities[(j * 3) + 2][0];
                }
            }
            
            bool trueAnswer = (probRepublican > probDemocrat && sets[i].data[dataNumber][16] == 'r');
            if (trueAnswer || (probRepublican < probDemocrat && sets[i].data[dataNumber][16] == 'd')) {
                ++trueAnswers;
            }
                
            
        }

    }
    return trueAnswers;

}

int main() {
    srand(time(NULL));
    vector<NaiveBayesClassifier> sets(10);
    int dataSize = readData(sets, "testData.txt");
    makeTables(sets);
    int trueAnswers = democratOrRepublican(sets);
    //cout << "trueAnswers " << trueAnswers << endl;
    cout << (double)trueAnswers / (double)dataSize << endl;
    return 0;
}
