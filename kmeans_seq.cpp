#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <math.h>
using namespace std;
class Kmeans
{
    string fileName;
    int K; // number of clusters
    int maxIterations ;
    vector<double> profits; 
    vector<double> centroids; // centroids of clusters
    vector<double> clusters; // clusters of data points

public:
    Kmeans(string filename, int k)
    {
        this->fileName = filename;
        this->K = k;
        this->maxIterations = 100;
        FileRead();
        for(int i = 0; i < this->K; i++)
        {
            this->centroids.push_back(this->profits[i]); // initialize centroids with first K elements
        }
        for(int i = 0; i < this->profits.size(); i++)
        {
            this->clusters.push_back(-1); // initialize clusters with -1
        }
    }
    void FileRead()
    {
        ifstream file(this->fileName);
        string line;
        if (!file.is_open())
        {
            cout << "Unable to open file" << endl;
            return;
        }
        while (std::getline(file, line)) {
            // cout << line << endl;
            double number = stof(line);
            this->profits.push_back(number);
        }
        file.close();
        // for(int i = 0; i < this->profits.size(); i++)
        // {
        //     cout << this->profits[i] << endl;
        // }
    }
    void CalculateCentroids()
    {
        for(int i = 0; i < this->K; i++)
        {
            double sum = 0;
            int count = 0;
            for(int j = 0; j < this->clusters.size(); j++)
            {
                if(this->clusters[j] == i)
                {
                    sum += this->profits[j];
                    count++;
                }
            }
            this->centroids[i] = sum / count;
        }
    }
    void CalculateClusters()
    {
        // Run for maxIterations
        for(int i = 0; i < this->maxIterations; i++)
        {
            // Run for all data points
            for(int j = 0; j < this->profits.size(); j++)
            {
                double minDistance = INT8_MAX;
                int cluster = -1;
                // Run for all centroids
                for(int k = 0; k < this->K; k++)
                {
                    double distance = abs(this->profits[j] - this->centroids[k]);
                    if(distance < minDistance)
                    {
                        minDistance = distance;
                        cluster = k;
                    }
                }
                this->clusters[j] = cluster;   
            }
            // Update Centroids
            CalculateCentroids();
        }
    }
    void ShowClusters()
    {  
        for (int i = 0; i < K; i++)
        {
            cout << "Cluster " << i << ": ";
            for (int j = 0; j < profits.size(); j++)
            {
                if (clusters[j] == i)
                {
                    cout << "- " << profits[j] << endl ;
                }
            }
            cout << endl;
        }
    }
    void run()
    {
        CalculateClusters();
        ShowClusters();
    }
    void SaveInData()
    {
        ofstream fout;
        fout.open("newData.csv");
        for(int i = 0; i < this->profits.size(); i++)
        {
            fout << this->profits[i] << ", " << this->clusters[i] << endl;
        }
        fout.close();
    }
    
};


int main(int argc, char const *argv[])
{
    Kmeans kmeans("data.csv", 5);
    // kmeans.CalculateClusters();
    // kmeans.ShowClusters();
    kmeans.run();
    kmeans.SaveInData();


    return 0;
}
