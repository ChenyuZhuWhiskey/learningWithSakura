# Learning With Sakura

## 这是什么

学习的日常进度记录

## 目前进度

<details>
<summary>Week1-4:学习C++并编写简单的STL</summary>
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
</details>

<details>
<summary>Week1～2: 学习编写简单的sgi stl</summary>

### 主项目：

传送门: [GraVity0-stl](https://github.com/yytgravity/Daily-learning-record/tree/master/第1～2周/GraVity0_stl)

### Question 1 ： vector编写过程中的安全问题思考：

传送门: [vector编写过程中的安全问题思考](https://github.com/yytgravity/Daily-learning-record/blob/master/%E7%AC%AC1%EF%BD%9E2%E5%91%A8/%E6%80%9D%E8%80%83%E9%A2%98/vector%E7%BC%96%E5%86%99%E8%BF%87%E7%A8%8B%E4%B8%AD%E7%9A%84%E5%AE%89%E5%85%A8%E9%97%AE%E9%A2%98%E6%80%9D%E8%80%83.md)

### Question 2 ：为什么实现了uninitialized_xxx和copy/fill这样两组不同的函数：

传送门: [为什么实现了uninitialized_xxx和copy/fill这样两组不同的函数](https://github.com/yytgravity/Daily-learning-record/blob/master/%E7%AC%AC1%EF%BD%9E2%E5%91%A8/%E6%80%9D%E8%80%83%E9%A2%98/%E4%B8%BA%E4%BB%80%E4%B9%88%E5%AE%9E%E7%8E%B0%E4%BA%86uninitialized_xxx%E5%92%8Ccopy:fill%E8%BF%99%E6%A0%B7%E4%B8%A4%E7%BB%84%E4%B8%8D%E5%90%8C%E7%9A%84%E5%87%BD%E6%95%B0.md)

### Question 3 ：绘制每个容器在内存里的对象存储图

传送门: [绘制每个容器在内存里的对象存储图](https://github.com/yytgravity/Daily-learning-record/blob/master/%E7%AC%AC1%EF%BD%9E2%E5%91%A8/%E6%80%9D%E8%80%83%E9%A2%98/%E7%BB%98%E5%88%B6%E6%AF%8F%E4%B8%AA%E5%AE%B9%E5%99%A8%E5%9C%A8%E5%86%85%E5%AD%98%E9%87%8C%E7%9A%84%E5%AF%B9%E8%B1%A1%E5%AD%98%E5%82%A8%E5%9B%BE.md)

### Question 4 ：测试题目

传送门: [小测试](https://github.com/yytgravity/Daily-learning-record/tree/master/第1～2周/小测验)

### Question 5 ：学习一下师傅们的漏洞思路：

传送门: [学习一下](https://github.com/yytgravity/Daily-learning-record/blob/master/%E7%AC%AC1%EF%BD%9E2%E5%91%A8/%E6%80%9D%E8%80%83%E9%A2%98/%E5%80%9F%E9%89%B4.md)

</details>

### STL

- allocator
- iterator
- container
  - vector
  - list 
  - tree
  - hashtable
  - set/map
  - hashset/hashmap
- functor
- adaptor
- unintitialized_xxx/copy fill

Compiler：

- PA2
- PA3
- PA4

### 计算机系统基础（一）

- 完成

### CSAPP Lab

data，bomb，attack，buffer，arch，cache，perform，shell，还差malloc，proxy