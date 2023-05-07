#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <math.h>
#include <limits>
#include <chrono>
#include <pthread.h>
using namespace std;

#define NO_OF_PARAMS 385
#define K 5
#define N 300

int num_threads = 5;
int max_iter = 10;
pthread_mutex_t centroid_mutex;
struct Stock
{
    vector<double> parameters = vector<double>(NO_OF_PARAMS, 0);
    int cluster;
    Stock()
    {
        cluster = -1;
    }
};

vector<Stock> stocks;
vector<Stock> centroids;
void DataRead(string fileName)
{
    ifstream infile(fileName);
    vector<vector<double>> data;
    string line;
    stocks.resize(N);
    if (!infile)
    {
        cout << "File not found" << endl;
        exit(0);
    }

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
    for (int i = 0; i < N; i++)
    {
        stocks[i].parameters = data[i];
    }
    // count number of rows
    int No = data.size();
    cout << "Number of rows: " << No << endl;
    // count number of each columns
    int M = data[100].size();
    cout << "Number of columns: " << M << endl;
}
Stock addTwo(const Stock &a, const Stock &b)
{
    Stock c;
    for (int i = 0; i < NO_OF_PARAMS; i++)
    {
        c.parameters[i] = a.parameters[i] + b.parameters[i];
    }
    return c;
}
Stock divideTwo(const Stock &stock1, const int count)
{
    Stock c;
    for (int i = 0; i < NO_OF_PARAMS; i++)
    {
        c.parameters[i] = stock1.parameters[i] / static_cast<double>(count);
    }
    return c;
}
void mean_recompute()
{
    int count[K] = {0};
    Stock sum[K] = {Stock()};
    for (int i = 0; i < N; i++)
    {
        count[stocks[i].cluster]++;
        sum[stocks[i].cluster] = addTwo(sum[stocks[i].cluster], stocks[i]);
    }
    for (int i = 0; i < K; i++)
    {
        centroids[i] = divideTwo(sum[i], count[i]);
    }
}

double computeDistance(const Stock a, const Stock b)
{
    double distance = 0.0;
    for (int i = 0; i < NO_OF_PARAMS; i++)
    {
        distance += pow(a.parameters[i] - b.parameters[i], 2);
    }
    return distance;
}
void AssignClusters()
{
    vector<double> distances(K, 0);
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < K; j++)
        {
            distances[j] = computeDistance(stocks[i], centroids[j]);
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
    for (int i = start; i < end; i++)
    {
        pthread_mutex_lock(&centroid_mutex);
        int min = INT_MAX;
        int index = -1;
        for (int j = 0; j < K; j++)
        {
            double dist = computeDistance(stocks[i], stocks[j]);
            if (dist < min)
            {
                index = i;
            }
        }
        stocks[i].cluster = index;
        pthread_mutex_unlock(&centroid_mutex);
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
        int old[N] = {0};
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
    } while (itreation != max_iter);
}

int main(int argc, char const *argv[])
{
    string fileName = "StockData.csv";
    chrono::duration<double> elapsed;
    chrono::high_resolution_clock::time_point start = chrono::high_resolution_clock::now();
    DataRead(fileName);
    Kmeans_pthread();
    cout << endl
         << endl;
    ofstream myfile;
    myfile.open("output.csv");

    ofstream file("output2.csv");
    for (int i = 0; i < K; i++)
    {
        cout << " ---------------------- Cluster ---------------------- " << i << endl;
        for (int j = 0; j < N; j++)
        {
            if (stocks[j].cluster == i)
            {
                // Print Names
                file << stocks[j].parameters[0] << ", " << stocks[j].parameters[1] << ", " << stocks[j].parameters[2] << ", " << stocks[j].parameters[3] << ", " << stocks[j].parameters[4] << ", " << stocks[j].cluster << endl;
                cout << stocks[j].parameters[0] << endl;
            }
        }
    }
    myfile.close();
    chrono::high_resolution_clock::time_point end = chrono::high_resolution_clock::now();
    elapsed = end - start;
    cout << "Time taken by function: " << elapsed.count() << " seconds" << endl;

    return 0;
}