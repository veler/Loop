using System.Collections.Generic;

namespace LoopWebsite.Models
{
	public class Page
	{
		public string Url { get; set; }
		public string Title { get; set; }
		public string Description { get; set; }
		public int ViewCount { get; set; }
		public int FavoriteCount { get; set; }
		public int ShareCount { get; set; }
		public int LikeCount { get; set; }
		public List<object> Photos { get; set; }
	}
}