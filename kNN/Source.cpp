#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <queue>
#include <utility>

using namespace std;

class Data {
    private:
        double first;
        double second;
        double third;
        double fourth;
        short irisType;

    public:

        Data(double first, double second, double third, double fourth) {
            this -> first  = first;
            this -> second = second;
            this -> third  = third;
            this -> fourth = fourth;
        }

        Data(double first, double second, double third, double fourth, short irisType) {
            this -> first    = first;
            this -> second   = second;
            this -> third    = third;
            this -> fourth   = fourth;
            this -> irisType = irisType;
        }

        short getIrisType() {
            return irisType;
        }

        double getFirst() {
            return first;
        }

        double getSecond() {
            return second;
        }

        double getThird() {
            return third;
        }

        double getFourth() {
            return fourth;
        }

        void setIrisName(short irisType) {
            this -> irisType = irisType;
        }

        Data(const Data& data) {
            this -> first    = data.first;
            this -> second   = data.second;
            this -> third    = data.third;
            this -> fourth   = data.fourth;
            this -> irisType = data.irisType;
        }

        Data& operator=(const Data& data) {
            this -> first    = data.first;
            this -> second   = data.second;
            this -> third    = data.third;
            this -> fourth   = data.fourth;
            this -> irisType = data.irisType;
            return *this;
        }

        Data& operator=(Data&& data) {
            this -> first    = move(data.first);
            this -> second   = move(data.second);
            this -> third    = move(data.third);
            this -> fourth   = move(data.fourth);
            this -> irisType = move(data.irisType);
            return *this;
        }



        friend ostream& operator<<(ostream& os, const Data& data);

};

ostream& operator<<(ostream& os, const Data& data) {
    os << data.first << ", ";
    os << data.second << ", ";
    os << data.third << ", ";
    os << data.fourth << ", ";
    os << data.irisType << endl;
    return os;
}

short getIrisType(string irisName) {
    short irisType;
    if (irisName == "Iris-setosa") {
        irisType = 1;
    } else if (irisName == "Iris-versicolor") {
        irisType = 2;
    } else {
        irisType = 3;
    }
    return irisType;
}

void readData(string fileName, vector<Data>& trainSet, vector<Data> &testSet, int testSize) {

    ifstream file;
    file.open(fileName);
    if (!file) {
        cerr << "error! cannot open the file" << endl;
        exit(1);
    }

    string str;
    double first;
    double second;
    double third;
    double fourth;
    string irisName;

    if (file.good()){
        while (!getline(file, str, ',').eof()) {
            first = stod(str);                      
            getline(file, str, ',');
            second = stod(str);                                                   
            getline(file, str, ',');
            third = stod(str);
            getline(file, str, ',');
            fourth = stod(str);                                   
            getline(file, irisName);
            Data data(first, second, third, fourth, getIrisType(irisName));
            if (((rand() % 60) % 5) == 0 && testSet.size() < testSize) {
                testSet.push_back(data);
            } else {
                trainSet.push_back(data);
            }

        }
    }

    file.close();
}

struct compare {
    bool operator()(pair<double, int>& data1, pair<double, int>& data2) {
        return data1.first > data2.first;
    }
};

double findDistance(Data& a, Data& b) {
    double firstArgDiff = a.getFirst() - b.getFirst();
    double secondArgDiff = a.getSecond() - b.getSecond();
    double thirdArgDiff = a.getThird() - b.getThird();
    double fourthArgDiff = a.getFourth() - b.getFourth();
    double distance;
    distance = sqrt(pow(firstArgDiff, 2) + pow(secondArgDiff, 2) +
        + pow(thirdArgDiff, 2) + pow(fourthArgDiff, 2));
    return distance;
}


int getAnswerIrisType(int k, Data& data, vector<Data>& dataSet) {
    priority_queue<pair<double, int>, vector<pair<double, int>>, compare> priorityData;
    int distance;
    for(auto& currentData : dataSet) {
        double distance = findDistance(data, currentData);
        priorityData.push(make_pair(distance, currentData.getIrisType()));
    }
    vector<int> classes(3, 0);
    for (int i = 0; i < k; ++i) {
        pair<double, int> neighbour = priorityData.top();
        priorityData.pop();
        switch (neighbour.second) {
        case 1:
            classes[0]++;
            break;
        case 2:
            classes[1]++;
            break;
        case 3:
            classes[2]++;
        }
    }   
    
    pair<int, int> max(1, classes[0]);
    for (int i = 1; i < 3; ++i) {
        if (max.second < classes[i]) {
            max = make_pair(i + 1, classes[i]);
        }
    }
    return max.first;
}


int main() {
    srand(time(NULL));
    //string test = "5.4,3.9,1.7,0.4,Iris-setosa";
    int testSetSize = 20;
    vector<Data> trainSet;
    vector<Data> testSet;
    readData("./iris.data", trainSet, testSet, testSetSize);
    
    
    cout << "test set; size: " << testSet.size() << '\n';
    for (auto testData : testSet) {
        cout << testData;
    }

    int k = 5;
    int truePredictions = 0;
    for (int i = 0; i < testSetSize; i++) {
        int predicted = getAnswerIrisType(k, testSet[i], trainSet);
        cout << "predicted: " << predicted << " real: " << testSet[i].getIrisType() << endl;
        if (predicted == testSet[i].getIrisType()) {
            truePredictions++;
        }
    }
    double successRate = truePredictions / (double) testSetSize;
    cout << "Success rate: " << successRate << '\n';
          
    return 0;
}
