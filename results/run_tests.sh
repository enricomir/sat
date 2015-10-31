#!/usr/bin/fish

for x in (seq 30)
	for dir in (ls dat/cnf)
		for file in (ls dat/cnf/$dir)
			./bin/opt.app $dir $file
		end
	end
end

