# html超链接

	## 简介



​	超链接有如下要素：链源、链宿、链属性。可以通过**热字**或者是**热图**、**热区**（如地图）、**热点**、**热属性**来作为链源。这里重点学习热字和热图。

链宿：是指链接到哪里。主要有**外部链接**、**内部链接**以及**其他**。外部链接在本机之外，例如链接到网页，需要URL等一些外部信息。内部链接就是链接到自己机器内的文件或者本地网页，可以使用相对路径来跳转。**其他**链接中有一种代表性的，叫做锚点链接，是链接到本篇文章内的 ，类似于书签跳转。最后，还有空链接，它不会跳转，但是也是设置链接了。空链接通常用于引发事件，当做一个按钮，结合之后学习的JavaScript可实现很多功能。





<img src="assets/image-20251202142725259.jpg" alt="image-20251202142725259" style="zoom:25%;" />

## 超链接的设置语法

```html
<a href="链宿">链源</a>
```



可由上述语法设置超链接。链源和链宿是必要的，其他的属性按照需求设置。

例如以外部链接，打开百度为例，其完整代码如下：

```html
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>超链接设置</title>
</head>
<body>
   <!--open a web page-->
   click <a href="https://baidu.com">here</a> to open baidu.
  <!--这就是链接到了百度网站-->
</body>
</html>
```

效果如下：

<img src="assets/image-20251202144305815.png" alt="image-20251202144305815" style="zoom:50%;" />

