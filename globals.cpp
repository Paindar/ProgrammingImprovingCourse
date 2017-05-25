#include "include\globals.h"
#include <fstream>
#include <cstdlib>
#include <io.h> 
#include <Windows.h>

#define SSTREAM 1

#ifdef SSTREAM
#include <sstream>
#endif
using namespace std;
Logger logger;
const string filePath = "resOri.data";
const string outputPath = "output.txt";
void tolow(string & native)
{
    for (auto iter = native.begin(); iter != native.end(); iter++)
        *iter += (*iter > 'A'&&*iter < 'Z') ? 32 : 0;
}
void s_split(const string& native, string&& markdelim, vector<string>& res)
{
    #ifndef SSTREAM
	int str = 0, end = 0;
	while ((end = native.find(markdelim,str)) != -1)
	{
		res.push_back(native.substr(str, end - str));
		str = end+markdelim.size();
	}
    string s = native.substr(str);
    if(!s.empty())
	    res.push_back(s);
#else
    istringstream iss(native);
    string s;
    while (iss >> s)
    {
        res.push_back(s);
    }
#endif
}

vector<string> docHelper(string path)
{
    ifstream fp(path, ios_base::in);
    string s;
    vector<string> ori,res;
    if (!fp.good())
        return res;
    while (getline(fp, s))
    {
        ori.push_back(s);
    }
    while (1)//res.size() < 10)
    {
        if (ori.empty())
            break;
        unsigned i = rand() % ori.size();
        res.push_back(ori[i]);
        ori.erase(ori.begin() + i);
    }
    return res;
}

string dtos(double i)
{
    char s[24] = {};
    sprintf_s(s, "%f", i);
    return string(s);
}

void getFiles(string path, vector<string>& files)
{
    long   hFile   =   0;
    struct _finddata_t info;
	string p;
	if((hFile = _findfirst(p.assign(path).append("\\*").c_str(),&info)) !=  -1)
	{
		do
		{
			if((info.attrib &  _A_SUBDIR))
			{
				if(strcmp(info.name,".") != 0  &&  strcmp(info.name,"..") != 0)
					getFiles( p.assign(path).append("\\").append(info.name), files );
			}
			else
			{
				files.push_back(p.assign(path).append("\\").append(info.name) );
			}
		}while(_findnext(hFile, &info)  == 0);
		_findclose(hFile);
	}
}