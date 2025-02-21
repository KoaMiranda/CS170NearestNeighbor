#include <iostream>
#include <set> //for current set of features
#include <algorithm> //to do an intersection for the above
#include <vector> //to hold the data matrix
#include <fstream> //for txt puller
#include <sstream> //same
#include <ctime> //for rand
#include <cstdlib> //same
using namespace std;

//README 1
vector<vector<double>> txtPuller()
{
    ifstream file("CS170_Small_Data__23.txt");
    vector<vector<double>> returnData; //self-explanatory
    string entry; //temp holding string

    while (getline(file, entry)) //while were pulling txt
    {
        stringstream ss(entry); //extractor thingy
        vector<double> fileRow; //accumulates the values
        double value; //the value

        while (ss >> value) //extractor does its thing
        {
            fileRow.push_back(value); //load all the values into the row
        }
        returnData.push_back(fileRow); //load the row into the matrix
    }
    file.close();

    return returnData;
} 

//see if we can shorten these 
double leaveOneOutCross(vector<vector<double>> data, set<int> currentSetOfFeatures, int nextOne)
{
    srand(time(0));
    //random num stub
    return rand()%100 + 1;
}

//README 2
void featureSearch(vector<vector<double>> data)
{
    set<int> currentSetOfFeatures = {};

    //data[0]-1 is number of columns -1 bc class label
    for (int i=0; i<(data[0].size()-1); ++i)
    {
        cout << "On the " << i << "th level of the search tree\n";
        int featureToAdd = 0;
        int bestSoFar = 0;
        
        for (int j=0; j<(data[0].size()-1); ++j)
        {   
            //did the iterator make it all the way to the end w/out finding?
            if (currentSetOfFeatures.find(j) == currentSetOfFeatures.end())
            {
                cout <<"--Considering adding the " << j << "th feature\n";
                double accuracy = leaveOneOutCross(data, currentSetOfFeatures, j+1);

                if (accuracy > bestSoFar)
                {
                    bestSoFar = accuracy;
                    featureToAdd = j;
                }
            }
        }
        currentSetOfFeatures.insert(featureToAdd);
        cout << "On level " << i << ", added feature " << featureToAdd << " to the current set\n";
        cout << "Best Accuracy we got: " << bestSoFar << '\n';
    }
}

int main()
{
    //stuff
    vector<vector<double>> testdata = txtPuller();
    featureSearch(testdata);
    return 0;
}