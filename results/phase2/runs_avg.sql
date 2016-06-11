truncate runs_avg;
insert into runs_avg select mh, stage, problem, pop, avg(fitness) from runs
group by mh, stage, problem, pop;