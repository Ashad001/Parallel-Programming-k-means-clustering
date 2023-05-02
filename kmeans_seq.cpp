#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <math.h>
#include <limits>
using namespace std;


// Stock movements and Volumes
class Stock
{
    public:
    // string name;
    double movement;
    double volume;
    int cluster;
    Stock(double movement = 0.f, double volume = 0.f)
    {
        // this->name = name;
        this->movement = movement;
        this->volume = volume;
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

    KMeans(string fileName,  int K, int N, int max_iter)
    {
        this->K = K;
        this->N = N;
        this->max_iter = max_iter;
        DataRead(fileName);
    }
    void DataRead(string fileName)
    {
        ifstream file(fileName);
        string line;
        while(getline(file, line))
        {
            stringstream ss(line);
            string comma;
            double movement;
            long double volume;
            ss >> movement >> comma >> volume;
            Stock stock(movement, volume);
            stocks.push_back(stock);
        }
    }
    void mean_recompute()
    {
        int count[K];
        Stock sum[K];
        for(int i = 0; i < this->N; i++)
        {
            count[stocks[i].cluster]++;
            sum[stocks[i].cluster] = addTwo(sum[stocks[i].cluster], stocks[i]);
        }
        for(int i = 0; i < this->K; i++)
        {
            centroids[i].movement = sum[i].movement / count[i];
            centroids[i].volume = sum[i].volume / count[i];
        }

    }
    Stock addTwo(Stock a, Stock b)
    {
        Stock c;
        c.movement = a.movement + b.movement;
        c.volume = a.volume + b.volume;
        return c;
    }
    void AssignClusters()
    {
        vector<double> distances(this->K, 0);
        for(int i = 0; i < N; i++)
        {
            for(int j = 0; j < this->K; j++)
            {
                distances[j] = EuclideanDistance(stocks[i], centroids[j]);
            }
            int index = 0;
            for (int i = 1; i < this->K; i++)
            {
                if (distances[i] < distances[index])
                {
                    index = i;
                }
            }
            stocks[i].cluster = index;
        }
    }
    double EuclideanDistance(Stock a, Stock b)
    {
        double x = a.movement - b.movement;
        double y = a.volume - b.volume;
        return sqrt(x*x + y*y);
    }
    void Run()
    {
        // Initialize centroids
        for(int i = 0; i < this->K; i++)
        {
            centroids.push_back(stocks[i]);
        }
        for(int i = 0; i < this->max_iter; i++)
        {
            AssignClusters();
            mean_recompute();
        }
    }
    // print points by clusters in a table
    void Print()
    {
        for(int i = 0; i < this->K; i++)
        {
            cout << "Cluster " << i << endl;
            for(int j = 0; j < this->N; j++)
            {
                if(stocks[j].cluster == i)
                {
                    cout << " ==> " <<   stocks[j].movement << " " << stocks[j].volume << endl;
                }
            }
        }
    }
};
int main(int argc, char const *argv[])
{
    string fileName = "normalized_data.csv";
    int K = 5;
    int N = 500;
    int max_iter = 10;
    KMeans kmeans(fileName, K, N, max_iter);
    kmeans.Run();
    kmeans.Print();

    
    return 0;
}

