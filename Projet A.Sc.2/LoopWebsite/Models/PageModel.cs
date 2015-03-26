using System.Collections.Generic;

namespace LoopWebsite.Models
{
	public class PageModel
	{
		public PageModel()
		{
			NumberOfPageToDisplay = 10;
			ActualPageIndex = 0;
		}

		public int Id { get; set; }
		public List<Page> Pages { get; set; }
		public string Query { get; set; }
		public int NumberOfPageToDisplay { get; set; }
		public int ActualPageIndex { get; set; }
	}
}