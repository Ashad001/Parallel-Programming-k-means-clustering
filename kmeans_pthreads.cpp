#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <math.h>
#include <limits>
#include <pthread.h>
using namespace std;

int K = 5;
int N = 500;
int num_threads=100;
int max_iter = 10;
pthread_mutex_t centroid_mutex;
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
        this->cluster = -1;
    }
};

vector<Stock> stocks;
vector<Stock> centroids;
void DataRead(string fileName)
{
    ifstream file(fileName);
    string line;
    while (getline(file, line))
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
Stock addTwo(Stock a, Stock b)
{
    Stock c;
    c.movement = a.movement + b.movement;
    c.volume = a.volume + b.volume;
    return c;
}
void mean_recompute()
{
    int count[K] = {0};
    Stock sum[K] = {Stock(0, 0)};
    for (int i = 0; i < N; i++)
    {
        count[stocks[i].cluster]++;
        sum[stocks[i].cluster] = addTwo(sum[stocks[i].cluster], stocks[i]);
    }
    for (int i = 0; i < K; i++)
    {
        centroids[i].movement = sum[i].movement / count[i];
        centroids[i].volume = sum[i].volume / count[i];
    }
}
double EuclideanDistance(Stock a, Stock b)
{
    double x = a.movement - b.movement;
    double y = a.volume - b.volume;
    return sqrt(x * x + y * y);
}
void AssignClusters()
{
    vector<double> distances(K, 0);
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < K; j++)
        {
            distances[j] = EuclideanDistance(stocks[i], centroids[j]);
        }
        int index = 0;
        for (int y = 1; y < K; y++)
        {
            if (distances[y] < distances[index])
            {
                index = y;
            }
        }
        stocks[i].cluster = index;
    }
}
void *thread_runner(void *tid)
{
    int *t_id = (int *)tid;
    int start = (N * (*t_id)) / num_threads;
    int end = (N * ((*t_id) + 1)) / num_threads;
    if (*t_id == num_threads - 1)
    {
        end = N;
        for (int i = start; i < end; i++)
        {
            int min = 9999999;
            int index = -1;
            for (int j = 0; j < K; j++)
            {
                double dist = EuclideanDistance(stocks[i], stocks[j]);
                if (dist < min)
                {
                    index = i;
                }
            }
            pthread_mutex_lock(&centroid_mutex);
            stocks[i].cluster = index;
            pthread_mutex_unlock(&centroid_mutex);
        }
    }
    return NULL;
}
void Kmeans_pthread()
{
    int t = num_threads;
    pthread_t threads[t];
    int *tid = new int[t];
    for (int i = 0; i < K; i++)
    {
        centroids.push_back(stocks[i]);
    }
    int itreation = 0;
    int count;
    do
    {
        AssignClusters();
        mean_recompute();
        int old[N];
        for (int i = 0; i < N; i++)
        {
            old[i] = stocks[i].cluster;
        }
        pthread_mutex_init(&centroid_mutex, NULL);
        for (int i = 0; i < t; i++)
        {
            tid[i] = i;
            pthread_create(&threads[i], NULL, thread_runner, &tid[i]);
        }
        for (int i = 0; i < t; i++)
        {
            pthread_join(threads[i], NULL);
        }
        itreation++;
        count = 0;
        for (int i = 0; i < N; i++)
        {
            if (old[i] == stocks[i].cluster)
                count++;
        }
    } while (itreation!=max_iter);
}

int main(int argc, char const *argv[])
{
    string fileName = "normalized_data.csv";
    DataRead(fileName);
    Kmeans_pthread();
    cout  << endl << endl;
    ofstream myfile;
    myfile.open("output.csv");
    for (int i = 0; i < K; i++)
    {
        cout << "Cluster " << i << endl;
        for (int j = 0; j < N; j++)
        {
            if (stocks[j].cluster == i)
            {
                myfile << i << ", " << stocks[j].movement << ", " << stocks[j].volume << endl;
                cout << " ==> " << stocks[j].movement << " " << stocks[j].volume << endl;
            }
        }
    }
    return 0;
}