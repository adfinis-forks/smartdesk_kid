MyLibs=$(git remote | grep "MyLibs")
if  [ ! -z "MyLibs" ] ; then
	echo "remote MyLibs is existed"
    MyLibs=$(git remote | grep "MyLibs")
    if  [ ! -z "my_libs" ] ; then
        git subtree pull --prefix=my_libs MyLibs master --squash
    fi
else
    echo "Please setup subtree first" 
fi
