#include <iostream>
#include <set> //for current set of features
#include <algorithm> //to do an intersection for the above
#include <vector> //to hold the data matrix
#include <fstream> //for txt puller
#include <sstream> //same
#include <ctime> //for rand
#include <cstdlib> //same
#include <limits> //for nearesst neighbor "inifinity"
using namespace std;

//README 1
vector<vector<double>> txtPuller(string& txtfile)
{
    ifstream file(txtfile);
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

//README 2; see if we can shorten these
double leaveOneOutCross(vector<vector<double>>& data, set<int>& currentSetOfFeatures, int nextOne)
{
    double numberCorrect = 0; //for return accuracy

    //current features we will be testing...
    set<int> features = currentSetOfFeatures;
    //...and the next feature we will be testing
    features.insert(nextOne);

    for (int i=0; i<(data.size()); ++i)
    {
        //for each row i, construct new vector with just the features
        vector<double>& objectToClassify = data[i]; //first col of ith row
        int objectLabel = data[i][0];

        //arbitrarily large temp value
        double nearestNeighborDist = numeric_limits<double>::infinity();
        double nearestNeighborLoc = -1;
        double nearestNeighborLabel = -1;

        for (int j=0; j<(data.size()); ++j)
        {
            if (j == i) continue; //dont compare with self

            double dist=0; //euclidean distance 

            //measure distance for each feature (index) in the objects 
            for (int feature : features)
            {
                //holds value to make math look cleaner/readable
                double temp = (objectToClassify[feature] - data[j][feature]);
                dist += (temp * temp);
            }

            dist = sqrt(dist);
            
            if (dist < nearestNeighborDist)
            {
                nearestNeighborDist = dist;
                nearestNeighborLoc = j;
                nearestNeighborLabel = data[nearestNeighborLoc][0]; 
            }
        }
        /*
        cout << "Object " << i+1 << " is class " << objectLabel <<'\n';
        cout << "It's nearest_neighbor is " << nearestNeighborLoc << " which is in class " << nearestNeighborLabel << '\n';
        */

        //test the resulting accuracy
        if (objectLabel == nearestNeighborLabel) numberCorrect++;
    }
    return (double)numberCorrect / data.size();
}

//README 2
void featureSearch(vector<vector<double>>& data)
{
    set<int> currentSetOfFeatures = {};

    //data[0]-1 is number of columns -1 bc class label
    for (int i=0; i<(data[0].size()-1); ++i)
    {
        cout << "On the " << i << "th level of the search tree\n";
        int featureToAdd = 0;
        double bestSoFar = -1;
        
        for (int j=0; j<(data[0].size()-1); ++j)
        {   
            //did the iterator make it all the way to the end w/out finding?
            if (currentSetOfFeatures.find(j) == currentSetOfFeatures.end())
            {
                cout <<"--Considering adding the " << j << "th feature\n";
                double accuracy = leaveOneOutCross(data, currentSetOfFeatures, j+1);
                cout << accuracy << '\n';
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
    string filename{};
    cout << "Enter file name: ";
    cin >> filename;
    vector<vector<double>> testdata = txtPuller(filename);
    featureSearch(testdata);
    return 0;
}