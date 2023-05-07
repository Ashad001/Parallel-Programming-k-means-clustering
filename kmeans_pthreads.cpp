#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <math.h>
#include <limits>
#include <chrono>
using namespace std;
#define NO_OF_PARAMS 385

struct Stock
{
    vector<double> parameters = vector<double>(NO_OF_PARAMS, 0);
    int cluster;
    Stock()
    {
        cluster = -1;
    }
};

class KMeans
{
public:
    int K;
    int N;
    int max_iter;
    vector<Stock> stocks;
    vector<Stock> centroids;

    KMeans(string fileName, int K, int N, int max_iter)
    {
        this->K = K;
        this->N = N;
        this->max_iter = max_iter;
        this->stocks.resize(N);
        DataRead(fileName);

        // initialize centroids randomly
        for (int i = 0; i < K; i++)
        {
            int idx = rand() % N;
            centroids.push_back(stocks[idx]);
        }
    }

    void DataRead(string fileName)
    {
        ifstream infile(fileName);
        vector<vector<double>> data;
        string line;

        // skip the first line
        getline(infile, line);

        while (getline(infile, line))
        {
            vector<double> row;
            stringstream ss(line);
            string value_str;
            while (getline(ss, value_str, ','))
            {
                double value = stod(value_str);
                row.push_back(value);
            }
            data.push_back(row);
        }
        // put data in stocks
        for (int i = 0; i < this->N ; i++)
        {
            stocks[i].parameters = data[i];
        }
        // print stock
        for (int i = 0; i < this->N; i++)
        {
            for (int j = 0; j < NO_OF_PARAMS; j++)
            {
                // cout << stocks[i].parameters[j] << " ";
            }
            // cout << endl;
        }

        // count number of rows
        int N = data.size();
        cout << "Number of rows: " << N << endl;
        // count number of each columns
        int M = data[100].size();
        cout << "Number of columns: " << M << endl;
    }
    void mean_recompute()
    {
        int count[K] = {0};
        Stock sum[K] = {Stock()};
        for (int i = 0; i < this->N; i++)
        {
            count[stocks[i].cluster]++;
            sum[stocks[i].cluster] = addTwo(sum[stocks[i].cluster], stocks[i]);
        }
        for(int i = 0; i < this->K; i++)
        {
            centroids[i] = divideTwo(sum[i], count[i]);
        }
    }
    Stock divideTwo(const Stock &stock1, const int count)
    {
        Stock c;
        for(int i = 0; i < NO_OF_PARAMS; i++)
        {
            c.parameters[i] = stock1.parameters[i] / static_cast<double>(count);
        }
        return c;
    }
    Stock addTwo(const Stock &a, const Stock &b)
    {
        Stock c;
        for(int i = 0; i < NO_OF_PARAMS; i++)
        {
            c.parameters[i] = a.parameters[i] + b.parameters[i];
        }
        return c;
    }
    void AssignClusters()
    {
        vector<double> distances(this->K, 0);
        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < this->K; j++)
            {
                distances[j] = computeDistance(stocks[i], centroids[j]);
            }
            int index = 0;
            for (int k = 1; k < this->K; k++)
            {
                if (distances[k] < distances[index])
                {
                    index = k;
                }
            }
            stocks[i].cluster = index;
        }
    }
    double computeDistance(const Stock a, const Stock b)
    {
        double distance = 0.0;
        for(int i = 0; i < NO_OF_PARAMS; i++)
        {
            distance += pow(a.parameters[i] - b.parameters[i], 2);
        }
        return distance;
    }
    void run()
    {
        // Initialize centroids randomly
        for (int i = 0; i < this->K; i++)
        {
            centroids.push_back(stocks[i]);
        }
        for (int i = 0; i < this->max_iter; i++)
        {
            AssignClusters();
            mean_recompute();
        }
    }

    // print points by clusters in a table
    void Print()
    {
        ofstream file("output.csv");
        for (int i = 0; i < this->K; i++)
        {
            cout << "Cluster " << i << endl;
            for (int j = 0; j < this->N; j++)
            {
                if (stocks[j].cluster == i)
                {
                    // Print Names
                    file << stocks[j].parameters[0] << ", " << stocks[j].parameters[1] << ", " << stocks[j].parameters[2] << ", " << stocks[j].parameters[3] << ", " << stocks[j].parameters[4] << ", " << stocks[j].cluster << endl;
                    cout << stocks[j].parameters[0] << endl;
                }
            }
        }
    }
};
int main(int argc, char const *argv[])
{
    string fileName = "StockData.csv";
    int K = 5;
    int N = 300;
    int max_iter = 500;
    chrono::duration<double> elapsed;
    auto start = chrono::high_resolution_clock::now();
    KMeans kmeans(fileName, K, N, max_iter);
    kmeans.run();
    kmeans.Print();
    auto finish = chrono::high_resolution_clock::now();
    elapsed = finish - start;
    cout << "Elapsed time: " << elapsed.count() << " s\n";

    return 0;
}
