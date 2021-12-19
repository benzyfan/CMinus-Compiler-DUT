#pragma once
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <set>
using namespace std;


class WordAnalyze {
public:
    WordAnalyze(ifstream& file);
    void wordsdivide();//词法分析
    void output(ofstream &file); //输出词法分析结果
    void outeach(ofstream& file); //输出保留字表 ID 和sign
    map<pair<string, string>, pair<int, int>> ERRORLIST; //错误位置和情况
private:
    ifstream& sourceFile;           // 源程序文件输入流
    friend class GrammerAnalyze;
    vector < pair< pair< string, string >,pair<int,int> > > Words; //词法分析结果
    map<string, string> keep;//保留字表
    set< string > IDs; //ID表
    set< string > Signs;//字符表
};
