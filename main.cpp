#include <iostream>     
#include <iomanip>
#include <vector>
#include <string>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include "include\SearchingEngine.h"
#include "include\globals.h"
//#define DEBUG 1

using namespace std;
const string query_path = "query.txt";

int main()
{
    logger.SetMessageLevel(Logger::info);
	SearchingEngine engine;
    srand((unsigned)time(0));
    vector<string> allFiles;
    cout << "[Info] Get all files." << endl;
    logger.Info("Get all files.");
    getFiles("C:\\Data", allFiles);

    cout << "[Info] Read  files." << endl;
    logger.Info("Read all files.");
    int i = 0;
    const int part = allFiles.size() / 250;
    cout << fixed;
    for(string path:allFiles)   
    {
        engine.readFile(path.c_str());
        if ((allFiles.size() - ++i) % part == 0)
            cout << "  Progress:" << setprecision(2) << i*100.0 / allFiles.size() << "%\r";
    }
    cout << endl;

    cout << "[Info] Initialize engine data." << endl;
    logger.Info("Initialize engine data.");
    engine.init();

    string s; 
    ofstream f1("1527405022matching.txt", ios_base::out), f2("1527405022vsm.txt", ios_base::out), f3("1527405022lm.txt", ios_base::out);
    int count = 0;

    cout << "[Info] Get Query data." << endl;
    logger.Info("Get Query data.");
    vector<string> source = docHelper(query_path);

    for(string s:source)
    {
        cout <<"  \r"<< s <<"\t\t\t\t";
        if (s.empty())
            continue;
        vector<string> reason;
#ifdef NORMAL
        reason = engine.findSong(s, 10);
        f1 <<  s << endl<< endl;
        for (auto iter : reason)
        {
            f1 << iter << endl;
        }
#ifdef DEBUG
        f1 << "------found " << reason.size() << " reasons." << endl;
#endif
        f1 << "@@@"<<endl;
#endif
        reason = engine.findSongX(s, 10);
        f2 << s << endl << endl;
        for (auto iter : reason)
        {
            f2 << iter<< endl;
        }
#ifdef DEBUG
        f2 << "------found " << reason.size() <<" reasons."<< endl;
#endif
        f2 << "@@@" << endl;
        reason = engine.findSongEX(s, 10);
        f3  << s << endl << endl;
        for (auto iter : reason)
        {
            f3 << iter << endl;
        }
#ifdef DEBUG
        f3 << "------found " << reason.size() << " reasons." << endl;
#endif
        f3 << "@@@" << endl;
        count++;
    }
    cout << endl;

    cout << "[Info] Save Files." << endl;
    logger.Info("Save Files.");
    f1.close();
    f2.close();
    f3.close();
	return 0;
}
