#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include "include\SearchingEngine.h"
#include "include\globals.h"
#define DEBUG 1

using namespace std;
const string query_path = "query.txt";

int main()
{
    logger.SetMessageLevel(Logger::warn);
	SearchingEngine engine;
    srand((unsigned)time(0));
    vector<string> allFiles;
    getFiles("C:\\Data", allFiles);
    int i = 0;
    for(string path:allFiles)   
    {
        engine.readFile(path.c_str());
        if (++i % 100 == 0)
            cout << i << "/" << allFiles.size() << endl;
    }
    engine.init();
    string s; 
    ofstream f1("1527405022matching.txt", ios_base::out), f2("1527405022vsm.txt", ios_base::out), f3("1527405022lm.txt", ios_base::out);
    int count = 0;
    vector<string> source = docHelper(query_path);
    //vector<string> source = { "后来","イ","后来这首歌挺好听的" };
    for(string s:source)
    {
        cout << s << endl;
        if (s.empty())
            continue;
        vector<string> reason = engine.findSong(s,10);
        f1 <<  s << endl<< endl;
        for (auto iter : reason)
        {
            f1 << iter << endl;
        }
#ifdef DEBUG
        f1 << "------found " << reason.size() << " reasons." << endl;
#endif
        f1 << "@@@"<<endl;
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
        if (count%10 == 0)
            cout << count << " queries have been done." << endl;
    }
    f1.close();
    f2.close();
    f3.close();
	return 0;
}
