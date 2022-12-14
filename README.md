
# pssh

虚拟shell终端，支持重定向和管道以及自己实现的命令。

## shell
 * `readline`实现行读入
 * 忽略`ctrl^C`对shell的影响，`ctrl^D`结束shell
 * 启动时读取.sh环境变量

## 特性
 * 重定向
 * 管道
 * 管道重定向多次复合
 * `&` （未处理后台结束提示）
 
## 内建命令
 * `echo`
 * `export`
 * `cd`
 * `pwd`
 * `exit`
 * `kill`
 
## 外部命令
 * `ls`, `ls -l`
   * 可以显示`symbolic link`，有`total` 
 * `cat`
   * 多个文件无分隔符
 * `more`
   * 多个文件用`----------`分隔
 * `xargs`, `xargs -d`
   * `-d`参数形式可以为`-dx || -d x` 
 * `touch`
   *文件初始权限
 * `mkdir`
   * 文件夹初始权限
 * `ln`, `ln -s`
 * `cp`
   * mode一并复制
   * `cp file file`
   * `cp file ... dir`
   * 覆写提示
 * `mv`
   * `mv file new_name`
   * `mv file ... dir`
   * 覆写提示
 * `rm`
   * 调用`glibc`函数时排除目录
 
## 其他功能 
 * 某些特定输出颜色和高亮
 * 齐全的报错
 * 读入时处理了`<>|`和命令参数连在一起的输入
