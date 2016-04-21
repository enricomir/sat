select mh, count(*) from 
(select problem,mh from
(select a.problem, a.mh, a.avg_fit
from
avg_mh a
left join
    (select problem,min(avg_fit) as minfit from avg_mh where mh like 'ed%' group by problem) as b
    on b.problem = a.problem
where a.avg_fit = b.minfit and a.mh like 'ed%') bestpsos
group by problem
having count(*)=1) best_single_psos
group by mh
order by count(*) desc