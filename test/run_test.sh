#!/bin/bash
# Ansi color code variables
red="\e[0;91m"
blue="\e[0;94m"
expand_bg="\e[K"
blue_bg="\e[0;104m${expand_bg}"
red_bg="\e[0;101m${expand_bg}"
green_bg="\e[0;102m${expand_bg}"
green="\e[0;92m"
white="\e[0;97m"
bold="\e[1m"
uline="\e[4m"
reset="\e[0m"

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


print_help () {
	echo "usage:  ./run_test.sh -[blo]

  --help  prints this help
  -b      test binaries
  -l      test libraries
  -o      test objects";
}

run_comparison () {
	for str in "$@"; do
		name=${str##*/}
		mylog=$LOG"diff_"$name".txt"
		ft_res=$LOG"ft_"$name".txt"
		res=$LOG$name".txt"

		echo -n "$name: "

		./ft_nm $str 1> $ft_res 2> /dev/null
		ftret=$?
		nm $str 1> $res 2> /dev/null
		ret=$?
		diff $ft_res $res > $mylog

		if [ $ftret != $ret ]; then
			echo "bad return value: ft_nm->$ftret - nm->$ret" >> $mylog ; fi
		count=$(wc -c < $mylog)
		if [ $count == "0" ]; then
			echo -e "${green}OK! ✅ ${reset}";
			rm $mylog $ft_res $res;
		else
			echo -e "${red}KO! ❌ ${blue} check this-> ${white}$mylog${reset}";
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
elif [ "$1" == "--help" ]; then print_help;
else print_help;
fi
