package benchmarks;

import java.io.File;
import java.io.FileWriter;
import java.io.IOException;

public class FileUtil {
	File f;
	
	public FileUtil(String fileName){
		f = new File(fileName);
	}
	
	public void delete(){
		if(f.exists()){
			f.delete();	
		}
	}
	
	public int appendToFile(String content){
		FileWriter fw; 
		//PrintStream p;
		try {
			
			fw = new FileWriter(f, true);
			//out = new BufferedWriter(fw);
		} catch (IOException e) {
			// TODO Auto-generated catch block
			System.out.println("Error Creating/Opening file in write mode - "+ f.getAbsolutePath());
			return 1;
		}
		try {
			fw.append(content+"\n");
			//fw.newLine();
		} catch (Exception e) {
			// TODO Auto-generated catch block
			System.out.println("Error Writing to File - " + f.getAbsolutePath());
			return 1;
		}
		finally{
			try {
				fw.close();
			} catch (Exception e) {
				// TODO Auto-generated catch block
				System.out.println("Error closing fileWriter");
			}
		}
		
		return 0;		//success
	}

}