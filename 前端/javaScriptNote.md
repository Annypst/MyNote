# JavaScript

解释性脚本语言，不能独立存在，需要嵌入网页运行。与Java没什么关系，仅仅是语言相同。

## 嵌入或外链html

- 使用双标签script嵌入

- 外链

  ```html
  <script type="text.javascript" src="url"></script> url是js文件路径
  ```

  

## 语法

### 变量

- 严格区分大小写

- 结尾分号可有可无

- 注释：  单行//    多行/*  */

- 顺序执行 

数据类型3种：

- 简单数据类型：number    string     boolean
- 复合数据类型：Array   function   Objects
- 特殊数据类型：无定义undefined   空数据，表示没有值和不存在的引用：null

变量命名：

- 必须字母数字下划线
- 不能数字开头
- 不能以保留字作为变量名 
- 严格区分大小写

关键字：

```js
var a
var a,b,c
var a=1,b,c=3;
var str="你好"
var bl=true  
//可以连续赋值
a=b=c=3
```

分为局部和全局变量，全局变量可在整个html文档生效

### 运算符

- 算术运算符

> [!CAUTION]
>
> 如果字符串是标签，那么输出后可以起到标签的同样效果。

- 赋值运算符
- 位运算

- 特有的

  != 不等于      == 等于（关注数据类型，String 2和number 2一样）         !==    严格不等于（关注数据类型，String 2和numbe r2不一样）          

   ===     严格等于，数据类型也需一样   

- 逻辑运算：非  与  或

- 三目运算： ？:

- 

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  







