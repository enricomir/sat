select
	pro.clauses/1500,
	pro.variables/300,
	pro.ratio/10,
	pro.inv_ratio,
	pro.normalized_ratio/10,

	pro.cptn_avg,
	pro.cptn_stddev,
	pro.cptn_vc,
	pro.cptn_min,
	pro.cptn_max,
	pro.cptn_ent,

	pro.vptn_avg,
	pro.vptn_stddev,
	pro.vptn_vc,
	pro.vptn_min,
	pro.vptn_max,
	pro.vptn_ent,

	pro.unary_c,
	pro.binary_c,
	pro.ternary_c,

	po.size/50,
	
	po.euc_avg,
	po.euc_stddev,
	po.euc_vc,
	po.euc_min,
	po.euc_max,
	po.euc_ent,

	po.ham_avg/100,
	po.ham_stddev,
	po.ham_vc,
	po.ham_min/100,
	po.ham_max/100,
	po.ham_ent,

	po.sha_avg,
	po.sha_stddev,
	po.sha_vc,
	po.sha_min,
	po.sha_max,
	po.sha_ent,

	po.simp_avg,
	po.simp_stddev,
	po.simp_vc,
	po.simp_min,
	po.simp_max,
	po.simp_ent,

	po.prob_avg,
	po.prob_stddev,
	po.prob_vc,
	po.prob_min,
	po.prob_max,
	po.prob_ent,

	rb.best_mh
from
	runs_bests rb
inner join problem_meta pro on rb.problem = pro.problem
inner join population_meta po on rb.pop = po.population
where
	rb.num_best=1