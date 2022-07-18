今日总结：
1.学习daplink的接线方式
2.sutdio环境搭建
  2.1按照文档要求更改daplink相关文件 
  2.2更改调试配置选项debugger中的芯片名称
  2.3更新PyOCD0.1.4
3.Git的使用
  3.1私钥获取
    3.1.1TortoiseGit中选择PuTTUgen创建密钥
    3.1.2github中选择个人设置，选择SSH and GPG keys
    3.1.3将PuTTUgen中创建的密钥粘贴进Key栏中，并选择私钥保存
  3.2提交pr
    3.2.1新建仓库（拷贝别人）
    3.2.2复制code选项下的内容
    3.2.3在想要备份的文件夹下右键选择git bash here
    3.2.4输入git clone再加上复制的内容
    3.2.5将想要放入仓库的文件放在仓库文件夹下
    3.2.6右键TortoiseGit选择Setting，选择git分支下的远端
    3.2.7远端重新命名，URL选择自己新建或是拷贝的仓库（例：https://github.com/luuuuuuki/summer2022.git），推送相同，密钥选择3.1.3中的私钥，确定。
    3.2.8右键仓库文件夹，选择TortoiseGit中的添加，勾选需要添加的文件选择提交，填写日志，设置日期及作者，选择提交。
    3.2.9选择推送，远端选择新建的远端，点击确定，提交成功
111111