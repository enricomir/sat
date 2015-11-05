#!/usr/bin/fish

for dir in (ls dat/cnf)
	for file in (ls dat/cnf/$dir)
		for x in (seq 10)
			./bin/opt.app $dir $file
		end
	end
end

