package benchmarks;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import dataAccess.SQLiteConnection;

public class Benchmark {
	String name;
	String sizeClass;
	String suiteName;
	String schedulePolicy;
	int numThreadsAvail;
	private double execTime = -1;		//in milliseconds
	private long numInst = 0;
	private long numBranchInst = 0;
	private float branchMissRate = 0;
	private float CPI = 0;
	private int numLoopIter = 0;
	private int numLSInst = 0;		//Load/Store
	private double Mops_total = 0;
	private int nThreadsUsed = 0;
	private float cacheMissRate = -1;
	private float l1dCacheLoadMissRate = -1;
	private String compileCommand;
	private String execPath;
	private String suitePath;		//Inherit NPB benchmark from this class and make this field as static
	private static String perfEvents = "cycles,instructions,branches,branch-misses,cache-references,cache-misses,L1-dcache-loads,L1-dcache-load-misses,page-faults,minor-faults,major-faults,cpu-migrations,context-switches";
	public Map<String,Double> perfMap;
	private String[] envp;
	private static double execTimeSerial = -1;		//not a good design, but let it be for now
	//private long numLoopIter = 0;
	//private long loopIterDepth = 0;
	double speedUp = 0;
		
	public Benchmark(String suiteName, String name, String sizeClass, int numThreads, String schedulePolicy){
		this.suiteName = suiteName;
		this.name = name;
		this.sizeClass = sizeClass;
		this.numThreadsAvail = numThreads;
		this.schedulePolicy = schedulePolicy;
		if(suiteName.equalsIgnoreCase("NPB")){
			compileCommand = "make "+name+" CLASS="+ sizeClass;
			suitePath = NPBStats.m_BenchMarkSuitePath;
			execPath = "./bin/"+name.toLowerCase()+"."+sizeClass.toUpperCase()+".x" ;
		}	
		else{
			String outputFileName = getOutputFileName(name);
			compileCommand = "gcc -fopenmp "+name+" -lm -o "+outputFileName;
			suitePath = OpenMPProgramStats.programsDirectory;
			execPath = "./"+outputFileName;
		}
		perfMap = new HashMap<String, Double>(perfEvents.length());
		envp = new String[2];
		envp[0] = "OMP_NUM_THREADS="+numThreadsAvail;
		envp[1] = "OMP_SCHEDULE="+schedulePolicy;
	}
	
	public int generateExecutable(){
		List<String> output = new ArrayList<String>();
		return BashInteractor.Execute(compileCommand, output, null, suitePath);
	}
	
	private static String getOutputFileName(String inputFileName){
		int extIndex = inputFileName.lastIndexOf('.');
		return inputFileName.substring(0, extIndex)+".out";
	}
	
	public double getExecTime() {
		return execTime;
	}

	public long getNumInst() {
		return numInst;
	}

	public long getNumBranchInst() {
		return numBranchInst;
	}

	public float getBranchMissRate() {
		return branchMissRate;
	}
	
	public float getcacheMissRate() {
		return cacheMissRate;
	}

	public float getCPI() {
		return CPI;
	}

	public int getNumLoopIter() {
		return numLoopIter;
	}

	public int getNumLSInst() {
		return numLSInst;
	}

	public double getMops_total() {
		return Mops_total;
	}

	public int getThreadsUsed() {
		return nThreadsUsed;
	}

	
	public void profileAndPerfStats(){
		List<String> output = new ArrayList<String>();
		int result = 0;
		//if(suiteName.equalsIgnoreCase("NPB")){
			result = BashInteractor.Execute(compileCommand, output, null, suitePath);
		//}	
		if(result != -1){
			output = new ArrayList<String>();
			//String envp[] = {"OMP_NUM_THREADS="+numThreadsAvail, "OMP_SCHEDULE="+schedulePolicy};
			//String execCommand = "./"+execPath;//+" "+numThreadsAvail;
			if(BashInteractor.Execute(execPath, output, envp, suitePath) != -1){
				System.out.println("check printing");
				extractRunTimeInfo(output);
				gatherPerfStats(execPath);
				//runLLVMExtractor();	//enable once the script is available
			}
		}
		if(numThreadsAvail == 1){
			execTimeSerial = execTime;
		}
		speedUp = execTimeSerial/execTime;
	}		
	
	private void runLLVMExtractor(){
		List<String> output = new ArrayList<String>();
		BashInteractor.Execute(OpenMPProgramStats.llvmScriptName, output, null, suitePath);
		extractLLVMInfo(output);
	}
	
	private void extractLLVMInfo(List<String> output){
		for(int i=0;i<output.size();i++){
			if(output.get(i).contains("Number of loop iterations")){
				numLoopIter = Integer.parseInt(extractValueFromAssignmentString(output.get(i)));
			}
		}	
	}
	
	private void extractRunTimeInfo(List<String> output){		//parse the output and extract runtime Info
		if(suiteName.equalsIgnoreCase("NPB")){
			for(int i=0;i<output.size();i++){
				if(output.get(i).contains("Time in seconds")){
					execTime = Float.parseFloat(extractValueFromAssignmentString(output.get(i)));
				}
				if(output.get(i).contains("Mop/s total")){
					Mops_total = Float.parseFloat(extractValueFromAssignmentString(output.get(i)));
				}
				if(output.get(i).contains("Total threads")){
					nThreadsUsed = Integer.parseInt(extractValueFromAssignmentString(output.get(i)));
				}
				if(output.get(i).contains("Avail threads")){
					numThreadsAvail = Integer.parseInt(extractValueFromAssignmentString(output.get(i)));
				}
			}
		}
		else{
			nThreadsUsed = numThreadsAvail;
			for(int i=0;i<output.size();i++){
				if(output.get(i).contains("MegaOPS/dT")){
					Mops_total = Float.parseFloat(extractValueFromAssignmentString(output.get(i)));
				}	
			}
		}
	}
	
	private void gatherPerfStats(String execCommand){
		List<String> perfOutput = new ArrayList<String>(); 
		if(Perf.getStats(execCommand, envp, suitePath, perfOutput,perfEvents) != -1){
			extractPerfInfo(perfOutput);
		}
	}
	
	private void extractPerfInfo(List<String> perfOutput){
		List<String> events = new ArrayList<String>(Arrays.asList(perfEvents.split(",")));
		events.add("seconds time elapsed");
		events.set(events.indexOf("cpu-migrations"), "CPU-migrations");
		for(int i=0;i<events.size();i++){
			for(int j=0;j<perfOutput.size();j++){
				if(perfOutput.get(j).contains(events.get(i))){
					double value = Double.parseDouble(extractValueFromPrefixedString(perfOutput.get(j), events.get(i)));
					perfMap.put(events.get(i), value);
					events.remove(i);
					i--;
					break;
				}
			}	
		}
		if(!suiteName.equalsIgnoreCase("NPB")){
			execTime = perfMap.get("seconds time elapsed");
		}
		double cycles = perfMap.get("cycles");
		double instructions = perfMap.get("instructions");
		CPI = (float) (cycles/instructions);
		perfMap.put("CPI", (double)CPI);
		//double l1Loads = perfMap.get("L1-dcache-loads");
		//double l1LoadMisses = perfMap.get("L1-dcache-load-misses");
		//l1CacheMissRate = (float)(l1LoadMisses/l1Loads);
		//perfMap.put("l1-dcache-load-miss-rate", (double)l1CacheMissRate);
		l1dCacheLoadMissRate = (float)(perfMap.get("L1-dcache-load-misses")/perfMap.get("L1-dcache-loads"));
		cacheMissRate = (float) (perfMap.get("cache-misses")/perfMap.get("cache-references"));
		branchMissRate = (float) (perfMap.get("branch-misses")/perfMap.get("branches"));
	}
	
	private static String extractValueFromPrefixedString(String str, String parameter){
		int index;
		index = str.indexOf(parameter);
		String value = str.substring(0, index);
		return value.trim();
	}
	
	private static String extractValueFromAssignmentString(String str){
		int equalIndex = str.lastIndexOf('=');
		String value = str.substring(equalIndex + 1).trim();
		return value.trim();
	}
	
	public void saveStatsToDB(String dbPath){
		SQLiteConnection connection = new SQLiteConnection(dbPath);
		String query = "INSERT INTO benchmarkstats ( "+ getInsertFields()+" ) VALUES ( ";//+suiteName;
		query = query + getInsertValuesInSql()+ " )";
		try{
			connection.executeUpdateQuery(query);
		}catch(Exception e){
			
		}
	}
	
	private static String getInsertFields(){
		String fields = "";
		for(int i=1;i<TableBenchMarkStats.fields.length;i++){
			fields = fields + ","+TableBenchMarkStats.fields[i];
		}	                      
		fields = fields.trim().substring(1);//rereplaceFirst("," , "");
		fields = fields+","+"speed_up";
		return fields;
	}
	
	private String getInsertValuesInSql(){
			//String values = "";
		String values = "'"+suiteName+"'"+",";
		values += "'"+name+"'"+",";
		values += "'"+sizeClass+"'"+",";
		values += nThreadsUsed+",";
		values += numThreadsAvail+",";
		values += execTime+",";
		values += Mops_total+",";
		values += Mops_total/nThreadsUsed +",";
		values += perfMap.get("instructions")+",";
		values += perfMap.get("branches")+",";
		values += branchMissRate+",";
		values += cacheMissRate+",";
		values += l1dCacheLoadMissRate+",";
		values += CPI+",";
		values += perfMap.get("CPU-migrations")+",";
		values += "'"+schedulePolicy+"'"+",";
		values += speedUp;
		return values;
	}
	
	
	public void saveToFile(FileUtil fu){
		fu.appendToFile("SuiteName                   - "+suiteName);
		fu.appendToFile("Benchmark - "+name+" CLASS  - "+sizeClass);
		fu.appendToFile("Number of available threads - "+numThreadsAvail);
		fu.appendToFile("Threads Used                - "+nThreadsUsed+"\n");
		fu.appendToFile("-------------------------------------------------------------------");
		fu.appendToFile("Perf and profile stats \n");
		fu.appendToFile("Execution Time              - "+execTime);
		fu.appendToFile("Mop/s total                 - "+Mops_total);
		fu.appendToFile("Mop/s/thread                - "+Mops_total/nThreadsUsed);
		fu.appendToFile("Total Instructions          - "+perfMap.get("instructions"));
		fu.appendToFile("Branch Instructions         - "+perfMap.get("branches"));
		fu.appendToFile("Branch Miss Rate            - "+branchMissRate);
		fu.appendToFile("CPI                         - "+CPI);
		fu.appendToFile("CPU-migration               - "+perfMap.get("cpu-migration"));
		fu.appendToFile("Context Switches            - "+perfMap.get("context-switches"));
		fu.appendToFile("\n\n");
	}
}
