using System;
using System.Collections.Generic;
using System.Net.Sockets;
using System.Threading;
using System.Net;
using System.Linq;
using System.Text;
using System.IO;
using System.Runtime.InteropServices;

namespace SocketNetLib
{
   // [Guid("a83f8440-340f-45cf-b76c-4d2da0352590")]
    public class StateObject
    {
        public Socket workSocket = null;
        public const int BufferSize = 1024;
        public byte[] buffer = new byte[BufferSize];
        public StringBuilder sb = new StringBuilder();
    }
    
    class Srv
    {
        //private Thread m_serverThread;
        private Socket m_serverSocket;
        private string m_serverIP;
         
        String content = String.Empty;
        Client client;

              /// <summary>
        /// 开始服务
        /// </summary>
        /// 
        public bool Start( string iPaddr,int m_serverPort)
        {
            try
            { 
                m_serverSocket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
               // IPEndPoint localEndPoint = new IPEndPoint(IPAddress.Parse(iPaddr), m_serverPort);
                IPEndPoint localEndPoint = new IPEndPoint(IPAddress.Any, m_serverPort);
                m_serverSocket.Bind(localEndPoint);
                m_serverSocket.Listen(10);
                ReceiveAccept();
               // m_serverThread = new Thread(new ThreadStart(ReceiveAccept));//开线程接收连接
                //m_serverThread.Start(); 
                return true;
                
            }
            catch (SocketException se)
            {
               // throw new Exception(se.Message);
                return false;
            }
            catch (Exception ex)
            {
               // throw new Exception(ex.Message);
                return false;
            }
        }
        /// <summary>
        /// 停止服务
        /// </summary>
        private void Stop()
        {
            try
            {
                if (client.ClientSocket != null)
                    client.ClientSocket.Close();
                client.ClientSocket = null;
               // m_serverThread.Abort(); // 线程终止
                if (m_serverSocket != null)
                    m_serverSocket.Close(); // Socket Close   
                m_serverSocket = null;
            }
            catch (Exception ex)
            {
               // throw new Exception(ex.Message);
            }
        }
        private void ReceiveAccept()
        {
          //  while (true)
            {
                client = new Client();
                try
                {
                    client.ClientSocket = m_serverSocket.Accept();
                   // m_serverSocket.BeginAccept(,client.ClientSocket); 
                    
                }
                catch (Exception ex)
                {
                  //  throw new Exception(ex.Message);
                    return;
                }
            }
        }
      
        /// <summary>
        /// 获取本机地址列表
        /// </summary>
        public string[] GetLocalAddresses()
        {
            // 获取主机名
            string strHostName = Dns.GetHostName();
            // 根据主机名进行查找
            IPHostEntry iphostentry = Dns.GetHostEntry(strHostName);
            string[] retval = new string[iphostentry.AddressList.Length];
            int i = 0;
            foreach (IPAddress ipaddress in iphostentry.AddressList)
            {
                retval[i] = ipaddress.ToString();
                i++;
            }
            return retval;
        }

        public void ReadCallback(IAsyncResult ar)
        {
            StateObject state = (StateObject)ar.AsyncState;
            Socket hander = state.workSocket;
             
            int bytesRead = hander.EndReceive(ar);
            if (bytesRead > 0)
            {
                state.sb.Append(Encoding.ASCII.GetString(state.buffer, 0, bytesRead));
                content = state.sb.ToString();
            }
        }
        
   public string recv_( )
        {
            string strReceiveData  = "";
            try
            {
              //  StringBuilder a = new StringBuilder();
                
                
                byte[] receiveBuffer = new byte[1024];
                if (client.ClientSocket != null)
                    client.ClientSocket.Receive(receiveBuffer);
                //StateObject state = new StateObject();
                //state.workSocket = client.ClientSocket;
                //client.ClientSocket.BeginReceive(state.buffer, 0, StateObject.BufferSize, 0, new AsyncCallback(ReadCallback), state);
                
                
              // strReceiveData = Encoding.ASCII.GetString(receiveBuffer);
            }
            catch (Exception ex)
            {
               // throw new Exception(ex.Message);
                return ex.ToString();
            }
          //  System.Threading.Thread.Sleep(1000);
           // return content;
           return strReceiveData;
             
        }

   public int send_(string strSendData)
   {
       try
       {
           int sendBufferSize = Encoding.ASCII.GetByteCount(strSendData);
           byte[] sendBuffer = new byte[sendBufferSize];
           sendBuffer = Encoding.ASCII.GetBytes(strSendData);
           if (client.ClientSocket != null)
               client.ClientSocket.Send(sendBuffer, 0);
       }
       catch (Exception ex)
       {
         //  throw new Exception(ex.Message);
           return -1;
       }
       return 0;
   }

   public void close_()
   {
       Stop();
   }

 }

       

    public class Client
    {
        Socket m_clientSocket;
        public Client() { }
        public Socket ClientSocket
        {
            get { return m_clientSocket; }
            set { this.m_clientSocket = value; }
        }
    }

    // Program

     //[Guid("a83f8440-340f-45cf-b76c-4d2da0352591"), ClassInterface(ClassInterfaceType.None)]
    public class  SocketNetLibProgram  
    { 
          Srv objServer_0;          
         

        public int createSrv(string iPaddr, int port)
         {
             if (objServer_0.Start(iPaddr, port))
                 return 1;
             else
                 return -1;
             
            }
        public string recvMsg()
        {

            return objServer_0.recv_();
        }
           
        public int SendMsg( string buf)
        {
            
                objServer_0.send_(buf);
             
                 return 1;
        }
        public int CloseSrv()
        {
                 objServer_0.close_();
               //  objServer_0 = new Srv();
                return 1;
        }

         public string[] GetCurentIpAddr()
        {
               
                 return objServer_0.GetLocalAddresses();             
        }
        public int GetIpAddrCnt()
         {
             objServer_0 = new Srv();
             string[] IpCnt = objServer_0.GetLocalAddresses();
             return IpCnt.Length;

         }
    }
}

//参数传递
//C# ：Test中有个方法  TestFunc(string str)
//c++中：Test ^test = gcnew Test();  test->TestFunc(p1);  参数p1该怎么传呢，比如我要传aaa？

//CString strTest="aaa";
//System::String^ p1 = gcnew String(strTest);
//test->TestFunc(p1);

 