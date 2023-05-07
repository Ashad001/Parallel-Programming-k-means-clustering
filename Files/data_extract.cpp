#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>
#include <sstream>
#include <unistd.h>
#include <stdlib.h>
#include <numeric>
#include <cmath>

using namespace std;

vector<double> normalize(vector<double> vec) {
    vector<double> normalizedVec(vec.size());
    double vecMagnitude = 0.0;
    
    for (int i = 0; i < vec.size(); i++) {
        vecMagnitude += vec[i] * vec[i];
    }
    vecMagnitude = sqrt(vecMagnitude);
    
    for (int i = 0; i < vec.size(); i++) {
        normalizedVec[i] = vec[i] / vecMagnitude;
    }
    
    return normalizedVec;
}

int main(int argc, char const *argv[])
{
    ifstream fin;
    ofstream fout1;
    ofstream fout2;
    string line;
    vector<double> open_values;
    vector<double> close_values;
    vector<double> volumes;

    fin.open("BigData.csv");    
    if(!fin.is_open())
    {
        cout << "Error opening file" << endl;
        exit(EXIT_FAILURE);
    }    
    while(getline(fin, line))
    {
        stringstream ss(line);
        string token;
        int count = 0;
        while(getline(ss, token, ','))
        {
            if(count == 2)
            {
                open_values.push_back(stof(token));
            }
            else if(count == 3)
            {
                close_values.push_back(stof(token));
            }
            else if(count == 4)
            {
                volumes.push_back(stof(token));
            }
            count++;
        }
    }
    fin.close();
    cout << open_values.size() << " " << close_values.size() << endl;
    for(int i = 0; i < open_values.size(); i++)
    {
        fout1 << close_values[i] - open_values[i] << ", " << volumes[i] << endl;
        // fout2 << i << ", " <<  open_values[i] - close_values[i] << ", " << volumes[i] << endl;
    }
    fout1.close();
    fout2.close();

    // Find max and min
    // double max = open_values[0] - close_values[0];
    // double min = open_values[0] - close_values[0];
    // for(int i = 0; i < open_values.size(); i++)
    // {
    //     if(open_values[i] - close_values[i] > max)
    //     {
    //         max = open_values[i] - close_values[i];
    //     }
    //     if(open_values[i] - close_values[i] < min)
    //     {
    //         min = open_values[i] - close_values[i];
    //     }
    // }
    // cout << "Max: " << max << endl;
    // cout << "Min: " << min << endl;

    double max = volumes[0];
    double min = volumes[0];
    for(int i = 0; i < volumes.size(); i++)
    {
        if(volumes[i] > max)
        {
            max = volumes[i];
        }
        if(volumes[i] < min)
        {
            min = volumes[i];
        }
    }
    cout << "Max: " << max << endl;
    cout << "Min: " << min << endl;


    vector<double> profits;
    for(int i = 0; i < open_values.size(); i++)
    {
        profits.push_back(close_values[i] - open_values[i]);
    }
    
    volumes = normalize(volumes);

    fout1.open("normalized_data.csv");

    for(int i = 0; i < profits.size(); i++)
    {
        fout1 << profits[i] << ", " << volumes[i] << endl;
    }




    return 0;
}
