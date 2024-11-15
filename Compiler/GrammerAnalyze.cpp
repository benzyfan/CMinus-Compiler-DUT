#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <iomanip>
#include <stack>
#include "WordAnalyzer.h"
#include "GrammerAnalyzer.h"
using namespace std;

/*错误类型
* 1.尝试打开错误的文件
* 2.使用了不合法的符号
* 3.头文件未声明或声明错误
* 4.未进行变量定义
* 5.错误定义变量
* 6.条件语句作为执行
* 7.赋值语句使用错误        
* 8.While语句没有配套的do
* 9.While语句之间定义了变量
* 10.While语句之后缺少判断语句
* 11.While语句组成不完整
* 12.for语句缺少组成成分
* 13.在for语句之中定义变量
* 14.重定义相同的变量
* 15.在While语句之中定义了变量
* 16.if语句组成不完整
* 17.if语句没有配套的then
* 18.then没有后续语句
* 19.{没有配套的}作为结尾
* 20.赋值表达式没有;作为结尾
* 21.变量定义语句没有;作为结尾
* 22.输入语句没有正确书写
* 23.输出语句没有正确书写
* 24.输入语句输入了多个变量或表达式
* 25.输出语句输出了多个变量或表达式
* 26.（没有）作为匹配
* 27.文法没有执行语句
* 28.文法没有定义语句
* 29.使用了未定义的变量
* 30.除法表达式使用0作为分母   
*/


/*语法表
<程序> ::= <头文件定义>{<分程序>}
<头文件定义>::= #include<iostream> int main()
<分程序> ::= <变量定义><执行语句>
 <变量定义>::= <变量类型><标识符>;{<变量定义 >}
<标识符> ::= <字母>{<字母>|<数字>}
<执行语句>::=<输入语句>|<输出语句>|<赋值语句>|<条件语句>|<While语句>|<For语句>|{<执行语句>}
<赋值语句>::=<标识符> <赋值运算符> <表达式 > ;
<While语句>::= while(<条件语句>)do<执行语句>
<For语句>::= for([<赋值语句>] <条件语句> ; <赋值语句> ) <执行语句>
<条件语句>::= if( <条件> ) then <执行语句> [ else <执行语句> ]
<条件语句>::= <逻辑或表达式> { || <逻辑或表达式> }
<逻辑或表达式>::= <逻辑与表达式> { && <逻辑与表达式> }
<逻辑与表达式>::= <表达式><关系运算符><表达式>
<表达式>::= <按位或表达式> { |  < 按位或表达式>}
<按位或表达式>::= <按位与表达式> { &  < 按位与表达式>}
<按位与表达式>::= <位移表达式> { <位移运算符> <位移表达式>}
<位移表达式>::= <乘除表达式>{<加减运算符> <乘除表达式>}
<乘除表达式>::= <运算式>{<乘除运算符> <运算式>}
<运算式>::=(<运算式>) |<标识符>|<整数>
<乘除运算符>::= * | /
<加减运算符>::= + | -
<位移运算符>::= >> | <<
<关系运算符> ::= == | != | < | <= | > | >=
<赋值运算符>::= = | += | -= | *= | /=
<输入语句> ::= scanf(<标识符>)
<输出语句> ::= printf(<标识符>)
<变量类型>::= int | char | string
<字母> ::= a|b|...|X|Y|Z
<数字> ::= 0|1|2|...|8|9
<整数> ::= [-] <数字> 
*/
pair<string, string> Errorname; //错误类型
pair<int, int> Errorposi; //错误位置
pair<string, string> IDNEED;
stack <string> outposi;//用于保存多层循环时的跳出
int tempValue = 0; //总觉得要用栈
int Line = 100; //跳转时使用的位置标号

bool hasLEF = false; //标记是否拥有左括号
int forstart = 0;
int Sdeep = 0;
int times = 0;
#define VEC vector < pair< pair< string, string >, pair<int, int> > >::iterator
#define Wresult wordanalyze.Words
#define Elist wordanalyze.ERRORLIST
#define KEEP wordanalyze.keep
#define ERRORSET(s)  Errorname = make_pair(it->first.first, s);\
wordanalyze.ERRORLIST.insert({ Errorname,it->second });

#define Four(s1,s2,s3,s4) Foursentence line;\
line.Type = s1;\
line.Number_1 = s2;\
line.Number_2 = s3;\
line.Name =s4;\
line.Posi = Line++;\
FourLine.push_back(line);

#define FoueTemp(s1,s2,s3,s4) Foursentence line_temp;\
line_temp.Type = s1; \
line_temp.Number_1 = s2; \
line_temp.Number_2 = s3; \
line_temp.Name = s4; \
line_temp.Posi = Line++; \
FourLine.push_back(line_temp);

GrammerAnalyze::GrammerAnalyze(WordAnalyze& word) :wordanalyze(word) {

    Relop.insert("="); Relop.insert("=="); Relop.insert("<="); Relop.insert("<"); Relop.insert(">=");
    Relop.insert(">");  Relop.insert("!="); Relop.insert(">");
    DEQU.insert("=");
    DEQU.insert("*=");
    DEQU.insert("/=");
    DEQU.insert("+=");
    DEQU.insert("-=");
    it = Wresult.begin();

}
void GrammerAnalyze::Define() 
{
    if (it->first.second == "INT")
    {
        it++;
        if (it->first.second == "ID")
        {

            if (IDList.find(it->first.first) != IDList.end())//检查重定义
            {
                cout << "重定义元素！"  <<"     错误位置" << it->second.first << "行" << it->second.second << "列";
                exit(0);
            }
            IDNEED = make_pair(it->first.first,"_");
            IDList.insert(IDNEED);
            string TempID = it->first.first;
            it++;
            if (it->first.first == ";")
            {
                it++;
                //生成D的四元式
                Four("INT", TempID, "_", "_");
            }
            else
            {
                cout << "缺少;结尾" << "     错误位置" 
                    << it->second.first << "行" << it->second.second << "列";
                exit(0);
            }
        }
        else
        {
            Errorname = make_pair(it->first.second, "D()fail");
            cout << "缺少声明的变量名 " << "     错误位置" 
                << it->second.first << "行" << it->second.second << "列";
            wordanalyze.ERRORLIST.insert({ Errorname,it->second });
            exit(0);
        }
        if (it->first.second == "INT" || it->first.second == "CHAR" || it->first.second == "STRING")
            Define();
    }
    else if (it->first.second == "CHAR")
    {
        it++;
        if (it->first.second == "ID")
        {

            if (IDList.find(it->first.first) != IDList.end())//检查重定义
            {
                cout << "重定义元素！" << "     错误位置" << it->second.first << "行" << it->second.second << "列";
                exit(0);
            }
            IDNEED = make_pair(it->first.first, "_");
            IDList.insert(IDNEED);
            string TempID = it->first.first;
            it++;
            if (it->first.first == ";")
            {
                it++;
                //生成D的四元式
                Four("CHAR", TempID, "_", "_");
            }
            else
            {
                cout << "缺少;结尾" << "     错误位置"
                    << it->second.first << "行" << it->second.second << "列";
                exit(0);
            }

        }
        else
        {
            Errorname = make_pair(it->first.second, "D()fail");
            cout << "缺少声明的变量名 " << "     错误位置"
                << it->second.first << "行" << it->second.second << "列";
            wordanalyze.ERRORLIST.insert({ Errorname,it->second });
            exit(0);
        }
        if (it->first.second == "INT" || it->first.second == "CHAR" || it->first.second == "STRING")
            Define();
    }
    else if (it->first.second == "STRING")
    {
        it++;
        if (it->first.second == "ID")
        {

            if (IDList.find(it->first.first) != IDList.end())//检查重定义
            {
                cout << "重定义元素！" << "     错误位置" << it->second.first << "行" << it->second.second << "列";
                exit(0);
            }
            IDNEED = make_pair(it->first.first, "_");
            IDList.insert(IDNEED);
            string TempID = it->first.first;
            it++;
            if (it->first.first == ";")
            {
                it++;
                //生成D的四元式
                Four("STR", TempID, "_", "_");
            }
            else
            {
                cout << "缺少;结尾" << "     错误位置"
                    << it->second.first << "行" << it->second.second << "列";
                exit(0);
            }
        }
        else
        {
            Errorname = make_pair(it->first.second, "D()fail");
            cout << "缺少声明的变量名 " << "     错误位置"
                << it->second.first << "行" << it->second.second << "列";
            wordanalyze.ERRORLIST.insert({ Errorname,it->second });
            exit(0);
        }
        if (it->first.second == "INT" || it->first.second == "CHAR" || it->first.second == "STRING")
            Define();
    }
    else
    {
        Errorname = make_pair(it->first.second, "定义变量错误");
        cout << "Please Check where ID was define" << "     错误位置"
            << it->second.first << "行" << it->second.second << "列";
        wordanalyze.ERRORLIST.insert({ Errorname,it->second });
        exit(0);
    }

}
void GrammerAnalyze::Numgiven()
{
    if (IDList.find(it->first.first) == IDList.end())//检查是否被定义
    {
        cout << "未被定义的变量" << it->first.first << "    错误位置:" << it->second.first << "行"
            << it->second.second << "列";
        exit(0);
    }
    else
    {
        string tempID = it->first.first;//记录当前ID号
        it++;
        if (it->first.first == "*=")
        {
            it++;
            string stringpoint = "t" + to_string(++tempValue);
            Four("*", tempID, OR(), stringpoint);
            FoueTemp ("=", tempID, stringpoint, "_")
        }
        else if (it->first.first == "+=")
        {
            it++;
            string stringpoint = "t" + to_string(++tempValue);
            Four("+", tempID, OR(), stringpoint);
            FoueTemp("=", tempID, stringpoint, "_")
        }
        else if (it->first.first == "-=")
        {
            it++;
            string stringpoint = "t" + to_string(++tempValue);
            Four("-", tempID, OR(), stringpoint);
            FoueTemp("=", tempID, stringpoint, "_")
        }
        else if (it->first.first == "/=")
        {
            it++;
            string stringpoint = "t" + to_string(++tempValue);
            Four("/", tempID, OR(), stringpoint);
            FoueTemp("=", tempID, stringpoint, "_")
        }
        else if (it->first.first == "=")
        {
            it++;
            Four("=", tempID, OR(), "_");
        }
        else
        {
            it++;
            cout << "不知名的赋值符号" << it->first.first << "    错误位置:" << it->second.first << "行"
                << it->second.second << "列";
            exit(0);
        }
    }
    if (it->first.first == ")" && forstart > 0 )
    {
        return;
    }
    if (it->first.first != ";")
    {
        cout << "缺少 ';' " << "    错误位置:" << it->second.first << "行"
            << it->second.second << "列";;
        exit(0);
    }
    else
    {
        it++;
    }
}
void GrammerAnalyze::IFCHECK()
{
    if (it->first.second == "IF")
    {
        it++;
        if (it->first.second == "(")
        {
            it++;
            string temp_if = Bool();
            int trueout, falseout;
            Four("JNZ", temp_if, "_", "");//真出口
            trueout = Line;
            FoueTemp("JEZ", temp_if, "_", "");//假出口
            falseout = Line;
            if (it->first.second == ")")
            {
                it++;
                if (it->first.second == "THEN")
                {
                    it++;
                    FourLine[trueout - 101].Name = to_string(Line);
                    jumpsymbol = true;
                    Sdeep++;
                    Start();
                    Sdeep--;
                    //无条件跳转语句
                    Foursentence Jtemp;
                    Jtemp.Type = "J";
                    Jtemp.Number_1 = Jtemp.Number_2 = "_";
                    Jtemp.Name = "";
                    Jtemp.Posi = Line++;
                    int jump_posi = Line;
                    FourLine.push_back(Jtemp);
                    if (it->first.second == "ELSE")
                    {
                        it++;
                        FourLine[falseout - 101].Name = to_string(Line);
                        jumpsymbol = true;
                        Sdeep++;
                        Start();
                        Sdeep--;
                    }
                    FourLine[jump_posi - 101].Name = to_string(Line);
                }
                else
                {
                    cout << "缺少Then" <<  "    错误位置:" << it->second.first << "行"
                        << it->second.second << "列";
                    exit(0);
                }
            }
            else
            {
                cout << "缺少）" << "    错误位置:" << it->second.first << "行"
                    << it->second.second << "列";
                exit(0);
            }

        }
        else
        {
            cout << "缺少IF" << "    错误位置:" << it->second.first << "行"
                << it->second.second << "列";
            wordanalyze.ERRORLIST.insert({ Errorname,it->second });
            exit(0);
        }
    }
}
void GrammerAnalyze::WhileCheck()
{
    if (it->first.second == "WHILE")
    {
    it++;
    if (it->first.second == "(")
    {
        it++;
        int jumpback = Line;
        outposi.push(to_string(jumpback) );//加入栈中 方便多层循环使用
        string temp_while = Bool();
        int trueout, falseout;
        Four("JNZ", temp_while, "_", "");//真出口
        trueout = Line;
        FoueTemp("JEZ", temp_while, "_", "");//假出口
        falseout = Line;
        if (it->first.second == ")")
        {
            it++;
            if (it->first.second == "DO")
            {
                it++;
                FourLine[trueout - 101].Name = to_string(Line); //当表达式为真的时候 我们要让他执行Do的语句
                jumpsymbol = true;
                Sdeep++;
                Start();
                Sdeep--;
                //当一次循环周期完成之后 我们要让他回到while的头部，再次进行判断
                Foursentence Jtemp;
                Jtemp.Type = "J";
                Jtemp.Number_1 = Jtemp.Number_2 = "_";
                Jtemp.Name = outposi.top();
                outposi.pop();//当循环执行完毕之后 我们要返回语句最前面
                Jtemp.Posi = Line++;
                FourLine.push_back(Jtemp);
                //在下面 我们就能知道 假出口在哪里了
                FourLine[falseout - 101].Name = to_string(Line);
            }
            else
            {
                cout << "缺少Do" << "    错误位置:" << it->second.first << "行"
                    << it->second.second << "列";;
                exit(0);
            }
        }
        else {
            cout << "缺少）"  << "    错误位置:" << it->second.first << "行"
                << it->second.second << "列";
            exit(0);
        }
    }
    else
    {
        cout << "缺少(" << "    错误位置:" << it->second.first << "行"
            << it->second.second << "列";;
        exit(0);
    }
    }
}
void GrammerAnalyze::FORCHECK()
{
    it++;
    if (it->first.second == "(")
    {
        it++;
        if (it->first.second == "INT")
        {
            cout << "请在最开始声明变量！ "<<"     错误位置" 
                << it->second.first <<"行" << it->second.second << "列";
            exit(0);
        }
        else if (it->first.second == "ID")
        {//初始化条件
            jumpsymbol = true;
            Numgiven();
        }
        else if (it->first.second == ";")
            it++;
        else
        {
            cout << "不支持如此定义for语句" << "     错误位置" 
                << it->second.first << "行" << it->second.second << "列";
            exit(0);
        }
    }
    else
    {
        cout << "for语句缺少(" << "     错误位置" 
            << it->second.first << "行" << it->second.second << "列";
        exit(0);
    }
    //for的退出语句
    string jumpback = to_string(Line);
        outposi.push(jumpback);//加入栈中 方便多层循环使用
    string temp_if = Bool(); //这里完成的是退出语句
    int trueout, falseout;
    Four("JNZ", temp_if, "_", "");//真出口
    trueout = Line;
    FoueTemp("JEZ", temp_if, "_", "");//假出口
    falseout = Line;

    if (it->first.second == ";")
    {
        it++;
        //FourLine[trueout - 101].Name = to_string(Line);
        jumpsymbol = true;
        string numback = to_string(Line);
        Numgiven();
        //outposi.push(to_string(Line - 1 ));//插入当前语句的位置

        Foursentence Jtemp;
        Jtemp.Type = "J";
        Jtemp.Number_1 = Jtemp.Number_2 = "_";
        Jtemp.Name = outposi.top();
        outposi.pop();//当循环执行完毕之后 我们要返回While语句最前面
        Jtemp.Posi = Line++;
        FourLine.push_back(Jtemp);
        outposi.push(numback);

        FourLine[trueout - 101].Name = to_string(Line);

        if (it->first.second == ")")
        {
            it++;
            if ((it->first.second == "IF" || it->first.second == "WHILE" || it->first.second == "ID" ||
                it->first.second == "CHAR" || it->first.second == "FOR" || it->first.second == "PRINTF"
                || it->first.second == "{" || it->first.second == "SCANF") 
                )
            {
                Sdeep++;
                Start();
                Sdeep--;
            }
            //当一次循环周期完成之后 我们要让他回到for的头部，再次进行判断
            Foursentence Jtemp;
            Jtemp.Type = "J";
            Jtemp.Number_1 = Jtemp.Number_2 = "_";
            Jtemp.Name = outposi.top();
            outposi.pop();//当循环执行完毕之后 我们要返回While语句最前面
            Jtemp.Posi = Line++;
            FourLine.push_back(Jtemp);
            //在下面 我们就能知道 假出口在哪里了
            FourLine[falseout - 101].Name = to_string(Line);
        }
        else
        {
            cout << "for语句缺少）" << "    错误位置:" << it->second.first << "行"
                << it->second.second << "列";
            exit(0);
        }
    }
    else
    {
        cout << "for语句缺少 ; " << "     错误位置" 
            << it->second.first << "行" << it->second.second << "列";
        exit(0);
    }
}
void GrammerAnalyze::Printout()
{
    if (it->first.second == "PRINTF")
    {
        it++;
        if (it->first.second == "(")
        {
            it++;
            if ((it->first.second == "ID") && ((it + 1)->first.second == ")"))
            {
                string outtemp = OR();
                Four("OUT", outtemp, "_", "_");
                if (it->first.second == ")")
                {
                    it++;
                    /*
                    if ((it->first.second == "IF" || it->first.second == "WHILE" || it->first.second == "ID"
                        || it->first.second == "FOR" || it->first.second == "PRINTF" ||it->first.second == "{"
                        || it->first.second == "SCANF")
                        && ((Sdeep - forstart) != 1 || forstart == 0))
                    {
                        Start();
                    }*/
                }
                else
                {
                    cout << "Print语句缺少 )" << "    位置:" << it->second.first << "行"
                        << it->second.second << "列";
                    exit(0);
                }
            }
            else
            {
                cout << "printf语句每次只能输出一个变量！" << "    错误位置:" << it->second.first << "行"
                    << it->second.second << "列";
                exit(0);
            }
        }
        else
        {
            cout << "Print语句缺少 (" << "    位置:" << it->second.first << "行"
                << it->second.second << "列";
            exit(0);
        }
    }
}
void GrammerAnalyze::Scanfin()
{
    if (it->first.second == "SCANF")
    {
        it++;
        if (it->first.second == "(")
        {
            it++;
            if ( ( it->first.second == "ID"  )
                &&( (it + 1)->first.second == ")" ) )
            {
                string outtemp = OR();
                Four("IN", outtemp, "_", "_");
                if (it->first.second == ")")
                {
                    it++;
                    /*if ((it->first.second == "IF" || it->first.second == "WHILE" || it->first.second == "ID"
                        || it->first.second == "FOR" || it->first.second == "PRINTF" ||it->first.second == "{")
                        && ((Sdeep - forstart) != 1 || forstart == 0))
                    {
                        Start();
                    }*/
                }
                else
                {
                    cout << "sacnf语句缺少 )" << "    错误位置:" << it->second.first << "行"
                        << it->second.second << "列";
                    exit(0);
                }
            }
            else
            {
                cout << "scanf语句每次只能赋值一个变量！" << "    错误位置:" << it->second.first << "行"
                    << it->second.second << "列";
                exit(0);
            }
        }
        else
        {
            cout << "scanf语句缺少 (" << "    错误位置:" << it->second.first << "行"
                << it->second.second << "列";
            exit(0);
        }
    }
}
void GrammerAnalyze::Start()
{
    if (it->first.second == "IF")
    {
        forstart++;
        IFCHECK();
        forstart--;
        if ((it->first.second == "IF" || it->first.second == "WHILE" || it->first.second == "ID" || 
            it->first.second == "CHAR"|| it->first.second == "FOR" || it->first.second == "PRINTF" 
            || it->first.second == "{"|| it->first.second == "SCANF"))
        {
            Start();
        }
    }
    else if (it->first.second == "WHILE")
    {
        forstart++;
        WhileCheck();
        forstart--;
        if ((it->first.second == "IF" || it->first.second == "WHILE" || it->first.second == "ID" ||
            it->first.second == "CHAR" || it->first.second == "FOR" || it->first.second == "PRINTF"
            || it->first.second == "{" || it->first.second == "SCANF"))
        {
            Start();
        }
    }
    else if (it->first.first == "{")
    {
        it++;
        jumpsymbol = false;
        Start();
        if (it->first.first == "}")
        {
            it++;
            if ((it->first.second == "IF" || it->first.second == "WHILE" || it->first.second == "ID" ||
                it->first.second == "CHAR" || it->first.second == "FOR" || it->first.second == "PRINTF"
                || it->first.second == "{" || it->first.second == "SCANF"))
            {
                if (forstart == Sdeep && (forstart != 0) && (Sdeep!=0))
                    return;
                Start();
            }
        }
        else
        {
            cout << "括号不匹配" << it->second.first << "," << it->second.second;
            exit(0);
        }

    }
    else if (it->first.second == "ID" ||
        it->first.second == "CHAR" || 
        it->first.second == "STRING")
    {
        while (it->first.second == "ID" || it->first.second == "CHAR" || it->first.second == "STRING")
        {
            Numgiven();
            if (jumpsymbol == true)
            {
                jumpsymbol = false;
                break;
            }
        }
        if ((it->first.second == "IF" || it->first.second == "WHILE" || it->first.second == "ID" ||
            it->first.second == "CHAR" || it->first.second == "FOR" || it->first.second == "PRINTF"
            || it->first.second == "{" || it->first.second == "SCANF"))
        {
            Start();
        }
    }
    else if (it->first.second == "FOR")
    {
        forstart++; 
        FORCHECK();
        forstart--;
        if ((it->first.second == "IF" || it->first.second == "WHILE" || it->first.second == "ID" ||
            it->first.second == "CHAR" || it->first.second == "FOR" || it->first.second == "PRINTF"
            || it->first.second == "{" || it->first.second == "SCANF"))
        {
           // if (forstart == Sdeep && (forstart != 0) && (Sdeep != 0))
                //return;
            Start();
        }
    }
    else if (it->first.second == "PRINTF")
    {
        Printout();
        if ((it->first.second == "IF" || it->first.second == "WHILE" || it->first.second == "ID"
            || it->first.second == "FOR" || it->first.second == "PRINTF" ||it->first.second == "{"
            || it->first.second == "SCANF"))
        {
            Start();
        }
    }
    else if (it->first.second == "SCANF")
    {
        Scanfin();
        if ((it->first.second == "IF" || it->first.second == "WHILE" || it->first.second == "ID" ||
            it->first.second == "CHAR" || it->first.second == "FOR" || it->first.second == "PRINTF"
            || it->first.second == "{" || it->first.second == "SCANF"))
        {
            Start();
        }
    }
    else
    {
        cout << "缺少执行语句！"  << "    错误位置:" << it->second.first << "行"
            << it->second.second << "列";;
        exit(0);
    }
}
string GrammerAnalyze::Bool()// ||
{
    string tempstring_pos1 = DoubleAnd();
    string tempstring_pos2;
    while (it->first.second == "||")
    {
        it++;
        tempstring_pos2 = DoubleAnd();
        string stringpoint = "t" + to_string(++tempValue);
        Four("|", tempstring_pos1, tempstring_pos2, stringpoint);
        string stringpoint_2 = "t" + to_string(++tempValue);
        FoueTemp("!=", stringpoint, "0", stringpoint_2);
        tempstring_pos1 = stringpoint_2;
    }
    return tempstring_pos1;
}
string GrammerAnalyze::DoubleAnd() // &&
{
    string tempstring_pos1 = Relopcheck();
    string tempstring_pos2;
    while (it->first.second == "&&")
    {
        it++;
        tempstring_pos2  = Relopcheck();
        string stringpoint = "t" + to_string(++tempValue);
        Four("&", tempstring_pos1, tempstring_pos2, stringpoint);
        string stringpoint_2 = "t" + to_string(++tempValue);
        FoueTemp("!=", stringpoint, "0", stringpoint_2);
        tempstring_pos1 = stringpoint_2;
    }
    return tempstring_pos1;
}
string GrammerAnalyze::Relopcheck()
{
    string tempstring_pos1 = OR();
    string tempstring_pos2;
    while (Relop.find(it->first.second) != Relop.end()) //判断>= > <= < != == 
    {
        string temp_type = it->first.first;
        it++;
        tempstring_pos2 =  OR();
        string stringpoint = "t" + to_string(++tempValue);
        Four(temp_type, tempstring_pos1, tempstring_pos2, stringpoint);
        tempstring_pos1 = stringpoint;
    }
    return tempstring_pos1;
}
string GrammerAnalyze::OR()
{
    string tempstring_pos1 = AND();
    string tempstring_pos2;
    while (it->first.second == "|")
    {
        string temp_type = it->first.first;
        it++;
        tempstring_pos2 = AND();
        string stringpoint = "t" + to_string(++tempValue);
        Four(temp_type, tempstring_pos1, tempstring_pos2, stringpoint);
        tempstring_pos1 = stringpoint;
    }
    return tempstring_pos1;
}
string GrammerAnalyze::AND()
{
    string tempstring_pos1 = Drift();
    string tempstring_pos2;
    while (it->first.second == "&")
    {
        string temp_type = it->first.first;
        it++;
        tempstring_pos2 = Drift();
        string stringpoint = "t" + to_string(++tempValue);
        Four(temp_type, tempstring_pos1, tempstring_pos2, stringpoint);
        tempstring_pos1 = stringpoint;
    }
    return tempstring_pos1;
}
string GrammerAnalyze::Drift()
{
    string tempstring_pos1 = ADDCheck();
    string tempstring_pos2;
    while ( it->first.second == ">>" || it->first.second == "<<" )
    {
        string temp_type = it->first.first;
        it++;
        tempstring_pos2 = ADDCheck();
        string stringpoint = "t" + to_string(++tempValue);
        Four(temp_type, tempstring_pos1, tempstring_pos2, stringpoint);
        tempstring_pos1 = stringpoint;
    }
    return tempstring_pos1;
}
string GrammerAnalyze::ADDCheck()//E→T { +T | -T }
{
    string tempstring_pos1 = MulCheck();
    string tempstring_pos2;
    while (it->first.second == "+" || it->first.second == "-")
    {
        string temp_type = it->first.first;
        it++;
        tempstring_pos2 = MulCheck();
        string stringpoint = "t" + to_string(++tempValue);
        Four(temp_type, tempstring_pos1, tempstring_pos2, stringpoint);
        tempstring_pos1 = stringpoint;
    }
    return tempstring_pos1;
}
string GrammerAnalyze::MulCheck()
{
    string tempstring_pos1 = Equal();
    string tempstring_pos2;
    while (it->first.second == "*" || it->first.second == "/")
    {
        string temp_type = it->first.first;
        it++;
        tempstring_pos2 = Equal();
        string stringpoint = "t" + to_string(++tempValue);
        Four(temp_type, tempstring_pos1, tempstring_pos2, stringpoint);
        tempstring_pos1 = stringpoint;
    }
    return tempstring_pos1;
}
string GrammerAnalyze::Equal()
{
    if (it->first.second == "(")
    {
        it++;
       string tempstring = OR();
       if (it->first.second == ")")
       {
           it++;
           return tempstring;
       }
        else
        {
            cout << "不合规的输入语句"  << it->second.first << "行" << it->second.second  << "列";
            exit(0);
        }
    }
    else if (it->first.second == "NUM")
    {
        it++;
        return (it - 1)->first.first;//返回常量值
    }
    else if (it->first.second == "ID")
    {
        if (IDList.find(it->first.first) == IDList.end())//检查是否被定义
        {
            cout << "未被定义的变量" << it->first.first << " 位置   " <<it->second.first 
                << "行" << it->second.second << "列";
            exit(0);
        }
        it++;
        return (it - 1)->first.first; //返回变量名
    }
    else
    {
        cout << "不合规的语句" << it->first.first << " 位置   " << it->second.first
            << "行" << it->second.second << "列";
        exit(0);
    }
}
void GrammerAnalyze::Program() 
{//头文件检测
    int i = 0;
    //检测P的first集合
    for (it = Wresult.begin(); it != Wresult.begin() + 10; it++, i++)
    {
        switch (i)
        {
        case 0:
            if (it->first.second != "BEGIN")
            {
                Errorname = make_pair(it->first.second, "Should start with a '#'");
                cout << "Should start with a '#'";
                wordanalyze.ERRORLIST.insert({ Errorname,it->second });
                exit(0);
            }
            break;
        case 1:
            if (it->first.second != "INCLUDE")
            {
                Errorname = make_pair(it->first.first, "Expect for 'include'");
                wordanalyze.ERRORLIST.insert({ Errorname,it->second });
                cout << "Expect for 'include'";
                exit(0);
            }
            break;
        case 2:
            if (it->first.first != "<")
            {
                Errorname = make_pair(it->first.first, "please ues '<'");
                wordanalyze.ERRORLIST.insert({ Errorname,it->second });
                cout << "please ues '<'";
                exit(0);
            }
            break;
        case 3:
            if (it->first.second != "IOSTREAM")
            {
                Errorname = make_pair(it->first.first, "Expect for 'iostream' to start");
                wordanalyze.ERRORLIST.insert({ Errorname,it->second });
                cout << "Expect for 'iostream' to start";
                exit(0);
            }
            break;
        case 4:
            if (it->first.first != ">")
            {
                Errorname = make_pair(it->first.first, "please use '>'");
                wordanalyze.ERRORLIST.insert({ Errorname,it->second });
                cout << "please use '>'";
                exit(0);
            }
            break;
        case 5:
            if (it->first.first != "int")
            {
                Errorname = make_pair(it->first.first, "need 'int'");
                wordanalyze.ERRORLIST.insert({ Errorname,it->second });
                cout << "need '{'";
                exit(0);
            }
            break;
        case 6:
            if (it->first.first != "main")
            {
                Errorname = make_pair(it->first.first, "need '{'");
                wordanalyze.ERRORLIST.insert({ Errorname,it->second });
                cout << "need '{'";
                exit(0);
            }
            break;
        case 7:
            if (it->first.first != "(")
            {
                Errorname = make_pair(it->first.first, "need '{'");
                wordanalyze.ERRORLIST.insert({ Errorname,it->second });
                cout << "need '{'";
                exit(0);
            }
            break;
        case 8:
            if (it->first.first != ")")
            {
                Errorname = make_pair(it->first.first, "need '{'");
                wordanalyze.ERRORLIST.insert({ Errorname,it->second });
                cout << "need '{'";
                exit(0);
            }
            break;
        case 9:
            if (it->first.first != "{")
            {
                Errorname = make_pair(it->first.first, "need '{'");
                wordanalyze.ERRORLIST.insert({ Errorname,it->second });
                cout << "need '{'";
                exit(0);
            }
            break;
        }
    }
    Define();
    Start();
    if (it->first.first != "}")
    {
        cout << "缺少}作为结尾"  << "位置 "<< it->second .first << "行" << it->second.second <<"列";
        exit(0);
    }
    else
    {
        it++;
    }
}
void GrammerAnalyze::outputfour(ofstream &file)
{
    vector <Foursentence>::iterator itout = FourLine.begin();
    cout <<endl <<  "生成四元式结果：" << endl << endl;
    for (; itout != FourLine.end(); itout++)
    {
        file  << itout->Posi << " ";
        file << "(" << setiosflags(ios::left) << setw(4) << itout->Type << " ," << setw(4) << itout->Number_1;
        file << ","<< setw(4) << itout->Number_2 << "," << setw(4) << itout->Name << ")" << endl;

        cout << itout->Posi << " ";
        cout << "(" << setiosflags(ios::left) << setw(4) << itout->Type << " ," << setw(4) << itout->Number_1;
        cout << "," << setw(4) << itout->Number_2 << ","<< setw(4) << itout->Name << ")" << endl;
    }
}
bool GrammerAnalyze::grammeranalyze()
{

    Program();
    if (it->first.second != "FINISH")
    {
        cout << "语法分析失败";
        return false;
    }
    Four("FINISH", "_", "_", "_");
    return true;
}