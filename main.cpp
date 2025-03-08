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

//README 2/3; see if we can shorten these
double leaveOneOutCross(vector<vector<double>>& data, set<int>& currentSetOfFeatures, int nextOne)
{
    double numberCorrect = 0; //for return accuracy

    //current features we will be testing...
    set<int> features = currentSetOfFeatures;
    //...and the next feature we will be testing
    if (nextOne != -1) features.insert(nextOne); //if statement for BackwardElim compatibility

    for (int i=0; i<(data.size()); ++i)
    {
        //for each row i, construct new vector with just the features
        vector<double>& objectToClassify = data[i]; //first col of ith row
        int objectLabel = data[i][0]; //the actual correct class based on txt file

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
void featureSearchForward(vector<vector<double>>& data)
{
    set<int> bestCombo = {}; //track the best combination of features
    double bestComboAccuracy = -1.0; //Best accuracy we can get
    set<int> currentSetOfFeatures = {}; //the current combo were working on

    for (int i=0; i < data[0].size()-1; ++i)
    {
        currentSetOfFeatures.insert(i); //inserting them from txt
    }
    
    //leaveOneOut checks for -1, doesn't modify currentset. Baseline accuracy
    bestCombo = currentSetOfFeatures;
    bestComboAccuracy = leaveOneOutCross(data, bestCombo, -1);

    cout << "Running nearest neighbor with all 4 features, using 'leave-one-out' evaluation, I get an accuracy of" << bestComboAccuracy*100 << "%\n\n";

    currentSetOfFeatures = {};

    cout << "Beginning Search.\n\n";
    //data[0]-1 is number of columns -1 bc class label
    for (int i=0; i<(data[0].size()-1); ++i)
    {
        //cout << "On the " << i+1 << "th level of the search tree\n";
        int featureToAdd = 0;
        double bestSoFar = -1;
        
        for (int j=0; j<(data[0].size()-1); ++j)
        {   
            //did the iterator make it all the way to the end w/out finding?
            if (currentSetOfFeatures.find(j) == currentSetOfFeatures.end())
            {
                //cout <<"--Considering adding the " << j << "th feature\n";
                double accuracy = leaveOneOutCross(data, currentSetOfFeatures, j+1);
                cout << "\tUsing feature(s) { ";
                for (auto feature : currentSetOfFeatures)
                {
                    cout << feature+1 << ' ';
                }
                cout << j+1 << " } accuracy is " << accuracy*100 << "%\n";

                if (accuracy > bestSoFar)
                {
                    bestSoFar = accuracy;
                    featureToAdd = j;
                }
            }
        }
        currentSetOfFeatures.insert(featureToAdd);
        //cout << "On level " << i << ", added feature " << featureToAdd << " to the current set\n";
        //cout << "Best Accuracy we got: " << bestSoFar << '\n';

        cout << '\n';
        if (bestSoFar < bestComboAccuracy)
        {
            cout << "(Warning, Accuracy has decreased! Continue search in case of local maxima)\n";
        } 
        else if (bestSoFar > bestComboAccuracy) //this controls more than two features in results.
        {
            bestComboAccuracy = bestSoFar;
            bestCombo = currentSetOfFeatures;
        }
        cout << "Feature set { " << featureToAdd+1 <<" } was best, accuracy is " << bestSoFar*100 << "%\n";
    }

    cout << "\nFinished Search!! The best feature subset is { ";
    for (auto feature : bestCombo)
    {
        cout << feature+1 << ' ';
    }
    cout << "} which has an accuracy of " << bestComboAccuracy*100 << "%\n";
}

//README 5, mostly same as above
void featureSearchBackwards(vector<vector<double>>& data)
{
    set<int> bestCombo;
    double bestComboAccuracy = -1.0;
    set<int> currentSetOfFeatures; //now start with all possible features in currentSet
    for (int i=0; i < data[0].size()-1; ++i)
    {
        currentSetOfFeatures.insert(i); //inserting them from txt
    }
    
    //leaveOneOut checks for -1, doesn't modify currentset. Baseline accuracy
    bestCombo = currentSetOfFeatures;
    bestComboAccuracy = leaveOneOutCross(data, bestCombo, -1);

    cout << "Running nearest neighbor with all 4 features, using 'leave-one-out' evaluation, I get an accuracy of" << bestComboAccuracy*100 << "%\n\n";
    cout << "Beginning search.\n\n";
    for (int i=0; i<(data[0].size()-1); ++i)
    {
        //cout << "On the " << i+1 << "th level of the search tree\n";
        int featureToRemove = -1; //-1 instead of 0 cause we might accidentally remove 0
        double bestSoFar = -1;
        
        for (int j=0; j<(currentSetOfFeatures.size()-1); ++j)
        {   
            //try all features, figure out which one is the worst, -1 means dont change the current set
            set<int> reducedSet = currentSetOfFeatures; //self explanatory
            reducedSet.erase(j); //try it without this one

            //find the accuracy of the reduced set and print it
            double accuracy = leaveOneOutCross(data, reducedSet, -1);
            cout << "\tUsing feature(s) { ";
            for (auto feature : reducedSet) 
            {
                cout << feature+1 << ' ';
            }
            cout << j+1 << " } accuracy is " << accuracy*100 << "%\n";

            if (accuracy >= bestSoFar) //Greater than OR equal to in order to guarantee a feature gets removed every tree level
            {
                bestSoFar = accuracy;
                featureToRemove = j;
            }
        }
        cout << '\n';
        if (featureToRemove != -1)
        {
            currentSetOfFeatures.erase(featureToRemove); //get rid of the worst feature, submit new reduced set for next loop
            if (bestSoFar < bestComboAccuracy)
            {
                cout << "(Warning, Accuracy has decreased! Continue search in case of local maxima)\n";
            }
            cout << "Feature { " << featureToRemove+1 << " } removed, accuracy is now " << bestSoFar*100 << "%\n";

            if (bestSoFar >= bestComboAccuracy) //Again '>' OR '=' to ensure smallest subset
            {
                bestComboAccuracy = bestSoFar;
                bestCombo = currentSetOfFeatures;
            }
        }
    }
    cout << "Finished search!! The best feature subset is { ";
    for (auto feature : bestCombo)
    {
        cout << feature+1 << ' ';
    }
    cout << "} which has an accuracy of " << bestComboAccuracy*100 << "%\n";
}

int main()
{
    int fileChoice{};
    int userChoice{};
    string fileName = "";

    cout << "Welcome to Koa Miranda's Feature Selection Algorithm.\n";
    cout << "Please choose from the following: \n";
    cout << "\t1) Forward Selection\n\t2) Backwards Elimination\n\n";
    cin >> userChoice;
    cout << "\t1) Small Data\n\t2) Large Data\n\t3) Enter a file name\n";
    cin >> fileChoice;
    if (fileChoice== 1) fileName="CS170_Small_Data__24.txt";
    else if (fileChoice == 2) fileName="CS170_Large_Data__109.txt";
    else if (fileChoice == 3) 
    {
        cout << "File name: ";
        cin >> fileName;
    }
    vector<vector<double>> testdata = txtPuller(fileName);
    cout << "\nThis dataset has " << testdata[0].size()-1 << " features (not including the class attribute), with " << testdata.size() << " instances.\n\n\n";

    if (userChoice == 1) featureSearchForward(testdata);
    else if (userChoice == 2) featureSearchBackwards(testdata);
    return 0;
}