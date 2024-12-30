# 须知

**求大家多多star，还有就是用完了之后不要取消star，🙏🙏拜托了，这对我真的很重要

# 使用方法

**运行环境 ： VS Code即可完成运行（原运行环境为VS2019）**

## 实现功能

int char string类型的变量定义，for，if-then-else，while-do语句的循环嵌套，+，-，×，/ 运算，左移右移，按位与（&）按位或（|）运算，逻辑与（&&）逻辑或（||）运算，输入输出语句，+=，-=，=，/= ，！=，== 运算判别，四元式的拉链回填等操作。使用LR（1）进行相关分析

## 文法定义

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



## **使用步骤**

首先在已将安装好有VS Code 的Windows设备上，点击Compiler.sln即可开始运行

（无法打开的可以尝试在VS Code 中新建一个空白项目，然后将Compiler文件夹下的全部.cpp和.h文件配置到相关路径即可尝试打开）



## 测试文件位置

测试文件储存在test文件夹下，原有的test.txt文件是展示所有功能使用，无实际意义。其余以数字命名的txt文件是错误类型展示，在完全了解程序之前不建议运行使用

测试文件正确运行的词法、语法分析结果见result文件夹下，出现错误时会立即提醒
