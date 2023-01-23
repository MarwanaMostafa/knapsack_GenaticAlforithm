#include <cstdlib>
#include <bits/stdc++.h>
#include <iostream>
#include <vector>
using namespace std;
// Population size
#define POPULATION_SIZE 10000
#define NUMBEROFGENERATIONS 100
ofstream MyFile("10000.txt");
#define PC 0.7
#define PM 0.01

int max(int a, int b)
{
    return (a > b) ? a : b;
}
int knapSack(int sizeKnapSack, vector<int> wei, vector<int> val)
{
    int i, w;
    vector<vector<int>> Solutions(val.size() + 1, vector<int>(sizeKnapSack + 1));
    for (i = 0; i <= val.size(); i++)
    {
        for (w = 0; w <= sizeKnapSack; w++)
        {
            if (i == 0 || w == 0)
                Solutions[i][w] = 0;
            else if (wei[i - 1] <= w)
                Solutions[i][w] = max(val[i - 1] +
                                          Solutions[i - 1][w - wei[i - 1]],
                                      Solutions[i - 1][w]);
            else
                Solutions[i][w] = Solutions[i - 1][w];
        }
    }
    return Solutions[val.size()][sizeKnapSack];
}

void initializePopulation(vector<vector<int>> &pop, int sizegene) // size of gene represent flag for each item so numberOfItems=sizeOfGene
{
    int sizeGene = sizegene;
    for (int i = 0; i < POPULATION_SIZE; i++)
    {
        vector<int> temp;
        for (int j = 0; j < sizeGene; j++)
            temp.push_back(rand() % 2); // 0,1 taken ot no
        pop.push_back(temp);
    }
}

void readValAndWei(int elementsOFTESTCASE, ifstream &ReadFile, vector<int> &values, vector<int> &weights)
{
    string readLine;
    int elementsOfTestCase = elementsOFTESTCASE;
    for (int i = 0; i < elementsOfTestCase; i++)
    {
        getline(ReadFile, readLine);
        int pos = readLine.find(" ");
        values.push_back(stoi(readLine.substr(pos + 1)));
        weights.push_back(stoi(readLine.substr(0, pos)));
    }
}

int calculateFitness(int target, vector<pair<int, int>> &Fitness, vector<int> Values, vector<vector<int>> Pop, vector<int> Weights, int Size)
{
    int summationVal = 0;
    int summationWeight = 0;
    Fitness.clear();
    int i;
    for (i = 0; i < Pop.size(); i++)
    {
        for (int j = 0; j < Values.size(); j++)
        {
            summationVal += Values[j] * Pop[i][j];
            summationWeight += Weights[j] * Pop[i][j];
        }
        // cout << summationVal << endl;
        if (summationVal == target)
            return i;
        else if (summationWeight <= Size)
            Fitness.push_back({summationVal, i});
        else                           // Handling infeasible solutions
            Fitness.push_back({1, i}); // we give infeasible soultion less weight because may produce good solution
        summationVal = 0;
        summationWeight = 0;
    }
    return -1;
}

void rankSelection(vector<pair<int, int>> Fitness, int &index1, int &index2)
{
    vector<float> rankChromosome;
    int lenFitness = Fitness.size();
    int summationRank = lenFitness * (lenFitness + 1) / 2;
    for (int i = 0; i < lenFitness; i++)
        rankChromosome.push_back((i + 1) / summationRank);
    index1 = -1;
    index2 = -1;
    //  this function so we have in first index less weigth
    float x = (float(rand()) / float(RAND_MAX));
    if (x < rankChromosome[0])
        index1 = Fitness[0].second;

    for (int i = 1; i < Fitness.size(); i++)
    {
        x = (float(rand()) / float(RAND_MAX));
        if (rankChromosome[i - 1] <= x && x < rankChromosome[i])
            index1 = Fitness[0].second;
    }
    x = (float(rand()) / float(RAND_MAX));
    if (x < rankChromosome[0])
        index2 = Fitness[0].second;
    for (int i = 1; i < Fitness.size(); i++)
    {
        x = (float(rand()) / float(RAND_MAX));
        if (rankChromosome[i - 1] <= x && x < rankChromosome[i])
            index2 = Fitness[0].second;
    }
}
void mutation(vector<vector<int>> offsprings, int index1, int index2)
{
    int i = index1;
    for (int counter = 0; counter < 2; counter++)
    {
        for (int j = 0; j < offsprings[0].size(); j++)
        {
            float r2 = (float)rand() / RAND_MAX;
            if (r2 <= PM)
            {
                // cout << "DONE MUTATION : " << r2 << endl;
                if (offsprings[i][j] == 0)
                    offsprings[i][j] = 1; // change meaning because if bit less than 0.5 means we will not take this bit else we will take this bit
                else
                    offsprings[i][j] = 0; // change meaning because if bit less than 0.5 means we will not take this bit else we will take this bit
            }
            r2 = 0.0;
        }
        i = index2;
    }
}

void crossOverAndMutation(vector<vector<int>> &pop, int index1, int index2)
{
    int cuttingPoint = rand() % (pop[0].size() - 1); // length of chromosome-1
    float r2 = (float)rand() / RAND_MAX;
    if (r2 < PC)
    {
        for (int j = cuttingPoint; j < pop[0].size(); j++)
            swap(pop[index1][j], pop[index2][j]);

        mutation(pop, index1, index2);
    }
}
void replacement(vector<pair<int, int>> FitnessParents, vector<pair<int, int>> FitnessOffSprings, vector<vector<int>> &pop, vector<vector<int>> offSprings)
{
    for (int i = 0; i < 2; i++)
        if (FitnessOffSprings[i].first > FitnessParents[FitnessOffSprings[i].second].first)
            pop[FitnessOffSprings[i].second] = offSprings[i];
}

int getOptimalSolution(vector<int> Values, vector<int> Weights, vector<vector<int>> Pop, int Size)
{
    int summationVal = 0;
    int summationWeight = 0;
    int tempSummationVal = 0;
    int index = 0;
    int i;
    for (i = 0; i < POPULATION_SIZE; i++)
    {
        for (int j = 0; j < Values.size(); j++)
        {
            summationVal += Values[j] * Pop[i][j];
            summationWeight += Weights[j] * Pop[i][j];
        }
        if (summationWeight <= Size && summationVal > tempSummationVal)
        {
            tempSummationVal = summationVal;
            index = i;
        }
        summationVal = 0;
        summationWeight = 0;
    }
    return index;
}
int main()
{
    srand(time(NULL));
    ifstream MyReadFile("knapsack_input.txt");
    string readLine = "";
    getline(MyReadFile, readLine);
    int numberOfTestCase = stoi(readLine);

    for (int i = 0; i < numberOfTestCase; i++)
    {
        getline(MyReadFile, readLine);
        if (readLine == "") // because input file has many empty line
        {
            i--;
            continue;
        }
        int sizeKnapSack = stoi(readLine);
        getline(MyReadFile, readLine);
        int lengthOfGene = stoi(readLine);

        // read data for vector Values and Weights
        vector<int> Values;
        vector<int> Weights;
        readValAndWei(lengthOfGene, MyReadFile, Values, Weights);

        // we solve problem first by alogrithm knapsack to know best soltuion
        int Target = knapSack(sizeKnapSack, Weights, Values);

        // Initialization step
        vector<vector<int>> pop;
        vector<pair<int, int>> Fitness;
        initializePopulation(pop, lengthOfGene);
        vector<vector<int>> selectedChromosomes;
        vector<pair<int, int>> fitnessSelectedChromosome;
        int indexSelected1, indexSelected2;
        int flag = 0; // define if we get best solution or no ,and contain index best soultion if found
        for (int i = 0; i < NUMBEROFGENERATIONS; i++)
        {
            cout << "T " << i << endl;
            Fitness.clear();
            selectedChromosomes.clear();
            fitnessSelectedChromosome.clear();
            flag = calculateFitness(Target, Fitness, Values, pop, Weights, sizeKnapSack);
            if (flag != -1) // if found best soultion exit from loop
            {
                int numberOfSelectedItems = 0;

                for (int i = 0; i < pop[0].size(); i++)
                    if (pop[flag][i] == 1)
                        numberOfSelectedItems++;

                MyFile << "Number Of Selected Items Is : " << numberOfSelectedItems << " TOTAL VALUE  IS : " << Target;
                for (int j = 0; j < pop[0].size(); j++)
                {
                    if (pop[flag][j] == 0)
                        continue;
                    MyFile << " Item Is " << Values[j] << " And Weight Is " << Weights[j];
                }
                MyFile << endl;
                break;
            }
            sort(Fitness.begin(), Fitness.end()); // sort according to Values
            rankSelection(Fitness, indexSelected1, indexSelected2);
            if (indexSelected1 == -1 || indexSelected2 == -1) // if we select <= 1 chromosome
                continue;
            selectedChromosomes.push_back(pop[indexSelected1]);
            selectedChromosomes.push_back(pop[indexSelected2]);
            crossOverAndMutation(pop, indexSelected1, indexSelected2);
            calculateFitness(Target, fitnessSelectedChromosome, Values, selectedChromosomes, Weights, sizeKnapSack);
            replacement(Fitness, fitnessSelectedChromosome, pop, selectedChromosomes);
        }
        if (flag == -1)
        {
            int flag = getOptimalSolution(Values, Weights, pop, sizeKnapSack);
            int numberOfSelectedItems = 0;
            for (int i = 0; i < pop[0].size(); i++)
                if (pop[flag][i] == 1)
                    numberOfSelectedItems++;

            int totalValue = 0;
            for (int j = 0; j < Values.size(); j++)
                totalValue += Values[j] * pop[flag][j];

            MyFile << "Number Of Selected Items Is : " << numberOfSelectedItems << " TOTAL VALUE  IS : " << totalValue;
            for (int j = 0; j < pop[0].size(); j++)
            {
                if (pop[flag][j] == 0)
                    continue;
                MyFile << " Item Is " << Values[j] << " And Weight Is " << Weights[j];
            }
            MyFile << endl;
        }
    }
    MyReadFile.close();
    MyFile.close();
}