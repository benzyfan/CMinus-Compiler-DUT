#include <ctype.h>
#include <map>
#include <string>
#include <set>
#include <iomanip>
#include <stack>
#include "WordAnalyzer.h"
using namespace std;

char ch;       //当前未读入的字符
bool read = false;
int temp;
string string_get; //读到的字符串
string sym;
pair<int, int> position(1,1);             //保留当前读取到的位置 <行，列>
WordAnalyze::WordAnalyze(ifstream& file) : sourceFile(file) {
    keep.insert({ "int","INT" });
    keep.insert({ "while", "WHILE" });
    keep.insert({ "do","DO" });
    keep.insert({ "else","ELSE" });
    keep.insert({ "if","IF" });
    keep.insert({ "then","THEN" });
    keep.insert({ "scanf","SCANF" });
    keep.insert({ "printf","PRINTF" });
    keep.insert({ "include","INCLUDE" });
    keep.insert({ "iostream","IOSTREAM" });
    keep.insert({ "for","FOR" });
    keep.insert({ "char","CHAR" });
    keep.insert({ "string","STRING" });
    keep.insert({ "main","MAIN" });
    //插入保留字
}

#define GET     ch = sourceFile.get()   // 定义宏，简化从文件中读取一个字符的操作
#define PEEK    ch = sourceFile.peek()  // 定义宏，简化查看文件流第一个字符的操作
#define ADD(c)  string_get.append(1, c)     // 定义宏，简化往字符串结尾添加字符的操作

void WordAnalyze::wordsdivide()
{
    pair <string, string> newitem;
    pair < pair <string, string>, pair<int, int> > wordcheck;
#define PUSH(s1,s2) newitem =  make_pair(s1,s2);\
wordcheck = make_pair(newitem,position);\
Words.push_back(wordcheck);
	GET;
    while (ch != EOF) //
    {
        while (isspace(ch)) //去除空格
        {
            if (ch == ' ')
            {
                position.second++;
            }
            else if (ch == '\n')
            {
                position.first++;
                position.second = 1;
            }
            GET;
        }
        if (isdigit(ch))
        {                   
            temp = ch - '0';            
            for (GET; isdigit(ch); GET)
            {
                temp = temp * 10 + ch - '0';
                position.second++;
            }
            PUSH(to_string(temp), "NUM");
        }
        else if (isalpha(ch))
        {
            string_get.clear();
            ADD(ch);
            for (GET; isalnum(ch); GET)
            {
                ADD(ch);//获取完整个字符串
                position.second++;
            }

            map<string, string>::iterator it = keep.find(string_get);
            if (it == keep.end())
            {// 不在保留字表中，说明是标识符
                PUSH(string_get, "ID");
                IDs.insert(string_get);//将获得到的ID放在ID表里
            }
            else
            {// 在保留字表中
                PUSH(it->first, it->second);
            }
        }
        else
        {
            switch (ch)
            {
                // 以下，在这个 switch 块中，每一种情况执行完之后，都使得 ch 是处理完的字符
                // 在这个 switch 块外面，统一再读一个新字符备用
            case '&':
                string_get.clear();
                ADD(ch);
                PEEK;
                if (ch == '&')
                {
                    ADD(ch);
                    GET;
                    PUSH(string_get, "&&");
                    Signs.insert("&&");
                    position.second++;
                    break;
                }
                else
                {
                    PUSH(string_get, "&");
                    Signs.insert("&");
                    break;
                }
            case '|':
                string_get.clear();
                ADD(ch);
                PEEK;
                if (ch == '|')
                {
                    ADD(ch);
                    GET;
                    PUSH(string_get, "||");
                    Signs.insert("||");
                    position.second++;
                    break;
                }
                else
                {
                    PUSH(string_get, "|");
                    Signs.insert("|");
                    break;
                }
            case '(':
                PUSH("(", "(");
                Signs.insert("(");
                break;
            case ')':
                PUSH(")", ")");
                Signs.insert(")");
                break;
            case ';':
                PUSH(";", ";");
                Signs.insert(";");
                break;
            case '+':
                string_get.clear();
                ADD(ch);
                PEEK;
                if (ch == '=')
                {
                    ADD(ch);
                    GET;
                    PUSH(string_get, "+=");
                    Signs.insert("+=");
                    position.second++;
                    break;
                }
                else
                {
                    PUSH(string_get, "+");
                    Signs.insert("+");
                    break;
                }
            case '-':
                string_get.clear();
                ADD(ch);
                PEEK;
                if (ch == '=')
                {
                    ADD(ch);
                    GET;
                    PUSH(string_get, "-=");
                    Signs.insert("-=");
                    position.second++;
                    break;
                }
                else if (isdigit(ch))
                {
                    GET;
                    temp = ch - '0';            // 将数字字符转化为整数
                    for (GET; isdigit(ch); GET)
                    {
                        temp = temp * 10 + ch - '0';
                        position.second++;
                    }
                    PUSH( "-" + to_string(temp), "NUM");
                    read = true;
                    break;
                }
                else
                {
                    PUSH(string_get, "-");
                    Signs.insert("-");
                    break;
                }
            case '*':
                string_get.clear();
                ADD(ch);
                PEEK;
                if (ch == '=')
                {
                    ADD(ch);
                    GET;
                    PUSH(string_get, "*=");
                    Signs.insert("*=");
                    position.second++;
                    break;
                }
                else
                {
                    PUSH(string_get, "*");
                    Signs.insert("*");
                    break;
                }
            case '/':
                string_get.clear();
                ADD(ch);
                PEEK;
                if (ch == '=')
                {
                    ADD(ch);
                    GET;
                    PUSH(string_get, "/=");
                    Signs.insert("/=");
                    position.second++;
                    break;
                }
                else
                {
                    PUSH(string_get, "/");
                    Signs.insert("/");
                    break;
                }
            case '=':
                string_get.clear();
                ADD(ch);
                PEEK;
                if (ch == '=')
                {
                    ADD(ch);
                    GET;
                    PUSH(string_get, "==");
                    Signs.insert("==");
                    position.second++;
                    break;
                }
                else
                {
                    PUSH(string_get, "=");
                    Signs.insert("=");
                    break;
                }
            case '<':
                string_get.clear();
                ADD(ch);
                PEEK;                           
                switch (ch)
                {
                case '=':
                    ADD(ch);
                    GET;
                    PUSH(string_get, "<=");
                    Signs.insert("<=");
                    position.second++;
                    break;
                case '<':
                    ADD(ch);
                    GET;
                    PUSH(string_get, "<<");
                    Signs.insert("<<");
                    position.second++;
                    break;
                default:
                    PUSH(string_get, "<");
                    Signs.insert("<");
                    break;
                }
                break;
            case '#':
                string_get.clear();
                ADD(ch);
                PUSH(string_get, "BEGIN");
                Signs.insert("#");
                break;
            case '>':
                string_get.clear();
                ADD(ch);
                PEEK;                           // 同上，这里不能用 get!
                if (ch == '=')
                {
                    ADD(ch);
                    GET;
                    PUSH(string_get, ">=");
                    Signs.insert(">=");
                    position.second++;
                    break;
                }
                else if (ch == '>')
                {
                    ADD(ch);
                    GET;
                    PUSH(string_get, ">>");
                    Signs.insert(">>");
                    position.second++;
                    break;
                }
                else
                {
                    PUSH(string_get, ">");
                    Signs.insert(">");
                    break;
                }
            case '{':
            {
                PUSH("{", "{");
                Signs.insert("{");
                break;
            }
            case '}':
            {
                PUSH("}", "}");
                Signs.insert("}");
                break;
            }
            case '!':
                string_get.clear();
                ADD(ch);
                PEEK;                           
                if (ch == '=')
                {
                    ADD(ch);
                    GET;
                    PUSH(string_get, "!=");
                    Signs.insert("!=");
                    position.second++;
                    break;
                }
                else
                {
                    newitem = make_pair(string_get, "不合法字符");
                    ERRORLIST.insert({ newitem,position });
                    break;
                }
            default:                            
                string_get.clear();                        //  如果读到的是其他字符，一定是非法字符
                ADD(ch);
                newitem = make_pair(string_get, "不合法字符");
                ERRORLIST.insert({ newitem,position });
                break;
            }
            position.second++;
            if (read == false)
            {
                GET;  // 此处需要再 get 一个新的字符备用
            }
            else
            {
                read = false;
            }
        }
    }
    PUSH("#", "FINISH");
#undef PUSH
}

void WordAnalyze::output(ofstream & file)
{
    if (!ERRORLIST.empty())//错误表不空，存在语法错误
    {
        for (auto it = ERRORLIST.begin(); it != ERRORLIST.end(); it++)
        {
            cout << it->first.second  << setw(6) << it->first.first << "    ";
            cout << "位置：" << it->second.first << "行" << it->second.second <<"列" << endl;
        }
        return;
    }
    cout << "词法分析结果" << endl  << endl;
    for (auto it = Words.begin(); it != Words.end(); it++)
    {
        file << "字符值:" << setiosflags(ios::left) 
            << setw(6) << it->first.first<<"符号类别:" << setw(6) << it->first.second << endl;
        cout << "字符值:" << setiosflags(ios::left)
            << setw(6) << it->first.first << "符号类别:" << setw(6) << it->first.second 
            << "  位置:" << it->second.first<< ","<< it->second.second << endl;
    }
}

void WordAnalyze::outeach(ofstream& file)
{
    cout << endl << "ID表;" << endl;
    file << endl << "ID表;" << endl;
    for (auto it = IDs.begin(); it != IDs.end(); it++)
    {
        file << "字符值:" << setiosflags(ios::left)
            << setw(6) << *it << "符号类别:ID"  <<endl;
        cout << "字符值:" << setiosflags(ios::left)
            << setw(6) << *it << "符号类别:ID" << endl;
    }

    cout << endl << "符号表;" << endl;
    file << endl << "符号表;" << endl;
    for (auto it = Signs.begin(); it != Signs.end(); it++)
    {
        file << "符号值:" << setiosflags(ios::left)
            << setw(6) << *it << "符号类别:Sign" << endl;
        cout << "符号值:" << setiosflags(ios::left)
            << setw(6) << *it << "符号类别:Sign" << endl;
    }

    cout << endl << "保留字表;" << endl;
    file << endl << "保留字表;" << endl;
    for (auto it = keep.begin(); it != keep.end(); it++)
    {
        file << "符号值:" << setiosflags(ios::left)
            << setw(6) << it->first << "符号类别:保留字" << endl;
        cout << "符号值:" << setiosflags(ios::left)
            << setw(6) << it->first << "符号类别:保留字" << endl;
    }
}