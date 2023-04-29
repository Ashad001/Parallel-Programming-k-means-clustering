#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>
#include <sstream>
#include <unistd.h>
#include <stdlib.h>
using namespace std;

void normalize(std::vector<std::vector<double>>& data)
{
    // Calculate mean and standard deviation for each column
    std::vector<double> means(data[0].size(), 0);
    std::vector<double> std_devs(data[0].size(), 0);
    for (int col = 0; col < data[0].size(); col++)
    {
        double sum = 0;
        double sq_sum = 0;
        for (int row = 0; row < data.size(); row++)
        {
            sum += data[row][col];
        }
        means[col] = sum / data.size();
        for (int row = 0; row < data.size(); row++)
        {
            sq_sum += (data[row][col] - means[col]) * (data[row][col] - means[col]);
        }
        std_devs[col] = sqrt(sq_sum / data.size());
        // cout << "Mean: " << means[col] << " Std_dev: " << std_devs[col] << endl;
    }

    // Normalize each column
    for (int col = 0; col < data[0].size(); col++)
    {
        for (int row = 0; row < data.size(); row++)
        {
            data[row][col] = (data[row][col] - means[col]) / std_devs[col];
        }
    }
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

    fin.open("lastest_data.csv");    
    fout1.open("data.csv");
    fout2.open("display.csv");
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
        fout1 << open_values[i] - close_values[i] << ", " << volumes[i] << endl;
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
    
    std::vector<std::vector<double>> data;
    for (int i = 0; i < open_values.size(); i++)
    {
        std::vector<double> row = {open_values[i] - close_values[i], volumes[i]};
        data.push_back(row);
    }


    // print data
    cout << "Data: " << data[0].size() << ", " << data.size()  << endl;
    // for(int i = 0; i < data.size(); i++)
    // {
    //     for(int j = 0; j < data[0].size(); j++)
    //     {
    //         cout << data[i][j] << ", ";
    //     }
    //     cout << endl;
    // }


    normalize(data);
    fout1.open("normalized_data.csv");
    for(int i = 0; i < open_values.size(); i++)
    {
        fout1 << data[0][i]  << ", " << data[1][i] << endl;
    }
    fout1.close();




    return 0;
}
