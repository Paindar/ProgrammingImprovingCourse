#include "include\SearchingEngine.h"
#include <iostream>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <fstream>
#include <cmath>
#include "include\globals.h"
#include "include\globals.h"
using namespace std;

void SearchingEngine::evaluation(int optRigNum, int allNum, int rigNum)
{
	_p = optRigNum*1.0f / allNum;
	_r = optRigNum*1.0f / rigNum;
	_f = 2 * _r*_p / (_r + _p);
}

std::vector<string> SearchingEngine::findSong(const std::string title, int max)
{
    string str = title;
    vector<string> query;
    s_split(title, " ",query);
    vector<pair<int, int>> sorted;
    for (unsigned i = 0; i < allItem.size(); i++)
    {
        int flag = 0;
        vector<string>* list = allItem[i].context;
        for (string str : query)
        {
            if (find(list->begin(),list->end(),str)!=list->end())
                flag++;
        }
        sorted.push_back(pair<int, int>(i, flag));
    }
    sort(sorted.begin(), sorted.end(), [](pair<int, int>&a, pair<int, int>&b) {return a.second > b.second; });
    std::vector<string> res;
    for (auto& iter:sorted)
    {
        if (res.size() > max && max != 0)
            break;
        if (iter.second <1)
            break;
        res.push_back(allItem[iter.first].title);
        logger.Info("Type 1:" + allItem[iter.first].title + ":"+dtos(iter.second));
    }
    return res;
}


std::vector<string> SearchingEngine::findSongX(const std::string& title,int max)
{
    vector<string> reason;
    unordered_map<int, double> midRes;//
    unordered_map<string, double> tmpDVer;
    double dTmp = 0.00;
    string str = title;
    tolow(str);
    vector<string> result;
    s_split(title, " ", result);
    for(string __tmp_word:result)
    {
        if (tmpDVer[__tmp_word] != 0)//重复单词
        {
            tmpDVer[__tmp_word] *=2;
        }
        else
        {
            unordered_map<int, int>& tmp = invlist[__tmp_word];
            tmpDVer[__tmp_word] = tmp.size()!=0?log10(invlist.size() / float(tmp.size())+1):0;
        }
    }
    for (auto iter = tmpDVer.begin(); iter != tmpDVer.end(); iter++)
    {
        dTmp += (*iter).second*(*iter).second;
    }
    dTmp = sqrt(dTmp);
    for (auto iter = tmpDVer.begin(); iter != tmpDVer.end(); iter++)
    {
        tmpDVer[(*iter).first] = (*iter).second / dTmp;
    }//normalize
    unordered_map<int,double> vdERes;
    for (auto iter = docWordWeight.begin(); iter != docWordWeight.end(); iter++)
    {
        if (iter->first == -1)
            continue;
        dTmp = 0;
        for (auto iterB = iter->second.begin(); iterB != iter->second.end(); iterB++)
        {
            if (tmpDVer.find(iterB->first)!=tmpDVer.end())
                dTmp += tmpDVer[iterB->first] * iterB->second;
        }
        vdERes[iter->first]= dTmp;//储存结果
    }
    vector<pair<int,double>> cmpVector(vdERes.begin(),vdERes.end());
    filter(cmpVector,4);
    for (auto& iter:cmpVector)//显示最优结果
    {
        if (reason.size()> max && max!=0 )
            break;
        reason.push_back(allItem[iter.first].title);
        logger.Info("Type 2:" + allItem[iter.first].title + ":" + dtos(iter.second));
    }
    return reason;

}

vector<string> SearchingEngine::findSongEX(const std::string & title, int max)
{
    std::vector<string> reason;
    unordered_map<int, double> w;
    string str = title;
    tolow(str);
    vector<string> result;
     s_split(title, " ", result);
    for (unsigned j = 0; j < prob.size(); j++)
    {
        double pr=0.00;
        unordered_map<string, unordered_map<string, double>>* doc=prob[j];
        if (result.size() == 1)
        {
            result.push_back("\n");
        }
        vector<pair<double, double>> buffer;//record the likelihood and weight of a word
        for (unsigned i = 0; i < result.size() - 1; i++)
        {
            string word = result[i + 1];
            double tmpIdf = (invlist[word].size()!=0)? invlist.size()/double(invlist[word].size()):1 ;
            if (doc->find(result[i]) != doc->end())
            {
                unordered_map<string, double>& layer1 = doc->at(result[i]);
                if (layer1.find(word) != layer1.end())
                {
                    buffer.push_back(pair<double, double>(layer1[word], tmpIdf));
                }
                else
                    buffer.push_back(pair<double, double>(0, tmpIdf));
            }
            else
                buffer.push_back(pair<double, double>(0, tmpIdf));
        }
        for (auto p : buffer)
        {
            pr = lerp(pr, p.first, 0.618);
        }
        w[j] = pr;
    }
    vector<pair<int, double>> cmpVector(w.begin(), w.end());
    filter(cmpVector, 4);
    for (auto& iter : cmpVector)//显示最优结果
    {
        if (reason.size()> max && max != 0)
            break;
        reason.push_back(allItem[iter.first].title);
        logger.Info("Type 3:" + allItem[iter.first].title + ":" + dtos(iter.second));
    }
    return reason;
}


float SearchingEngine::getP() { return _p; }
float SearchingEngine::getR() { return _r; }
float SearchingEngine::getF() { return _f; }

void SearchingEngine::readFile(const char * filePath)
{
    ifstream file(filePath, ios_base::in);
    string s;
    if (file.fail())
    {
        cerr << "[Fatal] Failed to open " << filePath << endl;
        logger.Fatal("Failed to open" + string(filePath));
        return;
    }
    Item item;
    item.title = string(filePath).substr(5);
    while (getline(file, s))
    {
        if (s.find("_token") == -1)
            continue;
        vector<string>* res = new vector<string>;
        allData.push_back(res);
        s_split(s, " ", *res);
        item.context = res;
    }
    file.close();
    allItem.push_back(item);
}

double SearchingEngine::lerp(double d1, double d2, double ins)
{
    if (ins <= 0)return d1;
    if (ins >= 1) return d2;
    return d1+(d2-d1)*cos(1.5707963*(1-ins)); 
}

void SearchingEngine::filter(vector<pair<int, double>>&cmpVector, int mode)
{
    sort(cmpVector.begin(), cmpVector.end(),
        [](pair<int, double> a, pair<int, double>b)
    {
        return a.second > b.second;
    });//排序
    switch(mode)
    {
    case 4:
        if (cmpVector.size() <= 1)
            return;
        else
        {
            int tail = cmpVector.size() - 1;
            double k = (cmpVector[0].second - cmpVector[tail].second)/(tail+1);
            for (unsigned i = 0; i < cmpVector.size() - 1; i++)
            {
                double nk = cmpVector[i].second - cmpVector[i + 1].second;
                if (k < nk)
                {
                    tail = i + 1;
                    k=nk;
                }
            }
            cmpVector.erase(cmpVector.begin() + tail, cmpVector.end());
        }
        break;
    default:
        for (auto iter = cmpVector.end()-1; iter >= cmpVector.begin();)
        {
            if (iter->second <= 1e-6)
                cmpVector.erase(iter--);
            else
                break;
        }
    }
}

SearchingEngine::~SearchingEngine()
{
    for (auto iter : allData)
    {
        delete iter;
    }
}

void SearchingEngine::init()
{
    string word;
    ifstream fp;
    int __map_key = 0;
    int proc = 0;
    double dTmp;
    for (unsigned i = 0; i < allItem.size(); i++)
    {
        unordered_map<string, int> funcWordToNum;//sum the amount of word
        pair<int, string> __pair;
        vector<string>* result = allItem[i].context;
        for (string s : *result)
        {   
            funcWordToNum[s]++;
        }
        int sum = 0;
        for (auto&& iterB:funcWordToNum)
        {
            invlist[iterB.first][i] = iterB.second;
        }//将分析结果储存
        if (i % 100 == 0)
            cout << i << '/' << allItem.size() << endl;
        auto* doc=new unordered_map<string, unordered_map<string, double>>();
        if (result->size() == 1)
        {
            (*doc)[((*result)[0])]["\n"]++;
            sum++;
        }
        for (unsigned j = 0; j < result->size() - 1; j++)
        {
            (*doc)[((*result)[0])][(*result)[j + 1]]++;
            sum++;
        }
        for (auto& iter : *doc)
        {
            sum = 0;
            for (auto& iter1 : iter.second)
            {
                sum += (int)iter1.second;
            }
            for (auto& iter1 : iter.second)
            {
                 iter1.second/=sum;
            }
        }
        prob.push_back(doc);
    }
    for (auto &&vStrIter : invlist)//生成单词信息
    {
        const auto& word = vStrIter.first;
        auto& wordFreqInDoc = invlist[word];
        for (auto &iter : wordFreqInDoc)
        {
            auto dTf = log10(double(iter.second)+1);
            auto dIdf = log10(invlist.size() / double(wordFreqInDoc.size())+1);
            auto dW = dTf*dIdf;
            docWordWeight[iter.first][word] = dW;//计算权值
        }
    }
    //==Debug==
    std::cout << " 语料库大小为:" << invlist.size() << endl;
    std::cout << " 文档数量为:" << docWordWeight.size() << endl;
    //
    for (auto&& iter:docWordWeight)//归一化
    {
        dTmp = 0.00;
        for (auto&& iterB:iter.second)
        {
            dTmp += (iterB.second * iterB.second);
        }//Calculate the sum
        dTmp = sqrt(dTmp);
        for (auto&& iterB : iter.second)
        {
            docWordWeight[iter.first][iterB.first] = iterB.second / dTmp;
        }
    }
}
