## View

📺 满分示范：`addCust.jsp` 核心代码

Java

```jsp
<%@ page contentType="text/html;charset=UTF-8" language="java" %>
<html>
<head>
    <title>输入顾客信息</title>
</head>
<body>
    <h3>顾客信息</h3> <form action="${pageContext.request.contextPath}/CustServlet" method="post">
        
        姓名：<input type="text" name="name"> <br/> 地址：<input type="text" name="address"> <br/> <input type="submit" value="提交"> <input type="reset" value="重置">  </form>
</body>
</html>
```

### 📝 纸考必背的核心得分点（敲黑板！）

阅卷老师在批改这 5 分时，手里的扣分标准主要是看以下三个地方有没有写对 ：  

1. 

   **`action="/CustServlet"` 的路径问题** ： 表单必须明确写出要把数据提交给谁。题干要求提交给 `CustServlet` 。在实际大项目里，动态获取项目路径写成 `action="${pageContext.request.contextPath}/CustServlet"` 是最完美的 。如果你嫌长，在纸考时直接写 `action="CustServlet"` 或者 `action="/exam/CustServlet"` 老师通常也会给分 。  

2. **`method="post"` 的选择**：

   只要是向数据库**添加/插入**数据的表单，表单的提交方式一律写 `post`。因为前面咱们复习过，`POST` 传输数据没有大小限制，而且参数不会暴露在地址栏上。

3. **`<input>` 的 `name` 属性**： 这是**最重要**的得分点！输入框里必须写 `name="name"` 和 `name="address"` 。如果漏掉了 `name` 属性，后面的 Servlet 组件通过 `request.getParameter("参数名")` 就什么数据都拿不到了！这个 `name` 的取名必须和数据库或者大题里的 JavaBean 属性保持一致。  

根据题干要求：信息输入成功后，执行 `addSuccess.jsp` 页面，提示“顾客信息输入成功！”，并且可以通过点击**超链接**继续回到输入页面（`addCust.jsp`） 。  

这一问在纸考中同样属于基础送分题，主要考查的是 HTML 的超链接标签 `<a>` 。  

#### 📺 满分示范：`addSuccess.jsp` 核心代码

Java

```jsp
<%@ page contentType="text/html;charset=UTF-8" language="java" %>
<html>
<head>
    <title>输入成功</title>
</head>
<body>
    <h2>顾客信息输入成功！</h2> <a href="${pageContext.request.contextPath}/addCust.jsp">继续输入</a>
</body>
</html>
```

### 📝 纸考必背的核心得分点

这两分阅卷老师主要看一个地方：

- 

  **`<a href="...">` 超链接的路径** ： 超链接必须准确指向你刚才写的 `addCust.jsp` 页面。在纸考中，写 `href="addCust.jsp"` 或者带有项目路径的 `href="${pageContext.request.contextPath}/addCust.jsp"` 都是完美的。  



## Model

📺 满分示范：`JDBCUtils.java` 核心代码   

Java

```java
import java.sql.Connection;
import java.sql.DriverManager;

public class JDBCUtils {
    // 定义数据库连接的四大核心参数
    private static final String DRIVER = "com.mysql.cj.jdbc.Driver"; // 驱动类名
    private static final String URL = "jdbc:mysql://localhost:3306/jdbc?serverTimezone=GMT%2B8"; // 数据库URL
    private static final String USER = "root"; // 用户名
    private static final String PASS = "root"; // 密码

    // 静态代码块：随着类的加载而执行，只执行一次（用于注册驱动）
    static {
        try {
            Class.forName(DRIVER); // 1. 注册/加载数据库驱动
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    // 提供一个公共的静态方法，用于让外界获取数据库连接对象
    public static Connection getConnection() throws Exception {
        // 2. 传入URL、用户名、密码，建立并返回网络公路连接
        return DriverManager.getConnection(URL, USER, PASS);
    }
}
```

### 📝 纸考必背的核心得分点

老师在批改这 5 分时，眼里只有这两行核心的 API 语句 ：  

1. 

   **`Class.forName("驱动类名")`** ： 这是注册驱动的固定死写法。**学术严谨性注意**：新版 MySQL 的驱动类名是 `com.mysql.cj.jdbc.Driver` （多了个 `.cj`），如果纸考时写成旧版的 `com.mysql.jdbc.Driver` ，阅卷老师通常也不会扣分，但建议写新版的显得更严谨。  

2. 

   **`DriverManager.getConnection(url, user, password)`** ： 这是获取连接的核心方法。注意前面的调用者是数据库管理器 **`DriverManager`**，返回的类型是 **`Connection`** 。  

3. **异常处理（`try-catch` 或 `throws`）**：

   JDBC 操作属于受检异常，代码里必须有异常处理，不然代码编译报错，纸考直接扣分。在方法名后面直接甩出 `throws Exception` 是在考卷上最省时省力且绝对不会出错的技巧。

### 🧱 第二关：模型部分（续）——编写 `CustDao` 类



**Dao 层（Data Access Object，数据访问对象）** 专门负责写纯粹的 SQL 语句来操作数据库 。 在这一问中，我们要把前面写好的 `Customer`（数据实体）和 `JDBCUtils`（公路连接）组合起来，将顾客信息塞进数据库的 `customers` 表中 。  

在纸考中，老师极其喜欢考 **`PreparedStatement`（预编译执行对象）** ，因为它能防止 SQL 注入漏洞，且语法结构非常规范 。  

#### 📺 满分示范：`CustDao.java` 核心代码

Java

```java
import java.sql.Connection;
import java.sql.PreparedStatement;

public class CustDao {
    
    // 插入顾客信息的方法，返回受影响的行数（int）
    public int insert(Customer prod) throws Exception {
        Connection con = null;
        PreparedStatement preStmt = null;
        int num = 0; // 记录受影响的行数

        try {
            // 1. 调用工具类，获取数据库网络公路连接
            con = JDBCUtils.getConnection();

            // 2. 编写带有参数占位符 '?' 的 SQL 语句（id是自增的，不需要手动插入）
            String sql = "INSERT INTO customers (name, address) VALUES (?, ?)";

            // 3. 创建预编译执行对象
            preStmt = con.prepareStatement(sql);

            // 4. 为 SQL 语句中的参数占位符 '?' 赋值（注意：JDBC 索引从 1 开始！）
            preStmt.setString(1, prod.getName());    // 第一个问号对应姓名
            preStmt.setString(2, prod.getAddress()); // 第二个问号对应地址

            // 5. 执行 SQL 语句，返回受影响的记录数目
            num = preStmt.executeUpdate();

        } finally {
            // 6. 回收资源（关闭流，写在 finally 块中确保一定会执行）
            if (preStmt != null) preStmt.close();
            if (con != null) con.close();
        }

        return num; // 返回结果
    }
}
```

### 📝 纸考必背的核心得分点

这一问代码较长，但阅卷老师在纸上批改时，主要的给分步长都在这几个核心节点上：

1. 

   **SQL 语句的参数占位符 `?`** ： 用问号做占位符是 `PreparedStatement` 的标志 。千万**不要**自己在纸上苦哈哈地去用字符串拼接（如 `'" + name + "'`），不仅容易写错写乱，纸考中写字符串拼接还会被直接扣分。  

2. 

   **`preStmt.setXXX(参数索引, 值)` 的规范** ：  

   - 

     **类型要对应**：因为 `name` 和 `address` 在数据库和 JavaBean 里都是 String 类型，所以用 `setString()` 。  

   - 

     **索引从 1 开始** ：**这是极易扣分的重灾区！** 程序员习惯了数组从 0 开始计数，但 JDBC 协议中，**第一个问号的索引必须是 `1`，第二个是 `2`** 。如果写成 0，纸考会直接判错。  

3. 

   **`executeUpdate()` 的调用** ： 只要是执行 **`INSERT`、`UPDATE`、`DELETE`** 语句，一律调用 **`executeUpdate()`** 方法 。它的返回值是 `int`，表示数据库中受该 SQL 语句影响的记录数目 。  

4. 

   **资源回收 `close()`** ： 在纸考代码的末尾，必须写上 `preStmt.close(); con.close();` 。不写释放资源在闭卷纸考中属于典型的语义不完整，会酌情扣除 1~2 分。  

### 📘 Java Web 复习笔记：Dao 层三大核心套路速查

#### 1. 【增/删/改】套路（Insert, Delete, Update）

只要是改变数据库内容的，代码和我们刚刚写的 `insert` 几乎一模一样：

- **SQL 语句示例**：

  - 修改：`"UPDATE users SET name=?, address=? WHERE id=??"`   
  - 删除：`"DELETE FROM customers WHERE id=??"`   

- 

  **执行核心**：调用 **`preStmt.executeUpdate();`**   

- 

  **返回值**：`int`（表示受影响的行数） 。  

#### 2. 【查单个对象】套路（Select One）

如果考试让你“根据 ID 查找某一个顾客或图书”，代码需要做出以下调整：

- **SQL 语句示例**：`"SELECT * FROM customers WHERE id = ?"`

- 

  **执行核心**：改用 **`ResultSet rs = preStmt.executeQuery();`**（因为查询会返回结果集）.  

- 

  **处理结果**：使用 `if (rs.next())` 。如果能指到下一行，说明查到了，就通过 `rs.getXXX("字段名")` 把数据拿出来封装进 JavaBean 。  

- **核心代码片段示范**：

Java

```java
String sql = "SELECT * FROM customers WHERE id = ?";
preStmt = con.prepareStatement(sql);
preStmt.setInt(1, id);
ResultSet rs = preStmt.executeQuery(); // 🚀 查

Customer cust = null;
if (rs.next()) { // 📦 如果有数据，只读一行
    cust = new Customer();
    cust.setId(rs.getInt("id"));
    cust.setName(rs.getString("name"));
    cust.setAddress(rs.getString("address"));
}
return cust; // 返回封装好的对象
```

#### 3. 【查所有/列表】套路（Select List）

如果考试让你“查询所有的蛋糕或商品列表”，结构和查单个类似，只需要把 `if` 改成 **`while`** ：  

- 

  **SQL 语句示例**：`"SELECT * FROM customers"`   

- 

  **处理结果**：使用 `while (rs.next())` 。只要后面还有数据，就不断循环创建对象，并塞进一个 `ArrayList` 集合里。  

- **核心代码片段示范**：

Java

```java
String sql = "SELECT * FROM customers";
preStmt = con.prepareStatement(sql);
ResultSet rs = preStmt.executeQuery();

List<Customer> list = new ArrayList<>(); // 准备一个大箩筐
while (rs.next()) { // 🚀 只要还有下一行，就一直循环 
    Customer cust = new Customer();
    cust.setId(rs.getInt("id"));
    cust.setName(rs.getString("name"));
    cust.setAddress(rs.getString("address"));
    list.add(cust); // 往箩筐里塞
}
return list; // 返回整个列表
```

### 💡 纸考通关总结口诀

> **增删改用 `executeUpdate`，返回值是 `int`** ； **查询数据用 `executeQuery`，返回值是 `ResultSet`** ； **查单个用 `if(rs.next())`，查所有用 `while(rs.next())`** ！  

### 🛠️ 满分示范：`CustDao` 中的 `delete` 方法

Java

```java
import java.sql.Connection;
import java.sql.PreparedStatement;

public class CustDao {

    // 根据顾客 ID 删除顾客信息的方法，返回受影响的行数（int）
    public int delete(int id) throws Exception {
        Connection con = null;
        PreparedStatement preStmt = null;
        int num = 0; // 记录受影响的行数

        try {
            // 1. 获取连接
            con = JDBCUtils.getConnection();

            // 2. 编写删除 SQL 语句（根据主键 id 进行精确删除）
            String sql = "DELETE FROM customers WHERE id = ?";

            // 3. 创建预编译对象
            preStmt = con.prepareStatement(sql);

            // 4. 为问号赋值（索引依旧从 1 开始！）
            preStmt.setInt(1, id);

            // 5. 执行删除操作，返回受影响的行数（如果删除成功返回 1，找不到该用户返回 0）
            num = preStmt.executeUpdate();

        } finally {
            // 6. 严谨回收资源
            if (preStmt != null) preStmt.close();
            if (con != null) con.close();
        }

        return num; // 返回结果给控制层 Servlet
    }
}
```

### 📝 纸考“删除”细节避坑

1. 

   **SQL 语句千万不能漏掉 `WHERE` 条件** ： 如果纸考时脑子一热写成 `"DELETE FROM customers"`，这就变成了把整张表的所有顾客数据全部清空！这在学术开发和期末阅卷里属于重大灾难，5 分的步骤分会瞬间被扣光。必须加上 `WHERE id = ?` 。  

2. 

   **调用方法依然是 `executeUpdate()`** ： 删除、插入、修改在 JDBC 眼里都是对数据的“写操作”，因此统一调用 `executeUpdate()` 。  

## Controller

### 🧱 第三关：控制器部分——编写 `CustServlet` 类

在太原理工大学的真题中，控制器的流程控制逻辑是高度雷同的：

1. 

   **接收数据**：通过客户端请求拿到用户在表单里输入的参数 。  

2. **乱码处理**：纸考中**极其容易漏掉**的得分点，必须处理请求乱码。

3. 

   **调用模型**：把数据打包交给 Dao 层去执行添加 。  

4. 

   **页面跳转**：如果数据库影响行数 `> 0`（成功），重定向到成功页面；否则转发回添加页面 。  

#### 📺 满分示范：`CustServlet.java` 核心代码

Java

```java
import javax.servlet.ServletException;
import javax.servlet.annotation.WebServlet;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.io.IOException;

// 1. 使用注解配置虚拟访问路径（和 addCust.jsp 中的 form action 对应）
@WebServlet("/CustServlet")
public class CustServlet extends HttpServlet {

    @Override
    protected void doPost(HttpServletRequest request, HttpServletResponse response) 
            throws ServletException, IOException {
        
        // 2. 核心得分点：解决客户端通过 POST 方式提交中文数据的乱码问题
        request.setCharacterEncoding("utf-8");

        // 3. 接收表单参数（这里的参数名必须和 addCust.jsp 中 input 标签的 name 属性完全一致！）
        String name = request.getParameter("name");
        String address = request.getParameter("address");

        // 4. 将接收到的数据封装进实体类 JavaBean 对象中
        Customer cust = new Customer();
        cust.setName(name);
        cust.setAddress(address);

        // 5. 调用 Dao 层执行数据库插入操作
        CustDao dao = new CustDao();
        try {
            int num = dao.insert(cust); // 执行插入，返回受影响行数

            // 6. 根据执行结果进行流程控制（页面跳转）
            if (num > 0) {
                // 成功：使用重定向（URL会变）跳转到成功提示页面
                response.sendRedirect(request.getContextPath() + "/addSuccess.jsp");
            } else {
                // 失败：使用请求转发（URL不变）回到表单页面重新输入
                request.getRequestDispatcher("/addCust.jsp").forward(request, response);
            }
        } catch (Exception e) {
            e.printStackTrace();
            // 发生数据库异常，也跳转回添加页面
            request.getRequestDispatcher("/addCust.jsp").forward(request, response);
        }
    }
}
```

### 📝 纸考必背的核心得分点

阅卷老师在批改 Servlet 的 8 分时，是按照以下步骤给出步骤分的：

1. **`request.setCharacterEncoding("utf-8");`（1分）**：

   凡是接收 `POST` 请求的 Servlet，第一行雷打不动一定要写这一句！纸考中如果不写，通常会被直接扣掉 1 分。

2. **`request.getParameter("参数名")`（2分）**：

   这是从客户端请求中提取表单数据的标准方法。

3. 

   **`response.sendRedirect(...)`（重定向，2分）** ： 题干明确指出，如果插入**成功**，跳转到 `addSuccess.jsp` 。因为是一次页面切换，且不需要共享上一次的 `request` 域，所以符合规范的做法是使用**重定向** 。  

4. 

   **`request.getRequestDispatcher(...).forward(...)`（请求转发，2分）** ： 题干指出，如果插入**失败**，自动跳转回 `addCust.jsp` 。为了能在页面上保留用户之前输入的数据或者提示错误信息，这里标准做法是使用**请求转发** 。