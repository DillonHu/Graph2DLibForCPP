第1步：
运行installGraph2D.exe或手动安装如下：
将glut.h、freegult.h、freglut_ext.h、freeglut_std.h拷贝到Code::Blocks安装目录下的mingw\include\GL文件夹中；
将graph2d.h拷贝到Code::Blocks安装目录下的mingw\include文件夹中；
将libfreeglut.a和libgraph2d.a拷贝到Code::Blocks安装目录下的mingw\lib文件夹中；
将freeglut.dll和graph2d.dll拷贝到操作系统安装目录下的windows\system32(32位)或windows\sysWOW64(64位)文件夹中；

第2步：
在codeblocks中选择settings->compiler and debugger->link settings中依次添加libgdi32, libopengl32, libglu32，libfreeglut和libgraph2d。
