package dataAccess;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.ResultSet;
import java.sql.Statement;

public class SQLiteConnection {
	
	Connection connection = null;
	
	public SQLiteConnection(String dbPath){
		try{
		 Class.forName("org.sqlite.JDBC");  
         connection = DriverManager  
                 .getConnection("jdbc:sqlite:"+dbPath);
		}catch(Exception e){
			e.printStackTrace();
		}
	}
	
	public int executeUpdateQuery(String query){
		int success = 0;
		try{
		Statement statement = connection.createStatement();
		success = statement.executeUpdate(query);
		}catch(Exception e){
			e.printStackTrace();
		}
		return success;
	}
	
	
	/*public static void main(String[] args){
		 Connection connection = null;  
	        ResultSet resultSet = null;  
	        Statement statement = null;  
	  
	        try {  
	            Class.forName("org.sqlite.JDBC");  
	            connection = DriverManager  
	                    .getConnection("jdbc:sqlite:ProfileStats.db");  
	            statement = connection.createStatement();  
	            resultSet = statement.executeQuery("SELECT * FROM benchmarkstats");  
	            while (resultSet.next()) {  
	                System.out.println("EMPLOYEE NAME:"  
	                        + resultSet.getString("EMPNAME"));  
	            }  
	        } catch (Exception e) {  
	            e.printStackTrace();  
	        } finally {  
	            try {  
	                resultSet.close();  
	                statement.close();  
	                connection.close();  
	            } catch (Exception e) {  
	                e.printStackTrace();  
	            }  
	        }  
	    }*/
	}

