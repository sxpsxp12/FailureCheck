#!/bin/bash

select COMMAND in "清除中间文件" "清除交叉编译好的二进制文件" "交叉编译(已经包含删除中间文件命令)" "退出"
do 
case $COMMAND in
"清除中间文件")
	rm *.o;
	rm moc_*;
	rm ui_*;
	;;
"清除交叉编译好的二进制文件")
	make distclean;
	;;
"交叉编译(已经包含删除中间文件命令)")
	/home/lh/SystemPorting/Qt/build-qte-4.8.2/bin/qmake;
	make;
	rm *.o;
	rm moc_*;
	rm ui_*;
	echo "交叉编译完成！";
	;;
"退出")
	break
	;;
*)
	echo "选择错误,请重新选择!"
	continue
	;;
esac
done

