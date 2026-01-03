# 计划

到期末有这几件事需要完成：

## 考试复习

数据结构

实验报告！！！！！！！！

Java 重点：面向对象 正则表达式 数据库

概率论

马克思



数据结构非常紧急，王道书题、头哥作业题需要完成，看算法代码。

Java每天需保持最少1h学习

概率论每天学一点，等考完这些以后，全天学习概率论。其中穿插马克思。

## 课题

申请软件著作权，写材料。

找时间和吴老师去实验室。









嗯，大概就这些了。







```
package step2;

import java.lang.reflect.Constructor;
import java.lang.reflect.Field;
import java.lang.reflect.Method;
import java.lang.reflect.Modifier;

class Apple {
    private String name;
    public Apple(){}
    public Apple(String name){}
    public void setName(String name) {
        this.name = name;
    }
}

public class Reflect_stu {

    public static void main(String[] args) {
        // 请根据提供的 classPath 获取 step2.Apple 的 Class 对象， 请使用 Class.forName() 方法， 注意捕获异常
        // 通关之后，你也可以修改 clasapath 为其他类路径，分析某个类的能力， 例如: java.util.Date
        String classPath = "step2.Apple";
        Class clazz = null;
        /********** Begin *********/
        try {
            // 通过Class.forName获取Apple的Class对象
            clazz = Class.forName(classPath);
        } catch (ClassNotFoundException e) {
            e.printStackTrace();
        }
        /********** End *********/
        printFields(clazz);
        printConstructors(clazz);
        printMethods(clazz);
    }
    
  	/**
     * 请打印类的每个域,输出格式为：修饰符 类型 变量名;
     * @param clazz
     */
    public static void printFields(Class clazz) {
        /********** Begin *********/
        // 获取所有声明的字段（包括private）
        Field[] fields = clazz.getDeclaredFields();
        for (Field field : fields) {
            // 打印修饰符 + 类型 + 变量名 + 分号
            System.out.print(Modifier.toString(field.getModifiers()) + " ");
            System.out.print(field.getType().getName() + " ");
            System.out.println(field.getName() + ";");
        }
        /********** End *********/
    }

    /**
     * 打印构造函数,输出格式为：修饰符 方法名称（参数）
     * @param clazz
     */
    public static void printConstructors(Class clazz) {
 		Constructor[] constructors = clazz.getDeclaredConstructors();
        for (Constructor constructor : constructors) {
            Class[] paramTypes = null;
            /********** Begin *********/
            // 获取构造器的参数类型数组
            paramTypes = constructor.getParameterTypes();
            // 打印修饰符 + 构造器全类名 + (
            System.out.print(Modifier.toString(constructor.getModifiers()) + " ");
            System.out.print(constructor.getDeclaringClass().getName() + "(");
            /********** End *********/
            printParamTypes(paramTypes);
        }
    }

    /**
     * 请针对每个方法打印其签名，格式为：修饰符 返回值类型 方法名称(参数);
     * @param clazz
     */
    public static void printMethods(Class clazz) {
        Method[] methods = clazz.getDeclaredMethods(); // 修正拼写错误：methos -> methods
        for (Method method : methods) {
            Class[] paramTypes = null;
            /********** Begin *********/
            // 获取方法的参数类型数组
            paramTypes = method.getParameterTypes();
            // 打印修饰符 + 返回值类型 + 方法名 + (
            System.out.print(Modifier.toString(method.getModifiers()) + " ");
            System.out.print(method.getReturnType().getName() + " ");
            System.out.print(method.getName() + "(");
            /********** End *********/
            printParamTypes(paramTypes);
        }
    }


    /**
     * 打印方法参数
     * @param paramTypes
     */
    private static void printParamTypes(Class[] paramTypes) {
        for (int j = 0; j < paramTypes.length; ++j) {
            if (j > 0) {
                System.out.print(",");
            }
            System.out.print(paramTypes[j].getName());
        }
        System.out.println(");");
    }
    
}
```

