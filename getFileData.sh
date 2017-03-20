

info=`cat /proc/mounts | grep '/dev/sd' | cut -d ' ' -f 2 `
USBMountPath=${info}
LogInfoPath="../FailureCheck/Files/Log/"
HistoryDataPath="../FailureCheck/Files/historyData/"
NewDataPath="../FailureCheck/Files/newData/"

echo "USBMounthPath: " ${USBMountPath}"/";
echo "LogInfoPath:"${LogInfoPath};
echo "HistoryDataPath:"${HistoryDataPath};
echo "NewDataPath:"${NewDataPath};
echo ;

if  [ -d ${USBMountPath}/log ]
then
	echo "Log 目录已经存在";
else  
	mkdir ${USBMountPath}/log
fi

if [ -d ${USBMountPath}/historyData ]
then 
	echo "historyPath 目录已经存在";
else 
	mkdir ${USBMountPath}/historyData
fi

if [ -d ${USBMountPath}/newData ]
then 
	echo "newData 目录已经存在";
else 
	mkdir ${USBMountPath}/newData
fi

echo ;
echo "请选择功能!";
select COMMAND in "导出Log文件" "导出上一次数据" "导出最新数据" "退出"

do 
case $COMMAND in
"导出Log文件")
	cp $LogInfoPath* ${USBMountPath}/log/
	;;
"导出上一次数据")
	cp $HistoryDataPath* ${USBMountPath}/historyData/
	;;
"导出最新数据")
	cp $NewDataPath* ${USBMountPath}/newData/
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
