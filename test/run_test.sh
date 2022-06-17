#!/bin/bash

##### PATH #####
curr_dir=$PWD
dir=${curr_dir##*/}

if [ $dir = 'test' -o $dir = 'libft' -o $dir = 'srcs' ]; then
	cd ..
elif [ $dir = 'ft_nm' ]; then
	cd .
else
	echo "you are'nt in a good directory, go to ft_nm/";
	exit;
fi

if [ ! -d test/log ]; then mkdir test/log; fi


BIN="test/bin/"
LIB="test/lib/"
OBJ="test/obj/"
LOG="test/log/"

bins64=($BIN*_x64)
bins32=($BIN*_x32)
libs64=($LIB*_x64.so)
libs32=($LIB*_x32.so)
objs64=($OBJ*_x64.o)
objs32=($OBJ*_x32.o)


help () {
	echo "
 usage:  ./run_test.sh -[blo]

   --help  prints this help
   -b      test binaries
   -l      test libraries
   -o      test objects
";
}

run_comparison () {
	for str in "$@"; do
		name=${str##*/}
		mylog=$LOG"diff_"$name".txt"
		ft_res=$LOG"ft_"$name".txt"
		res=$LOG$name".txt"

		echo -n "$name: "

		./ft_nm $str > $ft_res
		ftret=$?
		nm $str > $res
		ret=$?
		diff $ft_res $res > $mylog

		if [ $ftret != $ret ]; then
			echo "bad return value: ft_nm->$ftret - nm->$ret" >> $mylog ; fi
		count=$(wc -c < $mylog)
		if [ $count == "0" ]; then
			echo "OK!";
			rm $mylog $ft_res $res;
		else
			echo "KO! check this-> $mylog";
		fi
	done
}

rm -f $LOG*.txt

if [ "$1" == "-b" ]; then
	run_comparison ${bins64[@]};
	run_comparison ${bins32[@]};
elif [ "$1" == "-l" ];then
	run_comparison ${libs64[@]};
	run_comparison ${libs32[@]};
elif [ "$1" == "-o" ]; then
	run_comparison ${objs64[@]};
	run_comparison ${objs32[@]};
elif [ "$1" == "--help" ]; then help;
else help;
fi
