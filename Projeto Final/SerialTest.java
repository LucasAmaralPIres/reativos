package Arduino;

import java.sql.*;
import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.io.OutputStream;
//import gnu.io.*;
import gnu.io.CommPortIdentifier; 
import gnu.io.SerialPort;
import gnu.io.SerialPortEvent; 
import gnu.io.SerialPortEventListener; 
import java.util.Enumeration;

import com.sun.org.apache.xalan.internal.utils.Objects;


public class SerialTest implements SerialPortEventListener 
{
	SerialPort serialPort;
        /** The port we're normally going to use. */
	private static final String PORT_NAMES[] = 
	{ 
			"/dev/tty.usbserial-A9007UX1", // Mac OS X
                        "/dev/ttyACM0", // Raspberry Pi
			"/dev/ttyUSB0", // Linux
			"COM3", // Windows
	};
	/**
	* A BufferedReader which will be fed by a InputStreamReader 
	* converting the bytes into characters 
	* making the displayed results codepage independent
	*/
	private BufferedReader input;
	/** The output stream to the port */
	private OutputStream output;
	/** Milliseconds to block while waiting for port open */
	private static final int TIME_OUT = 2000;
	/** Default bits per second for COM port. */
	private static final int DATA_RATE = 9600;

	public void initialize() 
	{
		CommPortIdentifier portId = null;
		Enumeration portEnum = CommPortIdentifier.getPortIdentifiers();

		//First, Find an instance of serial port as set in PORT_NAMES.
		while (portEnum.hasMoreElements()) 
		{
			CommPortIdentifier currPortId = (CommPortIdentifier) portEnum.nextElement();
			for (String portName : PORT_NAMES) 
			{
				if (currPortId.getName().equals(portName)) 
				{
					portId = currPortId;
					break;
				}
			}
		}
		if (portId == null) 
		{
			System.out.println("Could not find COM port.");
			return;
		}
//		System.out.println("AQUI.");
		try 
		{
			// open serial port, and use class name for the appName.
			serialPort = (SerialPort) portId.open(this.getClass().getName(),
					TIME_OUT);

			// set port parameters
			serialPort.setSerialPortParams(DATA_RATE,
					SerialPort.DATABITS_8,
					SerialPort.STOPBITS_1,
					SerialPort.PARITY_NONE);

			// open the streams
			input = new BufferedReader(new InputStreamReader(serialPort.getInputStream()));
			output = serialPort.getOutputStream();
			// add event listeners
			serialPort.addEventListener(this);
			serialPort.notifyOnDataAvailable(true);
		}
		catch (Exception e) 
		{
			System.err.println(e.toString());
		}
	}

	/**
	 * This should be called when you stop using the port.
	 * This will prevent port locking on platforms like Linux.
	 */
	public synchronized void close() 
	{
		if (serialPort != null) 
		{
			serialPort.removeEventListener();
			serialPort.close();
		}
	}

	/**
	 * Handle an event on the serial port. Read the data and print it.
	 */
	public synchronized void serialEvent(SerialPortEvent oEvent) 
	{
		if (oEvent.getEventType() == SerialPortEvent.DATA_AVAILABLE) 
		{
			try 
			{
				String inputLine=input.readLine();
				System.out.println(inputLine);
				if(edicao == true)
				{
					Professor(inputLine);
				}
				BuscarAluno(inputLine);
//				output.write(1);
			} catch (Exception e) {
//				System.err.println(e.toString());
			}
		}
		// Ignore all the other eventTypes, but you should consider the other ones.
	}
	private boolean jaProf = false;
	private boolean isAdd = false;
	private String num_cartao_add;
	public void Professor(String st)
	{
		boolean resposta = false;
		try
		{
			if(jaProf == false)
			{
				String query = "Select * from Pessoa where nCartao = '" + st + "' and isProf = 1;";
				resposta = executarSQL(query);
				if(resposta == true)
				{
					output.write(4);
					jaProf = true;
				}
				else
				{
					edicao = false;
					output.write(1);
				}
			}
			else
			{
				if(isAdd == false)
				{
					String query = "Select * from Pessoa where nCartao = '" + st + "';";
					resposta = executarSQL(query);
					if(resposta == true)
					{
						String query2 = "Delete from pessoa where nCartao = '" + st + "';";
						executarSQLUpdate(query2);
						edicao = false;
						jaProf = false;
						output.write(1);
					}
					else
					{
						num_cartao_add = st;
						isAdd = true;
						output.write(4);
					}
				}
				else
				{
					String query = "Insert into Pessoa (matricula, nCartao, isProf) values ('" + st + "', '" + num_cartao_add + "', false);";
					executarSQLUpdate(query);
					isAdd = false;
					edicao = false;
					jaProf = false;
				}
			}
		}
		catch(Exception e)
		{
	//		System.err.println(e.toString());
		}		
	}
	
    private static boolean executarSQL(String sql) throws Exception
    {
		boolean r;
    	ResultSet rs = null;
		Connection conn=Conexao.abrirConexao();
	    Statement stat=conn.createStatement();
	    rs=stat.executeQuery(sql);
	    r = rs.first();
    	stat.close();
    	conn.close();
    	return r;
    }
    private static void executarSQLUpdate(String sql) throws Exception
    {
		Connection conn=Conexao.abrirConexao();
	    Statement stat=conn.createStatement();
	    stat.executeUpdate(sql);
    	stat.close();
    	conn.close();
    }
	private boolean edicao = false;
	public int BuscarAluno (String st)
	{
		if(Objects.equals("!", st))
		{
			try
			{
				if(!edicao)
				{
					output.write(3);
				}
				else
				{
					output.write(1);
				}
				edicao = !edicao;
			}
			catch (Exception e)
			{
		//		System.err.println(e.toString());
			}
		}
		else if(edicao == true)
		{
			
		}
		else
		{
			boolean resposta = false;
			String query = "Select * from Pessoa where matricula = '" + st + "' or nCartao = '" + st + "';";
			try
			{
				resposta = executarSQL(query);
				if(resposta == true)
				{
					output.write(2);
				}
				else
				{
					output.write(1);
				}
			}
			catch(Exception e)
			{
			//	System.err.println(e.toString());
			}
		}
		return 0;
	}
	
	public static void main(String[] args) throws Exception
	{
//		Connection c=Conexao.abrirConexao();
//		System.out.println(c);
		SerialTest main = new SerialTest();
		main.initialize();
		Thread t=new Thread() 
		{
			public void run() 
			{
				//the following line will keep this app alive for 1000 seconds,
				//waiting for events to occur and responding to them (printing incoming messages to console).
				try {Thread.sleep(1000000);
				} catch (InterruptedException ie) {}
			}
		};
		t.start();
		System.out.println("Started");
	}
}
