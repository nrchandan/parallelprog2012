package benchmarks;

public class TableBenchMarkStats {
	
	public static String[] fields = {"stat_id",
									"benchmark_suite",
									"benchmark_name",
									"size_class",
									"num_threads_used",
									"num_threads_avail",
									"exec_time",
									"mops_per_sec_total",
									"mops_per_sec_per_thread",
									"inst_total",
									"branch_inst_total",
									"branch_miss_rate",
									"cache_miss_rate",
									"l1_dcache_load_miss_rate",
									"cpi",
									"cpu_migrations",
									"schedule_policy",
									};
	
	
	
	public static boolean existsInDB(int statId){
		return true;
	}
	
	public static int getPK(String benchmarkSuite, String benchmarkName, String sizeClass){
		return 0;
	}
	
	

}
