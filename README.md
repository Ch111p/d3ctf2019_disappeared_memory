> 出题人：Ch1p@Vidar
> 考察点：kernel dump文件分析、windows10 compressed memory

这道题是我在看blackhat 19年的议题时 看到了fireeye团队的

`https://i.blackhat.com/USA-19/Thursday/us-19-Sardar-Paging-All-Windows-Geeks-Finding-Evil-In-Windows-10-Compressed-Memory.pdf`

`https://i.blackhat.com/USA-19/Thursday/us-19-Sardar-Paging-All-Windows-Geeks-Finding-Evil-In-Windows-10-Compressed-Memory-wp.pdf`

这两篇文章 觉得蛮有意思 可以作为一个实践题 于是就出了这道题 但从做题情况看来可能是一个屑题:(

考点如下：

首先是对于dump文件的分析 -> windbg的使用

可执行程序为D3CTF.exe 所以选手可以很快的定位，且dump出可执行程序的代码段

稍加分析可以看出来 做的事情就是在桌面上递归去用xor加密*.png文件

这里值得一提的是代码段上只能看到第一页的数据 但是主体函数都在这里 只是函数符号出不来了..为了解决这个问题 这里函数调用都是用LoadLibrary、GetProcAddress获取(现在想来可能是这几页被放在了虚拟内存里面)

但是当选手想去访问数据段(加密后的数据) 会发现并不能访问到

然后结合上述的两篇文章 继续搜索 应该可以发现下面这篇文章(事实上后面给的hint拿来直接搜就可以看到下面这篇文章了)

`https://www.fireeye.com/blog/threat-research/2019/08/finding-evil-in-windows-ten-compressed-memory-part-two.html`

前两篇是个大概 重点是第三篇 除了最后一个偏移有点问题之外(1709是1848,实际上那附近的指针值也不多 尝试一下就出来了) 其他其实都可以直接靠着这个解出来orz

所以到最后也没有解我是蛮意外的 现在想来可能是一开始的description给的不太明显吧..但是里面其实也指出了可疑进程以及windows10的新特性(

也有可能是由于代码段后面几个页面也看不到 对选手造成了迷惑

但最后给的hint蛮明显的(..所以还是题目问题 出题人在这里谢罪了:(