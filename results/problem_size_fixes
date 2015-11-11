update meta_problem_size set ratio = cast(clauses as real)/variables, rec_ratio = cast(variables as real)/clauses  where ratio is null;
update meta_problem_size set ratio_sq = ratio*ratio, ratio_cube = ratio*ratio*ratio where ratio_sq is null;
update meta_problem_size set rec_ratio_sq = rec_ratio * rec_ratio, rec_ratio_cube = rec_ratio*rec_ratio*rec_ratio where rec_ratio_sq is null;
update meta_problem_size set lin_ratio = abs(4.6-ratio) where lin_ratio is null;
update meta_problem_size set lin_ratio_sq = lin_ratio * lin_ratio, lin_ratio_cube = lin_ratio*lin_ratio*lin_ratio where lin_ratio_sq is null;

select * from meta_problem_size;