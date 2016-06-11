truncate runs_bests;
insert into runs_bests
select stage, problem, pop, min(avg_fit),
(select rb.mh from runs_avg rb where rb.stage=ra.stage and rb.pop = ra.pop and rb.problem = ra.problem
and rb.avg_fit = min(ra.avg_fit) limit 1
),
(select count(*) from runs_avg rb where rb.stage=ra.stage and rb.pop = ra.pop and rb.problem = ra.problem
and rb.avg_fit = min(ra.avg_fit) )
 from runs_avg ra

group by stage, problem, pop