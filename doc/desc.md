# 用户操作流程
1. create \<filename>
> 新建文件，空文件。

> 如果文件存在则报错
2. open \<filename>
> 打开文件，将文件内容从磁盘中载入到内存

> 如果文件不存在则创建文件
3. write [option] \<filename> \<content>
> option: -a, -w

> 如果文件不存在则报错
4. read \<filename>
> 读取文件内容

> 如果文件不存在则报错
5. cat [option] \<filename>...
> option: -h[n], -t[n], -m, -n

> \<filename>... 是一个数组，把多个文件按照顺序合并显示
> 如果文件不存在则报错
6. rm [option] \<filename>...
> option: -r, -R, -f, -i, -d, -v

> \<filename>... 是一个数组，删除多个文件

> 如果文件不存在则报错

