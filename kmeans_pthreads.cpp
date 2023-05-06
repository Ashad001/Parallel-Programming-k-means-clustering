#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <math.h>
#include <limits>
#include <pthread.h>
using namespace std;

// Stock movements and Volumes
class Stock
{
public:
    // string name;
    double movement;
    double volume;
    int cluster;
    int count;
    Stock(double movement = 0.f, double volume = 0.f)
    {
        // this->name = name;
        this->movement = movement;
        this->volume = volume;
        this->cluster = -1;
        this->count = 0;
    }
};
//to store the thread data
struct thread_parameter
{
    int id;
    int noofthreads;
    int N;
    int K;
    vector<Stock> stocks;
    vector<Stock> centroids;
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
        DataRead(fileName);
    }
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

    //a function to run each thread
    static void *thread_runner(void *arg)
    {
        thread_parameter *t_args = (thread_parameter *)arg;
        int id = t_args->id;
        int noofthreads = t_args->noofthreads;
        //since function is static in class, need to initialize different variables for each thread
        int thread_K = t_args->K;
        int total = t_args->N; 
        vector<Stock> s = t_args->stocks;
        vector<Stock> c = t_args->centroids;
        
        //a mutex declaration for parallel calculation of clusters and avoid race condition
        pthread_mutex_t centroid_mutex[thread_K];
        //checking start and end of every thread
        int start = (total * id) / noofthreads;
        int end = (total * (id + 1)) / noofthreads;

        int count[thread_K] = {0};
        Stock sum[thread_K] = {Stock(0, 0)};
        //initializing mutex
        for (int i = 0; i < thread_K; i++)
        {
            pthread_mutex_init(&centroid_mutex[i], NULL);
        }

        for (int i = start; i < end; i++)
        {

            double min_dist = numeric_limits<double>::max();
            int cluster_check = -1;
            for (int j = 0; j < thread_K; j++)
            {
                //calculation euclidean distance for each cluster
                double dist = sqrt(pow(s[i].movement - c[j].movement, 2) +
                                   pow(s[i].volume - c[j].volume, 2));
                //minimum distance is greater than change cluster
                if (dist < min_dist)
                {
                    min_dist = dist;
                    cluster_check = j;
                }
            }
            s[i].cluster = cluster_check;
            count[cluster_check]++;
            Stock temp_sum;
            temp_sum.movement = sum[cluster_check].movement + s[i].movement;
            temp_sum.volume = sum[cluster_check].volume + s[i].volume;
            sum[cluster_check] = temp_sum;
        }

        for (int i = 0; i < thread_K; i++)
        {
            pthread_mutex_lock(&centroid_mutex[i]);
            if (count[i] == 0)
            {
                c[i] = c[i];
            }
            else
            {
                double avg_movement = sum[i].movement / count[i];
                double avg_volume = sum[i].volume / count[i];
                Stock new_stock(avg_movement, avg_volume);
                c[i] = new_stock;
            }

            pthread_mutex_unlock(&centroid_mutex[i]);
        }

        return 0;
    }

    void mean_recompute()
    {
        int num_threads = N / K;
        thread_parameter t_parameter[num_threads];

        for (int i = 0; i < num_threads; i++)
        {
            t_parameter[i].id = i;
            t_parameter[i].noofthreads = num_threads;
            t_parameter[i].N = N;
            t_parameter[i].K = K;
            t_parameter[i].stocks = stocks;
            t_parameter[i].centroids = centroids;
            pthread_t thread;
            pthread_create(&thread, NULL, thread_runner, (void *)&t_parameter[i]);
            pthread_join(thread, NULL);
        }
    }
    void AssignClusters()
    {
        vector<double> distances(this->K, 0);
        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < this->K; j++)
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
        return sqrt(x * x + y * y);
    }
    void Run()
    {
        // Initialize centroids
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
        for (int i = 0; i < this->K; i++)
        {
            cout << "Cluster " << i << endl;
            for (int j = 0; j < this->N; j++)
            {
                if (stocks[j].cluster == i)
                {
                    cout << " ==> " << stocks[j].movement << " " << stocks[j].volume << endl;
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