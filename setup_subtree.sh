MyLibs=$(git remote | grep "MyLibs")
if  [ ! -z "MyLibs" ] ; then
	echo "remote MyLibs is existed"
else
    git remote add MlabvietEsp8266 https://github.com/quangthanh010290/my_libs.git
    echo "Created remote MyLibs" 
    git remote
fi

if  [ ! -d "my_libs" ] ; then
    git subtree add --prefix=my_libs MyLibs master
    git fetch MyLibs master
    git push origin master
fi


