#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include "WordAnalyzer.h"
#include "GrammerAnalyzer.h"
#include "Explain.h"
using namespace std;

int main()
{
	string file_name;
	cout << "请输入测试文件名称" << endl;
	cin >> file_name;
	ifstream fin;
	fin.open("../test/" + file_name, ios::in);// 打开源程序文件
	if (!fin.is_open()) {   // 检查文件是否成功打开，如否，报错并退出程序
		cerr << "无法打开文件 " << file_name << endl;
		cerr << "请检查文件名称和位置是否正确" << endl;
		return 0;
	}

	//词法分析
	WordAnalyze word(fin);
	word.wordsdivide();
	ofstream fout;
	fout.open("../result/" + file_name  + "_Wordresule.txt", ios::out);
	word.output(fout);
	if (!word.ERRORLIST.empty())
	{
		cout << "词法分析有错误，请检查代码对应位置";
		return 0;
	}
	ofstream fout_d;
	fout_d.open("../result/" + file_name + "_Worddivid.txt", ios::out);
	word.outeach(fout_d);
	//语法分析
	GrammerAnalyze grammer(word);
	grammer.grammeranalyze();
	ofstream fout_grammer;
	fout_grammer.open("../result/" + file_name + "_Grammerresule.txt", ios::out);
	grammer.outputfour(fout_grammer);
	//解释程序
	Explain explain(grammer);
	cout << endl<<endl<<"请选择执行方式：1.直接执行 2.按行调试 3.从断点之后开始执行,退出则按q即可" << endl;
	while (1)
	{
		char choice;
		cin >> choice;
		switch (choice)
		{
		case '1':
				explain.outing(9999);
				break;
			case '2':
				explain.outing(0);
				break;
			case '3':
				int posi;
				cout << "请输入行号：";
				cin >> posi;
				while (posi > explain.grammerresult.FourLine.size() +99  || posi < 100) //输入的顺序过大了
				{
					cout << "输入行号不合理，请重新输入！" << endl;
					cout << "请输入行号：";
					cin >> posi;
				}
				explain.outing(posi - 100);
				break;
			case 'q':
				cout << "执行结束!" << endl;
				fin.close();
				fout.close();
				fout_grammer.close();
				fout.close();
				return 0;
			default:
				cout << "请输入正确的选择号！";
				break;
		}
	}
	fin.close();
	fout.close();
	fout_grammer.close();
	fout.close();
	return 0;
}