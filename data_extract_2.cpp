#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <cmath>

using namespace std;

struct Stock
{
    vector<double> parameters;
    int cluster;
} stocks[399];

vector<double> normalize(vector<double> vec)
{
    vector<double> normalizedVec(vec.size());
    double vecMagnitude = 0.0;

    for (int i = 0; i < vec.size(); i++)
    {
        vecMagnitude += vec[i] * vec[i];
    }
    vecMagnitude = sqrt(vecMagnitude);

    for (int i = 0; i < vec.size(); i++)
    {
        normalizedVec[i] = vec[i] / vecMagnitude;
    }

    return normalizedVec;
}

void DataRead()
{
    ifstream infile("BigData.csv");
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
    for (int i = 0; i < data.size(); i++)
    {
        stocks[i].parameters = data[i];
    }
    // print stock
    for (int i = 0; i < 399; i++)
    {
        for (int j = 0; j < 30; j++)
        {
            cout << stocks[i].parameters[j] << " ";
        }
        cout << endl;
    }

    // count number of rows 
    int N = data.size();
    cout << "Number of rows: " << N << endl;
    // count number of each columns
    int M = data[100].size();
    cout << "Number of columns: " << M << endl;
}
int main(int argc, char const *argv[])
{
    DataRead();

    return 0;
}
