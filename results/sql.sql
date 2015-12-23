select mh, sum(is_best) as problemas from best_single_mhs group by mh order by sum(is_best) desc

select problem, count(*), 15*6 as target from eda group by problem

select count(*), 15*6*555 as target from eda
