

# 工作流程

## 1. 克隆（clone）仓库
协作流程不应该随意地提交到主分支，每个协作者都应该在本地仓库跑通了再提交，所以需要克隆仓库  
进入你想要存放本地仓库的**前一级文件夹**，在终端里运行`git clone https://github.com/COITSILLE/Spider_Robot`把仓库克隆到本地  
> `git clone [url]`  
> 把远程仓库克隆到**当前文件夹下**（可以看终端光标前面的路径）
    > > 比如，当前是在`PS D:\Documents\Spider_Robot> `，执行`git clone https://github.com/COITSILLE/Spider_Robot`会创建文件夹 `D:\Documents\Spider_Robot\Spider_Robot`，`Spider_Robot`里才是仓库的内容  

>  这个命令会自动将远程仓库命名为`origin` **注意，这个只是一个本地使用的代号，相当于`https://github.com/COITSILLE/Spider_Robot`，这样后面命令就不用输入这一长串url了，它和远程仓库实际的名字无关**  
> <img src="./mdpic/12.png" alt="origin名字" width="50%" height="50%">  

> `cd [路径]`
> `.`代表当前文件夹，`..`代表上一级文件夹，`cd ..`可以方便地进入上一级文件夹
> > 比如，创建了`D:\Documents\Spider_Robot>`并且使用vscode打开，终端显示当前是在`PS D:\Documents\Spider_Robot> `。执行`cd ..`就会显示`PS D:\Documents>`，表明当前是在`Documents`，如果你希望把仓库放进你新建这个文件夹，这时运行`git clone`就刚刚好  
> > 完成后，`cd .\Spider_Robot`即可返回



## 2. 议题（issue）和分支(branch)
对于一项更改或建议，比如添加某个功能/修某个bug/，有具体的想法后，在GitHub上提issue，详细说明要改哪部分
> 比如  
> <img src="./mdpic/6.png" alt="issue" width="50%" height="50%">  
留意圈出的编号

https://git-scm.com/book/zh/v2/Git-%e5%88%86%e6%94%af-%e5%88%86%e6%94%af%e7%ae%80%e4%bb%8b
对于实际的议题（不是建议/讨论性质的），在右方`Development`处点击`Create a branch` for this issue 
> <img src="./mdpic/8.png" alt="issue" width="50%" height="50%">  
分支名字保持默认，也就是[issue编号]-[issue说明]，分支源保持默认，即主分支
然后提示了两行命令
> <img src="./mdpic/9.png" alt="issue" width="50%" height="50%">
打开本地克隆下来的仓库，终端运行这两条命令
> `git fetch [origin]` 会告诉你远程仓库相比你的本地仓库有什么更改`
> > 比如  
> > <img src="./mdpic/10.png" alt="fetch" width="50%" height="50%">
>  > 这就是说远程仓库有了新分支`2-完成快速数学库`

> `git checkout [分支名]` 会checkout（签出，或者说切换）到（已有的）分支  
你应该看到左下角的当前分支名变了  
<img src="./mdpic/13.png" alt="fetch" width="50%" height="50%">




## 3. 写代码/更改文件
进入你工作的分支，在开始任何一项更改前，建议先拉取主分支以跟上整体进度，但不建议频繁拉取  
一般来说，应该确保你不是直接在主分支开始工作
> `git pull [远程仓库名] [分支名]` 相当于`git fetch` + `git merge`   
> 注意，如果你是中途拉取（也就是开始更改前已拉取过，更改进行时想再拉取），而在这段时间主分支有变更，`git merge`就会把主分支直接合并到你现在的工作。如果这不是你想要的，建议分开，先`git fetch`  
<img src="./mdpic/7.png" alt="拉取" width="50%" height="50%">  

vscode是可以进入仓库里面任何一个文件夹进行更改的，git会自动往上找到仓库的根文件夹
使用vscode打开项目中的某个文件夹后
<img src="./mdpic/2.png" alt="父存储库" width="50%" height="50%">  
点击“打开存储库”即可
> 比如，我想更改CubeMX项目`.\Software\Servo`，需要用vscode打开这个文件夹（这样STM32的插件才能认到CMake配置等），是可以的

现在可以愉快地写代码了，留意一下git有没有在跟踪你的文件  
<img src="./mdpic/3.png" alt="跟踪" width="50%" height="50%">  

## 4. 提交（commit）、拉取请求（Pull Request / PR）
**任何时候都可以保存，本地的保存和git是独立的**  

已经完成了一个部分，比如写好了一个函数，就可以点击“√提交”。这个提交只对本地仓库和当前分支起作用，且会在历史提交记录中留下痕迹。但也不建议随意提交
<img src="./mdpic/4.png" alt="提交" width="50%" height="50%">  
注意：必须在红圈那里输入“消息”，也就是提供更改的说明。如果执意留空，会默认把文件存入暂存区，而暂存区这部分比较复杂，会引入不必要的麻烦(如果想了解看 https://git-scm.com/book/zh/v2/Git-%e5%9f%ba%e7%a1%80-%e8%ae%b0%e5%bd%95%e6%af%8f%e6%ac%a1%e6%9b%b4%e6%96%b0%e5%88%b0%e4%bb%93%e5%ba%93)

工作在非主分支，可以按需推送到远程仓库的该分支，
通常来说，一个分支只有一个人在工作，但以防万一


**经过完整的测试**，确定刚才写的这部分能跑通了，并且你的本地仓库现在是干净的，没有别的被追踪的更改了，就可以和远程仓库打交道

1. **下载Github Pull Requests插件**  
<img src="./mdpic/1.png" alt="插件" width="60%" height="60%">   
2. 插件安装完成，就可以看到这个选项  
<img src="./mdpic/5.png" alt="插件" width="60%" height="60%">  
注意，如果你不小心当前分支是主分支，不要直接点击这个“同步”！ 这会直接将更改传到主分支 











