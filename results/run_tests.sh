#!/usr/bin/fish

echo $argv
for x in (seq $argv)
	for dir in (ls dat/cnf)
		for file in (ls dat/cnf/$dir)
			echo $dir $file
			./bin/opt.app $dir $file
		end
	end
end

