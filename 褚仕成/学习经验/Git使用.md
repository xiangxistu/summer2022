## Git使用

工作区 ----> 暂存区---->本地库---->远程库



#### 小技巧：

yy ：复制；

p ：粘贴；

输入部分命令后双击TAB补全；

ll ：查看工作区的文件；

cat  <文件名> : 查看文件；

vim <文件名> ：编辑文件；

i ：让文本处于编辑状态；

:wq ：保存并退出；

Ctrl+L ：清屏命令；

Ctrl + Insert ：复制；

Shift + Insert ：粘贴；

双击左键 ：复制；

单击鼠标中键 ：粘贴；

ESC + dd ：删除一整行；







#### 命令：

git init ：首先将要使用git管理的文件初始化；

git status ：查看暂存区的文件状态；

git add ： 将文件从工作区传到暂存区；

git rm --cached <文件名> ：删除暂存区的文件；

git commit -m "日志信息"<文件名> ： 将暂存区的文件提交到本地库；

git reflog ：查看日志版本信息；

git log ：查看日志版本详细信息；（输入q退出）

git log --pretty=oneline ：查看日志版本信息（简化）；

git reset --hard <版本号> ：版本穿梭；



###### 分支的操作：

git branch 分支名 ：创建分支；

git branch -v：查看分支；

git checkout 分支名：切换分支；

git merge 分支名 ：把指定的分支合并到当前分支上；



###### 远程仓库：

git remote -v ：查看当前所有远程地址别名；

git remote add 别名 远程地址 ：给远程地址起别名；

git push 别名 分支 ：推送本地分支到远程仓库；

git pull 别名 分支 ：拉取远程仓库分支到本地；

git clone 链接 ：克隆远程库到本地（拉取代码，初始化本地仓库，创建别名）







#### 注意事项：

1. 合并有冲动的分支后，要重新上传到暂存区，然后上传到本地库时，commit命令后面不能加文件名。