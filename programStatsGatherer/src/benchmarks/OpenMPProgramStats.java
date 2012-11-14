package benchmarks;

public class OpenMPProgramStats {
	public static String programsDirectory = System.getProperty("user.dir")+"/openMPPrograms";
	private static String[] programs = {//"compute_pi.c",
										//"dot_product.c",
										//"mandelbrot_openmp.c",
										//"mxm.c",
										//"mxv_openmp.c",
										//"prime_openmp.c"
										//"quad_openmp.c",
										//"random_openmp.c",
										//"omptransposeIP.c",
										"omptransposeOP.c"
										};
	private static String[] schedulePolicies = {"static",
		
											"dynamic",//1",
											//"dynamic,2",
											//"dynamic,4",
											
											//"guided,1",
											//"guided,2",
											"guided,4"
											};
	
	private static String dbPath = "ProfileStats.db";
	private static int max_num_threads = 32;
	public static String llvmScriptName = "";
	
	public static void runAll(){
		for(int i=0;i<programs.length;i++){
			runSpecificProgram(programs[i], programsDirectory);
		}
	}
	
	public static void runSpecificProgram(String programName, String programDirectory){
			
			
			for(int sp=0;sp<schedulePolicies.length;sp++){
				for(int numThreads=1;numThreads<=max_num_threads;numThreads++){
				System.gc();
				Benchmark bm = new Benchmark("OpenMPPrograms", programName, "", numThreads, schedulePolicies[sp]);
				bm.profileAndPerfStats();
				bm.saveStatsToDB(dbPath);
			}
		}	
	}

	
}
