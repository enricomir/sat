#!/bin/bash

for i in $(seq 0 9)
do
	rm stage$i.ql	
	cp stage.ql.org stage$i.ql
done

