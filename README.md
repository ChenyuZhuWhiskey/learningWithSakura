# Learning With Sakura

## 这是什么

学习的日常进度记录

## 目前进度

<details>
<summary>Week1-5:学习C++并编写简单的STL</summary>

### 主项目
传送门：[My-Tiny-STL](https://github.com/ChenyuZhuWhiskey/MyTinySTL)

### 总结
- 主要配合侯捷视频以及SGI STL编写。STL分为6个组件：allocator，iterator，container，functor，adaptor，algorithm
- allocator(simple)：内存分配
	- 分配内存(allocate)的过程实际上就是`operator::new`的包装，`operator::new`就是`malloc`。BTW，没有必要使用侯捷推崇的内存池分配法，因为GNU C的`malloc()`本来就是在用内存池，侯捷没读过`malloc()`的源代码张口就莱。
	- deallocate的过程则包装了free()
	- construct:调用构造函数`::new()`
	- destruct:这里使用了模板偏特化的技巧，对于`__IS_POD_TYPE()`为`__true_type`的(primitive)，跳过析构过程，对于`__IS_POD_TYPE()`为`__false_type`(对象)则调用析构函数
- iterator：迭代器实际上就是泛化的指针，不同的iter操作级别不同，forward只能向前，bidirectional能向前向后，random_access能任意加减(一般指向连续空间的容器，比如vector)
- adaptor：简单容器改造一下得到的其他容器
- container：容器是STL提供的标准数据结构，我在myTinySTL中实现了几个比较重要的数据结构：
	- vector：三根指向连续内存，空间不够时申请一块2*size的新内存然后用`initialized_copy()`拷贝赋值
		![stl_vector](https://chenyu-blog-img-1302348848.cos.ap-shanghai.myqcloud.com/stl_mem_diagram/stl_vector.png)	
	- list：双向链表，首尾相连（环状）
![stl_list](https://chenyu-blog-img-1302348848.cos.ap-shanghai.myqcloud.com/stl_mem_diagram/stl_list.png)
	- tree:红黑树。
由于在写STL时没有系统学习过数据结构，因此补充了一下数据结构的知识。搜索树结构实际上启发于binary search，对于一个定义了比较的并且sorted in ordered的sequence，我们就可以使用binary search将复杂度降到log(n)。于是想到构造一种二叉搜索树：
```C++
struct BSTree{
  _Data data;
  BSTree* left;
  BSTree* right;
};
```
其中满足性质`left < data, right > data`，因此在搜索节点时就可以使用binary search了：
```C++
static BSTree* Search(BSTree* __tree, _Data& __data){
    if(!__tree){
        return nullptr;
    }
    if(__data.equal(__tree.data)){
        return __tree;
    }
    if(__data > __tree.data){
        return Search(__tree->right, __data);
    }else{
        return Search(__tree->left, __data);
    }
}
```
但如果全部节点集中在left或者right，那么`BSTree`就退化成了有序链表，搜索复杂度重新变回o(N)。于是我们就想办法改进一下。
#### B Tree
构造一个叫B Tree的结构，让每一个节点能存储L个数据，并且让这个节点能连接L+1个子节点，这样每个数据就对应了一个left和right，并且我们形式上保留BSTree的`left<data, right>data`。
#### Rotation
另外一种改进方法就是定义rotation操作。leftrorate就是把这个节点和`__tree.right`交换位置，让父节点指向它，然后它再移动到`__tree.right`的`left`上，通过有限次的`rotate`操作，我们可以将`BSTree`变成最佳的平衡二叉状态。
#### 红黑树
##### 定义与性质
有了前面的两种优化思路，我们就能定义红黑树了，红黑树其实就是`2-3Tree + Btree`,我们知道，B Tree其实在形式上已经高度平衡了，但肯定是BSTree才能用，那么我们就做一个约定将2-3 Tree转化为BSTree，这种带有约定的BStree就是红黑树：
split一个node后，右着（大的）成为父节点
![BT_to_RBT](https://chenyu-learning-with-sakura-img-1302348848.cos.ap-nanjing.myqcloud.com/20200608/BTree_to_RBT.png)
同时，将split后的边标为红色，其余为黑色。
根据这个定义，我们能得到红黑树的两条性质：
- 一个节点不可能有两条红色的边
- 根节点到每一条路径的黑色边数量相等
性质一是显然的，假如一个节点有两条红边，对应的2-3Tree就是这样：
![RBTImp](https://chenyu-learning-with-sakura-img-1302348848.cos.ap-nanjing.myqcloud.com/20200608/RBTree_imba_case.png)
显然违反了2-3Tree的定义。
性质二也是显然的，2-3Tree本来就是平衡树，split只会产生新的红边，那么每个路径通过的黑边自然相同。
根据性质1和2，能得到推论：
- 红黑树root到leaf最大路径长度是2N+1，其中N是黑边数量
证明：根据性质1，一个节点不可能有两条红边，那么一个路径上边最多的情况就是红黑相间，此时红边最多N+1个，于是最大路径不超过N+(N+1)=2N+1，证毕。
根据此推论，可以得到红黑树搜索算法时间复杂度：o(logn).
##### 插入算法
对于红黑树节点的插入算法，实际上就是把2-3Tree的插入算法步骤通过定义约定映射到BSTree就行了：
- 第一步：add。add会出现两种case：insert的新值到左或者右边（其余case都是insert后不违反定义的，就不提了）
![Insert_Add](https://chenyu-learning-with-sakura-img-1302348848.cos.ap-nanjing.myqcloud.com/20200608/RBT_insert_add.png)
![Insert_Add_case2](https://chenyu-learning-with-sakura-img-1302348848.cos.ap-nanjing.myqcloud.com/20200608/RBT_insert_add_case2.png)
假如是case2，我们先用rotateRight将它变为case1;
![Insert_rotate_case2to1](https://chenyu-learning-with-sakura-img-1302348848.cos.ap-nanjing.myqcloud.com/20200608/RBT_insert_rotate_case2tocase1.png)
- 第二部splite：实际上就是变换边的颜色：
![Insert_split](https://chenyu-learning-with-sakura-img-1302348848.cos.ap-nanjing.myqcloud.com/20200608/RBT_insert_splite.png)
假如插入节点没有违反红黑树规则1，那么就不用flip，假如flip后父节点违反规则1，则递归进行前面步骤。
##### 顺便吐槽一下
STL C++的红黑树插入没用递归来写，代码的if嵌套极其复杂，不知道开发人员怎么想的。
	- hashtable：算是一种vector的adaptor。
#### hash table
hashtable追求的是检索时算法复杂度为o(1)。在检索时对一个数组输入key时会获得常量复杂度，hash table就基于这样的思想。hash table设计时没用什么数学，纯粹是经验。现在我有a series of obj要存储到一个空间中，并且在search时像数组输入key一样立即返回值，那么一种可行的方法如下：
- 给出一个hash func，根据obj的key计算对应hash值
- 将obj存储在对应值编号的空间中
- 如果有不同的obj计算出了相同的hash值，那就将这个空间作为链表，在保证链表长度足够短时，检索链表的迭代时间就可以忽略不计，当作常量处理
大多数hash table用的hash函数就是将key除以bucket的size取余：
```C++
size_type hash_func(Obj& __obj, Bucket& __bucket){
    return __obj.key%__bucket.size()
}
```
具体实现地话，在stl中，bucket就是一个(`std::vector<*_Node>`)，`_Node`是单链表：
![stl_hashtable](https://chenyu-blog-img-1302348848.cos.ap-shanghai.myqcloud.com/stl_mem_diagram/stl_hashtable.png)
判断链表是否过长时使用一种经验方法：当插入数大于bucket的size时，就认为链表过长，此时重新申请更大空间，并按照hash函数重新分配各个obj。
![HT_size](https://chenyu-learning-with-sakura-img-1302348848.cos.ap-nanjing.myqcloud.com/20200608/hashTable_size.png)
stl的size就是这么取的，第一个是53，接下来不断加倍，取最近的素数作为新的size。
 - map/set：adaptor，底层数据结构就是一个红黑树
 - hashmap/hashset：adaptor，底层数据结构就是一个hashtable.

### 思考题：

1.一些STL会误用的场景：

- 假如容器中的对象中包含指针类型，使用默认拷贝构造函数(浅拷贝)，析构时会触发UAF。一个自定义拷贝构造函数往往就对应一个析构函数，算是常识。
- vector的`erase()`不检查边界，误用会访问未初始化的内存：

```c++
#include <vector>
int main() {
    std::vector<int> test = std::vector<int>();
    test.push_back(1);
    auto iter = test.begin();
    test.erase(iter + 2);
    return 0;
}
```

- 使用`insett()`,`erase()`等会改变vector内部三根指针指向位置的method时，如果预先cache了`begin()`,`end()`,`size()`等，再使用这些method后cache的值就不等于真正的值了，可能就会导致访问未初始化的内存

2. STL中为什么要定义`uninitialized_copy/fill`和`initialized_copy/fill`两组函数？

   uninitialized_xxx是拷贝构造，会向操作系统申请容器空间。initialized_xxx是拷贝赋值，向已经申请过的内存写入。

3. 容器内存模型

   上面的图有了

</details>

<details>
<summary>Week6-9:CS143 Compiler</summary>

### 主项目

传送门：[CS143_Compiler](https://github.com/ChenyuZhuWhiskey/CS143_Compiler)

### 总结

#### Program Assignment：

- PA2：![PA2](https://weekly-assignment-1302348848.cos.ap-shanghai.myqcloud.com/week5/PA2.png)

  见  https://github.com/ChenyuZhuWhiskey/CS143_Compiler/tree/master/assignments/PA2

- PA3：

![PA3](https://weekly-assignment-1302348848.cos.ap-shanghai.myqcloud.com/week5/PA3.png)

​	见 https://github.com/ChenyuZhuWhiskey/CS143_Compiler/blob/master/assignments/PA3

- PA4：

![PA4](https://weekly-assignment-1302348848.cos.ap-shanghai.myqcloud.com/week5/PA4.png)

​	见 https://github.com/ChenyuZhuWhiskey/CS143_Compiler/tree/master/assignments/PA4 

##### 小插曲：

在写AST语法树的时候，因为观察到了标准答案一些语法检查是通过多态实现，所以想尝试一下使用模板偏特化实现，结果模板会把一个指向子类对象的父类指针推到为父类，然后就炸了

#### 思考题

1. 浮点数和无符号整数的文法

   浮点数：

   ```
   S -> optionalSign Int optionalDecimal optionalExp
   optionalSign -> +|-|epsilon
   Int digit | Intpart
   optionalDecimal -> .Int | epsilon
   optionalExp -> e optionalSign Int | epsilon
   digit -> 0|1|2|3|4|5|6|7|8|9|
   ```

   无符号整数：

   ```
   S -> positive A | A
   A -> A digit | digit
   digit -> 0 | positive
   positive -> 0|1|2|3|4|5|6|7|8|9|
   ```

2. 描述每种文法（LL(1),SLR, LR(1), LALR等...)的使用条件，和它是为了解决什么问题？

   LL1是为了解决自顶向下推导算法的左递归无限循环，以及backtracking导致的效率低下问题，通过每条文法的SELECT集进行预测分析，因此LL1的使用条件是每条正则文法的SELECT集不冲突，不产生歧义。

   SLR是通过每个项目的FOLLOW集来判断遇到移入-规约冲突时进行移入操作还是规约操作，主要解决LR(0)遇到的移入-规约冲突。使用前提就是他们的FOLLOW集两两不相交

   LR(1)时为了解决SLT仅仅通过FLOOW集解决移入-规约冲突的不足。有项目：

   $A \rightarrow \alpha \cdot a \beta$

   $B \rightarrow \gamma\cdot$

   若a是B的FOLLOW集，则用产生式B规约是必要条件二非充分条件。LR (1)通过向前看一个展望符构造项目集闭包的自动机来解决移入-规约冲突。LR(1)的使用条件是要求文法是LR(1)的，几乎所有context-free grammar都是LR1的。

   LALR文法主要是为了解决LR(1)劈裂LR(0)产生的状态数过于庞大的问题，主要通过合并LR(1)的同心集实现，使用前提是合并后不产生归约-归约冲突。

3. 阅读用flex和bison生成出来的cool代码lexer和parser，理清代码骨架。

   flex见博客：[https://chenyuzhuwhiskey.github.io/2020/06/21/flex-lexer%E5%88%86%E6%9E%90/](https://chenyuzhuwhiskey.github.io/2020/06/21/flex-lexer分析/) 

   bison见博客：[https://chenyuzhuwhiskey.github.io/2020/06/27/bison-parser%E6%B7%B1%E5%85%A5%E5%88%86%E6%9E%90/](https://chenyuzhuwhiskey.github.io/2020/06/27/bison-parser深入分析/)

</details>

<details>
<summary>Week10-13:CSAPP Lab</summary>

### 主项目

传送门：[Csapp-Lab](https://github.com/ChenyuZhuWhiskey/Csapp-Lab)

### 总结

- datalab：整数和浮点数二进制的操作，熟悉它们的编码规则就不难：https://github.com/ChenyuZhuWhiskey/Csapp-Lab/blob/master/DATALAB_README.md

- bomb和attack,buffer都是实施缓冲区溢出攻击，主要是学一学gdb，pwndbg，ROPgagdget这些常见工具。当ASLR开启时，栈的地址就被随机化了，然后就需要使用gagdget。

  - bomb:  https://github.com/ChenyuZhuWhiskey/Csapp-Lab/blob/master/BOMB_README.md 
  - attack ： https://github.com/ChenyuZhuWhiskey/Csapp-Lab/blob/master/ATTACKLAB_README.md 
  - buffer: https://github.com/ChenyuZhuWhiskey/Csapp-Lab/blob/master/BUFFER_README.md 

- arch lab：PartA是根据对应的c代码写它的汇编版本，挺简单的。B就是在架构中添加iaddq，也不是很难，PartC难炸了，做不来。https://github.com/ChenyuZhuWhiskey/Csapp-Lab/blob/master/ARCHLAB_README.md 

- cache lab：

  - PartA模拟缓存的工作。首先要知道缓存的算法流程：

  ![cache](https://chenyu-blog-img-1302348848.cos.ap-shanghai.myqcloud.com/kernel-note/00-Write-back_with_write-allocation.png)

  接下来照着写就行。

  - PartB是优化矩阵转置的算法，思路就是去提高代码的时间空间局部性
  - 见：https://github.com/ChenyuZhuWhiskey/Csapp-Lab/blob/master/CACHELAB_README.md

- shell lab：写一个linux shell。照着CSAPP文中给的代码框架来就可以了。有个坑就是用bash去运行shell时，默认情况下，shell也会是bash的子进程，那么我们用ctrl+c是，实际上bash会向shell和每个子进程都发送`SIGINT`信号，这是部队的，因此在fork后execve前重写shell子进程的pid保证bash有且只有shell一个子进程

- malloc lab：花时间最多的lab，因为翻译the art of software security assessment的内存损坏漏洞时提到heap overflow的基础是对free时的frelist链表写入操作进行利用，但这里并没有写的很详细，所以除了花时间看CSAPP第九章虚拟内存以外（感觉把物理存储当作内存的cache，以及详细的页表操作对malloc lab好像帮助不大，不过对虚拟地址空间有了深入了解），还花了时间去读glibc malloc的源码，参考了CTF wiki和ptmalloc源码分析这两个资料，写了篇博客： [https://chenyuzhuwhiskey.github.io/2020/08/14/glibc-malloc-%E6%BA%90%E7%A0%81%E5%88%86%E6%9E%90/](https://chenyuzhuwhiskey.github.io/2020/08/14/glibc-malloc-源码分析/) 。不过由于时间太赶，只读了核心的数据结构部分，实际上就是内存池（链表数组）+内存池的cache（fast bin），相应size的free chunk会在对应的index中被链在链表里，并且物理相邻的free chunk会被合并，所以确实把C++的allocator交给malloc就足够了，再单独写一个内存池说不定性能还没glibc的malloc优秀。malloc lab需要实现的malloc没有那么复杂，就是内存池（小块）+平衡二叉树（大块），不过没拿到满分，可能是没有写fast bin当作内存池的cache来加快有时间空间局部性内存申请的速度

  评分：

  ```
  Team Name:evangelion
  Member 1 :Chenyu ZHU:id1
  Measuring performance with gettimeofday().
  
  Testing mm malloc
  Reading tracefile: short1-bal.rep
  Checking mm_malloc for correctness, efficiency, and performance.
  
  Results for mm malloc:
  trace  valid  util     ops      secs  Kops
   0       yes   66%      12  0.000000 24000
  Total          66%      12  0.000000 24000
  
  Perf index = 40 (util) + 40 (thru) = 80/100
  ```

</details>

<details>
<summary>Week14-20:Linux Kernel</summary>

### 主项目

还没搞完，传送门：[ucore](https://github.com/ChenyuZhuWhiskey/simple_kernel)

内核分析的博客更新中：https://chenyuzhuwhiskey.github.io/categories/%E6%93%8D%E4%BD%9C%E7%B3%BB%E7%BB%9F/

</details>

<details>
<summary>支线任务：翻译the art of software security assessment</summary>

目前已翻译:chap1,2,5,6

博客传送门：https://chenyuzhuwhiskey.github.io/categories/translate/

</details>