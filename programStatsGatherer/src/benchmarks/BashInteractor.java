package benchmarks;

import java.io.BufferedReader;
//import java.io.File;
import java.io.File;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.List;

public class BashInteractor {
	
	public static long Execute(String[] cmdAry, String output){
		//File curDir = new File(fileDir);
		//String[] envp= {"bash"}; 
		long timeCounter = -1;
		
		Process proc = null;
		try{
			long start = System.nanoTime();
			for(int i=0;i<cmdAry.length;i++){
			proc = Runtime.getRuntime().exec(cmdAry[i]);//, envp, curDir);
			try {
				proc.waitFor();
				timeCounter = System.nanoTime() - start;
			} catch (InterruptedException e1) {
				// TODO Auto-generated catch block
				e1.printStackTrace();
			}
			timeCounter = System.nanoTime() - start;
			InputStreamReader stream = new InputStreamReader(proc.getInputStream());
			InputStreamReader errStream = new InputStreamReader(proc.getErrorStream());
			BufferedReader read = new BufferedReader(stream);
			BufferedReader errRead = new BufferedReader(errStream);
			try{
			//try{
				//proc.waitFor();
			//	timeCounter = System.nanoTime() - start;
		//	}
		//	catch(InterruptedException e) {
			//	System.out.println(e.getMessage());
				//return -1;
			//}
			//System.out.println("Process output");	
			while(read.ready()){
				System.out.println(read.readLine());
			}
			//System.out.println("Process error stream");
			while(errRead.ready()){
				System.out.println(errRead.readLine());
			}
			}finally{
				read.close();
				if(stream != null){
					try{
						stream.close();
					}catch (IOException e) {
                         e.printStackTrace();
                    }
				}
			}
			}
		}
		catch(IOException e){
			System.out.println(e.getMessage());
			return -1;
		}
		
		finally{
			proc.destroy();
		}
		return timeCounter;
	}	
	
	public static int Execute(String command, List<String> output, String[] envp, String directory){
		
		Process proc = null;
		int returnCode = 0;
		System.out.println("Command executed - "+command);
		File dir;
		if (directory == null){
			dir = null;
		}
		else{
			dir = new File(directory);
		}	
		try{
		
			proc = Runtime.getRuntime().exec(command, envp, dir);
			//timeCounter = System.nanoTime() - start;
			try {
				proc.waitFor();
			} catch (InterruptedException e1) {
				// TODO Auto-generated catch block
				returnCode = -1;
				e1.printStackTrace();
			}
			InputStreamReader stream = new InputStreamReader(proc.getInputStream());
			InputStreamReader errStream = new InputStreamReader(proc.getErrorStream());
			BufferedReader read = new BufferedReader(stream);
			BufferedReader errRead = new BufferedReader(errStream);
			try{
					while(read.ready()){
					output.add(read.readLine());
					System.out.println(output.get(output.size()-1));
				}
				//System.out.println("Process error stream");
				while(errRead.ready()){
					output.add(errRead.readLine());
					System.out.println(output.get(output.size()-1));
				}
			}finally{
				errRead.close();
				read.close();
				if(stream != null){
					try{
						stream.close();
					}catch (IOException e) {
                         e.printStackTrace();
                    }
				}
				if(errStream != null){
					try{
						errStream.close();
					}catch (IOException e) {
                         e.printStackTrace();
                    }
				}
			}
			
		}catch(IOException e){
			System.out.println(e.getMessage());
			returnCode = -1;
		}
		finally{
			proc.destroy();
			return returnCode;
		}
		//return -1;
	}

}
