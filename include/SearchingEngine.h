#pragma once
#include <vector>
#include <unordered_map>
#include <string>

using namespace std;

class SearchingEngine
{
    struct Item
    {
        string title;
        vector<string>* context;
    };
public:
	void evaluation(int optRigNum, int allNum, int rigNum);
    std::vector<string> findSong(const std::string title, int max = 10);
    std::vector<string> findSongX(const std::string&, int max=10);
    std::vector<string> findSongEX(const std::string&, int max = 10);
    void readFile(const char* filepath);
    void init();
	float getP();
	float getR();
	float getF();
    ~SearchingEngine();
protected:
private:
	float _p;
	float _r;
	float _f;
    const char* invalidChar = "\\/:*\?\"<>|";
    vector<Item> allItem;
    unordered_map<string, unordered_map<int, int> > invlist;//µ•¥ ->Œƒµµ±‡∫≈->¥ ∆µ
    unordered_map<int, unordered_map<string, double> > docWordWeight;//Œƒµµ±‡∫≈->µ•¥ ->w÷µ 
    vector<vector<string>*> allData;
    
    //-------------------------------
    vector<unordered_map<string, unordered_map<string, double>>*> prob;//Language Model Document
    double lerp(double d1, double d2, double ins);
    void filter(vector<pair<int, double>>&, int mode);
};

