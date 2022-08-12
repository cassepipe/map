#!/usr/bin/zsh

for i in `seq 1 10`
do
	if [[ -f tree$i.dot ]]
	then
		xdot tree$i.dot
	fi
done
