using System;
using System.Diagnostics;
using System.IO;
using System.Net;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Cluster_DNS_Robot
{
    static class Program
    {
        static Task _task;

        static void Main()
        {
            _task = new Task(Clustering);
            _task.Start();

            while (true)
            {
                Console.ReadKey();
            }
        }

        static void Clustering()
        {
            ExecuteCommand("ipconfig /flushdns");

            var ip = "";
            var dns = new Dns("localhost", null, null);
            var col = dns.GetARecord("loop.com", "www");
            foreach (var o in col)
                ip = o.Properties["IPAddress"].Value.ToString();

            Console.WriteLine("Current IP : " + ip);
            while (true)
            {
                var exception = false;
                HttpWebResponse response = null;
                try
                {
                    var webRequest = (HttpWebRequest)WebRequest.Create("http://www.loop.com/PageIndex/Submit");
                    webRequest.AllowAutoRedirect = false;
                    webRequest.ContentLength = 0;
                    webRequest.Timeout = 5000;
                    response = (HttpWebResponse)webRequest.GetResponse();
                }
                catch (Exception ex)
                {
                    Console.WriteLine("ERREUR");
                    File.WriteAllText("C:\\log.txt", ex.Message);
                    exception = true;
                }
                if (exception || response.StatusCode != HttpStatusCode.OK)
                {
                    Console.WriteLine("ERREUR TRAITE");
                    if (ip == "192.168.37.110")
                        ip = "192.168.37.120";
                    else if (ip == "192.168.37.120")
                        ip = "192.168.37.130";
                    else if (ip == "192.168.37.130")
                        ip = "192.168.37.110";
                    dns.RemoveARecord("loop.com", "www");
                    dns.AddARecord("loop.com", "www", ip);
                    ExecuteCommand("ipconfig /flushdns");
                    Process.Start(Application.ExecutablePath);
                    Environment.Exit(0);
                }
                response.Close();
                response.Dispose();
                Thread.Sleep(1000);
            }
        }

        static void ExecuteCommand(string command)
        {
            try
            {
                var process = new Process();
                process.StartInfo.FileName = "cmd.exe ";
                process.StartInfo.Arguments = "/c " + command;
                process.StartInfo.UseShellExecute = false;
                process.StartInfo.RedirectStandardOutput = true;
                process.Start();
                Thread.Sleep(1000);
                process.Close();
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message);
            }
        }
    }
}
