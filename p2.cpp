
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include<time.h>
#include <vector>
#include <unordered_map>
using namespace std;

int find(int page, vector<int> frames){
    for(int i = 0; i < frames.size(); i++){
        if(page == frames.at(i))
            return i;
    }
    return 16;
} 
int fifo(int pages[], int numFrames, ofstream myfile){
    //ofstream myfile;
    time_t seconds;
    vector<int> frames;
    int misses = 0;
    int spot = 0;
/*
    if(numFrames == 4)
        myfile.open("hortonc_proj2_output_4frames.txt");
    else
        myfile.open("hortonc_proj2_output_8frames.txt");
*/
    for(int i = 0; i < numFrames; i++)
        frames.push_back(16);
    
    for(int i = 0; i < 100; i++){
        seconds = time(NULL);
        if(find(pages[i], frames) == 16){
            cout << "Miss on " << pages[i] << "\n";
            myfile << seconds << " Miss on " << pages[i] << "\n";
            //cout << spot % numFrames << "\n";
            frames.at(spot % numFrames) = pages[i];
            spot++;
            misses++;
        }
        else{
            cout << "Hit on " << pages[i] << "\n";
            myfile << seconds << " Hit on " << pages[i] << "\n";
        }

        for(int j = 0; j < numFrames; j++){
            cout << frames.at(j) << " ";
        //    myfile << frames.at(j) << " ";
        }
        cout << "\n";
      //  myfile << "\n";
    }
    cout << "Total page faults: " << misses << "\n";
    myfile << "Total page faults for fifo: " << misses << "\n";

    return 0;
}

int lru(int pages[], int numFrames, ofstrean myfile){
    //ofstream myfile;
    time_t seconds;
    vector<int> frames;
    unordered_map<int, int> freq;
    int misses = 0;

    /*
    if(numFrames == 4)
        myfile.open("hortonc_proj2_output_4frames.txt");
    else
        myfile.open("hortonc_proj2_output_8frames.txt");
    */

    for(int i = 0; i < numFrames; i++)
        frames.push_back(16);

    int loc;
    vector<int>::iterator it;
    for(int i = 0; i < 100; i++){
        seconds = time(NULL);
        loc = find(pages[i], frames);

        if(loc == 16){
            cout << "Miss on " << pages[i] << "\n";
            myfile << seconds << " Miss on " << pages[i] << "\n";
            freq[frames.at(0)]--;
            frames.erase(frames.begin());
            frames.push_back(pages[i]);
            freq[pages[i]]++;
            misses++;
        }
        else{
            cout << "Hit on " << pages[i] << "\n";
            myfile << seconds << " Hit on " << pages[i] << "\n";
            freq[pages[i]]++;
            it = frames.begin() + loc;
            frames.erase(it);
            frames.push_back(pages[i]);
        }
        int k = frames.size() - 2;
         while (freq[frames[k]] > freq[frames[k + 1]] && k > -1) {
            swap(frames[k + 1], frames[k]);
            k--;
        }
        for(int j = 0; j < numFrames; j++){
            cout << frames.at(j) << " ";
         //   myfile << frames.at(j) << " ";
        }
        cout << "\n";
      //  myfile << "\n";
    }
    cout << "Total page faults: " << misses << "\n";
    myfile << "Total page faults for lru: " << misses << "\n";

    return 0;
}

int second_chance(int pages[], int numFrames, ofstream myfile){
   // ofstream myfile;
    time_t seconds;
    vector<int> frames;
    bool chance[17];
    int misses = 0;
    int spot = 0;
    int loc;

    /*
    if(numFrames == 4)
        myfile.open("hortonc_proj2_output_4frames.txt");
    else
        myfile.open("hortonc_proj2_output_8frames.txt");
    */

    for(int i = 0; i < numFrames; i++)
        frames.push_back(16);
    for(int i = 0; i < 17; i++)
        chance[i] = false;

    for(int i = 0; i < 100; i++){
        loc = find(pages[i], frames);
        seconds = time(NULL);

        if(loc == 16){
            cout << "Miss on " << pages[i] << "\n";
            myfile << seconds << " Miss on " << pages[i] << "\n";
            misses++;
            while(true){
                if(chance[frames.at(spot % numFrames)] == false){
                    frames.at(spot % numFrames) = pages[i];
                    spot++;
                    break;
                }
                else{
                    chance[frames.at(spot % numFrames)] = false;
                    spot++;
                }

            }

        }
        else{
            cout << "Hit on " << pages[i] << "\n";
            myfile << seconds << " Hit on " << pages[i] << "\n";
            chance[frames.at(loc)] = true;
        }
        for(int j = 0; j < numFrames; j++){
            cout << frames.at(j) << " ";
           // myfile << frames.at(j) << " ";
        }
        cout << "\n";
      //  myfile << "\n";
    }
    cout << "Total page faults: " << misses << "\n";
    myfile << "Total page faults for Second Chance: " << misses << "\n";

   // myfile.close();
    return 0;
}

int main()
{
    int system, numFrames;
    bool go = true;
    int pages[100];
    ofstream myfile, myfile4, myfile8;
    time_t t;
    int tmp;

    myfile.open("hortonc_proj2_input.txt");
    myfile4.open("hortonc_proj2_output_4frames.txt");
    myfile8.open("hortonc_proj2_output_8frames.txt");
    srand(time(0));
    for(int i = 0; i < 100; i++){
        pages[i] = (rand() % 16);
        myfile << pages[i] << "\n";
    }
    myfile.close();

    while(go){
        cout << "Enter 0 for FIFO, 1 for LRU, 2 for Second Chance\n";
        cin >> system;
        cout << "Enter number of page frames (4 or 8)\n";
        cin >> numFrames;

        switch(system){
            case 0:
                cout << "FIFO with " << numFrames << " page frames\n";
                if(numFrames == 4)
                    fifo(pages, numFrames, myfile4);
                else
                    fifo(pages, numFrames, myfile8);
                break;
            case 1:
                cout << "LRU with " << numFrames << " page frames\n";
                if(numFrames == 4)
                    lru(pages, numFrames, myfile4);
                else
                    lru(pages, numFrames, myfile8);
                break;
            case 2:
                cout << "Second Chance with " << numFrames << " page frames\n";
                if(numFrames == 4)
                    second_chance(pages, numFrames, myfile4);
                else
                    second_chance(pages, numFrames, myfile8);
                break;
        }
        cout << "Enter 1 to go again\n";
        cin >> tmp;
        if(tmp != 1)
            go = false;
    }
    myfile4.close();
    myfile8.close();
    return 0;
}