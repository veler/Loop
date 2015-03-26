using System;
using System.Management;

namespace Cluster_DNS_Robot
{
	class Dns
	{
		#region Fields

		private readonly string _server;
	    private readonly ManagementScope _session;

	    #endregion

		#region Constructor

		public Dns(string serverName, string userName, string password)
		{
		    _server = serverName;
		    var mNameSpace = "\\\\" + _server + "\\root\\microsoftdns";
			var con = new ConnectionOptions();
			con.Username = userName;
			con.Password = password;
			con.Impersonation = ImpersonationLevel.Impersonate;
			_session = new ManagementScope(mNameSpace);
			_session.Options = con;
			_session.Connect();
		}

		#endregion

		#region Methods

		public void AddARecord(string domain, string recordName, string ipDestination)
		{
			if (DomainExists(recordName + "." + domain))
				throw new Exception("Cet enregistrement existe déjà!");

			var man = new ManagementClass(_session, new ManagementPath("MicrosoftDNS_ATYPE"), null);
			var vars = man.GetMethodParameters("CreateInstanceFromPropertyData");
			vars["DnsServerName"] = _server;
			vars["ContainerName"] = domain;
			if (recordName == null)
				vars["OwnerName"] = domain;
			else
				vars["OwnerName"] = recordName + "." + domain;
			vars["IPAddress"] = ipDestination;
			man.InvokeMethod("CreateInstanceFromPropertyData", vars, null);
		}

		public void RemoveARecord(string domain, string aRecord)
		{
			var wql = "SELECT * ";
			wql += "FROM MicrosoftDNS_ATYPE ";
			wql += "WHERE OwnerName = '" + aRecord + "." + domain + "'";
			var q = new ObjectQuery(wql);
			var s = new ManagementObjectSearcher(_session, q);
			var col = s.Get();
			foreach (var managementBaseObject in col)
			{
				var o = (ManagementObject)managementBaseObject;
				o.Delete();
			}
		}

		#endregion

		#region Functions

		public bool DomainExists(string domainName)
		{
			var retval = false;
			var wql = "SELECT * ";
			wql += "FROM MicrosoftDNS_ATYPE ";
			wql += "WHERE OwnerName = '" + domainName + "'";
			var q = new ObjectQuery(wql);
			var s = new ManagementObjectSearcher(_session, q);
			var col = s.Get();
			foreach (var o in col)
				retval = true;
			return retval;
		}

		public ManagementObjectCollection GetARecord(string domain, string aRecord)
		{
			var wql = "SELECT * ";
			wql += "FROM MicrosoftDNS_ATYPE ";
			wql += "WHERE OwnerName = '" + aRecord + "." + domain + "'";
			var q = new ObjectQuery(wql);
			var s = new ManagementObjectSearcher(_session, q);
			return s.Get();
		}

		#endregion
	}
}
