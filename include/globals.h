#pragma once
#include <string>
#include <vector>
#include "Logger.h"

using namespace std;

extern Logger logger;
extern const string filePath;
extern const string outputPath;
void tolow(string& native);
void s_split(const string& native, string&& markdelim, vector<string>& res);

vector<string> docHelper(string path);

string dtos(double i);

void getFiles(string path, vector<string>& files);
