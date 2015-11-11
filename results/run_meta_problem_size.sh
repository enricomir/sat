#!/usr/bin/fish

for dir in (ls dat/cnf)
	for file in (ls dat/cnf/$dir)
		./bin/opt.app $dir $file
	end
end

