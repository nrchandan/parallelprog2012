package benchmarks;

import java.util.List;

public class Perf {
	
	public static String[] perfList =  
						   {"branches",
						    "cycles",                   //   [Hardware event]
                            "instructions",                             //  [Hardware event]
                            "cache-references",                         //  [Hardware event]
                            "cache-misses",                            //  [Hardware event]
                            "branch-instructions OR branches",          //  [Hardware event]
                            "branch-misses",                            //  [Hardware event]
                            "bus-cycles",                               //  [Hardware event]

                            "cpu-clock",                                //  [Software event]
                            "task-clock",                               //  [Software event]
                            "page-faults",			                   //  [Software event]
                            "minor-faults",                             //  [Software event]
                            "major-faults",                             //  [Software event]
                            "context-switches",                         //  [Software event]
                            "cpu-migrations",			               //  [Software event]

                            "L1-dcache-loads",                          //  [Hardware cache event]
                            "L1-dcache-load-misses",                    //  [Hardware cache event]
                            "L1-dcache-stores",                         //  [Hardware cache event]
                            "L1-dcache-store-misses",                   //  [Hardware cache event]
                            "L1-dcache-prefetches",                     //  [Hardware cache event]
                            "L1-dcache-prefetch-misses",                //  [Hardware cache event]
                            "L1-icache-loads",                          //  [Hardware cache event]
                            "L1-icache-load-misses",                    //  [Hardware cache event]
                            "L1-icache-prefetches",                     //   [Hardware cache event]
                            "L1-icache-prefetch-misses",                //  [Hardware cache event]
                            "LLC-loads",                                // [Hardware cache event]
                            "LLC-load-misses",                          //  [Hardware cache event]
                            "LLC-stores",                               //  [Hardware cache event]
                            "LLC-store-misses",                         //  [Hardware cache event]
                            "LLC-prefetches",                           //  [Hardware cache event]
                            "LLC-prefetch-misses",                      //  [Hardware cache event]
                            "dTLB-loads",                               //  [Hardware cache event]
                            "dTLB-load-misses",                         //  [Hardware cache event]
                            "dTLB-stores",                              //  [Hardware cache event]
                            "dTLB-store-misses"};
	
	public static int getStats(String command, String[] envp, String directory, List<String> perfStats, String perfEvents){
		command = "perf stat -e "+perfEvents+" -c --log-fd 1 "+ command;
		System.out.println(command);
		if(BashInteractor.Execute(command, perfStats, envp, directory) != -1){
			return 0;
		}
		return -1;
	}
}
