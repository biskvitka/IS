#include <iostream>
#include <cmath>
#include <vector>
#include <fstream>
#include <algorithm>

#define RND ((double)rand() / RAND_MAX)

using namespace std;

int myRandom(int i) { return rand() % i; }


struct Chromo {
    Chromo(int dimc) { 
        vector<bool> v(dimc);
        items = v;
    }

    ~Chromo() {
    }

    Chromo(const Chromo &chromo) {
        this->items = chromo.items;
        this->f = chromo.f;
    }

    Chromo& operator=(const Chromo &ch1) {
        this->items = ch1.items;
        this->f = ch1.f;
        return *this;
    }

    void mutate(const int dimc, const int count) {
        int mi; 
        for (int i = 0; i <  count; ++i) {
            mi = (int)(round((rand()/RAND_MAX) * (dimc - 1)));
            items[mi] = !items[mi];
        }
    }

    vector<bool> items;
    int f;
};

void findBackpackPrice(const vector<bool> &x, const vector<int> &v, const vector<int> &w) {
    int price = 0;
    int weight = 0;
    for (int i = 0; i < v.size(); ++i) {
        price += x[i] * v[i];
        weight += x[i] * w[i];
    }
    cout << "weight: " << weight << endl;
    cout << "price: " << price << endl;
}

int fitness(vector<bool> &x, const int dimc, const vector<int> &v, const vector<int> &w, const int limit) {
    int fit = 0;
    int wsum = 0;
    for (int i = 0; i < dimc; ++i) {
        wsum += x[i] * w[i];
        fit += x[i] * v[i];
    }
    if (wsum > limit) {
        fit -= 7 * (wsum - limit); //penalty for invalid solutions
    }
    return fit;
}

void crossover1p(const Chromo &c1, const Chromo &c2, Chromo &c3, const int dimc, const int cp) {
    for (int i = 0; i < dimc; ++i) {
        if (i < cp) {
            c3.items[i] = c1.items[i];
        } else {
            c3.items[i] = c2.items[i];
        }
    }
}

void crossover1p_b(const Chromo &c1, const Chromo &c2, Chromo &c3, const int dimc, int cp) {
    for (int i = 0; i < dimc; ++i) {
        if (i > cp) {
            c3.items[i] = c2.items[i];
        } else {
            c3.items[i] = c1.items[i];
        }
    }
}


void crossoverRand(const Chromo &c1, const Chromo &c2, Chromo &c3, const int dimc) {
    for (int i = 0; i < dimc; ++i) {
        if (round(RND)) {
            c3.items[i] = c1.items[i];
        } else {
            c3.items[i] = c2.items[i];
        }
    }
}

void crossoverArit(const Chromo &c1, const Chromo &c2, Chromo &c3, const int dimc) {
    for (int i = 0; i < dimc; ++i) {
        c3.items[i] = (c1.items[i] ^ c2.items[i]);
    }
}

bool cfit(const Chromo &c1, const Chromo &c2) {
    return c1.f > c2.f;
}

bool cmpFunc (const pair<int, double> &r1, const pair<int, double> &r2) {
    return r1.second > r2.second;
}

int coin(const double crp) {
    if (RND < crp) {
        return 1;
    }
    return 0;
}

void initpopg(vector<vector<bool> > &c, const vector<int> &w, const vector<int> v, const int dimw, const int limit, const int pop) {
    vector<pair<int, double> > rvals(dimw);
    for (int i = 0; i < dimw; ++i) {
        rvals.push_back(pair<int, double>(make_pair(i, (double)v[i] / (double)w[i])));
    }

    sort(rvals.begin(), rvals.end(), cmpFunc);
    int currentw = 0;
    //int populationWithoutOne = pop - 1;
    int k;
    //for (int i = 0; i < dimw; ++i) {
    //    k = rvals[i].first;
    //    if (currentw + w[k] <= limit) {
    //        currentw += w[k];
    //        cout <<"hi " <<  currentw << endl;
    //        c[populationWithoutOne][k] = 1;
    //   } else {
    //        c[populationWithoutOne][k] = 0;
    //    }
    //}

    for (int i = 0; i < pop; ++i) {
        currentw = 0;
        for (int j = 0; j < dimw; ++j) {
            k = rand() % 2;
            if (k && (currentw + w[j]) <= limit) {
                currentw += w[j];
                c[i][j] = 1;
            } else {
                c[i][j] = 0;
            }
        }
    }
}

int main() {

    srand(time(NULL));
    fstream testFile("testData.txt", ios_base::in);
    vector<int> v, w;
    int limit;
    testFile >> limit;
    cout << limit << endl;
    int itemsCount;
    testFile >> itemsCount;
    cout << "items count: " << itemsCount << endl;
    int possibleItems = itemsCount;
    int cost;
    int weight;
    // push all the items that are not heavier than the capacity of the knapsack
    for (int i = 0; i < itemsCount; ++i) {
        testFile >> cost;
        testFile >> weight;
        if (weight > limit) {
            --possibleItems;
        } else {
            v.push_back(cost);
            w.push_back(weight);
        }
    }

    const int population = (int)(itemsCount);
    const int generations = itemsCount * 2;
    const int discarded = (int)(ceil(population * 0.3));
    int best, ind, ind2, parentChromosome;
    best = ind = ind2 = parentChromosome = 0;
    int repetitionBest = 0;
    double crossoverProbability = 0.65;
    
    vector<Chromo> ch(population, Chromo(possibleItems));
    vector<vector<bool> >c(population, vector<bool>(possibleItems));

    clock_t start = clock();
    initpopg(c, w, v, possibleItems, limit, population);   
    int fit = fitness(ch[0].items, possibleItems, v, w, limit);
    for (int i = 0; i < population; ++i) {
        ch[i].items = c[i];
        ch[i].f = fit; 
    }

    for (int p = 0; p < generations; ++p) {

        for (int i = 0; i < population; ++i) {
            if (i > population - discarded) {
                if (coin(crossoverProbability)) {
                    ind = parentChromosome + round(discarded * RND);
                    ind2 = parentChromosome + 1 + round(discarded * RND);

                    crossover1p(ch[(ind % population)], ch[(ind2 % population)], ch[i], possibleItems, round(RND * (possibleItems - 1)));
                    ch[i].f =  fitness(ch[i].items, possibleItems, v, w, limit);
                    ++parentChromosome;
                } else {
                    ch[i].mutate(possibleItems, 1);
                    ch[i].f = fitness(ch[i].items, possibleItems, v, w, limit);
                }
            }
            if (ch[i].f > best) {
                best = ch[i].f;
                repetitionBest = 0;
            }
        }
        sort(ch.begin(), ch.end(), cfit);
        if (p == 10 || ((p % 25 ) == 0) || ((p + 1) == generations)) {
            findBackpackPrice(ch[0].items, v, w);
        } else if (++repetitionBest >= 25) {
            findBackpackPrice(ch[0].items, v, w);
            break;
        }
    }
    clock_t end = clock();
    cout << (double)(end - start) / CLOCKS_PER_SEC << endl;

    return 0;
}
