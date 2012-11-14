package benchmarks;

public class NPBStats {
	
	//private static final String[] m_dataSizes = {"S","W", "A", "B", "C", "D"};
	private static final String[] m_dataSizes = { "W", "A", "B"};
	//private static final String[] m_benchmarkName = {"bt", "cg", "dc", "ep", "ft", "is", "lu", "mg", "sp", "ua"};
	private static final String[] m_benchmarkName = {"ft"};
	private static String[] schedulePolicies = {"static","dynamic","guided"};
	public static String m_BenchMarkSuitePath = System.getProperty("user.dir") + "/NPB3.3-OMP-C";
	private static String m_outputFilePath;
	private static FileUtil fu;
	private static int max_num_threads = 16; 
	private static String StatsDB = "ProfileStats.db";
		
	public static void InitNPB(){
		//m_BenchMarkSuitePath = "/home/nitin/IIIT-H/Parallel Programming/NPB3.3-OMP-C";
	}
	
	public static void runEntireSuite(){
		for(int j=0;j<m_benchmarkName.length;j++){
			fu = new FileUtil(m_BenchMarkSuitePath+"_"+m_benchmarkName[j]+"\n");
			fu.delete();
			//System.gc();
			for(int i=0;i<m_dataSizes.length;i++){
				fu.appendToFile("DataSize - "+m_dataSizes[i]+"\n");
				//Benchmark bm = new Benchmark("NPB", m_benchmarkName[j], m_dataSizes[i]);
				runSpecificBenchmark(m_benchmarkName[j], m_dataSizes[i]);
			}
		}	
	}
	
	//private void get makeFileCommand
	public static void runSpecificBenchmark(String benchMarkName, String dataSize){
		/*String[] output = new String[500];
		String makeCommand = "make "+benchMarkName+" CLASS="+ dataSize; 
		if(BashInteractor.Execute(makeCommand, output, m_BenchMarkSuitePath) != -1){
			setOutputFilePath(benchMarkName, dataSize);
			if(BashInteractor.Execute(m_outputFilePath, output, m_BenchMarkSuitePath) != -1){
				processOutput(output);
			}	*/
			
				for(int sp=0;sp<schedulePolicies.length;sp++){
					for(int nThreads=1;nThreads<=max_num_threads;nThreads++){
					Benchmark bm = new Benchmark("NPB", benchMarkName, dataSize, nThreads, schedulePolicies[sp]);
					bm.profileAndPerfStats();
					bm.saveStatsToDB(StatsDB);
					bm.saveToFile(fu);
					System.gc();
					/*output = new String[500];
					if(BashInteractor.Execute(m_outputFilePath+" "+nThreads, output, m_BenchMarkSuitePath) != -1){
						processOutput(output);
						if(nThreads == 1){ 
							String[] perfStats = new String[50];
							Perf.getStats(m_outputFilePath, m_BenchMarkSuitePath, perfStats);
						}
					}*/
				}
			}
		//}	
	}
	
	/*private static void processOutput(String[] outStrings){
		for(int i=0;i<outStrings.length;i++){
			if(outStrings[i] == null){
				break;
			}
			if(outStrings[i].contains("Time in seconds")){
				fu.appendToFile(outStrings[i]);
			}
			if(outStrings[i].contains("Mop")){
				fu.appendToFile(outStrings[i]);
			}
			if(outStrings[i].contains("threads")){
				fu.appendToFile(outStrings[i]);
			}
			/*if(outStrings[i].contains("time")){
				fu.appendToFile(outStrings[i]);
			}
			if(outStrings[i].contains("time")){
				fu.appendToFile(outStrings[i]);
			}
			if(outStrings[i].contains("time")){
				fu.appendToFile(outStrings[i]);
			}
		}
	}*/
	
	
	private static void setOutputFilePath(String benchMarkName, String dataSize){
		//m_outputFilePath = m_BenchMarkSuitePath+"/bin/"+benchMarkName.toLowerCase()+"."+dataSize.toUpperCase()+".x" ; 
		m_outputFilePath = "./bin/"+benchMarkName.toLowerCase()+"."+dataSize.toUpperCase()+".x" ;
	}
	
	public static void NPBRun(){
		InitNPB();
		runEntireSuite();
	}
}
