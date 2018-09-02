## 如何運行

在 `./ctarget` 命令後添加 `-q`，避免連接評分服務器即可。

``` shell
(gdb) break *0x40190b
Breakpoint 1 at 0x40190b: file visible.c, line 73.
(gdb) run -q ./hex2raw  < touch3.sol.txt 
The program being debugged has been started already.
Start it from the beginning? (y or n) y
Starting program: /home/cwwang/文档/csapp/lab3/target1/ctarget -q ./hex2raw  < touch3.sol.txt
Cookie: 0x59b997fa

Program received signal SIGSEGV, Segmentation fault.
0x00000000004017bd in getbuf () at buf.c:16
16	in buf.c
(gdb) run -q touch3.sol.r.txt 
The program being debugged has been started already.
Start it from the beginning? (y or n) y
Starting program: /home/cwwang/文档/csapp/lab3/target1/ctarget -q touch3.sol.r.txt
Cookie: 0x59b997fa
Type string:^[[A
No exploit.  Getbuf returned 0x1
Normal return
[Inferior 1 (process 68565) exited normally]
(gdb) run -q -i touch3.sol.r.txt 
Starting program: /home/cwwang/文档/csapp/lab3/target1/ctarget -q -i touch3.sol.r.txt
Cookie: 0x59b997fa

Breakpoint 1, 0x000000000040190b in touch3 (
    sval=0x606010 "\210$\255", <incomplete sequence \373>) at visible.c:73
73	visible.c: 没有那个文件或目录.
(gdb) x/20w 0x5561dc78
0x5561dc78:	1075378792	49920	0	0
0x5561dc88:	0	0	0	0
0x5561dc98:	0	0	1431855104	0
0x5561dca8:	9	0	4202276	0
0x5561dcb8:	0	0	-185273100	-185273100
(gdb) print /x 10
$1 = 0xa
(gdb) x/20w 0x5561dc78
0x5561dc78:	0x4018fa68	0x0000c300	0x00000000	0x00000000
0x5561dc88:	0x00000000	0x00000000	0x00000000	0x00000000
0x5561dc98:	0x00000000	0x00000000	0x55586000	0x00000000
0x5561dca8:	0x00000009	0x00000000	0x00401f24	0x00000000
0x5561dcb8:	0x00000000	0x00000000	0xf4f4f4f4	0xf4f4f4f4
(gdb) delete
删除所有断点吗？ (y or n) y
(gdb) break *0x401916
Breakpoint 2 at 0x401916: file visible.c, line 73.
(gdb) run -q -i touch3.sol.r.txt 
The program being debugged has been started already.
Start it from the beginning? (y or n) y
Starting program: /home/cwwang/文档/csapp/lab3/target1/ctarget -q -i touch3.sol.r.txt
Cookie: 0x59b997fa

Breakpoint 2, 0x0000000000401916 in touch3 (
    sval=0x606010 "\210$\255", <incomplete sequence \373>) at visible.c:73
73	in visible.c
(gdb) x/20w 0x5561dc78
0x5561dc78:	0x383e4500	0xc4aff7fc	0x00606010	0x00000000
0x5561dc88:	0x55685fe8	0x00000000	0x00000004	0x00000000
0x5561dc98:	0x00401916	0x00000000	0x55586000	0x00000000
0x5561dca8:	0x00000009	0x00000000	0x00401f24	0x00000000
0x5561dcb8:	0x00000000	0x00000000	0xf4f4f4f4	0xf4f4f4f4
(gdb) 

```
