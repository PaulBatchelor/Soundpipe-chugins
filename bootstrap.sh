if [ "$#" -ne 2 ]
then
    echo "Usage: sh bootstrap.sh soundpipe_name chugin_name"
    exit
fi


NAME=$1
CHUCKNAME=$2
DATA=$(printf "%sData" $NAME)

cp -r foo $NAME
cd $NAME
mv foo.cpp $NAME.cpp
sed -i s/foo/$NAME/g $NAME.cpp
sed -i s/FooData/$DATA/g $NAME.cpp
sed -i s/Foo/$CHUCKNAME/g $NAME.cpp 
cd ..